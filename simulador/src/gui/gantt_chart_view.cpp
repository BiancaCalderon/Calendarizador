#include "gantt_chart_view.h"
#include <iostream>
#include <algorithm>
#include <random>

GanttChartView::GanttChartView()
    : currentTime(0), viewportStart(0), viewportWidth(20) {
}

void GanttChartView::updateTimeline(const std::map<int, std::string>& newTimeline, int time) {
    timeline = newTimeline;
    currentTime = time;
    
    // Asignar colores a nuevos procesos
    assignColors(timeline);
    
    // Ajustar el viewport para mantener visible el tiempo actual
    if (currentTime >= viewportStart + viewportWidth) {
        viewportStart = currentTime - viewportWidth + 1;
    }
}

void GanttChartView::setCurrentTime(int time) {
    currentTime = time;
    
    // Ajustar el viewport para mantener visible el tiempo actual
    if (currentTime >= viewportStart + viewportWidth) {
        viewportStart = currentTime - viewportWidth + 1;
    }
}

void GanttChartView::scrollLeft(int amount) {
    viewportStart = std::max(0, viewportStart - amount);
}

void GanttChartView::scrollRight(int amount) {
    int maxTime = timeline.empty() ? 0 : timeline.rbegin()->first;
    viewportStart = std::min(maxTime, viewportStart + amount);
}

void GanttChartView::setViewportWidth(int width) {
    if (width > 0) {
        viewportWidth = width;
    }
}

void GanttChartView::render() {

    std::cout << "Diagrama de Gantt (Ciclos " << viewportStart << " a " 
              << (viewportStart + viewportWidth - 1) << "):" << std::endl;
    
    std::cout << "Ciclo:  ";
    for (int i = viewportStart; i < viewportStart + viewportWidth; i++) {
        std::cout << i << "\t";
    }
    std::cout << std::endl;
    
    std::cout << "Proceso:";
    for (int i = viewportStart; i < viewportStart + viewportWidth; i++) {
        auto it = timeline.find(i);
        if (it != timeline.end()) {
            std::cout << it->second << "\t";
        } else {
            std::cout << "-\t";
        }
    }
    std::cout << std::endl;
    
    // Marcador del tiempo actual
    std::cout << "Actual: ";
    for (int i = viewportStart; i < viewportStart + viewportWidth; i++) {
        if (i == currentTime) {
            std::cout << "^\t";
        } else {
            std::cout << " \t";
        }
    }
    std::cout << std::endl;
}

void GanttChartView::reset() {
    timeline.clear();
    currentTime = 0;
    viewportStart = 0;
    // Mantener los colores asignados para consistencia visual
}

void GanttChartView::assignColors(const std::map<int, std::string>& timeline) {
    // Recolectar todos los IDs de procesos u00fanicos
    for (const auto& entry : timeline) {
        const std::string& processId = entry.second;
        if (processId != "-" && processColors.find(processId) == processColors.end()) {
            // Asignar un color aleatorio
            processColors[processId] = getColorForProcess(processId);
        }
    }
}

int GanttChartView::getColorForProcess(const std::string& processId) {
    // Si ya tiene un color asignado, retornarlo
    auto it = processColors.find(processId);
    if (it != processColors.end()) {
        return it->second;
    }
    
    
    std::hash<std::string> hasher;
    std::size_t hash = hasher(processId);
    
    // Limitar el rango de colores (supongamos 8 colores distintos)
    return hash % 8;
}