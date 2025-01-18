#ifndef MOVE_BACKWARD_ACTION_H
#define MOVE_BACKWARD_ACTION_H

#include "RobotAction.h"
#include "RobotController.h"

class MoveBackwardAction : public RobotAction {
public:
    MoveBackwardAction(RobotController* controller) {
        setAction([controller]() {
            controller->setCurrentState(RobotController::MOVING_BACKWARD);
            controller->moveBackward(Constants::DEFAULT_SPEED);
        });

        setCompletedCondition([controller]() {
            bool isFinished = controller->isSafeObstacle();
            if (!isFinished) return false;

            Serial.println("Finished moving backward.");
            return true;
        });
    }
};

#endif // MOVE_BACKWARD_ACTION_H
