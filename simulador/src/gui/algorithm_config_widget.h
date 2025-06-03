#ifndef ALGORITHM_CONFIG_WIDGET_H
#define ALGORITHM_CONFIG_WIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class AlgorithmConfigWidget : public QWidget {
    Q_OBJECT

public:
    explicit AlgorithmConfigWidget(QWidget *parent = nullptr);
    void setMode(bool isScheduling);
    int getQuantum() const;

signals:
    void algorithmChanged(int index);
    void quantumChanged(int value);
    void syncMechanismChanged(int index);

private slots:
    void onAlgorithmChanged(int index);
    void onQuantumChanged(int value);
    void onSyncMechanismChanged(int index);

private:
    bool schedulingMode;  // true = modo calendarización, false = modo sincronización
    
    QVBoxLayout *mainLayout;
    QComboBox *algorithmCombo;      // Para algoritmos de calendarización
    QComboBox *syncMechanismCombo;  // Para mecanismos de sincronización
    QSpinBox *quantumSpin;          // Para configurar quantum
    QLabel *quantumLabel;           // Etiqueta para quantum
};

#endif