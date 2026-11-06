# Projeto dos Trens

Trabalho de Sistemas Operacionais (IMD0036 - 2026.2).

Autores: Gustavo Henrique de Sousa Sobral e Álvaro Barbosa Filho

O programa controla uma malha ferroviária com seis trens (de a até f). Cada trem
roda em uma thread própria e circula no sentido anti-horário sem bater em outro e
sem travar a malha. O código fica em `Trem_-_Codigo_Fonte/Código QT5 Ubuntu - Trem/`
e parte do código-fonte disponibilizado no SIGAA.

## Como compilar e executar

### Requisitos

- Ubuntu (ou outra distribuição Linux com ambiente gráfico)
- Qt 5, `qmake` e um compilador com suporte a C++17

Para instalar tudo no Ubuntu:

```sh
sudo apt install build-essential qtbase5-dev qt5-qmake
```

O Qt Creator é opcional (`sudo apt install qtcreator`).

### Pelo terminal

```sh
cd "Trem_-_Codigo_Fonte/Código QT5 Ubuntu - Trem"
mkdir build && cd build
qmake ../Trem.pro
make
./Trem
```

A pasta `build/` é ignorada pelo Git. Depois de mexer no código, rode `make` de
novo dentro dela para recompilar.

### Pelo Qt Creator

Abra `Trem.pro` (File → Open File or Project), escolha o kit padrão e execute
com Ctrl+R.

### Problemas comuns

- `qmake: command not found`: falta instalar o Qt 5 (veja os requisitos acima).
- O programa fecha ao abrir com `GLib-GIO-ERROR: Settings schema
  'org.gnome.settings-daemon.plugins.xsettings' does not contain a key named
  'antialiasing'`. Isso acontece quando o Qt lê configurações do GNOME que ele não
  reconhece, o que é comum no terminal integrado do VS Code instalado via snap.
  Rode o programa em um terminal normal do sistema ou desative o tema da plataforma:

  ```sh
  QT_QPA_PLATFORMTHEME= ./Trem
  ```

  Se ainda falhar no terminal do VS Code, limpe também a variável do snap:

  ```sh
  env -u GSETTINGS_SCHEMA_DIR QT_QPA_PLATFORMTHEME= ./Trem
  ```

- `QSocketNotifier: Can only be used with threads started with QThread`: é só um
  aviso do Qt. O programa funciona normalmente.

## Organização do código

| Arquivo | Responsabilidade |
| --- | --- |
| `trem.h` / `trem.cpp` | Thread de um trem: anda pelo trilho e trava/destrava as regiões críticas da sua rota |
| `malha.h` / `malha.cpp` | Geometria da malha, as 9 regiões críticas, os 4 semáforos dos cruzamentos e a rota de cada trem |
| `mainwindow.*` | Tela com a malha (Figura 1) e os controles de velocidade (Figura 3) |

## Controle de velocidade

Cada trem tem um slider de 0 a 200. Em 0 o trem fica parado. Acima disso ele anda
`20 × velocidade` pixels por segundo, ou seja, 4000 px/s no máximo.

O slider `m` é o mestre. Quando ele se move, todos os sliders individuais vão para
o mesmo valor. Daí em diante dá para ajustar cada trem separadamente, até alguém
mexer no mestre de novo.

## Como as colisões são evitadas

As 9 regiões críticas da Figura 2 são trechos de trilho que dois trens
compartilham, e cada uma tem seu `QMutex`. Uma região inclui todos os pixels que o
trem ocupa ao passar pelo trecho, até os cantos onde ele encosta em outros trechos.

O trem trava o mutex no passo em que encostaria na região e destrava quando sai dela
por inteiro. Com isso ele só para se a região estiver mesmo ocupada, e o outro trem
pode entrar logo que ela fica livre.

## Como os deadlocks são evitados

As regiões se encontram em quatro cruzamentos:

| Cruzamento | Regiões | Trens que fazem a curva |
| --- | --- | --- |
| superior | 1, 2 e 3 | a, b e c |
| central | 3, 5 e 6 | b, c e e |
| esquerdo | 4, 5 e 8 | b, d e e |
| direito | 6, 7 e 9 | c, e e f |

Em cada cruzamento, a região por onde um trem sai é a mesma por onde outro entra.
No cruzamento superior, `a` vai de 1 para 2, `c` de 2 para 3 e `b` de 3 para 1. Se
os três chegarem ao mesmo tempo, cada um fica esperando a região que o outro ocupa
e ninguém sai do lugar.

Para evitar esse deadlock, cada cruzamento tem um `QSemaphore` com 2 permissões. O
trem pega uma permissão antes de entrar na região que leva ao cruzamento e devolve
logo depois de entrar na região seguinte. Só dois dos três trens conseguem estar
nessa situação ao mesmo tempo, então o ciclo de espera nunca se completa.

Os trens `b`, `c` e `e` passam por três cruzamentos seguidos, sempre com o central
no meio. Se cada um segurasse o semáforo de um cruzamento enquanto espera o do
próximo, os três poderiam acabar esperando uns pelos outros entre cruzamentos
diferentes. Para isso não acontecer, todos pegam os semáforos na mesma ordem,
começando pelo do cruzamento central logo na entrada do trecho compartilhado.

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

Na tabela, "antes" e "depois" são em relação ao momento em que o trem trava o
mutex da região. O mutex é destravado quando o trem sai da região.

## Encerramento

Quando a janela é fechada, cada trem recebe um pedido de interrupção. Ele segue na
velocidade máxima até sair das regiões críticas e só então encerra a thread, para
que nenhum outro trem fique preso esperando um mutex que ninguém vai destravar.
