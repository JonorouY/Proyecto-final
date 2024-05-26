#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QTimer>

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

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene1;
    QGraphicsScene *scene2;    QGraphicsPixmapItem *fig1;
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
    QTimer *misilTimer;
    QTimer *launchTimer;
    QTimer *moveTimer;
    double reductionStep;
    int tiempoTotal;
    int misilCount;
    int lvl;
    bool pierde;
    bool canLaunch;
    void launchMisil();
    void setupScene1();
    void setupScene2();
    void loadCurrentScene();
    void resetScene1();
    QGraphicsLineItem *l1, *l2;
    QList<QGraphicsRectItem*> obst;
    // Variables para movimiento circular
    double angle; // Ángulo actual
    double radius; // Radio del círculo
    QPointF center; // Centro del círculo
    QTimer *jumpTimer;
    bool isJumping;
    qreal initialVelocity;
    qreal gravity;
    qreal jumpTime;
    qreal startX;
    qreal startY;
    qreal velocityX;
    qreal velocityY;

    void startJump();
    void updateJump();

};
#endif // MAINWINDOW_H
