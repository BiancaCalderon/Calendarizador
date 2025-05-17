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
    // Iterar sobre las acciones pendientes
    auto it = pendingActions.begin();
    while (it != pendingActions.end()) {
        Action& action = *it;
        Resource* resource = getResourceByName(action.getResourceName());
        
        if (resource) {
            // Verificar si el recurso esta disponible
            bool acquired = false;
            
            // Si la accion ya tiene acceso al recurso
            if (action.getStatus() == ActionStatus::ACCESSED) {
                // Liberar el recurso y marcar como completada
                resource->release(action.getProcessId());
                action.setStatus(ActionStatus::COMPLETED);
                
                // Actualizar timeline
                updateTimeline(action.getProcessId(), ActionStatus::COMPLETED);
                
                // Mover a la lista de acciones completadas
                completedActions.push_back(action);
                it = pendingActions.erase(it);
                continue;
            }
            
            // Mutex: solo un proceso puede acceder al mismo tiempo
            if (resource->getAvailableCount() > 0) {
                // Adquirir el recurso
                acquired = resource->acquire(action.getProcessId());
                
                if (acquired) {
                    // Actualizar el estado de la accion
                    action.setStatus(ActionStatus::ACCESSED);
                    
                    // Actualizar timeline
                    updateTimeline(action.getProcessId(), ActionStatus::ACCESSED);
                } else {
                    // Agregar a la cola de espera del recurso
                    resource->addToQueue(action.getProcessId());
                    
                    // Actualizar timeline
                    updateTimeline(action.getProcessId(), ActionStatus::WAITING);
                }
            } else {
                // Recurso no disponible, agregar a la cola de espera
                resource->addToQueue(action.getProcessId());
                
                // Actualizar timeline
                updateTimeline(action.getProcessId(), ActionStatus::WAITING);
            }
        }
        
        ++it;
    }
}