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
    , lvl(1)
    , pierde(false)
    , misilTimer(new QTimer(this))
    , launchTimer(new QTimer(this))
    , moveTimer(new QTimer(this)) // Inicializar el nuevo temporizador
    , reductionStep(0.04)
    , tiempoTotal(0)
    , misilCount(0)
    , canLaunch(true)
    , scene1(new QGraphicsScene(this))
    , scene2(new QGraphicsScene(this))
    , angle(0) // Inicializar el ángulo a 0
    , radius(120) // Establecer el radio del círculo
    , center(160, 150) // Establecer el centro del círculo
{
    ui->setupUi(this);
    setupScene1();
    setupScene2();
    loadCurrentScene();

    // Conectar el temporizador de movimiento a la nueva función
    connect(moveTimer, &QTimer::timeout, this, &MainWindow::updatePositions);
    moveTimer->start(50); // Establecer el intervalo del temporizador (50 ms)
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
    launchTimer->start(5000); // Habilitar el lanzamiento después de 5 segundos
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

        if (fig3->collidesWithItem(fig1))
        {
            lvl = 2; // Verificar colisión con el avión y establecer gana a true
            misilTimer->stop();
            fig3->setVisible(false);
            loadCurrentScene(); // Cambiar a scene2
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

void MainWindow::updatePositions()
{
    // Incrementar el ángulo
    angle += 0.05; // Ajusta este valor para cambiar la velocidad del movimiento circular

    // Calcular la nueva posición en el movimiento circular
    qreal newX = center.x() + radius * cos(angle);
    qreal newY = center.y() + radius * sin(angle);

    // Actualizar la posición de fig1
    fig1->setPos(newX, newY);
}

void MainWindow::setupScene1()
{
    // Fondo scene1
    QImage fondo1("D:/Documents/ProyectoFinal/Imagenes/Fondo.png");
    QBrush brocha1(fondo1);
    scene1->setBackgroundBrush(brocha1);
    // Configuramos el fondo
    scene1->setSceneRect(300, 200, 1, 1);
    ui->graphicsView->scale(1.2, 1.2);

    // Avion
    QPixmap avion("D:/Documents/ProyectoFinal/Imagenes/avion.png");
    fig1 = new QGraphicsPixmapItem();
    scene1->addItem(fig1);
    fig1->setPixmap(avion);
    fig1->setScale(0.5);
    fig1->setPos(5, 35);

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
}

void MainWindow::setupScene2()
{
    // Fondo scene2
    QImage fondo2("D:/Documents/ProyectoFinal/Imagenes/avion.png");
    QBrush brocha2(fondo2);
    scene2->setBackgroundBrush(brocha2);

    // Aquí puedes añadir más elementos a scene2
    QGraphicsTextItem *texto = scene2->addText("¡Felicidades! Has pasado al nivel 2");
    texto->setPos(100, 100);
    texto->setDefaultTextColor(Qt::white);
}

void MainWindow::loadCurrentScene()
{
    if (lvl == 1) {
        ui->graphicsView->setScene(scene1);
    } else if (lvl == 2) {
        ui->graphicsView->setScene(scene2);
    }
}

void MainWindow::resetScene1()
{
    scene1->clear();
    misilCount = 0;
    pierde = false;
    lvl = 1;

    setupScene1();
    loadCurrentScene();
}
