#ifndef SYNC_VIEW_H
#define SYNC_VIEW_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "../models/process.h"
#include "../models/resource.h"
#include "../models/action.h"
#include "../synchronization/sync_mechanism.h"
#include "../synchronization/mutex.h"
#include "../synchronization/semaphore.h"
#include "gantt_chart_view.h"

// Enum para los tipos de mecanismos de sincronizacion
enum class SyncMechanismType {
    MUTEX,
    SEMAPHORE
};

class SyncView {
private:
    std::unique_ptr<SyncMechanism> syncMechanism;  // Mecanismo de sincronización actual
    std::vector<Process> processes;                // Procesos cargados
    std::vector<Resource> resources;               // Recursos cargados
    std::vector<Action> actions;                   // Acciones cargadas
    SyncMechanismType currentMechanism;            // Mecanismo actual
    bool simulationCompleted;                      // Bandera de simulación completada
    GanttChartView ganttChartView;                 // Vista del diagrama de Gantt

public:
    // Constructor
    SyncView();

    // Cargar archivos de procesos, recursos y acciones
    bool loadFiles(const std::string& processFile, 
                   const std::string& resourceFile, 
                   const std::string& actionFile);

    void setSyncMechanism(SyncMechanismType mechanism);

    void reset();

    void step();

    void render();

    bool isSimulationCompleted() const;

private:

    void createSyncMechanism();
};

#endif // SYNC_VIEW_H