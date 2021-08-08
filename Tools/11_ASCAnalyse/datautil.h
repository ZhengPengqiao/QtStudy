#ifndef DATAUTIL_H
#define DATAUTIL_H

#include <QString>

class DataUtil
{
public:
    DataUtil();
    /**
     * @brief readAscFile
     * 将文件内容读取到字符串列表中
     * @param fileName:文件名及路径
     * @return:包含文件内容的字符串列表
     */
    static QStringList readAscFile(QString fileName);

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
    static int readAscSignal(QStringList strList, int signalIndex, int signalID, int signalOffset, int signalLen, QVector<double> *time, QVector<double> *val, bool Tx, bool Rx);
    static int getArrayOffsetValue(QStringList strList,  int signalIndex, int offset, int len);
};

#endif // DATAUTIL_H
