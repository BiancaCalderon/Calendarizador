#include "file_loader_widget.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QFileDialog>

FileLoaderWidget::FileLoaderWidget(QWidget *parent)
    : QWidget(parent), schedulingMode(true) {
    // Procesos
    processLineEdit = new QLineEdit(this);
    processButton = new QPushButton("...");
    connect(processButton, &QPushButton::clicked,
            this, &FileLoaderWidget::browseProcess);

    // Recursos
    resourceLineEdit = new QLineEdit(this);
    resourceButton = new QPushButton("...");
    connect(resourceButton, &QPushButton::clicked,
            this, &FileLoaderWidget::browseResource);

    // Acciones
    actionLineEdit = new QLineEdit(this);
    actionButton = new QPushButton("...");
    connect(actionButton, &QPushButton::clicked,
            this, &FileLoaderWidget::browseAction);

    // Botón Cargar
    loadButton = new QPushButton("Cargar");
    connect(loadButton, &QPushButton::clicked,
            this, &FileLoaderWidget::emitFilesLoaded);

    auto *layout = new QHBoxLayout(this);
    layout->addWidget(new QLabel("Procesos:", this));
    layout->addWidget(processLineEdit);
    layout->addWidget(processButton);
    layout->addSpacing(10);

    layout->addWidget(new QLabel("Recursos:", this));
    layout->addWidget(resourceLineEdit);
    layout->addWidget(resourceButton);
    layout->addSpacing(10);

    layout->addWidget(new QLabel("Acciones:", this));
    layout->addWidget(actionLineEdit);
    layout->addWidget(actionButton);
    layout->addSpacing(10);

    layout->addWidget(loadButton);
    setLayout(layout);

    // Modo inicial
    setMode(true);
}

void FileLoaderWidget::setMode(bool scheduling) {
    schedulingMode = scheduling;
    // Solo mostrar recursos y acciones en modo sincronización
    resourceLineEdit->setVisible(!schedulingMode);
    resourceButton->setVisible(!schedulingMode);
    actionLineEdit->setVisible(!schedulingMode);
    actionButton->setVisible(!schedulingMode);
}

void FileLoaderWidget::browseProcess() {
    QString file = QFileDialog::getOpenFileName(
        this, "Seleccionar archivo de procesos", EXAMPLES_DIR "/processes", "Archivos de texto (*.txt)");
    if (!file.isEmpty())
        processLineEdit->setText(file);
}

void FileLoaderWidget::browseResource() {
    QString file = QFileDialog::getOpenFileName(
        this, "Seleccionar archivo de recursos", EXAMPLES_DIR "/resources", "Archivos de texto (*.txt)");
    if (!file.isEmpty())
        resourceLineEdit->setText(file);
}

void FileLoaderWidget::browseAction() {
    QString file = QFileDialog::getOpenFileName(
        this, "Seleccionar archivo de acciones", EXAMPLES_DIR "/actions", "Archivos de texto (*.txt)");
    if (!file.isEmpty())
        actionLineEdit->setText(file);
}

void FileLoaderWidget::emitFilesLoaded() {
    emit filesLoaded(
        processLineEdit->text(),
        schedulingMode ? QString() : resourceLineEdit->text(),
        schedulingMode ? QString() : actionLineEdit->text());
}