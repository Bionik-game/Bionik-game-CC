#ifndef BLOCK_H
#define BLOCK_H
#include <sstream>
#include <fstream>
#include <limits>
#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <vector>
using namespace std;
using namespace cv;



class Block
{
public:
    Block();
    ~Block(void);

    Block(string name);
    Block(string name, Scalar minValues, Scalar maxValues);

    int getXPos();
    void setXPos(int x);

    int getYPos();
    void setYPos(int y);

    Scalar getHSVmin();
    Scalar getHSVmax();

    void setHSVmin(Scalar min);
    void setHSVmax(Scalar max);

    string getType(){return type;}
    void setType(string t){type = t;}

    Scalar getColour(){
        return Colour;
    }
    void setColour(Scalar c){

        Colour = c;
    }

    double getArea() const;

    void setArea(double value);

private:

    int xPos, yPos;
    double area;
    string type;
    Scalar HSVmin, HSVmax;
    Scalar Colour;
};

#endif
