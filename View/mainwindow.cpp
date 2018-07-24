#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QListView"
#include <QStackedLayout>
#include <QScrollArea>
#include <qdebug.h>
#include "Model/mainmodel.h"
#include "Controller/maincontroller.h"

DrawArea *draw;
EditArea *edit;
QStackedLayout *renderLayout;

MainController* Controller;
MainModel* Model;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Model = new MainModel();
    Controller = new MainController(Model);


    QObject::connect(Controller, &MainController::messageChanged,
                     this, &MainWindow::onStatusMessageChanged);

    renderLayout = new QStackedLayout;
    draw = new DrawArea(Controller);

    draw->setMinimumSize(DRAWING_WIDTH, DRAWING_HEIGHT);
    QScrollArea* drawScroll = new QScrollArea();
    drawScroll->setBackgroundRole(QPalette::Dark);
    drawScroll->setWidget(draw);
    renderLayout->addWidget(drawScroll);

    edit = new EditArea(Controller);
    edit->setMinimumSize(DRAWING_WIDTH, DRAWING_HEIGHT);
    QScrollArea* editScroll = new QScrollArea();
    editScroll->setBackgroundRole(QPalette::Dark);
    editScroll->setWidget(edit);
    renderLayout->addWidget(editScroll);

    ui->centralWidget->setLayout(renderLayout);

    on_actionDraw_triggered(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStatusMessageChanged(QString message)
{
    statusBar()->showMessage(message);
}

void MainWindow::on_actionDraw_triggered(bool checked)
{
    if (checked) {
        renderLayout->setCurrentIndex(0);
        ui->actionEdit->setChecked(false);
        ui->actionDelete->setDisabled(true);
        Controller->setState(MainController::DRAW);
        draw->update();
    }
}

void MainWindow::on_actionEdit_triggered(bool checked)
{
    if (checked) {
        renderLayout->setCurrentIndex(1);
        ui->actionDraw->setChecked(false);
        ui->actionDelete->setDisabled(false);
        Controller->setState(MainController::EDIT);
        edit->update();
    }
}

void MainWindow::on_actionDelete_triggered()
{
    if (renderLayout->currentIndex() == 1) {
        edit->deleteSelected();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
        draw->validateForm();
    else if (event->modifiers() == Qt::CTRL) {
        Controller->ControlPressed(true);
        draw->update();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control) {
        Controller->ControlPressed(false);
        draw->update();
    }
}
