#ifndef PRIORITY_H
#define PRIORITY_H

#include "scheduler.h"

// Implementación del algoritmo Priority Scheduling
class Priority : public Scheduler {
private:
    bool preemptive;  // Indica si el algoritmo es preemptivo o no

public:
    Priority(bool isPreemptive = false);
    
    // Setter para modo preemptivo
    void setPreemptive(bool isPreemptive);
    
    // Implementación del avance de un ciclo en la simulación
    void tick() override;
    
protected:
    // Seleccionar el siguiente proceso seguido de criterio de prioridad
    int selectNextProcess() override;
};

#endif // PRIORITY_H