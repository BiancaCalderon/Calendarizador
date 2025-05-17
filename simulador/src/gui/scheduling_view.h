#ifndef SCHEDULING_VIEW_H
#define SCHEDULING_VIEW_H

#include <string>
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
#include "gantt_chart_view.h"

class SchedulingView {
private:
    std::unique_ptr<Scheduler> scheduler;  // Scheduler actual
    std::vector<Process> processes;        // Procesos cargados
    SchedulerType currentAlgorithm;        // Algoritmo actual
    int quantum;                           // Quantum para Round Robin
    bool simulationCompleted;              // Bandera de simulaciu00f3n completada
    GanttChartView ganttChartView;         // Vista del diagrama de Gantt
    
public:
    // Constructor
    SchedulingView();
    
    // Cargar archivo de procesos
    bool loadProcessFile(const std::string& filename);
    
    // Establecer el algoritmo de calendarizaciu00f3n
    void setSchedulerAlgorithm(SchedulerType algorithm);
    
    // Establecer el quantum para Round Robin
    void setQuantum(int value);
    
    // Reiniciar la simulaciu00f3n
    void reset();
    
    // Avanzar un paso en la simulaciu00f3n
    void step();
    
    // Obtener las mu00e9tricas actuales
    double getAverageWaitingTime() const;
    double getAverageTurnaroundTime() const;
    
    // Renderizar la vista
    void render();
    
    // Verificar si la simulaciu00f3n ha terminado
    bool isSimulationCompleted() const;
    
private:
    // Crear el scheduler apropiado segu00fan el algoritmo seleccionado
    void createScheduler();
};

#endif // SCHEDULING_VIEW_H