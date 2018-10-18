#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{
}


void OpenGLWidget::initializeGL()
{
    //调用内容初始化函数
    initializeOpenGLFunctions();

    //三角形顶点坐标
    GLfloat vertex[] = {
        -0.5f, 0.0f,
        0.5f, 0.0f,
        0.0f, 0.5f,
    };

    //三角形顶点索引
    GLuint triIndexs[] = {0, 1, 2};

    //三角形顶点颜色
    GLfloat colors[] = {1.0f, 0.0f, 0.0f, 1.0f,
                        0.0f, 1.0f, 0.0f, 1.0f,
                        0.0f, 0.0f, 1.0f, 1.0f,
                       };

    program = new QOpenGLShaderProgram(this);

    if(!program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader.vert"))
    {
        return;
    }

    if(!program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader.frag"))
    {
        return;
    }

    if(!program->link())
    {
        return;
    }

    if(!program->bind())
    {
        return;
    }

    // 获取shaderprogram的id号，然后可以通过id号获取一些属性...
    programId = program->programId();

    // 从shaderprogram里面获取变量标识
    matrixLocation = glGetUniformLocation(programId, "proMatrix");
    vertexLocation = glGetAttribLocation(programId, "vertexPos");
    clorLocation = glGetAttribLocation(programId, "textureColor");

    // 计算获得数组长度
    vVerticesLen = sizeof(vertex)/sizeof(GLfloat);
    triIndexLen = sizeof(triIndexs)/sizeof(GLuint);
    colorsLen = sizeof(colors)/sizeof(GLfloat);

    // 初始化顶点buffer并装载数据到显存
    glGenBuffers(1, &verVbo);
    glBindBuffer(GL_ARRAY_BUFFER, verVbo);
    glBufferData(GL_ARRAY_BUFFER, vVerticesLen * sizeof(GLfloat), vertex, GL_STATIC_DRAW);

    // 初始化索引buffer并装载数据到显存
    glGenBuffers(1, &indexVbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triIndexLen * sizeof(GLuint), triIndexs, GL_STATIC_DRAW);

    // 初始化颜色buffer并装载数据到显存
    glGenBuffers(1, &colorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glBufferData(GL_ARRAY_BUFFER, colorsLen * sizeof(GLfloat), colors, GL_STATIC_DRAW);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // shader传入模型视图矩阵 projection可以理解为建立了一个坐标系空间，可以再这个空间内设置图形
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, projection.data());

    // shader绑定并启用颜色数组buffer
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);
    glVertexAttribPointer(clorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(clorLocation);
    // 颜色值rgba，所以每四个float值作为一个颜色值，如果只是希望rgb，取三个值作为颜色值即可!


    // shader绑定并启用顶点数组buffer
    glBindBuffer(GL_ARRAY_BUFFER, verVbo);
    glVertexAttribPointer( vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLocation);

    // shader绑定并顶点索引数组buffer - 索引无需启用
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexVbo);
    glDrawElements(GL_TRIANGLES, triIndexLen, GL_UNSIGNED_INT,0);

    // 解绑buffer、关闭启用顶点、颜色数组
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(vertexLocation);
    glDisableVertexAttribArray(clorLocation);
}
