#include "round_robin.h"
#include <algorithm>

RoundRobin::RoundRobin(int q) : Scheduler(), quantum(q), timeInQuantum(0) {
    // Round Robin no es preemptivo en sentido estricto, pero alterna entre procesos
    isPreemptive = false;
}

void RoundRobin::setQuantum(int q) {
    if (q > 0) {
        quantum = q;
    }
}

void RoundRobin::reset() {
    // Llamar al reseteador de la clase base
    Scheduler::reset();
    
    // Reiniciar el contador de tiempo en quantum
    timeInQuantum = 0;
}

void RoundRobin::tick() {
    // Actualizar la cola de procesos listos
    updateReadyQueue();
    
    // Si no hay un proceso en ejecuciu00f3n o se agotu00f3 su quantum, seleccionar uno nuevo
    if (currentProcessIndex == -1 || timeInQuantum >= quantum) {
        // Si hay un proceso en ejecuciu00f3n y no ha terminado
        if (currentProcessIndex != -1 && !readyQueue[currentProcessIndex].isCompleted()) {
            // Mover el proceso actual al final de la cola (algoritmo Round Robin)
            Process currentProcess = readyQueue[currentProcessIndex];
            readyQueue.erase(readyQueue.begin() + currentProcessIndex);
            readyQueue.push_back(currentProcess);
        }
        
        // Seleccionar el siguiente proceso
        currentProcessIndex = selectNextProcess();
        
        // Reiniciar el tiempo en quantum para el nuevo proceso
        timeInQuantum = 0;
    }
    
    // Si hay un proceso en ejecuciu00f3n
    if (currentProcessIndex != -1) {
        // Obtener el proceso actual
        Process& currentProcess = readyQueue[currentProcessIndex];
        
        // Actualizar timeline
        updateTimeline(currentProcess.getPID());
        
        // Decrementar el tiempo restante
        currentProcess.decrementRemainingTime();
        
        // Incrementar el tiempo que ha estado en este quantum
        timeInQuantum++;
        
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
            timeInQuantum = 0;
        }
    }
    
    // Actualizar tiempos de espera
    updateWaitingTimes();
    
    // Incrementar el tiempo actual
    currentTime++;
}

int RoundRobin::selectNextProcess() {
    // En Round Robin, simplemente tomamos el primer proceso en la cola
    // ya que la ordenamos de manera circular al terminar el quantum
    return readyQueue.empty() ? -1 : 0;
} 