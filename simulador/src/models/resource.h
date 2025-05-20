#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include <vector>

class Resource {
private:
    std::string name;          // Nombre del recurso
    int initialCount;          // Contador inicial de instancias disponibles
    int availableCount;        // Contador actual de instancias disponibles
    std::vector<std::string> accessQueue;  // Cola de procesos esperando acceso

public:
    // Constructor
    Resource(const std::string& name, int count);
    
    // Getters
    std::string getName() const;
    int getInitialCount() const;
    int getAvailableCount() const;
    const std::vector<std::string>& getAccessQueue() const;
    
    // Mu00e9todos para manejo de acceso
    bool acquire(const std::string& processId);
    void release(const std::string& processId);
    void release();
    
    // Agregar proceso a la cola de espera
    void addToQueue(const std::string& processId);
    
    // Eliminar proceso de la cola de espera
    void removeFromQueue(const std::string& processId);
    
    // Verificar si un proceso estu00e1 en la cola
    bool isInQueue(const std::string& processId) const;
    
    // Reinicio del recurso a estado inicial
    void reset();
};

#endif // RESOURCE_H
