#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>
#include "jugador.h"
#include "personajemov.h"

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
    void movOndular();
    void enemyShoot();
    void Finlvl2();
    void on_pushButton_clicked();
    void on_Button_Clicked1();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene1;
    QGraphicsScene *scene2;
    QGraphicsScene *scene3;

    QGraphicsScene *scene4;

    QGraphicsPixmapItem *fig1, *fig2, *fig3, *fig4, *fig5, *fig6, *fig7, *fig8, *fig9, *fig10, *fig11, *fig12, *fig13, *fig14, *fig15, *fig16, *fig17, *fig18, *fig19, *fig20, *fig21;
    QGraphicsPixmapItem *balaEnSprite;
    QGraphicsPixmapItem *enemigo1, *enemigo2, *enemigo3, *enemigo4, *enemigo5, *enemigo6, *enemigo7, *enemigo8, *enemigo9;
    QGraphicsPixmapItem *bala1, *bala2, *bala3, *bala4, *bala5, *bala6, *bala7, *bala8, *bala9;
    QTimer *misilTimer;
    QTimer *launchTimer;
    QTimer *moveTimer;
    QTimer *balaTimer;
    QTimer *terminarLvl2;
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

    void setupScene4();

    void loadCurrentScene();
    void resetScene1();
    void resetScene2();
    void resetScene3();
    void checkCollisions();
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
    PersonajeMov *jug1;
    int intentos;
    int cantidadEne;
    QString nombre;

    QList<QGraphicsRectItem*> muros;
    QTimer *movOndularT;
    QTimer fig20Timer;

    int puntaje;

    QTimer *enemyShootTimer;
    bool bala1Existe, bala2Existe, bala3Existe, bala4Existe, bala5Existe, bala6Existe, bala7Existe, bala8Existe, bala9Existe;


};
#endif // MAINWINDOW_H
