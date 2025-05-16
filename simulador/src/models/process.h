#ifndef PROCESS_H
#define PROCESS_H

#include <string>

// Clase que representa un proceso en el sistema
class Process {
private:
    std::string pid;        // Identificador del proceso
    int burstTime;          // Tiempo de ejecución
    int arrivalTime;        // Tiempo de llegada
    int priority;           // Prioridad (menor número = mayor prioridad)
    int remainingTime;      // Tiempo restante de ejecución
    int waitingTime;        // Tiempo de espera
    int turnaroundTime;     // Tiempo de retorno

public:
    // Constructor
    Process(const std::string& pid, int burstTime, int arrivalTime, int priority);
    
    // Getters
    std::string getPID() const;
    int getBurstTime() const;
    int getArrivalTime() const;
    int getPriority() const;
    int getRemainingTime() const;
    int getWaitingTime() const;
    int getTurnaroundTime() const;
    
    // Setters
    void setRemainingTime(int time);
    void setWaitingTime(int time);
    void setTurnaroundTime(int time);
    
    // Actualización de tiempo restante
    void decrementRemainingTime();
    
    // Verificar si el proceso ha terminado
    bool isCompleted() const;
};

#endif // PROCESS_H