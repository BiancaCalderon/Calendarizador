#include "fifo.h"

FIFO::FIFO() {
    // FIFO no es preemptive
    isPreemptive = false;
}

void FIFO::tick() {
    // Actualizar la cola de procesos listos
    updateReadyQueue();
    
    // Si no hay un proceso en ejecuciu00f3n, seleccionar uno nuevo
    if (currentProcessIndex == -1 && !readyQueue.empty()) {
        currentProcessIndex = selectNextProcess();
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
    updateWaitingTimes();
    
    // Incrementar el tiempo actual
    currentTime++;
}

int FIFO::selectNextProcess() {
    // En FIFO, simplemente se selecciona el primer proceso de la cola
    // Ya que la cola se actualiza en orden de llegada, el primero
    // es el que llegu00f3 primero (First In First Out)
    return 0; // Devuelve el u00edndice del primer proceso en la cola de listos
}