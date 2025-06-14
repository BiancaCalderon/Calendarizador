#include "file_loader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

std::vector<Process> FileLoader::loadProcesses(const std::string& filename) {
    std::vector<Process> processes;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') { 
            try {
                processes.push_back(parseProcessLine(line));
            } catch (const std::exception& e) {
                throw std::runtime_error("Error al procesar línea: " + line + ". " + e.what());
            }
        }
    }
    
    file.close();
    return processes;
}

std::vector<Resource> FileLoader::loadResources(const std::string& filename) {
    std::vector<Resource> resources;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
            try {
                resources.push_back(parseResourceLine(line));
            } catch (const std::exception& e) {
                throw std::runtime_error("Error al procesar línea: " + line + ". " + e.what());
            }
        }
    }
    
    file.close();
    return resources;
}

std::vector<Action> FileLoader::loadActions(const std::string& filename) {
    std::vector<Action> actions;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') { 
            try {
                actions.push_back(parseActionLine(line));
            } catch (const std::exception& e) {
                throw std::runtime_error("Error al procesar línea: " + line + ". " + e.what());
            }
        }
    }
    
    file.close();
    return actions;
}

bool FileLoader::validateProcessFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') { 
            try {
                parseProcessLine(line);
            } catch (const std::exception&) {
                file.close();
                return false;
            }
        }
    }
    
    file.close();
    return true;
}

bool FileLoader::validateResourceFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') { 
            try {
                parseResourceLine(line);
            } catch (const std::exception&) {
                file.close();
                return false;
            }
        }
    }
    
    file.close();
    return true;
}

bool FileLoader::validateActionFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line[0] != '#') {
            try {
                parseActionLine(line);
            } catch (const std::exception&) {
                file.close();
                return false;
            }
        }
    }
    
    file.close();
    return true;
}

Process FileLoader::parseProcessLine(const std::string& line) {
    std::istringstream ss(line);
    std::string item;
    std::vector<std::string> tokens;
    
    // Separar por comas
    while (std::getline(ss, item, ',')) {
        tokens.push_back(trim(item));
    }
    
    if (tokens.size() != 4) {
        throw std::invalid_argument("Formato inválido. El formato debe ser: <PID>, <BurstTime>, <ArrivalTime>, <Priority>");
    }
    
    std::string pid = tokens[0];
    int burstTime, arrivalTime, priority;
    
    try {
        burstTime = std::stoi(tokens[1]);
        arrivalTime = std::stoi(tokens[2]);
        priority = std::stoi(tokens[3]);
    } catch (const std::exception&) {
        throw std::invalid_argument("Error de conversión de tipos. BurstTime, ArrivalTime y Priority deben ser números enteros.");
    }
    
    if (burstTime <= 0) {
        throw std::invalid_argument("BurstTime debe ser un número positivo.");
    }
    
    if (arrivalTime < 0) {
        throw std::invalid_argument("ArrivalTime no puede ser negativo.");
    }
    
    return Process(pid, burstTime, arrivalTime, priority);
}

Resource FileLoader::parseResourceLine(const std::string& line) {
    std::istringstream ss(line);
    std::string item;
    std::vector<std::string> tokens;
    
  
    while (std::getline(ss, item, ',')) {
        tokens.push_back(trim(item));
    }
    
    if (tokens.size() != 2) {
        throw std::invalid_argument("Formato inválido. El formato debe ser: <NombreRecurso>, <Contador>");
    }
    
    std::string name = tokens[0];
    int count;
    
    try {
        count = std::stoi(tokens[1]);
    } catch (const std::exception&) {
        throw std::invalid_argument("Error de conversión de tipos. Contador debe ser un número entero.");
    }
    
    if (count <= 0) {
        throw std::invalid_argument("Contador debe ser un número positivo.");
    }
    
    return Resource(name, count);
}

Action FileLoader::parseActionLine(const std::string& line) {
    std::istringstream ss(line);
    std::string item;
    std::vector<std::string> tokens;
    

    while (std::getline(ss, item, ',')) {
        tokens.push_back(trim(item));
    }
    
    if (tokens.size() != 4) {
        throw std::invalid_argument("Formato inválido. El formato debe ser: <PID>, <Acción>, <Recurso>, <Ciclo>");
    }
    
    std::string pid = tokens[0];
    std::string actionTypeStr = tokens[1];
    std::string resourceName = tokens[2];
    int cycle;
    
    try {
        cycle = std::stoi(tokens[3]);
    } catch (const std::exception&) {
        throw std::invalid_argument("Error de conversión de tipos. Ciclo debe ser un número entero.");
    }
    
    if (cycle < 0) {
        throw std::invalid_argument("Ciclo no puede ser negativo.");
    }
    
    ActionType actionType;
    try {
        actionType = Action::stringToActionType(actionTypeStr);
    } catch (const std::exception& e) {
        throw std::invalid_argument(e.what());
    }
    
    return Action(pid, actionType, resourceName, cycle);
}

std::string FileLoader::trim(const std::string& str) {
    auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char c) {
        return std::isspace(c);
    });
    
    auto end = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) {
        return std::isspace(c);
    }).base();
    
    return (start < end) ? std::string(start, end) : std::string();
}

FileLoader::ExampleData
FileLoader::loadExample(const std::string& name)
{
    fs::path base(EXAMPLES_DIR);
    fs::path pFile = base / "processes"  / (name + ".txt");
    fs::path rFile = base / "resources"  / (name + ".txt");
    fs::path aFile = base / "actions"    / (name + ".txt");

    // Procesos   (obligatorio)
    if (!fs::exists(pFile))
        throw std::runtime_error("No existe archivo de procesos: " + pFile.string());

    ExampleData data;
    data.processes = loadProcesses(pFile.string());

    // Recursos   (opcional)
    if (fs::exists(rFile))
        data.resources = loadResources(rFile.string());

    // Acciones   (opcional)
    if (fs::exists(aFile))
        data.actions   = loadActions(aFile.string());

    return data;
}