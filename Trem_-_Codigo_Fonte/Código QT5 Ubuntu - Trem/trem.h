#ifndef TREM_H
#define TREM_H

#include <QThread>

/*
 * Classe Trem herda QThread
 * Classe Trem passa a ser uma thread.
 * A função START inicializa a thread. Após inicializada, a thread irá executar a função RUN.
 * Para parar a execução da função RUN da thread, basta executar a função REQUESTINTERRUPTION.
 *
*/
class Trem : public QThread
{
    Q_OBJECT

public:
    Trem(int id, int x, int y, QObject *parent = nullptr);

//Cria um sinal
signals:
    void updateGUI(int id, int x, int y);

protected:
    void run() override;    //função a ser executada pela thread

private:
    int id;                 //ID do trem
    int x;                  //posição X do trem na tela
    int y;                  //posição Y do trem na tela
    int velocidade = 100;   //Velocidade. É o tempo de dormir em milisegundos entre a mudança de posição do trem
};

#endif // TREM_H
