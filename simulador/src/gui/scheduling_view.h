#ifndef SCHEDULING_VIEW_H
#define SCHEDULING_VIEW_H

#include <QString>
#include <QMap>
#include <vector>
#include <memory>
#include <map>
#include "../models/process.h"
#include "../scheduling/scheduler.h"
#include "../scheduling/fifo.h"
#include "../scheduling/sjf.h"
#include "../scheduling/srt.h"
#include "../scheduling/round_robin.h"
#include "../scheduling/priority.h"

class SchedulingView {
public:
    SchedulingView();

    bool loadProcessFile(const std::string &filename);
    void setSchedulerAlgorithm(SchedulerType algorithm);
    void setQuantum(int q);
    void reset();
    void step();

    double getAverageWaitingTime() const;
    double getAverageTurnaroundTime() const;

    // ——— Métodos para la GUI
    QMap<int, QString> getTimeline() const;
    int getCurrentTime() const;
    bool isSimulationCompleted() const;
    
    // Getter para el tamaño de la colección de procesos
    size_t getProcessesSize() const { return processes.size(); }

private:
    void createScheduler();

    std::unique_ptr<Scheduler> scheduler;  
    std::vector<Process> processes;        
    SchedulerType currentAlgorithm;        
    int quantum;                           
    bool simulationCompleted;              

    // ——— Estado interno para la GUI
    std::map<int, std::string> lastTimeline;
    int lastTime = 0;
};

#endif
