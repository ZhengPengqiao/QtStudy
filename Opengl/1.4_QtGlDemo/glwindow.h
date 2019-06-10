#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWindow>
#include <QMatrix4x4>
#include <QVector3D>

QT_BEGIN_NAMESPACE

class QOpenGLTexture;
class QOpenGLShaderProgram;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;

QT_END_NAMESPACE

class GLWindow : public QOpenGLWindow
{
    Q_OBJECT
    Q_PROPERTY(float z READ z WRITE setZ)
    Q_PROPERTY(float r READ r WRITE setR)
    Q_PROPERTY(float r2 READ r2 WRITE setR2)

public:
    GLWindow();
    ~GLWindow();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    float z() const { return m_eye.z(); }
    void setZ(float v);

    float r() const { return m_r; }
    void setR(float v);
    float r2() const { return m_r2; }
    void setR2(float v);
private slots:
    void startSecondStage();
private:
    QOpenGLTexture *m_texture;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vbo;
    QOpenGLBuffer *m_vbo2;
    QOpenGLVertexArrayObject *m_vao;
    int m_projMatrixLoc;
    int m_camMatrixLoc;
    QMatrix4x4 m_proj;
    QVector3D m_eye;
    QVector3D m_target;
    float m_r;
    float m_r2;
    int vCount;
};

#endif
