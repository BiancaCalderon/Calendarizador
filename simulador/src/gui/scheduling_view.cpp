#include "scheduling_view.h"
#include "../utils/file_loader.h"
#include <iostream>

SchedulingView::SchedulingView()
    : currentAlgorithm(SchedulerType::FIFO),
      quantum(2),
      simulationCompleted(false),
      lastTime(0) {
    createScheduler();
}

bool SchedulingView::loadProcessFile(const std::string& filename) {
    try {
        processes = FileLoader::loadProcesses(filename);
        reset();
        std::cout << "\n=== Archivo de procesos cargado exitosamente ===\n"
                  << "✓ " << processes.size() << " procesos cargados\n"
                  << "=============================================\n" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar el archivo de procesos: "
                  << e.what() << std::endl;
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
        lastTimeline.clear();
        lastTime = 0;
        simulationCompleted = false;
    }
}

void SchedulingView::step() {
    if (!scheduler || processes.empty() || simulationCompleted)
        return;

    // Ejecuta un tick del scheduler
    scheduler->tick();

    // Guarda estado para GUI
    lastTimeline = scheduler->getTimeline();
    lastTime     = scheduler->getCurrentTime();

    // Marca fin de simulación
    simulationCompleted = scheduler->isSimulationFinished();
}

double SchedulingView::getAverageWaitingTime() const {
    return scheduler ? scheduler->getAverageWaitingTime() : 0.0;
}

double SchedulingView::getAverageTurnaroundTime() const {
    return scheduler ? scheduler->getAverageTurnaroundTime() : 0.0;
}

// ——— Implementación de los getters para GUI ———

QMap<int, QString> SchedulingView::getTimeline() const {
    QMap<int, QString> map;
    for (const auto &entry : lastTimeline) {
        map.insert(entry.first, QString::fromStdString(entry.second));
    }
    return map;
}

int SchedulingView::getCurrentTime() const {
    return lastTime;
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
    if (scheduler && !processes.empty())
        scheduler->initialize(processes);
}
