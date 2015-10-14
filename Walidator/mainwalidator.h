#ifndef MAINWALIDATOR_H
#define MAINWALIDATOR_H

#include <QObject>
#include "Common-utils/datacollector.h"
#include "Common-utils/mapelements.h"

class MainWalidator : public QObject
{
    Q_OBJECT
private:
    typedef DataCollector<std::vector<Robot>, std::vector<ColorBox>, RobotCommands> MyDataCollector;
    MyDataCollector dataCollector;
    std::vector<cv::Point2i> boardCornersVec;
public:
    explicit MainWalidator();

private:
    RobotCommands correctionVelocityVector ( Robot robot, RobotCommands robotCommand );

signals:
    void robotCommandUpdateCorrected(RobotCommands robotCommands);

private slots:
    void processData();

public slots:
    void gameState(std::vector<Robot> robotVec, std::vector<ColorBox> colorBoxVec);
    void robotCommandUpdateRaw(RobotCommands robotCommands);
    void boardPos( std::vector<cv::Point2i> boardCorners);
};

#endif // MAINWALIDATOR_H
