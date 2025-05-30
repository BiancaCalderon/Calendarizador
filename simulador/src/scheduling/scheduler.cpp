#include "scheduler.h"
#include "../utils/metrics.h"
#include <algorithm>

Scheduler::Scheduler()
    : currentTime(0), currentProcessIndex(-1), isPreemptive(false) {}

void Scheduler::initialize(const std::vector<Process>& processes) {
    this->processes = processes;
    reset();
}

void Scheduler::runSimulation() {
    while (!isSimulationFinished()) {
        tick();
    }
}

void Scheduler::reset() {
    currentTime = 0;
    currentProcessIndex = -1;
    readyQueue.clear();
    finishedProcesses.clear();
    timeline.clear();
    
    // Restablecer los procesos a su estado inicial
    for (auto& process : processes) {
        process.setRemainingTime(process.getBurstTime());
        process.setWaitingTime(0);
        process.setTurnaroundTime(0);
    }
}

int Scheduler::getCurrentTime() const {
    return currentTime;
}

const std::vector<Process>& Scheduler::getProcesses() const {
    return processes;
}

const std::vector<Process>& Scheduler::getReadyQueue() const {
    return readyQueue;
}

const std::vector<Process>& Scheduler::getFinishedProcesses() const {
    return finishedProcesses;
}

const std::map<int, std::string>& Scheduler::getTimeline() const {
    return timeline;
}

bool Scheduler::isSimulationFinished() const {
    return finishedProcesses.size() == processes.size();
}

double Scheduler::getAverageWaitingTime() const {
    return Metrics::calculateAverageWaitingTime(finishedProcesses);
}

double Scheduler::getAverageTurnaroundTime() const {
    return Metrics::calculateAverageTurnaroundTime(finishedProcesses);
}

void Scheduler::updateReadyQueue() {
    // Agregar procesos que llegan en el tiempo actual a la cola de listos
    for (const auto& process : processes) {
        // Si el proceso ya ha llegado, no esta completo y no esta en la cola de listos
        if (process.getArrivalTime() <= currentTime &&
            !process.isCompleted() &&
            std::find_if(readyQueue.begin(), readyQueue.end(),
                        [&process](const Process& p) { return p.getPID() == process.getPID(); }) == readyQueue.end() &&
            std::find_if(finishedProcesses.begin(), finishedProcesses.end(),
                        [&process](const Process& p) { return p.getPID() == process.getPID(); }) == finishedProcesses.end()) {
            
            readyQueue.push_back(process);
        }
    }
}

void Scheduler::updateWaitingTimes() {
    // Incrementar el tiempo de espera para todos los procesos en la cola de listos
    // excepto el proceso en ejecucion
    for (auto& process : readyQueue) {
        if (currentProcessIndex == -1 || 
            process.getPID() != readyQueue[currentProcessIndex].getPID()) {
            int waitingTime = process.getWaitingTime() + 1;
            process.setWaitingTime(waitingTime);
        }
    }
}

void Scheduler::updateTimeline(const std::string& processId) {
    timeline[currentTime] = processId;
}