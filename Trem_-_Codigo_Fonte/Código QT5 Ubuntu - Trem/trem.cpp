#include "trem.h"

namespace {

constexpr int PASSO = 1;                        // px por passo
constexpr unsigned long ESPERA_PARADO_MS = 20;  // espera com o trem parado

constexpr unsigned long FATOR_INTERVALO_US = 50'000;  // intervalo = fator / velocidade

QRect areaDoTrem(QPoint posicao)
{
    return QRect(posicao, QSize(Trem::TAMANHO, Trem::TAMANHO));
}

}

Trem::Trem(int id, QRect trilho, QPoint inicio, std::vector<Trecho> rota, QObject *parent)
    : QThread(parent), id(id), trilho(trilho), posicao(inicio), rota(std::move(rota))
{
}

void Trem::setVelocidade(int velocidade)
{
    this->velocidade = velocidade;
}

void Trem::encerrar()
{
    velocidade = VELOCIDADE_MAXIMA;  // sai logo das regiões ocupadas
    requestInterruption();
}

void Trem::run()
{
    // só termina fora das regiões críticas
    while (!isInterruptionRequested() || !regioesOcupadas.empty()) {
        const int velocidadeAtual = velocidade;
        if (velocidadeAtual == 0) {  // trem parado
            msleep(ESPERA_PARADO_MS);
            continue;
        }

        andar();
        usleep(FATOR_INTERVALO_US / velocidadeAtual);
    }
}

void Trem::andar()
{
    const QPoint destino = proximaPosicao();
    entrarNaProximaRegiao(destino);
    posicao = destino;
    emit updateGUI(id, posicao.x(), posicao.y());
    sairDasRegioesDeixadas();
}

QPoint Trem::proximaPosicao() const
{
    int x = posicao.x();
    int y = posicao.y();

    // sentido anti-horário
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

void Trem::entrarNaProximaRegiao(QPoint destino)
{
    const Trecho &trecho = rota[proximoTrecho];
    const bool vaiEncostar = !areaDoTrem(posicao).intersects(trecho.regiao->area)
                             && areaDoTrem(destino).intersects(trecho.regiao->area);
    if (!vaiEncostar)
        return;

    // entrada na região crítica
    for (QSemaphore *semaforo : trecho.adquirir)
        semaforo->acquire();
    trecho.regiao->mutex.lock();  // espera se outro trem estiver na região
    for (QSemaphore *semaforo : trecho.liberar)  // libera vaga no cruzamento
        semaforo->release();

    regioesOcupadas.push_back(trecho.regiao);
    proximoTrecho = (proximoTrecho + 1) % rota.size();
}

void Trem::sairDasRegioesDeixadas()
{
    while (!regioesOcupadas.empty()
           && !areaDoTrem(posicao).intersects(regioesOcupadas.front()->area)) {
        regioesOcupadas.front()->mutex.unlock();  // saída da região crítica
        regioesOcupadas.pop_front();
    }
}
