#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "trem.h"

#include <QMainWindow>

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

    //Cria os objetos TREM's
    std::unique_ptr<Trem> trem1;
    std::unique_ptr<Trem> trem2;
};

#endif // MAINWINDOW_H
