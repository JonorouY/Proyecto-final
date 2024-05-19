#ifndef JUGADOR_H
#define JUGADOR_H

#include "personajes.h"
#include <QMainWindow>

class Jugador : public Personajes
{
    Q_OBJECT
private:
    int energia;
    int municion;

public:
    Jugador(int vida1,int municion1,int velocidad1,int energia1);
    ~Jugador();
    int getEnergia(void);
    int getMunicion(void);
    void setEnergia(int energia);
    void setMunicion(int);

    //sobrecarga de metodos
    int Vida() const;
    int Municion() const;
    int Velocidad() const;
    int Energia() const;

    void operator=(const Jugador& nuevoJugador);
    bool operator==(const Jugador& nuevoJugador);


};

#endif // JUGADOR_H
