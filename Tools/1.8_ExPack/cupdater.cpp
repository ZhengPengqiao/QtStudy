#include "cupdater.h"
#include <QFile>
#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QString>
#include <QProcess>
#include <QFileInfo>
#include <QStringList>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

CUpdater::CUpdater(QObject *parent) : QObject(parent)
{
    setAutoDelete(false);

    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(50);
    connect(this, SIGNAL(startTimer()), m_pTimer, SLOT(start()));
    connect(this, SIGNAL(stopTimer()), m_pTimer, SLOT(stop()));
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(OnTimerout()));
}

CUpdater::~CUpdater()
{
    m_pTimer->stop();
    delete m_pTimer;
}

bool CUpdater::CheckPackage(QString file_name)
{
    m_nOffset = 0;
    m_szPkg=file_name;
    if(!QFile::exists(m_szPkg))
    {

        qDebug() << "No Update package:" << m_szPkg;
        return false;
    }
    qDebug() << "Update package:" << m_szPkg;

    QFile fn(m_szPkg);
    if(fn.open(QIODevice::ReadOnly)) {
        char tmp[384];
        int rlen = fn.read(tmp, 384);
        int magic = ((int *)tmp)[0];
        if(rlen != 384 || magic != UPDATE_MAGIC) {
            fn.close();
            return false;
        }

        m_nOffset = tmp[267];
        m_nOffset += (tmp[268] << 8);
        m_szVer = tmp + 32;
        fn.close();
        qDebug() << "Package :" << m_szVer << m_nOffset;
    }

    return true;
}

const QString &CUpdater::GetVersion()
{
    return m_szVer;
}

void CUpdater::run()
{

    const QString cmdUdp("dd if=%1 bs=%2 skip=1 | tar joxv -C ./");
    QProcess p;

    QStringList args;

    // set timer interval with pkg file size
    QFileInfo fi(m_szPkg);
    qint64 fsz = fi.size() - m_nOffset;
    int tt = fsz / 1491 / 60 + 0.5f;
    if(tt < 16) tt = 16;
    m_pTimer->setInterval(tt);

    m_nProgress = 15;
    emit progress(m_nProgress);
    emit startTimer();
    QTime tc;
    tc.start();

    qDebug() << "Extract :" << m_szPkg;
    args.clear();
    args << "-c" << cmdUdp.arg(m_szPkg).arg(m_nOffset);
    p.start("/bin/sh", args);
    bool bret = p.waitForFinished(-1);
    qDebug() << "Extract time :" << tc.elapsed() << ", sz :" << fsz;
    if(!bret || p.exitCode()) {
        sync();
        emit updateFail();
        return;
    }
    emit stopTimer();


    emit progress(100);
    sync();
    emit updateDone();
}

void CUpdater::OnTimerout()
{
    if(m_nProgress < 75)
        m_nProgress += 1;
    emit progress(m_nProgress);
}

