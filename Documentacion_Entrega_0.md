# Documentación del Proyecto: Estructuras de Datos - Entrega 0

Ivan Wright  
Sebastian  
Universidad Javeriana

## Declaración de Uso de IA Generativa (Cumplimiento Numeral 3.5)

Esta entrega ha sido estructurada y parcialmente documentada con el apoyo de herramientas de IA generativa (Antigravity). El uso de IA se ha limitado estrictamente a la generación de plantillas de texto, estructuración de la presentación, definición de flujos de usuario y soporte en el refinamiento del código base en C++.

### Prompts Utilizados:

*   **Prompt 1 (Estructuración, Plantilla y Verificación):** "Actúa como un desarrollador de software documentando un proyecto de Estructuras de Datos. El PDF del enunciado oficial está dentro de la carpeta D:\ProyectoPedro. Necesito que generes el contenido de un archivo en formato Markdown (.md) que sirva como documentación y presentación del código específicamente para la Entrega 0 (semana 3). Además, proporciona una plantilla para estructurar toda la información requerida de manera organizada. Reglas estrictas para tu respuesta: NO incluyas saludos, introducciones, confirmaciones, ni conclusiones. Tu respuesta debe ser única y exclusivamente el contenido del archivo Markdown. Basándote en el numeral 3.1 y 2.4 del documento adjunto, el Markdown debe explicar claramente cómo el código fuente cumple con los requisitos obligatorios (prompt $, validaciones, comando ayuda, sistema centralizado, mensajes exactos). Añade un Checklist de Envío con las reglas estrictas de la sección 3."
*   **Prompt 2 (Verificación de Redacción y Criterios):** "Realiza una verificación de que el contenido generado cumple con todos los criterios solicitados en el enunciado."
*   **Prompt 3 (Flujos de Usuario - User Paths):** "Para guiar la implementación del código, tener claridad sobre qué hacer y evitar errores lógicos, primero hicimos los flujos de usuario (User Paths). Con base en el numeral 2.1 del enunciado oficial, genera los flujos de usuario detallando paso a paso la interacción de la consola para: el flujo de onboarding y la inicialización de la partida, el ciclo del turno de juego (reclutamiento, ataque, fortificación), los comandos de consulta/estrategia, y el flujo de guardado y cierre."
*   **Prompt 4 (Creación de la Estructura Base y Módulo de Ayuda en C++):** "Actúa como un desarrollador experto en C++. Con base en los requerimientos del enunciado oficial (numerales 2.4 y 3.1) y la especificación de los User Paths previamente estructurados, genera el archivo principal de la Entrega 0 (main.cpp). El código debe incluir: 1. Las librerías estándar necesarias (, , , , , etc.). 2. Una estructura de datos adecuada (como un std::map de structs) para registrar todos los comandos del sistema junto con su sintaxis de uso y descripción detallada. 3. Las funciones auxiliares mostrar_ayuda() y mostrar_ayuda_comando(const string& cmd) para gestionar la salida del comando ayuda tanto en su forma general como específica (ayuda ). 4. Asegúrate de incluir únicamente los encabezados e implementaciones esenciales para esta fase inicial, manteniendo una arquitectura limpia y extensible."
*   **Prompt 5 (Verificación, Corrección y Auditoría del Bucle REPL):** "Actúa como un revisor de código (Code Reviewer) en C++. Analiza la implementación de la consola interactiva (ciclo REPL en main.cpp) y verifícala minuciosamente contra las especificaciones del enunciado oficial, concretamente los numerales 2.1 (formato de comandos y mensajes de salida), 2.4 (interacción, prompt $ y mensajes) y 3.1 (criterios de evaluación de la Entrega 0). Realiza las correcciones necesarias sobre el código fuente para garantizar que: 1. El prompt de la línea de comandos sea estrictamente el carácter $ seguido de un espacio (cout << "$ ";). 2. Se realice una tokenización correcta de la entrada con std::stringstream para validar la cantidad exacta de parámetros de cada comando. 3. Los mensajes de respuesta (de éxito, error de parámetros o juego no inicializado) coincidan carácter por carácter (incluyendo paréntesis y ortografía) con las plantillas del enunciado. Requisito de documentación: Al inicio del archivo o en la sección de revisión del documento, añade un bloque de comentarios detallado que documente: El código base previo sometido a revisión y la lista explícita de correcciones realizadas junto con el motivo técnico de cada ajuste."

## 1. Cumplimiento de Requisitos Obligatorios

A continuación, se explica de qué manera la estructura y lógica del código fuente satisfacen cada uno de los criterios exigidos para esta fase inicial:

### 1.1. Consola Interactiva y Prompt Estricto
El sistema se ha diseñado con un ciclo de ejecución principal (REPL) que simula una terminal o consola del sistema operativo. Se ha configurado el flujo de salida estándar de modo que, en cada iteración del ciclo, el indicador de línea de comandos (prompt) presentado al usuario sea estrictamente el carácter `$` seguido de un espacio (`cout << "$ ";`) para la captura de texto, garantizando la interacción continua según el numeral 2.4 y evaluado en el numeral 3.1.

### 1.2. Validación de Comandos y Parámetros
El código incluye un módulo de análisis léxico utilizando `std::stringstream` que toma la entrada del usuario y separa el comando principal de sus argumentos:
*   **Identificación del comando:** El primer token es verificado contra la estructura de comandos permitidos.
*   **Verificación de parámetros:** Se evalúan los argumentos restantes mediante `args.size()` o `args.empty()`, comprobando el cumplimiento estricto de la cantidad exacta esperada. Si hay inconsistencias o número incorrecto de argumentos, se imprime el mensaje de error correspondiente y se despliega la ayuda específica del comando.

### 1.3. Implementación del Comando ayuda
Se implementó un módulo de asistencia basado en un diccionario `std::map<string, ComandoAyuda>` que almacena la sintaxis y descripción de cada comando:
*   **ayuda:** Sin argumentos, itera sobre la colección e imprime la lista completa de comandos disponibles.
*   **ayuda <comando>:** Con el nombre de un comando válido como parámetro, extrae y despliega la sintaxis exacta de uso y su descripción.

### 1.4. Integración en un Sistema Centralizado
Todo el procesamiento se realiza en un flujo único de ejecución dentro del archivo principal `main.cpp`. Las funcionalidades no operan como programas independientes, sino que están totalmente modularizadas e integradas en una consola unificada (numeral 2.4).

### 1.5. Formatos Exactos de Mensajes
La salida estándar está estrictamente controlada. Cada comando valida su estado (ej. `is_initialized`) e imprime los mensajes de éxito o error respetando paréntesis, mayúsculas y ortografía exacta según las plantillas del enunciado (ej. `(Comando correcto) ...`, `(Juego no inicializado) ...`).

## 2. Flujos de Usuario (User Paths)

Para guiar la implementación del código y evitar fallos lógicos, se definieron los siguientes flujos de interacción basados en el numeral 2.1 del enunciado:

### 2.1. Flujo de Onboarding e Inicialización de Partida
*   **Paso 1:** El usuario inicia el programa y recibe el prompt `$ `.
*   **Paso 2:** Ejecuta `inicializar archivo_inicio.txt`.
*   **Paso 3:** El sistema valida la cantidad de parámetros (1) y retorna: `(Comando correcto) El juego se ha inicializado correctamente.`

### 2.2. Flujo del Turno de Juego (Core Gameplay Loop)
*   **Reclutamiento:** `obtener_unidades <nombre_jugador>` → Retorna `(Comando correcto) El jugador <nombre_jugador> ha terminado de reclamar y ubicar sus unidades.`
*   **Ataque:** `atacar <nombre_jugador>` → Retorna `(Comando correcto) El jugador <nombre_jugador> ha terminado de atacar.`
*   **Fortificación:** `fortificar <nombre_jugador>` → Retorna `(Comando correcto) El jugador <nombre_jugador> ha terminado de fortificar su posicion.`

### 2.3. Flujo de Planeación y Estrategia
*   **Consulta General:** `estado_juego` → Muestra el resumen del tablero y el jugador en turno.
*   **Cálculos de Grafo:** `costo_conquista` y `conquista_mas_barata` → Muestran las rutas y costos de conquista.

### 2.4. Flujo de Guardado y Cierre
*   **Guardado:** Comandos `guardar <archivo>` y `guardar_comprimido <archivo>`.
*   **Cierre:** `salir` cambia la bandera del ciclo REPL finalizando la ejecución de forma segura.

## 3. Auditoría de Código y Registro de Correcciones (`main.cpp`)

Durante la fase de revisión y refactorización del código base, se identificaron e implementaron las siguientes mejoras técnicas:

| Componente / Sección | Código Previo (Sometido a Revisión) | Código Corregido e Implementado | Motivo Técnico de la Corrección |
| :--- | :--- | :--- | :--- |
| **Procesamiento de Entrada** | Lógica manual mediante `find(' ')`, `substr()` y `erase()`. | Tokenización limpia mediante `std::stringstream ss(linea);` | Evitar errores de índice, mejorar la eficiencia en O(N) y manejar múltiples espacios consecutivos correctamente. |
| **Validación de Estado Juego** | Bifurcaciones if-else repetitivas comprobando `args.size() == 0`. | Uso de `args.empty()` y estructuración directa de condicionales. | Mayor legibilidad del código, reducción de código duplicado y apego a buenas prácticas de C++. |
| **Terminación del Programa** | Llamada directa a la función `exit(0);` dentro del comando `salir`. | Modificación de bandera de control `is_running = false;` en el bucle REPL. | Garantizar una finalización limpia de la pila de llamadas, liberación de recursos y un retorno de main() adecuado. |

## 4. Checklist de Envío (Cumplimiento Sección 3)

*   [x] **Formato Único de Compresión:** La entrega consta de un único archivo comprimido en formato `.zip`.
*   [x] **Sin Estructura de Carpetas Internas:** El archivo `.zip` NO contiene subcarpetas. Todos los archivos están ubicados en la raíz.
*   [x] **Archivos Exclusivos Permitidos:** El comprimido contiene únicamente el documento en formato `.pdf` y el archivo de código fuente `main.cpp`.
