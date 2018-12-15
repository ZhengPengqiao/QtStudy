#include "mainwindow.h"
#include <QApplication>
#include <QFont>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font;
    font.setPixelSize(20);
    font.setFamily("simsun");
    QApplication::setFont(font);

    MainWindow w;
    w.show();

    return a.exec();
}
