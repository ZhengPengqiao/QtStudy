#ifndef TTHBPFORM_H
#define TTHBPFORM_H

#include <QWidget>
#include <QModelIndex>
#include <QSerialPort>

namespace Ui {
class TTHBPForm;
}

class TTHBPForm : public QWidget
{
    Q_OBJECT

public:
    explicit TTHBPForm(QWidget *parent = 0);
    ~TTHBPForm();
    void convertByteArrayToHexStr(const QByteArray &byteArray, QString &str);
    void sendInfoAsHex(const QString &info);
    void sendInfoAsASCII(const QString &info);
    char convertCharToHex(char ch);
    void convertStringHexToByteArray(const QString &str, QByteArray &byteData);


    QSerialPort *m_serialPort;

    char frameId;
    char P[5];

public slots:
    void button_SerialPort_onClick();
    void pushButton_SendText_onClick();
    void button_OpenSerial_onClick();
    void button_CloseSerial_onClick();
    void serialPort_onReceive();
    void pushButton_PIN_onClick();

private:
    Ui::TTHBPForm *ui;
};

#endif // TTHBPFORM_H
