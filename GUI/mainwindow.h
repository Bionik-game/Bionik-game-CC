#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Common-utils/mapelements.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

signals:
    void quittingApplication();
    void startColourConfiguration();
    void startBoardConfiguration();

public slots:
    void updateRobotCommands(RobotCommands robotCommands);
    void updateMainImage( cv::Mat frame );
    void updateColourCalibImage( cv::Mat frame );
    void updateBoardConfImage( cv::Mat frame );

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H
