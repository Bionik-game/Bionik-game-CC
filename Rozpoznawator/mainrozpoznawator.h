#ifndef MAINROZPOZNAWATOR_H
#define MAINROZPOZNAWATOR_H

#define DEBUG

#include <vector>
#include <math.h>
#include <string>

#include <QThread>
#include <QVector>
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
    const unsigned int robot1Id = 125;         //Id znacznika robota nr 1
    const unsigned int robot2Id = 175;          //Id znacznika robota nr 2
    const unsigned int topLeftBoardId = 618;
    const unsigned int topRightBoardId = 1005;   // Id znaczników planszy
    const unsigned int bottomLeftBoardId = 428;
    const unsigned int bottomRightBoardId = 341;

    std::vector<cv::Point2i> boardCorners;


private:
    QTimer rythm;
    cv::VideoCapture cameraCapture;
    cv::Mat cameraFrame;

    std::vector<Block> block_types;              // Wektor przechowujący bloki z informacją o wyszukiwanych kolorach
    std::vector<ColorBox> colorBoxesInfo;
    std::vector<Block> all_blocks;



    aruco::MarkerDetector mDetector;
    std::vector<Robot> robotsinfo;

#ifdef DEBUG
    const std::string windowName = "Debug_window";
    cv::Mat drawFrame;
#endif

    unsigned int centerX;   // Współrzędna środka klatki
    unsigned int centerY;   // Współrzędna środka klatki

    unsigned int minColRange;   // Wartości określające przedziały wartości
    unsigned int maxColRange;   // do wycięcia z klatki, po wykryciu planszy
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
    void findBoxes();                           // Wyszukiwanie kolorowych obiektów
    void findRobots();                          // Wyszukiwanie robotów
    void morphOps(cv::Mat &thresh);             // Operacja pomocnicza do przetwarzania obrazu
    void trackFilteredObject(vector<Block>* blocks, Block theBlock, cv::Mat threshold,  //Śledzenie znalezionych kolorowych obiektów
           cv::Mat HSV, cv::Mat &cameraFeed);
    void crop2Board();                          // Obcinanie klatki obrazu do znalezionej planszy
    void findBoardPos(cv::Mat*  cameraFeed);    // Wyszukiwanie znaczników planszy


    void drawObject(vector<Block> theBlocks, cv::Mat &frame); // Rysowanie znalezionych kolorowych obiektów
    void drawCross( cv::Mat* frame ); // Rysowanie pomocniczego krzyzyka na srodku klatki

    bool pointInPolygon( int x, int y );

signals:
    void gameState(std::vector<Robot> robotsinfo, std::vector<ColorBox> colorBoxesInfo);
    void updateMainImage( cv::Mat frame );
    void updateColourCalibImage( cv::Mat frame );
    void updateBoardConfImage( cv::Mat frame );


public slots:
    void colourCalibration();           //Slot na sygnał z przycisku z głównego okna
    void boardConfiguration();          //Slot na sygnał z przycisku z głównego okna

private slots:
    void mainWork();

};

#endif // MAINROZPOZNAWATOR_H


