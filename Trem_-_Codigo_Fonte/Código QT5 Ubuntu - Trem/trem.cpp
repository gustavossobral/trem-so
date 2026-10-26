#include "trem.h"

namespace {

constexpr int PASSO = 1;                        // pixels percorridos a cada passo
constexpr unsigned long ESPERA_PARADO_MS = 20;  // intervalo entre verificações com o trem parado

// Intervalo entre passos = FATOR_INTERVALO_US / velocidade, ou seja, o trem anda
// 20 * velocidade pixels por segundo: 20 px/s na velocidade 1 e 4000 px/s na máxima
constexpr unsigned long FATOR_INTERVALO_US = 50'000;

// Pixels cobertos pelo trem quando o seu canto superior esquerdo está em 'posicao'
QRect areaDoTrem(QPoint posicao)
{
    return QRect(posicao, QSize(Trem::TAMANHO, Trem::TAMANHO));
}

} // namespace

Trem::Trem(int id, QRect trilho, QPoint inicio, std::vector<Regiao *> rota, QObject *parent)
    : QThread(parent), id(id), trilho(trilho), posicao(inicio), rota(std::move(rota))
{
}

void Trem::setVelocidade(int velocidade)
{
    this->velocidade = velocidade;
}

void Trem::encerrar()
{
    // Na velocidade máxima o trem sai logo das regiões que ocupa, liberando os outros
    velocidade = VELOCIDADE_MAXIMA;
    requestInterruption();
}

// Função executada pela thread após trem->start()
void Trem::run()
{
    // Só sai do laço fora das regiões críticas, para nenhum outro trem ficar
    // esperando para sempre por um mutex que esta thread deixou travado
    while (!isInterruptionRequested() || !regioesOcupadas.empty()) {
        const int velocidadeAtual = velocidade;
        if (velocidadeAtual == 0) {
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

// Trava a próxima região da rota no passo em que o trem vai encostar nela.
// Se outro trem estiver na região, a thread fica bloqueada aqui até ele sair.
void Trem::entrarNaProximaRegiao(QPoint destino)
{
    // A posição atual também é testada porque, numa rota com duas regiões, a
    // próxima da rota é justamente aquela de onde o trem ainda está saindo
    Regiao *regiao = rota[proximaRegiao];
    const bool vaiEncostar = !areaDoTrem(posicao).intersects(regiao->area)
                             && areaDoTrem(destino).intersects(regiao->area);
    if (!vaiEncostar)
        return;

    regiao->mutex.lock();
    regioesOcupadas.push_back(regiao);
    proximaRegiao = (proximaRegiao + 1) % rota.size();
}

// Destrava as regiões que o trem já deixou por completo
void Trem::sairDasRegioesDeixadas()
{
    while (!regioesOcupadas.empty()
           && !areaDoTrem(posicao).intersects(regioesOcupadas.front()->area)) {
        regioesOcupadas.front()->mutex.unlock();
        regioesOcupadas.pop_front();
    }
}
