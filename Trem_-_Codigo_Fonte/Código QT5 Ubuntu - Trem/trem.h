#ifndef TREM_H
#define TREM_H

#include <QMutex>
#include <QPoint>
#include <QRect>
#include <QThread>

#include <atomic>
#include <deque>
#include <vector>

// Região crítica: trecho de trilho compartilhado por dois trens
struct Regiao
{
    QRect area;      // pixels ocupados por um trem enquanto passa pelo trecho
    QMutex mutex{};  // só um trem por vez pode ocupar o trecho
};

/*
 * Classe Trem herda QThread
 * Classe Trem passa a ser uma thread.
 * A função START inicializa a thread. Após inicializada, a thread irá executar a função RUN.
 * Para parar a execução da função RUN da thread, basta executar a função ENCERRAR.
 *
 * O trem percorre o contorno do seu trilho no sentido anti-horário. A rota lista as
 * regiões críticas na ordem em que o trem passa por elas: antes de encostar numa
 * região, o trem trava o seu mutex; depois de sair dela por completo, destrava.
 */
class Trem : public QThread
{
    Q_OBJECT

public:
    static constexpr int TAMANHO = 20;  // lado, em pixels, do quadrado que representa o trem
    static constexpr int VELOCIDADE_MAXIMA = 200;

    // trilho: retângulo percorrido pelo canto superior esquerdo do trem
    Trem(int id, QRect trilho, QPoint inicio, std::vector<Regiao *> rota,
         QObject *parent = nullptr);

    // Pede para a thread terminar assim que o trem deixar as regiões críticas
    void encerrar();

public slots:
    // 0 para o trem; VELOCIDADE_MAXIMA o deixa quase invisível de tão rápido
    void setVelocidade(int velocidade);

signals:
    void updateGUI(int id, int x, int y);

protected:
    void run() override;

private:
    void andar();
    QPoint proximaPosicao() const;
    void entrarNaProximaRegiao(QPoint destino);
    void sairDasRegioesDeixadas();

    const int id;
    const QRect trilho;
    QPoint posicao;
    const std::vector<Regiao *> rota;
    std::size_t proximaRegiao = 0;         // índice, na rota, da próxima região crítica
    std::deque<Regiao *> regioesOcupadas;  // regiões travadas, da mais antiga à mais nova
    std::atomic<int> velocidade{0};        // alterada pela interface, lida pela thread do trem
};

#endif // TREM_H
