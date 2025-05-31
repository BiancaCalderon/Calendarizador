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
    // La simulacion termina cuando todas las acciones iniciales han sido completadas.
    // Esto significa que cada accion en la lista original 'actions' debe estar en 'completedActions'.
    if (actions.empty()) return true; // Si no hay acciones, la simulacion 'termina' inmediatamente

    if (completedActions.size() != actions.size()) return false; // Optimizacion rapida

    // Verificacion completa: cada accion original debe estar en la lista de completadas.
    for (const auto& original_action : actions) {
        bool found = false;
        for (const auto& completed_action : completedActions) {
            // Comparar acciones por sus identificadores unicos (PID, Recurso, Ciclo)
            if (original_action.getProcessId() == completed_action.getProcessId() &&
                original_action.getResourceName() == completed_action.getResourceName() &&
                original_action.getCycle() == completed_action.getCycle()) {
                found = true;
                break;
            }
        }
        if (!found) return false; // Si una accion original no esta en completadas, la simulacion no ha terminado.
    }

    return true; // Todas las acciones originales encontradas en completadas.
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
    // Iterar sobre la lista original de acciones y añadir a pendingActions las que llegan en este ciclo
    // y que aun no han sido completadas ni estan ya en pendingActions.

    for (const auto& action : actions) {
        // Solo añadir si el ciclo de llegada es igual al currentTime
        if (action.getCycle() == currentTime) {
            // Verificar si esta accion ya ha sido completada
            bool isCompleted = std::find_if(completedActions.begin(), completedActions.end(),
                                           [&action](const Action& a) {
                                               return a.getProcessId() == action.getProcessId() &&
                                                      a.getResourceName() == action.getResourceName() &&
                                                      a.getCycle() == action.getCycle();
                                           }) != completedActions.end();

            // Verificar si ya está en pendingActions
             bool isPending = std::find_if(pendingActions.begin(), pendingActions.end(),
                                           [&action](const Action& a) {
                                               return a.getProcessId() == action.getProcessId() &&
                                                      a.getResourceName() == action.getResourceName() &&
                                                      a.getCycle() == action.getCycle();
                                           }) != pendingActions.end();

            if (!isCompleted && !isPending) {
                Action newAction = action;
                newAction.setStatus(ActionStatus::WAITING); // Estado inicial al ser añadida
                pendingActions.push_back(newAction);
            }
        }
        // Acciones con ciclo < currentTime que no se añadieron ya se consideraron tarde.
        // Acciones con ciclo > currentTime seran consideradas en ciclos futuros.
    }
    // Debugging: Mostrar acciones pendientes añadidas en este ciclo (opcional)
    // std::cerr << "Debug: updatePendingActions finished at time " << currentTime << ". Pending size: " << pendingActions.size() << std::endl;
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