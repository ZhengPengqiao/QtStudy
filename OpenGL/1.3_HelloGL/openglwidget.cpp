#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
    vCount = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;
}

OpenGLWidget::~OpenGLWidget()
{
    delete program;
}


void OpenGLWidget::setXRotation(int val)
{
    if (val != zRot)
    {
        xRot = val;
        update(this->x(), this->y(), this->width(), this->height());
    }
}

void OpenGLWidget::setYRotation(int val)
{
    if (val != zRot)
    {
        yRot = val;
        update(this->x(), this->y(), this->width(), this->height());
    }
}

void OpenGLWidget::setZRotation(int val)
{
    if (val != zRot)
    {
        zRot = val;
        update(this->x(), this->y(), this->width(), this->height());
    }
}


void OpenGLWidget::timerEvent(QTimerEvent *)
{
    // Request an update
    update(this->x(), this->y(), this->width(), this->height());
}


void OpenGLWidget::initializeGL()
{
    //调用内容初始化函数
    initializeOpenGLFunctions();

    //三角形顶点坐标
    // -0.5f, 0.5f, 0.0f,  0.5f, 0.5f, 0.0f,
    // -0.5f,-0.5f, 0.0f,  0.5f,-0.5f, 0.0f,
    //          0.0f,0.0f,1.0f,
    GLfloat vertex[] = {
        //1
         0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        //2
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        //3
         0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        //4
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.0f,  0.0f, 1.0f,
        //5
         0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        //6
         0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,

    };



    vCount = sizeof(vertex)/(sizeof(GLfloat)*2);

    //三角形顶点颜色
    GLfloat colors[] = {
                        0.0f, 0.0f, 1.0f, 1.0f,
                        0.0f, 1.0f, 0.0f, 1.0f,
                        1.0f, 0.0f, 0.0f, 1.0f,

                        0.0f, 0.0f, 1.0f, 1.0f,
                        0.0f, 1.0f, 1.0f, 1.0f,
                        0.0f, 1.0f, 0.0f, 1.0f,

                        0.5f, 0.0f, 1.0f, 1.0f,
                        0.0f, 1.0f, 1.0f, 1.0f,
                        1.0f, 1.0f, 1.0f, 1.0f,

                        0.0f, 0.5f, 1.0f, 1.0f,
                        0.0f, 1.0f, 1.0f, 1.0f,
                        1.0f, 1.0f, 1.0f, 1.0f,

                        1.0f, 0.0f, 1.0f, 1.0f,
                        0.0f, 1.0f, 1.0f, 1.0f,
                        1.0f, 1.0f, 1.0f, 1.0f,

                        0.5f, 0.5f, 1.0f, 1.0f,
                        0.0f, 1.0f, 1.0f, 1.0f,
                        1.0f, 1.0f, 1.0f, 1.0f,
                       };
    // 应用程序相关初始化
    {
        program = new QOpenGLShaderProgram(this);

        if(!program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader.vert"))
            { return; }
        if(!program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.frag"))
            { return; }
        if(!program->link())
            { return; }
        if(!program->bind())
            { return; }

        // 初始化顶点buffer并装载数据到显存
        glGenBuffers(1, &verVbo);
        glBindBuffer(GL_ARRAY_BUFFER, verVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

        // 初始化颜色buffer并装载数据到显存
        glGenBuffers(1, &colorVbo);
        glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    }
    timer.start(33, this);
}


void OpenGLWidget::resizeGL(int w, int h)
{
    //当窗口大小改变时，调整界面坐标显示高度和宽度
    glViewport(0, 0, w, h);
    // 模型矩阵重置
    projection.setToIdentity();
    // 透视投影
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    projection.perspective(60.0f, aspect, 1.0f, 100.0f);
    // 增加了模型矩阵，需要做一定偏移量，保证物体刚开始渲染出来时可以被看到！
    projection.translate(0.0f, 0.0f, -2.0f);
}


void OpenGLWidget::paintGL()
{
    qDebug() << " xRot:" << xRot << " yRot:" << yRot << " zRot:" << zRot;
    program->bind();
    {
        // Enable depth buffer, Enable back face culling
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);


        QMatrix4x4 matrix;
        matrix.rotate(xRot, 1.0f, 0.0f, 0.0f);
        matrix.rotate(yRot, 0.0f, 1.0f, 0.0f);
        matrix.rotate(zRot, 0.0f, 0.0f, 1.0f);

        // shader传入模型视图矩阵
        program->setUniformValue("proMatrix", projection * matrix);
        // shader绑定并启用颜色数组buffer
        GLuint clorLocation = program->attributeLocation("textureColor");
        glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
        program->enableAttributeArray(clorLocation);
        program->setAttributeBuffer(clorLocation, GL_FLOAT, 0, 4, 0);
        // shader绑定并启用顶点数组buffer
        GLuint vertexLocation = program->attributeLocation("vertexPos");
        glBindBuffer(GL_ARRAY_BUFFER, verVbo);
        program->enableAttributeArray(vertexLocation);
        program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, 0);

        glDrawArrays(GL_TRIANGLES, 0, vCount);
    }
    program->release();
}
