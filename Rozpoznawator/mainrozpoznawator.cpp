#include "mainrozpoznawator.h"

const int FRAME_WIDTH = 1280;
const int FRAME_HEIGHT = 720;

using namespace  cv;
using namespace aruco;


MainRozpoznawator::MainRozpoznawator()
    : rythm(this), robotPositionX(30)
{
    this->cameraCapture.open(0);
    //set height and width of capture frame
    cameraCapture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    cameraCapture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

    //////////////////////// Crop2Board
    this->cameraCapture.read(cameraFrame);
    //findBoardPos();
    ////////////////////////

#ifdef DEBUG
    namedWindow(windowName, 1);
#endif
    /////////////////////// Setup for tracking boxes
    Block red_block("red_block"), green_block("green_block"),
            blue_block("blue_block"), yellow_block("yellow_block");

    this->block_types.push_back(red_block);
    this->block_types.push_back(green_block);
    this->block_types.push_back(blue_block);
    this-> block_types.push_back(yellow_block);
    ////////////////////////////////////////////////////////////////


    ////////////////////// Setup for tracking robots
    this->mDetector.setDesiredSpeed(3);
    ////////////////////////////////////////////////////////////////


    /**
     * Wystartowanie zegara z czasem 0 powoduje jego natychmiastowe
     * włączanie kiedy tylko nie ma żadnych innych zdarzeń
     * do przetworzenia w pętli.
     */
    QObject::connect(&rythm, &QTimer::timeout, this, &MainRozpoznawator::mainWork);
    rythm.start(0);
}

void MainRozpoznawator::updateRobotPosition()
{
    emit robotPositionUpdate(this->robotsPosition);

}

void MainRozpoznawator::updateBoxesPositions()
{

    emit boxesPositionUpdate(this->boxesPosition);
}

void MainRozpoznawator::findBoardPos(){

    vector<Marker> markers;
    this->mDetector.detect(this->cameraFrame,markers);

    Marker topLeft, topRight, bottomLeft, bottomRight;

    for (unsigned int i=0;i<markers.size();i++) {
        if( markers.at(i).id == this->topLeftBoardId ){
            topLeft = markers.at(i);
        }else if( markers.at(i).id == this->topRightBoardId ){
            topLeft = markers.at(i);
        }else if( markers.at(i).id == this->bottomLeftBoardId ){
            bottomLeft = markers.at(i);
        }else if( markers.at(i).id == this->bottomRightBoardId ){
            bottomRight = markers.at(i);
        }
    }


    this->minColRange = (topLeft[0].y < bottomLeft[0].y) ? topLeft[0].y : bottomLeft[0].y;
    this->minRowRange = (topLeft[0].x < topRight[1].x) ? topLeft[0].x : topRight[1].x;
    this->maxColRange = (topRight[1].y > bottomRight[2].y) ? topRight[1].y : bottomRight[2].y;
    this->maxRowRange = (bottomLeft[2].x > bottomRight[2].x) ? bottomLeft[2].x : bottomRight[2].x;
}

void MainRozpoznawator::crop2Board(){
    this->cameraFrame = this->cameraFrame.colRange(this->minColRange,this->maxColRange);
    this->cameraFrame = this->cameraFrame.rowRange(this->minRowRange,this->maxRowRange);
}

void MainRozpoznawator::findBoxes(){

    this->boxesPosition.clear();

    Mat cameraFeed = this->cameraFrame;
    Mat threshold;
    Mat HSV;

    vector<Block> all_blocks;



    flip(cameraFeed, cameraFeed, 1);

    // Run through all types of blocks and find them on the image
    cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
    for(unsigned int i = 0; i < this->block_types.size(); ++i){
        inRange(HSV, this->block_types.at(i).getHSVmin(), this->block_types.at(i).getHSVmax(),
                threshold);
        morphOps(threshold);
        trackFilteredObject(&all_blocks, this->block_types.at(i), threshold, HSV,
                cameraFeed);
    }



    for (unsigned int i = 0; i < all_blocks.size(); ++i) {

        ColorBoxPosition::Color color;
        if(all_blocks.at(i).getType()=="red_block"){

        color = ColorBoxPosition::RED;

        }else if(all_blocks.at(i).getType()=="green_block"){

        color = ColorBoxPosition::GREEN;

        }else if(all_blocks.at(i).getType()=="blue_block"){

        color = ColorBoxPosition::BLUE;

        }else if(all_blocks.at(i).getType()=="yellow_block"){

        }
        boxesPosition.push_back(ColorBoxPosition(all_blocks.at(i).getXPos(),all_blocks.at(i).getYPos(),color));
    }

    drawObject(all_blocks, drawFrame);



}

void MainRozpoznawator::morphOps(Mat &thresh) {

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

    erode(thresh, thresh, erodeElement);
    erode(thresh, thresh, erodeElement);

    dilate(thresh, thresh, dilateElement);
    dilate(thresh, thresh, dilateElement);
}



void MainRozpoznawator::trackFilteredObject(vector<Block>* blocks, Block theBlock, Mat threshold,
        Mat HSV, Mat &cameraFeed) {

    //max number of objects to be detected in frame
    const int MAX_NUM_OBJECTS = 50;
    //minimum object area
    int MIN_OBJECT_AREA = 20 * 20;

    Mat temp;
    threshold.copyTo(temp);
    //these two vectors needed for output of findContours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    findContours(temp, contours, hierarchy, CV_RETR_CCOMP,
            CV_CHAIN_APPROX_SIMPLE);
    //use moments method to find our filtered object

    if (hierarchy.size() > 0) {
        int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if (numObjects < MAX_NUM_OBJECTS) {
            for (int index = 0; index >= 0; index = hierarchy[index][0]) {

                Moments moment = moments((cv::Mat) contours[index]);
                double area = moment.m00;

                //if the area is less than 20 px by 20px then it is probably just noise
                //if the area is the same as the 3/2 of the image size, probably just a bad filter
                //we only want the object with the largest area so we safe a reference area each
                //iteration and compare it to the area in the next iteration.
                if (area > MIN_OBJECT_AREA) {

                    Block block;

                    block.setXPos(moment.m10 / area);
                    block.setYPos(moment.m01 / area);
                    block.setType(theBlock.getType());
                    block.setColour(theBlock.getColour());

                    blocks->push_back(block);

                }

            }

        } else
            putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50),
                    1, 2, Scalar(0, 0, 255), 2);
    }
}

void MainRozpoznawator::drawObject(vector<Block> theBlocks, Mat &frame) {

    for (unsigned int i = 0; i < theBlocks.size(); i++) {

        cv::circle(frame,
                cv::Point(theBlocks.at(i).getXPos(), theBlocks.at(i).getYPos()),
                10, cv::Scalar(0, 0, 255));
        cv::putText(frame,
                to_string(theBlocks.at(i).getXPos()) + " , "
                        + to_string(theBlocks.at(i).getYPos()),
                cv::Point(theBlocks.at(i).getXPos(),
                        theBlocks.at(i).getYPos() + 20), 1, 1,
                Scalar(0, 255, 0));
        cv::putText(frame, theBlocks.at(i).getType(),
                cv::Point(theBlocks.at(i).getXPos(),
                        theBlocks.at(i).getYPos() - 30), 1, 2,
                theBlocks.at(i).getColour());
    }
}

double calculate_angle(Marker marker) {

    cv::Point2f point1 = marker[1];
    cv::Point2f point2 = marker[2];
    //cout << point1 << "     " << point2 << endl;
    //cout << point1.y << "     " << point2.y << endl;
    double slope = (point1.x - point2.x) / (point1.y - point2.y);

    double angle_beta = atan(slope);

    if ((point1.x >= point2.x) && (point1.y < point2.y)) {			// 1 cwiartka
        return (-angle_beta);
    } else if ((point1.x >= point2.x) && (point1.y >= point2.y)) {  // 2 cwiartka
        return (-angle_beta + M_PI);
    } else if ((point1.x < point2.x) && (point1.y >= point2.y)) {   // 3 cwiartka
        return (-angle_beta + M_PI);
    } else {                                                        // 4 cwiartka
        return (-angle_beta + 2*M_PI);
    }
}

void MainRozpoznawator::findRobots() {
    //Detecting AR markers //
    vector<Marker> markers;
    this->mDetector.detect(this->cameraFrame,markers);
    double angle = 0;


    for (unsigned int i=0;i<markers.size();i++) {
        if( markers.at(i).id == this->robot1Id ){
            angle  = calculate_angle(markers[i]);
            this->robotsPosition.push_back(RobotPosition( markers.at(i).getCenter().x, markers.at(i).getCenter().y, angle));
#ifdef DEBUG
            putText(this->drawFrame, to_string(180 * angle / M_PI),    //draw marker info and its boundaries in the image
                    cv::Point(markers[i][0].x,
                            markers[i][0].y), 1, 3,
                    Scalar(0, 0, 255),3);
            markers[i].draw(this->drawFrame,Scalar(0,0,255),2);
#endif;
        } else if( markers.at(i).id == this->robot2Id ){
            angle  = calculate_angle(markers[i]);
            this->robotsPosition.push_back(RobotPosition( markers.at(i).getCenter().x, markers.at(i).getCenter().y, angle));
#ifdef DEBUG
            putText(this->drawFrame, to_string(180 * angle / M_PI),    //draw marker info and its boundaries in the image
                    cv::Point(markers[i][0].x,
                            markers[i][0].y), 1, 3,
                    Scalar(0, 0, 255),3);
            markers[i].draw(this->drawFrame,Scalar(0,0,255),2);
#endif;
        }

    }

}

void MainRozpoznawator::mainWork()
{
  //  int sleepTime = 100 + qrand()%900;
  //  QThread::msleep(sleepTime);

    this->cameraCapture.read(this->cameraFrame);
    //crop2Board();
#ifdef DEBUG
    this->drawFrame = this->cameraFrame;
#endif
    findBoxes();
    findRobots();

#ifdef DEBUG
    imshow(windowName, this->drawFrame);
#endif

   // updateRobotPosition();
   // updateBoxesPositions();
}

