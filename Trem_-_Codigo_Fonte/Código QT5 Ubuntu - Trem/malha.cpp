#include "malha.h"

namespace {

// colunas e linhas dos trilhos
constexpr int X0 = 30, X1 = 130, X2 = 230, X3 = 330, X4 = 430, X5 = 530, X6 = 630;
constexpr int Y0 = 30, Y1 = 150, Y2 = 270, Y3 = 390;

QRect faixa(QPoint inicio, QPoint fim)
{
    return QRect(inicio, fim + QPoint(Trem::TAMANHO - 1, Trem::TAMANHO - 1));
}

}

Malha::Malha()
    : regioes{{
          {faixa({X1, Y1}, {X3, Y1})},  // 1: a e b
          {faixa({X3, Y1}, {X5, Y1})},  // 2: a e c
          {faixa({X3, Y1}, {X3, Y2})},  // 3: b e c
          {faixa({X1, Y2}, {X2, Y2})},  // 4: b e d
          {faixa({X2, Y2}, {X3, Y2})},  // 5: b e e
          {faixa({X3, Y2}, {X4, Y2})},  // 6: c e e
          {faixa({X4, Y2}, {X5, Y2})},  // 7: c e f
          {faixa({X2, Y2}, {X2, Y3})},  // 8: d e e
          {faixa({X4, Y2}, {X4, Y3})},  // 9: e e f
      }}
{
    QSemaphore *superior = &cruzamentoSuperior;
    QSemaphore *central = &cruzamentoCentral;
    QSemaphore *esquerdo = &cruzamentoEsquerdo;
    QSemaphore *direito = &cruzamentoDireito;

    // trem a
    criarTrem({X1, Y0}, {X5, Y1}, {X3, Y0}, {
        {regiao(1), {superior}, {}},
        {regiao(2), {}, {superior}},
    });

    // trem b (central primeiro)
    criarTrem({X1, Y1}, {X3, Y2}, {X1, (Y1 + Y2) / 2}, {
        {regiao(4), {central, esquerdo}, {}},
        {regiao(5), {}, {esquerdo}},
        {regiao(3), {superior}, {central}},
        {regiao(1), {}, {superior}},
    });

    // trem c (central primeiro)
    criarTrem({X3, Y1}, {X5, Y2}, {X5, (Y1 + Y2) / 2}, {
        {regiao(2), {central, superior}, {}},
        {regiao(3), {}, {superior}},
        {regiao(6), {direito}, {central}},
        {regiao(7), {}, {direito}},
    });

    // trem d
    criarTrem({X0, Y2}, {X2, Y3}, {X0, (Y2 + Y3) / 2}, {
        {regiao(8), {esquerdo}, {}},
        {regiao(4), {}, {esquerdo}},
    });

    // trem e (central primeiro)
    criarTrem({X2, Y2}, {X4, Y3}, {X3, Y3}, {
        {regiao(9), {central, direito}, {}},
        {regiao(6), {}, {direito}},
        {regiao(5), {esquerdo}, {central}},
        {regiao(8), {}, {esquerdo}},
    });

    // trem f
    criarTrem({X4, Y2}, {X6, Y3}, {X6, (Y2 + Y3) / 2}, {
        {regiao(7), {direito}, {}},
        {regiao(9), {}, {direito}},
    });
}

Malha::~Malha()
{
    for (auto &trem : trens)
        trem->encerrar();  // avisa as threads
    for (auto &trem : trens)
        trem->wait();  // espera cada uma terminar
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
                      std::vector<Trecho> rota)
{
    const int id = static_cast<int>(trens.size());
    const QRect trilho(cantoSuperiorEsquerdo, cantoInferiorDireito);
    trens.push_back(std::make_unique<Trem>(id, trilho, inicio, std::move(rota)));
}
