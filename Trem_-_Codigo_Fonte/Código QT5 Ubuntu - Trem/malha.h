#ifndef MALHA_H
#define MALHA_H

#include "trem.h"

#include <memory>
#include <vector>

/*
 * Malha ferroviária da Figura 1: seis trens (a até f), cada um no seu trilho.
 * A malha é dona dos trens e encerra as threads deles ao ser destruída.
 */
class Malha
{
public:
    static constexpr int QUANTIDADE_TRENS = 6;

    Malha();
    ~Malha();

    Trem &trem(int id);

private:
    void criarTrem(QPoint cantoSuperiorEsquerdo, QPoint cantoInferiorDireito, QPoint inicio);

    std::vector<std::unique_ptr<Trem>> trens;
};

#endif // MALHA_H
