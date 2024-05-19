#include "jugador.h"

Jugador::Jugador(int vida1, int municion1, int velocidad1, int energia1) {
    setVida(vida1);
    setVelocidad(velocidad1);
    energia = energia1;
    municion = municion1;
}
int Jugador::getEnergia(void){
    return energia;
}
void Jugador::setEnergia(int energia1){
    energia = energia1;
}
int Jugador::getMunicion(void){
    return municion;
}
void Jugador::setMunicion(int municion1){
    municion = municion1;
}


int Jugador::Vida() const{
    return vida;
}
int Jugador::Municion() const{
    return municion;
}
int Jugador::Velocidad() const{
    return velocidad;
}
int Jugador::Energia() const{
    return energia;
}

void Jugador::operator=(const Jugador& nuevoJugador){
    vida = nuevoJugador.Vida();
    municion = nuevoJugador.Municion();
    velocidad = nuevoJugador.Velocidad();
    energia = nuevoJugador.Energia();
}
bool Jugador::operator==(const Jugador& nuevoJugador){
    return((this->vida==nuevoJugador.Vida()) && (this->municion==nuevoJugador.Municion()) && (this->velocidad==nuevoJugador.Velocidad())&& (this->energia==nuevoJugador.Energia()));
}
Jugador::~Jugador(){

}
