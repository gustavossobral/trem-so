# Projeto dos Trens

Trabalho de Sistemas Operacionais (IMD0036 - 2026.2).

Autor: Gustavo Sobral

Controlador de uma malha ferroviária com seis trens (a até f) que circulam no
sentido anti-horário, cada um em sua própria thread, sem colisões e sem deadlocks.
O código fica em `Trem_-_Codigo_Fonte/Código QT5 Ubuntu - Trem/` e parte do
código-fonte disponibilizado no SIGAA.

## Como compilar e executar

É preciso ter o Qt 5 (no Ubuntu, o pacote `qtbase5-dev`) e um compilador com C++17.

No Qt Creator, basta abrir `Trem.pro` e executar. Pelo terminal:

```sh
cd "Trem_-_Codigo_Fonte/Código QT5 Ubuntu - Trem"
mkdir build && cd build
qmake ../Trem.pro
make
./Trem
```

## Organização do código

| Arquivo | Responsabilidade |
| --- | --- |
| `trem.h` / `trem.cpp` | Thread de um trem: anda pelo trilho e trava/destrava as regiões críticas da sua rota |
| `malha.h` / `malha.cpp` | Geometria da malha, as 9 regiões críticas, os 4 semáforos dos cruzamentos e a rota de cada trem |
| `mainwindow.*` | Tela com a malha (Figura 1) e os controles de velocidade (Figura 3) |

## Controle de velocidade

Cada trem tem um slider de 0 a 200. Em 0 o trem fica parado; acima disso ele anda
`20 × velocidade` pixels por segundo, chegando a 4000 px/s na velocidade máxima.

O slider `m` é o controle mestre: ao ser movido, coloca todos os sliders
individuais no seu valor. Depois disso, cada trem pode ser ajustado sozinho até o
mestre ser movido de novo.

## Como as colisões são evitadas

Cada uma das 9 regiões críticas da Figura 2 é um trecho de trilho compartilhado
por dois trens e tem um `QMutex`. A região corresponde aos pixels que um trem cobre
ao passar pelo trecho, incluindo os cantos onde ele encontra outros trechos.

O trem trava o mutex no passo em que vai encostar na região e o destrava assim que
sai dela por completo. Assim, ele só espera quando a próxima região está realmente
ocupada e a libera para o outro trem no primeiro instante possível.

## Como os deadlocks são evitados

As regiões se encontram em quatro cruzamentos:

| Cruzamento | Regiões | Trens que fazem a curva |
| --- | --- | --- |
| superior | 1, 2 e 3 | a, b e c |
| central | 3, 5 e 6 | b, c e e |
| esquerdo | 4, 5 e 8 | b, d e e |
| direito | 6, 7 e 9 | c, e e f |

Em cada cruzamento, cada trem sai por uma região pela qual outro trem entra. No
cruzamento superior, por exemplo, `a` vai de 1 para 2, `c` de 2 para 3 e `b` de 3
para 1. Se os três chegarem juntos, cada um espera a região do próximo e nenhum
anda: é o deadlock.

Para impedir isso, cada cruzamento tem um `QSemaphore` com 2 permissões. O trem
adquire uma permissão antes de entrar na região que leva ao cruzamento e a libera
logo depois de entrar na região seguinte. Como no máximo dois dos três trens ficam
nessa situação ao mesmo tempo, a espera circular nunca se fecha.

Os trens `b`, `c` e `e` passam por três cruzamentos seguidos, sempre com o central
no meio. Se cada um esperasse pelo semáforo do próximo cruzamento segurando o do
anterior, eles poderiam se esperar em ciclo de um cruzamento para outro. Por isso
todos adquirem os semáforos na mesma ordem: o do cruzamento central primeiro, já na
entrada do percurso compartilhado.

| Trem | Região em que entra | Adquire antes | Libera depois |
| --- | --- | --- | --- |
| a | 1 | superior | |
| | 2 | | superior |
| b | 4 | central, esquerdo | |
| | 5 | | esquerdo |
| | 3 | superior | central |
| | 1 | | superior |
| c | 2 | central, superior | |
| | 3 | | superior |
| | 6 | direito | central |
| | 7 | | direito |
| d | 8 | esquerdo | |
| | 4 | | esquerdo |
| e | 9 | central, direito | |
| | 6 | | direito |
| | 5 | esquerdo | central |
| | 8 | | esquerdo |
| f | 7 | direito | |
| | 9 | | direito |

"Antes" e "depois" se referem a travar o mutex da região. Ao sair de cada região,
o trem destrava o seu mutex.

## Encerramento

Ao fechar a janela, cada trem recebe um pedido de interrupção, anda na velocidade
máxima até sair das regiões críticas e só então termina a thread. Assim nenhum trem
fica bloqueado para sempre por um mutex deixado travado.
