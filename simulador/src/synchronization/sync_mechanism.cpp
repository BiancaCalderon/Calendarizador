#ifndef SYNC_MECHANISM_H
#define SYNC_MECHANISM_H

#include <vector>
#include <string>
#include <map>
#include <queue>
#include "../models/process.h"
#include "../models/resource.h"
#include "../models/action.h"

class SyncMechanism {
protected:
    std::vector<Process> processes;      // Lista de procesos
    std::vector<Resource> resources;     // Lista de recursos
    std::vector<Action> actions;         // Lista de acciones a realizar
    std::vector<Action> pendingActions;  // Acciones pendientes de ejecucion
    std::vector<Action> completedActions; // Acciones completadas
    int currentTime;                     // Tiempo actual de la simulacion
    std::map<int, std::string> timeline; // Historial de eventos

public:

    SyncMechanism();
    virtual ~SyncMechanism() = default;

    virtual void initialize(const std::vector<Process>& processes,
                           const std::vector<Resource>& resources,
                           const std::vector<Action>& actions);
    
    // Avanzar un ciclo en la simulacion
    virtual void tick() = 0;
    
    // Obtener el tiempo actual
    int getCurrentTime() const;
    
    // Obtener la linea de tiempo para el diagrama de Gantt
    const std::map<int, std::string>& getTimeline() const;
    
    // Verificar si la simulacion ha terminado
    bool isSimulationFinished() const;
    

    virtual void reset();
    
protected:
    // Actualizar acciones pendientes para el tiempo actual
    virtual void updatePendingActions();
    

    virtual void processActions() = 0;
    
   
    Resource* getResourceByName(const std::string& name);
    
    void updateTimeline(const std::string& processId, ActionStatus status);
};

#endif // SYNC_MECHANISM_H