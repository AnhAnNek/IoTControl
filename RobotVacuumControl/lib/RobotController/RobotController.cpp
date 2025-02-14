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
void RobotController::pushAction(std::unique_ptr<RobotAction> action) {
    RobotAction* rawAction = action.release();
	if (xQueueSend(_actionQueue, &rawAction, 0) != pdTRUE) {
		Serial.println("Failed to push RobotAction to queue");
        delete rawAction;
	}
}

// Start executing actions
void RobotController::executeAction() {
    RobotAction* rawAction = nullptr;
	if (!_currentAction && uxQueueMessagesWaiting(_actionQueue) > 0) 
    {
		xQueueReceive(_actionQueue, &rawAction, 0);
        _currentAction.reset(rawAction);
	}

    if (!_currentAction)
    {
		Serial.println("There are not any actions");
        return;
    }

    Serial.println("Starting new action");
    _currentAction->execute();

    if (_currentAction->isCompleted()) 
    {
        Serial.println("Current action completed");
        _currentAction.reset();
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

    std::unique_ptr<RobotAction> stop = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::STOPPING);
            this->stop();
        })
        .setCompletedCondition([]() {
            Serial.println("Stopped.");
            return true;
        })
        .build();

    std::unique_ptr<RobotAction> moveBackward = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::MOVING_BACKWARD);
            this->moveBackward(Constants::DEFAULT_SPEED);
        })
        .setCompletedCondition([]() {
            return true;
        })
        .build();

    pushAction(std::move(stop));
    pushAction(std::move(moveBackward));
}

void RobotController::handleAllSidesObstacle()
{
    Serial.println("Obstacles detected on all sides! Moving backward...");

    std::unique_ptr<RobotAction> movingBackward = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::MOVING_BACKWARD);
            this->moveBackward(Constants::DEFAULT_SPEED);
        })
        .setCompletedCondition([this]() {
            bool isFinished = isSafeObstacle();
            if (!isFinished) return false;

            Serial.println("Finished moving backward.");
            return true;
        })
        .build();

    pushAction(std::move(movingBackward));
}

void RobotController::handleFrontLeftObstacle()
{
    Serial.println("Obstacles detected on the front and left sides! Moving backward and turning right...");

    std::unique_ptr<RobotAction> moveBackward = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::MOVING_BACKWARD_AND_ROTATING_RIGHT);
            this->moveBackward(Constants::DEFAULT_SPEED);
        })
        .setCompletedCondition([this]() { return this->isSafeObstacle(); })
        .build();

    std::unique_ptr<RobotAction> rotateRight = RobotAction::Builder()
        .setAction([this]() { 
            this->setCurrentState(RobotController::ROTATING_RIGHT); 
            this->rotateRight(Constants::DEFAULT_SPEED); 
        })
        .setCompletedCondition([this]() { 
            return this->isSafeObstacle(); 
        })
        .build();

    pushAction(std::move(moveBackward));
    pushAction(std::move(rotateRight));
}

void RobotController::handleFrontRightObstacle()
{
    Serial.println("Obstacles detected on the front and right sides! Moving backward and turning left...");

    std::unique_ptr<RobotAction> moveBackward = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::MOVING_BACKWARD_AND_ROTATING_LEFT);
            this->moveBackward(Constants::DEFAULT_SPEED);
        })
        .setCompletedCondition([this]() { 
            return this->isSafeObstacle(); 
        })
        .build();

    std::unique_ptr<RobotAction> rotateLeft = RobotAction::Builder()
        .setAction([this]() { 
            this->setCurrentState(RobotController::ROTATING_LEFT); 
            this->rotateLeft(Constants::DEFAULT_SPEED); 
        })
        .setCompletedCondition([this]() { 
            return this->isSafeObstacle(); 
        })
        .build();

    pushAction(std::move(moveBackward));
    pushAction(std::move(rotateLeft));
}

void RobotController::handleFrontObstacle()
{
    Serial.println("Obstacle detected in front!");

    std::unique_ptr<RobotAction> stop = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::STOPPING);
            this->stop();
        })
        .setCompletedCondition([]() {
            Serial.println("Stopped.");
            return true;
        })
        .build();

    std::unique_ptr<RobotAction> rotateRight = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::ROTATING_RIGHT);
            this->rotateRight(Constants::DEFAULT_SPEED);
        })
        .setCompletedCondition([this]() {
            return TimerUtils::hasDurationElapsed(_previousMillis, millis(), Constants::STOP_DURATION);
        })
        .build();

    pushAction(std::move(stop));
    pushAction(std::move(rotateRight));
}

void RobotController::handleLeftObstacle()
{
    Serial.println("Obstacle detected on the left!");

    std::unique_ptr<RobotAction> stop = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::STOPPING);
            this->stop();
        })
        .setCompletedCondition([]() {
            Serial.println("Stopped.");
            return true;
        })
        .build();

    std::unique_ptr<RobotAction> rotateRight = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::ROTATING_RIGHT);
            this->rotateRight(Constants::DEFAULT_SPEED);
        })
        .setCompletedCondition([this]() {
            return this->isSafeObstacle();
        })
        .build();

    pushAction(std::move(stop));
    pushAction(std::move(rotateRight));
}

void RobotController::handleRightObstacle()
{
    Serial.println("Obstacle detected on the right!");

    std::unique_ptr<RobotAction> stop = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::STOPPING);
            this->stop();
        })
        .setCompletedCondition([]() {
            Serial.println("Stopped.");
            return true;
        })
        .build();

    std::unique_ptr<RobotAction> rotateLeft = RobotAction::Builder()
        .setAction([this]() {
            this->setCurrentState(RobotController::ROTATING_LEFT);
            this->rotateLeft(Constants::DEFAULT_SPEED);
        })
        .setCompletedCondition([this]() {
            return this->isRightObstacleDistanceSafe();
        })
        .build();

    pushAction(std::move(stop));
    pushAction(std::move(rotateLeft));
}

void RobotController::handleNoObstacle(unsigned long currentMillis)
{
    Serial.println("Moving forward...");

    std::unique_ptr<RobotAction> stop = RobotAction::Builder()
        .setAction([this, currentMillis]() {
            transitionToState(RobotController::MOVING_FORWARD, currentMillis);
            this->moveForward(Constants::DEFAULT_SPEED);
        })
        .setCompletedCondition([this]() {
            return !this->isSafeObstacle();
        })
        .build();

    pushAction(std::move(stop));
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