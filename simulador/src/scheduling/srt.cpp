#include "srt.h"
#include <algorithm>
#include <climits>

SRT::SRT() {
    // SRT es un algoritmo preemptivo
    isPreemptive = true;
}

void SRT::tick() {
    // Actualizar la cola de procesos listos
    updateReadyQueue();
    
    // Siempre seleccionar el proceso con menor tiempo restante
    int newProcessIndex = selectNextProcess();
    
    // Si encontramos un proceso para ejecutar
    if (newProcessIndex != -1) {
        // Si hay un cambio de proceso o no habu00eda ninguno en ejecuciu00f3n
        if (currentProcessIndex != newProcessIndex) {
            currentProcessIndex = newProcessIndex;
        }
    }
    
    // Si hay un proceso en ejecuciu00f3n
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
            
            // No hay proceso en ejecuciu00f3n
            currentProcessIndex = -1;
        }
    }
    
    // Actualizar tiempos de espera
    for (auto& process : readyQueue) {
        if (currentProcessIndex != -1 && !readyQueue.empty() && currentProcessIndex < readyQueue.size() && process.getPID() != readyQueue[currentProcessIndex].getPID()) {
            process.setWaitingTime(process.getWaitingTime() + 1);
        }
    }
    
    // Incrementar el tiempo actual
    currentTime++;
}

int SRT::selectNextProcess() {
    // Si no hay procesos en la cola de listos
    if (readyQueue.empty()) {
        return -1;
    }
    
    // Buscar el proceso con menor tiempo restante
    int shortestIndex = 0;
    int shortestTime = INT_MAX;
    
    for (size_t i = 0; i < readyQueue.size(); i++) {
        int remainingTime = readyQueue[i].getRemainingTime();
        
        // Si encontramos un proceso con menor tiempo restante
        if (remainingTime < shortestTime) {
            shortestTime = remainingTime;
            shortestIndex = static_cast<int>(i);
        }
    }
    
    return shortestIndex;
} 