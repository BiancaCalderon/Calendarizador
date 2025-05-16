#ifndef ACTION_H
#define ACTION_H

#include <string>

// Enum para representar los tipos de acciones
enum class ActionType {
    READ,
    WRITE
};

// Enum para representar el estado de la acciu00f3n
enum class ActionStatus {
    WAITING,    // Esperando acceso al recurso
    ACCESSED,   // Con acceso al recurso
    COMPLETED   // Acciu00f3n completada
};

class Action {
private:
    std::string processId;      // ID del proceso que ejecuta la acciu00f3n
    ActionType type;            // Tipo de acciu00f3n (READ o WRITE)
    std::string resourceName;   // Nombre del recurso a acceder
    int cycle;                  // Ciclo en el que se inicia la acciu00f3n
    ActionStatus status;        // Estado actual de la acciu00f3n

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
    
    // Conversiu00f3n de string a ActionType
    static ActionType stringToActionType(const std::string& typeStr);
    
    // Obtener representaciu00f3n string del tipo de acciu00f3n
    static std::string actionTypeToString(ActionType type);
    
    // Obtener representaciu00f3n string del estado de la acciu00f3n
    static std::string actionStatusToString(ActionStatus status);
};

#endif // ACTION_H
