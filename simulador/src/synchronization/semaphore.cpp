#include "semaphore.h"
#include <algorithm>
#include <iostream>

Semaphore::Semaphore() : SyncMechanism() {
}

void Semaphore::tick() {
    // Actualizar y procesar acciones pendientes para el tiempo actual
    updatePendingActions();
    

    processActions();
   
    currentTime++;
}

void Semaphore::processActions() {
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
                
                // Comprobar si hay procesos esperando para adquirir este recurso
                const auto& queue = resource->getAccessQueue();
                if (!queue.empty() && resource->getAvailableCount() > 0) {
                    // Buscar la primera accion pendiente correspondiente al proceso en la cola
                    auto waitingIt = std::find_if(pendingActions.begin(), pendingActions.end(),
                                               [&queue](const Action& a) {
                                                   return a.getProcessId() == queue[0] && 
                                                          a.getStatus() == ActionStatus::WAITING;
                                               });
                    
                    if (waitingIt != pendingActions.end()) {
                        // Dar acceso al siguiente proceso en la cola
                        resource->acquire(waitingIt->getProcessId());
                        waitingIt->setStatus(ActionStatus::ACCESSED);
                        
                        // Remover de la cola de espera
                        resource->removeFromQueue(waitingIt->getProcessId());
                    }
                }
                
                continue;
            }
            
            // semaforo: multiples procesos pueden acceder dependiendo del contador
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
