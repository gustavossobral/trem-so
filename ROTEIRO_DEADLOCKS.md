# Roteiro do vídeo: parte 2, deadlocks

Integrante: **Álvaro Barbosa Filho**

Esta é a segunda metade do vídeo do Projeto dos Trens (IMD0036 - 2026.2). Você
explica como os deadlocks são evitados, mostra mais composições de velocidade e
encerra o vídeo. A primeira metade, com a abertura e as colisões, fica com
Gustavo Henrique de Sousa Sobral e está em `ROTEIRO_COLISOES.md`.

Duração da sua parte: cerca de 1 min 40 s (1:58 a 3:37).

## O vídeo completo

| Tempo | Quem | Parte |
| --- | --- | --- |
| 0:00 – 0:17 | Gustavo | Abertura |
| 0:17 – 0:38 | Gustavo | A tela |
| 0:38 – 0:52 | Gustavo | Caso 1: todos parados |
| 0:52 – 1:05 | Gustavo | Caso 2: todos no máximo |
| 1:05 – 1:58 | Gustavo | Como as colisões são evitadas (composição 1) |
| 1:58 – 3:00 | **Álvaro** | Como os deadlocks são evitados (composição 2) |
| 3:00 – 3:25 | **Álvaro** | Composições 3 e 4 |
| 3:25 – 3:37 | **Álvaro** | Encerramento |

Total: cerca de 3 min 37 s, abaixo do limite de 4 minutos.

## O que o enunciado exige

- Em dupla, os dois integrantes aparecem no vídeo **com câmera e áudio**: um
  explica a solução para as colisões e o outro a solução para os deadlocks. As
  outras partes podem ser feitas por qualquer um.
- O vídeo tem no máximo 4 minutos e mostra os trens primeiro todos parados,
  depois todos no máximo e depois em composições variadas, com pelo menos 5
  segundos de cada caso. Os dois primeiros casos ficam na parte 1; as
  composições variadas continuam na sua parte.
- Não é obrigatório mostrar código.

## Antes de gravar

- Grave a tela e a câmera juntas, por exemplo no OBS Studio, com a câmera num
  canto que não cubra a malha nem os sliders. Teste o microfone antes.
- Compile e abra o programa por um terminal comum do sistema (veja o README). Se
  ele fechar ao abrir, use `QT_QPA_PLATFORMTHEME= ./Trem`.
- Deixe a janela do programa à esquerda e o PDF do enunciado à direita, na
  página 1, com a Figura 2 visível. O programa não mostra o número das regiões.
- Os sliders não mostram números, então os valores abaixo são posições
  aproximadas: 0 no início, 100 no meio, 150 a três quartos e 200 no fim.
- Atalhos úteis: clique na alça do slider (isso dá foco sem mudar o valor) e use
  `Home` para ir a 0, `End` para ir a 200, `PageUp`/`PageDown` para ±10 e as
  setas para ±1.
- Nenhum trem fica em 0 na sua parte. O trem parado aparece só no primeiro caso,
  na parte 1.
- Em cada composição, deixe os sliders parados por pelo menos 6 segundos, para
  garantir os 5 exigidos.

**Antes de apertar para gravar**, deixe a composição 2 pronta:

| a | b | c | d | e | f |
| --- | --- | --- | --- | --- | --- |
| 200 (fim) | ~180 | ~160 | ~20 | ~10 | ~30 |

Os trens de cima (a, b e c) ficam perto do máximo e os de baixo (d, e e f) perto
do mínimo.

## 6. Como os deadlocks são evitados (1:58 – 3:00)

**Tela:** composição 2 rodando. Aponte com o mouse para o cruzamento de cima, no
meio da borda inferior do trilho do `a`, onde as regiões 1, 2 e 3 se encontram.
Mostre o mesmo ponto na Figura 2.

> Eu sou Álvaro Barbosa Filho e vou explicar os deadlocks. Deixei os trens de cima perto do
> máximo e os de baixo perto do mínimo.
>
> Só com mutex, porém, pode haver deadlock nos quatro cruzamentos onde três regiões
> se encontram. No de cima, o a vai da região 1 para a 2, o c da 2 para a 3 e o b
> da 3 para a 1. Se os três chegam juntos, cada um segura uma região esperando a do
> próximo, e ninguém anda.
>
> Para isso, cada cruzamento tem um semáforo com duas permissões. O trem pega uma
> antes de entrar na região que leva ao cruzamento e devolve logo depois de entrar
> na seguinte. Com no máximo dois dos três trens ali, a espera circular nunca se
> fecha.
>
> Os trens b, c e e passam por três cruzamentos seguidos. Para não se esperarem em
> ciclo de um cruzamento para outro, os três pegam primeiro a permissão do
> cruzamento central, sempre na mesma ordem.

## 7. Composições 3 e 4 (3:00 – 3:25)

**Ações:**

1. `m` em ~10 (bem perto do início, sem chegar a 0). Mantenha por 6 segundos.
2. `f` em 200 (fim). Mantenha por 6 segundos.
3. `m` em ~100 (meio). Todos os sliders, inclusive o do `f`, vão para o meio.
   Mantenha por 6 segundos.

> Agora todos bem devagar, perto do mínimo, mas sem parar.
>
> *(pausa)*
>
> Coloco só o f no máximo. Ele dispara e, de vez em quando, para esperando o c ou o
> e liberar as regiões 7 e 9.
>
> *(pausa)*
>
> E se eu mexer no mestre de novo, todos os sliders, inclusive o do f, voltam a
> seguir o mestre.

## 8. Encerramento (3:25 – 3:37)

**Tela:** programa rodando com o mestre no meio.

> Resumindo: nove mutexes evitam as colisões, quatro semáforos evitam os deadlocks,
> cada trem anda na sua própria thread e só para quando realmente precisa.
> Agradecemos a atenção!

## Conferência da sua parte

- [ ] Você aparece na câmera e dá para ouvir a sua voz do início ao fim.
- [ ] Você diz o seu nome no começo da explicação dos deadlocks.
- [ ] As composições 2, 3 e 4 ficam na tela por pelo menos 5 segundos cada, sem
      nenhum trem em 0.
- [ ] Depois do `f` ajustado sozinho, o mestre sobrescreve todos os sliders.

## Conferência do vídeo completo

- [ ] As duas partes juntas duram no máximo 4 minutos. Se passar, encurte as
      pausas da parte 7.
- [ ] Os dois integrantes aparecem com câmera e áudio, cada um explicando a sua
      solução.
- [ ] A dupla foi informada no SIGAA dentro do prazo. Sem isso, o trabalho conta
      como individual, e enviar em dupla dá nota mínima aos dois.
- [ ] O nome dos dois integrantes está na submissão.
- [ ] Se o vídeo for enviado por link, ele abre numa janela anônima sem pedir
      acesso.
