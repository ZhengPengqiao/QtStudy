#ifndef SPR2801S_H
#define SPR2801S_H


#include <iostream>
#include <chrono>
#include <cmath>
#include "json.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/opencv_modules.hpp>
#include <QString>
#include <QDebug>
#include <GTILib.h>
#include <GtiLog.h>

class SPR2801S
{

public:

#define IMAGE_SIZE 448 // this is the maximum image size
#define IMAGE_DEPTH 3
#define WINDOW_SIZE 800
    int image_size = 224;  // this is the actual image size
    GtiModel * model;
    char *gBuffer[IMAGE_SIZE * IMAGE_SIZE * IMAGE_DEPTH];
    bool showFPS = 0;
    float inferenceTime = 0, fps = 0;

public:
    SPR2801S();
    ~SPR2801S();
    int initModel(const char *modelName);
    int main_showImage(QString imagePath);
    int main_cameraShow(QString cameraPath);
    int main_playVideo(QString videoPath);
    int main_slideShow(QString folderPath);
    void showImage(const char *filename, const int imgsize=244);
    void cameraShow(const char *cameraIndex, const int imgsize=244);
    void playVideo(const char *filename, const int imgsize=244);
    void slideShow(const char *filename, const int imgsize=244);
    void processImage(cv::Mat &img, GtiModel *model, const char *title, const int imgsize);
    void setLabel(cv::Mat &im, QString &label, const cv::Point &orig, const double scale);
};


#endif // SPR2801S_H
