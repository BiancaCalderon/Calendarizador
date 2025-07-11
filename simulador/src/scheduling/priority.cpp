#include "priority.h"
#include <algorithm>
#include <climits>
#include <iostream>

Priority::Priority(bool isPreemptive) : Scheduler() {
    // Establecer si el algoritmo es preemptivo o no
    this->isPreemptive = isPreemptive;
}

void Priority::setPreemptive(bool isPreemptive) {
    this->isPreemptive = isPreemptive;
}

void Priority::tick() {
    // Actualizar la cola de procesos listos
    updateReadyQueue();
    
    // Debug: Estado al inicio del tick
    std::cerr << "[Priority Tick] Time: " << currentTime << ", Ready Queue Size: " << readyQueue.size() << ", Current Process Index: " << currentProcessIndex << std::endl;
    if (currentProcessIndex != -1 && static_cast<size_t>(currentProcessIndex) < readyQueue.size()) {
        std::cerr << "  Executing Process: " << readyQueue[currentProcessIndex].getPID() << ", Remaining: " << readyQueue[currentProcessIndex].getRemainingTime() << std::endl;
    }
    
    // Si no hay un proceso en ejecución o si es preemptivo, seleccionar uno nuevo
    if (currentProcessIndex == -1 || isPreemptive) {
        int newProcessIndex = selectNextProcess();
        
        // Si encontramos un nuevo proceso para ejecutar
        if (newProcessIndex != -1) {
            // Si estábamos ejecutando un proceso diferente y es preemptivo
            if (currentProcessIndex != -1 && isPreemptive && newProcessIndex != currentProcessIndex) {
                // La prioridad del nuevo proceso es mayor que la del actual
                Process& currentProcess = readyQueue[currentProcessIndex];
                Process& newProcess = readyQueue[newProcessIndex];
                
                // Si la prioridad del nuevo proceso es mayor (número menor)
                if (newProcess.getPriority() < currentProcess.getPriority()) {
                    // Preempción: cambiar al nuevo proceso
                    currentProcessIndex = newProcessIndex;
                }
            } else if (currentProcessIndex == -1) {
                // No había proceso en ejecución, asignar el nuevo
                currentProcessIndex = newProcessIndex;
            }
        }
    }
    
    // Si hay un proceso en ejecución
    if (currentProcessIndex != -1) {
        // Obtener el proceso actual
        Process& currentProcess = readyQueue[currentProcessIndex];
        
        // Actualizar timeline
        updateTimeline(currentProcess.getPID());
        
        // Decrementar el tiempo restante
        currentProcess.decrementRemainingTime();
        
        // Si el proceso ha terminado
        if (currentProcess.isCompleted()) {
            // Calcular tiempo de retorno (turnaround time)
            currentProcess.setTurnaroundTime(currentTime + 1 - currentProcess.getArrivalTime());
            
            // Mover a la lista de procesos terminados
            finishedProcesses.push_back(currentProcess);
            
            // Eliminar de la cola de listos
            readyQueue.erase(readyQueue.begin() + currentProcessIndex);
            
            // No hay proceso en ejecución
            currentProcessIndex = -1;
        }
    }
    
    // Actualizar tiempos de espera para los procesos en la cola que NO están ejecutando en este ciclo
    std::cerr << "  Updating Waiting Times..." << std::endl;
    for (auto& process : readyQueue) {
        // Solo incrementar si no es el proceso actual (si currentProcessIndex es válido)
        bool isExecuting = (currentProcessIndex != -1 && static_cast<size_t>(currentProcessIndex) < readyQueue.size() && process.getPID() == readyQueue[currentProcessIndex].getPID());
        if (!isExecuting) {
             process.setWaitingTime(process.getWaitingTime() + 1);
             std::cerr << "    Incremented WT for " << process.getPID() << ", new WT: " << process.getWaitingTime() << std::endl;
        }
    }
    
    // Incrementar el tiempo actual
    currentTime++;
}

int Priority::selectNextProcess() {
    // Si no hay procesos en la cola de listos
    if (readyQueue.empty()) {
        return -1;
    }
    
    // Buscar el proceso con mayor prioridad (menor número)
    int highestPriorityIndex = 0;
    int highestPriority = INT_MAX;
    
    for (size_t i = 0; i < readyQueue.size(); i++) {
        int priority = readyQueue[i].getPriority();
        
        // Si encontramos un proceso con mayor prioridad (número menor)
        if (priority < highestPriority) {
            highestPriority = priority;
            highestPriorityIndex = static_cast<int>(i);
        }
    }
    
    return highestPriorityIndex;
}