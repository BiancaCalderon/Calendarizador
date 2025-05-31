#ifndef ACTION_H
#define ACTION_H

#include <string>

// Enum para representar los tipos de acciones
enum class ActionType {
    READ,
    WRITE
};

// Enum para representar el estado de la acción
enum class ActionStatus {
    WAITING,    // Esperando acceso al recurso
    ACCESSED,   // Con acceso al recurso
    COMPLETED   // Acción completada
};

class Action {
private:
    std::string processId;      // ID del proceso que ejecuta la acción
    ActionType type;            // Tipo de acción (READ o WRITE)
    std::string resourceName;   // Nombre del recurso a acceder
    int cycle;                  // Ciclo en el que se inicia la acción
    ActionStatus status;        // Estado actual de la acción

public:
    // Constructor
    Action(const std::string& processId, ActionType type, 
           const std::string& resourceName, int cycle);
    
    // Getters
    std::string getProcessId() const;
    ActionType getType() const;
    std::string getResourceName() const;
    int getCycle() const;
    ActionStatus getStatus() const;
    
    // Setter para el estado
    void setStatus(ActionStatus newStatus);
    
    // Conversión de string a ActionType
    static ActionType stringToActionType(const std::string& typeStr);
    
    // Obtener representación string del tipo de acción
    static std::string actionTypeToString(ActionType type);
    
    // Obtener representación string del estado de la acción
    static std::string actionStatusToString(ActionStatus status);
};

#endif // ACTION_H
