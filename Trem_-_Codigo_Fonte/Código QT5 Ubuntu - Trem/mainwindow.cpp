#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);

    //Cria o trem com seu (ID, posição X, posição Y)
    trem1 = std::make_unique<Trem>(1, 60, 30);
    trem2 = std::make_unique<Trem>(2, 330, 30);

    /*
     * Conecta o sinal UPDATEGUI à função UPDATEINTERFACE.
     * Ou seja, sempre que o sinal UPDATEGUI foi chamado, será executada a função UPDATEINTERFACE.
     * Os 3 parâmetros INT do sinal serão utilizados na função.
     * Trem1 e Trem2 são os objetos que podem chamar o sinal. Se um outro objeto chamar o
     * sinal UPDATEGUI, não haverá execução da função UPDATEINTERFACE
     */
    connect(trem1.get(), &Trem::updateGUI, this, &MainWindow::updateInterface);
    connect(trem2.get(), &Trem::updateGUI, this, &MainWindow::updateInterface);

    //Os trens começam a andar assim que a janela é aberta
    trem1->start();
    trem2->start();
}

//Ao fechar a janela, pede para as threads terminarem e espera cada uma sair do laço
MainWindow::~MainWindow()
{
    trem1->requestInterruption();
    trem2->requestInterruption();
    trem1->wait();
    trem2->wait();
}

//Função que será executada quando o sinal UPDATEGUI for emitido
void MainWindow::updateInterface(int id, int x, int y)
{
    switch (id) {
    case 1: //Atualiza a posição do objeto da tela (quadrado) que representa o trem1
        ui->label_trem1->setGeometry(x, y, 21, 17);
        break;
    case 2: //Atualiza a posição do objeto da tela (quadrado) que representa o trem2
        ui->label_trem2->setGeometry(x, y, 21, 17);
        break;
    default:
        break;
    }
}
