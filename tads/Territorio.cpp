#include "Territorio.h"

/*
===========================================================================================
Funciones:

1. constructores: Uno es un constructor que esta por default vacio y otro recibe el codigo,
nombre, id del contienente, color del ocupante, y cantidad de ejercitos.
2. agregarVecino: Recibe un puntero a un territorio, primero revisa si es nulo o igual a el 
territorio al cual vamos a anadir el vecino retornamos false. si no es ninguno de los 2
revisa si el puntero ya es de uno de los vecinos, en caso de serlo retorna false. de caso
de no serlo se agrega al final de la lista y se retorna true. (recibe una const T& ya que
no queremos que sea editado y que no sea una copia) este metodo retorna un bool.
3. esVecinoDe: Reecibe una referencia constante a el codigo de un territorio y revisa 
si es vecino de este, primero revisa si ese codigo no es nulo o es de el mismo territorio,
en caso de que lo sea retorna false, en caso de que no revisa la lista de vecinos, si 
esta adentro de la lista retorna true, y si no retorna false.
4. asignarOcupante: Recibe una referencia constante a el color de el nuevo ocupante y la 
cantidad de ejercitos.
5. modificarEjercitos: Recibe un entero que es la cantidad de ejercitos a modificar.
===========================================================================================
*/

Territorio::Territorio() 
    : codigo(""), nombre(""), continenteId(0), colorOcupante(""), numEjercitos(0) {}

Territorio::Territorio(const std::string& cod, const std::string& nom, int contId)
    : codigo(cod), nombre(nom), continenteId(contId), colorOcupante(""), numEjercitos(0) {}

bool Territorio::agregarVecino(Territorio* vecinoPtr) {
    if (vecinoPtr == NULL || vecinoPtr == this) {
        return false;
    }

    // Verificar si ya fue agregado previamente para evitar duplicados
    for (int i = 0; i < vecinos.longitud(); i++) {
        if (vecinos.obtener(i)->codigo == vecinoPtr->codigo) {
            return false;
        }
    }

    vecinos.agregarAlFinal(vecinoPtr);
    return true;
}

bool Territorio::esVecinoDe(const std::string& codigoDestino) const {
    if (codigoDestino == "" || codigoDestino==this->codigo) {
        return false;
    }
    for (int i = 0; i < vecinos.longitud(); i++) {
        if (vecinos.obtener(i)->codigo == codigoDestino) {
            return true;
        }
    }
    return false;
}

void Territorio::asignarOcupante(const std::string& nuevoColor, int cantidad) {
    colorOcupante = nuevoColor;
    numEjercitos = cantidad;
}

void Territorio::modificarEjercitos(int delta) {
    numEjercitos += delta;
}
