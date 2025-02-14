#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include "SensorManager.h"
#include "MotorController.h"
#include "RobotAction.h"

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

    static const char* stateToString(State state) {
        switch (state) {
            case IDLE: return "IDLE";
            case ROTATING_RIGHT: return "ROTATING_RIGHT";
            case ROTATING_LEFT: return "ROTATING_LEFT";
            case MOVING_FORWARD: return "MOVING_FORWARD";
            case MOVING_BACKWARD: return "MOVING_BACKWARD";
            case STOPPING: return "STOPPING";
            case MOVING_BACKWARD_AND_ROTATING_RIGHT: return "MOVING_BACKWARD_AND_ROTATING_RIGHT";
            case MOVING_BACKWARD_AND_ROTATING_LEFT: return "MOVING_BACKWARD_AND_ROTATING_LEFT";
            default: return "UNKNOWN";
        }
    }

    RobotController(SensorManager& sensorManager, MotorController& motorController);

    ~RobotController()
    {
        RobotAction* rawAction = nullptr;
        while (uxQueueMessagesWaiting(_actionQueue) > 0) {
            xQueueReceive(_actionQueue, &rawAction, 0);
            delete rawAction; // Clean up any remaining actions
        }
        vQueueDelete(_actionQueue); // Delete the queue
    }

    void pushAction(std::unique_ptr<RobotAction> action);
    void executeAction();

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

    void handleAutoMode(unsigned long currentMillis);
    
    bool isSafeObstacle();
    bool isFrontObstacleDistanceSafe();
    bool isLeftObstacleDistanceSafe();
    bool isRightObstacleDistanceSafe();

    RobotController::State getCurrentState() const;
    void setCurrentState(RobotController::State newState);

    bool isAutoMode() const;
    void setAutoMode(bool autoMode);
private:
    SensorManager& sensorManager;
    MotorController& motorController;

    RobotController::State _currentState = IDLE;
    bool _autoMode = true;

    xQueueHandle _actionQueue;
    std::unique_ptr<RobotAction> _currentAction = nullptr;

    unsigned long _previousMillis = 0;

    void transitionToState(RobotController::State newState, unsigned long currentMillis);

    void handleNearStairs();
    void handleAllSidesObstacle();
    void handleFrontLeftObstacle();
    void handleFrontRightObstacle();
    void handleFrontObstacle();
    void handleLeftObstacle();
    void handleRightObstacle();
    void handleNoObstacle(unsigned long currentMillis);
};

#endif // ROBOTCONTROLLER_H
