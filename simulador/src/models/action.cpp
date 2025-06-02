#include "action.h"
#include <stdexcept>

Action::Action(const std::string& processId, ActionType type, 
               const std::string& resourceName, int cycle)
    : processId(processId), type(type), resourceName(resourceName), 
      cycle(cycle), status(ActionStatus::WAITING) {}

std::string Action::getProcessId() const {
    return processId;
}

ActionType Action::getType() const {
    return type;
}

std::string Action::getResourceName() const {
    return resourceName;
}

int Action::getCycle() const {
    return cycle;
}

ActionStatus Action::getStatus() const {
    return status;
}

void Action::setStatus(ActionStatus newStatus) {
    status = newStatus;
}

ActionType Action::stringToActionType(const std::string& typeStr) {
    if (typeStr == "READ") {
        return ActionType::READ;
    } else if (typeStr == "WRITE") {
        return ActionType::WRITE;
    } else {
        throw std::invalid_argument("Tipo de acción no válido: " + typeStr);
    }
}

std::string Action::actionTypeToString(ActionType type) {
    switch (type) {
        case ActionType::READ: return "READ";
        case ActionType::WRITE: return "WRITE";
        default: return "UNKNOWN";
    }
}

std::string Action::actionStatusToString(ActionStatus status) {
    switch (status) {
        case ActionStatus::WAITING:
            return "WAITING";
        case ActionStatus::ACCESSED:
            return "ACCESSED";
        case ActionStatus::COMPLETED:
            return "COMPLETED";
        default:
            return "UNKNOWN";
    }
}
