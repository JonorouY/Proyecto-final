#ifndef PERSONAJES_H
#define PERSONAJES_H

#include <QMainWindow>

class Personajes: public QObject
{
    Q_OBJECT
protected:
    int vida;
    int velocidad;
public:
    Personajes();
    ~Personajes();
    int getVida(void);
    int getVelocidad(void);

    void setVida(int);
    void setMunicion(int);
    void setVelocidad(int);
};

#endif // PERSONAJES_H
