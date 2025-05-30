#ifndef CONTROL_PANEL_WIDGET_H
#define CONTROL_PANEL_WIDGET_H

#include <QWidget>

class QPushButton;
class QSlider;
class QLabel;

class ControlPanelWidget : public QWidget {
    Q_OBJECT

public:
    explicit ControlPanelWidget(QWidget *parent = nullptr);
    void setRunning(bool running);

signals:
    void startClicked();
    void pauseClicked();
    void stepClicked();
    void resetClicked();
    void speedChanged(int ms);

private slots:
    void onStart();
    void onPause();
    void onStep();
    void onReset();
    void onSpeedSliderChanged(int value);

private:
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *stepButton;
    QPushButton *resetButton;
    QSlider    *speedSlider;
};

#endif