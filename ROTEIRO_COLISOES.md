# Roteiro do vídeo: parte 1, colisões

Integrante: **Gustavo Henrique de Sousa Sobral**

Esta é a primeira metade do vídeo do Projeto dos Trens (IMD0036 - 2026.2). Você
abre o vídeo, apresenta a tela, mostra os trens parados e no máximo e explica
como as colisões são evitadas. A segunda metade, com os deadlocks, fica com
Álvaro Barbosa Filho e está em `ROTEIRO_DEADLOCKS.md`.

Duração da sua parte: cerca de 2 minutos (0:00 a 1:58).

## O vídeo completo

| Tempo | Quem | Parte |
| --- | --- | --- |
| 0:00 – 0:17 | **Gustavo** | Abertura |
| 0:17 – 0:38 | **Gustavo** | A tela |
| 0:38 – 0:52 | **Gustavo** | Caso 1: todos parados |
| 0:52 – 1:05 | **Gustavo** | Caso 2: todos no máximo |
| 1:05 – 1:58 | **Gustavo** | Como as colisões são evitadas (composição 1) |
| 1:58 – 3:00 | Álvaro | Como os deadlocks são evitados (composição 2) |
| 3:00 – 3:25 | Álvaro | Composições 3 e 4 |
| 3:25 – 3:37 | Álvaro | Encerramento |

Total: cerca de 3 min 37 s, abaixo do limite de 4 minutos.

## O que o enunciado exige

- Em dupla, os dois integrantes aparecem no vídeo **com câmera e áudio**: um
  explica a solução para as colisões e o outro a solução para os deadlocks. As
  outras partes podem ser feitas por qualquer um.
- O vídeo tem no máximo 4 minutos e mostra os trens primeiro todos parados,
  depois todos no máximo e depois em composições variadas, com pelo menos 5
  segundos de cada caso.
- Não é obrigatório mostrar código.

## Antes de gravar

- Grave a tela e a câmera juntas, por exemplo no OBS Studio, com a câmera num
  canto que não cubra a malha nem os sliders. Teste o microfone antes.
- Compile e abra o programa por um terminal comum do sistema (veja o README). Se
  ele fechar ao abrir, use `QT_QPA_PLATFORMTHEME= ./Trem`.
- Deixe a janela do programa à esquerda e o PDF do enunciado à direita, na
  página 1, com a Figura 2 visível. O programa não mostra o número das regiões.
- Comece a gravar com o programa recém-aberto: todos os sliders começam em 20.
- Os sliders não mostram números, então os valores abaixo são posições
  aproximadas: 0 no início, 100 no meio, 150 a três quartos e 200 no fim.
- Atalhos úteis: clique na alça do slider (isso dá foco sem mudar o valor) e use
  `Home` para ir a 0, `End` para ir a 200, `PageUp`/`PageDown` para ±10 e as
  setas para ±1.
- Em cada caso, deixe os sliders parados por pelo menos 6 segundos, para garantir
  os 5 exigidos.

## 1. Abertura (0:00 – 0:17)

**Tela:** programa já aberto, trens andando na velocidade inicial.

> Olá, eu sou Gustavo Henrique de Sousa Sobral, e este é o Projeto dos Trens, de
> Sistemas Operacionais, IMD0036, do professor Nelson Oliveira, feito em dupla com
> Álvaro Barbosa Filho. Eu explico como evitamos as colisões, e Álvaro explica os
> deadlocks.

## 2. A tela (0:17 – 0:38)

**Tela:** passe o mouse pela malha e depois pelos sliders.

> À esquerda está a malha da Figura 1, com os seis trens, de a até f, cada um
> girando no sentido anti-horário no seu próprio trilho. À direita, um slider de 0
> a 200 para cada trem e o controle mestre, m. Os trens já saem andando quando o
> programa abre, e cada um roda numa thread independente.

## 3. Caso 1: todos parados (0:38 – 0:52)

**Ação:** leve o slider `m` até o início (0). Todos os sliders acompanham.
Espere pelo menos 6 segundos com tudo parado.

> Primeiro, a velocidade mínima. Levo o controle mestre para zero, e todos os
> sliders vão junto. Todos os trens param.

*(pausa de 6 segundos)*

## 4. Caso 2: todos no máximo (0:52 – 1:05)

**Ação:** leve o slider `m` até o fim (200). Mantenha por pelo menos 6 segundos.

> Agora, a velocidade máxima: mestre em 200. Os trens andam tão rápido que quase
> somem da tela, e mesmo assim nenhum colide e nada trava.

## 5. Como as colisões são evitadas (1:05 – 1:58)

**Ações, enquanto fala a primeira frase:**

1. `m` em ~150 (três quartos): todos os trens em 150.
2. `b` em ~5 (quase no início).
3. `a` em 200 (fim).

**Tela:** depois das ações, aponte com o mouse para o trem `a` parado logo antes da
região 1 enquanto o `b` passa por ela. Mostre a região 1 na Figura 2.

> Agora vou variar as velocidades: todo mundo perto de 150, o b quase parado e o a
> no máximo.
>
> Para evitar colisões, cada uma das nove regiões críticas da Figura 2, que são os
> trechos de trilho divididos por dois trens, tem um mutex. Antes de encostar numa
> região, o trem trava o mutex dela; se outro trem estiver lá, ele espera. Assim
> que sai da região por completo, ele destrava.
>
> Repara no trem a: antes ele era quase invisível, e agora aparece parado logo
> antes da região 1, esperando o b passar. Ele segue no mesmo instante em que o b
> sai. Como o trem só trava a região quando vai realmente entrar nela, ele só
> espera quando precisa, e a malha é bem aproveitada.
>
> Agora, Álvaro explica como evitamos os deadlocks.

Aqui termina a sua parte.

## Conferência da sua parte

- [ ] Você aparece na câmera e dá para ouvir a sua voz do início ao fim.
- [ ] Os nomes dos dois integrantes são ditos na abertura.
- [ ] Todos parados por pelo menos 5 segundos.
- [ ] Todos no máximo por pelo menos 5 segundos.
- [ ] A composição 1 fica na tela por pelo menos 5 segundos, sem nenhum trem em 0.
- [ ] A sua parte dura cerca de 2 minutos. Se passar muito disso, o vídeo
      completo pode estourar os 4 minutos.
