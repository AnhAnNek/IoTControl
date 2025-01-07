#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include "SensorManager.h"
#include "MotorController.h"

class RobotController {
public:
    RobotController(SensorManager& sensorManager, MotorController& motorController);

    void moveForward(int speed);
    void moveBackward(int speed);
    void rotateLeft(int speed);
    void rotateRight(int speed);
    void stop();

    void setBrushSpeeds(int sideBrushSpeed, int mainBrushSpeed);
    void stopBrushes();

private:
    SensorManager& sensorManager;
    MotorController& motorController;
};

#endif // ROBOTCONTROLLER_H
