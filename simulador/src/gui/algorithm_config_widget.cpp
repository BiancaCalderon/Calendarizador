#include "algorithm_config_widget.h"
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>

AlgorithmConfigWidget::AlgorithmConfigWidget(QWidget *parent)
    : QWidget(parent), schedulingMode(true) {
    
    mainLayout = new QVBoxLayout(this);
    
    // Combo para algoritmos de calendarización
    algorithmCombo = new QComboBox();
    algorithmCombo->addItem("FIFO");
    algorithmCombo->addItem("SJF");
    algorithmCombo->addItem("SRT");
    algorithmCombo->addItem("Round Robin");
    algorithmCombo->addItem("Priority");
    
    // Combo para mecanismos de sincronización
    syncMechanismCombo = new QComboBox();
    syncMechanismCombo->addItem("Mutex");
    syncMechanismCombo->addItem("Semáforo");
    syncMechanismCombo->hide(); // Inicialmente oculto
    
    // Spin box para quantum (solo visible en Round Robin)
    quantumSpin = new QSpinBox();
    quantumSpin->setRange(1, 100);
    quantumSpin->setValue(2);
    quantumSpin->hide(); // Inicialmente oculto
    
    quantumLabel = new QLabel("Quantum:");
    quantumLabel->hide(); // Inicialmente oculto
    
    // Layout para quantum
    QHBoxLayout* quantumLayout = new QHBoxLayout();
    quantumLayout->addWidget(quantumLabel);
    quantumLayout->addWidget(quantumSpin);
    quantumLayout->addStretch();
    
    // Agregar widgets al layout principal
    mainLayout->addWidget(algorithmCombo);
    mainLayout->addWidget(syncMechanismCombo);
    mainLayout->addLayout(quantumLayout);
    
    // Conectar señales
    connect(algorithmCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AlgorithmConfigWidget::onAlgorithmChanged);
    connect(quantumSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &AlgorithmConfigWidget::onQuantumChanged);
    connect(syncMechanismCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AlgorithmConfigWidget::onSyncMechanismChanged);
}

void AlgorithmConfigWidget::setMode(bool isScheduling) {
    schedulingMode = isScheduling;
    
    if (schedulingMode) {
        // Modo calendarización
        algorithmCombo->show();
        syncMechanismCombo->hide();
        
        // Mostrar quantum solo si es Round Robin
        bool isRoundRobin = algorithmCombo->currentIndex() == 3;
        quantumSpin->setVisible(isRoundRobin);
        quantumLabel->setVisible(isRoundRobin);
    } else {
        // Modo sincronización
        algorithmCombo->hide();
        syncMechanismCombo->show();
        quantumSpin->hide();
        quantumLabel->hide();
    }
}

int AlgorithmConfigWidget::getQuantum() const {
    return quantumSpin->value();
}

void AlgorithmConfigWidget::onAlgorithmChanged(int index) {
    // Mostrar/ocultar quantum solo en modo calendarización y solo para Round Robin
    if (schedulingMode) {
        bool isRoundRobin = index == 3;
        quantumSpin->setVisible(isRoundRobin);
        quantumLabel->setVisible(isRoundRobin);
    }
    
    emit algorithmChanged(index);
}

void AlgorithmConfigWidget::onQuantumChanged(int value) {
    emit quantumChanged(value);
}

void AlgorithmConfigWidget::onSyncMechanismChanged(int index) {
    emit syncMechanismChanged(index);
}
