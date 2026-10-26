#include "malha.h"

namespace {

/*
 * Linhas e colunas percorridas pelo canto superior esquerdo dos trens.
 * Os trilhos desenhados em mainwindow.ui seguem estas mesmas coordenadas.
 *
 *        X0    X1    X2    X3    X4    X5    X6
 *   Y0         +-----------------------+
 *              |           a           |
 *   Y1         +-----1-----+-----2-----+
 *              |     b     3     c     |
 *   Y2   +-----+--4--+--5--+--6--+--7--+-----+
 *        |     d     8     e     9     f     |
 *   Y3   +-----------+-----------+-----------+
 */
constexpr int X0 = 30, X1 = 130, X2 = 230, X3 = 330, X4 = 430, X5 = 530, X6 = 630;
constexpr int Y0 = 30, Y1 = 150, Y2 = 270, Y3 = 390;

// Pixels cobertos por um trem ao percorrer o trecho reto de 'inicio' até 'fim'
QRect faixa(QPoint inicio, QPoint fim)
{
    return QRect(inicio, fim + QPoint(Trem::TAMANHO - 1, Trem::TAMANHO - 1));
}

} // namespace

Malha::Malha()
    : regioes{{
          {faixa({X1, Y1}, {X3, Y1})},  // 1: trens a e b
          {faixa({X3, Y1}, {X5, Y1})},  // 2: trens a e c
          {faixa({X3, Y1}, {X3, Y2})},  // 3: trens b e c
          {faixa({X1, Y2}, {X2, Y2})},  // 4: trens b e d
          {faixa({X2, Y2}, {X3, Y2})},  // 5: trens b e e
          {faixa({X3, Y2}, {X4, Y2})},  // 6: trens c e e
          {faixa({X4, Y2}, {X5, Y2})},  // 7: trens c e f
          {faixa({X2, Y2}, {X2, Y3})},  // 8: trens d e e
          {faixa({X4, Y2}, {X4, Y3})},  // 9: trens e e f
      }}
{
    // Cada trem começa num trecho que não é compartilhado com outro trem. A rota
    // lista as regiões críticas na ordem em que o trem passa por elas a partir dali.
    criarTrem({X1, Y0}, {X5, Y1}, {X3, Y0},
              {regiao(1), regiao(2)});                         // a
    criarTrem({X1, Y1}, {X3, Y2}, {X1, (Y1 + Y2) / 2},
              {regiao(4), regiao(5), regiao(3), regiao(1)});   // b
    criarTrem({X3, Y1}, {X5, Y2}, {X5, (Y1 + Y2) / 2},
              {regiao(2), regiao(3), regiao(6), regiao(7)});   // c
    criarTrem({X0, Y2}, {X2, Y3}, {X0, (Y2 + Y3) / 2},
              {regiao(8), regiao(4)});                         // d
    criarTrem({X2, Y2}, {X4, Y3}, {X3, Y3},
              {regiao(9), regiao(6), regiao(5), regiao(8)});   // e
    criarTrem({X4, Y2}, {X6, Y3}, {X6, (Y2 + Y3) / 2},
              {regiao(7), regiao(9)});                         // f
}

// Pede para todas as threads terminarem e só então espera cada uma sair do laço
Malha::~Malha()
{
    for (auto &trem : trens)
        trem->encerrar();
    for (auto &trem : trens)
        trem->wait();
}

Trem &Malha::trem(int id)
{
    return *trens.at(id);
}

Regiao *Malha::regiao(int numero)
{
    return &regioes.at(numero - 1);
}

void Malha::criarTrem(QPoint cantoSuperiorEsquerdo, QPoint cantoInferiorDireito, QPoint inicio,
                      std::vector<Regiao *> rota)
{
    const int id = static_cast<int>(trens.size());
    const QRect trilho(cantoSuperiorEsquerdo, cantoInferiorDireito);
    trens.push_back(std::make_unique<Trem>(id, trilho, inicio, std::move(rota)));
}
