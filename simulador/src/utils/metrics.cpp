#include "metrics.h"
#include <numeric>

double Metrics::calculateAverageWaitingTime(const std::vector<Process>& processes) {
    if (processes.empty()) {
        return 0.0;
    }

    double totalWaitingTime = 0.0;
    for (const auto& process : processes) {
        totalWaitingTime += process.getWaitingTime();
    }

    return totalWaitingTime / processes.size();
}

double Metrics::calculateAverageTurnaroundTime(const std::vector<Process>& processes) {
    if (processes.empty()) {
        return 0.0;
    }

    double totalTurnaroundTime = 0.0;
    for (const auto& process : processes) {
        totalTurnaroundTime += process.getTurnaroundTime();
    }

    return totalTurnaroundTime / processes.size();
}

double Metrics::calculateCPUUtilization(const std::vector<Process>& processes, int totalTime) {
    if (totalTime == 0 || processes.empty()) {
        return 0.0;
    }

    double totalBurstTime = 0.0;
    for (const auto& process : processes) {
        totalBurstTime += process.getBurstTime();
    }

    return (totalBurstTime / totalTime) * 100.0; // retornamos en porcentaje
}

double Metrics::calculateThroughput(const std::vector<Process>& processes, int totalTime) {
    if (totalTime == 0) {
        return 0.0;
    }

    // Aqui se calcula el throughput = Numero de procesos completados / Tiempo total
    return static_cast<double>(processes.size()) / totalTime;
}