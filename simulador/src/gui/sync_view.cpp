#include "sync_view.h"
#include "../utils/file_loader.h"
#include <iostream>

SyncView::SyncView()
    : currentMechanism(SyncMechanismType::MUTEX),
      simulationCompleted(false),
      lastTime(0) {
    createSyncMechanism();
}

bool SyncView::loadFiles(const std::string& processFile,
                         const std::string& resourceFile,
                         const std::string& actionFile) {
    try {
        processes = FileLoader::loadProcesses(processFile);
        resources = FileLoader::loadResources(resourceFile);
        actions   = FileLoader::loadActions(actionFile);
        reset();
        std::cout << "\n=== Archivos cargados exitosamente ===\n"
                  << "✓ Procesos: " << processes.size() << " procesos cargados\n"
                  << "✓ Recursos: " << resources.size() << " recursos disponibles\n"
                  << "✓ Acciones: " << actions.size() << " acciones programadas\n"
                  << "=====================================\n" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar archivos: " << e.what() << std::endl;
        return false;
    }
}

void SyncView::setSyncMechanism(SyncMechanismType mechanism) {
    currentMechanism = mechanism;
    createSyncMechanism();
    reset();
}

void SyncView::reset() {
    if (syncMechanism) {
        syncMechanism->initialize(processes, resources, actions);
        lastTimeline.clear();
        lastTime = 0;
        simulationCompleted = false;
    }
}

void SyncView::step() {
    if (!syncMechanism || processes.empty() || resources.empty() ||
        actions.empty() || simulationCompleted)
        return;

    // Ejecuta un tick del mecanismo
    syncMechanism->tick();

    // Guarda estado para GUI
    lastTimeline = syncMechanism->getTimeline();
    lastTime     = syncMechanism->getCurrentTime();

    // Actualiza bandera de fin
    simulationCompleted = syncMechanism->isSimulationFinished();

    // === Mejorado: Actualizar historial textual detallado ===
    eventHistory.clear();
    for (int ciclo = 0; ciclo < lastTime; ++ciclo) {
        for (const auto& act : actions) {
            if (act.getCycle() == ciclo) {
                // Buscar el estado real de la acción en pendingActions o completedActions
                QString estadoAccion = "WAITING";
                // Buscar en completedActions primero
                auto itComp = std::find_if(syncMechanism->getCompletedActions().begin(), syncMechanism->getCompletedActions().end(),
                    [&](const Action& a) {
                        return a.getProcessId() == act.getProcessId() &&
                               a.getResourceName() == act.getResourceName() &&
                               a.getCycle() == act.getCycle();
                    });
                if (itComp != syncMechanism->getCompletedActions().end()) {
                    estadoAccion = "ACCESSED";
                } else {
                    // Buscar en pendingActions si está en ACCESSED
                    auto itPend = std::find_if(syncMechanism->getPendingActions().begin(), syncMechanism->getPendingActions().end(),
                        [&](const Action& a) {
                            return a.getProcessId() == act.getProcessId() &&
                                   a.getResourceName() == act.getResourceName() &&
                                   a.getCycle() == act.getCycle() &&
                                   a.getStatus() == ActionStatus::ACCESSED;
                        });
                    if (itPend != syncMechanism->getPendingActions().end()) {
                        estadoAccion = "ACCESSED";
                    }
                }

                // Estado del recurso
                QString recurso = QString::fromStdString(act.getResourceName());
                QString recursoEstado;
                auto resIt = std::find_if(resources.begin(), resources.end(), [&](const Resource& r){ return r.getName() == act.getResourceName(); });
                if (resIt != resources.end()) {
                    int disp = resIt->getAvailableCount();
                    if (disp > 0) {
                        recursoEstado = QString("%1 disponible").arg(recurso);
                    } else {
                        QString duenio = "-";
                        const auto& cola = resIt->getAccessQueue();
                        if (!cola.empty()) {
                            duenio = QString::fromStdString(cola.front());
                        }
                        recursoEstado = QString("%1 ocupado por %2").arg(recurso, duenio);
                    }
                }

                QString linea = QString("Ciclo %1: %2 %3 %4 → %5 (%6)")
                    .arg(ciclo)
                    .arg(QString::fromStdString(act.getProcessId()))
                    .arg(QString::fromStdString(Action::actionTypeToString(act.getType())))
                    .arg(recurso)
                    .arg(estadoAccion)
                    .arg(recursoEstado);
                eventHistory << linea;
            }
        }
    }
}

bool SyncView::isSimulationCompleted() const {
    return simulationCompleted;
}

// ——— Implementación de los getters para GUI ———

QMap<int, QString> SyncView::getTimeline() const {
    QMap<int, QString> map;
    for (const auto &entry : lastTimeline) {
        map.insert(entry.first, QString::fromStdString(entry.second));
    }
    return map;
}

int SyncView::getCurrentTime() const {
    return lastTime;
}

QStringList SyncView::getEventHistory() const {
    return eventHistory;
}

void SyncView::createSyncMechanism() {
    switch (currentMechanism) {
        case SyncMechanismType::MUTEX:
            syncMechanism = std::make_unique<Mutex>();
            break;
        case SyncMechanismType::SEMAPHORE:
            syncMechanism = std::make_unique<Semaphore>();
            break;
    }
    if (syncMechanism && !processes.empty() && !resources.empty() && !actions.empty()) {
        syncMechanism->initialize(processes, resources, actions);
    }
}
