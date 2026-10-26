#ifndef MALHA_H
#define MALHA_H

#include "trem.h"

#include <QSemaphore>

#include <array>
#include <memory>
#include <vector>

/*
 * Malha ferroviária da Figura 1: seis trens (a até f), cada um no seu trilho, as
 * nove regiões críticas da Figura 2 e os semáforos dos quatro cruzamentos, onde
 * três regiões se encontram. A malha é dona dos trens e encerra as threads deles
 * ao ser destruída.
 */
class Malha
{
public:
    static constexpr int QUANTIDADE_TRENS = 6;

    Malha();
    ~Malha();

    Trem &trem(int id);

private:
    Regiao *regiao(int numero);  // numeração da Figura 2, de 1 a 9
    void criarTrem(QPoint cantoSuperiorEsquerdo, QPoint cantoInferiorDireito, QPoint inicio,
                   std::vector<Trecho> rota);

    // Três trens disputam cada cruzamento; só dois podem se aproximar dele ao mesmo tempo
    static constexpr int PERMISSOES_POR_CRUZAMENTO = 2;

    std::array<Regiao, 9> regioes;
    QSemaphore cruzamentoSuperior{PERMISSOES_POR_CRUZAMENTO};  // regiões 1, 2 e 3
    QSemaphore cruzamentoCentral{PERMISSOES_POR_CRUZAMENTO};   // regiões 3, 5 e 6
    QSemaphore cruzamentoEsquerdo{PERMISSOES_POR_CRUZAMENTO};  // regiões 4, 5 e 8
    QSemaphore cruzamentoDireito{PERMISSOES_POR_CRUZAMENTO};   // regiões 6, 7 e 9

    // Declarado por último para ser destruído primeiro: os trens param antes de
    // as regiões e os semáforos deixarem de existir
    std::vector<std::unique_ptr<Trem>> trens;
};

#endif // MALHA_H
