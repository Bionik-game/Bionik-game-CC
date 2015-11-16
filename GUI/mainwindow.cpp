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

void MainWindow::updateRobotCommandsBionik(RobotCommands robotCommands)
{
    ui->xSpeedValue->setNum(robotCommands.xCentimetersPerSecond);
    ui->ySpeedValue->setNum(robotCommands.yCentimetersPerSecond);
    ui->rotationValue->setNum(robotCommands.zRadiansPerSecond);
}
void MainWindow::updateRobotCommandsCzerwony(RobotCommands robotCommands)
{
    ui->xSpeedVaCzerwony->setNum(robotCommands.xCentimetersPerSecond);
    ui->ySpeedVaCzerwony->setNum(robotCommands.yCentimetersPerSecond);
    ui->rotateVaCzerwony->setNum(robotCommands.zRadiansPerSecond);
}
void MainWindow::updateRobotCommandsNiebieski(RobotCommands robotCommands)
{
    ui->xSpeedVaNiebieski->setNum(robotCommands.xCentimetersPerSecond);
    ui->ySpeedVaNiebieski->setNum(robotCommands.yCentimetersPerSecond);
    ui->rotateVaNiebieski->setNum(robotCommands.zRadiansPerSecond);
}
void MainWindow::closeEvent(QCloseEvent *)
{
    emit quittingApplication();
}
