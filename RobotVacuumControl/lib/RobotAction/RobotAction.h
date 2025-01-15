#ifndef ROBOT_ACTION_H
#define ROBOT_ACTION_H

#include <functional>

class RobotAction {
public:
    virtual ~RobotAction() = default;

    // Execute the action
    virtual void execute() {
        if (action) {
            action();
        }
    }

    // Set the action behavior
    void setAction(std::function<void()> newAction) {
        action = newAction;
    }

    bool isCompleted() {
        return completedCondition();
    }

    // Set the completed condition
    void setCompletedCondition(std::function<bool()> condition) {
        completedCondition = condition;
    }

protected:
    std::function<void()> action = []() {}; // The specific action to perform
    std::function<bool()> completedCondition = []() { return false; }; // Condition to check if action is complete
};

#endif // ROBOT_ACTION_H
