#ifndef TREM_H
#define TREM_H

#include <QMutex>
#include <QPoint>
#include <QRect>
#include <QSemaphore>
#include <QThread>

#include <atomic>
#include <deque>
#include <vector>

// região crítica
struct Regiao
{
    QRect area;
    QMutex mutex{};  // um trem por vez
};

// região da rota e semáforos do cruzamento
struct Trecho
{
    Regiao *regiao;
    std::vector<QSemaphore *> adquirir;  // antes de travar o mutex
    std::vector<QSemaphore *> liberar;   // depois de travar o mutex
};

// cada trem é uma thread
class Trem : public QThread
{
    Q_OBJECT

public:
    static constexpr int TAMANHO = 20;  // lado do trem, em px
    static constexpr int VELOCIDADE_MAXIMA = 200;

    Trem(int id, QRect trilho, QPoint inicio, std::vector<Trecho> rota,
         QObject *parent = nullptr);

    void encerrar();  // pede para a thread terminar

public slots:
    void setVelocidade(int velocidade);

signals:
    void updateGUI(int id, int x, int y);

protected:
    void run() override;  // corpo da thread

private:
    void andar();
    QPoint proximaPosicao() const;
    void entrarNaProximaRegiao(QPoint destino);
    void sairDasRegioesDeixadas();

    const int id;
    const QRect trilho;
    QPoint posicao;
    const std::vector<Trecho> rota;
    std::size_t proximoTrecho = 0;
    std::deque<Regiao *> regioesOcupadas;  // mutexes travados
    std::atomic<int> velocidade{0};        // escrita pela interface, lida pela thread
};

#endif
