#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImageReader>
#include <QPixmap>
#include <QBrush>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImageWriter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    painterwidget(new PainterWidget),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    painterwidget->setPixmap(QPixmap(":/image/background2.png"));
    connect(painterwidget, &PainterWidget::onPointChange, this, &MainWindow::onPointChange);
    connect(painterwidget, &PainterWidget::onRectItemChange, this, &MainWindow::onRectItemChange);

    ui->scrollArea->setWidget(painterwidget);

    item_model = new QStandardItemModel();
    item_model->setHorizontalHeaderItem(0, new QStandardItem(QObject::tr("Name")));
    item_model->setHorizontalHeaderItem(1, new QStandardItem(QObject::tr("image_X.")));
    item_model->setHorizontalHeaderItem(2, new QStandardItem(QObject::tr("image_Y")));
    item_model->setHorizontalHeaderItem(3, new QStandardItem(QObject::tr("image_W")));
    item_model->setHorizontalHeaderItem(4, new QStandardItem(QObject::tr("image_H")));
    item_model->setHorizontalHeaderItem(5, new QStandardItem(QObject::tr("OpenGL_x,y,w,h")));

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->setModel(item_model);
    ui->statusBar->showMessage("open : /image/background2.png");
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addItem()
{
    ui->statusBar->showMessage("添加一条项目");
    painterwidget->addRectItem(ui->lineEdit->text());
}


void MainWindow::delItem()
{
    int currRow = ui->tableView->currentIndex().row();


    item_model->removeRow(currRow);
    painterwidget->delRectItem(currRow);

    painterwidget->update();
    ui->statusBar->showMessage("删除一条项目 " + QString::number(currRow));
}



static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog)
    {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/png");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}


void MainWindow::openFile()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted )
    {
        painterwidget->setPixmap( QPixmap(dialog.selectedFiles().first()) ) ;
        break;
    }

    ui->statusBar->showMessage("openFile: " + dialog.selectedFiles().first());

}


/**
 * @brief MainWindow::onPointChange
 *
 * 当前绘制的顶点有变化
 * @param rectTmp ： 变化中的rect数据
 */
void MainWindow::onPointChange(QRect &rectTmp)
{
    ui->statusBar->showMessage( "onPointChange (" + QString::number( rectTmp.x(), 10) +","
                                + QString::number( rectTmp.y(), 10) + ","
                                + QString::number( rectTmp.height(), 10)+ ","
                                + QString::number( rectTmp.width(), 10)+")");
    ui->spinBoxX->setValue(rectTmp.x());
    ui->spinBoxY->setValue(rectTmp.y());
    ui->spinBoxH->setValue(rectTmp.height());
    ui->spinBoxW->setValue(rectTmp.width());
}


/**
 * @brief MainWindow::onRectItemChange
 *
 * painterWidget 中的rectList的选项有变化
 * @param rectList ： painterWidget中的选项的点
 * @param strList ： painterWidget中的选项的名字
 */
void MainWindow::onRectItemChange(QList<QRect*> rectList, QList<QString> strList, QList<QRectF*> rectGlList)
{
    ui->statusBar->showMessage( "onRectItemChange" + QString::number(rectList.length()) );
    for(int i = 0; i < rectList.length(); i++)
    {
        item_model->setItem(i, 0, new QStandardItem( strList[i] ) );
        item_model->setItem(i, 1, new QStandardItem( QString::number( rectList[i]->x(), 10) ));
        item_model->setItem(i, 2, new QStandardItem( QString::number( rectList[i]->y(), 10) ));
        item_model->setItem(i, 3, new QStandardItem( QString::number( rectList[i]->width(), 10) ));
        item_model->setItem(i, 4, new QStandardItem( QString::number( rectList[i]->height(), 10) ));
        item_model->setItem(i, 5, new QStandardItem( "(" + QString::number(rectGlList[i]->x(), 'f', 4) + ","
                                                     + QString::number(rectGlList[i]->y(), 'f', 4) + ","
                                                     + QString::number(rectGlList[i]->width(), 'f', 4) + ","
                                                     + QString::number(rectGlList[i]->height(), 'f', 4) + ")" ));
    }
    ui->tableView->setModel(item_model);
}

void MainWindow::onSpinBoxXChange(int value)
{
    ui->statusBar->showMessage( "onSpinBoxXChange (" + QString::number( value, 10));
    painterwidget->rectTmp.setLeft(value);
    painterwidget->update();
}

void MainWindow::onSpinBoxYChange(int value)
{
    ui->statusBar->showMessage( "onSpinBoxYChange (" + QString::number( value, 10));
    painterwidget->rectTmp.setTop(value);
    painterwidget->update();
}

void MainWindow::onSpinBoxWidthChange(int value)
{
    ui->statusBar->showMessage( "onSpinBoxWidthChange (" + QString::number( value, 10));
    painterwidget->rectTmp.setWidth(value);
    painterwidget->update();
}

void MainWindow::onSpinBoxHeightChange(int value)
{
    ui->statusBar->showMessage( "onSpinBoxHeightChange (" + QString::number( value, 10));
    painterwidget->rectTmp.setHeight(value);
    painterwidget->update();
}

void MainWindow::onSaveSubImagePress()
{
    int count = painterwidget->saveSubImage();
    ui->statusBar->showMessage( "Save SubImage Ok 数目 : " + QString::number(count, 10));
    QMessageBox::information(NULL, "information", "图片已经保存完成 : 数目：" + QString::number(count, 10), QMessageBox::Yes, QMessageBox::Yes);
}
