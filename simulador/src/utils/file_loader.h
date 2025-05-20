#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include <string>
#include <vector>

#include "../models/process.h"
#include "../models/resource.h"
#include "../models/action.h"

class FileLoader {
public:
    // Metodos para cargar procesos, recursos y acciones desde archivos
    static std::vector<Process> loadProcesses(const std::string& filename);
    static std::vector<Resource> loadResources(const std::string& filename);
    static std::vector<Action> loadActions(const std::string& filename);

    // metodo para validar el formato de archivo de procesos
    static bool validateProcessFile(const std::string& filename);

    // metodo para validar el formato de archivo de recursos
    static bool validateResourceFile(const std::string& filename);

    // metodo para validar el formato de archivo de acciones
    static bool validateActionFile(const std::string& filename);

    struct ExampleData {
        std::vector<Process>  processes;
        std::vector<Resource> resources;
        std::vector<Action>   actions;
    };

    static ExampleData loadExample(const std::string& exampleName);

private:
    // Metodos auxiliar para las lineas
    static Process parseProcessLine(const std::string& line);
    static Resource parseResourceLine(const std::string& line);
    static Action parseActionLine(const std::string& line);

    // metodo para eliminar espacios en blanco al inicio y fin de una cadena
    static std::string trim(const std::string& str);
};

#endif 