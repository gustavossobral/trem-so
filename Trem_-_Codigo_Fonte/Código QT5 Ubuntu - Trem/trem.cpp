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
        case 1:     //Trem 1
            if (y == 30 && x < 330)
                x += 10;
            else if (x == 330 && y < 150)
                y += 10;
            else if (x > 60 && y == 150)
                x -= 10;
            else
                y -= 10;
            emit updateGUI(id, x, y);    //Emite um sinal
            break;
        case 2:     //Trem 2
            if (y == 30 && x < 600)
                x += 10;
            else if (x == 600 && y < 150)
                y += 10;
            else if (x > 330 && y == 150)
                x -= 10;
            else
                y -= 10;
            emit updateGUI(id, x, y);    //Emite um sinal
            break;
        default:
            break;
        }
        msleep(velocidade);
    }
}
