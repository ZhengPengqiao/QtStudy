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
    inst = this;
}


LogForm::~LogForm()
{
    delete ui;
    if( inst != this && inst != nullptr )
    {
        delete inst;
    }
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


void LogForm::logI(QString tag, QString log)
{
    if( inst == nullptr )
    {
        qDebug()<< "inst: Log Form is not init";
    }
    inst->ui->textEdit_log->append(tag + " [I]:"+log);
}


void LogForm::logD(QString tag, QString log)
{
    if( inst == nullptr )
    {
        qDebug()<< "inst: Log Form is not init";
    }
    inst->ui->textEdit_log->append(tag + " [D]:"+log);
}


void LogForm::logE(QString tag, QString log)
{
    if( inst == nullptr )
    {
        qDebug()<< "inst: Log Form is not init";
    }
    inst->ui->textEdit_log->append(tag + " [E]:"+log);
}


void LogForm::logW(QString tag, QString log)
{
    if( inst == nullptr )
    {
        qDebug()<< "inst: Log Form is not init";
    }
    inst->ui->textEdit_log->append(tag + " [W]:"+log);
}
