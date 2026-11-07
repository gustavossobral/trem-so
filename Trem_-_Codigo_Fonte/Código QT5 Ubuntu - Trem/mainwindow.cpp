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
    ui->velocidadeMestre->setMaximum(Trem::VELOCIDADE_MAXIMA);

    for (int id = 0; id < Malha::QUANTIDADE_TRENS; ++id) {
        Trem &trem = malha.trem(id);
        QSlider *slider = slidersVelocidade.at(id);

        // thread do trem -> interface
        connect(&trem, &Trem::updateGUI, this, &MainWindow::updateInterface);

        // slider -> velocidade do trem
        slider->setMaximum(Trem::VELOCIDADE_MAXIMA);
        connect(slider, &QSlider::valueChanged, &trem, &Trem::setVelocidade);
        trem.setVelocidade(slider->value());

        // controle mestre
        connect(ui->velocidadeMestre, &QSlider::valueChanged, slider, &QSlider::setValue);

        trem.start();  // inicia a thread
    }
}

MainWindow::~MainWindow() = default;

void MainWindow::updateInterface(int id, int x, int y)
{
    labelsTrens.at(id)->move(x, y);
}
