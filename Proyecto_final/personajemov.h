#ifndef PERSONAJEMOV_H
#define PERSONAJEMOV_H
#include <QGraphicsItem>
#include <QGraphicsView>


class Qtimer;
class PersonajeMov: public QGraphicsItem
{
public:
    PersonajeMov(QGraphicsView *view, float velIn, qreal xIn, qreal yIn, float theta);
    void moveDy(int dx, int dy);
    void movParabolico(float *dy);
private:

    qreal posX;
    qreal posY;
    QSize viewRect;
    float velIn, theta,dir;
    Qtimer *timer;
    qreal xIn, yIn;
};
#endif // PERSONAJEMOV_H
