#include "trem.h"

namespace {

constexpr int PASSO = 1;  // pixels percorridos a cada passo

} // namespace

Trem::Trem(int id, QRect trilho, QPoint inicio, QObject *parent)
    : QThread(parent), id(id), trilho(trilho), posicao(inicio)
{
}

// Função executada pela thread após trem->start()
void Trem::run()
{
    while (!isInterruptionRequested()) {
        posicao = proximaPosicao();
        emit updateGUI(id, posicao.x(), posicao.y());
        msleep(velocidade);
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
