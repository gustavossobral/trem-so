#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);
    labelsTrens = {ui->tremA, ui->tremB, ui->tremC, ui->tremD, ui->tremE, ui->tremF};

    for (int id = 0; id < Malha::QUANTIDADE_TRENS; ++id) {
        Trem &trem = malha.trem(id);

        // Sempre que o trem emitir UPDATEGUI, UPDATEINTERFACE move o seu quadrado na tela
        connect(&trem, &Trem::updateGUI, this, &MainWindow::updateInterface);

        // Os trens começam a andar assim que a janela é aberta
        trem.start();
    }
}

MainWindow::~MainWindow() = default;

// Função executada quando o sinal UPDATEGUI é emitido
void MainWindow::updateInterface(int id, int x, int y)
{
    labelsTrens.at(id)->move(x, y);
}
