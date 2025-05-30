#include "gantt_chart_widget.h"
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QVBoxLayout>
#include <QRandomGenerator>

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
    drawTimeline(timeline, currentTime);
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
    // Dibuja los ciclos desde min hasta max dentro de viewportWidth
    int minCycle = qMax(0, currentTime - viewportWidth + 1);
    int maxCycle = minCycle + viewportWidth - 1;
    int y = 0;

    // Dibujar etiquetas de ciclo
    for (int cycle = minCycle; cycle <= maxCycle; ++cycle) {
        auto *text = scene->addSimpleText(QString::number(cycle));
        text->setPos((cycle - minCycle) * cycleWidth, -20);
    }

    // Dibujar barras de proceso
    for (auto it = timeline.constBegin(); it != timeline.constEnd(); ++it) {
        int cycle = it.key();
        const QString &proc = it.value();
        if (cycle < minCycle || cycle > maxCycle || proc == "-") continue;
        QColor color = getColorForProcess(proc);
        int x = (cycle - minCycle) * cycleWidth;
        auto *rect = scene->addRect(x, y, cycleWidth, cycleHeight,
                                    QPen(Qt::black), QBrush(color));
        auto *label = scene->addSimpleText(proc);
        label->setPos(x + 2, y + 2);
    }

    // Opcional: destacar ciclo actual con línea vertical
    int xCurrent = (currentTime - minCycle) * cycleWidth;
    scene->addLine(xCurrent, 0, xCurrent, cycleHeight,
                   QPen(Qt::red, 2));
}
