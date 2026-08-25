# Proyecto de Estructuras de Datos - Sistema de Apoyo para Risk

Este proyecto implementa un sistema de apoyo interactivo para el clásico juego de mesa **Risk**, diseñado para la asignatura de Estructuras de Datos. 

Actualmente, el proyecto refleja los requisitos de la **Entrega 1**, enfocada en el **Componente 1: Configuración del juego** implementado a partir de estructuras lineales.

## Características de la Entrega 1

En esta etapa, el proyecto cuenta con un entorno interactivo de línea de comandos (REPL) y la lógica inicial del juego con las siguientes características:
- **Consola Interactiva:** Un ciclo infinito que lee entradas del usuario tras el prompt `$ `.
- **Validación y Estructuras Lineales:** Los comandos principales del juego (`inicializar`, `obtener_unidades`, `atacar`, `fortificar`, `estado_juego`) validan sus parámetros y comienzan a incorporar la lógica por turnos usando estructuras de datos lineales.
- **Sistema Integrado:** Todos los componentes se gestionan desde un mismo programa unificado.
- **Mensajes de Interfaz:** Respuestas adaptadas a cada comando (mensajes de éxito, advertencia de "juego no inicializado" y control de errores por parámetros inválidos).

*(Nota: La compresión con Árboles de Huffman y las estrategias de conquista con Grafos se incorporarán en las entregas 2 y 3 respectivamente).*

## Documentación Técnica y Auditoría

Para una revisión en profundidad de los requerimientos cumplidos, decisiones de diseño de software (TADs), justificación de las estructuras de datos lineales y el registro de correcciones de IA, por favor consulte el documento de auditoría de la entrega:

- [Auditoría Técnica y Documentación - Entrega 1](docs/ENTREGA1_DOCUMENTACION.md)

## Instrucciones de Compilación y Ejecución

El proyecto está diseñado para compilarse utilizando el compilador GNU C++ (`g++`).

### 1. Compilar
Abre tu terminal en el directorio raíz del proyecto y ejecuta:
```bash
g++ src/main.cpp -I tads -o risk_app
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

## Estructura del Proyecto

- `tads/`: Implementación e interfaces de los Tipos Abstractos de Datos (`.h`, `.hxx`, `.cpp`).
- `src/`: Código fuente principal de la aplicación (`main.cpp`).
- `docs/`: Documentación, requisitos y especificaciones del proyecto (`.md`).
- `data/`: Archivos de prueba y datos de inicio (`.txt`).

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