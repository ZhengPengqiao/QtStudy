#ifndef CUPDATER_H
#define CUPDATER_H

#include <QObject>
#include <QRunnable>

#define UPDATE_MAGIC    0x474b5054

class QTimer;
class CUpdater : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit CUpdater(QObject *parent = nullptr);
    ~CUpdater();

    bool CheckPackage(QString file_name);
    const QString &GetVersion();

    virtual void run();

signals:
    void progress(int);
    void updateDone();
    void updateFail();

    void startTimer();
    void stopTimer();

public slots:
    void OnTimerout();

private:
    QString m_szVer;
    QString m_szPkg;
    int m_nOffset;

    QTimer *m_pTimer;
    int m_nProgress;
};


#endif // CUPDATER_H
