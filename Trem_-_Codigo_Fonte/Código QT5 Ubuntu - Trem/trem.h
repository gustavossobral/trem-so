#ifndef TREM_H
#define TREM_H

#include <QPoint>
#include <QRect>
#include <QThread>

/*
 * Classe Trem herda QThread
 * Classe Trem passa a ser uma thread.
 * A função START inicializa a thread. Após inicializada, a thread irá executar a função RUN.
 * Para parar a execução da função RUN da thread, basta executar a função REQUESTINTERRUPTION.
 *
 * O trem percorre o contorno do seu trilho no sentido anti-horário.
 */
class Trem : public QThread
{
    Q_OBJECT

public:
    // trilho: retângulo percorrido pelo canto superior esquerdo do trem
    Trem(int id, QRect trilho, QPoint inicio, QObject *parent = nullptr);

signals:
    void updateGUI(int id, int x, int y);

protected:
    void run() override;

private:
    QPoint proximaPosicao() const;

    const int id;
    const QRect trilho;
    QPoint posicao;
    int velocidade = 5;  // tempo, em milissegundos, entre um passo e outro
};

#endif // TREM_H
