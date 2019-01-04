#include "mainwindow.h"
#include <QApplication>
#include <QRect>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font;
    font.setPixelSize(20);
    font.setFamily("://assert/simsun.ttc");
    QApplication::setFont(font);

    MainWindow w;
    w.setObjectName("mainWindow");
    w.setStyleSheet("MainWindow#mainWindow{border-image:url(:/assert/windows.png);}");
    w.setAutoFillBackground(true);

    w.show();

    return a.exec();
}
