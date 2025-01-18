#ifndef ACTION_FACTORY_H
#define ACTION_FACTORY_H

#include "MoveBackwardAction.h"
#include "StopAction.h"
#include "RotateRightAction.h"

class ActionFactory {
public:
    static RobotAction* createMoveBackwardAction(RobotController* controller) {
        return new MoveBackwardAction(controller);
    }

    static RobotAction* createStopAction(RobotController* controller) {
        return new StopAction(controller);
    }

    static RobotAction* createRotateRightAction(RobotController* controller) {
        return new RotateLeftAction(controller);
    }
};

#endif // ACTION_FACTORY_H
