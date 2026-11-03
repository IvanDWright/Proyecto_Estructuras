#include "TableroRisk.h"

// El mapa se construye al invocar configurarMapaBase()
TableroRisk::TableroRisk() {}

// Destructor que limpia los nodos
TableroRisk::~TableroRisk() { vaciarTablero(); }

// Intercambia el contenido con otro tablero; Recibe: TableroRisk& otro; Retorna: void
// [IA] Generada con Claude (Prompt 15, Apendice A).
void TableroRisk::intercambiar(TableroRisk &otro) {
  listaTerritoriosGlob.intercambiar(otro.listaTerritoriosGlob);
  listaContinentesGlob.intercambiar(otro.listaContinentesGlob);
}

// Limpia y libera las colecciones
void TableroRisk::vaciarTablero() {
  listaTerritoriosGlob.limpiar();
  listaContinentesGlob.limpiar();
}

// Inicializa completamente los 42 territorios, los 6 continentes y todas las
// fronteras oficiales, armando el grafo del mapa.
void TableroRisk::configurarMapaBase() {
  vaciarTablero();

  // -------------------------------------------------------------------------
  // 1. Inicialización de los 6 Continentes con sus Bonos Reglamentarios
  // -------------------------------------------------------------------------
  listaContinentesGlob.agregarAlFinal(Continente(1, "America del Norte", 5));
  listaContinentesGlob.agregarAlFinal(Continente(2, "America del Sur", 2));
  listaContinentesGlob.agregarAlFinal(Continente(3, "Europa", 5));
  listaContinentesGlob.agregarAlFinal(Continente(4, "Africa", 3));
  listaContinentesGlob.agregarAlFinal(Continente(5, "Asia", 7));
  listaContinentesGlob.agregarAlFinal(Continente(6, "Australia", 2));

  // -------------------------------------------------------------------------
  // 2. Creación de los 42 Territorios del Tablero (Cuadro 1 del Enunciado)
  // -------------------------------------------------------------------------
  // América del Norte (1.1 - 1.9)
  listaTerritoriosGlob.agregarAlFinal(Territorio("1.1", "Alaska", 1));
  listaTerritoriosGlob.agregarAlFinal(Territorio("1.2", "Alberta", 1));
  listaTerritoriosGlob.agregarAlFinal(Territorio("1.3", "America Central", 1));
  listaTerritoriosGlob.agregarAlFinal(Territorio("1.4", "Estados Unidos Orientales", 1));
  listaTerritoriosGlob.agregarAlFinal(Territorio("1.5", "Groenlandia", 1));
  listaTerritoriosGlob.agregarAlFinal(Territorio("1.6", "Territorio Noroccidental", 1));
  listaTerritoriosGlob.agregarAlFinal(Territorio("1.7", "Ontario", 1));
  listaTerritoriosGlob.agregarAlFinal(Territorio("1.8", "Quebec", 1));
  listaTerritoriosGlob.agregarAlFinal(Territorio("1.9", "Estados Unidos Occidentales", 1));

  // América del Sur (2.1 - 2.4)
  listaTerritoriosGlob.agregarAlFinal(Territorio("2.1", "Argentina", 2));
  listaTerritoriosGlob.agregarAlFinal(Territorio("2.2", "Brasil", 2));
  listaTerritoriosGlob.agregarAlFinal(Territorio("2.3", "Peru", 2));
  listaTerritoriosGlob.agregarAlFinal(Territorio("2.4", "Venezuela", 2));

  // Europa (3.1 - 3.7)
  listaTerritoriosGlob.agregarAlFinal(Territorio("3.1", "Gran Bretana", 3));
  listaTerritoriosGlob.agregarAlFinal(Territorio("3.2", "Islandia", 3));
  listaTerritoriosGlob.agregarAlFinal(Territorio("3.3", "Europa del Norte", 3));
  listaTerritoriosGlob.agregarAlFinal(Territorio("3.4", "Escandinavia", 3));
  listaTerritoriosGlob.agregarAlFinal(Territorio("3.5", "Europa del Sur", 3));
  listaTerritoriosGlob.agregarAlFinal(Territorio("3.6", "Ucrania", 3));
  listaTerritoriosGlob.agregarAlFinal(Territorio("3.7", "Europa Occidental", 3));

  // África (4.1 - 4.6)
  listaTerritoriosGlob.agregarAlFinal(Territorio("4.1", "Congo", 4));
  listaTerritoriosGlob.agregarAlFinal(Territorio("4.2", "Africa Oriental", 4));
  listaTerritoriosGlob.agregarAlFinal(Territorio("4.3", "Egipto", 4));
  listaTerritoriosGlob.agregarAlFinal(Territorio("4.4", "Madagascar", 4));
  listaTerritoriosGlob.agregarAlFinal(Territorio("4.5", "Africa del Norte", 4));
  listaTerritoriosGlob.agregarAlFinal(Territorio("4.6", "Africa del Sur", 4));

  // Asia (5.1 - 5.12)
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.1", "Afghanistan", 5));
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.2", "China", 5));
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.3", "India", 5));
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.4", "Irkutsk", 5));
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.5", "Japon", 5));
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.6", "Kamchatka", 5));
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.7", "Medio Oriente", 5));
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.8", "Mongolia", 5));
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.9", "Siam", 5));
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.10", "Siberia", 5));
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.11", "Ural", 5));
  listaTerritoriosGlob.agregarAlFinal(Territorio("5.12", "Yakutsk", 5));

  // Australia (6.1 - 6.4)
  listaTerritoriosGlob.agregarAlFinal(Territorio("6.1", "Australia Oriental", 6));
  listaTerritoriosGlob.agregarAlFinal(Territorio("6.2", "Indonesia", 6));
  listaTerritoriosGlob.agregarAlFinal(Territorio("6.3", "Nueva Guinea", 6));
  listaTerritoriosGlob.agregarAlFinal(Territorio("6.4", "Australia Occidental", 6));

  // -------------------------------------------------------------------------
  // 3. Vincular Referencias de Territorios a sus Continentes Respectivos
  // -------------------------------------------------------------------------
  for (int i = 0; i < listaTerritoriosGlob.longitud(); i++) {
    Territorio *tPtr = &(listaTerritoriosGlob.obtener(i));
    int contId = tPtr->idContinente;
    listaContinentesGlob.obtener(contId - 1).agregarTerritorio(tPtr);
  }

  // -------------------------------------------------------------------------
  // 4. Configurar Adyacencias Oficiales del Tablero (Grafo no dirigido)
  // -------------------------------------------------------------------------
  // América del Norte
  establecerFrontera("1.1", "1.2");
  establecerFrontera("1.1", "1.6");
  establecerFrontera("1.1", "5.6"); // Intercontinental: Alaska <-> Kamchatka
  establecerFrontera("1.2", "1.6");
  establecerFrontera("1.2", "1.7");
  establecerFrontera("1.2", "1.9");
  establecerFrontera("1.3", "1.4");
  establecerFrontera("1.3", "1.9");
  establecerFrontera("1.3", "2.4"); // Intercontinental: América Central <-> Venezuela
  establecerFrontera("1.4", "1.7");
  establecerFrontera("1.4", "1.8");
  establecerFrontera("1.4", "1.9");
  establecerFrontera("1.5", "1.6");
  establecerFrontera("1.5", "1.7");
  establecerFrontera("1.5", "1.8");
  establecerFrontera("1.5", "3.2"); // Intercontinental: Groenlandia <-> Islandia
  establecerFrontera("1.6", "1.7");
  establecerFrontera("1.7", "1.8");
  establecerFrontera("1.7", "1.9");

  // América del Sur
  establecerFrontera("2.1", "2.2");
  establecerFrontera("2.1", "2.3");
  establecerFrontera("2.2", "2.3");
  establecerFrontera("2.2", "2.4");
  establecerFrontera("2.2", "4.5"); // Intercontinental: Brasil <-> África del Norte
  establecerFrontera("2.3", "2.4");

  // Europa
  establecerFrontera("3.1", "3.2");
  establecerFrontera("3.1", "3.3");
  establecerFrontera("3.1", "3.4");
  establecerFrontera("3.1", "3.7");
  establecerFrontera("3.2", "3.4");
  establecerFrontera("3.3", "3.4");
  establecerFrontera("3.3", "3.5");
  establecerFrontera("3.3", "3.6");
  establecerFrontera("3.3", "3.7");
  establecerFrontera("3.4", "3.6");
  establecerFrontera("3.5", "3.6");
  establecerFrontera("3.5", "3.7");
  establecerFrontera("3.5", "4.3"); // Intercontinental: Europa del Sur <-> Egipto
  establecerFrontera("3.5", "4.5"); // Intercontinental: Europa del Sur <-> África del Norte
  establecerFrontera("3.5", "5.7"); // Intercontinental: Europa del Sur <-> Medio Oriente
  establecerFrontera("3.6", "5.1"); // Intercontinental: Ucrania <-> Afghanistán
  establecerFrontera("3.6", "5.7"); // Intercontinental: Ucrania <-> Medio Oriente
  establecerFrontera("3.6", "5.11"); // Intercontinental: Ucrania <-> Ural
  establecerFrontera("3.7", "4.5"); // Intercontinental: Europa Occidental <-> África del Norte

  // África
  establecerFrontera("4.1", "4.2");
  establecerFrontera("4.1", "4.5");
  establecerFrontera("4.1", "4.6");
  establecerFrontera("4.2", "4.3");
  establecerFrontera("4.2", "4.4");
  establecerFrontera("4.2", "4.5");
  establecerFrontera("4.2", "4.6");
  establecerFrontera("4.2", "5.7"); // Intercontinental: África Oriental <-> Medio Oriente
  establecerFrontera("4.3", "4.5");
  establecerFrontera("4.3", "5.7"); // Intercontinental: Egipto <-> Medio Oriente
  establecerFrontera("4.4", "4.6");

  // Asia
  establecerFrontera("5.1", "5.2");
  establecerFrontera("5.1", "5.3");
  establecerFrontera("5.1", "5.7");
  establecerFrontera("5.1", "5.11");
  establecerFrontera("5.2", "5.3");
  establecerFrontera("5.2", "5.8");
  establecerFrontera("5.2", "5.9");
  establecerFrontera("5.2", "5.10");
  establecerFrontera("5.2", "5.11");
  establecerFrontera("5.3", "5.7");
  establecerFrontera("5.3", "5.9");
  establecerFrontera("5.4", "5.6");
  establecerFrontera("5.4", "5.8");
  establecerFrontera("5.4", "5.10");
  establecerFrontera("5.4", "5.12");
  establecerFrontera("5.5", "5.6");
  establecerFrontera("5.5", "5.8");
  establecerFrontera("5.6", "5.8");
  establecerFrontera("5.6", "5.12");
  establecerFrontera("5.8", "5.10");
  establecerFrontera("5.9", "6.2"); // Intercontinental: Siam <-> Indonesia
  establecerFrontera("5.10", "5.11");
  establecerFrontera("5.10", "5.12");

  // Australia
  establecerFrontera("6.1", "6.3");
  establecerFrontera("6.1", "6.4");
  establecerFrontera("6.2", "6.3");
  establecerFrontera("6.2", "6.4");
  establecerFrontera("6.3", "6.4");
}

// Crea una conexión de ida y vuelta entre dos territorios; Recibe: const string& cod1, const string& cod2; Retorna: void
// Busca ambos territorios por su código y, si ambos existen, se agregan mutuamente a sus listas de adyacentes.
void TableroRisk::establecerFrontera(const std::string &cod1,
                                     const std::string &cod2) {
  Territorio *t1 = obtenerTerritorioPorId(cod1);
  Territorio *t2 = obtenerTerritorioPorId(cod2);
  if (t1 != NULL && t2 != NULL) {
    t1->agregarVecino(t2);
    t2->agregarVecino(t1);
  }
}

// Busca un territorio por su código oficial; Recibe: const string& idBuscado; Retorna: Territorio*
// Recorre la lista de territorios y retorna el puntero si coincide el código; si no existe retorna NULL.
Territorio *TableroRisk::obtenerTerritorioPorId(const std::string &idBuscado) {
  for (int i = 0; i < listaTerritoriosGlob.longitud(); i++) {
    if (listaTerritoriosGlob.obtener(i).idTerritorio == idBuscado) {
      return &(listaTerritoriosGlob.obtener(i));
    }
  }
  return NULL;
}

// Busca un territorio por su código oficial (versión constante); Recibe: const string& idBuscado; Retorna: const Territorio*
// Igual que el anterior pero retorna un puntero constante no editable.
const Territorio *
TableroRisk::obtenerTerritorioPorId(const std::string &idBuscado) const {
  for (int i = 0; i < listaTerritoriosGlob.longitud(); i++) {
    if (listaTerritoriosGlob.obtener(i).idTerritorio == idBuscado) {
      return &(listaTerritoriosGlob.obtener(i));
    }
  }
  return NULL;
}

// Cuenta territorios controlados por un jugador; Recibe: const string& colorJugador; Retorna: int
// Recorre los 42 territorios y cuenta aquellos cuyo color coincide con el recibido.
int TableroRisk::cantidadTerritoriosOcupadosPor(
    const std::string &colorJugador) const {
  int total = 0;
  for (int i = 0; i < listaTerritoriosGlob.longitud(); i++) {
    if (listaTerritoriosGlob.obtener(i).colorPropietario == colorJugador) {
      total++;
    }
  }
  return total;
}

// Calcula el bono de continentes dominados al 100%; Recibe: const string& colorJugador; Retorna: int
// Recorre los continentes y suma el bono de aquellos controlados totalmente por el jugador.
int TableroRisk::obtenerBonoPorContinentesDominados(
    const std::string &colorJugador) const {
  int bonoTotal = 0;
  for (int i = 0; i < listaContinentesGlob.longitud(); i++) {
    if (listaContinentesGlob.obtener(i).esControladoTotalmentePor(colorJugador)) {
      bonoTotal += listaContinentesGlob.obtener(i).tropasAdicionales;
    }
  }
  return bonoTotal;
}

// Obtiene todos los territorios que ocupa un jugador; Recibe: const string& colorJugador, vector<Territorio*>& resultado; Retorna: void
// Limpia el vector resultado y lo llena con los punteros a los territorios ocupados por el color recibido.
void TableroRisk::territoriosDeJugador(const std::string &colorJugador,
                                       std::vector<Territorio *> &resultado) {
  resultado.clear();
  for (int i = 0; i < listaTerritoriosGlob.longitud(); i++) {
    Territorio *t = &(listaTerritoriosGlob.obtener(i));
    if (t->colorPropietario == colorJugador) {
      resultado.push_back(t);
    }
  }
}

// Territorios desde los que se puede atacar; Recibe: const string& colorJugador, vector<Territorio*>& resultado; Retorna: void
// Filtra los territorios del jugador con mas de 1 tropa y al menos un vecino enemigo.
void TableroRisk::territoriosAptosParaAtacar(const std::string &colorJugador,
                                             std::vector<Territorio *> &resultado) {
  std::vector<Territorio *> propios;
  territoriosDeJugador(colorJugador, propios);
  resultado.clear();
  for (size_t i = 0; i < propios.size(); i++) {
    if (propios[i]->cantidadTropas > 1 && propios[i]->tieneVecinoEnemigo(colorJugador)) {
      resultado.push_back(propios[i]);
    }
  }
}

// Territorios desde los que se puede fortificar; Recibe: const string& colorJugador, vector<Territorio*>& resultado; Retorna: void
// Filtra los territorios del jugador con mas de 1 tropa y al menos un vecino propio.
void TableroRisk::territoriosAptosParaFortificar(const std::string &colorJugador,
                                                 std::vector<Territorio *> &resultado) {
  std::vector<Territorio *> propios;
  territoriosDeJugador(colorJugador, propios);
  resultado.clear();
  for (size_t i = 0; i < propios.size(); i++) {
    if (propios[i]->cantidadTropas > 1 && propios[i]->tieneVecinoPropio(colorJugador)) {
      resultado.push_back(propios[i]);
    }
  }
}
