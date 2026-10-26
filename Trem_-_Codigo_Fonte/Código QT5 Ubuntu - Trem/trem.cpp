#include "trem.h"

namespace {

constexpr int PASSO = 1;                        // pixels percorridos a cada passo
constexpr unsigned long ESPERA_PARADO_MS = 20;  // intervalo entre verificações com o trem parado

// Intervalo entre passos = FATOR_INTERVALO_US / velocidade, ou seja, o trem anda
// 20 * velocidade pixels por segundo: 20 px/s na velocidade 1 e 4000 px/s na máxima
constexpr unsigned long FATOR_INTERVALO_US = 50'000;

} // namespace

Trem::Trem(int id, QRect trilho, QPoint inicio, QObject *parent)
    : QThread(parent), id(id), trilho(trilho), posicao(inicio)
{
}

void Trem::setVelocidade(int velocidade)
{
    this->velocidade = velocidade;
}

// Função executada pela thread após trem->start()
void Trem::run()
{
    while (!isInterruptionRequested()) {
        const int velocidadeAtual = velocidade;
        if (velocidadeAtual == 0) {
            msleep(ESPERA_PARADO_MS);
            continue;
        }

        posicao = proximaPosicao();
        emit updateGUI(id, posicao.x(), posicao.y());
        usleep(FATOR_INTERVALO_US / velocidadeAtual);
    }
}

// Na tela o eixo y cresce para baixo, então o sentido anti-horário é:
// esquerda em cima, desce pela esquerda, direita embaixo e sobe pela direita.
QPoint Trem::proximaPosicao() const
{
    int x = posicao.x();
    int y = posicao.y();

    if (y == trilho.top() && x > trilho.left())
        x -= PASSO;
    else if (x == trilho.left() && y < trilho.bottom())
        y += PASSO;
    else if (y == trilho.bottom() && x < trilho.right())
        x += PASSO;
    else
        y -= PASSO;

    return {x, y};
}
