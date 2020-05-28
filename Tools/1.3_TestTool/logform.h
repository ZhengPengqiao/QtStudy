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

    static LogForm *instances();

    static void logI(QString log);
    static void logD(QString log);
    static void logE(QString log);
    static void logW(QString log);

public slots:
    void pushButton_logClear_onClicked();
    void pushButton_logSave_onClicked();

private:
    Ui::LogForm *ui;
};

#endif // LOGFORM_H
