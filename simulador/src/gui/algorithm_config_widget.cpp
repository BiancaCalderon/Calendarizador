#include "algorithm_config_widget.h"
#include <QComboBox>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QLabel>

AlgorithmConfigWidget::AlgorithmConfigWidget(QWidget *parent)
    : QWidget(parent), schedulingMode(true) {
    algorithmCombo = new QComboBox(this);
    algorithmCombo->addItems({"FIFO", "SJF", "SRT", "Round Robin", "Priority"});
    connect(algorithmCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AlgorithmConfigWidget::onAlgorithmSelected);

    quantumSpin = new QSpinBox(this);
    quantumSpin->setRange(1, 100);
    quantumSpin->setValue(2);
    connect(quantumSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &AlgorithmConfigWidget::onQuantumValueChanged);

    auto *layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("Algoritmo:", this));
    layout->addWidget(algorithmCombo);
    layout->addSpacing(20);
    layout->addWidget(new QLabel("Quantum:", this));
    layout->addWidget(quantumSpin);
    setLayout(layout);

    setMode(true);
}

void AlgorithmConfigWidget::setMode(bool scheduling) {
    schedulingMode = scheduling;
    algorithmCombo->setVisible(schedulingMode);
    quantumSpin->setVisible(schedulingMode && algorithmCombo->currentIndex() == 3);
    // El índice 3 corresponde a "Round Robin"
}

void AlgorithmConfigWidget::onAlgorithmSelected(int index) {
    emit algorithmChanged(index);
    // Mostrar spin sólo si es Round Robin
    if (schedulingMode) {
        quantumSpin->setVisible(index == 3);
    }
}

void AlgorithmConfigWidget::onQuantumValueChanged(int value) {
    emit quantumChanged(value);
}
