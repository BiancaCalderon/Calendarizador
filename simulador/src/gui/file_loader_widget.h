// src/gui/file_loader_widget.h
#ifndef FILE_LOADER_WIDGET_H
#define FILE_LOADER_WIDGET_H

#include <QWidget>
#include <QString>

class QPushButton;
class QLineEdit;
class QLabel;

class FileLoaderWidget : public QWidget {
    Q_OBJECT

public:
    explicit FileLoaderWidget(QWidget *parent = nullptr);
    void setMode(bool schedulingMode);

signals:
    void filesLoaded(const QString &processFile,
                     const QString &resourceFile,
                     const QString &actionFile);

private slots:
    void browseProcess();
    void browseResource();
    void browseAction();
    void emitFilesLoaded();

private:
    bool schedulingMode;
    QLineEdit *processLineEdit;
    QPushButton *processButton;
    QLineEdit *resourceLineEdit;
    QPushButton *resourceButton;
    QLineEdit *actionLineEdit;
    QPushButton *actionButton;
    QPushButton *loadButton;
};

#endif