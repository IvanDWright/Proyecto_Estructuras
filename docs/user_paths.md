# User Paths (Flujos de Usuario) - Proyecto Risk (Entregas 1, 2 y 3)

Con base en los requerimientos del enunciado oficial, la interacción del sistema se rige mediante una consola interactiva de comandos (prompt `$ `). A continuación, se definen los **User Paths** (flujos de usuario) completos del sistema integrando los tres componentes: **Componente 1** (configuración del juego con estructuras lineales), **Componente 2** (almacenamiento de partidas con árbol de Huffman) y **Componente 3** (estrategias de conquista con grafos).

---

## 1. Flujo de Onboarding e Inicialización de Partida
**Objetivo:** El usuario abre la aplicación y configura una partida nueva desde un archivo de texto plano o retoma una partida guardada (en texto o comprimida `.bin`).

* **Paso 1:** El usuario inicia el programa, se inicializa la semilla aleatoria (`srand`) y recibe el prompt interactivo `$ `.
* **Paso 2 (Opcional):** El usuario teclea `ayuda` para listar los comandos disponibles, o `ayuda <comando>` para ver la sintaxis y descripción de uno específico.
* **Paso 3:** El usuario teclea `inicializar <archivo_inicio>` (archivo `.txt` o `.bin`).
* **Paso 4 (Validaciones del sistema):**
  * *Parámetros incorrectos:* `Error: Numero de parametros incorrecto.` + ayuda del comando.
  * *Partida previa activa:* `(Juego en curso) El juego ya ha sido inicializado.`
  * *Archivo no encontrado / no legible:* `(Archivo erróneo) <archivo_inicio> no se encuentra o no puede leerse.`
  * *Archivo vacío:* `(Archivo vacío) <archivo_inicio> no contiene información.`
  * *Formato inválido o inconsistencias en reglas:* `(Archivo sin formato) <archivo_inicio> no contiene información en el formato esperado.`
    * Valida: `N` entre 3 y 6, nombres de máximo 8 caracteres sin repetir, colores sin repetir, 42 territorios con código oficial válido y sin repetir, colores pertenecientes a jugadores registrados, al menos 1 tropa por territorio y sin datos sobrantes.
    * En archivos de texto plano se intenta primero como archivo de inicio: valida además que cada jugador tenga al menos 1 territorio y la suma exacta de unidades iniciales (`35` para 3 jugadores, `30` para 4, `25` para 5 y `20` para 6).
    * Si el texto solo falla por esa suma (partida generada por `guardar` después de jugar), se carga como partida en curso, igual que un `.bin` (numeral 2.2).
    * En archivos `.bin` valida la cabecera Huffman (`1 <= n <= 256`, sin caracteres repetidos, `f_i > 0`, `w > 0`, `sum(f_i) = w`) y la decodificación exacta de `w` caracteres.
* **Paso 5 (Éxito):** El sistema carga las entidades en `ListaLineal` (sobre un juego temporal y luego intercambia en O(1)), ubica el turno en el primer jugador con territorios y muestra:
  `(Comando correcto) El juego se ha inicializado correctamente.`
  * Si se cargó una partida en curso desde texto, se agrega el aviso `[*] Se cargo una partida en curso: ...`.

---

## 2. Flujo del Turno de Juego (Core Gameplay Loop - Componente 1)
**Objetivo:** Un jugador activo realiza las fases reglamentarias de su turno en orden estricto (Reclutamiento -> Ataque -> Fortificación).

> **Validaciones Generales de Turno (aplican a Fases 1, 2 y 3):**
> * Si no hay partida cargada: `(Juego no inicializado) Esta partida no ha sido inicializada correctamente.`
> * Si la partida concluyó: `(Juego terminado) Esta partida ya tuvo un ganador.`
> * Si el jugador no existe: `(Jugador no válido) El jugador <nombre_jugador> no forma parte de esta partida.`
> * Si no es el turno del jugador: `(Jugador fuera de turno) No es el turno del jugador <nombre_jugador>.`

---

### Fase 1: Reclutamiento y Asignación de Unidades (`obtener_unidades`)
* **Paso 1:** El usuario teclea `obtener_unidades <nombre_jugador>`.
* **Paso 2 (Validación de fase):** Si ya ejecutó `obtener_unidades` en este turno:
  `(Jugador ya obtuvo unidades) El jugador <nombre_jugador> ya ejecuto obtener_unidades en este turno.`
* **Paso 3 (Cálculo del sistema):**
  * *Territorios ocupados:* `territorios / 3` (división entera, sin mínimo de 3, según numeral 1.5 del enunciado).
  * *Continentes completos:* América del Sur (+2), Australia (+2), África (+3), América del Norte (+5), Europa (+5), Asia (+7).
  * *Cartas:* Si el jugador posee un grupo canjeable (3 iguales, 1 de cada tipo o un comodín + 2 cualesquiera), se le pregunta si desea canjearlo (`s/n`). El bono sigue la secuencia `4, 6, 8, 10, 12, 15, 20, 25, ...` y suma `+2` tropas directamente en cada territorio propio distinto que aparezca en las cartas del grupo.
* **Paso 4 (Interacción de asignación):**
  * El sistema informa el total de unidades por ubicar (incluso si es `0`: `Tienes 0 unidades para ubicar en este turno.`).
  * Lista todos los territorios del jugador con su **código oficial** y tropas actuales.
  * Solicita iterativamente el **código del territorio** y la **cantidad** (`1` a disponibles) hasta ubicar todas las unidades.
* **Paso 5 (Éxito):** Avanza a fase `Atacar` y muestra:
  `(Comando correcto) El jugador <nombre_jugador> ha terminado de reclamar y ubicar sus unidades.`

---

### Fase 2: Combate y Conquista (`atacar`)
* **Paso 1:** El usuario teclea `atacar <nombre_jugador>`.
* **Paso 2 (Validación de fase):**
  * Si no ha reclamado unidades: `(Jugador no ha ubicado unidades) El jugador <nombre_jugador> no ha ejecutado el comando obtener_unidades.`
  * Si ya terminó la fase de ataque: `(Jugador ya fortifico) El jugador <nombre_jugador> ya no puede atacar.`
  * Si no tiene territorios con más de 1 tropa y vecinos enemigos: `(Jugador no puede atacar) <nombre_jugador> no tiene territorios con mas de 1 tropa y vecinos enemigos para atacar.` (y pasa automáticamente a fase `Fortificar`).
* **Paso 3 (Sub-flujo interactivo de ataque):**
  1. Muestra todos los territorios del jugador con su código (indicando cuáles no están disponibles y el motivo) más la opción `0` para terminar la fase de ataque.
  2. El usuario ingresa el **código de origen** (o `0` para cerrar la fase de ataque y recibir carta si conquistó).
  3. El usuario ingresa la **cantidad de tropas atacantes** (`1` a `tropas_origen - 1`). Si es inválida, informa el error sin alterar el estado y el jugador permanece en fase de ataque.
  4. Muestra los **vecinos enemigos** del origen (obtenidos desde `Territorio::vecinosEnemigos`) y pide el **código del territorio destino**.
  5. **Lanzamiento de dados:** El atacante lanza `min(3, atacantes)` dados y el defensor `min(2, defensores)`, ordenados de mayor a menor; los empates favorecen al defensor. Se muestran dados, duelos y tropas restantes.
  6. **Continuación / Resolución:** El jugador decide si continuar (`s/n`) mientras ambos tengan tropas.
     * Si el defensor llega a `0`: conquista el territorio con las tropas sobrevivientes, marca conquista del turno y verifica victoria global (42 territorios -> `ESTADO_TERMINADO`).
     * Si el atacante llega a `0` o se detiene: las tropas sobrevivientes regresan al origen.
* **Paso 4 (Éxito):** Habilita `fortificar` y muestra:
  `(Comando correcto) El jugador <nombre_jugador> ha terminado de atacar.`

---

### Fase 3: Fortificación de Posición (`fortificar`)
* **Paso 1:** El usuario teclea `fortificar <nombre_jugador>`.
* **Paso 2 (Validación de secuencia):** Si no ha pasado por `atacar`:
  `(Jugador no ha atacado) El jugador <nombre_jugador> no ha ejecutado el comando atacar.`
* **Paso 3 (Sub-flujo interactivo):**
  * Cierra la fase de ataque (otorga 1 carta si conquistó en el turno y aún no la recibió).
  * Pregunta si desea fortificar (`s/n`).
  * Si responde `s`: lista los territorios aptos (propios con `> 1` tropa y vecino propio), solicita código de origen, lista los **vecinos propios** (desde `Territorio::vecinosPropios`), solicita código de destino y cantidad (`1` a `tropas_origen - 1`).
* **Paso 4 (Éxito y cambio de turno):**
  `(Comando correcto) El jugador <nombre_jugador> ha terminado de fortificar su posición.`
  *El sistema avanza el turno circularmente al siguiente jugador que aún ocupe territorios.*

---

## 3. Flujo de Información y Estado del Juego (`estado_juego`)
**Objetivo:** Consultar en cualquier momento la situación global de la partida.

* **Paso 1:** El usuario teclea `estado_juego`.
* **Validaciones:**
  * Si no está inicializado: `(Juego no inicializado) Esta partida no ha sido inicializada correctamente.`
  * Si terminó: `(Juego terminado) Esta partida ya tuvo un ganador.`
* **Salida:** Número de jugadores, nombres y colores, jugador en turno actual y lista de los 42 territorios con código, nombre, propietario y unidades.

---

## 4. Flujo de Planeación y Estrategia (Componente 3: Grafos)
**Objetivo:** Apoyar la toma de decisiones de un jugador mediante el análisis de rutas de conquista de costo mínimo sobre un grafo dirigido y ponderado (algoritmo de Dijkstra multi-origen).

> **Nota de uso:** Estos comandos son de consulta estratégica (no modifican el tablero, las tropas ni el turno actual) y pueden ejecutarse para cualquier jugador válido de la partida en curso, sea o no su turno.

### 4.1. Costo de Conquista (`costo_conquista`)
* **Paso 1:** El usuario teclea `costo_conquista <nombre_jugador> <territorio>`.
  * `<territorio>` puede escribirse por su **código oficial** (ej. `5.6`) o por su **nombre** (ej. `Kamchatka`, `Gran Bretana`, con o sin tildes/mayúsculas, incluyendo nombres con espacios).
* **Paso 2 (Validaciones en orden):**
  1. Parámetros incorrectos (menos de 2 argumentos, o más de 2 si los tokens del territorio no forman un territorio válido): `Error: Numero de parametros incorrecto.` + ayuda del comando.
  2. Juego no inicializado: `(Juego no inicializado) Esta partida no ha sido inicializada correctamente.`
  3. Juego terminado: `(Juego terminado) Esta partida ya tuvo un ganador.`
  4. Jugador inexistente: `(Jugador no válido) El jugador <nombre_jugador> no forma parte de esta partida.`
  5. Territorio inexistente: `(Territorio no válido) El territorio <territorio> no existe en el tablero.`
  6. Jugador eliminado (0 territorios controlados): `(Jugador sin territorios) El jugador <nombre_jugador> no controla territorios desde los cuales atacar.`
  7. Territorio ya controlado por `<nombre_jugador>`:
     `(Territorio ya controlado) El jugador <nombre_jugador> ya controla el territorio <territorio>. Debe conquistar 0 unidades de ejército.`
* **Paso 3 (Cálculo sobre el grafo):**
  * El sistema construye el grafo dirigido y ponderado del tablero desde la perspectiva de `<nombre_jugador>` (arista hacia territorio enemigo con peso igual a sus unidades; hacia territorio propio con peso 0).
  * Ejecuta Dijkstra multi-origen partiendo desde todos los territorios controlados por `<nombre_jugador>` (distancia 0), minimizando primero el costo total en unidades y desempatando por menor número de saltos (territorios intermedios).
* **Paso 4 (Salida de éxito):**
  * Con territorios intermedios:
    `(Comando correcto) Para conquistar el territorio <territorio>, <nombre_jugador> debe atacar desde <territorio_1>, pasando por los territorios <territorio_2>, <territorio_3>, ..., <territorio_m>. Debe conquistar <n> unidades de ejército.`
  * Sin territorios intermedios (vecino directo):
    `(Comando correcto) Para conquistar el territorio <territorio>, <nombre_jugador> debe atacar desde <territorio_1>. Debe conquistar <n> unidades de ejército.`

### 4.2. Conquista Más Barata (`conquista_mas_barata`)
* **Paso 1:** El usuario teclea `conquista_mas_barata <nombre_jugador>`.
* **Paso 2 (Validaciones en orden):**
  1. Parámetros incorrectos (`args != 1`): `Error: Numero de parametros incorrecto.` + ayuda del comando.
  2. Juego no inicializado: `(Juego no inicializado) Esta partida no ha sido inicializada correctamente.`
  3. Juego terminado: `(Juego terminado) Esta partida ya tuvo un ganador.`
  4. Jugador inexistente: `(Jugador no válido) El jugador <nombre_jugador> no forma parte de esta partida.`
  5. Jugador sin territorios o sin territorios enemigos alcanzables:
     `(Jugador sin territorios) El jugador <nombre_jugador> no tiene conquistas disponibles en este momento.`
* **Paso 3 (Cálculo sobre el grafo):**
  * Construye el grafo ponderado desde el punto de vista de `<nombre_jugador>` y ejecuta Dijkstra multi-origen.
  * Evalúa todos los territorios enemigos alcanzables del tablero y selecciona el de menor costo total en unidades de ejército (desempatando por menor cantidad de saltos).
* **Paso 4 (Salida de éxito):**
  * Con territorios intermedios:
    `(Comando correcto) La conquista más barata es avanzar sobre el territorio <territorio_1> desde el territorio <territorio_2>. Para conquistar el territorio <territorio_1>, debe atacar desde <territorio_2>, pasando por los territorios <territorio_3>, <territorio_4>, ..., <territorio_m>. Debe conquistar <n> unidades de ejército.`
  * Sin territorios intermedios (vecino directo):
    `(Comando correcto) La conquista más barata es avanzar sobre el territorio <territorio_1> desde el territorio <territorio_2>. Para conquistar el territorio <territorio_1>, debe atacar desde <territorio_2>. Debe conquistar <n> unidades de ejército.`

---

## 5. Flujo de Guardado y Cierre de Sesión (Componente 2)
**Objetivo:** Persistir el estado de la partida en texto plano o binario comprimido con Huffman y finalizar la aplicación.

* **Guardado en Texto Plano (`guardar <nombre_archivo>`):**
  * Si no está inicializado: `(Juego no inicializado) Esta partida no ha sido inicializada correctamente.`
  * Éxito: `(Comando correcto) La partida ha sido guardada correctamente.`
  * Error de escritura: `(Error al guardar) La partida no ha sido guardada correctamente.`
* **Guardado Comprimido (`guardar_comprimido <nombre_archivo>`):**
  * Si no está inicializado: `(Juego no inicializado) Esta partida no ha sido inicializada correctamente.`
  * Éxito: `(Comando correcto) La partida ha sido codificada y guardada correctamente.`
  * Error de codificación/escritura: `(Error al codificar y/o guardar) La partida no ha sido codificada ni guardada correctamente.`
* **Cierre del Sistema (`salir`):**
  * Sin parámetros; finaliza el ciclo REPL limpiamente sin imprimir mensajes adicionales.
