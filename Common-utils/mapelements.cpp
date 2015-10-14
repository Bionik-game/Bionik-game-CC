#include "mapelements.h"

/**
 * Poniższa funkcja musi być wywołana, aby klasy mogły być przesyłane w sygnałach
 */
void registerMetaTypes()
{
    static bool unregistered = true;
    if (unregistered)
    {
        qRegisterMetaType<Robot>("Robot");
        qRegisterMetaType<ColorBox>("ColorBox");
        qRegisterMetaType<RobotCommands>("RobotCommands");
        qRegisterMetaType<BoardIndicator>("BoardIndicator");
        qRegisterMetaType<RobotGoal>("RobotGoal");
        qRegisterMetaType<std::vector<Robot>>("std::vector<Robot>");
        qRegisterMetaType<std::vector<ColorBox>>("std::vector<ColorBox>");
        qRegisterMetaType<std::vector<RobotCommands>>("std::vector<RobotCommands>");
        qRegisterMetaType<std::vector<BoardIndicator>>("std::vector<BoardIndicator>");
        qRegisterMetaType<std::vector<RobotGoal>>("std::vector<RobotGoal>");
        qRegisterMetaType<cv::Mat>("cv::Mat");
        qRegisterMetaType<std::vector<cv::Point2i>>("std::vector<cv::Point2i");

        unregistered = false;
    }
}
