#include "TableroRisk.h"

/*
================================================================================

================================================================================
*/

TableroRisk::TableroRisk() {
  // El mapa se construye al invocar inicializarMapa()
}

TableroRisk::~TableroRisk() { limpiar(); }

void TableroRisk::limpiar() {
  territorios.limpiar();
  continentes.limpiar();
}

void TableroRisk::inicializarMapa() {
  limpiar();

  // -------------------------------------------------------------------------
  // 1. Inicialización de los 6 Continentes con sus Bonos Reglamentarios
  // -------------------------------------------------------------------------
  continentes.agregarAlFinal(Continente(1, "America del Norte", 5));
  continentes.agregarAlFinal(Continente(2, "America del Sur", 2));
  continentes.agregarAlFinal(Continente(3, "Europa", 5));
  continentes.agregarAlFinal(Continente(4, "Africa", 3));
  continentes.agregarAlFinal(Continente(5, "Asia", 7));
  continentes.agregarAlFinal(Continente(6, "Australia", 2));

  // -------------------------------------------------------------------------
  // 2. Creación de los 42 Territorios del Tablero (Cuadro 1 del Enunciado)
  // -------------------------------------------------------------------------
  // América del Norte (1.1 - 1.9)
  territorios.agregarAlFinal(Territorio("1.1", "Alaska", 1));
  territorios.agregarAlFinal(Territorio("1.2", "Alberta", 1));
  territorios.agregarAlFinal(Territorio("1.3", "America Central", 1));
  territorios.agregarAlFinal(Territorio("1.4", "Estados Unidos Orientales", 1));
  territorios.agregarAlFinal(Territorio("1.5", "Groenlandia", 1));
  territorios.agregarAlFinal(Territorio("1.6", "Territorio Noroccidental", 1));
  territorios.agregarAlFinal(Territorio("1.7", "Ontario", 1));
  territorios.agregarAlFinal(Territorio("1.8", "Quebec", 1));
  territorios.agregarAlFinal(
      Territorio("1.9", "Estados Unidos Occidentales", 1));

  // América del Sur (2.1 - 2.4)
  territorios.agregarAlFinal(Territorio("2.1", "Argentina", 2));
  territorios.agregarAlFinal(Territorio("2.2", "Brasil", 2));
  territorios.agregarAlFinal(Territorio("2.3", "Peru", 2));
  territorios.agregarAlFinal(Territorio("2.4", "Venezuela", 2));

  // Europa (3.1 - 3.7)
  territorios.agregarAlFinal(Territorio("3.1", "Gran Bretana", 3));
  territorios.agregarAlFinal(Territorio("3.2", "Islandia", 3));
  territorios.agregarAlFinal(Territorio("3.3", "Europa del Norte", 3));
  territorios.agregarAlFinal(Territorio("3.4", "Escandinavia", 3));
  territorios.agregarAlFinal(Territorio("3.5", "Europa del Sur", 3));
  territorios.agregarAlFinal(Territorio("3.6", "Ucrania", 3));
  territorios.agregarAlFinal(Territorio("3.7", "Europa Occidental", 3));

  // África (4.1 - 4.6)
  territorios.agregarAlFinal(Territorio("4.1", "Congo", 4));
  territorios.agregarAlFinal(Territorio("4.2", "Africa Oriental", 4));
  territorios.agregarAlFinal(Territorio("4.3", "Egipto", 4));
  territorios.agregarAlFinal(Territorio("4.4", "Madagascar", 4));
  territorios.agregarAlFinal(Territorio("4.5", "Africa del Norte", 4));
  territorios.agregarAlFinal(Territorio("4.6", "Africa del Sur", 4));

  // Asia (5.1 - 5.12)
  territorios.agregarAlFinal(Territorio("5.1", "Afghanistan", 5));
  territorios.agregarAlFinal(Territorio("5.2", "China", 5));
  territorios.agregarAlFinal(Territorio("5.3", "India", 5));
  territorios.agregarAlFinal(Territorio("5.4", "Irkutsk", 5));
  territorios.agregarAlFinal(Territorio("5.5", "Japon", 5));
  territorios.agregarAlFinal(Territorio("5.6", "Kamchatka", 5));
  territorios.agregarAlFinal(Territorio("5.7", "Medio Oriente", 5));
  territorios.agregarAlFinal(Territorio("5.8", "Mongolia", 5));
  territorios.agregarAlFinal(Territorio("5.9", "Siam", 5));
  territorios.agregarAlFinal(Territorio("5.10", "Siberia", 5));
  territorios.agregarAlFinal(Territorio("5.11", "Ural", 5));
  territorios.agregarAlFinal(Territorio("5.12", "Yakutsk", 5));

  // Australia (6.1 - 6.4)
  territorios.agregarAlFinal(Territorio("6.1", "Australia Oriental", 6));
  territorios.agregarAlFinal(Territorio("6.2", "Indonesia", 6));
  territorios.agregarAlFinal(Territorio("6.3", "Nueva Guinea", 6));
  territorios.agregarAlFinal(Territorio("6.4", "Australia Occidental", 6));

  // -------------------------------------------------------------------------
  // 3. Vincular Referencias de Territorios a sus Continentes Respectivos
  // -------------------------------------------------------------------------
  for (int i = 0; i < territorios.longitud(); i++) {
    Territorio *tPtr = &(territorios.obtener(i));
    int contId = tPtr->continenteId;
    continentes.obtener(contId - 1).agregarTerritorio(tPtr);
  }

  // -------------------------------------------------------------------------
  // 4. Configurar Adyacencias Oficiales del Tablero (Grafo no dirigido)
  // -------------------------------------------------------------------------
  // América del Norte
  conectarBidireccional("1.1", "1.2");
  conectarBidireccional("1.1", "1.6");
  conectarBidireccional("1.1", "5.6"); // Intercontinental: Alaska <-> Kamchatka
  conectarBidireccional("1.2", "1.6");
  conectarBidireccional("1.2", "1.7");
  conectarBidireccional("1.2", "1.9");
  conectarBidireccional("1.3", "1.4");
  conectarBidireccional("1.3", "1.9");
  conectarBidireccional(
      "1.3", "2.4"); // Intercontinental: América Central <-> Venezuela
  conectarBidireccional("1.4", "1.7");
  conectarBidireccional("1.4", "1.8");
  conectarBidireccional("1.4", "1.9");
  conectarBidireccional("1.5", "1.6");
  conectarBidireccional("1.5", "1.7");
  conectarBidireccional("1.5", "1.8");
  conectarBidireccional("1.5",
                        "3.2"); // Intercontinental: Groenlandia <-> Islandia
  conectarBidireccional("1.6", "1.7");
  conectarBidireccional("1.7", "1.8");
  conectarBidireccional("1.7", "1.9");

  // América del Sur
  conectarBidireccional("2.1", "2.2");
  conectarBidireccional("2.1", "2.3");
  conectarBidireccional("2.2", "2.3");
  conectarBidireccional("2.2", "2.4");
  conectarBidireccional("2.2",
                        "4.5"); // Intercontinental: Brasil <-> África del Norte
  conectarBidireccional("2.3", "2.4");

  // Europa
  conectarBidireccional("3.1", "3.2");
  conectarBidireccional("3.1", "3.3");
  conectarBidireccional("3.1", "3.4");
  conectarBidireccional("3.1", "3.7");
  conectarBidireccional("3.2", "3.4");
  conectarBidireccional("3.3", "3.4");
  conectarBidireccional("3.3", "3.5");
  conectarBidireccional("3.3", "3.6");
  conectarBidireccional("3.3", "3.7");
  conectarBidireccional("3.4", "3.6");
  conectarBidireccional("3.5", "3.6");
  conectarBidireccional("3.5", "3.7");
  conectarBidireccional("3.5",
                        "4.3"); // Intercontinental: Europa del Sur <-> Egipto
  conectarBidireccional(
      "3.5", "4.5"); // Intercontinental: Europa del Sur <-> África del Norte
  conectarBidireccional(
      "3.5", "5.7"); // Intercontinental: Europa del Sur <-> Medio Oriente
  conectarBidireccional("3.6",
                        "5.1"); // Intercontinental: Ucrania <-> Afghanistán
  conectarBidireccional("3.6",
                        "5.7"); // Intercontinental: Ucrania <-> Medio Oriente
  conectarBidireccional("3.6", "5.11"); // Intercontinental: Ucrania <-> Ural
  conectarBidireccional(
      "3.7", "4.5"); // Intercontinental: Europa Occidental <-> África del Norte

  // África
  conectarBidireccional("4.1", "4.2");
  conectarBidireccional("4.1", "4.5");
  conectarBidireccional("4.1", "4.6");
  conectarBidireccional("4.2", "4.3");
  conectarBidireccional("4.2", "4.4");
  conectarBidireccional("4.2", "4.5");
  conectarBidireccional("4.2", "4.6");
  conectarBidireccional(
      "4.2", "5.7"); // Intercontinental: África Oriental <-> Medio Oriente
  conectarBidireccional("4.3", "4.5");
  conectarBidireccional("4.3",
                        "5.7"); // Intercontinental: Egipto <-> Medio Oriente
  conectarBidireccional("4.4", "4.6");

  // Asia
  conectarBidireccional("5.1", "5.2");
  conectarBidireccional("5.1", "5.3");
  conectarBidireccional("5.1", "5.7");
  conectarBidireccional("5.1", "5.11");
  conectarBidireccional("5.2", "5.3");
  conectarBidireccional("5.2", "5.8");
  conectarBidireccional("5.2", "5.9");
  conectarBidireccional("5.2", "5.10");
  conectarBidireccional("5.2", "5.11");
  conectarBidireccional("5.3", "5.7");
  conectarBidireccional("5.3", "5.9");
  conectarBidireccional("5.4", "5.6");
  conectarBidireccional("5.4", "5.8");
  conectarBidireccional("5.4", "5.10");
  conectarBidireccional("5.4", "5.12");
  conectarBidireccional("5.5", "5.6");
  conectarBidireccional("5.5", "5.8");
  conectarBidireccional("5.6", "5.8");
  conectarBidireccional("5.6", "5.12");
  conectarBidireccional("5.8", "5.10");
  conectarBidireccional("5.9", "6.2"); // Intercontinental: Siam <-> Indonesia
  conectarBidireccional("5.10", "5.11");
  conectarBidireccional("5.10", "5.12");

  // Australia
  conectarBidireccional("6.1", "6.3");
  conectarBidireccional("6.1", "6.4");
  conectarBidireccional("6.2", "6.3");
  conectarBidireccional("6.2", "6.4");
  conectarBidireccional("6.3", "6.4");
}

void TableroRisk::conectarBidireccional(const std::string &cod1,
                                        const std::string &cod2) {
  Territorio *t1 = buscarTerritorio(cod1);
  Territorio *t2 = buscarTerritorio(cod2);
  if (t1 != NULL && t2 != NULL) {
    t1->agregarVecino(t2);
    t2->agregarVecino(t1);
  }
}

Territorio *TableroRisk::buscarTerritorio(const std::string &codigo) {
  for (int i = 0; i < territorios.longitud(); i++) {
    if (territorios.obtener(i).codigo == codigo) {
      return &(territorios.obtener(i));
    }
  }
  return NULL;
}

const Territorio *
TableroRisk::buscarTerritorio(const std::string &codigo) const {
  for (int i = 0; i < territorios.longitud(); i++) {
    if (territorios.obtener(i).codigo == codigo) {
      return &(territorios.obtener(i));
    }
  }
  return NULL;
}

int TableroRisk::contarTerritoriosDe(const std::string &colorJugador) const {
  int total = 0;
  for (int i = 0; i < territorios.longitud(); i++) {
    if (territorios.obtener(i).colorOcupante == colorJugador) {
      total++;
    }
  }
  return total;
}

int TableroRisk::calcularBonoContinentes(
    const std::string &colorJugador) const {
  int bonoTotal = 0;
  for (int i = 0; i < continentes.longitud(); i++) {
    if (continentes.obtener(i).esControladoTotalmentePor(colorJugador)) {
      bonoTotal += continentes.obtener(i).bonoUnidades;
    }
  }
  return bonoTotal;
}
