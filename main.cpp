#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

using namespace std;
/*
=============================================================================
Revisión de Código y Registro de Correcciones (Auditoría del Bucle REPL)
=============================================================================
El siguiente bloque documenta el código base previo que fue sometido a revisión
y la lista explícita de correcciones realizadas junto con el motivo técnico de
cada ajuste, cumpliendo con los requerimientos de documentación del Prompt 5.

1. Procesamiento de Entrada (Tokenización)
   - Código Previo: 
       string arg; 
       while(linea.find(' ') != string::npos) { 
           // lógica manual con substr y erase repetitivo 
       }
   - Código Corregido: Se implementó tokenización limpia mediante std::stringstream ss(linea);
   - Motivo Técnico: Evitar errores de índice, mejorar la eficiencia en O(N) y manejar
     múltiples espacios consecutivos correctamente, proveyendo un parseo seguro.

2. Validación de Estado de Juego
   - Código Previo: 
       else if (comando == "estado_juego") { 
           if(args.size() == 0 && is_initialized == true) { ... } 
           else if(args.size() == 0 && is_initialized == false) { ... } 
       }
   - Código Corregido: Se utilizó args.empty() y se reestructuraron los condicionales directos.
   - Motivo Técnico: Mayor legibilidad del código, reducción de código duplicado 
     y apego a las buenas prácticas de C++.

3. Terminación del Programa
   - Código Previo: 
       else if (comando == "salir") { 
           exit(0); 
       }
   - Código Corregido: Se modificó la bandera de control del ciclo:
       else if (comando == "salir") { 
           is_running = false; 
       }
   - Motivo Técnico: Garantizar una finalización limpia de la pila de llamadas, 
     liberación de recursos y un retorno de main() adecuado sin forzar la salida.
=============================================================================
*/

// Estructura para almacenar información de ayuda de comandos
struct ComandoAyuda {
    string uso;
    string descripcion;
};

// Diccionario de comandos para el comando 'ayuda'
map<string, ComandoAyuda> comandos = {
    {"inicializar", {"inicializar <archivo>", "Inicializa el juego con los datos de un archivo de texto plano o binario."}},
    {"obtener_unidades", {"obtener_unidades <nombre_jugador>", "Calcula y asigna unidades adicionales al jugador."}},
    {"atacar", {"atacar <nombre_jugador>", "Inicia la fase de ataque para el jugador especificado."}},
    {"fortificar", {"fortificar <nombre_jugador>", "Inicia la fase de fortificacion para el jugador especificado."}},
    {"estado_juego", {"estado_juego", "Muestra el estado actual del juego."}},
    {"salir", {"salir", "Termina la ejecucion de la aplicacion."}},
    {"guardar", {"guardar <nombre_archivo>", "Guarda el estado del juego en un archivo de texto plano."}},
    {"guardar_comprimido", {"guardar_comprimido <nombre_archivo>", "Guarda el estado del juego en un archivo binario comprimido (Huffman)."}},
    {"costo_conquista", {"costo_conquista <nombre_jugador> <territorio>", "Calcula el costo y secuencia de territorios para conquistar el territorio indicado."}},
    {"conquista_mas_barata", {"conquista_mas_barata <nombre_jugador>", "Calcula y sugiere la conquista que implica la menor perdida de ejercitos."}},
    {"ayuda", {"ayuda [comando]", "Muestra la lista de comandos disponibles o detalla el uso de un comando especifico."}}
};

// Función auxiliar para imprimir la lista general de comandos
void mostrar_ayuda() {
    cout << "Lista de comandos disponibles:\n";
    for (const auto& par : comandos) {
        cout << " - " << par.first << "\n";
    }
}

// Función auxiliar para imprimir el detalle de un comando
void mostrar_ayuda_comando(const string& cmd) {
    if (comandos.find(cmd) != comandos.end()) {
        cout << "Uso: " << comandos[cmd].uso << "\n";
        cout << "Descripcion: " << comandos[cmd].descripcion << "\n";
    } else {
        cout << "El comando '" << cmd << "' no existe. Escriba 'ayuda' para ver los comandos disponibles.\n";
    }
}

int main() {
    string linea;
    bool is_running = true;
    bool is_initialized = false; 
    
    // Ciclo de evaluación de la consola interactiva (REPL)
    while (is_running) {
        // Imprime estrictamente el caracter $ como indicador de linea
        cout << "$ ";
        if (!getline(cin, linea)) {
            break; // Finalizar si ocurre EOF
        }
        
        if (linea.empty()) continue;
        
        // Tokenización básica por espacios
        stringstream ss(linea);
        string comando;
        vector<string> args;
        
        ss >> comando;
        string arg;
        while (ss >> arg) {
            args.push_back(arg);
        }
        
        // Validación de comandos y parámetros
        if (comando == "inicializar") {
            if (args.size() == 1) {
                is_initialized = true; // Para la Entrega 0 solo validamos
                cout << "(Comando correcto) El juego se ha inicializado correctamente.\n";
            } else {
                cout << "Error: Numero de parametros incorrecto.\n";
                mostrar_ayuda_comando(comando);
            }
        } 
        else if (comando == "obtener_unidades") {
            if (args.size() == 1) {
                if (!is_initialized) {
                    cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n";
                } else {
                    cout << "(Comando correcto) El jugador " << args[0] << " ha terminado de reclamar y ubicar sus unidades.\n";
                }
            } else {
                cout << "Error: Numero de parametros incorrecto.\n";
                mostrar_ayuda_comando(comando);
            }
        }
        else if (comando == "atacar") {
            if (args.size() == 1) {
                if (!is_initialized) {
                    cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n";
                } else {
                    cout << "(Comando correcto) El jugador " << args[0] << " ha terminado de atacar.\n";
                }
            } else {
                cout << "Error: Numero de parametros incorrecto.\n";
                mostrar_ayuda_comando(comando);
            }
        }
        else if (comando == "fortificar") {
            if (args.size() == 1) {
                if (!is_initialized) {
                    cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n";
                } else {
                    cout << "(Comando correcto) El jugador " << args[0] << " ha terminado de fortificar su posicion.\n";
                }
            } else {
                cout << "Error: Numero de parametros incorrecto.\n";
                mostrar_ayuda_comando(comando);
            }
        }
        else if (comando == "estado_juego") {
            if (args.empty()) {
                if (!is_initialized) {
                    cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n";
                } else {
                    cout << "(Comando correcto) Mostrando estado del juego (simulado).\n";
                }
            } else {
                cout << "Error: Numero de parametros incorrecto.\n";
                mostrar_ayuda_comando(comando);
            }
        }
        else if (comando == "salir") {
            if (args.empty()) {
                is_running = false; // Finaliza el bucle
            } else {
                cout << "Error: Numero de parametros incorrecto.\n";
                mostrar_ayuda_comando(comando);
            }
        }
        else if (comando == "guardar") {
            if (args.size() == 1) {
                if (!is_initialized) {
                    cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n";
                } else {
                    cout << "(Comando correcto) La partida ha sido guardada correctamente.\n";
                }
            } else {
                cout << "Error: Numero de parametros incorrecto.\n";
                mostrar_ayuda_comando(comando);
            }
        }
        else if (comando == "guardar_comprimido") {
            if (args.size() == 1) {
                if (!is_initialized) {
                    cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n";
                } else {
                    cout << "(Comando correcto) La partida ha sido codificada y guardada correctamente.\n";
                }
            } else {
                cout << "Error: Numero de parametros incorrecto.\n";
                mostrar_ayuda_comando(comando);
            }
        }
        else if (comando == "costo_conquista") {
            if (args.size() == 2) {
                if (!is_initialized) {
                    cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n";
                } else {
                    cout << "(Comando correcto) Para conquistar el territorio " << args[1] 
                         << ", " << args[0] << " debe atacar desde X, pasando por los territorios Y, Z... Debe conquistar n unidades de ejercito.\n";
                }
            } else {
                cout << "Error: Numero de parametros incorrecto.\n";
                mostrar_ayuda_comando(comando);
            }
        }
        else if (comando == "conquista_mas_barata") {
            if (args.size() == 1) {
                if (!is_initialized) {
                    cout << "(Juego no inicializado) Esta partida no ha sido inicializada correctamente.\n";
                } else {
                    cout << "(Comando correcto) La conquista mas barata es avanzar sobre el territorio X desde el territorio Y.\n";
                }
            } else {
                cout << "Error: Numero de parametros incorrecto.\n";
                mostrar_ayuda_comando(comando);
            }
        }
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
        else {
            cout << "Comando no reconocido. Escriba 'ayuda' para ver la lista de comandos disponibles.\n";
        }
    }
    
    return 0;
}
