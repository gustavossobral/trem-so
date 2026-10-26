#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "malha.h"

#include <QLabel>
#include <QMainWindow>
#include <QSlider>

#include <array>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void updateInterface(int id, int x, int y);

private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::array<QLabel *, Malha::QUANTIDADE_TRENS> labelsTrens;
    std::array<QSlider *, Malha::QUANTIDADE_TRENS> slidersVelocidade;

    // Declarada por último para ser destruída primeiro: os trens param antes da interface
    Malha malha;
};

#endif // MAINWINDOW_H
