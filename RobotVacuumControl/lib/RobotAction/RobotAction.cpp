#include "RobotAction.h"

// Constructor implementation
RobotAction::RobotAction(std::function<void()> action, std::function<bool()> condition)
    : _action(action), _completedCondition(condition) {}

// Executes the action if defined
void RobotAction::execute() {
    if (_action) {
        _action();
    }
}

// Checks if the action is completed
bool RobotAction::isCompleted() {
    return _completedCondition();
}

// Builder method: Set the action
RobotAction::Builder& RobotAction::Builder::setAction(std::function<void()> action) {
    _action = action;
    return *this;
}

// Builder method: Set the completed condition
RobotAction::Builder& RobotAction::Builder::setCompletedCondition(std::function<bool()> condition) {
    _completedCondition = condition;
    return *this;
}

// Builder method: Build the RobotAction
std::unique_ptr<RobotAction> RobotAction::Builder::build() {
    return std::unique_ptr<RobotAction>(new RobotAction(_action, _completedCondition));
}
