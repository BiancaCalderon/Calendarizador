#ifndef SRT_H
#define SRT_H

#include "scheduler.h"

// Implementación del algoritmo Shortest Remaining Time (SRT)
class SRT : public Scheduler {
public:
    SRT();
    
    // Implementación del avance de un ciclo en la simulación
    void tick() override;
    
protected:
    // Seleccionar el siguiente proceso seguido de criterio de menor tiempo restante
    int selectNextProcess() override;
};

#endif // SRT_H