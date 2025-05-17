#include "sync_view.h"
#include "../utils/file_loader.h"
#include <iostream>

SyncView::SyncView()
    : currentMechanism(SyncMechanismType::MUTEX),
      simulationCompleted(false) {
    createSyncMechanism();
}

bool SyncView::loadFiles(const std::string& processFile, 
                         const std::string& resourceFile, 
                         const std::string& actionFile) {
    try {
        processes = FileLoader::loadProcesses(processFile);
        resources = FileLoader::loadResources(resourceFile);
        actions = FileLoader::loadActions(actionFile);
        
        reset();
        
        std::cout << "Archivos cargados con u00e9xito." << std::endl;
        std::cout << "Procesos: " << processes.size() << std::endl;
        std::cout << "Recursos: " << resources.size() << std::endl;
        std::cout << "Acciones: " << actions.size() << std::endl;
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar los archivos: " << e.what() << std::endl;
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
        ganttChartView.reset();
        simulationCompleted = false;
    }
}

void SyncView::step() {
    if (!syncMechanism || processes.empty() || resources.empty() || actions.empty() || simulationCompleted) {
        return;
    }
    
    // Avanzar un paso
    syncMechanism->tick();
    
    // Actualizar la vista del diagrama de Gantt
    ganttChartView.updateTimeline(syncMechanism->getTimeline(), syncMechanism->getCurrentTime());
    
    // Verificar si la simulación ha terminado
    simulationCompleted = syncMechanism->isSimulationFinished();
    
    if (simulationCompleted) {
        std::cout << "Simulaciu00f3n de sincronizaciu00f3n completada." << std::endl;
    }
}

void SyncView::render() {
    // Renderizar el diagrama de Gantt
    ganttChartView.render();
    
}

bool SyncView::isSimulationCompleted() const {
    return simulationCompleted;
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
    
    // Inicializar el mecanismo con los datos actuales
    if (syncMechanism && !processes.empty() && !resources.empty() && !actions.empty()) {
        syncMechanism->initialize(processes, resources, actions);
    }
}