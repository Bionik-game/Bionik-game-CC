#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateRobotCommands(RobotCommands robotCommands)
{
    ui->xSpeedValue->setText(QString::number(robotCommands.xCentimetersPerSecond));
    ui->ySpeedValue->setText(QString::number(robotCommands.yCentimetersPerSecond));
    ui->rotationValue->setText(QString::number(robotCommands.zRadiansPerSecond));

}

void MainWindow::closeEvent(QCloseEvent *)
{
    emit quittingApplication();
}

void MainWindow::on_pushButton_clicked()
{
    emit startColourConfiguration();
}

void MainWindow::on_pushButton_2_clicked()
{
    emit startBoardConfiguration();
}
