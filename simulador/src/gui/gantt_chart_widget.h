#ifndef GANTT_CHART_WIDGET_H
#define GANTT_CHART_WIDGET_H

#include <QGraphicsView>
#include <QMap>
#include <QString>

class QGraphicsScene;

class GanttChartWidget : public QGraphicsView {
    Q_OBJECT

public:
    explicit GanttChartWidget(QWidget *parent = nullptr);
    void updateTimeline(const QMap<int, QString> &timeline, int currentTime);
    void reset();

private:
    QGraphicsScene *scene;
    int viewportWidth;  // número de ciclos mostrados
    int cycleHeight;    // alto de cada barra
    int cycleWidth;     // ancho de cada ciclo en píxeles
    QMap<QString, QColor> processColors;

    QColor getColorForProcess(const QString &procId);
    void drawTimeline(const QMap<int, QString> &timeline, int currentTime);
};

#endif