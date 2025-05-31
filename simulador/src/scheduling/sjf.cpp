#include "sjf.h"
#include <algorithm>
#include <climits>

SJF::SJF() : Scheduler() {
    // SJF no es preemptivo
    isPreemptive = false;
}

void SJF::tick() {
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
            currentProcess.setTurnaroundTime(currentTime + 1);
            
            // Calcular tiempo de espera (waiting time)
            // WT = TAT - BurstTime
            currentProcess.setWaitingTime(currentProcess.getTurnaroundTime() - currentProcess.getBurstTime());
            
            // Mover a la lista de procesos terminados
            finishedProcesses.push_back(currentProcess);
            
            // Eliminar de la cola de listos
            readyQueue.erase(readyQueue.begin() + currentProcessIndex);
            
            // No hay proceso en ejecución
            currentProcessIndex = -1;
        }
    }
    
    // Actualizar tiempos de espera para los procesos en la cola
    for (auto& process : readyQueue) {
        if (currentProcessIndex != -1 && !readyQueue.empty() && currentProcessIndex < readyQueue.size() && process.getPID() != readyQueue[currentProcessIndex].getPID()) {
            process.setWaitingTime(process.getWaitingTime() + 1);
        }
    }
    
    // Incrementar el tiempo actual
    currentTime++;
}

int SJF::selectNextProcess() {
    // Si no hay procesos en la cola de listos
    if (readyQueue.empty()) {
        return -1;
    }
    
    // Buscar el proceso con menor burst time
    int shortestIndex = 0;
    int shortestTime = INT_MAX;
    
    for (size_t i = 0; i < readyQueue.size(); i++) {
        int burstTime = readyQueue[i].getBurstTime();
        
        // Si encontramos un proceso con menor burst time
        if (burstTime < shortestTime) {
            shortestTime = burstTime;
            shortestIndex = static_cast<int>(i);
        }
    }
    
    return shortestIndex;
} 