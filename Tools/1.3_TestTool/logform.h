#ifndef LOGFORM_H
#define LOGFORM_H

#include <QWidget>

namespace Ui {
class LogForm;
}

class LogForm : public QWidget
{
    Q_OBJECT

public:
    explicit LogForm(QWidget *parent = 0);
    ~LogForm();

    static void logI(QString tag, QString log);
    static void logD(QString tag, QString log);
    static void logE(QString tag, QString log);
    static void logW(QString tag, QString log);

public slots:
    void pushButton_logClear_onClicked();
    void pushButton_logSave_onClicked();

private:
    Ui::LogForm *ui;
};

#endif // LOGFORM_H
