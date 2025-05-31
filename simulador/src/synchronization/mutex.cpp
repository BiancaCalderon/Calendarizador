#include "mutex.h"
#include <algorithm>
#include <iostream>

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
    auto it = pendingActions.begin();
    while (it != pendingActions.end()) {
        Action& action = *it;
        Resource* resource = getResourceByName(action.getResourceName());

        std::cerr << "Debug [Mutex]: Processing action " << action.getProcessId()
                  << " type " << Action::actionTypeToString(action.getType())
                  << " res " << action.getResourceName()
                  << " cycle " << action.getCycle()
                  << " current status " << Action::actionStatusToString(action.getStatus())
                  << " at time " << currentTime << std::endl;

        if (!resource) { // Recurso no valido
            std::cerr << "Debug [Mutex]: Invalid resource for action " << action.getProcessId() << std::endl;
            ++it;
            continue;
        }

        // Estado 1: ACCESSED (Accion usando el recurso en el ciclo anterior, completando ahora)
        if (action.getStatus() == ActionStatus::ACCESSED) {
            std::cerr << "Debug [Mutex]: Completing action " << action.getProcessId() << std::endl;
            resource->release(action.getProcessId()); // Liberar recurso
            action.setStatus(ActionStatus::COMPLETED); // Marcar como completada
            updateTimeline(action.getProcessId(), ActionStatus::COMPLETED); // Registrar en timeline
            justCompletedActions.push_back(action); // Añadir a lista de completadas de este ciclo
            it = pendingActions.erase(it); // Eliminar de pendientes
            continue; // Continuar al siguiente elemento
        }

        // Estado 2: WAITING (Accion esperando acceso)
        if (action.getStatus() == ActionStatus::WAITING) {
             std::cerr << "Debug [Mutex]: Attempting to acquire resource for action " << action.getProcessId() << std::endl;
            bool acquired = resource->acquire(action.getProcessId()); // Intentar adquirir (maneja cola)

            if (acquired) {
                std::cerr << "Debug [Mutex]: Acquired resource for action " << action.getProcessId() << std::endl;
                action.setStatus(ActionStatus::ACCESSED); // Adquirio, pasa a ACCESSED
                updateTimeline(action.getProcessId(), ActionStatus::ACCESSED); // Registrar en timeline
                ++it; // Permanece en pendingActions para completar en el prox tick
            } else {
                // Sigue esperando
                 std::cerr << "Debug [Mutex]: Could not acquire resource for action " << action.getProcessId() << ". Still waiting." << std::endl;
                updateTimeline(action.getProcessId(), ActionStatus::WAITING); // Registrar en timeline
                ++it; // Permanece en pendingActions
            }
            continue; // Continuar al siguiente elemento
        }
        
        // Si llega aqui, es una accion recien añadida por updatePendingActions (deberia ser WAITING)
        // O un estado inesperado. La tratamos como WAITING.
         std::cerr << "Debug [Mutex]: Unexpected action status for " << action.getProcessId() << ". Treating as WAITING." << std::endl;
         bool acquired = resource->acquire(action.getProcessId());
        if (acquired) {
             std::cerr << "Debug [Mutex]: Acquired resource (fallback) for action " << action.getProcessId() << std::endl;
             action.setStatus(ActionStatus::ACCESSED);
             updateTimeline(action.getProcessId(), ActionStatus::ACCESSED);
             ++it;
        } else {
             std::cerr << "Debug [Mutex]: Could not acquire resource (fallback) for action " << action.getProcessId() << ". Still waiting." << std::endl;
             updateTimeline(action.getProcessId(), ActionStatus::WAITING);
             ++it;
        }
    }

    // Mover acciones completadas de este ciclo a la lista general de completadas
    completedActions.insert(completedActions.end(), justCompletedActions.begin(), justCompletedActions.end());
     std::cerr << "Debug [Mutex]: ProcessActions finished. Total completed: " << completedActions.size() << ", Total actions: " << actions.size() << ", Pending actions: " << pendingActions.size() << std::endl;
}