#include "personajes.h"

Personajes::Personajes() {
    vida = -1;
    velocidad = -1;
}
int Personajes::getVida(void){
    return vida;
}

int Personajes::getVelocidad(void){
    return velocidad;
}


void Personajes::setVida(int vida1){
    vida = vida1;
}

void Personajes::setVelocidad(int velocidad1){
    velocidad = velocidad1;
}

Personajes::~Personajes(){
}
