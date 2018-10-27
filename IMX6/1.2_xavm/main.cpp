#include "cbackgroundwnd.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QFont>
#include <QFontDatabase>
#include <QTextCodec>
#include <QDebug>

#include "clayersetup.h"

int main(int argc, char *argv[])
{
    int ret;

    CLayerSetup *layer = CLayerSetup::getInstance();

    QApplication a(argc, argv);
    int sw = QApplication::desktop()->width();
    int sh = QApplication::desktop()->height();
    qDebug() << " Screen : " << sw << " x " << sh;
    qDebug() << " Time   : " << __DATE__ << " - " << __TIME__;
    layer->show();

#if 0
    QFont font;
    font.setPixelSize(20);
    font.setFamily("SimSun");
    QApplication::setFont(font);
#else
#ifdef Q_OS_WIN32
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
#else
    int nIndex = QFontDatabase::addApplicationFont("/home/tsh/res/simsun.ttc");
    if (nIndex != -1) {
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if (strList.count() > 0) {
            QFont fontThis(strList.at(0));
            fontThis.setPixelSize(20);
            a.setFont(fontThis);
        }
    }
#endif
#endif

    CBackgroundWnd w;
    w.show();

    ret = a.exec();
    delete layer;
    return ret;
}
