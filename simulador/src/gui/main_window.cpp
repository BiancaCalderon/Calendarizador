#include "main_window.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    connectSignals();
    setWindowTitle("Simulador SO: Calendarización y Sincronización");
}

void MainWindow::setupUi() {
    centralWidget = new QWidget(this);
    mainLayout    = new QVBoxLayout(centralWidget);

    modeCombo = new QComboBox();
    modeCombo->addItem("Calendarización");
    modeCombo->addItem("Sincronización");

    loaderWidget = new FileLoaderWidget();
    algoConfig   = new AlgorithmConfigWidget();
    controlPanel = new ControlPanelWidget();
    metricsPanel = new MetricsPanelWidget();
    ganttWidget  = new GanttChartWidget();

    mainLayout->addWidget(modeCombo);
    mainLayout->addWidget(loaderWidget);
    mainLayout->addWidget(algoConfig);
    mainLayout->addWidget(controlPanel);
    mainLayout->addWidget(metricsPanel);
    mainLayout->addWidget(ganttWidget);

    setCentralWidget(centralWidget);
}

void MainWindow::connectSignals() {
    connect(modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onModeChanged);

    connect(loaderWidget, &FileLoaderWidget::filesLoaded,
            this, &MainWindow::onFilesLoaded);

    connect(algoConfig, &AlgorithmConfigWidget::algorithmChanged,
            this, &MainWindow::onAlgorithmChanged);
    connect(algoConfig, &AlgorithmConfigWidget::quantumChanged,
            this, &MainWindow::onQuantumChanged);

    connect(controlPanel, &ControlPanelWidget::startClicked,
            this, &MainWindow::onStartClicked);
    connect(controlPanel, &ControlPanelWidget::pauseClicked,
            this, &MainWindow::onPauseClicked);
    connect(controlPanel, &ControlPanelWidget::stepClicked,
            this, &MainWindow::onStepClicked);
    connect(controlPanel, &ControlPanelWidget::resetClicked,
            this, &MainWindow::onResetClicked);
}

void MainWindow::onModeChanged(int index) {
    // 0 = scheduling, 1 = sync
    algoConfig->setMode(index == 0);
    loaderWidget->setMode(index == 0);
    schedulingView.reset(); 
    syncView.reset();
}

void MainWindow::onFilesLoaded(const QString &proc,
                               const QString &res,
                               const QString &act)
{
    bool ok = false;
    if (modeCombo->currentIndex() == 0) {
        ok = schedulingView.loadProcessFile(proc.toStdString());
    } else {
        ok = syncView.loadFiles(proc.toStdString(),
                                res.toStdString(),
                                act.toStdString());
    }
    if (!ok) {
        QMessageBox::warning(this, "Error", "No se pudieron cargar los archivos.");
    }
}

void MainWindow::onAlgorithmChanged(int idx) {
    schedulingView.setSchedulerAlgorithm(static_cast<SchedulerType>(idx));
}

void MainWindow::onQuantumChanged(int q) {
    schedulingView.setQuantum(q);
}

void MainWindow::onStartClicked() {
    controlPanel->setRunning(true);
    // aquí podrías arrancar un QTimer que llame a onStepClicked periódicamente
}

void MainWindow::onPauseClicked() {
    controlPanel->setRunning(false);
}

void MainWindow::onStepClicked() {
    if (modeCombo->currentIndex() == 0) {
        schedulingView.step();
        ganttWidget->updateTimeline(
            schedulingView.getTimeline(),
            schedulingView.getCurrentTime());
        metricsPanel->updateMetrics(
            schedulingView.getAverageWaitingTime(),
            schedulingView.getAverageTurnaroundTime());
    } else {
        syncView.step();
        ganttWidget->updateTimeline(
            syncView.getTimeline(),
            syncView.getCurrentTime());
    }
}

void MainWindow::onResetClicked() {
    schedulingView.reset();
    syncView.reset();
    metricsPanel->clear();
    ganttWidget->reset();
}
