#include "personajemov.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include <cmath>

PersonajeMov::PersonajeMov(QGraphicsView *view, float velIn, float theta, QGraphicsItem *im)
    : QGraphicsPixmapItem(im), velIn(velIn), theta(theta), xIn(520), yIn(327)
{
    dir = 0;
    tiempoTrans = 0;
    meta = true;
    caer = true;
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
    viewRect = view->size();
    QRectF sceneRect = view->sceneRect();
    qDebug() << viewRect << " " << sceneRect << " " << view->size().width();
    spriteSheet.load(":/Imagenes/Personaje.png");

    QPixmap sprite = spriteSheet.copy(spriteX, spriteY, spriteWidth, spriteHeight);
    setPixmap(sprite);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &PersonajeMov::applyGravity);
    timer->start(1);

    timer1 = new QTimer(this);
    connect(timer1, &QTimer::timeout, this, &PersonajeMov::mov);
}
int PersonajeMov::getPosicionX(){
    return x;
}
int PersonajeMov::getPosicionY(){
    return y;
}
bool PersonajeMov::getMeta(){
    return meta;
}
bool PersonajeMov::getCaer(){
    return caer;
}
void PersonajeMov::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    keysPressed.insert(key);
    if (key == Qt::Key_A) {
        TiempoA.start();
    }
    else if (key == Qt::Key_W) {
        TiempoW.start();
    }
    else if (key == Qt::Key_D) {
        TiempoD.start();
    }

    if (keysPressed.contains(Qt::Key_A) && keysPressed.contains(Qt::Key_W)) {
        // Inicia el evento especial cuando se presionan A y W simultáneamente
        if (isOnGround()) {
            qint64 intervalA = TiempoA.elapsed();
            qint64 intervalW = TiempoW.elapsed();
            if (qAbs(intervalA - intervalW) <= maxInterval) {
                // Movimiento parabólico hacia la izquierda
                dir = -1;
                xIn = x;
                yIn = y;
                tiempoTrans = 0;
                timer1->start(5);
                keysPressed.remove(Qt::Key_W);
                keysPressed.remove(Qt::Key_A);
            }
        }
    }
    else if (keysPressed.contains(Qt::Key_D) && keysPressed.contains(Qt::Key_W)) {
        // Inicia el evento especial cuando se presionan D y W simultáneamente
        if (isOnGround()) {
            qint64 intervalD = TiempoD.elapsed();
            qint64 intervalW = TiempoW.elapsed();
            if (qAbs(intervalD - intervalW) <= maxInterval) {
                // Movimiento parabólico hacia la derecha
                dir = 1;
                xIn = x;
                yIn = y;
                tiempoTrans = 0;
                timer1->start(5);
                keysPressed.remove(Qt::Key_W);
                keysPressed.remove(Qt::Key_D);
            }
        }
    }
    else {
        switch (event->key()) {
        case Qt::Key_A:
            if (isOnGround()) {
                moveBy(-5, 0);
            }
            break;
        case Qt::Key_D:
            if (isOnGround()) {
                moveBy(5, 0);
            }
            break;
        case Qt::Key_W:
            if (isOnGround()) {
                dir = 0;
                xIn = x;
                yIn = y;
                tiempoTrans = 0;
                timer1->start(5);
            }
            break;
        case Qt::Key_S:
            if (isOnGround()) {
                moveBy(0, 5);
            }
            break;
        default:
            QGraphicsItem::keyPressEvent(event);
        }
    }
}

void PersonajeMov::keyReleaseEvent(QKeyEvent *event)
{
    keysPressed.remove(event->key());
    QGraphicsItem::keyReleaseEvent(event);
}

void PersonajeMov::moveBy(int dx, int dy)
{
    qreal newX = x + dx;
    qreal newY = y + dy;

    if (newX > viewRect.width() - spriteWidth || newX < 0) {
        newX = x;
    }
    if (newY > viewRect.height() - spriteHeight) {
        newY = y;
    }

    x = newX;
    y = newY;

    setPos(x, y);
    qDebug() << "Position: " << x << ", " << y;
}

void PersonajeMov::setSprite(int dir)
{
    spriteX = 60 * cont;
    spriteY = dir;

    if (spriteX + spriteWidth <= spriteSheet.width() && spriteY + spriteHeight <= spriteSheet.height()) {
        QPixmap sprite = spriteSheet.copy(spriteX, spriteY, spriteWidth, spriteHeight);
        setPixmap(sprite);
        cont = (cont + 1) % 7;
    } else {
        qDebug() << "Sprite out of bounds!";
    }
}

bool PersonajeMov::isOnGround()
{
    QList<QGraphicsItem*> collidingItemsList = collidingItems();
    for (QGraphicsItem* item : collidingItemsList) {
        if (dynamic_cast<QGraphicsPixmapItem*>(item)) {
            qDebug() << "On ground";
            return true;
        }
    }
    qDebug() << "Not on ground";
    return false;
}

void PersonajeMov::applyGravity()
{
    if (!isOnGround()) {
        moveBy(0, 1);
    }
    if ( x >=  250 && x <= 310 && y >= 50 && y <= 100)
    {
        meta=true;
        qDebug() << "Meta true";
    }
    else{
        meta = false;
        qDebug() << "Meta false";
    }
    if(y >=390 ){
        caer=true;
    }
    else{
        caer=false;
    }


}

void PersonajeMov::movParabolico(float *dt)
{
    timer->stop();
    x = xIn + (velIn * cos(theta) * *dt) * dir;
    y = yIn - (velIn * sin(theta) * *dt) + (0.5 * 9.8 * *dt * *dt);

    if (isOnGround() && tiempoTrans >= 0.5) {
        timer1->stop();
        timer->start(10);
        return;
    }

    setPos(x, y);
}

void PersonajeMov::mov()
{
    movParabolico(&tiempoTrans);
    tiempoTrans += 0.01;
}

PersonajeMov::~PersonajeMov() {
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }
    if (timer1) {
        timer1->stop();
        delete timer1;
        timer1 = nullptr;
    }
}

