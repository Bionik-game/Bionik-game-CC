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


void MainWindow::updateMainImage( cv::Mat frame ){
    cv::imshow( "Debug_window", frame );
}

void MainWindow::updateColourCalibImage( cv::Mat frame ){
    cv::imshow( "Calibration", frame );
}

void MainWindow::updateBoardConfImage( cv::Mat frame ){
    cv::imshow( "Board_Config", frame );
}
