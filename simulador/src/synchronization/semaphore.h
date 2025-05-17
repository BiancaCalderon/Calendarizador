#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "sync_mechanism.h"

class Semaphore : public SyncMechanism {
public:
 
    Semaphore();

    // Implementacion del avance de un ciclo en la simulacion
    void tick() override;

protected:
    // Procesar las acciones pendientes con el mecanismo de semaforo
    void processActions() override;
};

#endif