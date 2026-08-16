# Proyecto de Estructuras de Datos - Sistema de Apoyo para Risk

Este proyecto implementa un sistema de apoyo interactivo para el clásico juego de mesa **Risk**, diseñado para la asignatura de Estructuras de Datos. 

Actualmente, el proyecto refleja los requisitos de la **Entrega 0**, enfocada de manera exclusiva en la **Interfaz de Usuario y Validación de Comandos**.

## Características de la Entrega 0

En esta etapa inicial, el proyecto cuenta con un entorno interactivo de línea de comandos (REPL) con las siguientes características:
- **Consola Interactiva:** Un ciclo infinito que lee entradas del usuario tras el prompt `$ `.
- **Validación Paramétrica:** Antes de ejecutar cualquier simulación, el sistema valida que los comandos ingresados coincidan exactamente en nombre y cantidad de parámetros con lo especificado en el diseño.
- **Sistema Integrado:** Todos los componentes se gestionan desde un mismo programa unificado.
- **Mensajes de Interfaz:** Respuestas adaptadas a cada comando (mensajes de éxito, advertencia de "juego no inicializado" y control de errores por parámetros inválidos).

*(Nota: La lógica interna de Estructuras Lineales, Árboles de Huffman para compresión y Grafos para estrategias de conquista se incorporarán en las entregas 1, 2 y 3 respectivamente).*

## Instrucciones de Compilación y Ejecución

El proyecto está diseñado para compilarse utilizando el compilador GNU C++ (`g++`).

### 1. Compilar
Abre tu terminal en el directorio del proyecto y ejecuta el siguiente comando:
```bash
g++ main.cpp -o risk_app
```

### 2. Ejecutar
Una vez compilado, puedes iniciar la consola interactiva:

**En Windows:**
```bash
.\risk_app.exe
```

**En Linux / macOS:**
```bash
./risk_app
```

## Lista de Comandos Disponibles

Dentro de la aplicación (cuando veas el prompt `$ `), puedes escribir:

- `ayuda`: Muestra la lista general de todos los comandos soportados.
- `ayuda <comando>`: Despliega la sintaxis correcta y descripción de un comando en particular.
- `inicializar <archivo_inicio>`: Inicializa la partida cargando un archivo de texto o comprimido.
- `obtener_unidades <nombre_jugador>`: Permite al jugador reclamar unidades extra.
- `atacar <nombre_jugador>`: Inicia el flujo de ataque hacia territorios enemigos.
- `fortificar <nombre_jugador>`: Mueve unidades entre territorios propios.
- `estado_juego`: Imprime en pantalla la situación actual (jugadores, territorios y unidades).
- `guardar <nombre_archivo>`: Exporta el estado actual en formato de texto plano.
- `guardar_comprimido <nombre_archivo>`: Exporta el estado actual comprimido mediante Huffman.
- `costo_conquista <nombre_jugador> <territorio>`: Calcula con grafos la ruta óptima a conquistar.
- `conquista_mas_barata <nombre_jugador>`: Indica cuál es la conquista que implica la menor pérdida posible.
- `salir`: Termina la ejecución y cierra el programa.