#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdlib.h>
#include <QWidget>
#include "renderarea.h"
#include "drawarea.h"
#include "editarea.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onStatusMessageChanged(QString message);

private slots:
    void on_actionDraw_triggered(bool checked);
    void on_actionEdit_triggered(bool checked);
    void on_actionDelete_triggered();

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
