#include "RobotController.h"
#include "Utils.h"

RobotController::RobotController(SensorManager& sensorManager, MotorController& motorController)
    : sensorManager(sensorManager), motorController(motorController) {}

void RobotController::begin()
{
  sensorManager.begin();
  motorController.begin();
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
    switch (currentState) {
        case RobotController::IDLE: {
            // Store sensor readings in boolean variables
            bool isFrontObstacle = sensorManager.isObstacleFront();
            bool isLeftObstacle = sensorManager.isObstacleLeft();
            bool isRightObstacle = sensorManager.isObstacleRight();
            bool isNearStairs = sensorManager.isNearStairs();

            if ((isFrontObstacle && isLeftObstacle && isRightObstacle) || (isLeftObstacle && isRightObstacle))
            {
                Serial.println("Obstacles detected on all sides! Moving backward...");
                transitionToState(RobotController::MOVING_BACKWARD, currentMillis);
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