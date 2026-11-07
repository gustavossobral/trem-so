#ifndef MALHA_H
#define MALHA_H

#include "trem.h"

#include <QSemaphore>

#include <array>
#include <memory>
#include <vector>

class Malha
{
public:
    static constexpr int QUANTIDADE_TRENS = 6;

    Malha();
    ~Malha();

    Trem &trem(int id);

private:
    Regiao *regiao(int numero);
    void criarTrem(QPoint cantoSuperiorEsquerdo, QPoint cantoInferiorDireito, QPoint inicio,
                   std::vector<Trecho> rota);

    static constexpr int PERMISSOES_POR_CRUZAMENTO = 2;  // 2 de 3 trens, evita deadlock

    std::array<Regiao, 9> regioes;  // regiões críticas 1 a 9
    QSemaphore cruzamentoSuperior{PERMISSOES_POR_CRUZAMENTO};  // regiões 1, 2 e 3
    QSemaphore cruzamentoCentral{PERMISSOES_POR_CRUZAMENTO};   // regiões 3, 5 e 6
    QSemaphore cruzamentoEsquerdo{PERMISSOES_POR_CRUZAMENTO};  // regiões 4, 5 e 8
    QSemaphore cruzamentoDireito{PERMISSOES_POR_CRUZAMENTO};   // regiões 6, 7 e 9

    std::vector<std::unique_ptr<Trem>> trens;  // por último: destruídos primeiro
};

#endif
