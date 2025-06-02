#include "main_window.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    connectSignals();
    setWindowTitle("Simulador SO: Calendarización y Sincronización");

    // Inicializar el timer
    simulationTimer = new QTimer(this);
    connect(simulationTimer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
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
    
    // Conectar la señal del deslizador de velocidad
    connect(controlPanel, &ControlPanelWidget::speedChanged,
            this, &MainWindow::onSpeedChanged);
}

void MainWindow::onModeChanged(int index) {
    // 0 = scheduling, 1 = sync
    algoConfig->setMode(index == 0);
    loaderWidget->setMode(index == 0);
    schedulingView.reset(); 
    syncView.reset();
    simulationTimer->stop(); // Detener simulación automática al cambiar de modo
}

void MainWindow::onFilesLoaded(const QString &proc,
                               const QString &res,
                               const QString &act)
{
    bool ok = false;
    if (modeCombo->currentIndex() == 0) {
        ok = schedulingView.loadProcessFile(proc.toStdString());
        if (ok) {
            QMessageBox::information(this, "Carga Exitosa", 
                QString("✓ Archivo de procesos cargado exitosamente\n"
                       "✓ %1 procesos cargados")
                .arg(schedulingView.getProcessesSize()));
        }
    } else {
        ok = syncView.loadFiles(proc.toStdString(),
                                res.toStdString(),
                                act.toStdString());
        if (ok) {
            QMessageBox::information(this, "Carga Exitosa", 
                QString("✓ Archivos cargados exitosamente\n\n"
                       "✓ %1 procesos cargados\n"
                       "✓ %2 recursos disponibles\n"
                       "✓ %3 acciones programadas")
                .arg(syncView.getProcessesSize())
                .arg(syncView.getResourcesSize())
                .arg(syncView.getActionsSize()));
        }
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
    if (!simulationTimer->isActive()) {
        controlPanel->setRunning(true);
        stepCounter = 0; // Reiniciar contador de pasos
        int speed = controlPanel->getSpeed();
        int interval = 1000 / speed;
        simulationTimer->start(interval);
    }
}

void MainWindow::onPauseClicked() {
    if (simulationTimer->isActive()) {
        controlPanel->setRunning(false);
        simulationTimer->stop();
    }
}

void MainWindow::onStepClicked() {
    // Pausar la simulación automática si estaba corriendo
    if (simulationTimer->isActive()) {
        simulationTimer->stop();
        controlPanel->setRunning(false);
    }

    executeSimulationStep();
}

void MainWindow::onResetClicked() {
    schedulingView.reset();
    syncView.reset();
    metricsPanel->clear();
    ganttWidget->reset();
    simulationTimer->stop();
    controlPanel->setRunning(false);
    stepCounter = 0; // Reiniciar contador de pasos
}

void MainWindow::onTimerTimeout() {
    executeSimulationStep();
}

void MainWindow::onSpeedChanged(int speed) {
    if (simulationTimer->isActive()) {
        simulationTimer->stop();
        int interval = 1000 / speed;
        simulationTimer->start(interval);
    }
}

void MainWindow::executeSimulationStep() {
    if (++stepCounter > maxSteps) {
        simulationTimer->stop();
        controlPanel->setRunning(false);
        QMessageBox::critical(this, "Error", "La simulación excedió el número máximo de pasos (posible ciclo infinito).");
        return;
    }
    if (modeCombo->currentIndex() == 0) {
        schedulingView.step();
        ganttWidget->updateTimeline(
            schedulingView.getTimeline(),
            schedulingView.getCurrentTime());
        metricsPanel->updateMetrics(
            schedulingView.getAverageWaitingTime(),
            schedulingView.getAverageTurnaroundTime());

        if (schedulingView.isSimulationCompleted()) {
            QMessageBox::information(this, "Simulación Terminada", "La simulación de calendarización ha completado.");
            simulationTimer->stop();
            controlPanel->setRunning(false);
        }
    } else {
        syncView.step();
        ganttWidget->updateTimeline(
            syncView.getTimeline(),
            syncView.getCurrentTime());

        if (syncView.isSimulationCompleted()) {
            QMessageBox::information(this, "Simulación Terminada", "La simulación de sincronización ha completado.");
            simulationTimer->stop();
            controlPanel->setRunning(false);
        }
    }
}
