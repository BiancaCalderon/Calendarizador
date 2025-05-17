#ifndef MUTEX_H
#define MUTEX_H

#include "sync_mechanism.h"

class Mutex : public SyncMechanism {
public:
 
    Mutex();

    // implementacion del avance de un ciclo en la simulacion
    void tick() override;

protected:
    // procesar las acciones pendientes con el mecanismo de mutex
    void processActions() override;
};

#endif