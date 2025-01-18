#ifndef ROTATE_RIGHT_ACTION_H
#define ROTATE_RIGHT_ACTION_H

#include "RobotAction.h"
#include "RobotController.h"

class RotateLeftAction : public RobotAction {
public:
    RotateLeftAction(RobotController* controller) {
        setAction([controller]() {
            controller->setCurrentState(RobotController::ROTATING_LEFT);
            controller->rotateRight(Constants::DEFAULT_SPEED);
        });

        setCompletedCondition([controller]() {
            bool isFinished = controller->isSafeObstacle();
            if (!isFinished) return false;

            Serial.println("Finished rotating right.");
            return true;
        });
    }
};

#endif // ROTATE_RIGHT_ACTION_H
