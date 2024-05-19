#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include "personajes.h"
#include <QMainWindow>

class Enemigos : public Personajes
{
    Q_OBJECT
public:
    Enemigos(int vida1,int velocidad1);
    ~Enemigos();
    int Vida() const;
    int Velocidad() const;

    void operator=(const Enemigos& nuevoEnemigo);
    bool operator==(const Enemigos& nuevoEnemigo);
};

#endif // ENEMIGOS_H
