#include "fifo.h"

FIFO::FIFO() {
    // FIFO no es preemptive
    isPreemptive = false;
}

void FIFO::tick() {
    // Actualizar la cola de procesos listos
    updateReadyQueue();
    
    // Si no hay un proceso en ejecución, seleccionar uno nuevo
    if (currentProcessIndex == -1 && !readyQueue.empty()) {
        currentProcessIndex = selectNextProcess();
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
    for (auto& process : readyQueue) {
        if (currentProcessIndex != -1 && !readyQueue.empty() && currentProcessIndex < readyQueue.size() && process.getPID() != readyQueue[currentProcessIndex].getPID()) {
            process.setWaitingTime(process.getWaitingTime() + 1);
        }
    }
    
    // Incrementar el tiempo actual
    currentTime++;
}

int FIFO::selectNextProcess() {
    // En FIFO, simplemente se selecciona el primer proceso de la cola
    // Ya que la cola se actualiza en orden de llegada, el primero
    // es el que llegó primero (First In First Out)
    return 0; // Devuelve el índice del primer proceso en la cola de listos
}