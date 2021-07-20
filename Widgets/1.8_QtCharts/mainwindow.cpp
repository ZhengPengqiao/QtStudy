#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 构建图表对象
    chart = new QChart();

    // 构建折线系列对象
    QLineSeries *series = new QLineSeries();
    for (quint32 i = 0; i < 1000; i++)
    {
        // 参数 x 为循环自增变量 i，参数 y 为正弦函数Y值
        series->append(i, sin(static_cast<double>(0.6f*i)));
    }

    // 将系列添加到图表
    chart->addSeries(series);
    // 基于已添加到图表的 series 来创建默认的坐标轴
    chart->createDefaultAxes();

    // 将图表绑定到视图
    ui->graphicsView->setChart(chart);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseMoveEvent(QMouseEvent *pEvent)
{
    if (m_bMiddleButtonPressed)
    {
        QPoint oDeltaPos = pEvent->pos() - m_oPrePos;
        this->chart->scroll(-oDeltaPos.x(), oDeltaPos.y());
        m_oPrePos = pEvent->pos();
        ui->statusBar->showMessage("MiddleButton mouseMoveEvent");
    }

}

void MainWindow::mousePressEvent(QMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::MiddleButton)
    {
        m_bMiddleButtonPressed = true;
        m_oPrePos = pEvent->pos();
        this->setCursor(Qt::OpenHandCursor);
        ui->statusBar->showMessage("MiddleButton mousePressEvent");
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *pEvent)
{
    if (pEvent->button() == Qt::MiddleButton)
    {
        m_bMiddleButtonPressed = false;
        this->setCursor(Qt::ArrowCursor);
        ui->statusBar->showMessage("MiddleButton mouseReleaseEvent");
    }
}

void MainWindow::wheelEvent(QWheelEvent *pEvent)
{
    qreal rVal = std::pow(0.999, pEvent->delta()); // 设置比例
    // 1. 读取视图基本信息
    QRectF oPlotAreaRect = this->chart->plotArea();
    QPointF oCenterPoint = oPlotAreaRect.center();
    // 2. 水平调整
    oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
    // 3. 竖直调整
    oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);
    // 4.1 计算视点，视点不变，围绕中心缩放
    //QPointF oNewCenterPoint(oCenterPoint);
    // 4.2 计算视点，让鼠标点击的位置移动到窗口中心
    //QPointF oNewCenterPoint(pEvent->pos());
    // 4.3 计算视点，让鼠标点击的位置尽量保持不动(等比换算，存在一点误差)
    QPointF oNewCenterPoint(2 * oCenterPoint - pEvent->pos() - (oCenterPoint - pEvent->pos()) / rVal);
    // 5. 设置视点
    oPlotAreaRect.moveCenter(oNewCenterPoint);
    // 6. 提交缩放调整
    this->chart->zoomIn(oPlotAreaRect);
    ui->statusBar->showMessage("MiddleButton wheelEvent");
}
