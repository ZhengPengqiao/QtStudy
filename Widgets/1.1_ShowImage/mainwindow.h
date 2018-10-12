#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QScrollBar>
#include <QLabel>
#include <QScrollArea>
#include <QtWidgets>
class MainWindow : public QMainWindow
{
public:
    MainWindow();
    bool loadFile(const QString &);

private slots:
    void open();

private:
    void setImage(const QImage &newImage);

    QImage image;
    QLabel *imageLabel;
    QScrollArea *scrollArea;

};
//! [0]

#endif
