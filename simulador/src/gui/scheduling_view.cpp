#include "scheduling_view.h"
#include "../utils/file_loader.h"
#include <iostream>

SchedulingView::SchedulingView()
    : currentAlgorithm(SchedulerType::FIFO),
      quantum(2),
      simulationCompleted(false) {
    createScheduler();
}

bool SchedulingView::loadProcessFile(const std::string& filename) {
    try {
        processes = FileLoader::loadProcesses(filename);
        reset();
        std::cout << "Archivo de procesos cargado con u00e9xito: " << processes.size() << " procesos." << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar el archivo de procesos: " << e.what() << std::endl;
        return false;
    }
}

void SchedulingView::setSchedulerAlgorithm(SchedulerType algorithm) {
    currentAlgorithm = algorithm;
    createScheduler();
    reset();
}

void SchedulingView::setQuantum(int value) {
    if (value > 0) {
        quantum = value;
        if (currentAlgorithm == SchedulerType::ROUND_ROBIN) {
            createScheduler();
            reset();
        }
    }
}

void SchedulingView::reset() {
    if (scheduler) {
        scheduler->initialize(processes);
        ganttChartView.reset();
        simulationCompleted = false;
    }
}

void SchedulingView::step() {
    if (!scheduler || processes.empty() || simulationCompleted) {
        return;
    }
    
    scheduler->tick();
    
    // Actualizar la vista del diagrama de Gantt
    ganttChartView.updateTimeline(scheduler->getTimeline(), scheduler->getCurrentTime());
    
    simulationCompleted = scheduler->isSimulationFinished();
    
    if (simulationCompleted) {
        std::cout << "Simulaciu00f3n completada." << std::endl;
        std::cout << "Tiempo promedio de espera: " << getAverageWaitingTime() << std::endl;
        std::cout << "Tiempo promedio de retorno: " << getAverageTurnaroundTime() << std::endl;
    }
}

double SchedulingView::getAverageWaitingTime() const {
    return scheduler ? scheduler->getAverageWaitingTime() : 0.0;
}

double SchedulingView::getAverageTurnaroundTime() const {
    return scheduler ? scheduler->getAverageTurnaroundTime() : 0.0;
}

void SchedulingView::render() {
    // Renderizar el diagrama de Gantt
    ganttChartView.render();
    
}

bool SchedulingView::isSimulationCompleted() const {
    return simulationCompleted;
}

void SchedulingView::createScheduler() {
    
    switch (currentAlgorithm) {
        case SchedulerType::FIFO:
            scheduler = std::make_unique<FIFO>();
            break;
            
        case SchedulerType::SJF:
            scheduler = std::make_unique<SJF>();
            break;
            
        case SchedulerType::SRT:
            scheduler = std::make_unique<SRT>();
            break;
            
        case SchedulerType::ROUND_ROBIN:
            scheduler = std::make_unique<RoundRobin>(quantum);
            break;
            
        case SchedulerType::PRIORITY:
            scheduler = std::make_unique<Priority>();
            break;
    }
    
    // Inicializar el scheduler con los procesos actuales
    if (scheduler && !processes.empty()) {
        scheduler->initialize(processes);
    }
}