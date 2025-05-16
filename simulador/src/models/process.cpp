#include "process.h"

Process::Process(const std::string& pid, int burstTime, int arrivalTime, int priority)
    : pid(pid), burstTime(burstTime), arrivalTime(arrivalTime), priority(priority),
      remainingTime(burstTime), waitingTime(0), turnaroundTime(0) {}

std::string Process::getPID() const {
    return pid;
}

int Process::getBurstTime() const {
    return burstTime;
}

int Process::getArrivalTime() const {
    return arrivalTime;
}

int Process::getPriority() const {
    return priority;
}

int Process::getRemainingTime() const {
    return remainingTime;
}

int Process::getWaitingTime() const {
    return waitingTime;
}

int Process::getTurnaroundTime() const {
    return turnaroundTime;
}

void Process::setRemainingTime(int time) {
    remainingTime = time;
}

void Process::setWaitingTime(int time) {
    waitingTime = time;
}

void Process::setTurnaroundTime(int time) {
    turnaroundTime = time;
}

void Process::decrementRemainingTime() {
    if (remainingTime > 0) {
        remainingTime--;
    }
}

bool Process::isCompleted() const {
    return remainingTime <= 0;
}
