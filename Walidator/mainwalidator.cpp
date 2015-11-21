#include "mainwalidator.h"

#include <iostream>

using namespace std;
using namespace cv;

#define MIN_DISTANCE_TO_WALL_PIXELS 10

MainWalidator::MainWalidator()
{
    QObject::connect(&dataCollector, &MyDataCollector::allDataNew, this, &MainWalidator::processData);
    QObject::connect(&dataCollector, &MyDataCollector::dataDropped,
                     [](unsigned int no)
    {
        std::cerr << "Element number " << no << " was replaced before rest of data arrived in MainWalidator" << std::endl;
    });
}

double calcDistPointToLine( cv::Point2i linePoint1, cv::Point2i linePoint2, cv::Point2i robotPos){

    double distance = fabs((linePoint2.y - linePoint1.y)*robotPos.x - (linePoint2.x - linePoint1.x)*robotPos.y + linePoint2.x*linePoint1.y - linePoint2.y*linePoint1.x   );
    distance = distance/ sqrt( (linePoint2.y - linePoint1.y)*(linePoint2.y - linePoint1.y)  + (linePoint2.x - linePoint1.x)*(linePoint2.x - linePoint1.x) );
    return distance;
}

struct vect2d {
    double x;
    double y;
};

RobotCommands MainWalidator::correctionVelocityVector ( Robot robot , RobotCommands robotCommand){

    int polyCorners = this->boardCornersVec.size();
    int   i, j=polyCorners-1 ;
    int xi,xj,yi,yj;

    vect2d normalVector;
    double dotProduct;
    vect2d velVector;
    vect2d correctionVector;

        for (i=0; i<polyCorners; i++) {
            xi = this->boardCornersVec.at(i).x ;
            xj = this->boardCornersVec.at(j).x ;
            yi = this->boardCornersVec.at(i).y ;
            yj = this->boardCornersVec.at(j).y ;



            velVector.x = robotCommand.xCentimetersPerSecond;
            velVector.y = robotCommand.yCentimetersPerSecond;

            cout<<velVector.x << " " << velVector.y << endl;


            if( calcDistPointToLine(this->boardCornersVec.at(i),this->boardCornersVec.at(j), Point2i(robot.xCentimeters,robot.yCentimeters)) < MIN_DISTANCE_TO_WALL_PIXELS ){
                //Calculate vector normal to wall, facing outwards
                normalVector.x = yi - yj;
                normalVector.y = xj - xi;
                double length = sqrt(normalVector.x*normalVector.x + normalVector.y*normalVector.y);
                normalVector.x = normalVector.x/length;
                normalVector.y = normalVector.y/length;
                //Calculate dot product of velocity vector and normal vector
                dotProduct = velVector.x*normalVector.x + velVector.y*normalVector.y;
                //Correct the original vector
                correctionVector.x = dotProduct*normalVector.x;
                correctionVector.y = dotProduct*normalVector.y;
                robotCommand.xCentimetersPerSecond -= correctionVector.x;
                robotCommand.yCentimetersPerSecond -= correctionVector.y;
            }

         }
    return robotCommand;
}

void MainWalidator::processData()
{
    vector<Robot> robotsVec = dataCollector.get<std::vector<Robot>>(0);
    vector<ColorBox> boxesVec = dataCollector.get<std::vector<ColorBox>>(1);


    RobotCommands robotCommandsCorrected = dataCollector.get<RobotCommands>(2);

    for(int i = 0; i <robotsVec.size(); i++){
        robotCommandsCorrected = correctionVelocityVector(robotsVec.at(i), robotCommandsCorrected);
    }


    emit robotCommandUpdateCorrected(robotCommandsCorrected);
}

void MainWalidator::gameState(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec)
{
    dataCollector.set(0, robotVec);
    dataCollector.set(1, colorBoxVec);
}

void MainWalidator::robotCommandUpdateRaw(RobotCommands robotCommands)
{
    dataCollector.set(2, robotCommands);
}

void MainWalidator::boardPos( std::vector<cv::Point2i> boardCorners) {
    this->boardCornersVec = boardCorners;
    std::cout << this->boardCornersVec.at(0);
}
