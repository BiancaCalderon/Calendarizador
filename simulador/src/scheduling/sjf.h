#ifndef SJF_H
#define SJF_H

#include "scheduler.h"

// Implementación del algoritmo Shortest Job First (SJF)
class SJF : public Scheduler {
public:
    SJF();
    
    // Implementación del avance de un ciclo en la simulación
    void tick() override;
    
protected:
    // Seleccionar el siguiente proceso seguido criterio de menor burst time
    int selectNextProcess() override;
};

#endif // SJF_H