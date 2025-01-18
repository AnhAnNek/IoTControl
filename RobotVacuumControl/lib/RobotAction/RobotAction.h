#ifndef ROBOT_ACTION_H
#define ROBOT_ACTION_H

#include <functional>
#include <memory>

class RobotAction {
public:
    RobotAction(std::function<void()> action, std::function<bool()> condition);
    virtual ~RobotAction() = default;

    // Execute the action
    virtual void execute();
    
    // Check if the action is completed
    bool isCompleted();

    // Builder class declaration
    class Builder {
    public:
        Builder& setAction(std::function<void()> action);
        Builder& setCompletedCondition(std::function<bool()> condition);
        std::unique_ptr<RobotAction> build();

    private:
        std::function<void()> _action = []() {};
        std::function<bool()> _completedCondition = []() { return false; };
    };

protected:
    std::function<void()> _action;
    std::function<bool()> _completedCondition;
};

#endif // ROBOT_ACTION_H
