#include "main_window.h"
#include <iostream>

MainWindow::MainWindow()
    : currentMode(SimulationMode::SCHEDULING),
      isRunning(false),
      simulationSpeed(500) { // velocidad
}

void MainWindow::initialize() {
    // Inicializar las vistas
    schedulingView = std::make_unique<SchedulingView>();
    syncView = std::make_unique<SyncView>();
    
    // Inicializar la GUI
    initializeGUI();
    
    std::cout << "Ventana principal inicializada." << std::endl;
}

void MainWindow::run() {
    // bucle para el ciclo de vida del simulador
    while (true) {
        
        handleEvents();
        
        // Si la simulacion esta en ejecucion, avanzar un paso
        if (isRunning) {
            stepSimulation();
            
        render();
    }
}

void MainWindow::setSimulationMode(SimulationMode mode) {
    // Si el modo es diferente al actual, cambiar y reiniciar
    if (mode != currentMode) {
        currentMode = mode;
        resetSimulation();
    }
}

bool MainWindow::loadFiles(const std::string& processFile, 
                         const std::string& resourceFile, 
                         const std::string& actionFile) {
    // Cargar archivos segun el modo actual
    switch (currentMode) {
        case SimulationMode::SCHEDULING:
            return schedulingView->loadProcessFile(processFile);
            
        case SimulationMode::SYNCHRONIZATION:
            if (resourceFile.empty() || actionFile.empty()) {
                std::cerr << "Para el modo de sincronizaciu00f3n se requieren archivos de recursos y acciones." << std::endl;
                return false;
            }
            return syncView->loadFiles(processFile, resourceFile, actionFile);
            
        default:
            return false;
    }
}

void MainWindow::startSimulation() {
    isRunning = true;
}

void MainWindow::pauseSimulation() {
    isRunning = false;
}

void MainWindow::resetSimulation() {
    // Detener la simulacion primero
    pauseSimulation();
    
    // Resetear la vista actual
    switch (currentMode) {
        case SimulationMode::SCHEDULING:
            schedulingView->reset();
            break;
            
        case SimulationMode::SYNCHRONIZATION:
            syncView->reset();
            break;
    }
}

void MainWindow::stepSimulation() {
    // Avanzar un paso en la simulacion actual
    switch (currentMode) {
        case SimulationMode::SCHEDULING:
            schedulingView->step();
            break;
            
        case SimulationMode::SYNCHRONIZATION:
            syncView->step();
            break;
    }
}

void MainWindow::setSimulationSpeed(int speedMs) {
    if (speedMs > 0) {
        simulationSpeed = speedMs;
    }
}

void MainWindow::initializeGUI() {

    std::cout << "GUI inicializada en modo " 
              << (currentMode == SimulationMode::SCHEDULING ? "Calendarizaciu00f3n" : "Sincronizaciu00f3n")
              << std::endl;
}

void MainWindow::handleEvents() {
  
}

void MainWindow::render() {

    switch (currentMode) {
        case SimulationMode::SCHEDULING:
            schedulingView->render();
            break;
            
        case SimulationMode::SYNCHRONIZATION:
            syncView->render();
            break;
    }
}
