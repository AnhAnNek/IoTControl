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

	if (_currentAction == nullptr && uxQueueMessagesWaiting(_actionQueue) > 0) {
		// Get the next action from the queue
		xQueueReceive(_actionQueue, &_currentAction, 0);
		Serial.println("Starting new action");
	}

	if (_currentAction != nullptr) {
		// Execute the current action
		_currentAction->execute();

		// Check if the current action is completed
		if (_currentAction->isCompleted()) {
			Serial.println("Current action completed");
			_currentAction = nullptr;  // Reset current action after completion
		}
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
		return;
	}

    const RobotController::State currentState = getCurrentState();
    switch (currentState) {
        case RobotController::IDLE: {
            // Store sensor readings in boolean variables
            long frontDistance = sensorManager.getFrontObstacleDistance();
            long leftDistance = sensorManager.getLeftObstacleDistance();
            long rightDistance = sensorManager.getRightObstacleDistance();
            bool isFrontObstacle = MethodUtils::isObstacle(frontDistance);
            bool isLeftObstacle = MethodUtils::isObstacle(leftDistance);
            bool isRightObstacle = MethodUtils::isObstacle(rightDistance);
            bool isNearStairs = sensorManager.isNearStairs();

            if ((isFrontObstacle && isLeftObstacle && isRightObstacle) || (isLeftObstacle && isRightObstacle))
            {
                Serial.println("Obstacles detected on all sides! Moving backward...");
                transitionToState(RobotController::MOVING_BACKWARD, currentMillis);

                RobotAction* movingBackward = new RobotAction();
                movingBackward->setAction([this]() {
					this->moveBackward(Constants::DEFAULT_SPEED);
                });
                movingBackward->setCompletedCondition([this]() {
					bool isFinished = isSafeObstacle();
					if (!isFinished) return false;

					Serial.println("Finished moving backward.");
					return true;
                });

				pushAction(movingBackward);
            } 
            else if (isFrontObstacle && isLeftObstacle) 
            {
                Serial.println("Obstacles detected on the front and left sides! Moving backward and turning right...");
                transitionToState(RobotController::MOVING_BACKWARD_AND_ROTATING_RIGHT, currentMillis);
            }
            else if (isFrontObstacle && isRightObstacle) 
            {
                Serial.println("Obstacles detected on the front and right sides! Moving backward and turning left...");
                transitionToState(RobotController::MOVING_BACKWARD_AND_ROTATING_LEFT, currentMillis);
            }
            else if (isFrontObstacle) 
            {
                Serial.println("Obstacle detected in front!");
                stop();
                transitionToState(RobotController::ROTATING_RIGHT, currentMillis);

                RobotAction* stop = new RobotAction();
                stop->setAction([this]() {
					this->stop();
                });
                stop->setCompletedCondition([this]() {
					bool isFinished = TimerUtils::hasDurationElapsed(_previousMillis, currentMillis, Constants::STOP_DURATION);
					if (!isFinished) return false;

					Serial.println("Finished moving backward.");
					return true;
                });

                RobotAction* rotateRight = new RobotAction();
                rotateRight->setAction([this]() {
					this->rotateRight(Constants::DEFAULT_SPEED);
                });
                rotateRight->setCompletedCondition([this]() {
					bool isFinished = TimerUtils::hasDurationElapsed(_previousMillis, currentMillis, Constants::STOP_DURATION);
					if (!isFinished) return false;

					Serial.println("Finished moving backward.");
					return true;
                });

				pushAction(stop);
				pushAction(rotateRight);
            } 
            else if (isLeftObstacle) 
            {
                Serial.println("Obstacle detected on the left!");
                stop();
                transitionToState(RobotController::ROTATING_RIGHT, currentMillis);
            } 
            else if (isRightObstacle) 
            {
                Serial.println("Obstacle detected on the right!");
                stop();
                transitionToState(RobotController::ROTATING_LEFT, currentMillis);
            } 
            else if (isNearStairs) 
            {
                Serial.println("Stairs detected on the front!");
                stop();
                transitionToState(RobotController::MOVING_BACKWARD, currentMillis);
            }
            else 
            {
                Serial.println("Moving forward...");
                transitionToState(RobotController::MOVING_FORWARD, currentMillis);
            }
            break; // End of IDLE case
        }

        case RobotController::ROTATING_RIGHT:
            if (TimerUtils::hasDurationElapsed(_previousMillis, currentMillis, Constants::ROTATION_DURATION)) {
                Serial.println("Finished rotating right.");
                transitionToState(RobotController::IDLE, currentMillis);
            } else {
                rotateRight(Constants::DEFAULT_SPEED);
            }
            break;

        case RobotController::ROTATING_LEFT:
            if (TimerUtils::hasDurationElapsed(_previousMillis, currentMillis, Constants::ROTATION_DURATION)) {
                Serial.println("Finished rotating left.");
                transitionToState(RobotController::IDLE, currentMillis);
            } else {
                rotateLeft(Constants::DEFAULT_SPEED);
            }
            break;

        case RobotController::MOVING_FORWARD:
            if (TimerUtils::hasDurationElapsed(_previousMillis, currentMillis, Constants::MOVING_DURATION)) {
                Serial.println("Finished moving forward.");
                transitionToState(RobotController::IDLE, currentMillis);
            } else {
                moveForward(Constants::DEFAULT_SPEED);
            }
            break;

        case RobotController::MOVING_BACKWARD:
            if (TimerUtils::hasDurationElapsed(_previousMillis, currentMillis, Constants::BACK_DURATION)) {
                Serial.println("Finished moving backward.");
                transitionToState(RobotController::IDLE, currentMillis);
            } else {
                moveBackward(Constants::DEFAULT_SPEED);
            }
            break;

        case RobotController::STOPPING:
            if (TimerUtils::hasDurationElapsed(_previousMillis, currentMillis, Constants::STOP_DURATION)) {
                Serial.println("Finished stopping.");
                transitionToState(RobotController::IDLE, currentMillis);
            }
            break;
        
        case RobotController::MOVING_BACKWARD_AND_ROTATING_RIGHT:
            if (TimerUtils::hasDurationElapsed(_previousMillis, currentMillis, Constants::BACK_DURATION)) {
                Serial.println("Finished moving backward, now rotating right.");
                transitionToState(RobotController::ROTATING_RIGHT, currentMillis); // Transition to rotating right after moving backward
            } else {
                moveBackward(Constants::DEFAULT_SPEED);
            }
            break;

        case RobotController::MOVING_BACKWARD_AND_ROTATING_LEFT: 
            if (TimerUtils::hasDurationElapsed(_previousMillis, currentMillis, Constants::BACK_DURATION)) {
                Serial.println("Finished moving backward, now rotating left.");
                transitionToState(RobotController::ROTATING_LEFT, currentMillis); // Transition to rotating right after moving backward
            } else {
                moveBackward(Constants::DEFAULT_SPEED);
            }
            break;
    }
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
	long frontDistance = this->sensorManager.getFrontObstacleDistance();
	long leftDistance = this->sensorManager.getLeftObstacleDistance();
	long rightDistance = this->sensorManager.getRightObstacleDistance();
	return frontDistance > Constants::SAFE_OBSTACLE_CM 
		&& leftDistance > Constants::SAFE_OBSTACLE_CM 
		&& rightDistance > Constants::SAFE_OBSTACLE_CM;
}