#include "RobotController.h"
#include <Arduino.h>
#include "Utils.h"
#include "RobotAction.h"

RobotController::RobotController(SensorManager& sensorManager, MotorController& motorController)
    : sensorManager(sensorManager), motorController(motorController) 
{
  _actionQueue = xQueueCreate(5, sizeof(RobotAction*));
}

void RobotController::begin()
{
  sensorManager.begin();
  motorController.begin();
}

// Add a RobotAction to the queue
void RobotController::pushAction(RobotAction* action) {
	if (xQueueSend(_actionQueue, &action, 0) != pdTRUE) {
		Serial.println("Failed to push RobotAction to queue");
	}
}

// Start executing actions
void RobotController::executeAction() {
	if (_currentAction == nullptr && uxQueueMessagesWaiting(_actionQueue) == 0) 
	{
		Serial.println("There are not any actions");
		return;
	}

	if (_currentAction == nullptr && uxQueueMessagesWaiting(_actionQueue) > 0) 
    {
		xQueueReceive(_actionQueue, &_currentAction, 0);
        _currentAction->execute();
		Serial.println("Starting new action");
	}

    if (_currentAction->isCompleted()) 
    {
        Serial.println("Current action completed");
        delete _currentAction;
        _currentAction = nullptr;  // Reset current action after completion
    }
}

void RobotController::moveForward(int speed) {
  motorController.setMovingSpeeds(speed, speed);
  motorController.moveForward();
}

void RobotController::moveBackward(int speed) {
  motorController.setMovingSpeeds(speed, speed);
  motorController.moveBackward();
}

void RobotController::rotateLeft(int speed) {
  motorController.setMovingSpeeds(speed, speed);
  motorController.rotateLeft();
}

void RobotController::rotateRight(int speed) {
  motorController.setMovingSpeeds(speed, speed);
  motorController.rotateRight();
}

void RobotController::stop() {
  motorController.stop();
}

void RobotController::setBrushSpeeds(int sideBrushSpeed, int mainBrushSpeed) {
  motorController.setBrushSpeeds(sideBrushSpeed, mainBrushSpeed);
}

void RobotController::stopBrushes() {
  motorController.stopBrushes();
}

void RobotController::stopSideBrush()
{
  motorController.setSideBrushSpeed(0);
}

void RobotController::stopMainBrush()
{
  motorController.setMainBrushSpeed(0);
}

void RobotController::startSideBrush(int speed)
{
  motorController.setSideBrushSpeed(speed);
}

void RobotController::startMainBrush(int speed)
{
  motorController.setMainBrushSpeed(speed);
}

void RobotController::setMovingSpeeds(int leftSpeed, int rightSpeed)
{
  motorController.setMovingSpeeds(leftSpeed, rightSpeed);
}

void RobotController::handleAutoMode(unsigned long currentMillis)
{
	if (_currentAction != nullptr || uxQueueMessagesWaiting(_actionQueue) > 0) {
        this->executeAction();
		return;
	}

    const RobotController::State currentState = getCurrentState();
    if (currentState != IDLE)
    {
        return;
    }

    // Store sensor readings in boolean variables
    long frontDistance = sensorManager.getFrontObstacleDistance();
    long leftDistance = sensorManager.getLeftObstacleDistance();
    long rightDistance = sensorManager.getRightObstacleDistance();
    bool isFrontObstacle = MethodUtils::isObstacle(frontDistance);
    bool isLeftObstacle = MethodUtils::isObstacle(leftDistance);
    bool isRightObstacle = MethodUtils::isObstacle(rightDistance);
    bool isNearStairs = sensorManager.isNearStairs();

    // Handle obstacle scenarios
    if (isNearStairs) {
        handleNearStairs();
    } else if (isFrontObstacle && isLeftObstacle && isRightObstacle) {
        handleAllSidesObstacle();
    } else if (isFrontObstacle && isLeftObstacle) {
        handleFrontLeftObstacle();
    } else if (isFrontObstacle && isRightObstacle) {
        handleFrontRightObstacle();
    } else if (isFrontObstacle) {
        handleFrontObstacle();
    } else if (isLeftObstacle) {
        handleLeftObstacle();
    } else if (isRightObstacle) {
        handleRightObstacle();
    } else {
        handleNoObstacle(currentMillis);
    }
}

void RobotController::handleNearStairs()
{
    Serial.println("Stairs detected on the front!");

    RobotAction* stop = createAction(
        [this]() {
            this->setCurrentState(RobotController::STOPPING);
            this->stop();
        },
        []() {
            Serial.println("Stopped.");
            return true;
        }
    );

    RobotAction* moveBackward = createAction(
        [this]() {
            this->setCurrentState(RobotController::MOVING_BACKWARD);
            this->moveBackward(Constants::DEFAULT_SPEED);
        },
        []() { return true; }
    );

    pushAction(stop);
    pushAction(moveBackward);
}

void RobotController::handleAllSidesObstacle()
{
    Serial.println("Obstacles detected on all sides! Moving backward...");

    RobotAction* movingBackward = createAction(
        [this]() {
            this->setCurrentState(RobotController::MOVING_BACKWARD);
            this->moveBackward(Constants::DEFAULT_SPEED);
        },
        [this]() {
            bool isFinished = isSafeObstacle();
            if (!isFinished) return false;

            Serial.println("Finished moving backward.");
            return true;
        }
    );

    pushAction(movingBackward);
}

void RobotController::handleFrontLeftObstacle()
{
    Serial.println("Obstacles detected on the front and left sides! Moving backward and turning right...");

    RobotAction* moveBackward = createAction(
        [this]() {
            this->setCurrentState(RobotController::MOVING_BACKWARD_AND_ROTATING_RIGHT);
            this->moveBackward(Constants::DEFAULT_SPEED);
        },
        [this]() { return this->isSafeObstacle(); }
    );

    RobotAction* rotateRight = createAction(
        [this]() { this->setCurrentState(RobotController::ROTATING_RIGHT); this->rotateRight(Constants::DEFAULT_SPEED); },
        [this]() { return this->isSafeObstacle(); }
    );

    pushAction(moveBackward);
    pushAction(rotateRight);
}

void RobotController::handleFrontRightObstacle()
{
    Serial.println("Obstacles detected on the front and right sides! Moving backward and turning left...");

    RobotAction* moveBackward = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::MOVING_BACKWARD_AND_ROTATING_LEFT);
            this->moveBackward(Constants::DEFAULT_SPEED);
        })
        .setCompletedCondition([this]() { 
            return this->isSafeObstacle(); 
        })
        .build();

    RobotAction* rotateLeft = createAction(
        [this]() { this->setCurrentState(RobotController::ROTATING_LEFT); this->rotateLeft(Constants::DEFAULT_SPEED); },
        [this]() { return this->isSafeObstacle(); }
    );

    pushAction(moveBackward);
    pushAction(rotateLeft);
}

void RobotController::handleFrontObstacle()
{
    Serial.println("Obstacle detected in front!");

    RobotAction* stop = createAction(
        [this]() {
            this->setCurrentState(RobotController::STOPPING);
            this->stop();
        },
        []() {
            Serial.println("Stopped.");
            return true;
        }
    );

    RobotAction* rotateRight = createAction(
        [this]() {
            this->setCurrentState(RobotController::ROTATING_RIGHT);
            this->rotateRight(Constants::DEFAULT_SPEED);
        },
        [this]() {
            return TimerUtils::hasDurationElapsed(_previousMillis, millis(), Constants::STOP_DURATION);
        }
    );

    pushAction(stop);
    pushAction(rotateRight);
}

void RobotController::handleLeftObstacle()
{
    Serial.println("Obstacle detected on the left!");

    RobotAction* stop = createAction(
        [this]() {
            this->setCurrentState(RobotController::STOPPING);
            this->stop();
        },
        []() {
            Serial.println("Stopped.");
            return true;
        }
    );

    RobotAction* rotateRight = createAction(
        [this]() {
            this->setCurrentState(RobotController::ROTATING_RIGHT);
            this->rotateRight(Constants::DEFAULT_SPEED);
        },
        [this]() {
            return this->isSafeObstacle();
        }
    );

    pushAction(stop);
    pushAction(rotateRight);
}

void RobotController::handleRightObstacle()
{
    Serial.println("Obstacle detected on the right!");

    RobotAction* stop = new RobotAction(
        [this]() {
            this->setCurrentState(RobotController::STOPPING);
            this->stop();
        },
        []() {
            Serial.println("Stopped.");
            return true;
        }
    );

    RobotAction* rotateLeft = new RobotAction(
        [this]() {
            this->setCurrentState(RobotController::ROTATING_LEFT);
            this->rotateLeft(Constants::DEFAULT_SPEED);
        },
        [this]() {
            return this->isRightObstacleDistanceSafe();
        }
    );

    pushAction(stop);
    pushAction(rotateLeft);
}

void RobotController::handleNoObstacle(unsigned long currentMillis)
{
    Serial.println("Moving forward...");
    transitionToState(RobotController::MOVING_FORWARD, currentMillis);
    this->moveForward(Constants::DEFAULT_SPEED);
}

RobotController::State RobotController::getCurrentState() const {
  return _currentState;
}

// Setter for _currentState
void RobotController::setCurrentState(RobotController::State newState) {
  _currentState = newState;
}

bool RobotController::isAutoMode() const
{
  return _autoMode;
}

void RobotController::setAutoMode(bool autoMode)
{
  _autoMode = autoMode;
}

void RobotController::transitionToState(RobotController::State newState, unsigned long currentMillis) {
    this->setCurrentState(newState);
    _previousMillis = currentMillis;
}

bool RobotController::isSafeObstacle()
{
    return isFrontObstacleDistanceSafe()
        && isLeftObstacleDistanceSafe()
        && isRightObstacleDistanceSafe();
}

bool RobotController::isFrontObstacleDistanceSafe()
{
	long frontDistance = this->sensorManager.getFrontObstacleDistance();
    return MethodUtils::isSafeDistance(frontDistance);
}

bool RobotController::isLeftObstacleDistanceSafe()
{
	long leftDistance = this->sensorManager.getLeftObstacleDistance();
    return MethodUtils::isSafeDistance(leftDistance);
}

bool RobotController::isRightObstacleDistanceSafe()
{
	long rightDistance = this->sensorManager.getRightObstacleDistance();
    return MethodUtils::isSafeDistance(rightDistance);
}