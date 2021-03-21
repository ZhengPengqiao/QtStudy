#include <iostream>
#include <chrono>
#include <cmath>
#include <GTILib.h>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include "spr2801s.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;


SPR2801S::SPR2801S()
{

}


SPR2801S::~SPR2801S()
{

}


void SPR2801S::setLabel(cv::Mat &im, QString &label, const cv::Point &orig, const double scale)
{
    int fontface = cv::FONT_HERSHEY_COMPLEX_SMALL;
    int thickness = scale;
    int baseline = 0;
    char secondLine[50];
    json labelJson = json::parse(label.toStdString().c_str());
    string fullLabel = labelJson["result"][0]["label"];

    string displayText = fullLabel.substr(fullLabel.find_first_of(" ") + 1);

    cv::Size text = cv::getTextSize(displayText, fontface,
            scale, thickness, &baseline);
    rectangle(im, orig + cv::Point(0, baseline), orig + cv::Point(text.width, -text.height),
            CV_RGB(50, 60, 70), CV_FILLED);
    putText(im, displayText, orig, fontface, scale,
            CV_RGB(255, 255, 0), thickness, 8);

    sprintf(secondLine, "Inference time=%3.2fms, FPS=%3.1f", inferenceTime, fps);
    displayText = string(secondLine);
    text = cv::getTextSize(displayText, fontface, scale, thickness, &baseline);
    rectangle(im, orig + cv::Point(0, baseline + text.height * 3 / 2),
              orig + cv::Point(text.width, text.height / 2), CV_RGB(50, 60, 70), CV_FILLED);
    putText(im, displayText, orig + cv::Point(0, text.height * 3 / 2),
            fontface, scale, CV_RGB(0, 255, 0), thickness, 8);
}

void SPR2801S::processImage(cv::Mat &img, GtiModel *model, const char *title, const int imgsize)
{
    static float inferenceTime_us;
    static int cnt = 0;
    cv::Mat img1;
    GtiTensor input;

    input.height = input.width = imgsize;
    input.depth = IMAGE_DEPTH;
    input.buffer = gBuffer;
    cv::resize(img, img1, cv::Size(input.height, input.width));
    std::vector<cv::Mat> splitBGR;
    split(img1, splitBGR);
    memcpy(input.buffer, splitBGR[0].data, imgsize * imgsize);
    memcpy((char *)input.buffer + imgsize * imgsize, splitBGR[1].data, imgsize * imgsize);
    memcpy((char *)input.buffer + imgsize * imgsize * 2, splitBGR[2].data, imgsize * imgsize);

    auto t0 = chrono::high_resolution_clock::now();
    GtiTensor *output = GtiEvaluate(model, &input);
    auto t1 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::duration<float>>(t1 - t0);
    inferenceTime_us += 1000000 * duration.count() + numeric_limits<float>::epsilon();
    cnt++;
    if (showFPS)
    {
        inferenceTime = (inferenceTime_us / 1000 / cnt);
        fps = (1000000 * cnt) / inferenceTime_us;
        cout << endl
             << "Image inference time = " << inferenceTime << "ms,  FPS = " << fps << endl
             << endl;
        inferenceTime_us = 0;
        cnt = 0;
        showFPS = 0;
    }

    if( output != nullptr )
    {
        QString label = QString((char *)output->buffer);
        double scale = 1;
        resize(img, img, cv::Size(WINDOW_SIZE, WINDOW_SIZE * img.rows / img.cols));
        setLabel(img, label, cv::Point(20, 20), scale);
    }
    else
    {
        qDebug() << "processImage output == nullptr";
    }
    imshow(title, img);
}


/**
 * @brief showImage
 * 识别图片
 * @param filename : 需要识别的图片
 * @param imgsize : 需要识别图片的尺寸
 */
void SPR2801S::showImage(const char *filename, const int imgsize)
{
    cv::Mat img;
    img = cv::imread(filename);
    if (img.empty())
    {
        cout << "count not open image file " << filename << endl;
        return;
    }
    auto title = "Image";
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::resizeWindow(title, WINDOW_SIZE, WINDOW_SIZE);
    showFPS = 1;
    processImage(img, model, title, imgsize);
    cout << endl;
}

/**
 * @brief slideShow
 * 识别文件夹中的图片
 * @param filename : 需要识别的文件夹
 * @param imgsize : 需要识别图片的尺寸
 */
void SPR2801S::slideShow(const char *filename, const int imgsize)
{
    vector<cv::String> fileArray;
    cv::String folder = filename;
    bool pause = false;
    glob(folder, fileArray);
    if (fileArray.size() == 0)
    {
        cout << "could not find " << filename << " as a media file/dir" << endl;
    }
    else
    {
        while (1)
        {
            for (auto &i : fileArray)
            {
                showImage(i.c_str(), imgsize);
                int key = cv::waitKey(1) & 0xff;
                if ((key == 'q') || (key == 27))
                    return;
                else if (key == ' ')
                    pause = !pause;
                while (pause == true)
                {
                    key = cv::waitKey(1) & 0xff;
                    if ((key == 'q') || (key == 27))
                        return;
                    else if (key == ' ')
                        pause = !pause;
                }
            }
        }
    }
}

/**
 * @brief playVideo
 * 识别视频文件
 * @param filename : 需要识别视频文件
 * @param imgsize : 需要识别视频图片的尺寸
 */
void SPR2801S::playVideo(const char *filename, const int imgsize)
{
    cv::VideoCapture cap;
    cv::Mat img;
    if (cap.open(filename) && (!cap.isOpened()))
    {
        cout << "count not open video file " << filename << endl;
        return;
    }
    int w = min(WINDOW_SIZE, (int)cap.get(CV_CAP_PROP_FRAME_WIDTH));
    int h = min(WINDOW_SIZE, (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    auto title = "Video";
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::resizeWindow(title, w, h);
    bool state = true;
    int cnt = 0;
    while (1)
    {
        if (state)
        {
            if (cap.read(img) == false)
            {
                cap.set(CV_CAP_PROP_POS_FRAMES, 0);
                continue;
            }
            if (cnt++ > 60)
            {
                showFPS = 1;
                cnt = 0;
            }
            processImage(img, model, title, imgsize);
        }
        int key = cv::waitKey(1) & 0xff;
        if (key == ' ')
            state = !state;
        else if ((key == 'q') || (key == 27))
            break;
    }
    cap.release();
}

/**
 * @brief cameraShow
 * 识别摄像头图片
 * @param filename : 需要识别的摄像头
 * @param imgsize : 需要识别摄像头图片的尺寸
 */
void SPR2801S::cameraShow(const char *cameraIndex, const int imgsize)
{
    cv::VideoCapture cap;
    cap.open(atoi(cameraIndex));
    if (!cap.isOpened())
    {
        cout << "count not open camera " << cameraIndex << endl;
        return;
    }
    int w = min(WINDOW_SIZE, (int)cap.get(CV_CAP_PROP_FRAME_WIDTH));
    int h = min(WINDOW_SIZE, (int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    const char *title = "camera show";
    cv::namedWindow(title, cv::WINDOW_NORMAL);
    cv::resizeWindow(title, w, h);

    cv::Mat img;
    while (1)
    {
        cap.grab();
        cap.retrieve(img);
        processImage(img, model, title, imgsize);
        int key = cv::waitKey(1) & 0xff;
        if ((key == 'q') || (key == 27))
            return;
    }
}

int SPR2801S::initModel(const char *modelName)
{
    model = GtiCreateModel(modelName);
    if (0 == model)
    {
        cout << "count not load Model from " << modelName << endl;
        return -1;
    }
    return 0;
}

int SPR2801S::main_showImage(QString imagePath)
{
    showImage(imagePath.toStdString().c_str(), image_size);
    return 0;
}


int SPR2801S::main_cameraShow(QString cameraPath)
{
    cameraShow(cameraPath.toStdString().c_str(), image_size);
    return 0;
}


int SPR2801S::main_playVideo(QString videoPath)
{
    playVideo(videoPath.toStdString().c_str(), image_size);
    return 0;
}


int SPR2801S::main_slideShow(QString folderPath)
{
    slideShow(folderPath.toStdString().c_str(), image_size);
    return 0;
}

