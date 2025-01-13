#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include "SensorManager.h"
#include "MotorController.h"

class RobotController {
public:
    enum State {
        IDLE,
        ROTATING_RIGHT,
        ROTATING_LEFT,
        MOVING_FORWARD,
        MOVING_BACKWARD,
        STOPPING,
        MOVING_BACKWARD_AND_ROTATING_RIGHT,
        MOVING_BACKWARD_AND_ROTATING_LEFT
    };


    RobotController(SensorManager& sensorManager, MotorController& motorController);

    void begin();
    void moveForward(int speed);
    void moveBackward(int speed);
    void rotateLeft(int speed);
    void rotateRight(int speed);
    void stop();

    void setMovingSpeeds(int leftSpeed, int rightSpeed);
    void setBrushSpeeds(int sideBrushSpeed, int mainBrushSpeed);
    void stopBrushes();

    void stopSideBrush();
    void stopMainBrush();
    void startSideBrush(int speed);
    void startMainBrush(int speed);

    RobotController::State getCurrentState() const;
    void setCurrentState(RobotController::State newState);

    bool isAutoMode();
    void setAutoMode(bool autoMode);
private:
    SensorManager& sensorManager;
    MotorController& motorController;

    RobotController::State _currentState = IDLE;
    bool _autoMode = true;
};

#endif // ROBOTCONTROLLER_H
