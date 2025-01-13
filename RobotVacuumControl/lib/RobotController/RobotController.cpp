#include "RobotController.h"

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

RobotController::State RobotController::getCurrentState() const {
  return _currentState;
}

// Setter for _currentState
void RobotController::setCurrentState(RobotController::State newState) {
  _currentState = newState;
}

bool RobotController::isAutoMode()
{
  return _autoMode;
}

void RobotController::setAutoMode(bool autoMode)
{
  _autoMode = autoMode;
}