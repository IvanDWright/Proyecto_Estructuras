# Especificación de Requisitos de Software - Entrega 1 (Risk)

El siguiente documento detalla los requerimientos formales de ingeniería que el **código fuente** debe cumplir para el Componente 1 (Configuración y Estructuras Lineales) del proyecto.

## 1. Requisitos Funcionales (RF)

Los requisitos funcionales describen qué debe hacer el código y las reglas de negocio que debe implementar.

- **RF-01 (Inicialización de Partida):** El sistema debe leer y parsear un archivo de texto (`archivo_inicio.txt`) para inicializar el estado del juego. Debe registrar a los jugadores (cantidad, nombres y colores) y asignar los 42 territorios al jugador correspondiente con su respectiva cantidad de unidades.
- **RF-02 (Cálculo de Unidades Adicionales):** El sistema debe calcular automáticamente la cantidad de infantería adicional que un jugador puede reclamar al inicio de su turno. El código debe calcular:
  - Unidades por territorios: `(total_territorios_del_jugador) / 3`.
  - Unidades por continentes controlados en su totalidad (bonificaciones predefinidas).
  - Unidades por intercambio de cartas (si aplica en esta entrega o se deja preparado).
- **RF-03 (Asignación de Unidades):** El programa debe interactuar con el usuario para restar unidades del "pool disponible" y sumarlas a territorios específicos que pertenezcan al jugador en turno.
- **RF-04 (Mecánica de Ataque):** El sistema debe recibir un territorio de origen y uno de destino (que sea vecino) y calcular las bajas mediante generación de números pseudoaleatorios (simulando lanzamiento de dados: máximo 3 dados rojos, máximo 2 blancos). 
- **RF-05 (Conquista de Territorios):** Si las unidades del territorio defensor llegan a 0 durante un ataque, el sistema debe cambiar la propiedad (color) del territorio al jugador atacante y permitir el traslado de infantería desde el territorio atacante.
- **RF-06 (Fortificación):** El sistema debe permitir el traslado de unidades desde un territorio de origen hacia un territorio destino, validando que ambos pertenezcan al jugador en turno y sean vecinos entre sí.
- **RF-07 (Estado de Juego):** El código debe recorrer las estructuras en memoria e imprimir un resumen con: cantidad de jugadores, detalles de cada uno, de quién es el turno, y un recorrido por los 42 territorios mostrando dueño y número de unidades.

## 2. Requisitos No Funcionales y Restricciones (RNF)

Restricciones arquitectónicas, de diseño y de rendimiento que debe seguir la implementación.

- **RNF-01 (Uso de Estructuras Lineales):** Las entidades y colecciones del juego (listas de jugadores, listas de territorios, lista de adyacencias, etc.) deben ser almacenadas y manipuladas en memoria utilizando **TADs basados en estructuras lineales** (tales como Listas enlazadas, arreglos dinámicos, colas o pilas). 
- **RNF-02 (Integración en un Sistema Único):** La lógica del juego no debe ser construida como scripts independientes. Todo el código del Componente 1 debe integrarse en la misma aplicación interactiva (bucle de comandos REPL).
- **RNF-03 (Compatibilidad de Compilación):** El código fuente en C++ debe compilar estrictamente en el compilador **`gnu-g++` (versión 4.0.0 o superior)** sin errores fatales.
- **RNF-04 (Coherencia con el Diseño):** Las clases, métodos (firmas), y TADs implementados en el código C++ deben ser un reflejo exacto del "Documento de Diseño" y sus diagramas. Si el diseño define una clase `Territorio` con un método `agregarVecino()`, el código debe tener la misma firma.

## 3. Requisitos de Validación y Manejo de Errores (RVE)

Casos borde y restricciones de integridad de datos. El código debe contener las condicionales y manejos de excepciones para atrapar:

- **RVE-01 (Integridad del Archivo):** Al ejecutar `inicializar`, el código debe validar:
  - Que los colores leídos en cada territorio existan en la lista de jugadores registrada en el archivo.
  - Que el conteo total de unidades por jugador en el archivo coincida con la suma definida por las reglas del juego.
  - Que existan exactamente 42 territorios asignados.
- **RVE-02 (Validación de Turno):** Para los comandos `obtener_unidades`, `atacar` y `fortificar`, el código debe comprobar:
  - `(Jugador no válido)`: Si el jugador consultado no existe en la partida.
  - `(Jugador fuera de turno)`: Si el nombre proporcionado existe pero no es el jugador actual del ciclo.
- **RVE-03 (Secuencia de Turno):** El sistema debe bloquear acciones desordenadas validando el estado interno del turno del jugador:
  - No puede atacar si no ha obtenido unidades (`Jugador no ha ubicado unidades`).
  - No puede fortificar si no atacó (o decidió no atacar explícitamente) (`Jugador no ha atacado`).
- **RVE-04 (Validaciones Geográficas):** Al atacar o fortificar, el código debe iterar sobre la estructura de datos del territorio origen para comprobar que el territorio destino está en su lista de vecinos y validar si el dueño del destino es enemigo (para ataque) o aliado (para fortificar).
- **RVE-05 (Coincidencia de Mensajes de Interfaz):** Cualquier salida a pantalla, sea exitosa o de error, debe coincidir caracter por caracter (case-sensitive) con la especificación del PDF (ej: `(Juego no inicializado) Esta partida no ha sido inicializada correctamente.`).

## 4. Estructuras de Datos / Tipos Abstractos de Datos Sugeridos (Modelo de Código)

A nivel de código, se espera que el diseño incluya al menos las siguientes entidades representadas como Clases (o `structs`) manejadas por Estructuras Lineales:

1. **`Juego` (o `Partida`):** Clase principal o Fachada. 
   - *Atributos:* Estado del juego (`enum`: no_inicializado, en_curso, terminado), Estructura lineal de `Jugador`es, Estructura lineal de todos los 42 `Territorio`s, Índice o apuntador al jugador con el turno actual.
2. **`Jugador`:** 
   - *Atributos:* Nombre (`string`), Color (`string`), Cartas en mano (estructura lineal), Unidades extra disponibles, Cantidad de territorios conquistados.
3. **`Territorio`:** 
   - *Atributos:* Código/ID, Nombre, Continente padre, Color del ocupante, Cantidad de infantería, **Lista lineal de referencias a territorios vecinos**.
4. **`Continente`:**
   - *Atributos:* Nombre, Bono de infantería, Lista lineal de territorios que lo componen.
