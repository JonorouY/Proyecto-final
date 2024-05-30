#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include "jugador.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateMisil();
    void enableLaunch();
    void updatePositions();
    void dispararEnemigo(QGraphicsPixmapItem* enemigo, QGraphicsPixmapItem* balaEnemigo);
    void checkCollisions();
    void movOndular();
    void resetInvulnerability();
    void resetFig20();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene1;
    QGraphicsScene *scene2;
    QGraphicsScene *scene3;
    QGraphicsPixmapItem *fig1;
    QGraphicsPixmapItem *fig2;
    QGraphicsPixmapItem *fig3;
    QGraphicsPixmapItem *fig4;
    QGraphicsPixmapItem *fig5;
    QGraphicsPixmapItem *fig6;
    QGraphicsPixmapItem *fig7;
    QGraphicsPixmapItem *fig8;
    QGraphicsPixmapItem *fig9;
    QGraphicsPixmapItem *fig10;
    QGraphicsPixmapItem *fig11;
    QGraphicsPixmapItem *fig12;
    QGraphicsPixmapItem *fig13;
    QGraphicsPixmapItem *fig14;
    QGraphicsPixmapItem *fig15;
    QGraphicsPixmapItem *fig16;
    QGraphicsPixmapItem *fig17;
    QGraphicsPixmapItem *fig18;
    QGraphicsPixmapItem *fig19;
    QGraphicsPixmapItem *fig20;
    QGraphicsPixmapItem *fig21;
    QGraphicsPixmapItem *balaEnSprite;
    QTimer *misilTimer;
    QTimer *launchTimer;
    QTimer *moveTimer;
    QTimer *balaTimer;
    double reductionStep;
    int tiempoTotal;
    int misilCount;
    int lvl;
    bool pierde;
    bool canLaunch;
    void launchMisil();
    void launchBala();
    void setupScene1();
    void setupScene2();
    void setupScene3();
    void loadCurrentScene();
    void resetScene1();
    void resetScene2();
    void resetScene3();
    QGraphicsLineItem *l1, *l2;
    QList<QGraphicsRectItem*> obst;
    // Variables para movimiento circular
    double angle; // Ángulo actual
    double radius; // Radio del círculo
    QPointF center; // Centro del círculo
    QPointF balaDirection;
    QTimer *jumpTimer;
    bool isJumping;
    bool balaEnMovimiento;
    bool Municion;
    qreal initialVelocity;
    qreal gravity;
    qreal jumpTime;
    qreal startX;
    qreal startY;
    qreal velocityX;
    qreal velocityY;

    void startJump();
    void updateJump();
    void updateBala();

    Jugador PP;

    QList<QGraphicsPixmapItem*> enemigos;
    QList<QGraphicsPixmapItem*> balasEnemigos;
    QList<QGraphicsRectItem*> muros;
    QList<QTimer*> disparoTimers;
    QTimer *movOndularT;
    bool isInvulnerable;
    QTimer invulnerabilityTimer;
    QTimer fig20Timer;
    bool isFig20Invulnerable;


};
#endif // MAINWINDOW_H
