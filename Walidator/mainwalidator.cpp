#include "mainwalidator.h"
#include "Common-utils/mapelements.h"
#include <iostream>

using namespace std;
using namespace cv;

unsigned int MIN_DISTANCE_TO_WALL_PIXELS = 100;
const int FRAME_WIDTH = 800;
const int FRAME_HEIGHT = 600;

MainWalidator::MainWalidator()
{
    QObject::connect(&dataCollector, &MyDataCollector::allDataNew, this, &MainWalidator::processData);
    QObject::connect(&dataCollector, &MyDataCollector::dataDropped,
                     [](unsigned int no)
    {
        std::cerr << "Element number " << no << " was replaced before rest of data arrived in MainWalidator" << std::endl;
    });

    normalVectors.clear();
    vect2d vec;
    vec.x = 0.0;
    vec.y = -1.0;
    normalVectors.push_back(vec);
    vec.x = 1.0;
    vec.y = 0.0;
    normalVectors.push_back(vec);
    vec.x = 0.0;
    vec.y = 1.0;
    normalVectors.push_back(vec);
    vec.x = -1.0;
    vec.y = 0.0;
    normalVectors.push_back(vec);
        this->boardCornersVec.push_back(Point2i(-FRAME_WIDTH/2,-FRAME_HEIGHT/2));       //Lewy dolny
     this->boardCornersVec.push_back(Point2i(FRAME_WIDTH/2,-FRAME_HEIGHT/2)); //Prawy dolny
        this->boardCornersVec.push_back(Point2i(FRAME_WIDTH/2,FRAME_HEIGHT/2)); //Prawy gorny
    this->boardCornersVec.push_back(Point2i(-FRAME_WIDTH/2,FRAME_HEIGHT/2));     //Lewy gorny





    cout << normalVectors.size() << endl;
    for(int i =0; i< normalVectors.size(); i++){
        cout << normalVectors.at(i).x << " " << normalVectors.at(i).y << endl;
    }

}

double calcDistPointToLine( cv::Point2i linePoint1, cv::Point2i linePoint2, cv::Point2i robotPos){

    double distance = fabs((linePoint2.y - linePoint1.y)*robotPos.x - (linePoint2.x - linePoint1.x)*robotPos.y + linePoint2.x*linePoint1.y - linePoint2.y*linePoint1.x   );
    distance = distance/ sqrt( (linePoint2.y - linePoint1.y)*(linePoint2.y - linePoint1.y)  + (linePoint2.x - linePoint1.x)*(linePoint2.x - linePoint1.x) );
    return distance;
}



double computeDistance( int robotX, int robotY, Point2i linePoint1, Point2i linePoint2 ){

    int point1y = linePoint1.y;
    int point1x = linePoint1.x;
    int point2y = linePoint2.y;
    int point2x = linePoint2.x;

    double distance = abs((point2y-point1y)*robotX - (point2x-point1x)*robotY + point2x*point1y - point2y*point1x)/(sqrt((point2y-point1y)*(point2y-point1y)+ (point2x-point1x)*(point2x-point1x)));
    return distance;

}

RobotCommands MainWalidator::correctionVelocityVector ( Robot robot , RobotCommands robotCommand){



    cout << "test1" << endl;

    int polyCorners =  this->boardCornersVec.size();
    int   i,j;
    int xi,xj,yi,yj;







    vect2d normalVector;
    double dotProduct;
    vect2d velVector;
    vect2d correctionVector;
   // cout << polyCorners << endl;
        for (i=0,j=1; i<polyCorners; ++i,j++) {
          //  cout << "test" << endl;
            if(j > 3)
                   j = 0;
        /*    xi = this->boardCornersVec.at(i).x ;
            xj = this->boardCornersVec.at(j).x ;                            //Tymczasowa zmiana!
            yi = this->boardCornersVec.at(i).y ;
            yj = this->boardCornersVec.at(j).y ;
        */
          //  cout << "Poly:  " << polyCorners << endl;


            velVector.x = robotCommand.xCentimetersPerSecond;
            velVector.y = robotCommand.yCentimetersPerSecond;

            cout<<velVector.x << " " << velVector.y << endl;



           double distToBorder = computeDistance(robot.xCentimeters,robot.yCentimeters,this->boardCornersVec.at(i),this->boardCornersVec.at(j));//calcDistPointToLine(this->boardCornersVec.at(i),this->boardCornersVec.at(j), Point2i(robot.xCentimeters,robot.yCentimeters));
          // double distToBorder = calcDistPointToLine(boardCornersVec.at(i), boardCornersVec.at(j), )

            normalVector = normalVectors.at(i);
            if( distToBorder < MIN_DISTANCE_TO_WALL_PIXELS ){
                //Calculate vector normal to wall, facing outwardsn
               /* normalVector.x = yi - yj; // @mateusz: nie wiem dlaczego tutaj sa yi,yj zamiast x
                normalVector.y = xj - xi;
                double length = sqrt(normalVector.x*normalVector.x + normalVector.y*normalVector.y);
                normalVector.x = normalVector.x/length;
                normalVector.y = normalVector.y/length;

                //Calculate dot product of velocity vector and normal vector                                     //Tymczasowa zmiana!
             //   cout << normalVector.x << " " << normalVector.y << "    " << i << endl;
                dotProduct = velVector.x*normalVector.x + velVector.y*normalVector.y;
                //Correct the original vector


                correctionVector.x = dotProduct*normalVector.x;

                correctionVector.y = dotProduct*normalVector.y;
                robotCommand.xCentimetersPerSecond -= correctionVector.x;
                robotCommand.yCentimetersPerSecond -= correctionVector.y;
                */
                if( i == 0){
                    if( robotCommand.yCentimetersPerSecond > 0 )
                        robotCommand.yCentimetersPerSecond = 0;
                }else if( i == 1){
                    if( robotCommand.xCentimetersPerSecond > 0 )
                        robotCommand.xCentimetersPerSecond = 0;
                }else if( i == 2){
                    if( robotCommand.yCentimetersPerSecond < 0 )
                        robotCommand.yCentimetersPerSecond = 0;
                }else if( i == 3){
                    if( robotCommand.xCentimetersPerSecond < 0 )
                        robotCommand.xCentimetersPerSecond = 0;
                }
            }
         }

    // @mateusz: dostosowalem kod z mainklocki.cpp do walidatora, tam go usunalem
    float robotAngleRad = robot.rotationRadians;
    float cs = cos(robotAngleRad);
    float sn = sin(robotAngleRad);
    float tempX = robotCommand.xCentimetersPerSecond*cs - robotCommand.yCentimetersPerSecond*sn;
    float tempY = robotCommand.xCentimetersPerSecond * sn + robotCommand.yCentimetersPerSecond * cs;
    robotCommand.xCentimetersPerSecond = tempX;
    robotCommand.yCentimetersPerSecond = tempY;
    // @mateusz: koniec wstawionego kodu
    if(robotCommand.xCentimetersPerSecond > 1000.0){
        robotCommand.xCentimetersPerSecond = 1000;
    }
    if(robotCommand.yCentimetersPerSecond > 1000.0){
        robotCommand.yCentimetersPerSecond = 1000;
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

}
