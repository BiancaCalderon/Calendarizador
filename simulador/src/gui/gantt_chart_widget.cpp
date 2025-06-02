#include "gantt_chart_widget.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QScrollBar>

GanttChartWidget::GanttChartWidget(QWidget *parent)
    : QGraphicsView(parent), viewportWidth(20), cycleHeight(30), cycleWidth(40) {
    scene = new QGraphicsScene(this);
    setScene(scene);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void GanttChartWidget::updateTimeline(const QMap<int, QString> &timeline, int currentTime) {
    scene->clear();
    
    if (timeline.isEmpty()) {
        // Si no hay datos en el timeline, resetear la escena.
        scene->setSceneRect(0, 0, 0, 0);
        return;
    }

    // Calcular el rango total de ciclos
    int firstCycle = timeline.constBegin().key();
    int lastCycle = timeline.constEnd().key(); // timeline.constEnd() apunta despues del ultimo elemento
    if (!timeline.isEmpty()) { // Corregir para obtener la clave del ultimo elemento
        lastCycle = (timeline.constEnd() - 1).key(); // QMap iterators are random access
    }

    // Calcular el ancho total de la escena necesario para todos los ciclos
    int totalWidth = (lastCycle - firstCycle + 1) * cycleWidth;
    // Considerar un margen extra al final
    int margin = 5 * cycleWidth; // Unos ciclos extra de margen
    totalWidth += margin;

    // Establecer el tamaño de la escena para habilitar el scroll
    // El alto puede ser fijo o basado en el numero de procesos, por ahora fijo.
    int totalHeight = cycleHeight + 40; // Espacio para etiquetas de ciclo
    scene->setSceneRect(0, -30, totalWidth, totalHeight + 30); // Ajustar rectangulo de escena (considerando etiquetas arriba)

    drawTimeline(timeline, currentTime);
    
    // Asegurar que la vista muestre el inicio del timeline (ciclo 0)
    // Usar ensureVisible en lugar de setValue(0) en el scroll bar
    ensureVisible(0, 0, 0, 0); // Asegurar que el punto (0,0) sea visible
}

void GanttChartWidget::reset() {
    scene->clear();
    processColors.clear();
}

QColor GanttChartWidget::getColorForProcess(const QString &procId) {
    if (processColors.contains(procId))
        return processColors.value(procId);
    QColor color = QColor::fromHsv(QRandomGenerator::global()->bounded(360), 200, 200);
    processColors.insert(procId, color);
    return color;
}

void GanttChartWidget::drawTimeline(const QMap<int, QString> &timeline, int currentTime) {
    // Dibuja los ciclos visibles en el viewport actual
    // El viewport es controlado por el QGraphicsView y su scrollbar.
    // Simplemente dibujamos todo el timeline; el QGraphicsView se encarga de mostrar la parte visible.

    int y = 0;

    // Dibujar etiquetas de ciclo y barras de proceso para todos los ciclos en el timeline
    for (auto it = timeline.constBegin(); it != timeline.constEnd(); ++it) {
        int cycle = it.key();
        const QString &proc = it.value();
        
        // Separar el ID del proceso y el estado
        QStringList parts = proc.split("|");
        QString processId = parts[0];
        QString status = parts.size() > 1 ? parts[1] : "";
        
        QColor color;
        if (status == "ACCESSED") {
            color = QColor(0, 255, 0); // Verde para ACCESSED
        } else if (status == "WAITING") {
            color = QColor(255, 0, 0); // Rojo para WAITING
        } else {
            color = getColorForProcess(processId); // Color normal para otros casos
        }
        
        int x = cycle * cycleWidth;

        // Dibujar barra (rectangulo)
        auto *rect = scene->addRect(x, y, cycleWidth, cycleHeight,
                                    QPen(Qt::black), QBrush(color));

        // Dibujar etiqueta (texto)
        auto *label = scene->addSimpleText(processId);
        label->setPos(x + 2, y + 2);
    }
    
    // Dibujar etiquetas de ciclo
    if (!timeline.isEmpty()) {
        int firstCycle = timeline.constBegin().key();
        int lastCycle = (timeline.constEnd() - 1).key();
        for (int cycle = firstCycle; cycle <= lastCycle + 5; ++cycle) {
            auto *text = scene->addSimpleText(QString::number(cycle));
            text->setPos(cycle * cycleWidth, -20);
        }
    }

    // Dibujar línea del tiempo actual
    if (!timeline.isEmpty()) {
        int firstCycle = timeline.constBegin().key();
        int xCurrent = (currentTime - firstCycle) * cycleWidth;
        scene->addLine(xCurrent, -30, xCurrent, y + cycleHeight + 10,
                     QPen(Qt::blue, 2));
    }
}
