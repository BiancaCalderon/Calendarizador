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
