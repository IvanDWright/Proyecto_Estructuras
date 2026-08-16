# Requisitos Entrega 0 - Proyecto de Estructuras de Datos (Risk)

Basado en el documento del proyecto (secciones 3.1 y 2.4), estos son los requisitos exclusivos y completos para la **Entrega 0 (Semana 3)**. Esta entrega se centra únicamente en la interfaz de usuario y la validación de comandos.

## 1. Interfaz de Consola Interactiva
- El programa debe funcionar como una **consola interactiva** basada en texto (similar a una terminal del sistema operativo).
- El indicador de línea de comando (prompt) debe ser obligatoriamente el caracter **`$`** acompañado de un espacio.
- El sistema debe quedarse a la espera de recibir los comandos tecleados por el usuario.
- Todos los comandos correspondientes a los diferentes componentes deben estar **ensamblados en un único sistema** (un mismo programa integrado, no como programas independientes).

## 2. Sistema de Ayuda
- **Comando `ayuda`**: Al teclear este comando sin parámetros, el sistema debe mostrar en pantalla una lista de todos los comandos disponibles en el proyecto.
- **Comando `ayuda <comando>`**: Al teclear `ayuda` seguido del nombre de otro comando válido, el sistema debe mostrar la forma correcta de hacer el llamado (su modo de uso, cantidad de parámetros y descripción).

## 3. Validación de Comandos y Parámetros
El sistema debe leer la entrada del usuario y realizar las siguientes validaciones **antes** de ejecutar cualquier lógica:
- **Comando válido**: Verificar que la primera palabra ingresada corresponda a uno de los comandos permitidos.
- **Cantidad de parámetros**: Verificar que el número de argumentos ingresados corresponda exactamente a los definidos para dicho comando.
- **Formato de los parámetros**: Validar el formato de los parámetros cuando sea necesario.

### Lista de comandos cuyas entradas y parámetros deben ser validados:
1. `inicializar <archivo_inicio>` (puede ser `.txt` o comprimido `.bin`)
2. `obtener_unidades <nombre_jugador>`
3. `atacar <nombre_jugador>`
4. `fortificar <nombre_jugador>`
5. `estado_juego`
6. `salir`
7. `guardar <nombre_archivo>`
8. `guardar_comprimido <nombre_archivo>`
9. `costo_conquista <nombre_jugador> <territorio>`
10. `conquista_mas_barata <nombre_jugador>`
11. `ayuda`
12. `ayuda <comando>`

## 4. Mensajes del Sistema
- Los comandos deben presentar en pantalla los **mensajes de resultado (éxito o error)**. Aunque la lógica del juego aún no esté implementada, si los parámetros son erróneos (en cantidad o formato), el sistema debe reportarlo.
- El sistema debe informar al usuario cuándo terminó el comando su procesamiento y el resultado de este.
- **Importante:** Todos los mensajes de éxito o error que se implementen deben seguir estrictamente el formato exacto de salida especificado en el enunciado para cada comando (ej. `(Comando correcto) ...`, `(Error al guardar) ...`).

## 5. Condiciones de Entrega y Evaluación (Reglas Generales)
- Archivo comprimido en formato **`.zip`**.
- El archivo ZIP debe contener **un mismo y único directorio** (sin estructura de carpetas internas).
- Tipos de archivos permitidos dentro del directorio:
  - Documentos: únicamente formato **`.pdf`**.
  - Código fuente C++: extensiones **`.h`**, **`.hxx`**, **`.cxx`**, **`.cpp`**.
- **Nota:** Si no se cumplen estas reglas de empaquetado (otros formatos o carpetas internas), la entrega será descartada con nota de 0.0.
