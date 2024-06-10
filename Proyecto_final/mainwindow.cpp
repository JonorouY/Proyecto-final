#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "personajemov.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QList>
#include <QColor>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lvl(1) //Se puede escoger el nivel cambiando éste valor 1,2 o 3
    , pierde(false)
    , misilTimer(new QTimer(this))
    , launchTimer(new QTimer(this))
    , moveTimer(new QTimer(this))
    , balaTimer(new QTimer(this))
    , movOndularT(new QTimer(this))
    , enemyShootTimer(new QTimer(this))
    , terminarLvl2(new QTimer(this))
    , reductionStep(0.04)
    , tiempoTotal(0)
    , misilCount(0)
    , cantidadEne(0)
    , intentos(1)
    , canLaunch(true)
    , scene1(new QGraphicsScene(this))
    , scene2(new QGraphicsScene(this))
    , scene3(new QGraphicsScene(this))
    , scene4(new QGraphicsScene(this))
    , angle(0) // Inicializar el ángulo a 0
    , radius(120) // Establecer el radio del círculo
    , center(160, 150) // Establecer el centro del círculo
    , PP(100,6,50,50)
    , balaEnMovimiento(false)
    , Municion(false)
    , bala1Existe(true)
    , bala2Existe(true)
    , bala3Existe(true)
    , bala4Existe(true)
    , bala5Existe(true)
    , bala6Existe(true)
    , bala7Existe(true)
    , bala8Existe(true)
    , bala9Existe(true)
    , puntaje(1000)
{
    ui->setupUi(this);
    ui->graphicsView->scale(1.2, 1.2);
    ui->lcdNumber->display(puntaje);
    setupScene1();
    setupScene2();
    setupScene3();
    setupScene4();
    loadCurrentScene();
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


    if (lvl == 3){
        // Obtener la posición actual
        qreal currentX = fig19->pos().x();
        qreal currentY = fig19->pos().y();

        QPixmap PersonajeW(":/Imagenes/Personaje_Ar.png");
        QPixmap PersonajeA(":/Imagenes/Personaje_Iz.png");
        QPixmap PersonajeS(":/Imagenes/Personaje_Ab.png");
        QPixmap PersonajeD(":/Imagenes/Perzonaje_De.png");

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

        // Verificar si las nuevas posiciones están dentro de los límites y no colisionan con muros
        bool collision = false;
        for (auto muro : muros) {
            if (muro->collidesWithItem(fig19)) {
                collision = true;
                break;
            }
        }

        if (!collision && newX >= 110 && newX <= 648 && newY >= 25 && newY <= 330)
        {
            fig19->setPos(newX, newY);
        }
        if(!Municion)
        {
            if(fig19->collidesWithItem(fig21))
            {
                PP.setMunicion(PP.getMunicion()+5);
                scene3->removeItem(fig21);
                Municion = true;
            }
        }
    }
}
void MainWindow::launchBala()
{
    if (PP.getMunicion() <= 0) {
        return;
    }

    QPixmap currentPixmap = fig19->pixmap();
    QPointF initialPos = fig19->pos();

    // Determinar la dirección de la bala basándonos en la imagen actual del personaje
    if (currentPixmap.cacheKey() == QPixmap(":/Imagenes/Personaje_Ar.png").cacheKey()) {
        balaDirection = QPointF(0, -1); // Arriba
        initialPos += QPointF(28, 0); // +10 en X
    } else if (currentPixmap.cacheKey() == QPixmap(":/Imagenes/Personaje_Iz.png").cacheKey()) {
        balaDirection = QPointF(-1, 0); // Izquierda
        initialPos += QPointF(-1, 8); // +2 en X
    } else if (currentPixmap.cacheKey() == QPixmap(":/Imagenes/Personaje_Ab.png").cacheKey()) {
        balaDirection = QPointF(0, 1); // Abajo
        initialPos += QPointF(8, 43); // +3 en X y +8 en Y
    } else if (currentPixmap.cacheKey() == QPixmap(":/Imagenes/Perzonaje_De.png").cacheKey()) {
        balaDirection = QPointF(1, 0); // Derecha
        initialPos += QPointF(44, 28); // +10 en X y +8 en Y
    }

    fig20->setPos(initialPos);
    fig20->setVisible(true);

    // Configurar el temporizador para mover la bala
    disconnect(balaTimer, &QTimer::timeout, this, &MainWindow::updateBala);
    connect(balaTimer, &QTimer::timeout, this, &MainWindow::updateBala);
    balaTimer->start(50);

    // Reducir la munición y deshabilitar el lanzamiento temporalmente
    PP.setMunicion(PP.getMunicion() - 1);
    canLaunch = false;
    balaEnMovimiento = true;
    QTimer::singleShot(2000, this, &MainWindow::enableLaunch);
}
void MainWindow::updateBala()
{
    QPointF currentPos = fig20->pos();
    QPointF newPos = currentPos + 20 * balaDirection; // Mantener la velocidad constante

    if (newPos.x() < 0 || newPos.x() > 1000 || newPos.y() < 0 || newPos.y() > 1000) {
        fig20->setVisible(false);
        balaTimer->stop();
        balaEnMovimiento = false;
        return;
    }

    if(!fig20->isVisible())
    {
        puntaje-=20;
        ui->lcdNumber->display(puntaje);
        fig20->setPos(1000,1000);
        balaEnMovimiento = false;
        balaTimer->stop();
        return;
    }

    fig20->setPos(newPos);

    for (auto muro : muros) {
        if (fig20->collidesWithItem(muro)) {
            puntaje-=5;
            ui->lcdNumber->display(puntaje);
            balaTimer->stop();
            balaEnMovimiento = false;
            fig20->setVisible(false);
            return;
        }
    }
}

void MainWindow::launchMisil()
{
    if (misilCount >= 5) {
        puntaje-=50;
        ui->lcdNumber->display(puntaje);
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
            puntaje-=20;
            ui->lcdNumber->display(puntaje);
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
            puntaje-=20;
            ui->lcdNumber->display(puntaje);
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
    balaEnMovimiento = false;
    launchTimer->stop();
}


void MainWindow::updatePositions()
{
    if(lvl == 1)
    {
        // Incrementar el ángulo
        angle += 0.1; // Ajusta este valor para cambiar la velocidad del movimiento circular

        // Calcular la nueva posición en el movimiento circular
        qreal newX = center.x() + radius * cos(angle);
        qreal newY = center.y() + radius * sin(angle);

        // Actualizar la posición de fig1
        fig1->setPos(newX, newY);
    }

}

void MainWindow::setupScene1()
{
    // Fondo scene1
    QImage fondo1(":/Imagenes/Fondo.png");
    QBrush brocha1(fondo1);
    scene1->setBackgroundBrush(brocha1);
    // Configuramos el fondo
    scene1->setSceneRect(300, 200, 1, 1);

    // Avion
    QPixmap avion(":/Imagenes/avion.png");
    fig1 = new QGraphicsPixmapItem();
    scene1->addItem(fig1);
    fig1->setPixmap(avion);
    fig1->setScale(0.5);
    fig1->setPos(5, 35);

    // Mirilla
    QPixmap mirilla(":/Imagenes/mirilla.png");
    fig2 = new QGraphicsPixmapItem();
    scene1->addItem(fig2);
    fig2->setPixmap(mirilla);
    fig2->setScale(0.25);
    fig2->setPos(527, 310);

    // Misil
    QPixmap misil(":/Imagenes/bomba.png");
    fig3 = new QGraphicsPixmapItem();
    fig3->setPixmap(misil);
    fig3->setScale(1.0); // Tamaño inicial del misil
    fig3->setVisible(false); // El misil no es visible inicialmente
    scene1->addItem(fig3);

    // Configurar el temporizador del misil
    connect(misilTimer, &QTimer::timeout, this, &MainWindow::updateMisil);
    connect(launchTimer, &QTimer::timeout, this, &MainWindow::enableLaunch);
    // Conectar el temporizador de movimiento a la nueva función
    connect(moveTimer, &QTimer::timeout, this, &MainWindow::updatePositions);
    moveTimer->start(50); // Establecer el intervalo del temporizador (50 ms)
}


void MainWindow::setupScene2()
{
    // Fondo scene2
    QImage fondo2(":/Imagenes/fondo2.jpeg");
    QBrush brocha2(fondo2);
    scene2->setBackgroundBrush(brocha2);
    scene2->setSceneRect(282, 220, 1, 1);


    QPixmap Plataforma1(":/Imagenes/Obs4.png");
    fig4 = new QGraphicsPixmapItem();
    scene2->addItem(fig4);
    fig4->setPixmap(Plataforma1);
    fig4->setScale(0.3);
    fig4->setPos(-50, 310);

    QPixmap Plataforma2(":/Imagenes/Obs1.png");
    fig5 = new QGraphicsPixmapItem();
    scene2->addItem(fig5);
    fig5->setPixmap(Plataforma2);
    fig5->setScale(0.15);
    fig5->setPos(70, 255);

    QPixmap Plataforma3(":/Imagenes/Obs2.png");
    fig6 = new QGraphicsPixmapItem();
    scene2->addItem(fig6);
    fig6->setPixmap(Plataforma3);
    fig6->setScale(0.2);
    fig6->setPos(260, 345);


    QPixmap Plataforma5(":/Imagenes/Obs3.png");
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


    QPixmap Plataforma9(":/Imagenes/Obs3In.png");
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



    jug1 = new PersonajeMov(ui->graphicsView,35,45*(3.1415/180));
    scene2 -> addItem(jug1);
    jug1->setPos(520,327);
    jug1->setScale(0.5);
    jug1->setFlag(QGraphicsItem::ItemIsFocusable);
    jug1->setFocus();


    //comprobar si el nivel 2 ya finalizo
    terminarLvl2= new QTimer(this);
    connect(terminarLvl2, &QTimer::timeout, this, &MainWindow::Finlvl2);
    terminarLvl2->start(10);
}

void MainWindow::Finlvl2(){
    if(jug1->getMeta()==true){

        terminarLvl2->stop();
        disconnect(terminarLvl2, &QTimer::timeout, this, &MainWindow::Finlvl2);

        lvl=3;
        loadCurrentScene();
    }

    if(jug1->getCaer()==true){
        puntaje-=20;
        ui->lcdNumber->display(puntaje);
        resetScene2();
    }
}
void MainWindow::setupScene3()
{

    // Fondo scene3
    QImage fondo3(":/Imagenes/fondo3.png");
    QBrush brocha1(fondo3);
    scene3->setBackgroundBrush(brocha1);
    scene3->setSceneRect(400, 200, 1, 1);

    qDebug() << "Agregando personaje";
    // Personaje
    QPixmap Personaje(":/Imagenes/Personaje_Ar.png");
    fig19 = new QGraphicsPixmapItem();
    scene3->addItem(fig19);
    fig19->setPixmap(Personaje);
    fig19->setScale(0.2);
    fig19->setPos(200, 300);

    qDebug() << "Agregando bala del personaje";
    // Bala del personaje
    QPixmap bala(":/Imagenes/bala.png");
    fig20 = new QGraphicsPixmapItem();
    fig20->setPixmap(bala);
    fig20->setScale(0.01);
    fig20->setVisible(false);
    scene3->addItem(fig20);

    qDebug() << "Agregando sprite para el movimiento ondulatorio";
    // Agregar sprite para el movimiento ondulatorio
    QPixmap balaEn(":/Imagenes/balaEn.png");
    balaEnSprite = new QGraphicsPixmapItem();
    balaEnSprite->setPixmap(balaEn);
    balaEnSprite->setScale(0.1);
    scene3->addItem(balaEnSprite);
    balaEnSprite->setPos(100, 150); // Posición inicial

    connect(movOndularT, &QTimer::timeout, this, &MainWindow::movOndular);
    movOndularT->start(50);

    qDebug() << "Creando enemigos";
    QPixmap enemigo(":/Imagenes/Enemigo_Ab.png");

    enemigo1 = new QGraphicsPixmapItem();
    enemigo1->setPixmap(enemigo);
    enemigo1->setScale(0.6);
    enemigo1->setPos(58 + (1 * (648 - 110) / 9), 30);
    scene3->addItem(enemigo1);

    enemigo2 = new QGraphicsPixmapItem();
    enemigo2->setPixmap(enemigo);
    enemigo2->setScale(0.6);
    enemigo2->setPos(58 + (2 * (648 - 110) / 9), 30);
    scene3->addItem(enemigo2);

    enemigo3 = new QGraphicsPixmapItem();
    enemigo3->setPixmap(enemigo);
    enemigo3->setScale(0.6);
    enemigo3->setPos(58 + (3 * (648 - 110) / 9), 30);
    scene3->addItem(enemigo3);

    enemigo4 = new QGraphicsPixmapItem();
    enemigo4->setPixmap(enemigo);
    enemigo4->setScale(0.6);
    enemigo4->setPos(58 + (4 * (648 - 110) / 9), 30);
    scene3->addItem(enemigo4);

    enemigo5 = new QGraphicsPixmapItem();
    enemigo5->setPixmap(enemigo);
    enemigo5->setScale(0.6);
    enemigo5->setPos(58 + (5 * (648 - 110) / 9), 30);
    scene3->addItem(enemigo5);

    enemigo6 = new QGraphicsPixmapItem();
    enemigo6->setPixmap(enemigo);
    enemigo6->setScale(0.6);
    enemigo6->setPos(58 + (6 * (648 - 110) / 9), 30);
    scene3->addItem(enemigo6);

    enemigo7 = new QGraphicsPixmapItem();
    enemigo7->setPixmap(enemigo);
    enemigo7->setScale(0.6);
    enemigo7->setPos(58 + (7 * (648 - 110) / 9), 30);
    scene3->addItem(enemigo7);

    enemigo8 = new QGraphicsPixmapItem();
    enemigo8->setPixmap(enemigo);
    enemigo8->setScale(0.6);
    enemigo8->setPos(58 + (8 * (648 - 110) / 9), 30);
    scene3->addItem(enemigo8);

    enemigo9 = new QGraphicsPixmapItem();
    enemigo9->setPixmap(enemigo);
    enemigo9->setScale(0.6);
    enemigo9->setPos(58 + (9 * (648 - 110) / 9), 30);
    scene3->addItem(enemigo9);

    QPixmap balaEnememigos(":/Imagenes/balaEn.png");

    bala1 = new QGraphicsPixmapItem();
    bala1->setPixmap(balaEnememigos);
    bala1->setScale(0.05);
    bala1->setPos(122, 65);
    scene3->addItem(bala1);

    bala2 = new QGraphicsPixmapItem();
    bala2->setPixmap(balaEnememigos);
    bala2->setScale(0.05);
    bala2->setPos(182, 65);
    scene3->addItem(bala2);

    bala3 = new QGraphicsPixmapItem();
    bala3->setPixmap(balaEnememigos);
    bala3->setScale(0.05);
    bala3->setPos(242, 65);
    scene3->addItem(bala3);

    bala4 = new QGraphicsPixmapItem();
    bala4->setPixmap(balaEnememigos);
    bala4->setScale(0.05);
    bala4->setPos(302, 65);
    scene3->addItem(bala4);

    bala5 = new QGraphicsPixmapItem();
    bala5->setPixmap(balaEnememigos);
    bala5->setScale(0.05);
    bala5->setPos(362, 65);
    scene3->addItem(bala5);

    bala6 = new QGraphicsPixmapItem();
    bala6->setPixmap(balaEnememigos);
    bala6->setScale(0.05);
    bala6->setPos(422, 65);
    scene3->addItem(bala6);

    bala7 = new QGraphicsPixmapItem();
    bala7->setPixmap(balaEnememigos);
    bala7->setScale(0.05);
    bala7->setPos(482, 65);
    scene3->addItem(bala7);

    bala8 = new QGraphicsPixmapItem();
    bala8->setPixmap(balaEnememigos);
    bala8->setScale(0.05);
    bala8->setPos(542, 65);
    scene3->addItem(bala8);

    bala9 = new QGraphicsPixmapItem();
    bala9->setPixmap(balaEnememigos);
    bala9->setScale(0.05);
    bala9->setPos(602, 65);
    scene3->addItem(bala9);

    qDebug() << "Creando muros";
    // Crear muros cuadrados
    QImage img(":/Imagenes/Muro.png");
    QBrush brocha(img);
    muros.append(scene3->addRect(153,25,30,50,QPen(Qt::black), brocha));
    muros.append(scene3->addRect(213,25,30,50,QPen(Qt::black), brocha));
    muros.append(scene3->addRect(273,25,30,50,QPen(Qt::black), brocha));
    muros.append(scene3->addRect(332,25,30,50,QPen(Qt::black), brocha));
    muros.append(scene3->addRect(392,25,30,50,QPen(Qt::black), brocha));
    muros.append(scene3->addRect(452,25,30,50,QPen(Qt::black), brocha));
    muros.append(scene3->addRect(512,25,30,50,QPen(Qt::black), brocha));
    muros.append(scene3->addRect(572,25,30,50,QPen(Qt::black), brocha));

    qDebug() << "Agregando munición";
    QPixmap municion(":/Imagenes/municion.png");
    fig21 = new QGraphicsPixmapItem();
    fig21->setPixmap(municion);
    fig21->setScale(0.5);
    scene3->addItem(fig21);
    fig21->setPos(635,30);

    // Conectar el temporizador para que los enemigos disparen cada 2 segundos
    connect(enemyShootTimer, &QTimer::timeout, this, &MainWindow::enemyShoot);
    enemyShootTimer->start(50);

    qDebug() << "setupScene3 completado";
}

void MainWindow::setupScene4(){

    QImage fondo3(":/Imagenes/Fondo4.png");
    QBrush brocha1(fondo3);
    scene4->setBackgroundBrush(brocha1);
    scene4->setSceneRect(400, 200, 1, 1);

    QGraphicsTextItem *texto = new QGraphicsTextItem("¡Felicidades, ha ganado!");

    QFont fuente("Arial", 20);
    texto->setFont(fuente);
    texto->setDefaultTextColor(Qt::white);
    texto->setPos(250, 70);
    scene4->addItem(texto);


    QPushButton *boton = new QPushButton("GUARDAR PUNTAJE");
    boton->setMinimumSize(100, 50);
    QGraphicsProxyWidget *proxyWidget = scene4->addWidget(boton);
    // Establecer la posición del botón
    proxyWidget->setPos(350, 300); // Ajusta las coordenadas según sea necesario
    // Conectar la señal del botón a un slot
    connect(boton, &QPushButton::clicked, this, &MainWindow::on_Button_Clicked1);
}
void MainWindow::movOndular()
{
    // Movimiento ondulatorio horizontal del sprite balaEn
    qreal waveAmplitude = 100; // Amplitud de la onda
    qreal waveFrequency = 0.05; // Frecuencia de la onda
    qreal startX = 110; // Posición inicial en X
    qreal endX = 648; // Posición final en X
    qreal speed =5;// Velocidad de movimiento


    // Calcular la nueva posición ondulatoria
    qreal waveX = balaEnSprite->pos().x() + speed;
    qreal waveY = center.y() + waveAmplitude * sin(waveFrequency * waveX);

    // Verificar los límites y reiniciar la posición si es necesario
    if (waveX > endX) {
        waveX = startX;
    }

    // Actualizar la posición de balaEnSprite
    balaEnSprite->setPos(waveX, waveY +90);

    if(fig19->collidesWithItem(balaEnSprite))
    {
        puntaje-=20;
        ui->lcdNumber->display(puntaje);
        resetScene3();
    }
}

void MainWindow::loadCurrentScene()
{
    if (lvl == 1) {
        ui->graphicsView->setScene(scene1);
    }
    else if (lvl == 2) {
        ui->graphicsView->setScene(scene2);
    }
    else if (lvl == 3){
        ui->graphicsView->setScene(scene3);
    }
    else if (lvl == 4){
        ui->graphicsView->setScene(scene4);
    }

}

void MainWindow::resetScene1()
{
    // Detener los temporizadores
    misilTimer->stop();
    launchTimer->stop();
    moveTimer->stop();

    // Desconectar las señales de los temporizadores
    disconnect(misilTimer, &QTimer::timeout, this, &MainWindow::updateMisil);
    disconnect(launchTimer, &QTimer::timeout, this, &MainWindow::enableLaunch);
    disconnect(moveTimer, &QTimer::timeout, this, &MainWindow::updatePositions);

    scene1->clear();
    misilCount = 0;
    pierde = false;
    lvl = 1;

    setupScene1();
    loadCurrentScene();
}

void MainWindow::resetScene2()
{

    terminarLvl2->stop();
    disconnect(terminarLvl2, &QTimer::timeout, this, &MainWindow::Finlvl2);


    delete jug1;
    jug1 = nullptr;
    scene2->removeItem(jug1);
    scene2->clear();

    setupScene2();
    loadCurrentScene();
}



void MainWindow::resetScene3()
{

    // Detener y desconectar el temporizador de movimiento ondulatorio
    if (movOndularT) {
        movOndularT->stop();
        disconnect(movOndularT, &QTimer::timeout, this, &MainWindow::movOndular);
    }

    // Detener y desconectar el temporizador de disparo de los enemigos
    if (enemyShootTimer) {
        enemyShootTimer->stop();
        disconnect(enemyShootTimer, &QTimer::timeout, this, &MainWindow::enemyShoot);
    }

    // Eliminar todos los elementos de la escena
    scene3->clear();

    // Limpiar la lista de muros
    muros.clear();

    // Restablecer cualquier otra variable relevante
    angle = 0;
    Municion = false;
    PP.setVida(100);
    PP.setMunicion(6);
    cantidadEne=0;

    bala1Existe =true;
    bala2Existe =true;
    bala3Existe =true;
    bala4Existe =true;
    bala5Existe =true;
    bala6Existe =true;
    bala7Existe =true;
    bala8Existe =true;
    bala9Existe =true;


    // Volver a configurar la escena
    setupScene3();
    loadCurrentScene();
}
void MainWindow::enemyShoot()
{
    if(bala1Existe)
    {
        if (bala1->pos().y() < 400) { // Limite de pantalla
            bala1->setPos(bala1->pos().x(), bala1->pos().y() + 10); // Mover hacia abajo
        } else {
            bala1->setPos(bala1->pos().x(), 65); // Resetear la posición
        }
        // Comprobar colisión con el personaje
        if (fig19->collidesWithItem(bala1)) {
            puntaje-=10;
            ui->lcdNumber->display(puntaje);
            resetScene3();
        }
    }
    if(bala2Existe)
    {
        if (bala2->pos().y() < 400) { // Limite de pantalla
            bala2->setPos(bala2->pos().x(), bala2->pos().y() + 10); // Mover hacia abajo
        } else {
            bala2->setPos(bala2->pos().x(), 65); // Resetear la posición
        }
        // Comprobar colisión con el personaje
        if (fig19->collidesWithItem(bala2)) {
            puntaje-=10;
            ui->lcdNumber->display(puntaje);
            resetScene3();
        }
    }
    if(bala3Existe)
    {
        if (bala3->pos().y() < 400) { // Limite de pantalla
            bala3->setPos(bala3->pos().x(), bala3->pos().y() + 10); // Mover hacia abajo
        } else {
            bala3->setPos(bala3->pos().x(), 65); // Resetear la posición
        }
        // Comprobar colisión con el personaje
        if (fig19->collidesWithItem(bala3)) {
            puntaje-=10;
            ui->lcdNumber->display(puntaje);
            resetScene3();
        }
    }
    if(bala4Existe)
    {
        if (bala4->pos().y() < 400) { // Limite de pantalla
            bala4->setPos(bala4->pos().x(), bala4->pos().y() + 10); // Mover hacia abajo
        } else {
            bala4->setPos(bala4->pos().x(), 65); // Resetear la posición
        }
        // Comprobar colisión con el personaje
        if (fig19->collidesWithItem(bala4)) {
            puntaje-=10;
            ui->lcdNumber->display(puntaje);
            resetScene3();
        }
    }
    if(bala5Existe)
    {
        if (bala5->pos().y() < 400) { // Limite de pantalla
            bala5->setPos(bala5->pos().x(), bala5->pos().y() + 10); // Mover hacia abajo
        } else {
            bala5->setPos(bala5->pos().x(), 65); // Resetear la posición
        }
        // Comprobar colisión con el personaje
        if (fig19->collidesWithItem(bala5)) {
            puntaje-=10;
            ui->lcdNumber->display(puntaje);
            resetScene3();
        }
    }
    if(bala6Existe)
    {
        if (bala6->pos().y() < 400) { // Limite de pantalla
            bala6->setPos(bala6->pos().x(), bala6->pos().y() + 10); // Mover hacia abajo
        } else {
            bala6->setPos(bala6->pos().x(), 65); // Resetear la posición
        }
        // Comprobar colisión con el personaje
        if (fig19->collidesWithItem(bala6)) {
            puntaje-=10;
            ui->lcdNumber->display(puntaje);
            resetScene3();
        }
    }
    if(bala7Existe)
    {
        if (bala7->pos().y() < 400) { // Limite de pantalla
            bala7->setPos(bala7->pos().x(), bala7->pos().y() + 10); // Mover hacia abajo
        } else {
            bala7->setPos(bala7->pos().x(), 65); // Resetear la posición
        }
        // Comprobar colisión con el personaje
        if (fig19->collidesWithItem(bala7)) {
            puntaje-=10;
            ui->lcdNumber->display(puntaje);
            resetScene3();
        }
    }
    if(bala8Existe)
    {
        if (bala8->pos().y() < 400) { // Limite de pantalla
            bala8->setPos(bala8->pos().x(), bala8->pos().y() + 10); // Mover hacia abajo
        } else {
            bala8->setPos(bala8->pos().x(), 65); // Resetear la posición
        }
        // Comprobar colisión con el personaje
        if (fig19->collidesWithItem(bala8)) {
            puntaje-=10;
            ui->lcdNumber->display(puntaje);
            resetScene3();
        }
    }
    if(bala9Existe)
    {
        if (bala9->pos().y() < 400) { // Limite de pantalla
            bala9->setPos(bala9->pos().x(), bala9->pos().y() + 10); // Mover hacia abajo
        } else {
            bala9->setPos(bala9->pos().x(), 65); // Resetear la posición
        }
        // Comprobar colisión con el personaje
        if (fig19->collidesWithItem(bala9)) {
            puntaje-=10;
            ui->lcdNumber->display(puntaje);
            resetScene3();
        }
    }
    checkCollisions();
}

void MainWindow::checkCollisions()
{
    if(bala1Existe)
    {
        if(fig20->collidesWithItem(enemigo1))
        {
            qDebug() << "Colisión entre fig20 y enemigo1";
            bala1Existe=false;
            enemigo1->setVisible(false);
            bala1->setVisible(false);
            cantidadEne++;
        }
    }
    if(bala2Existe)
    {
        if(fig20->collidesWithItem(enemigo2))
        {
            qDebug() << "Colisión entre fig20 y enemigo1";
            bala2Existe=false;
            enemigo2->setVisible(false);
            bala2->setVisible(false);
            cantidadEne++;
        }
    }
    if(bala3Existe)
    {
        if(fig20->collidesWithItem(enemigo3))
        {
            qDebug() << "Colisión entre fig20 y enemigo1";
            bala3Existe=false;
            enemigo3->setVisible(false);
            bala3->setVisible(false);
            cantidadEne++;
        }
    }
    if(bala4Existe)
    {
        if(fig20->collidesWithItem(enemigo4))
        {
            qDebug() << "Colisión entre fig20 y enemigo1";
            bala4Existe=false;
            enemigo4->setVisible(false);
            bala4->setVisible(false);
            cantidadEne++;
        }
    }
    if(bala5Existe)
    {
        if(fig20->collidesWithItem(enemigo5))
        {
            qDebug() << "Colisión entre fig20 y enemigo1";
            bala5Existe=false;
            enemigo5->setVisible(false);
            bala5->setVisible(false);
            cantidadEne++;
        }
    }
    if(bala6Existe)
    {
        if(fig20->collidesWithItem(enemigo6))
        {
            qDebug() << "Colisión entre fig20 y enemigo1";
            bala6Existe=false;
            enemigo6->setVisible(false);
            bala6->setVisible(false);
            cantidadEne++;
        }
    }
    if(bala7Existe)
    {
        if(fig20->collidesWithItem(enemigo7))
        {
            qDebug() << "Colisión entre fig20 y enemigo1";
            bala7Existe=false;
            enemigo7->setVisible(false);
            bala7->setVisible(false);
            cantidadEne++;
        }
    }
    if(bala8Existe)
    {
        if(fig20->collidesWithItem(enemigo8))
        {
            qDebug() << "Colisión entre fig20 y enemigo1";
            bala8Existe=false;
            enemigo8->setVisible(false);
            bala8->setVisible(false);
            cantidadEne++;
        }
    }
    if(bala9Existe)
    {
        if(fig20->collidesWithItem(enemigo9))
        {
            qDebug() << "Colisión entre fig20 y enemigo1";
            bala9Existe=false;
            enemigo9->setVisible(false);
            bala9->setVisible(false);
            cantidadEne++;
        }
    }

    if(cantidadEne == 9){

        // Detener y desconectar el temporizador de movimiento ondulatorio
        if (movOndularT) {
            movOndularT->stop();
            disconnect(movOndularT, &QTimer::timeout, this, &MainWindow::movOndular);
        }

        // Detener y desconectar el temporizador de disparo de los enemigos
        if (enemyShootTimer) {
            enemyShootTimer->stop();
            disconnect(enemyShootTimer, &QTimer::timeout, this, &MainWindow::enemyShoot);
        }

        // Eliminar todos los elementos de la escena
        scene3->clear();

        // Limpiar la lista de muros
        muros.clear();
        lvl=4;
        loadCurrentScene();
    }
}

void MainWindow::on_pushButton_clicked()
{
    if(intentos == 1 ){
        nombre = ui->nombre->text();
    }
    intentos--;
}

void MainWindow::on_Button_Clicked1() {
    std::string text = nombre.toStdString() + " " + std::to_string(puntaje) + "\n";
    std::ofstream outFile("./Puntaje.txt" , std::ios::app);

    if (outFile.is_open())
    {
        outFile << text;
        outFile.close();
    }
    else
    {
        qDebug() << "No se pudo abrir el archivo para escritura";
    }
}
