#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cv::namedWindow( "Debug_window", 1);
    cv::namedWindow("Calibration", 1);
    cv::namedWindow("Board_Config", 1);
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

void MainWindow::on_pushButton_clicked()
{
    emit startColourConfiguration();
}

void MainWindow::on_pushButton_2_clicked()
{
    emit startBoardConfiguration();
}


void MainWindow::updateMainImage( cv::Mat frame ){
    cv::imshow( "Debug_window", frame );
}

void MainWindow::updateColourCalibImage( cv::Mat frame ){
    cv::imshow( "Calibration", frame );
}

void MainWindow::updateBoardConfImage( cv::Mat frame ){
    cv::imshow( "Board_Config", frame );
}
