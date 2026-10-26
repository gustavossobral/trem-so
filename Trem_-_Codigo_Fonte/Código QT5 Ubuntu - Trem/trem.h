#ifndef TREM_H
#define TREM_H

#include <QPoint>
#include <QRect>
#include <QThread>

#include <atomic>

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
    static constexpr int VELOCIDADE_MAXIMA = 200;

    // trilho: retângulo percorrido pelo canto superior esquerdo do trem
    Trem(int id, QRect trilho, QPoint inicio, QObject *parent = nullptr);

public slots:
    // 0 para o trem; VELOCIDADE_MAXIMA o deixa quase invisível de tão rápido
    void setVelocidade(int velocidade);

signals:
    void updateGUI(int id, int x, int y);

protected:
    void run() override;

private:
    QPoint proximaPosicao() const;

    const int id;
    const QRect trilho;
    QPoint posicao;
    std::atomic<int> velocidade{0};  // alterada pela interface, lida pela thread do trem
};

#endif // TREM_H
