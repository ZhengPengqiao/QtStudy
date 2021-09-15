#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:

  /* 启动终端解析器 */
  void start_shell();
  /* 向终端解析器 写入指令 */
  void run_cmd();
  void run_cmd(QString cmd_str);

  /* 标准输出 */
  void on_readyReadStandardOutput();
  /* 标准错误输出 */
  void on_readyReadStandardError();

  /* QProcess程序启动输出展示 */
  void on_started();
  /* QPRocess程序写入展示 */
  void on_byteWritten();

private:
  Ui::MainWindow *ui;
  QProcess *cmd;
};

#endif // MAINWINDOW_H
