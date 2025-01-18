#ifndef STOP_ACTION_H
#define STOP_ACTION_H

#include "RobotAction.h"
#include "RobotController.h"

class StopAction : public RobotAction {
public:
    StopAction(RobotController* controller) {
        setAction([controller]() {
            controller->setCurrentState(RobotController::STOPPING);
            controller->stop();
        });

        setCompletedCondition([]() {
            Serial.println("Stopped.");
            return true;
        });
    }
};

#endif // STOP_ACTION_H
