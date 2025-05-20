#include "gui/main_window.h"
#include "scheduling/priority.h"
#include "scheduling/fifo.h"
#include "scheduling/sjf.h"
#include "scheduling/srt.h"
#include "scheduling/round_robin.h"
#include "models/process.h"
#include "utils/file_loader.h"

#include <iostream>
#include <vector>
#include <string>

std::vector<Process> createTestProcesses() {
    std::vector<Process> processes;
    

    processes.push_back(Process("P1", 5, 0, 3));
    processes.push_back(Process("P2", 4, 1, 1));
    processes.push_back(Process("P3", 7, 2, 2));
    processes.push_back(Process("P4", 3, 3, 4));
    processes.push_back(Process("P5", 6, 4, 2));
    
    return processes;
}

void printResults(const std::vector<Process>& processes) {
    std::cout << "\n--- Resultados finales ---\n";
    std::cout << "PID\tAT\tBT\tPrio\tWT\tTAT\n";
    
    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;
    
    for (const auto& p : processes) {
        std::cout << p.getPID() << "\t" 
                  << p.getArrivalTime() << "\t" 
                  << p.getBurstTime() << "\t" 
                  << p.getPriority() << "\t"
                  << p.getWaitingTime() << "\t"
                  << p.getTurnaroundTime() << "\n";
        
        totalWaitingTime += p.getWaitingTime();
        totalTurnaroundTime += p.getTurnaroundTime();
    }
    
    double avgWaitingTime = totalWaitingTime / processes.size();
    double avgTurnaroundTime = totalTurnaroundTime / processes.size();
    
    std::cout << "\nTiempo de espera promedio: " << avgWaitingTime << std::endl;
    std::cout << "Tiempo de retorno promedio: " << avgTurnaroundTime << std::endl;
}

// funcion para imprimir diagrama de Gantt simplificado
void printGanttChart(const std::map<int, std::string>& timeline, int totalTime) {
    std::cout << "\n--- Diagrama de Gantt ---\n";
    std::cout << "Tiempo: ";
    for (int i = 0; i <= totalTime; i++) {
        std::cout << i << "\t";
    }
    std::cout << "\nProceso: ";
    
    for (int i = 0; i <= totalTime; i++) {
        if (timeline.count(i) > 0) {
            std::cout << timeline.at(i) << "\t";
        } else {
            std::cout << "-\t";
        }
    }
    std::cout << std::endl;
}

// Prueba inicial, probar todos los algoritmos con los mismos procesos
/*
void testAllSchedulers() {
    std::vector<Process> testProcesses = createTestProcesses();
    
    // Array con los nombres de los algoritmos para mostrar
    std::string schedulerNames[] = {"FIFO", "SJF", "SRT", "Round Robin", "Priority (No Preemptivo)", "Priority (Preemptivo)"};
    int testNum = 0;
    
    // 1. Probar FIFO
    std::cout << "\n===============================\n";
    std::cout << "Prueba " << ++testNum << ": " << schedulerNames[0] << std::endl;
    std::cout << "===============================\n";
    
    FIFO fifoScheduler;
    fifoScheduler.initialize(testProcesses);
    
    // Ejecutar simulacion completa
    while (!fifoScheduler.isSimulationFinished()) {
        fifoScheduler.tick();
    }
    
    // mostrar resultados
    printGanttChart(fifoScheduler.getTimeline(), fifoScheduler.getCurrentTime()-1);
    printResults(fifoScheduler.getFinishedProcesses());
    
    // 2. probar SJF
    std::cout << "\n===============================\n";
    std::cout << "Prueba " << ++testNum << ": " << schedulerNames[1] << std::endl;
    std::cout << "===============================\n";
    
    SJF sjfScheduler;
    sjfScheduler.initialize(testProcesses);
    
    // Ejecutar simulacion completa
    while (!sjfScheduler.isSimulationFinished()) {
        sjfScheduler.tick();
    }
    
    // mostrar resultados
    printGanttChart(sjfScheduler.getTimeline(), sjfScheduler.getCurrentTime()-1);
    printResults(sjfScheduler.getFinishedProcesses());
    
    // 3. Probar SRT
    std::cout << "\n===============================\n";
    std::cout << "Prueba " << ++testNum << ": " << schedulerNames[2] << std::endl;
    std::cout << "===============================\n";
    
    SRT srtScheduler;
    srtScheduler.initialize(testProcesses);
    
    // ejecutar simulacion completa
    while (!srtScheduler.isSimulationFinished()) {
        srtScheduler.tick();
    }
    
    // Mostrar resultados
    printGanttChart(srtScheduler.getTimeline(), srtScheduler.getCurrentTime()-1);
    printResults(srtScheduler.getFinishedProcesses());
    
    // 4. Probar Round Robin en este test con el quantum de 2
    std::cout << "\n===============================\n";
    std::cout << "Prueba " << ++testNum << ": " << schedulerNames[3] << " (Quantum=2)" << std::endl;
    std::cout << "===============================\n";
    
    RoundRobin rrScheduler(2);
    rrScheduler.initialize(testProcesses);
    
    // ejecutar simulacion completa
    while (!rrScheduler.isSimulationFinished()) {
        rrScheduler.tick();
    }
    
    // mostrar resultados
    printGanttChart(rrScheduler.getTimeline(), rrScheduler.getCurrentTime()-1);
    printResults(rrScheduler.getFinishedProcesses());
    
    // 5. Probar Priority (No Preemptivo)
    std::cout << "\n===============================\n";
    std::cout << "Prueba " << ++testNum << ": " << schedulerNames[4] << std::endl;
    std::cout << "===============================\n";
    
    Priority priorityNonPreemptive(false);
    priorityNonPreemptive.initialize(testProcesses);
    
    // Ejecutar simulacion completa
    while (!priorityNonPreemptive.isSimulationFinished()) {
        priorityNonPreemptive.tick();
    }
    
    // Mostrar resultados
    printGanttChart(priorityNonPreemptive.getTimeline(), priorityNonPreemptive.getCurrentTime()-1);
    printResults(priorityNonPreemptive.getFinishedProcesses());
    
    // 6. Probar Priority (Preemptivo)
    std::cout << "\n===============================\n";
    std::cout << "Prueba " << ++testNum << ": " << schedulerNames[5] << std::endl;
    std::cout << "===============================\n";
    
    Priority priorityPreemptive(true);
    priorityPreemptive.initialize(testProcesses);
    
    // Ejecutar simulacion completa
    while (!priorityPreemptive.isSimulationFinished()) {
        priorityPreemptive.tick();
    }
    
    // Mostrar resultados
    printGanttChart(priorityPreemptive.getTimeline(), priorityPreemptive.getCurrentTime()-1);
    printResults(priorityPreemptive.getFinishedProcesses());
}
*/

int main(int argc, char *argv[])
{
    std::string exampleName = (argc > 1) ? argv[1] : "ejemplo_fifo";

    FileLoader::ExampleData data;
    try {
        data = FileLoader::loadExample(exampleName);
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar ejemplo: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    /* Aquí puedes decidir, a partir de lo que venga en 'data',
       si corres la simulación de calendarización (solo processes)
       o la de sincronización (processes + resources + actions).   */

    //  Ejemplo rápido: solo calendarización FIFO.
    FIFO fifoScheduler;
    fifoScheduler.initialize(data.processes);
    while (!fifoScheduler.isSimulationFinished())
        fifoScheduler.tick();

    printGanttChart(fifoScheduler.getTimeline(),
                    fifoScheduler.getCurrentTime() - 1);
    printResults(fifoScheduler.getFinishedProcesses());

    return 0;
}
