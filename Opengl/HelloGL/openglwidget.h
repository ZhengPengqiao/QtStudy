#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWidget(QWidget *parent = 0);

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

private:
    QOpenGLShaderProgram *program;
    GLuint programId;
    GLuint matrixLocation;
    GLuint vertexLocation;
    GLuint clorLocation;
    // 计算获得数组长度
    GLfloat vVerticesLen;
    GLuint triIndexLen;
    GLfloat colorsLen;
    GLuint verVbo;
    GLuint indexVbo;
    GLuint colorVbo;

    QMatrix4x4 projection;

signals:

public slots:

};

#endif // OPENGLWIDGET_H
