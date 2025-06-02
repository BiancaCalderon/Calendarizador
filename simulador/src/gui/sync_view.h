#ifndef SYNC_VIEW_H
#define SYNC_VIEW_H

#include <QString>
#include <QMap>
#include <vector>
#include <memory>
#include <map>
#include "../models/process.h"
#include "../models/resource.h"
#include "../models/action.h"
#include "../synchronization/sync_mechanism.h"
#include "../synchronization/mutex.h"
#include "../synchronization/semaphore.h"

enum class SyncMechanismType {
    MUTEX,
    SEMAPHORE
};

class SyncView {
public:
    SyncView();

    bool loadFiles(const std::string &processFile,
                   const std::string &resourceFile,
                   const std::string &actionFile);
    void setSyncMechanism(SyncMechanismType mechanism);
    void reset();
    void step();
    bool isSimulationCompleted() const;

    // ——— Métodos para la GUI
    QMap<int, QString> getTimeline() const;
    int getCurrentTime() const;
    
    // Getters para el tamaño de las colecciones
    size_t getProcessesSize() const { return processes.size(); }
    size_t getResourcesSize() const { return resources.size(); }
    size_t getActionsSize() const { return actions.size(); }

private:
    void createSyncMechanism();

    std::unique_ptr<SyncMechanism> syncMechanism;
    std::vector<Process> processes;
    std::vector<Resource> resources;
    std::vector<Action> actions;
    SyncMechanismType currentMechanism;
    bool simulationCompleted;

    // ——— Estado interno para la GUI
    std::map<int, std::string> lastTimeline;
    int lastTime = 0;
};

#endif
