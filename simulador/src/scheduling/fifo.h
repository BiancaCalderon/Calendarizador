#ifndef FIFO_H
#define FIFO_H

#include "scheduler.h"

// Algoritmo First In First Out (FIFO)
class FIFO : public Scheduler {
public:
    // Constructor
    FIFO();
    
    // Implementación del avance de un ciclo en la simulación
    void tick() override;
    
protected:
    // Seleccionar el siguiente proceso seguido de criterio FIFO
    int selectNextProcess() override;
};

#endif // FIFO_H