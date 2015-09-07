#ifndef MAINROZPOZNAWATOR_H
#define MAINROZPOZNAWATOR_H

#define DEBUG

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
#include "Rozpoznawator/block.h"


class MainRozpoznawator : public QThread
{
    Q_OBJECT

public:
    const unsigned int robot1Id = 1000;
    const unsigned int robot2Id = 500;
    const unsigned int topLeftBoardId = 678;
    const unsigned int topRightBoardId = 495;
    const unsigned int bottomLeftBoardId = 233;
    const unsigned int bottomRightBoardId = 341;


private:
    QTimer rythm;
    unsigned long robotPositionX;
    cv::VideoCapture cameraCapture;
    cv::Mat cameraFrame;

    bool calibrationOn;

    vector<Block> block_types;
    vector<ColorBoxPosition> boxesPosition;

    aruco::MarkerDetector mDetector;
    vector<RobotPosition> robotsPosition;

#ifdef DEBUG
    const std::string windowName = "Debug_window";
    cv::Mat drawFrame;
#endif





    unsigned int centerX;
    unsigned int centerY;

    unsigned int minColRange;
    unsigned int maxColRange;
    unsigned int minRowRange;
    unsigned int maxRowRange;


public:
    MainRozpoznawator();

    aruco::MarkerDetector detector() const;

    void setMinColRange(unsigned int value);

    void setMaxColRange(unsigned int value);

    void setMinRowRange(unsigned int value);

    void setMaxRowRange(unsigned int value);

private:
    void updateRobotPosition();
    void updateBoxesPositions();
    void findBoxes();
    void findRobots();
    void morphOps(cv::Mat &thresh);
    void trackFilteredObject(vector<Block>* blocks, Block theBlock, cv::Mat threshold,
           cv::Mat HSV, cv::Mat &cameraFeed);
    void crop2Board();
    void findBoardPos(cv::Mat*  cameraFeed);


    void drawObject(vector<Block> theBlocks, cv::Mat &frame);
    void drawCross( cv::Mat* frame );

signals:
    void robotPositionUpdate(std::vector<RobotPosition> robotPosition);
    void boxesPositionUpdate(std::vector<ColorBoxPosition> boxesPositionVector);

public slots:
    void colourCalibration();
    void boardConfiguration();

private slots:
    void mainWork();

};

#endif // MAINROZPOZNAWATOR_H

