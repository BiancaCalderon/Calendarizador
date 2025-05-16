#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <string>
#include <memory>
#include <map>
#include "../models/process.h"

// Enum para los tipos de algoritmos de calendarización
enum class SchedulerType {
    FIFO,
    SJF,
    SRT,
    ROUND_ROBIN,
    PRIORITY
};

// Estructura para representar un evento en el diagrama de Gantt
struct GanttEvent {
    std::string processId;  // ID del proceso o "IDLE" si ninguno está ejecutando
    int startTime;          // Tiempo de inicio
    int endTime;            // Tiempo de finalización
};

// Clase base para todos los algoritmos de calendarización
class Scheduler {
protected:
    std::vector<Process> processes;      // Lista de procesos a calendarizar
    std::vector<Process> readyQueue;     // Cola de procesos listos para ejecutar
    std::vector<Process> finishedProcesses; // Procesos completados
    int currentTime;                     // Tiempo actual de simulación
    int currentProcessIndex;             // índice del proceso actual en ejecución
    bool isPreemptive;                   // Si el algoritmo es preemptive o no
    std::map<int, std::string> timeline; // Historial de ejecución (para diagrama de Gantt)

public:
    // Constructor
    Scheduler();
    
    // Destructor virtual para permitir herencia
    virtual ~Scheduler() = default;
    
    // Inicializar el scheduler con una lista de procesos
    virtual void initialize(const std::vector<Process>& processes);
    
    // Avanzar un ciclo en la simulación
    virtual void tick() = 0;
    
    // Simular hasta que todos los procesos se completen
    virtual void runSimulation();
    
    // Reiniciar la simulación
    virtual void reset();
    
    // Obtener el tiempo actual
    int getCurrentTime() const;
    
    // Obtener la lista de procesos
    const std::vector<Process>& getProcesses() const;
    
    // Obtener la cola de procesos listos
    const std::vector<Process>& getReadyQueue() const;
    
    // Obtener la lista de procesos completados
    const std::vector<Process>& getFinishedProcesses() const;
    
    // Obtener la línea de tiempo para el diagrama de Gantt
    const std::map<int, std::string>& getTimeline() const;
    
    // Verificar si la simulación ha terminado
    bool isSimulationFinished() const;
    
    // Obtener métricas de rendimiento
    double getAverageWaitingTime() const;
    double getAverageTurnaroundTime() const;
    
protected:
    // Actualizar la cola de procesos listos con los procesos que llegan en el tiempo actual
    virtual void updateReadyQueue();
    
    // Seleccionar el siguiente proceso a ejecutar (implementado por las clases hijas)
    virtual int selectNextProcess() = 0;
    
    // Actualizar tiempos de espera para los procesos en la cola de listos
    virtual void updateWaitingTimes();
    
    // Agregar el proceso actual a la línea de tiempo
    virtual void updateTimeline(const std::string& processId);
};

#endif // SCHEDULER_H