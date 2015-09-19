#include "block.h"


std::fstream& GotoLine(std::fstream& file, unsigned int num){
    file.seekg(std::ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}

void readValues(std::fstream& file,  int* storage){

    //fstream file;
    //file.open("colour_values.txt", ios::in);


    for(int i=0;i < 6; ++i){
        file >> storage[i];
//		cout << storage [i];
    }

}

Block::Block()
{
    //set values for default constructor
    setType("null");
    setColour(Scalar(0,0,0));

}

Block::Block(string name){

    setType(name);
    fstream file;
    file.open("colour_data.txt", ios::in);
    if(!file.is_open()){
        std::cout << "File not found" << endl;
    }

    int colour_values[6];

    if(name=="red_block"){

        GotoLine(file, 1);


        //BGR value for Red:
        setColour(Scalar(0 ,0,255));

    }else if(name=="green_block"){

        //TODO: use "calibration mode" to find HSV min
        //and HSV max values

//	setHSVmin(Scalar(69,56,18));
//		setHSVmax(Scalar(83,255,181));
        GotoLine(file, 7);

        //BGR value for Green:
        setColour(Scalar(0,255,0));

    }else if(name=="blue_block"){

        //TODO: use "calibration mode" to find HSV min
        //and HSV max values

//		setHSVmin(Scalar(97,171,20));
//		setHSVmax(Scalar(169,256,243));
        GotoLine(file, 13);

        //BGR value for Blue:
        setColour(Scalar(255,0,0));

    }


    readValues( file,colour_values);

    setHSVmin(Scalar(colour_values[0],colour_values[1],colour_values[2]));
    setHSVmax(Scalar(colour_values[3],colour_values[4],colour_values[5]));
  //  cout << colour_values[0] << " " << colour_values[1] << " " << colour_values[2] << endl;
    file.close();
}

Block::Block(string name, Scalar minValues, Scalar maxValues){

    setType(name);

    setHSVmin(minValues);
    setHSVmax(maxValues);


}

Block::~Block(void)
{
}

int Block::getXPos(){

    return Block::xPos;

}

void Block::setXPos(int x){

    Block::xPos = x;

}

int Block::getYPos(){

    return Block::yPos;

}

void Block::setYPos(int y){

    Block::yPos = y;

}

Scalar Block::getHSVmin(){

    return Block::HSVmin;

}
Scalar Block::getHSVmax(){

    return Block::HSVmax;
}

void Block::setHSVmin(Scalar min){

    Block::HSVmin = min;
}


void Block::setHSVmax(Scalar max){

    Block::HSVmax = max;
}

double Block::getArea() const
{
    return area;
}

void Block::setArea(double value)
{
    area = value;
}
