#ifndef ALGORITHM_CONFIG_WIDGET_H
#define ALGORITHM_CONFIG_WIDGET_H

#include <QWidget>
#include <QString>

class QComboBox;
class QSpinBox;
class QLabel;

class AlgorithmConfigWidget : public QWidget {
    Q_OBJECT

public:
    explicit AlgorithmConfigWidget(QWidget *parent = nullptr);
    void setMode(bool schedulingMode);

signals:
    void algorithmChanged(int index);
    void quantumChanged(int quantum);

private slots:
    void onAlgorithmSelected(int index);
    void onQuantumValueChanged(int value);

private:
    bool schedulingMode;
    QComboBox *algorithmCombo;
    QSpinBox *quantumSpin;
};

#endif