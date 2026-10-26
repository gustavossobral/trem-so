#ifndef MALHA_H
#define MALHA_H

#include "trem.h"

#include <array>
#include <memory>
#include <vector>

/*
 * Malha ferroviária da Figura 1: seis trens (a até f), cada um no seu trilho, e as
 * nove regiões críticas da Figura 2. A malha é dona dos trens e encerra as threads
 * deles ao ser destruída.
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
                   std::vector<Regiao *> rota);

    std::array<Regiao, 9> regioes;

    // Declarado depois das regiões para ser destruído antes: os trens param primeiro
    std::vector<std::unique_ptr<Trem>> trens;
};

#endif // MALHA_H
