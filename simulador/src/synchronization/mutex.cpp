#include "mutex.h"
#include <algorithm>
#include <iostream>
#include <set>

Mutex::Mutex() : SyncMechanism() {
}

void Mutex::tick() {
    // Actualizar  y procesar las acciones pendientes para el tiempo actual
    updatePendingActions();

    processActions();
    

    currentTime++;
}

void Mutex::processActions() {
    std::vector<Action> justCompletedActions;
    std::set<std::string> recursosOcupadosEsteCiclo;

    auto it = pendingActions.begin();
    while (it != pendingActions.end()) {
        Action& action = *it;
        Resource* resource = getResourceByName(action.getResourceName());

        if (!resource) {
            ++it;
            continue;
        }

        // Si la acción estaba en ACCESSED, la completamos y liberamos el recurso
        if (action.getStatus() == ActionStatus::ACCESSED) {
            resource->release(action.getProcessId());
            action.setStatus(ActionStatus::COMPLETED);
            updateTimeline(action.getProcessId(), ActionStatus::COMPLETED);
            justCompletedActions.push_back(action);
            it = pendingActions.erase(it);
            continue;
        }

        // Si la acción está esperando, intentamos adquirir el recurso
        if (action.getStatus() == ActionStatus::WAITING) {
            // Si el recurso ya fue adquirido por otra acción en este ciclo, no intentamos adquirirlo
            if (recursosOcupadosEsteCiclo.count(action.getResourceName())) {
                updateTimeline(action.getProcessId(), ActionStatus::WAITING);
                ++it;
                continue;
            }

            bool acquired = resource->acquire(action.getProcessId());
            if (acquired) {
                action.setStatus(ActionStatus::ACCESSED);
                updateTimeline(action.getProcessId(), ActionStatus::ACCESSED);
                recursosOcupadosEsteCiclo.insert(action.getResourceName());
                ++it;
            } else {
                updateTimeline(action.getProcessId(), ActionStatus::WAITING);
                ++it;
            }
            continue;
        }

        // Estado inesperado
        ++it;
    }

    completedActions.insert(completedActions.end(), justCompletedActions.begin(), justCompletedActions.end());
     std::cerr << "Debug [Mutex]: ProcessActions finished. Total completed: " << completedActions.size() << ", Total actions: " << actions.size() << ", Pending actions: " << pendingActions.size() << std::endl;
}