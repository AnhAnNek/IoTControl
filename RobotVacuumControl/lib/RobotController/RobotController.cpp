#include "RobotController.h"

RobotController::RobotController(SensorManager& sensorManager, MotorController& motorController)
    : sensorManager(sensorManager), motorController(motorController) {}

void RobotController::moveForward(int speed) {
  motorController.setMotorSpeeds(speed, speed);
  motorController.moveForward();
}

void RobotController::moveBackward(int speed) {
  motorController.setMotorSpeeds(speed, speed);
  motorController.moveBackward();
}

void RobotController::rotateLeft(int speed) {
  motorController.setMotorSpeeds(speed, speed);
  motorController.rotateLeft();
}

void RobotController::rotateRight(int speed) {
  motorController.setMotorSpeeds(speed, speed);
  motorController.rotateRight();
}

void RobotController::stop() {
  motorController.stop();
}
