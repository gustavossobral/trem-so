#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);
    labelsTrens = {ui->tremA, ui->tremB, ui->tremC, ui->tremD, ui->tremE, ui->tremF};
    slidersVelocidade = {ui->velocidadeA, ui->velocidadeB, ui->velocidadeC,
                         ui->velocidadeD, ui->velocidadeE, ui->velocidadeF};

    for (int id = 0; id < Malha::QUANTIDADE_TRENS; ++id) {
        Trem &trem = malha.trem(id);
        QSlider *slider = slidersVelocidade.at(id);

        // Sempre que o trem emitir UPDATEGUI, UPDATEINTERFACE move o seu quadrado na tela
        connect(&trem, &Trem::updateGUI, this, &MainWindow::updateInterface);

        // Cada slider controla a velocidade do seu trem
        slider->setMaximum(Trem::VELOCIDADE_MAXIMA);
        connect(slider, &QSlider::valueChanged, &trem, &Trem::setVelocidade);
        trem.setVelocidade(slider->value());

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
