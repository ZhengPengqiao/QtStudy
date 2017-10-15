#include "radardiagram.h"
#include <QApplication>
#include <mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow m;
    m.setGeometry(100,100,500,500);
    m.show();

    return a.exec();
}
