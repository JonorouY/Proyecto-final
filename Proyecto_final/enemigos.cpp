#include "enemigos.h"

Enemigos::Enemigos(int vida1,int velocidad1) {
    setVida(vida1);
    setVelocidad(velocidad1);
}
int Enemigos::Vida() const{
    return vida;
}

int Enemigos::Velocidad() const{
    return velocidad;
}
void Enemigos::operator=(const Enemigos& nuevoEnemigo){
    vida = nuevoEnemigo.Vida();
    velocidad = nuevoEnemigo.Velocidad();
}
bool Enemigos::operator==(const Enemigos& nuevoEnemigo){
    return((this->vida==nuevoEnemigo.Vida()) && (this->velocidad==nuevoEnemigo.Velocidad()));
}
Enemigos::~Enemigos(){

}
