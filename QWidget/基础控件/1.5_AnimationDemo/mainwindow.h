#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include <QPropertyAnimation>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *ev);

private:
    QPushButton *button;
    QParallelAnimationGroup *m_group;
    QPropertyAnimation *pAnimation1;
    QPropertyAnimation *pAnimation2;
};

#endif // MAINWINDOW_H
