#include "trem.h"

//Construtor
Trem::Trem(int id, int x, int y, QObject *parent)
    : QThread(parent), id(id), x(x), y(y)
{
}

//Função a ser executada após executar trem->START
void Trem::run()
{
    while (!isInterruptionRequested()) {
        switch (id) {
        case 1:     //Trem 1 (sentido anti-horário)
            if (y == 30 && x > 60)          //em cima: anda para a esquerda
                x -= 10;
            else if (x == 60 && y < 150)    //à esquerda: desce
                y += 10;
            else if (y == 150 && x < 330)   //embaixo: anda para a direita
                x += 10;
            else                            //à direita: sobe
                y -= 10;
            emit updateGUI(id, x, y);    //Emite um sinal
            break;
        case 2:     //Trem 2 (sentido anti-horário)
            if (y == 30 && x > 330)         //em cima: anda para a esquerda
                x -= 10;
            else if (x == 330 && y < 150)   //à esquerda: desce
                y += 10;
            else if (y == 150 && x < 600)   //embaixo: anda para a direita
                x += 10;
            else                            //à direita: sobe
                y -= 10;
            emit updateGUI(id, x, y);    //Emite um sinal
            break;
        default:
            break;
        }
        msleep(velocidade);
    }
}
