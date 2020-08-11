#include <QApplication>
#include <QDebug>
#include <QString>
#include <QPixmap>
#include <QScreen>
#include <QGuiApplication>
#include <QApplication>
#include <QDesktopWidget>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDir>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int ret;

    qDebug() << "screen cap start";
    struct screenshooter *shooter;
    qDebug() << weston_config_get_libexec_dir();


    if (!shooter->client)
        shooter->client = weston_client_launch(shooter->ec,
                    &shooter->process,
                    "/usr/libexec/weston-screenshooter", screenshooter_sigchld);

    qDebug() << "screen cap end";
    return 0;
}
