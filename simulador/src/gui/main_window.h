#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <string>
#include <memory>
#include "scheduling_view.h"
#include "sync_view.h"

// Enum para los modos de simulacion
enum class SimulationMode {
    SCHEDULING,    // simulacion de calenndarizacion
    SYNCHRONIZATION // simulacion de sync
};

class MainWindow {
private:
    SimulationMode currentMode;         // Modo actual de simulacion
    std::unique_ptr<SchedulingView> schedulingView; // Vista para calendarizacion
    std::unique_ptr<SyncView> syncView;           // Vista para sync
    bool isRunning;                     // Bandera para indicar si la simulacion esta en ejecucion
    int simulationSpeed;                // Velocidad de la simulacion (ms por ciclo)

public:
 
    MainWindow();
    void initialize();
    void run();

    // Cambiar el modo de simulacion
    void setSimulationMode(SimulationMode mode);

    // Cargar archivos para la simulacion actual
    bool loadFiles(const std::string& processFile, 
                  const std::string& resourceFile = "", 
                  const std::string& actionFile = "");

    // Iniciar la simulaciu00f3n
    void startSimulation();

    // Pausar la simulaciu00f3n
    void pauseSimulation();

    // Reiniciar la simulaciu00f3n
    void resetSimulation();

    // Avanzar un paso en la simulaciu00f3n
    void stepSimulation();

    // Establecer la velocidad de la simulaciu00f3n
    void setSimulationSpeed(int speedMs);

private:
    // Inicializar la interfaz grafica
    void initializeGUI();

    // Manejar eventos de la interfaz
    void handleEvents();

    // Renderizar la ventana
    void render();
};

#endif