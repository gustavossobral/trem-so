#include "malha.h"

namespace {

/*
 * Linhas e colunas percorridas pelo canto superior esquerdo dos trens.
 * Os trilhos desenhados em mainwindow.ui seguem estas mesmas coordenadas.
 *
 *        X0    X1    X2    X3    X4    X5    X6
 *   Y0         +-----------------------+
 *              |           a           |
 *   Y1         +-----------+-----------+
 *              |     b     |     c     |
 *   Y2   +-----+-----+-----+-----+-----+-----+
 *        |     d     |     e     |     f     |
 *   Y3   +-----------+-----------+-----------+
 */
constexpr int X0 = 30, X1 = 130, X2 = 230, X3 = 330, X4 = 430, X5 = 530, X6 = 630;
constexpr int Y0 = 30, Y1 = 150, Y2 = 270, Y3 = 390;

} // namespace

// Cada trem começa num trecho do seu trilho que não é compartilhado com outro trem
Malha::Malha()
{
    criarTrem({X1, Y0}, {X5, Y1}, {X3, Y0});              // a
    criarTrem({X1, Y1}, {X3, Y2}, {X1, (Y1 + Y2) / 2});   // b
    criarTrem({X3, Y1}, {X5, Y2}, {X5, (Y1 + Y2) / 2});   // c
    criarTrem({X0, Y2}, {X2, Y3}, {X0, (Y2 + Y3) / 2});   // d
    criarTrem({X2, Y2}, {X4, Y3}, {X3, Y3});              // e
    criarTrem({X4, Y2}, {X6, Y3}, {X6, (Y2 + Y3) / 2});   // f
}

// Pede para todas as threads terminarem e só então espera cada uma sair do laço
Malha::~Malha()
{
    for (auto &trem : trens)
        trem->requestInterruption();
    for (auto &trem : trens)
        trem->wait();
}

Trem &Malha::trem(int id)
{
    return *trens.at(id);
}

void Malha::criarTrem(QPoint cantoSuperiorEsquerdo, QPoint cantoInferiorDireito, QPoint inicio)
{
    const int id = static_cast<int>(trens.size());
    const QRect trilho(cantoSuperiorEsquerdo, cantoInferiorDireito);
    trens.push_back(std::make_unique<Trem>(id, trilho, inicio));
}
