#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include "scheduler.h"

// Implementación del algoritmo Round Robin
class RoundRobin : public Scheduler {
private:
    int quantum;          // Quantum de tiempo por proceso
    int timeInQuantum;    // Tiempo que el proceso actual ha estado ejecutándose en este quantum

public:
    RoundRobin(int q = 2);  // Quantum predeterminado de 2
    
    // Setter para el quantum
    void setQuantum(int q);
    
    // Implementación del avance de un ciclo en la simulación
    void tick() override;
    
    // Reiniciar el planificador
    void reset() override;
    
protected:
    // Seleccionar el siguiente proceso seguido de criterio Round Robin
    int selectNextProcess() override;
};

#endif // ROUND_ROBIN_H