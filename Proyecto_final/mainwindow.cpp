#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "personajemov.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QDebug>
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , lvl(2) //Se puede escoger el nivel cambiando éste valor 1,2 o 3
    , pierde(false)
    , misilTimer(new QTimer(this))
    , launchTimer(new QTimer(this))
    , moveTimer(new QTimer(this))
    , balaTimer(new QTimer(this))
    , movOndularT(new QTimer(this))
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
    , PP(100,6,50,50)
    , balaEnMovimiento(false)
    , Municion(false)
    , isInvulnerable(false)
    , isFig20Invulnerable(false)
{
    ui->setupUi(this);
    setupScene1();
    setupScene2();
    setupScene3();
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
        fig20->setPos(1000,1000);
        balaEnMovimiento = false;
        balaTimer->stop();
        return;
    }

    // Verificar colisiones con enemigos
    for (int i = 0; i < enemigos.size(); ++i) {
        if (fig20->collidesWithItem(enemigos[i])) {
            scene3->removeItem(enemigos[i]);
            enemigos.removeAt(i);
            scene3->removeItem(balasEnemigos[i]);
            balasEnemigos.removeAt(i);
            fig20->setVisible(false);
            balaTimer->stop();
            balaEnMovimiento = false;
            return;
        }
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
    balaEnMovimiento = false;
    launchTimer->stop();
}


void MainWindow::updatePositions()
{
    if(lvl == 1)
    {
        // Incrementar el ángulo
        angle += 0.05; // Ajusta este valor para cambiar la velocidad del movimiento circular

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
    ui->graphicsView->scale(1.2, 1.2);

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

    QPixmap meta(":/Imagenes/bala.png");
    fig13 = new QGraphicsPixmapItem();
    scene2->addItem(fig13);
    fig13->setPixmap(meta);
    fig13->setScale(0.05);
    fig13->setPos(273, 70);

    fig13->setVisible(false);


    PersonajeMov *jug1 = new PersonajeMov(ui->graphicsView,35,45*(3.1415/180));
    scene2 -> addItem(jug1);
    jug1->setPos(520,327);
    jug1->setScale(0.5);
    jug1->setFlag(QGraphicsItem::ItemIsFocusable);
    jug1->setFocus();

}
void MainWindow::setupScene3()
{
    qDebug() << "Configurando fondo de scene3";
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
    // Crear enemigos
    QPixmap enemigo(":/Imagenes/Enemigo_Ab.png");
    for (int i = 0; i < 10; i++) {
        qDebug() << "Agregando enemigo" << i;
        QGraphicsPixmapItem* enemigoItem = new QGraphicsPixmapItem();
        enemigoItem->setPixmap(enemigo);
        enemigoItem->setScale(0.6);
        int xPos = 58 + (i * (648 - 110) / 9);
        enemigoItem->setPos(xPos, 30);
        scene3->addItem(enemigoItem);
        enemigos.append(enemigoItem);

        qDebug() << "Agregando bala para enemigo" << i;
        // Crear balas para los enemigos
        QGraphicsPixmapItem* balaEnemigo = new QGraphicsPixmapItem();
        balaEnemigo->setPixmap(QPixmap(":/Imagenes/balaEn.png"));
        balaEnemigo->setScale(0.05);
        balaEnemigo->setVisible(false);
        scene3->addItem(balaEnemigo);
        balasEnemigos.append(balaEnemigo);

        qDebug() << "Configurando temporizador de disparo para enemigo" << i;
        // Configurar el temporizador de disparo para cada enemigo
        QTimer* disparoTimer = new QTimer(this);
        connect(disparoTimer, &QTimer::timeout, this, [this, enemigoItem, balaEnemigo](){
            dispararEnemigo(enemigoItem, balaEnemigo);
        });
        disparoTimers.append(disparoTimer);
        disparoTimer->start(2000);
    }

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

    qDebug() << "setupScene3 completado";
    //Invulnerabilidad
    connect(&invulnerabilityTimer, &QTimer::timeout, this, &MainWindow::resetInvulnerability);
    invulnerabilityTimer.setSingleShot(true);

    // invulnerabilidad cuando chocan bala con bala
    connect(&fig20Timer, &QTimer::timeout, this, &MainWindow::resetFig20);
    fig20Timer.setSingleShot(true); // El temporizador solo dispara una vez
}
void MainWindow::movOndular()
{
    // Movimiento ondulatorio horizontal del sprite balaEn
    qreal waveAmplitude = 100; // Amplitud de la onda
    qreal waveFrequency = 0.05; // Frecuencia de la onda
    qreal startX = 110; // Posición inicial en X
    qreal endX = 648; // Posición final en X
    qreal speed = 5; // Velocidad de movimiento

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
        resetScene3();
    }

    checkCollisions();
}
void MainWindow::dispararEnemigo(QGraphicsPixmapItem* enemigo, QGraphicsPixmapItem* balaEnemigo)
{
    balaEnemigo->setPos(enemigo->pos() + QPointF(10, 20)); // Ajustar la posición inicial de la bala
    balaEnemigo->setVisible(true);

    // Configurar el movimiento de la bala
    QTimer* balaTimer = new QTimer(this);
    connect(balaTimer, &QTimer::timeout, this, [balaEnemigo, balaTimer](){
        QPointF newPos = balaEnemigo->pos() + QPointF(0, 10);
        if (newPos.y() > 600) { // Asumiendo que 600 es el límite inferior de la escena
            balaEnemigo->setVisible(false);
            balaTimer->stop();
            balaTimer->deleteLater();
        } else {
            balaEnemigo->setPos(newPos);
        }
    });
    balaTimer->start(50);
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
    qDebug() << "Borrando disparoTimers";
    // Detener y limpiar los temporizadores de disparo
    for (QTimer* timer : disparoTimers) {
        timer->stop();
        delete timer;
    }
    disparoTimers.clear();
    qDebug() << "limpaindo scene3";
    // Eliminar todos los elementos de la escena
    scene3->clear();

    // Limpiar las listas de enemigos, balas, muros, etc.
    enemigos.clear();
    balasEnemigos.clear();
    muros.clear();

    // Resetear cualquier otra variable relevante
    angle = 0;
    Municion = false;
    PP.setVida(100);
    PP.setMunicion(6);

    // Volver a configurar la escena
    setupScene3();
    loadCurrentScene();
}

void MainWindow::checkCollisions()
{
    // Primero, verificar colisiones entre fig20 y las balas de los enemigos
    if (!isFig20Invulnerable) { // Solo si fig20 no es invulnerable
        for (auto balaEnemigo : balasEnemigos) {
            if (fig20->collidesWithItem(balaEnemigo)) {
                isFig20Invulnerable = true;       // Activar la invulnerabilidad de fig20
                fig20Timer.start(1900);           // Iniciar el temporizador de invulnerabilidad por 1,9 segundos
                fig20->setVisible(false);         // Hacer invisible fig20
                balaEnemigo->setVisible(false);// Hacer invisible la bala
                break; // Salir del bucle una vez que se ha encontrado una colisión
            }
        }
    }

    if (isInvulnerable) {
        return; // Si es invulnerable, no hacer nada
    }

    // Verificar colisiones entre fig19 y las balas de los enemigos
    for (auto balaEnemigo : balasEnemigos) {
        if (fig19->collidesWithItem(balaEnemigo)) {
            PP.setVida(PP.getVida() - 40); // Reducir la vida del personaje
            if (PP.getVida() <= 0) {
                resetScene3(); // Si la vida es igual o menor a 0, reiniciar la escena 3
            } else {
                isInvulnerable = true;           // Activar la invulnerabilidad
                invulnerabilityTimer.start(500); // Iniciar el temporizador de invulnerabilidad por 1 segundo
            }
            return; // Solo se permite una colisión a la vez
        }
    }
}

void MainWindow::resetInvulnerability()
{
    isInvulnerable = false;
}

void MainWindow::resetFig20()
{
    isFig20Invulnerable = false; // Después de 1,9 segundos, se desactiva la invulnerabilidad
    fig20->setVisible(true);     // Hacer visible fig20 nuevamente
    for(auto balaEnemigo : balasEnemigos)
    {
        balaEnemigo->setVisible(true);
    }
}
