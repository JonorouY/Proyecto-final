#ifndef PERSONAJEMOV_H
#define PERSONAJEMOV_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QTimer>
#include <QElapsedTimer>

class PersonajeMov: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
private:
    qreal x = 520;
    qreal y = 327;
    const int maxInterval = 200;
    QSize viewRect;

    bool flag = true;
    bool meta;
    bool caer;
    int cont = 0;
    int spriteX = 0;
    int spriteY = 120;
    int spriteWidth = 40;
    int spriteHeight = 20;
    QPixmap sprite;
    QPixmap spriteSheet;
    QTimer *timer;
    QTimer *timer1;
    QSet<int> keysPressed;
    float velIn, theta, dir;
    qreal xIn, yIn;

    float tiempoTrans;

    QElapsedTimer TiempoA;
    QElapsedTimer TiempoD;
    QElapsedTimer TiempoW;

private slots:
    void applyGravity();
    void mov();

public:
    PersonajeMov(QGraphicsView *view, float velIn, float theta, QGraphicsItem* im = nullptr);
    ~PersonajeMov();
    void keyPressEvent(QKeyEvent *event) override;

    void moveBy(int dx, int dy);
    void moveKy(int dx, int dy);
    void keyReleaseEvent(QKeyEvent *event);
    void movParabolico(float *dt);
    int getPosicionX();
    int getPosicionY();
    bool getMeta();
    bool getCaer();

    void setSprite(int dir);
    bool isOnGround();
};

#endif
