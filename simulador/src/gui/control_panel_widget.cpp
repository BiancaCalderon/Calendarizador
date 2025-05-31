#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include "control_panel_widget.h"

ControlPanelWidget::ControlPanelWidget(QWidget *parent)
    : QWidget(parent) {
    startButton = new QPushButton("Start", this);
    pauseButton = new QPushButton("Pause", this);
    stepButton  = new QPushButton("Step", this);
    resetButton = new QPushButton("Reset", this);

    speedLabel = new QLabel("Speed:");
    speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(1, 10);
    speedSlider->setValue(5);

    auto *layout = new QHBoxLayout(this);
    layout->addWidget(startButton);
    layout->addWidget(pauseButton);
    layout->addWidget(stepButton);
    layout->addWidget(resetButton);
    layout->addSpacing(20);
    layout->addWidget(speedLabel);
    layout->addWidget(speedSlider);
    setLayout(layout);

    connect(startButton, &QPushButton::clicked, this, &ControlPanelWidget::onStart);
    connect(pauseButton, &QPushButton::clicked, this, &ControlPanelWidget::onPause);
    connect(stepButton,  &QPushButton::clicked, this, &ControlPanelWidget::onStep);
    connect(resetButton, &QPushButton::clicked, this, &ControlPanelWidget::onReset);
    connect(speedSlider, &QSlider::valueChanged, this, &ControlPanelWidget::onSpeedSliderChanged);
}

void ControlPanelWidget::setRunning(bool running) {
    startButton->setEnabled(!running);
    pauseButton->setEnabled(running);
    stepButton->setEnabled(!running);
    resetButton->setEnabled(true);
}

int ControlPanelWidget::getSpeed() const {
    return speedSlider->value();
}

void ControlPanelWidget::onSpeedSliderChanged(int value) {
    emit speedChanged(value);
}

void ControlPanelWidget::onStart() {
    emit startClicked();
}

void ControlPanelWidget::onPause() {
    emit pauseClicked();
}

void ControlPanelWidget::onStep() {
    emit stepClicked();
}

void ControlPanelWidget::onReset() {
    emit resetClicked();
}