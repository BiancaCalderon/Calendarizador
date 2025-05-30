#include "metrics_panel_widget.h"
#include <QLabel>
#include <QHBoxLayout>

MetricsPanelWidget::MetricsPanelWidget(QWidget *parent)
    : QWidget(parent) {
    waitingLabel     = new QLabel("AVG Waiting Time: N/A", this);
    turnaroundLabel  = new QLabel("Turnaround Time: N/A", this);

    auto *layout = new QHBoxLayout(this);
    layout->addWidget(waitingLabel);
    layout->addSpacing(20);
    layout->addWidget(turnaroundLabel);
    setLayout(layout);
}

void MetricsPanelWidget::updateMetrics(double waitingTime, double turnaroundTime) {
    waitingLabel->setText(QString("AVG Waiting Time: %1").arg(waitingTime, 0, 'f', 2));
    turnaroundLabel->setText(QString("Turnaround Time: %1").arg(turnaroundTime, 0, 'f', 2));
}

void MetricsPanelWidget::clear() {
    waitingLabel->setText("AVG Waiting Time: N/A");
    turnaroundLabel->setText("Turnaround Time: N/A");
}