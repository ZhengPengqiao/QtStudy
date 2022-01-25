#include "mainwindow.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setGeometry(100,100,720,480);

    button = new QPushButton(this);
    button->setStyleSheet("background-color:rgb(0,0,0)");

    //基础动画1
    pAnimation1 = new QPropertyAnimation(button, "geometry");
    pAnimation1->setDuration(700);
    pAnimation1->setStartValue(QRect(0, this->height()/2, 50, 50));
    pAnimation1->setEndValue(QRect(this->width()-50, this->height()/2, 50, 50));
    pAnimation1->setEasingCurve(QEasingCurve::Linear);

    //基础动画2
    pAnimation2 = new QPropertyAnimation(button, "geometry");
    pAnimation2->setDuration(700);
    pAnimation2->setStartValue(QRect(this->width()-50, this->height()/2, 50, 50));
    pAnimation2->setEndValue(QRect(0, this->height()/2, 50, 50));
    pAnimation2->setEasingCurve(QEasingCurve::Linear);

    //串行动画分组
    QSequentialAnimationGroup *pGroup = new QSequentialAnimationGroup(this);
    pGroup->addAnimation(pAnimation1);
    pGroup->addAnimation(pAnimation2);

    //不透明度
    QGraphicsOpacityEffect *pButtonOpacity = new QGraphicsOpacityEffect(this);
    pButtonOpacity->setOpacity(1);
    button->setGraphicsEffect(pButtonOpacity);

    //并行动画组
    m_group = new QParallelAnimationGroup(this);
    m_group->addAnimation(pGroup);

    m_group->setDirection(QAbstractAnimation::Forward);
    m_group->setLoopCount(-1);
    m_group->start();
}

MainWindow::~MainWindow()
{
    delete button;
}

/**
 * 窗口大小更改时,重新设置动画的宽度
 * @brief MainWindow::resizeEvent
 * @param ev
 */
void MainWindow::resizeEvent(QResizeEvent *ev)
{
    (void)ev;
    pAnimation1->setStartValue(QRect(0, this->height()/2, 50, 50));
    pAnimation1->setEndValue(QRect(this->width()-50, this->height()/2, 50, 50));

    pAnimation2->setStartValue(QRect(this->width()-50, this->height()/2, 50, 50));
    pAnimation2->setEndValue(QRect(0, this->height()/2, 50, 50));
}
