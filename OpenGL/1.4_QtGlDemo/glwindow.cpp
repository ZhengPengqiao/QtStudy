#include "glwindow.h"
#include <QImage>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLContext>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLExtraFunctions>
#include <QPropertyAnimation>
#include <QPauseAnimation>
#include <QSequentialAnimationGroup>
#include <QTimer>

GLWindow::GLWindow()
    : m_texture(0),
      m_program(0),
      m_vbo(0),
      m_vbo2(0),
      m_vao(0),
      m_target(0, 0, -1),
      m_r(0),
      m_r2(0)
{
    QSequentialAnimationGroup *animGroup = new QSequentialAnimationGroup(this);
    animGroup->setLoopCount(-1);
    QPropertyAnimation *zAnim0 = new QPropertyAnimation(this, QByteArrayLiteral("z"));
    zAnim0->setStartValue(1.5f);
    zAnim0->setEndValue(10.0f);
    zAnim0->setDuration(2000);
    animGroup->addAnimation(zAnim0);
    QPropertyAnimation *zAnim1 = new QPropertyAnimation(this, QByteArrayLiteral("z"));
    zAnim1->setStartValue(10.0f);
    zAnim1->setEndValue(50.0f);
    zAnim1->setDuration(4000);
    zAnim1->setEasingCurve(QEasingCurve::OutElastic);
    animGroup->addAnimation(zAnim1);
    QPropertyAnimation *zAnim2 = new QPropertyAnimation(this, QByteArrayLiteral("z"));
    zAnim2->setStartValue(50.0f);
    zAnim2->setEndValue(1.5f);
    zAnim2->setDuration(2000);
    animGroup->addAnimation(zAnim2);
    animGroup->start();

    QPropertyAnimation* rAnim = new QPropertyAnimation(this, QByteArrayLiteral("r"));
    rAnim->setStartValue(0.0f);
    rAnim->setEndValue(360.0f);
    rAnim->setDuration(2000);
    rAnim->setLoopCount(-1);
    rAnim->start();

    QTimer::singleShot(4000, this, &GLWindow::startSecondStage);
}

GLWindow::~GLWindow()
{
    makeCurrent();
    delete m_texture;
    delete m_program;
    delete m_vbo;
    delete m_vbo2;
    delete m_vao;
}

void GLWindow::startSecondStage()
{
    QPropertyAnimation* r2Anim = new QPropertyAnimation(this, QByteArrayLiteral("r2"));
    r2Anim->setStartValue(0.0f);
    r2Anim->setEndValue(360.0f);
    r2Anim->setDuration(20000);
    r2Anim->setLoopCount(-1);
    r2Anim->start();
}

void GLWindow::setZ(float v)
{
    m_eye.setZ(v);
    update();
}

void GLWindow::setR(float v)
{
    m_r = v;
    update();
}

void GLWindow::setR2(float v)
{
    m_r2 = v;
    update();
}

void GLWindow::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

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

    if (m_texture) {
        delete m_texture;
        m_texture = 0;
    }
    QImage img(":/qtlogo.png");
    Q_ASSERT(!img.isNull());
    m_texture = new QOpenGLTexture(img.scaled(32, 36).mirrored());

    if (m_program) {
        delete m_program;
        m_program = 0;
    }
    m_program = new QOpenGLShaderProgram;
    // Prepend the correct version directive to the sources. The rest is the
    // same, thanks to the common GLSL syntax.
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shader.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.fsh");
    m_program->link();

    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_camMatrixLoc = m_program->uniformLocation("camMatrix");

    // Create a VAO. Not strictly required for ES 3, but it is for plain OpenGL.
    if (m_vao) {
        delete m_vao;
        m_vao = 0;
    }
    m_vao = new QOpenGLVertexArrayObject;
    if (m_vao->create())
        m_vao->bind();

    if (m_vbo) {
        delete m_vbo;
        m_vbo = 0;
    }

    if (m_vbo2) {
        delete m_vbo2;
        m_vbo2 = 0;
    }


    m_program->bind();
    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();
    m_vbo->allocate(vertex, sizeof(vertex));
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    m_vbo->release();

    m_vbo2 = new QOpenGLBuffer;
    m_vbo2->create();
    m_vbo2->bind();
    m_vbo2->allocate(colors, sizeof(colors));
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    m_vbo2->release();

    f->glEnable(GL_DEPTH_TEST);
    f->glEnable(GL_CULL_FACE);
}

void GLWindow::resizeGL(int w, int h)
{
    m_proj.setToIdentity();
    m_proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void GLWindow::paintGL()
{
    // Now use QOpenGLExtraFunctions instead of QOpenGLFunctions as we want to
    // do more than what GL(ES) 2.0 offers.
    QOpenGLExtraFunctions *f = QOpenGLContext::currentContext()->extraFunctions();

    f->glClearColor(0, 0, 0, 1);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    m_texture->bind();

    QMatrix4x4 camera;
    camera.lookAt(m_eye, m_eye + m_target, QVector3D(0, 1, 0));
    m_program->setUniformValue(m_projMatrixLoc, m_proj*camera);

    // Now call a function introduced in OpenGL 3.1 / OpenGL ES 3.0. We
    // requested a 3.3 or ES 3.0 context, so we know this will work.
    f->glDrawArraysInstanced(GL_TRIANGLES, 0, vCount, 32 * 36);
}
