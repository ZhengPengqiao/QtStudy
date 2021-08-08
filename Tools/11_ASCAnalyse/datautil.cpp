#include "datautil.h"
#include<QStringList>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

DataUtil::DataUtil()
{

}

/**
 * @brief readAscFile
 * 将文件内容读取到字符串列表中
 * @param fileName:文件名及路径
 * @return:包含文件内容的字符串列表
 */
QStringList DataUtil::readAscFile(QString fileName)
{
    QFile ascFile(fileName);
    QStringList dataList;
    dataList.clear();
    if (ascFile.open(QIODevice::ReadWrite)) //对asc文件进行读写操作
    {
        QTextStream stream(&ascFile);
        while (!stream.atEnd())
        {
            dataList.push_back(stream.readLine()); //保存到List当中
        }
            ascFile.close();
    }
    else
    {
        QMessageBox::about(NULL, "asc文件", "未打开该文件！");
    }
    return dataList;
}


/**
 * @brief DataUtil::readAscSignal
 * @param strList:所有asc数据
 * @param signalIndex:有效数据的偏移位置
 * @param signalID:信号ID
 * @param signalOffset:信号值的偏移量
 * @param signalLen:信号值占用位数
 * @param time:时间轴
 * @param val:数据轴
 * @param Tx:是否检测Tx方向， True检测，False不检测
 * @param Rx:是否检测Tx方向， True检测，False不检测
 * @return:过滤出来的数据个数
 */
int DataUtil::readAscSignal(QStringList strList, int signalIndex, int signalID, int signalOffset, int signalLen, QVector<double> *time, QVector<double> *val, bool Tx, bool Rx)
{
    int value = -1;
    if( time == nullptr || val == nullptr || strList.count() <= 0)
    {
        return 0;
    }
    time->clear();
    val->clear();


    foreach (QString line, strList)
    {
        line = line.simplified();
        if( line.contains("version") )
        {
            qDebug() << line.split(QRegExp(" version ")).at(1);
        }
        else if( (Tx && line.contains(" Tx ")) || (Rx && line.contains(" Rx ")) )
        {
            // 只有Tx为True才会进行后面contains Tx判断，  Rx为True才会进行contains Rx判断
            QStringList list = line.split(" ");
            if( list.at(4).toInt(nullptr, 16) == signalID )
            {
                qDebug() << list;
                time->append(list.at(0).toDouble());
                value = getArrayOffsetValue(list, signalIndex, signalOffset, signalLen);
                val->append(value);
            }
        }

    }

    return time->count();
}

/**
 * @brief DataUtil::getArrayOffsetValue
 * 从字符串列表中获取对应的数据
 * @param strList:字符串列表
 * @param signalIndex:真实有效数据偏移位置
 * @param offset:提取数 = 据在有效数据 中的偏移位置
 * @param len:待提取的位数
 * @return :提取出来的数值， >0:正常-1:异常
 */
int DataUtil::getArrayOffsetValue(QStringList strList, int signalIndex, int offset, int len)
{
    int value = 0; // 计算出来的值
    int valueOffset = 0; // 记录已经偏移了多少个字节了
    int lenLeft = len; // 还需要提取多少位
    int arrayIndex = 0; // 当前需要处理字节在 array中的位置
    int arrayBitIndex = 0;  // 当前需要处理位bit在 array中的位置

    unsigned int byteVal = 0;

    qDebug() << "list.size:" << strList.count() << " signalIndex:" << signalIndex << " offset:" << offset << " len:"<< len;
    while(lenLeft) // 提取需要的位，直到提取完成
    {
        arrayIndex = offset/8; // 当前需要处理字节在array中的位置
        arrayBitIndex = offset%8;  // 当前需要处理位在 array中的位置, 且此Byte可以处理的位数// 当前需要处理位bit在 array中的位置


        qDebug() << "lenLeft:" << lenLeft << " arrayIndex:" << arrayIndex << " 8-arrayBitIndex:" << 8-arrayBitIndex << \
                    " offset:" << offset << " arrayBitIndex:" << arrayBitIndex << " value:" << value;

        if( strList.count() < arrayIndex+signalIndex )
        {
            return -1;
        }
        byteVal = strList.at(arrayIndex+signalIndex).toInt(nullptr,16);

        if( (8-arrayBitIndex) >= lenLeft )
        {
            // 当前字节的bit位数 满足需要提取的位数

            for( int j = 0; j < lenLeft; j++ )
            {
                // 按照对应位为0/1进行处理
                if( byteVal & (1<<(j+arrayBitIndex)) )
                {
                    value += (1<<valueOffset);
                    valueOffset++;
                }
                else
                {
                    value += 0;
                    valueOffset++;
                }

                lenLeft--;
                offset++;
            }
        }
        else if( ((8-arrayBitIndex) <= lenLeft) && (arrayBitIndex != 0) )
        {
            // 当前字节的bit位数 不够满足需要提取的位数
            for( int j = 0; j < (8-arrayBitIndex); j++ )
            {
                // 按照对应位为0/1进行处理
                if( byteVal & (1<<(j+arrayBitIndex)) )
                {
                    value += (1<<valueOffset);
                    valueOffset++;
                }
                else
                {
                    value += 0;
                    valueOffset++;
                }

                lenLeft--;
                offset++;
            }
        }
        else
        {
            value += (byteVal<<valueOffset);
            valueOffset += 8;
            lenLeft -= 8;
            offset += 8;
        }
    }

    return value;
}
