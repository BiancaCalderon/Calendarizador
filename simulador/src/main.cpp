#include "gui/main_window.h"
#include "scheduling/priority.h"
#include "scheduling/fifo.h"
#include "scheduling/sjf.h"
#include "scheduling/srt.h"
#include "scheduling/round_robin.h"
#include "models/process.h"
#include "utils/file_loader.h"
#include "synchronization/sync_sim.h" 

#include <iostream>
#include <vector>
#include <string>
#include <limits>

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

/* Prueba inicial, probar todos los algoritmos con los mismos procesos
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

static int askInt(const std::string& msg, int min, int max) {
    int v;
    while (true) {
        std::cout << msg;  std::cin >> v;
        if (std::cin && v >= min && v <= max) { std::cin.ignore(); return v; }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Entrada no válida. Inténtelo de nuevo.\n";
    }
}

static std::string askStr(const std::string& msg) {
    std::string s;
    std::cout << msg;
    std::getline(std::cin >> std::ws, s);
    return s;
}

int main() {
    while (true) {
        bool simulacionEjecutada = false;

        std::cout << "\n=== Simulador SO ===\n";
        std::cout << "1) Programación (Calendarización)\n"
                     "2) Sincronización (Mutex/Semáforo)\n"
                     "3) Salir del programa\n";
        int mode = askInt("\nSeleccione el tipo de simulación: ", 1, 3);
        if (mode == 3) break;

        FileLoader::ExampleData data;
        while (true) {
            std::string example = askStr("\nEjemplo de nombre (sin .txt): ");
            try {
                data = FileLoader::loadExample(example);
                break;
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << '\n';
            }
        }

        /* Programación */
        if (mode == 1) {
            while (true) {
                std::cout << "\nAlgoritmos:\n"
                             "1) FIFO   2) SJF   3) SRT   4) Round-Robin   5) Priority 6) Volver\n";
                int alg = askInt("Elige algoritmo: ", 1, 6);
                if (alg == 6) break;

                int q = 6;
                if (alg == 4) q = askInt("Quantum para RR: ", 1, 100);

                std::unique_ptr<Scheduler> scheduler;
                switch (alg) {
                    case 1: scheduler = std::make_unique<FIFO>(); break;
                    case 2: scheduler = std::make_unique<SJF>();  break;
                    case 3: scheduler = std::make_unique<SRT>();  break;
                    case 4: scheduler = std::make_unique<RoundRobin>(q); break;
                    case 5: scheduler = std::make_unique<Priority>();    break;
                }

                scheduler->initialize(data.processes);
                while (!scheduler->isSimulationFinished()) scheduler->tick();

                printGanttChart(scheduler->getTimeline(), scheduler->getCurrentTime() - 1);
                printResults(scheduler->getFinishedProcesses());
                simulacionEjecutada = true;
                break;
            }
        }
        /* Sincronización */
        else if (mode == 2) {
            if (data.resources.empty() || data.actions.empty()) {
                std::cerr << "El ejemplo debe incluir recursos y archivos de acciones.\n";
                continue;   
            }
            while (true) {
                std::cout << "\nMecanismo:\n1) Mutex   2) Semáforo 3) Volver\n";
                int mech = askInt("Elige el mecanismo: ", 1, 3);
                if (mech == 3) break;

                /* SyncSim ya llama a Resource::acquire()/release()
                   → funciona tanto para mutex (count=1) como para semáforo (count>1).            */
                SyncSim sim;
                sim.initialize(data.processes, data.resources, data.actions, mech == 1);
                sim.run();          // imprime línea de tiempo ACCESSED/WAITING
                simulacionEjecutada = true;
                break;
            }
        }

        if (simulacionEjecutada) {
            std::cout << "\n¿Desea realizar otra simulación? (s/n): ";
            char again;
            std::cin >> again;
            if (again != 's' && again != 'S') {
                std::cout << "Saliendo del simulador...\n";
                break;
            }
        }
    }
    return 0;
}
