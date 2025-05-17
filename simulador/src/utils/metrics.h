#ifndef METRICS_H
#define METRICS_H

#include <vector>
#include "../models/process.h"

class Metrics {
public:
    // calculo de tiempo promedio de espera
    static double calculateAverageWaitingTime(const std::vector<Process>& processes);

    // calculo de tiempo promedio de retorno
    static double calculateAverageTurnaroundTime(const std::vector<Process>& processes);

    // calculo de utilizacion de CPU
    static double calculateCPUUtilization(const std::vector<Process>& processes, int totalTime);

    // calculo del throuhgput
    static double calculateThroughput(const std::vector<Process>& processes, int totalTime);
};

#endif
