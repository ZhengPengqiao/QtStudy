#include "mainwidget.h"
#include <QMouseEvent>
#include <math.h>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    texture1(0),
    texture2(0),
    textureFlag(true),
    angle(0)
{
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    arrayBuf.destroy();
    delete texture1;
    delete texture2;
    doneCurrent();
}


void MainWidget::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    textureFlag = !textureFlag;
    qDebug() << "textureFlag:" << textureFlag;
}


void MainWidget::timerEvent(QTimerEvent *)
{
    angle += 0.3f;
    // Request an update
    update(this->x(), this->y(), this->width(), this->height());
}


void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

    // Enable depth buffer, Enable back face culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Generate 1 VBOs
    arrayBuf.create();

    // V3 V2
    // V0 V1
    VertexData vertices[] = {
        // Vertex data for face 0
        { QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)}, // v3
        { QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)}, // v0
        { QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)}, // v1

        { QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)}, // v1
        { QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(1.0f, 1.0f)}, // v2
        { QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)}, // v3
    };


    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, sizeof(vertices));

    // Use QBasicTimer because its faster than QTimer
    timer.start(33, this);
}


void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}


void MainWidget::initTextures()
{
    // Load cube.png image
    texture1 = new QOpenGLTexture(QImage(":/cube.png").mirrored());
    texture1->setMinificationFilter(QOpenGLTexture::Nearest);
    texture1->setMagnificationFilter(QOpenGLTexture::Linear);
    texture1->setWrapMode(QOpenGLTexture::Repeat);
    // Load cube2.jpg image
    texture2 = new QOpenGLTexture(QImage(":/cube2.jpg").mirrored());
    texture2->setMinificationFilter(QOpenGLTexture::Nearest);
    texture2->setMagnificationFilter(QOpenGLTexture::Linear);
    texture2->setWrapMode(QOpenGLTexture::Repeat);
}


void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}


void MainWidget::paintGL()
{
    program.bind();
    {
        qDebug() << "paintGL()" << " angle:" << angle;
        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if( textureFlag )
        {
            texture1->bind();
        }
        else
        {
            texture2->bind();
        }

        if( arrayBuf.bind() == false )
        {
            qDebug() << "arrayBuf.bind() Err";
        }

        // Calculate model view transformation
        QMatrix4x4 matrix;
        matrix.translate(0.0, 0.0, -5.0);
        matrix.rotate(angle, 0.0f, 0.0f, 1.0f);

        // Set modelview-projection matrix
        program.setUniformValue("mvp_matrix", projection * matrix);
        // Use texture unit 0 which contains cube.png
        program.setUniformValue("texture", 0);

        // Tell OpenGL programmable pipeline how to locate vertex position data
        int vertexLocation = program.attributeLocation("a_position");
        program.enableAttributeArray(vertexLocation);
        program.setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(VertexData));

        // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
        int texcoordLocation = program.attributeLocation("a_texcoord");
        program.enableAttributeArray(texcoordLocation);
        program.setAttributeBuffer(texcoordLocation, GL_FLOAT, sizeof(QVector3D), 2, sizeof(VertexData));

        // Draw cube geometry using indices from VBO 1
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    program.release();
}
