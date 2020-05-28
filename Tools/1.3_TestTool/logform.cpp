#include "logform.h"
#include "ui_logform.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>

static LogForm *inst = nullptr;

LogForm::LogForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogForm)
{
    ui->setupUi(this);
}

LogForm::~LogForm()
{
    delete ui;
}


void LogForm::pushButton_logClear_onClicked()
{
    ui->textEdit_log->clear();
    QMessageBox::information(this, "Clear LogForm", "Clear LogForm Ok!");
}

void LogForm::pushButton_logSave_onClicked()
{
    //将文本框数据取出并按行排列
    QFile file("LogForm.log");
    //文件命名
    if (!file.open(QFile::WriteOnly | QFile::Text)) //检测文件是否打开
    {
        QMessageBox::information(this, "Error Message", "File Path Err!");
        return;
    }
    QTextStream out(&file);
    //分行写入文件
    out << ui->textEdit_log->toPlainText();
    file.close();

    QMessageBox::information(this, "Save LogForm", "Save LogForm.log Ok!");
}


LogForm *LogForm::instances()
{
    if( inst == nullptr )
    {
        qDebug() << "inst is null, and new LogForm()";
        inst = new LogForm();
    }
    return inst;
}


void LogForm::logI(QString log)
{
    if( inst == nullptr )
    {
        instances();
    }
    inst->ui->textEdit_log->append("[I]:"+log);
}


void LogForm::logD(QString log)
{
    if( inst == nullptr )
    {
        instances();
    }
    inst->ui->textEdit_log->append("[D]:"+log);
}


void LogForm::logE(QString log)
{
    if( inst == nullptr )
    {
        instances();
    }
    inst->ui->textEdit_log->append("[E]:"+log);
}


void LogForm::logW(QString log)
{
    if( inst == nullptr )
    {
        instances();
    }
    inst->ui->textEdit_log->append("[W]:"+log);
}
