#include "sync_mechanism.h"
#include <algorithm>

SyncMechanism::SyncMechanism()
    : currentTime(0) {
}

void SyncMechanism::initialize(const std::vector<Process>& processes,
                               const std::vector<Resource>& resources,
                               const std::vector<Action>& actions) {
    this->processes = processes;
    this->resources = resources;
    this->actions = actions;
    
    reset();
}

int SyncMechanism::getCurrentTime() const {
    return currentTime;
}

const std::map<int, std::string>& SyncMechanism::getTimeline() const {
    return timeline;
}

bool SyncMechanism::isSimulationFinished() const {
    // La simulaciu00f3n termina cuando todas las acciones han sido completadas
    return completedActions.size() == actions.size();
}

void SyncMechanism::reset() {
    currentTime = 0;
    pendingActions.clear();
    completedActions.clear();
    timeline.clear();
    
    // Reiniciar los recursos a su estado inicial
    for (auto& resource : resources) {
        resource.reset();
    }
}

void SyncMechanism::updatePendingActions() {
    for (const auto& action : actions) {
        if (action.getCycle() <= currentTime && 
            std::find_if(pendingActions.begin(), pendingActions.end(),
                         [&action](const Action& a) {
                             return a.getProcessId() == action.getProcessId() &&
                                    a.getResourceName() == action.getResourceName() &&
                                    a.getCycle() == action.getCycle();
                         }) == pendingActions.end()) {
            Action newAction = action;
            newAction.setStatus(ActionStatus::WAITING);
            pendingActions.push_back(newAction);
        }
    }
}

Resource* SyncMechanism::getResourceByName(const std::string& name) {
    auto it = std::find_if(resources.begin(), resources.end(),
                         [&name](const Resource& r) {
                             return r.getName() == name;
                         });
    
    if (it != resources.end()) {
        return &(*it);
    }
    
    return nullptr;
}

void SyncMechanism::updateTimeline(const std::string& processId, ActionStatus status) {
    // Crear una representaciu00f3n visual del evento
    std::string eventLabel = processId + "|" + Action::actionStatusToString(status);
    timeline[currentTime] = eventLabel;
}