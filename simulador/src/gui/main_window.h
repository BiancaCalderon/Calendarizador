#ifndef MAIN_WINDOW_GUI_H
#define MAIN_WINDOW_GUI_H

#include "scheduling_view.h"
#include "sync_view.h"

#include <QMainWindow>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

#include "../scheduling/scheduler.h"
#include "../scheduling/fifo.h"
#include "../scheduling/sjf.h"
#include "../scheduling/srt.h"
#include "../scheduling/round_robin.h"
#include "../scheduling/priority.h"
#include "gantt_chart_widget.h"
#include "file_loader_widget.h"
#include "algorithm_config_widget.h"
#include "control_panel_widget.h"
#include "metrics_panel_widget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void onModeChanged(int index);
    void onFilesLoaded(const QString &proc,
                       const QString &res,
                       const QString &act);
    void onAlgorithmChanged(int idx);
    void onQuantumChanged(int q);
    void onStartClicked();
    void onPauseClicked();
    void onStepClicked();
    void onResetClicked();
    void onTimerTimeout();
    void onSpeedChanged(int speed);

private:
    void setupUi();
    void connectSignals();
    void executeSimulationStep();

    int stepCounter = 0;
    const int maxSteps = 10000;

    // Widgets
    QWidget              *centralWidget;
    QVBoxLayout          *mainLayout;
    QComboBox            *modeCombo;       // Calendarización / Sincronización
    FileLoaderWidget     *loaderWidget;
    AlgorithmConfigWidget *algoConfig;     // Combo+spin
    ControlPanelWidget   *controlPanel;    // Start, Pause, Step, Reset, Speed
    MetricsPanelWidget   *metricsPanel;    // Labels para métricas
    GanttChartWidget     *ganttWidget;     // Zona de dibujo

    // Backend views
    SchedulingView        schedulingView;
    SyncView              syncView;

    QTimer               *simulationTimer;
};
#endif // MAIN_WINDOW_GUI_H
