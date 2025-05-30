#ifndef METRICS_PANEL_WIDGET_H
#define METRICS_PANEL_WIDGET_H

#include <QWidget>

class QLabel;

class MetricsPanelWidget : public QWidget {
    Q_OBJECT

public:
    explicit MetricsPanelWidget(QWidget *parent = nullptr);
    void updateMetrics(double waitingTime, double turnaroundTime);
    void clear();

private:
    QLabel *waitingLabel;
    QLabel *turnaroundLabel;
};

#endif