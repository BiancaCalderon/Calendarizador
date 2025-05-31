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

    // Si no hay un proceso en ejecución o se agotó su quantum
    bool needsContextSwitch = (currentProcessIndex == -1 || timeInQuantum >= quantum);

    if (needsContextSwitch) {
        // Si hay un proceso en ejecución y no ha terminado ANTES de agotar quantum
        if (currentProcessIndex != -1 && !readyQueue[currentProcessIndex].isCompleted()) {
            // Mover el proceso actual al final de la cola
            Process currentProcess = readyQueue[currentProcessIndex];
            readyQueue.erase(readyQueue.begin() + currentProcessIndex);
            readyQueue.push_back(currentProcess);
        }

        // Seleccionar el siguiente proceso
        currentProcessIndex = selectNextProcess();

        // Reiniciar el tiempo en quantum para el nuevo proceso (si hay)
        timeInQuantum = 0;
    }

    // Si hay un proceso en ejecución (puede ser el mismo o uno nuevo si hubo cambio de contexto)
    if (currentProcessIndex != -1) {
        Process& currentProcess = readyQueue[currentProcessIndex];

        // Actualizar timeline
        updateTimeline(currentProcess.getPID());

        // Decrementar el tiempo restante
        currentProcess.decrementRemainingTime();

        // Incrementar el tiempo que ha estado en este quantum
        timeInQuantum++;

        // === Manejo de la finalización del proceso ===
        if (currentProcess.isCompleted()) {
            // Calcular tiempo de retorno (turnaround time)
            currentProcess.setTurnaroundTime(currentTime + 1 - currentProcess.getArrivalTime());

            // Mover a la lista de procesos terminados
            finishedProcesses.push_back(currentProcess);

            // Eliminar de la cola de listos
            readyQueue.erase(readyQueue.begin() + currentProcessIndex);

            // No hay proceso en ejecución después de este tick
            currentProcessIndex = -1; // Importante: se establece a -1 DESPUÉS de usarlo en este tick
            timeInQuantum = 0; // Reiniciar quantum ya que este proceso terminó

            // No necesitamos seleccionar el siguiente proceso inmediatamente aquí
            // porque el bucle principal en tick() o el siguiente tick lo harán.
        }
    }

    // Actualizar tiempos de espera para los procesos en la cola que NO están ejecutando en este ciclo
    for (auto& process : readyQueue) {
        // Solo incrementar si no es el proceso actual (si currentProcessIndex es válido)
        if (currentProcessIndex == -1 || (currentProcessIndex != -1 && !readyQueue.empty() && (static_cast<size_t>(currentProcessIndex) >= readyQueue.size() || process.getPID() != readyQueue[currentProcessIndex].getPID()))) {
             process.setWaitingTime(process.getWaitingTime() + 1);
        }
    }

    // Incrementar el tiempo actual
    currentTime++;
}

int RoundRobin::selectNextProcess() {
    // En Round Robin, simplemente tomamos el primer proceso en la cola
    // ya que la ordenamos de manera circular al terminar el quantum
    return readyQueue.empty() ? -1 : 0;
}
