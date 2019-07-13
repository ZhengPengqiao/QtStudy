#ifndef SERIALFORM_H
#define SERIALFORM_H

#include <QWidget>
#include <QModelIndex>
#include <QSerialPort>

namespace Ui {
class SerialForm;
}

class SerialForm : public QWidget
{
    Q_OBJECT

public:
    explicit SerialForm(QWidget *parent = 0);
    ~SerialForm();
    void convertByteArrayToHexStr(const QByteArray &byteArray, QString &str);
    void sendInfoAsHex(const QString &info);
    void sendInfoAsASCII(const QString &info);
    char convertCharToHex(char ch);
    void convertStringHexToByteArray(const QString &str, QByteArray &byteData);


    QSerialPort *m_serialPort;

public slots:
    void button_SerialPort_onClick();
    void pushButton_SendText_onClick();
    void listWidget_SendHistory_onClick(QModelIndex index);
    void button_OpenSerial_onClick();
    void button_CloseSerial_onClick();
    void serialPort_onReceive();

private:
    Ui::SerialForm *ui;
};

#endif // SERIALFORM_H
