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
    QGraphicsScene *scene2;
    QGraphicsPixmapItem *fig1;
    QGraphicsPixmapItem *fig2;
    QGraphicsPixmapItem *fig3;
    QTimer *misilTimer;
    QTimer *launchTimer;
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
    QList <QGraphicsRectItem*> obst;
    QTimer *moveTimer;
    qreal x1,x2,y1,y2;


};
#endif // MAINWINDOW_H
