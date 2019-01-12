#include "mainwindow.h"
#include <QApplication>
#include <QRect>
#include <QTextCodec>
#include <QFontDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#if 1
    int fontId = QFontDatabase::addApplicationFont(":/assert/simsun.ttc");
    //将字体Id传给applicationFontFamilies,得到一个QStringList,其中的第一个元素为新添加字体的family
    QString msyh = QFontDatabase::applicationFontFamilies ( fontId ).at(0);
    QFont font;
    font.setPixelSize(20);
    font.setFamily(msyh);
    //将此字体设为QApplication的默认字体
    QApplication::setFont(font);
#else
    QFont font;
    font.setPixelSize(20);
    font.setFamily("SimSun");
    QApplication::setFont(font);
#endif
    //打印字体信息
    qDebug() << a.font().rawName();
    qDebug() << a.font().family();
    qDebug() << a.font().defaultFamily();
    qDebug() << a.font().styleName();
    qDebug() << a.font().toString();
    qDebug() << a.font().key();

    MainWindow w;
    w.setObjectName("mainWindow");
    w.setStyleSheet("MainWindow#mainWindow{border-image:url(:/assert/windows.png);}");
    w.setAutoFillBackground(true);

    w.show();

    return a.exec();
}
