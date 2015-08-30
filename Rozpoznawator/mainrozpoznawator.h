#ifndef MAINROZPOZNAWATOR_H
#define MAINROZPOZNAWATOR_H

#include <vector>
#include <math.h>
#include <string>

#include <QThread>
#include <QTimer>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>

#include <aruco/aruco.h>
#include <aruco/cvdrawingutils.h>

#include "Common-utils/mapelements.h"



class MainRozpoznawator : public QThread
{
    Q_OBJECT

private:
    QTimer rythm;
    unsigned long robotPositionX;
    cv::VideoCapture cameraCapture;

public:
    MainRozpoznawator();

private:
    void updateRobotPosition();
    void updateBoxesPositions();

signals:
    void robotPositionUpdate(RobotPosition robotPosition);
    void boxesPositionUpdate(std::vector<ColorBoxPosition> boxesPositionVector);

private slots:
    void mainWork();
};

#endif // MAINROZPOZNAWATOR_H
