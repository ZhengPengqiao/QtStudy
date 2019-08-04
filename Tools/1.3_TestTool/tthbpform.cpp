#include "tthbpform.h"
#include "ui_tthbpform.h"
#include <QSerialPortInfo>
#include <QDebug>
#include <QTime>
#include <QListWidgetItem>
#include <QSerialPort>
#include <QThread>

TTHBPForm::TTHBPForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TTHBPForm)
{
    ui->setupUi(this);
    QStringList m_serialPortName;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        m_serialPortName << info.portName();
        ui->comboBox_SerialPort->addItem(info.portName());
    }

    m_serialPort = nullptr;
    frameId = 0;
    memset(P, 0, sizeof(P));
}

TTHBPForm::~TTHBPForm()
{
    delete ui;

    if( m_serialPort != nullptr )
    {
        if (m_serialPort->isOpen())
        {
            m_serialPort->close();
        }

        delete m_serialPort;
    }
}

void TTHBPForm::button_SerialPort_onClick()
{
    QStringList m_serialPortName;

    ui->comboBox_SerialPort->clear();
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        m_serialPortName << info.portName();
        ui->comboBox_SerialPort->addItem(info.portName());
    }
    m_serialPort = nullptr;
}


void TTHBPForm::pushButton_SendText_onClick()
{
    if( m_serialPort == nullptr )
    {
        ui->textEdit_Text->append("Send 串口没有打开");
        return;
    }


    if( !ui->lineEdit_Text->text().isEmpty() )
    {
        if( ui->radioButton_ASCII->isChecked() )
        {
            sendInfoAsASCII(ui->lineEdit_Text->text());
        }
        else
        {
            sendInfoAsHex(ui->lineEdit_Text->text());
        }

    }
}


void TTHBPForm::button_OpenSerial_onClick()
{

    m_serialPort = new QSerialPort();//实例化串口类一个对象

    if(m_serialPort->isOpen())//如果串口已经打开了 先给他关闭了
    {
        m_serialPort->clear();
        m_serialPort->close();
    }

    ui->label_SerialStatusBack->setStyleSheet("border-image: url(:/assert/ICON/101.png);");

    //设置串口名字 假设我们上面已经成功获取到了 并且使用第一个
    m_serialPort->setPortName(ui->comboBox_SerialPort->currentText());

    if(!m_serialPort->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
    {
        qDebug()<<ui->comboBox_SerialPort->currentText()<<" 打开失败!";
        return;
    }
    //打开成功

    ////设置波特率和读写方向
    m_serialPort->setBaudRate(QSerialPort::Baud9600,QSerialPort::AllDirections);
    m_serialPort->setDataBits(QSerialPort::Data8);      //数据位为8位
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);//无流控制
    m_serialPort->setParity(QSerialPort::NoParity); //无校验位
    m_serialPort->setStopBits(QSerialPort::OneStop); //一位停止位

    //连接信号槽 当下位机发送数据QSerialPortInfo 会发送个 readyRead 信号,我们
    //定义个槽void receiveInfo()解析数据
    connect(m_serialPort, &QSerialPort::readyRead, this, &TTHBPForm::serialPort_onReceive);

    qDebug() << "TTHBP Serial Open";
}

void TTHBPForm::button_CloseSerial_onClick()
{
    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
    }
    delete m_serialPort;

    ui->label_SerialStatusBack->setStyleSheet("border-image: url(:/assert/ICON/102.png);");
    qDebug() << "TTHBP Serial Close";
}


//接收单片机的数据
void TTHBPForm::serialPort_onReceive()
{
    QByteArray info = m_serialPort->readAll();

    if( ui->radioButton_ASCII->isChecked() )
    {
        ui->textEdit_Text->append(QTime::currentTime().toString("HH:mm:ss.zzz") +\
                                  "<----:" + info);
    }
    else
    {
        QString str;
        convertByteArrayToHexStr(info.toHex(), str);
        ui->textEdit_Text->append(QTime::currentTime().toString("HH:mm:ss.zzz") +\
                                  "<----:" + str);
    }
    emit tthbp_receiveData(info);

}

/**
 * @brief TTHBPForm::convertByteArrayToHexStr
 * @param str
 * @param byteData
 */
void TTHBPForm::convertByteArrayToHexStr(const QByteArray &byteArray, QString &str)
{
    int i;
    int len = byteArray.length();

    for( i = 0; i < len/2; i++ )
    {
        str.append(byteArray[2*i]);
        str.append(byteArray[2*i+1]);
        str.append(' ');
    }
}


/**
 * @brief TTHBPForm::sendInfoAsASCII
 * @param info
 */
void TTHBPForm::sendInfoAsASCII(const QString &info)
{
    QByteArray sendBuf;
    //把QString 转换 为 ByteArray
    sendBuf = info.toLatin1();

    // 将发送的指令,添加到历史列表和显示列表中
    ui->textEdit_Text->append(QTime::currentTime().toString("HH:mm:ss.zzz") \
                              + "---->:" + sendBuf);


    if( m_serialPort == nullptr )
    {
        ui->textEdit_Text->append("Send ASCII 串口没有打开");
        return;
    }
    else
    {
        // 将数据发送到Uart中
        m_serialPort->write(sendBuf, sendBuf.length());
    }
}

/**
 * @brief TTHBPForm::sendInfoAsHex
 * @param info
 */
void TTHBPForm::sendInfoAsHex(const QString &info)
{
    QByteArray sendBuf;

    convertStringHexToByteArray(info, sendBuf);

    // 将发送的指令,添加到历史列表和显示列表中
    ui->textEdit_Text->append(QTime::currentTime().toString("HH:mm:ss.zzz") \
                              + "---->:" + info);


    if( m_serialPort == nullptr )
    {
        ui->textEdit_Text->append("Send Hex 串口没有打开");
        return;
    }
    else
    {
        // 将数据发送到Uart中
        m_serialPort->write(sendBuf, sendBuf.length());
    }
}


void TTHBPForm::convertStringHexToByteArray(const QString &str, QByteArray &byteData)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    byteData.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char:lstr 获取紧接着的不是空格的字符
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }

        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = convertCharToHex(hstr);
        lowhexdata = convertCharToHex(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        byteData[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    byteData.resize(hexdatalen);
}


/**
 * @brief TTHBPForm::convertCharToHex
 * 0-9的ASCII值是(0x30,49)-(0x39,57)
 * A-F的ASCII值是(0x41,65)-(0x46,70)
 * a-f的ASCII值是(0x61,97)-(0x66,102)
 * @param ch : 将字符转换成ASCII对应的值
 * @return 转换后的ASCII的值
 */
char TTHBPForm::convertCharToHex(char ch)
{
    if((ch >= '0') && (ch <= '9'))
         return ch-0x30;
     else if((ch >= 'A') && (ch <= 'F'))
         return ch-'A'+10;
     else if((ch >= 'a') && (ch <= 'f'))
         return ch-'a'+10;
     else return (-1);
}

void TTHBPForm::pushButton_PIN_onClick()
{
    QString strName = sender()->objectName();
    QString tmp;
    int i=0, j=0;
    char reg = 0;
    char val = 0;
    QString str;
    QByteArray byteData;


    for( i = 0; i <= 3; i++ )
    {
        for( j = 0; j < 8; j++ )
        {

            byteData.clear();
            str.clear();
            tmp = QString("pushButton_P%1_%2").arg(i).arg(j);
            if( strName == tmp)
            {
                if( (P[i]&(1<<j)) == 0 )
                {
                    P[i] |= (1<<j);
                    val = 1;
                }
                else
                {
                    P[i] &= ~(1<<j);
                    val = 0;
                }
                frameId++;
                reg = ((i<<4)+j);
                byteData.append(0x03);
                byteData.append(frameId);
                byteData.append(0x02);
                byteData.append(reg);
                byteData.append(val);
                byteData.append(0x03^frameId^0x02^reg^val);
                byteData.append(0x3a);
                byteData.append(0x3c);
                convertByteArrayToHexStr(byteData.toHex(), str);

                sendInfoAsHex(str);
            }
        }
    }

    for( j = 0; j < 4; j++ )
    {
        tmp = QString("pushButton_P4_%1").arg(j);
        if( strName == tmp)
        {

            byteData.clear();
            str.clear();
            if( (P[4]&(1<<j)) == 0 )
            {
                P[4] |= (1<<j);
                val = 1;
            }
            else
            {
                P[i] &= ~(1<<j);
                val = 0;
            }

            reg = ((4<<4)+j);
            frameId++;
            byteData.append(0x03);
            byteData.append(frameId);
            byteData.append(0x02);
            byteData.append(reg);
            byteData.append(val);
            byteData.append(0x03^frameId^0x02^reg^val);
            byteData.append(0x3a);
            byteData.append(0x3c);
            convertByteArrayToHexStr(byteData.toHex(), str);

            sendInfoAsHex(str);
        }
    }
}


void TTHBPForm::pushButton_PINALL_onClick()
{
    QString strName = sender()->objectName();
    QString tmp;
    QByteArray byteData;
    QString str;

    char reg;
    int i;
    for( i = 0; i <= 4; i++ )
    {
        byteData.clear();
        str.clear();
        tmp = QString("pushButton_P%1_ALL").arg(i);
        if( strName == tmp)
        {
            P[i] = ~P[i];
            frameId++;
            reg = ((i<<4)+8);
            byteData.append(0x03);
            byteData.append(frameId);
            byteData.append(0x02);
            byteData.append(reg);
            byteData.append(P[i]);
            byteData.append(0x03^frameId^0x02^reg^P[i]);
            byteData.append(0x3a);
            byteData.append(0x3c);
            convertByteArrayToHexStr(byteData.toHex(), str);

            sendInfoAsHex(str);
        }
    }
}

void TTHBPForm::pushButton_PINUpdate_onClick()
{
    QByteArray byteData;
    QString str;

    char reg;
    int i;
    for( i = 0; i <= 4; i++ )
    {
        byteData.clear();
        str.clear();
        frameId++;
        reg = (i<<4)+8;
        byteData.append(0x04);
        byteData.append(frameId);
        byteData.append(0x01);
        byteData.append(reg);
        byteData.append(0x04^frameId^0x01^reg);
        byteData.append(0x3a);
        byteData.append(0x3c);
        convertByteArrayToHexStr(byteData.toHex(), str);

        sendInfoAsHex(str);
        QThread::msleep(3);
    }
}


void TTHBPForm::tthbp_sendData(QByteArray buf)
{
    QString str;
    if( m_serialPort == nullptr )
    {
        ui->textEdit_Text->append("Send 串口没有打开");
        return;
    }

    convertByteArrayToHexStr(buf.toHex(), str);
    sendInfoAsHex(str);
}
