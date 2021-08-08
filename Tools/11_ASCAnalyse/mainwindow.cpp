#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datautil.h"
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* 创建数据模型 */
    model = new QStandardItemModel(this);
    model->setColumnCount(4);
    QStringList labels = QObject::trUtf8("信号ID,信号开始位,信号位长度, 方向TxRx").simplified().split(",");
    model->setHorizontalHeaderLabels(labels);
    ui->tableView->setModel(model);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}


void MainWindow::setupPlot(int lineNum, QVector<double> times, QVector<double> values)
{
  // The following plot setup is mostly taken from the plot demos:
  ui->plot->addGraph();
  ui->plot->graph()->setLineStyle(QCPGraph::lsStepLeft); // 线的类型
  ui->plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));    // 每个数据绘制一个示意点
  ui->plot->graph()->setPen(QPen(QColor(qrand()%100+100, qrand()%100+100, qrand()%100+100, 255))); // 线的颜色
  ui->plot->graph(lineNum)->setData(times, values);
  ui->plot->axisRect()->setupFullAxesBox(false);
  ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

}


void MainWindow::pushButton_UpdateFile_OnCliecked()
{
    datalist.clear();
    datalist = DataUtil::readAscFile(ui->lineEdit->text());
    qDebug() << "file:" << ui->lineEdit->text() << "datalist.count():" << datalist.count();
}

void MainWindow::pushButton_UpdateSignal_OnCliecked()
{
    QVector<double> times;
    QVector<double> values;
    int count;
    double valMin = 1000000;
    double valMax = -1000000;

    double timMin = 1000000;
    double timMax = -1000000;

    if( datalist.count() <= 0 )
    {
        pushButton_UpdateFile_OnCliecked();
        if( datalist.count() <= 0 )
        {
            ui->statusBar->showMessage("File Data Size is Zero, Reread File");
            return;
        }
    }

    int dataOffset = ui->lineEdit_signalDataOffset->text().toInt();
    for ( int i = 0; i < model->rowCount(); i++)
    {

        int signalID = model->item(i,0)->text().toInt(nullptr, 16);
        int signalOffset = model->item(i,1)->text().toInt();
        int signalLen = model->item(i,2)->text().toInt();
        QString txrx = model->item(i,3)->text();
        count = DataUtil::readAscSignal(datalist, dataOffset, signalID, signalOffset, signalLen, &times, &values, txrx.contains("Tx"), txrx.contains("Rx"));
        if( count > 0 )
        {
            setupPlot(i, times, values);

            foreach (double val, times)
            {
                if( timMin > val )
                {
                    timMin = val;
                }
                if( timMax < val )
                {
                    timMax = val;
                }
            }

            foreach (double val, values)
            {
                if( valMin > val )
                {
                    valMin = val;
                }
                if( valMax < val )
                {
                    valMax = val;
                }
            }
            qDebug() << "timMin=" << timMin << " timMax:" << timMax;
            qDebug() << "valMin=" << valMin << " valMax:" << valMax;
            ui->plot->xAxis->setRange(timMin-1, timMax+1);
            ui->plot->yAxis->setRange(valMin-1, valMax+1);
            ui->plot->replot();
        }
        else
        {
            ui->statusBar->showMessage("num:" + QString::number(i) + " data size is empty : " + QString::number(signalID, 16));
        }
    }
}

/**
* @brief MainWindow::pushButton_AddSignal_OnCliecked
* 将数据添加到tableView中
*/
void MainWindow::pushButton_AddSignal_OnCliecked()
{
    QString txrx;
    int i = model->rowCount();
    model->setItem(i, 0, new QStandardItem(ui->lineEdit_signalID->text()));
    model->setItem(i, 1, new QStandardItem(ui->lineEdit_signalOffset->text()));
    model->setItem(i, 2, new QStandardItem(ui->lineEdit_signalLen->text()));

    if( ui->checkBox_Rx->isChecked() )
    {
        txrx += "Rx";
    }

    if( ui->checkBox_Tx->isChecked() )
    {
        txrx += "Tx";
    }
    model->setItem(i, 3, new QStandardItem(txrx));
}

/**
 * @brief MainWindow::pushButton_DelSignal_OnCliecked
 * 删除选中的行数据
 */
void MainWindow::pushButton_DelSignal_OnCliecked()
{
    if( ui->tableView->currentIndex().row() >= 0 )
    {
        model->removeRow(ui->tableView->currentIndex().row());
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    static int i=0;
    i++;

    if (ui->plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {
         ui->plot->axisRect()->setRangeZoom(ui->plot->xAxis->orientation());
    }
    else if (ui->plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    {
        ui->plot->axisRect()->setRangeZoom(ui->plot->yAxis->orientation());
    }
    else
    {
        ui->plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
    }
}
