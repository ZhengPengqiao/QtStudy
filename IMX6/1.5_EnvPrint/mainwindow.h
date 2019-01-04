#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFrame>
#include <QLabel>
#include <QTextEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QFrame
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *e);

public:
    explicit MainWindow(QFrame *parent = 0);
    ~MainWindow();
    QLabel *testLabel;
    QLabel *keyLabel;
    QTextEdit *envEdit;

    int key_count;
};

#endif // MAINWINDOW_H
