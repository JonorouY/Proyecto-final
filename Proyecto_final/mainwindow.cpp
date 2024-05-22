#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gana(false)
    , pierde(false)
    , misilTimer(new QTimer(this))
    , launchTimer(new QTimer(this))
    , reductionStep(0.04)
    , tiempoTotal(0)
    , misilCount(0)
    , canLaunch(true)
{
    ui->setupUi(this);

    scene1 = new QGraphicsScene();
    ui->graphicsView->setScene(scene1);

    // Fondo scene1
    QImage fondo1("D:/Documents/ProyectoFinal/Imagenes/Fondo.png");
    QBrush brocha1(fondo1);
    ui->graphicsView->setBackgroundBrush(brocha1);
    // Configuramos el fondo
    scene1->setSceneRect(300, 200, 1, 1);
    ui->graphicsView->scale(1.2, 1.2);

    // Avion
    QPixmap avion("D:/Documents/ProyectoFinal/Imagenes/avion.png");
    fig1 = new QGraphicsPixmapItem();
    scene1->addItem(fig1);
    fig1->setPixmap(avion);
    fig1->setScale(0.5);
    fig1->setPos(5, 30);

    // Mirilla
    QPixmap mirilla("D:/Documents/ProyectoFinal/Imagenes/mirilla.png");
    fig2 = new QGraphicsPixmapItem();
    scene1->addItem(fig2);
    fig2->setPixmap(mirilla);
    fig2->setScale(0.25);
    fig2->setPos(527, 310);

    // Misil
    QPixmap misil("D:/Documents/ProyectoFinal/Imagenes/bomba.png");
    fig3 = new QGraphicsPixmapItem();
    fig3->setPixmap(misil);
    fig3->setScale(1.0); // Tamaño inicial del misil
    fig3->setVisible(false); // El misil no es visible inicialmente
    scene1->addItem(fig3);

    // Configurar el temporizador del misil
    connect(misilTimer, &QTimer::timeout, this, &MainWindow::updateMisil);
    connect(launchTimer, &QTimer::timeout, this, &MainWindow::enableLaunch);

    // Crear scene2
    scene2 = new QGraphicsScene();
    // Puedes agregar aquí los elementos de scene2 si es necesario
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *w)
{
    // Obtener la posición actual
    qreal currentX = fig2->pos().x();
    qreal currentY = fig2->pos().y();

    // Variables para las nuevas posiciones
    qreal newX = currentX;
    qreal newY = currentY;

    // Cambiar las nuevas posiciones según la tecla presionada
    switch (w->key())
    {
    case Qt::Key_W: newY -= 5; break;
    case Qt::Key_S: newY += 5; break;
    case Qt::Key_A: newX -= 5; break;
    case Qt::Key_D: newX += 5; break;
    case Qt::Key_E: newX += 5; newY -= 5; break;
    case Qt::Key_Q: newX -= 5; newY -= 5; break;
    case Qt::Key_Z: newX -= 5; newY += 5; break;
    case Qt::Key_C: newX += 5; newY += 5; break;
    case Qt::Key_J:
        if (canLaunch) {
            launchMisil();
        }
        return;
    default: return; // Si no es una de las teclas mencionadas, salir
    }

    // Verificar si las nuevas posiciones están dentro de los límites
    if (newX >= 9 && newX <= 527 && newY >= 25 && newY <= 310)
    {
        fig2->setPos(newX, newY);
    }
}

void MainWindow::launchMisil()
{
    if (misilCount >= 5) {
        pierde = true;
        resetScene1();
        return;
    }

    fig3->setPos(fig2->pos()); // Posicionar el misil en la mirilla
    fig3->setVisible(true); // Hacer visible el misil
    fig3->setScale(1.0); // Restablecer el tamaño del misil
    tiempoTotal = 0; // Restablecer el tiempo total transcurrido
    misilTimer->start(50); // Iniciar el temporizador del misil
    misilCount++; // Incrementar el conteo de misiles
    canLaunch = false; // Deshabilitar el lanzamiento de misiles
    launchTimer->start(5000); // Habilitar el lanzamiento después de 7 segundos
}

void MainWindow::updateMisil()
{
    tiempoTotal += 50;

    if (tiempoTotal <= 2000)
    {
        fig3->setScale(fig3->scale() - reductionStep); // Reducir el tamaño del misil

        if (fig3->scale() <= 0.0)
        {
            misilTimer->stop();
            fig3->setVisible(false);
            return;
        }

        if (fig3->pos() == fig1->pos())
        {
            gana = true; // Verificar colisión con el avión y establecer gana a true
            misilTimer->stop();
            fig3->setVisible(false);
            ui->graphicsView->setScene(scene2); // Cambiar a scene2
            return;
        }
    }
    else if (tiempoTotal <= 5000)
    {
        fig3->setScale(fig3->scale() - reductionStep); // Continuar reduciendo el tamaño del misil

        if (fig3->scale() <= 0.0)
        {
            misilTimer->stop();
            fig3->setVisible(false);
            return;
        }
    }
    else
    {
        misilTimer->stop(); // Detener el temporizador después de 7 segundos
        fig3->setVisible(false); // Hacer invisible el misil
    }
}

void MainWindow::enableLaunch()
{
    canLaunch = true;
    launchTimer->stop();
}

void MainWindow::resetScene1()
{
    scene1->clear();
    misilCount = 0;
    pierde = false;
    gana = false;

    // Fondo scene1
    QImage fondo1("D:/Documents/ProyectoFinal/Imagenes/Fondo.png");
    QBrush brocha1(fondo1);
    ui->graphicsView->setBackgroundBrush(brocha1);
    // Configuramos el fondo
    scene1->setSceneRect(300, 200, 1, 1);
    ui->graphicsView->scale(1.2, 1.2);

    // Avion
    QPixmap avion("D:/Documents/ProyectoFinal/Imagenes/avion.png");
    fig1 = new QGraphicsPixmapItem();
    scene1->addItem(fig1);
    fig1->setPixmap(avion);
    fig1->setScale(0.5);
    fig1->setPos(5, 30);

    // Mirilla
    QPixmap mirilla("D:/Documents/ProyectoFinal/Imagenes/mirilla.png");
    fig2 = new QGraphicsPixmapItem();
    scene1->addItem(fig2);
    fig2->setPixmap(mirilla);
    fig2->setScale(0.25);
    fig2->setPos(527, 310);

    // Misil
    QPixmap misil("D:/Documents/ProyectoFinal/Imagenes/bomba.png");
    fig3 = new QGraphicsPixmapItem();
    fig3->setPixmap(misil);
    fig3->setScale(1.0); // Tamaño inicial del misil
    fig3->setVisible(false); // El misil no es visible inicialmente
    scene1->addItem(fig3);

    ui->graphicsView->setScene(scene1);
}
