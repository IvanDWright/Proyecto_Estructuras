#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// Inclusiones de los TADs del proyecto (Rutas relativas a la carpeta src)
#include "../tads/Carta.h"
#include "../tads/Continente.h"
#include "../tads/JuegoRisk.h"
#include "../tads/Jugador.h"
#include "../tads/ListaLineal.h"
#include "../tads/TableroRisk.h"
#include "../tads/Territorio.h"

using namespace std;

// Estructura para almacenar información de ayuda de comandos
struct ComandoAyuda {
  string uso;
  string descripcion;
};

// Diccionario de comandos para el comando 'ayuda'
map<string, ComandoAyuda> comandos = {
    {"inicializar",
     {"inicializar <archivo>", "Inicializa el juego con los datos de un "
                               "archivo de texto plano o binario."}},
    {"obtener_unidades",
     {"obtener_unidades <nombre_jugador>",
      "Calcula y asigna unidades adicionales al jugador."}},
    {"atacar",
     {"atacar <nombre_jugador>",
      "Inicia la fase de ataque para el jugador especificado."}},
    {"fortificar",
     {"fortificar <nombre_jugador>",
      "Inicia la fase de fortificacion para el jugador especificado."}},
    {"estado_juego", {"estado_juego", "Muestra el estado actual del juego."}},
    {"salir", {"salir", "Termina la ejecucion de la aplicacion."}},
    {"guardar",
     {"guardar <nombre_archivo>",
      "Guarda el estado del juego en un archivo de texto plano."}},
    {"guardar_comprimido",
     {"guardar_comprimido <nombre_archivo>",
      "Guarda el estado del juego en un archivo binario comprimido "
      "(Huffman)."}},
    {"costo_conquista",
     {"costo_conquista <nombre_jugador> <territorio>",
      "Calcula el costo y secuencia de territorios para conquistar el "
      "territorio indicado."}},
    {"conquista_mas_barata",
     {"conquista_mas_barata <nombre_jugador>",
      "Calcula y sugiere la conquista que implica la menor perdida de "
      "ejercitos."}},
    {"ayuda",
     {"ayuda [comando]", "Muestra la lista de comandos disponibles o detalla "
                         "el uso de un comando especifico."}}};

// Función auxiliar para imprimir la lista general de comandos
void mostrar_ayuda() {
  cout << "Lista de comandos disponibles:\n";
  for (const auto &par : comandos) {
    cout << " - " << par.first << "\n";
  }
}

int calcular_tropas(int a) {
  int total_tropas = 0;
  total_tropas += a / 3;
  return total_tropas;
}

// Función auxiliar para imprimir el detalle de un comando
void mostrar_ayuda_comando(const string &cmd) {
  if (comandos.find(cmd) != comandos.end()) {
    cout << "Uso: " << comandos[cmd].uso << "\n";
    cout << "Descripcion: " << comandos[cmd].descripcion << "\n";
  } else {
    cout << "El comando '" << cmd
         << "' no existe. Escriba 'ayuda' para ver los comandos disponibles.\n";
  }
}

/*
======================================================================================
Logica de ataque, se definen la cantidad de dados a utilizar segun la cantidad
de tropas. Se lanza un dado por cada dado a utilizar, se comparan los resultados
de mayor a menor. Si el dado del atacante es mayor al del defensor, el defensor
pierde 1 tropa. Si el dado del defensor es mayor o igual al del atacante, el
atacante pierde 1 tropa.
======================================================================================
*/
bool atacar(int &tropas_atacantes, int &tropas_defensores) {
  int dados_atacante = min(3, tropas_atacantes - 1);
  int dados_defensor = min(2, tropas_defensores);

  if (dados_atacante <= 0) {
    cout << "El atacante no tiene suficientes tropas para atacar.\n";
    return false;
  }

  vector<int> tiradas_atacante(dados_atacante);
  vector<int> tiradas_defensor(dados_defensor);

  cout << "\n[!] Lanzamiento de dados...\n";
  cout << "Atacante tira " << dados_atacante << " dado(s):\n";
  for (int i = 0; i < dados_atacante; ++i) {
    tiradas_atacante[i] = rand() % 6 + 1;
    cout << "  - Dado " << i + 1 << ": " << tiradas_atacante[i] << "\n";
  }

  cout << "Defensor tira " << dados_defensor << " dado(s):\n";
  for (int i = 0; i < dados_defensor; ++i) {
    tiradas_defensor[i] = rand() % 6 + 1;
    cout << "  - Dado " << i + 1 << ": " << tiradas_defensor[i] << "\n";
  }

  sort(tiradas_atacante.rbegin(), tiradas_atacante.rend());
  sort(tiradas_defensor.rbegin(), tiradas_defensor.rend());

  int duelos = min(dados_atacante, dados_defensor);
  cout << "\n--- Resultados de los duelos ---\n";

  for (int i = 0; i < duelos; ++i) {
    cout << "Duelo " << i + 1 << ": Atacante (" << tiradas_atacante[i]
         << ") vs Defensor (" << tiradas_defensor[i] << ") -> ";
    if (tiradas_atacante[i] > tiradas_defensor[i]) {
      tropas_defensores--;
      cout << "El defensor pierde 1 tropa.\n";
    } else {
      tropas_atacantes--;
      cout << "El atacante pierde 1 tropa.\n";
    }
  }
  cout << "--------------------------------\n";
  cout << "Tropas restantes - Atacante: " << tropas_atacantes
       << " | Defensor: " << tropas_defensores << "\n\n";

  if (tropas_defensores == 0) {
    cout << "¡El atacante ha conquistado el territorio!\n";
  } else {
    cout << "El defensor mantiene el territorio por ahora.\n";
  }

  return tropas_defensores == 0;
}

int main() {
  string linea;
  bool is_running = true;
  bool is_initialized = false;
  JuegoRisk juego;

  // Ciclo de evaluación de la consola interactiva (REPL)
  while (is_running) {
    // Imprime estrictamente el caracter $ como indicador de linea
    cout << "$ ";
    if (!getline(cin, linea)) {
      break; // Finalizar si ocurre EOF
    }

    if (linea.empty())
      continue;

    // Tokenización básica por espacios
    stringstream ss(linea);
    string comando;
    vector<string> args;

    ss >> comando;
    string arg;
    while (ss >> arg) {
      args.push_back(arg);
    }

    // ==========================================
    // COMANDO: inicializar
    // ==========================================
    if (comando == "inicializar") {
      if (args.size() == 1) {
        if (is_initialized) {
          cout << "(Juego en curso) El juego ya ha sido inicializado.\n";
        } else {
          ifstream archivo(args[0], ios::in);
          if (!archivo.is_open()) {
            cout << "(Archivo erróneo) " << args[0]
                 << " no se encuentra o no puede leerse.\n";
          } else if (archivo.peek() == ifstream::traits_type::eof()) {
            cout << "(Archivo vacío) " << args[0]
                 << " no contiene información.\n";
            archivo.close();
          } else {
            bool formatoCorrecto = true;
            int numJugadores;

            // 1. Leer número de jugadores
            if (!(archivo >> numJugadores) || numJugadores < 3 ||
                numJugadores > 6) {
              formatoCorrecto = false;
            }

            juego.jugadores.limpiar();
            juego.tablero.limpiar();
            juego.estado = NO_INICIALIZADO;

            map<string, int> tropasPorColor;

            // 2. Leer jugadores
            if (formatoCorrecto) {
              for (int i = 0; i < numJugadores; ++i) {
                string nombre, color;
                if (!(archivo >> nombre >> color)) {
                  formatoCorrecto = false;
                  break;
                }
                juego.jugadores.agregarAlFinal(Jugador(nombre, color));
                tropasPorColor[color] = 0;
              }
            }

            // 3. Inicializar mapa y 4. Asignar territorios
            if (formatoCorrecto) {
              juego.tablero.inicializarMapa();
              for (int i = 0; i < 42; ++i) {
                string codTer, colorOcupante;
                int numTropas;
                if (!(archivo >> codTer >> colorOcupante >> numTropas)) {
                  formatoCorrecto = false;
                  break;
                }

                if (tropasPorColor.find(colorOcupante) ==
                    tropasPorColor.end()) {
                  formatoCorrecto =
                      false; // Color no pertenece a un jugador válido
                  break;
                }

                Territorio *ter = juego.tablero.buscarTerritorio(codTer);
                if (ter == nullptr) {
                  formatoCorrecto = false; // Territorio no existe
                  break;
                }

                ter->asignarOcupante(colorOcupante, numTropas);
                tropasPorColor[colorOcupante] += numTropas;
              }
            }

            // 5. Validar reglas de Risk (Suma de tropas)
            if (formatoCorrecto) {
              int tropasEsperadas = 0;
              if (numJugadores == 3)
                tropasEsperadas = 35;
              else if (numJugadores == 4)
                tropasEsperadas = 30;
              else if (numJugadores == 5)
                tropasEsperadas = 25;
              else if (numJugadores == 6)
                tropasEsperadas = 20;

              for (const auto &par : tropasPorColor) {
                if (par.second != tropasEsperadas) {
                  formatoCorrecto = false;
                  break;
                }
              }
            }

            archivo.close();

            if (formatoCorrecto) {
              is_initialized = true;
              juego.estado = EN_CURSO;
              juego.indiceJugadorActual = 0;
              cout << "(Comando correcto) El juego se ha inicializado "
                      "correctamente.\n";
            } else {
              juego.jugadores.limpiar();
              juego.tablero.limpiar();
              cout << "(Archivo sin formato) " << args[0]
                   << " no contiene información en el formato esperado.\n";
            }
          }
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: obtener_unidades
    // ==========================================
    /*
    =====================================================================================
    Esta opcion permite anadir una cantidad de unidades a un territorio.
    Valida multiples cosas antes de hacerlo, despues menciona tus territorios
    mientras los guarda en un vector, elijes donde y cuantas tropas poner en tu
    territorio, y si te quedan tropas por poner vuelve a preguntar.
    =======================================================================================
    */
    else if (comando == "obtener_unidades") {
      // Valida cantidad de parámetros
      if (args.size() == 1) {
        // Validaciones de estado global
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        }
        // Validaciones del jugador y turnos
        else if (juego.buscarJugadorPorNombre(args[0]) == nullptr) {
          cout << "(Jugador no válido) El jugador " << args[0]
               << " no forma parte de esta partida.\n";
        } else if (juego.obtenerJugadorEnTurno()->nombre != args[0]) {
          cout << "(Jugador fuera de turno) No es el turno del jugador "
               << args[0] << ".\n";
        } else {
          vector<Territorio *> territorio_posible;
          for (int i = 0; i < 42; i++) {
            if (juego.tablero.territorios.obtener(i).colorOcupante ==
                juego.obtenerJugadorEnTurno()->color) {
              territorio_posible.push_back(
                  &juego.tablero.territorios.obtener(i));
            }
          }

          int tropas_a_anadir = calcular_tropas(territorio_posible.size());
          if (tropas_a_anadir < 3)
            tropas_a_anadir = 3;

          while (tropas_a_anadir > 0) {
            cout << "\nTienes " << tropas_a_anadir
                 << " unidades para ubicar.\n";
            cout << "En cual de tus territorios deseas ubicar unidades?\n";

            int index = 1;
            for (vector<Territorio *>::iterator i = territorio_posible.begin();
                 i != territorio_posible.end(); ++i) {
              cout << index << ". " << (*i)->nombre
                   << " (Actuales: " << (*i)->numEjercitos << ")\n";
              index++;
            }

            int opt = 0;
            cout << "Elige un territorio (1-" << territorio_posible.size()
                 << "): ";
            cin >> opt;

            if (opt > 0 && opt <= (int)territorio_posible.size()) {
              int cantidad = 0;
              cout << "Cuantas unidades deseas ubicar aqui? (Maximo "
                   << tropas_a_anadir << "): ";
              cin >> cantidad;

              if (cantidad > 0 && cantidad <= tropas_a_anadir) {
                territorio_posible[opt - 1]->modificarEjercitos(cantidad);
                tropas_a_anadir -= cantidad;

                cout << "-> Se han anadido " << cantidad << " tropas a "
                     << (*territorio_posible[opt - 1]).nombre << ".\n";
              } else {
                cout << "(Cantidad invalida, intenta de nuevo)\n";
              }
            } else {
              cout << "(Opcion invalida, intenta de nuevo)\n";
              cin.clear();
              cin.ignore(10000, '\n');
            }
          }

          // Limpiar el buffer de cin por si quedó un salto de línea antes de
          cin.ignore(10000, '\n');

          Jugador *jugadorActual = juego.obtenerJugadorEnTurno();
          jugadorActual->faseActual = FASE_ATACAR; // Avanza a la siguiente fase
          cout << "(Comando correcto) El jugador " << args[0]
               << " ha terminado de reclamar y ubicar sus unidades.\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: atacar
    // ==========================================
    else if (comando == "atacar") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else if (juego.buscarJugadorPorNombre(args[0]) == nullptr) {
          cout << "(Jugador no válido) El jugador " << args[0]
               << " no forma parte de esta partida.\n";
        } else if (juego.obtenerJugadorEnTurno()->nombre != args[0]) {
          cout << "(Jugador fuera de turno) No es el turno del jugador "
               << args[0] << ".\n";
        } else if (juego.obtenerJugadorEnTurno()->faseActual ==
                   FASE_OBTENER_UNIDADES) {
          // Valida que el jugador ya haya obtenido sus unidades
          cout << "(Jugador no ha ubicado unidades) El jugador " << args[0]
               << " no ha ejecutado el comando obtener_unidades.\n";
        } else {
          int index = 1;
          int opcion = 0;
          int opcion2 = 0;
          vector<Territorio *> misTerritorios;
          for (int i = 0; i < 42; i++) {
            if (juego.tablero.territorios.obtener(i).colorOcupante ==
                juego.obtenerJugadorEnTurno()->color) {
              misTerritorios.push_back(&juego.tablero.territorios.obtener(i));
            }
          }
          cout << "\n desde cual de tus territorios quieres atacar? ";
          for (vector<Territorio *>::iterator i = misTerritorios.begin();
               i != misTerritorios.end(); ++i) {
            cout << index << ". " << (*i)->nombre << " (tienes "
                 << (*i)->numEjercitos << " tropas)\n";
            index++;
          }
          cin >> opcion;
          if (opcion <= 0 && opcion >= (index - 1)) {
            cout << "Error: Opcion invalida\n";
          } else {
            cout << "\n a cual territorio quieres atacar? \n";
            for (int i = 0; i < misTerritorios[opcion - 1]->vecinos.longitud();
                 ++i) {
              Territorio *vecino =
                  misTerritorios[opcion - 1]->vecinos.obtener(i);
              cout << i + 1 << ". " << vecino->nombre << endl;
            }
            do {
              cin >> opcion2;
              if (opcion2 <= 0 &&
                  opcion2 >= (misTerritorios[opcion - 1]->vecinos.longitud())) {
                cout << "Error: Opcion invalida\n";
              }
            } while (opcion2 <= 0 &&
                     opcion2 >=
                         (misTerritorios[opcion - 1]->vecinos.longitud()));
          }
          // y dados
          Jugador *jugadorActual = juego.obtenerJugadorEnTurno();
          jugadorActual->faseActual =
              FASE_FORTIFICAR; // Avanza a la fase de fortificación
          cout << "(Comando correcto) El jugador " << args[0]
               << " ha terminado de atacar.\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: fortificar
    // ==========================================
    else if (comando == "fortificar") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else if (juego.buscarJugadorPorNombre(args[0]) == nullptr) {
          cout << "(Jugador no válido) El jugador " << args[0]
               << " no forma parte de esta partida.\n";
        } else if (juego.obtenerJugadorEnTurno()->nombre != args[0]) {
          cout << "(Jugador fuera de turno) No es el turno del jugador "
               << args[0] << ".\n";
        } else if (juego.obtenerJugadorEnTurno()->faseActual ==
                       FASE_OBTENER_UNIDADES ||
                   juego.obtenerJugadorEnTurno()->faseActual == FASE_ATACAR) {
          // Valida que el jugador haya pasado por la fase de ataque primero
          cout << "(Jugador no ha atacado) El jugador " << args[0]
               << " no ha ejecutado el comando atacar.\n";
        } else {
          // TODO: Lógica de movimiento de infantería entre territorios aliados
          // vecinos
          cout << "(Comando correcto) El jugador " << args[0]
               << " ha terminado de fortificar su posición.\n";
          // Al terminar la fortificación, finaliza el turno del jugador actual
          juego.pasarAlSiguienteTurno();
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: estado_juego
    // ==========================================
    else if (comando == "estado_juego") {
      if (args.empty()) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else {
          // Imprime el resumen de la partida en curso
          cout << "--- ESTADO DEL JUEGO ---\n";
          cout << "Jugadores en partida (" << juego.jugadores.longitud()
               << "):\n";
          for (int i = 0; i < juego.jugadores.longitud(); ++i) {
            Jugador &jug = juego.jugadores.obtener(i);
            cout << " - " << jug.nombre << " [Color: " << jug.color << "]\n";
          }

          Jugador *enTurno = juego.obtenerJugadorEnTurno();
          if (enTurno != nullptr) {
            cout << "\nJugador con el turno actual: " << enTurno->nombre
                 << "\n\n";
          }

          cout << "Lista de Territorios:\n";
          for (int i = 0; i < juego.tablero.territorios.longitud(); ++i) {
            Territorio &ter = juego.tablero.territorios.obtener(i);
            cout << " - " << ter.codigo << " (" << ter.nombre << "): "
                 << "Controlado por '" << ter.colorOcupante << "' con "
                 << ter.numEjercitos << " unidades.\n";
          }
          cout << "------------------------\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: salir
    // ==========================================
    else if (comando == "salir") {
      if (args.empty()) {
        is_running = false; // Finaliza la bandera de ejecución del REPL
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: guardar
    // ==========================================
    else if (comando == "guardar") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else {
          // TODO: Lógica de serialización del estado del juego en archivo de
          // texto
          cout << "(Comando correcto) La partida ha sido guardada "
                  "correctamente.\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: guardar_comprimido
    // ==========================================
    else if (comando == "guardar_comprimido") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else {
          // TODO: Lógica de compresión mediante Huffman (Componente 2)
          cout << "(Comando correcto) La partida ha sido codificada y guardada "
                  "correctamente.\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: costo_conquista
    // ==========================================
    else if (comando == "costo_conquista") {
      // Valida 2 parámetros: jugador y territorio
      if (args.size() == 2) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else {
          // TODO: Búsqueda en grafos (Dijkstra u otro)
          cout << "(Comando correcto) Para conquistar el territorio " << args[1]
               << ", " << args[0]
               << " debe atacar desde territorio_1, pasando por los "
                  "territorios territorio_2, territorio_3, ..., territorio_m. "
                  "Debe conquistar n unidades de ejército.\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: conquista_mas_barata
    // ==========================================
    else if (comando == "conquista_mas_barata") {
      if (args.size() == 1) {
        if (!is_initialized) {
          cout << "(Juego no inicializado) Esta partida no ha sido "
                  "inicializada correctamente.\n";
        } else if (juego.estaTerminado()) {
          cout << "(Juego terminado) Esta partida ya tuvo un ganador.\n";
        } else {
          // TODO: Algoritmo completo de rutas sobre grafos
          cout << "(Comando correcto) La conquista más barata es avanzar sobre "
                  "el territorio territorio_1 desde el territorio "
                  "territorio_2. Para conquistar el territorio territorio_1, "
                  "debe atacar desde territorio_2, pasando por los territorios "
                  "territorio_3, territorio_4, ..., territorio_m. Debe "
                  "conquistar n unidades de ejército.\n";
        }
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO: ayuda
    // ==========================================
    else if (comando == "ayuda") {
      if (args.empty()) {
        mostrar_ayuda();
      } else if (args.size() == 1) {
        mostrar_ayuda_comando(args[0]);
      } else {
        cout << "Error: Numero de parametros incorrecto.\n";
        mostrar_ayuda_comando(comando);
      }
    }
    // ==========================================
    // COMANDO NO RECONOCIDO
    // ==========================================
    else {
      cout << "Comando no reconocido. Escriba 'ayuda' para ver la lista de "
              "comandos disponibles.\n";
    }
  }

  return 0;
}
