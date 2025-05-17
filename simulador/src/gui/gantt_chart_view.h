#ifndef GANTT_CHART_VIEW_H
#define GANTT_CHART_VIEW_H

#include <string>
#include <map>
#include <vector>
#include <unordered_map>

class GanttChartView {
private:
    std::map<int, std::string> timeline;      // Historial (tiempo -> ID de proceso)
    int currentTime;                          // Tiempo actual
    int viewportStart;                        // Inicio del viewport (para scroll)
    int viewportWidth;                        // Ancho del viewport
    std::unordered_map<std::string, int> processColors; // Colores para cada proceso

public:
    // Constructor
    GanttChartView();

    // Actualizar con un nuevo estado
    void updateTimeline(const std::map<int, std::string>& newTimeline, int time);

    // Establecer el tiempo actual
    void setCurrentTime(int time);

    // Ajustar el viewport para el scroll
    void scrollLeft(int amount);
    void scrollRight(int amount);

    // Establecer el ancho del viewport
    void setViewportWidth(int width);

    // Renderizar el diagrama de Gantt
    void render();

    // Reiniciar la vista
    void reset();

private:
    // Asignar colores a los procesos
    void assignColors(const std::map<int, std::string>& timeline);

    // Obtener color para un proceso
    int getColorForProcess(const std::string& processId);
};

#endif // GANTT_CHART_VIEW_H