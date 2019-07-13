#include "openglform.h"
#include "ui_openglform.h"

OpenGLForm::OpenGLForm(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::OpenGLWidget),
    angle(0),
    vCount(0)
{
    ui->setupUi(this);

    angle = ui->horizontalSlider->value()/10.0f;
    ui->label->setText(QString("速度:%1").arg(angle));
}


OpenGLForm::~OpenGLForm()
{

    delete ui;
    positionBuf.destroy();
    colorBuf.destroy();
    delete program;
}

void OpenGLForm::timerEvent(QTimerEvent *)
{
    // Request an update
    update(this->x(), this->y(), this->width(), this->height());
}


void OpenGLForm::initializeGL()
{
    //调用内容初始化函数
    initializeOpenGLFunctions();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    QSurfaceFormat surfaceFormat;
    surfaceFormat.setSamples(4);//多重采样
    setFormat(surfaceFormat);

    //三角形顶点坐标
    GLfloat vertex[] = {
        -0.5f,  0.5f,
        -0.5f, -0.5f,
         0.5f,  0.5f,

         0.5f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f,
    };

    vCount = sizeof(vertex)/(sizeof(GLfloat)*2);

    //三角形顶点颜色
    GLfloat colors[] = {1.0f, 0.0f, 0.0f, 1.0f,
                        0.0f, 1.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f, 1.0f,

                        0.0f, 0.0f, 1.0f, 1.0f,
                        0.0f, 1.0f, 1.0f, 1.0f,
                        0.0f, 1.0f, 0.0f, 1.0f,
                       };
    // 应用程序相关初始化
    {
        program = new QOpenGLShaderProgram(this);

        if(!program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/assert/shader.vert"))
            { return; }
        if(!program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/assert/shader.frag"))
            { return; }
        if(!program->link())
            { return; }
        if(!program->bind())
            { return; }

        positionBuf.create();
        positionBuf.bind();
        GLuint vertexLocation = program->attributeLocation("vertexPos");
        qDebug() << "vertexLocation : " << vertexLocation;
        //方法1
        //program->enableAttributeArray(vertexLocation);
        //program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 2, 0);
        //方法2
        f->glEnableVertexAttribArray(vertexLocation);
        f->glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

        positionBuf.allocate(vertex, sizeof(vertex));
        positionBuf.release();


        colorBuf.create();
        colorBuf.bind();
        // shader绑定并启用颜色数组buffer
        GLuint clorLocation = program->attributeLocation("textureColor");
        qDebug() << "clorLocation : " << clorLocation;
        //方法1
        //program->enableAttributeArray(clorLocation);
        //program->setAttributeBuffer(clorLocation, GL_FLOAT, 0, 4, 0);
        //方法2
        f->glEnableVertexAttribArray(clorLocation);
        f->glVertexAttribPointer(clorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
        colorBuf.allocate(colors, sizeof(colors));
        colorBuf.release();
    }
    timer.start(33, this);
}


void OpenGLForm::resizeGL(int w, int h)
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


void OpenGLForm::paintGL()
{
    program->bind();
    {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        matrix.rotate(angle, 0.0f, 0.0f, 1.0f);
        // shader传入模型视图矩阵
        program->setUniformValue("proMatrix", projection * matrix);

        glDrawArrays(GL_TRIANGLES, 0, vCount);

    }
    program->release();
}

void OpenGLForm::valueChange(int val)
{
    angle = val/10.0f;
    ui->label->setText("速度:"+QString::number(angle, 'f', 1) );
}
