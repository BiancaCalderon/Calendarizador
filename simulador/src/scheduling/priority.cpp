#include "priority.h"
#include <algorithm>
#include <climits>

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
    
    // Actualizar tiempos de espera
    updateWaitingTimes();
    
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