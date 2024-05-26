#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QDebug>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lvl(3)
    , pierde(false)
    , misilTimer(new QTimer(this))
    , launchTimer(new QTimer(this))
    , moveTimer(new QTimer(this))
    , balaTimer(new QTimer(this))
    , reductionStep(0.04)
    , tiempoTotal(0)
    , misilCount(0)
    , canLaunch(true)
    , scene1(new QGraphicsScene(this))
    , scene2(new QGraphicsScene(this))
    , scene3(new QGraphicsScene(this))
    , angle(0) // Inicializar el ángulo a 0
    , radius(120) // Establecer el radio del círculo
    , center(160, 150) // Establecer el centro del círculo
{
    ui->setupUi(this);
    setupScene1();
    setupScene2();
    setupScene3();
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
    if(lvl==1){
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
    if (lvl==2){
        qreal currentX = fig15->pos().x();
        qreal currentY = fig15->pos().y();
        QPixmap PersonajeCC_De("Imagenes/PersonajeCC.png");
        QPixmap PersonajeCC_Iz("Imagenes/PersonajeCC_in.png");

        // Variables para las nuevas posiciones
        qreal newX = currentX;
        qreal newY = currentY;

        // Cambiar las nuevas posiciones según la tecla presionada
        switch (w->key())
        {
        case Qt::Key_W: newY -= 5; break;

        case Qt::Key_A: newX -= 5;
            fig15->setPixmap(PersonajeCC_Iz);
            break;
        case Qt::Key_D: newX += 5;
            fig15->setPixmap(PersonajeCC_De);
            break;
        default: return; // Si no es una de las teclas mencionadas, salir
        }

        // Verificar si las nuevas posiciones están dentro de los límites
        if (newX >= -10 && newX <= 562 && newY >= 40 && newY <= 1000)
        {
            fig15->setPos(newX, newY);
        }
    }
    if (lvl == 3){
        // Obtener la posición actual
        qreal currentX = fig19->pos().x();
        qreal currentY = fig19->pos().y();

        QPixmap PersonajeW("Imagenes/Personaje_Ar.png");
        QPixmap PersonajeA("Imagenes/Personaje_Iz.png");
        QPixmap PersonajeS("Imagenes/Personaje_Ab.png");
        QPixmap PersonajeD("Imagenes/Perzonaje_De.png");

        // Variables para las nuevas posiciones
        qreal newX = currentX;
        qreal newY = currentY;

        // Cambiar las nuevas posiciones según la tecla presionada
        switch (w->key())
        {
        case Qt::Key_W: newY -= 5;
            fig19->setPixmap(PersonajeW);
            break;
        case Qt::Key_S: newY += 5;
            fig19->setPixmap(PersonajeS);
            break;
        case Qt::Key_A: newX -= 5;
            fig19->setPixmap(PersonajeA);
            break;
        case Qt::Key_D: newX += 5;
            fig19->setPixmap(PersonajeD);
            break;
        case Qt::Key_J:
            if (canLaunch) {
                launchBala();
            }
            return;
        default: return; // Si no es una de las teclas mencionadas, salir
        }

        // Verificar si las nuevas posiciones están dentro de los límites
        if (newX >= 0 && newX <= 1000 && newY >= 0 && newY <= 1000)
        {
            fig19->setPos(newX, newY);
        }
    }
}
void MainWindow::launchBala()
{
    if (misilCount >= 6) {
        pierde = true;
        resetScene3();
        return;
    }

    fig20->setPos(fig19->pos());
    fig20->setVisible(true);

    QPixmap currentPixmap = fig19->pixmap(); // Obtener el pixmap actual
    if (currentPixmap.cacheKey() == QPixmap("Imagenes/Personaje_Ar.png").cacheKey()) {
        balaDirection = QPointF(0, -1); // Arriba
    } else if (currentPixmap.cacheKey() == QPixmap("Imagenes/Personaje_Iz.png").cacheKey()) {
        balaDirection = QPointF(-1, 0); // Izquierda
    } else if (currentPixmap.cacheKey() == QPixmap("Imagenes/Personaje_Ab.png").cacheKey()) {
        balaDirection = QPointF(0, 1); // Abajo
    } else if (currentPixmap.cacheKey() == QPixmap("Imagenes/Perzonaje_De.png").cacheKey()) {
        balaDirection = QPointF(1, 0); // Derecha
    }

    connect(balaTimer, &QTimer::timeout, this, &MainWindow::updateBala);
    balaTimer->start(50);

    misilCount++;
    canLaunch = false;
    QTimer::singleShot(2000, this, &MainWindow::enableLaunch);
}

void MainWindow::updateBala()
{
    QPointF currentPos = fig20->pos();
    QPointF newPos = currentPos + 5 * balaDirection;

    if (newPos.x() < 0 || newPos.x() > 1000 || newPos.y() < 0 || newPos.y() > 1000) {
        fig20->setVisible(false);
        balaTimer->stop();
        return;
    }

    fig20->setPos(newPos);
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
            canLaunch = true;
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
    QImage fondo1("Imagenes/Fondo.png");
    QBrush brocha1(fondo1);
    scene1->setBackgroundBrush(brocha1);
    // Configuramos el fondo
    scene1->setSceneRect(300, 200, 1, 1);
    ui->graphicsView->scale(1.2, 1.2);

    // Avion
    QPixmap avion("Imagenes/avion.png");
    fig1 = new QGraphicsPixmapItem();
    scene1->addItem(fig1);
    fig1->setPixmap(avion);
    fig1->setScale(0.5);
    fig1->setPos(5, 35);

    // Mirilla
    QPixmap mirilla("Imagenes/mirilla.png");
    fig2 = new QGraphicsPixmapItem();
    scene1->addItem(fig2);
    fig2->setPixmap(mirilla);
    fig2->setScale(0.25);
    fig2->setPos(527, 310);

    // Misil
    QPixmap misil("Imagenes/bomba.png");
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
    QImage fondo2("Imagenes/fondo2.jpeg");
    QBrush brocha2(fondo2);
    scene2->setBackgroundBrush(brocha2);
    scene2->setSceneRect(282, 220, 1, 1);


    QPixmap Plataforma1("Imagenes/Obs4.png");
    fig4 = new QGraphicsPixmapItem();
    scene2->addItem(fig4);
    fig4->setPixmap(Plataforma1);
    fig4->setScale(0.3);
    fig4->setPos(-50, 310);

    QPixmap Plataforma2("Imagenes/Obs1.png");
    fig5 = new QGraphicsPixmapItem();
    scene2->addItem(fig5);
    fig5->setPixmap(Plataforma2);
    fig5->setScale(0.15);
    fig5->setPos(70, 255);

    QPixmap Plataforma3("Imagenes/Obs2.png");
    fig6 = new QGraphicsPixmapItem();
    scene2->addItem(fig6);
    fig6->setPixmap(Plataforma3);
    fig6->setScale(0.2);
    fig6->setPos(260, 345);


    QPixmap Plataforma5("Imagenes/Obs3.png");
    fig8 = new QGraphicsPixmapItem();
    scene2->addItem(fig8);
    fig8->setPixmap(Plataforma5);
    fig8->setScale(0.30);
    fig8->setPos(450, 320);


    fig9 = new QGraphicsPixmapItem();
    scene2->addItem(fig9);
    fig9->setPixmap(Plataforma3);
    fig9->setScale(0.18);
    fig9->setPos(170, 210);

    fig10 = new QGraphicsPixmapItem();
    scene2->addItem(fig10);
    fig10->setPixmap(Plataforma3);
    fig10->setScale(0.18);
    fig10->setPos(310, 210);


    fig11 = new QGraphicsPixmapItem();
    scene2->addItem(fig11);
    fig11->setPixmap(Plataforma5);
    fig11->setScale(0.2);
    fig11->setPos(470, 180);


    QPixmap Plataforma9("Imagenes/Obs3In.png");
    fig12 = new QGraphicsPixmapItem();
    scene2->addItem(fig12);
    fig12->setPixmap(Plataforma9);
    fig12->setScale(0.18);
    fig12->setPos(235, 82);


    fig14 = new QGraphicsPixmapItem();
    scene2->addItem(fig14);
    fig14->setPixmap(Plataforma2);
    fig14->setScale(0.15);
    fig14->setPos(370, 130);

    QPixmap flecha("Imagenes/flecha.png");
    fig13 = new QGraphicsPixmapItem();
    scene2->addItem(fig13);
    fig13->setPixmap(flecha);
    fig13->setScale(0.25);
    fig13->setPos(480, 205);

    QPixmap Personaje("Imagenes/Personaje.png");
    fig15 = new QGraphicsPixmapItem();
    scene2->addItem(fig15);
    fig15->setPixmap(Personaje);
    fig15->setScale(0.5);
    fig15->setPos(515, 327);



    // Aquí puedes añadir más elementos a scene2

}
void MainWindow::setupScene3()
{
    // Fondo scene2
    QImage fondo3("Imagenes/fondo3.png");
    QBrush brocha1(fondo3);
    scene3->setBackgroundBrush(brocha1);
    // Configuramos el fondo
    //scene3->setSceneRect(300, 200, 1, 1);
    //ui->graphicsView->scale(1.2, 1.2);

    QPixmap Personaje("Imagenes/Personaje_Ar.png");
    fig19 = new QGraphicsPixmapItem();
    scene3->addItem(fig19);
    fig19->setPixmap(Personaje);
    fig19->setScale(0.5);
    fig19->setPos(515, 327);

    //Bala
    QPixmap bala("Imagenes/bomba.png");
    fig20 = new QGraphicsPixmapItem();
    fig20->setPixmap(bala);
    fig20->setScale(1.0); // Tamaño inicial del misil
    fig20->setVisible(false); // El misil no es visible inicialmente
    scene3->addItem(fig20);
}
void MainWindow::loadCurrentScene()
{
    if (lvl == 1) {
        ui->graphicsView->setScene(scene1);
    } else if (lvl == 2) {
        ui->graphicsView->setScene(scene2);
    }else if (lvl == 3){
        ui->graphicsView->setScene(scene3);
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

void MainWindow::resetScene2()
{
    scene2->clear();
    pierde = false;
    lvl = 2;

    setupScene2();
    loadCurrentScene();
}

void MainWindow::resetScene3()
{
    scene3->clear();
    misilCount = 0;
    pierde = false;
    lvl = 3;

    setupScene3();
    loadCurrentScene();
}
