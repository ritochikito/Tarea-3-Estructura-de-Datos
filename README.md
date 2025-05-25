# GraphQuest

GraphQuest es un juego de laberinto basado en grafos desarrollado en C. En este juego, el jugador recorre escenarios conectados, cada uno representado como un nodo del grafo, donde se presentan decisiones estratégicas y elementos coleccionables. El objetivo es partir desde la **Entrada principal** y llegar a un escenario final, maximizando el puntaje obtenido por los ítems recogidos antes de que se agote el tiempo.

## Tabla de Contenidos

- [GraphQuest](#graphquest)
  - [Tabla de Contenidos](#tabla-de-contenidos)
  - [Requisitos](#requisitos)
  - [Compilación y Ejecución](#compilación-y-ejecución)
  - [Descripción del Juego](#descripción-del-juego)
    - [Menú Principal](#menú-principal)
    - [Menú del Juego](#menú-del-juego)
  - [Ejemplo de Interacción](#ejemplo-de-interacción)
  - [Estado de las Funcionalidades](#estado-de-las-funcionalidades)
  - [Contribuciones](#contribuciones)
  - [Conclusión](#conclusión)

## Requisitos

- **Lenguaje de Programación:** C
- **Compilador:** GCC o cualquier compilador estándar de C
- **Librerías y Herramientas:**
  - [Graphviz](https://graphviz.org/): Utilizado para generar visualmente el grafo del laberinto. Asegúrate de tener instalado `dot` o el paquete correspondiente.
  - Funciones estándar de C para entrada/salida y manejo de archivos (para procesar el CSV).
- **Sistema Operativo:** Compatible con Windows (MinGW o Visual Studio), Linux y macOS.

## Compilación y Ejecución

1. **Descarga o clona el repositorio.**

2. **Compila el código fuente:**

   Navega a la carpeta del proyecto y ejecuta:

   ```bash
   gcc graphquest.c -o graphquest
   ```

   Si el proyecto está dividido en varios archivos fuente, puede usarse:

   ```bash
   gcc *.c -o graphquest
   ```

3. **Ejecuta la aplicación:**

   Asegúrate de tener el archivo `graphquest.csv` en la misma carpeta (o especifica la ruta correcta) y ejecuta:

   ```bash
   ./graphquest
   ```

   En Windows, el ejecutable será `graphquest.exe`.

## Descripción del Juego

GraphQuest se basa en recorrer un laberinto generado a partir de un grafo. Cada nodo del grafo representa un escenario con decisiones y elementos interactivos.

### Menú Principal

- **Cargar Laberinto desde Archivo CSV:**  
  El juego lee un archivo CSV (`graphquest.csv`) que describe cada escenario: su descripción, las decisiones (direcciones posibles), los ítems disponibles (nombre, peso, valor) y las conexiones entre escenarios. Con estos datos se construye de manera explícita el grafo mediante Graphviz.

- **Iniciar Partida:**  
  Con el laberinto cargado, se inicia la partida desde el escenario inicial. Se despliega el estado del juego y se presenta el menú de opciones para interactuar.

### Menú del Juego

Cada actualización de escenario o acción ejecutada muestra el siguiente menú:

- **Estado Actual:**

  - **Descripción del escenario:** Detalle del lugar en el que se encuentra el jugador.
  - **Ítems disponibles:** Lista de ítems presentes en el escenario (nombre, peso, valor).
  - **Tiempo restante:** Inicialmente, se define un tiempo (por ejemplo, 10 unidades) que se va descontando conforme se realizan acciones.
  - **Inventario del jugador:** Ítems recogidos, peso total acumulado y puntaje.
  - **Direcciones disponibles:** Opciones de movimiento (por ejemplo, arriba, abajo, izquierda, derecha).

- **Opciones del Jugador:**
  - **Recoger Ítem(s):**  
    Permite seleccionar uno o más ítems para agregarlos al inventario. Esta acción descuenta 1 unidad de tiempo.
  - **Descartar Ítem(s):**  
    Permite eliminar ítems del inventario para reducir el peso y facilitar el avance, consumiendo también 1 unidad de tiempo.
  - **Avanzar en una Dirección:**  
    El jugador elige una dirección válida. Al avanzar:
    - Se actualiza el escenario actual.
    - Se conserva el inventario.
    - Se descuenta el tiempo utilizado según la fórmula:
      \[
      T = \left\lceil\frac{\text{Peso Total del Inventario} + 1}{10}\right\rceil
      \]
    - Si se alcanza el escenario final, se muestra el inventario completo y el puntaje.
    - Si el tiempo se agota, se muestra un mensaje de derrota.
  - **Reiniciar Partida:**  
    Reinicia el juego desde la entrada principal, reiniciando el tiempo y vaciando el inventario.
  - **Salir del Juego:**  
    Finaliza la aplicación.

## Ejemplo de Interacción

1. **Inicio del Juego:**  
   Al ejecutar el programa (`./graphquest` o `graphquest.exe`), se muestra el menú principal con las opciones:

   - Cargar Laberinto desde Archivo CSV.
   - Iniciar Partida.

2. **Carga del Laberinto:**  
   Selecciona la opción para cargar el laberinto. El programa procesa el archivo `graphquest.csv` y utiliza Graphviz para generar el grafo que representa el laberinto.

3. **Inicio de la Partida:**  
   Al iniciar la partida, el juego comienza en el escenario de la "Entrada principal". Se muestra:

   - La descripción del escenario.
   - Los ítems disponibles.
   - El tiempo restante (por ejemplo, 10 unidades).
   - El inventario inicial (vacío).
   - Las direcciones válidas para avanzar.

4. **Ejemplo de Acción:**
   - Para **recoger un ítem**, el jugador selecciona la opción correspondiente; el juego descuenta 1 unidad de tiempo.
   - Para **avanzar**, elige una dirección válida. El tiempo se descuenta en función del peso del inventario, conforme a la fórmula indicada.  
     El proceso continúa hasta que el jugador alcanza el escenario final o se agota el tiempo.

## Estado de las Funcionalidades

- **Carga del Laberinto:**  
  Funciona correctamente al leer y procesar el archivo CSV.  
  _Posibles inconvenientes:_ Errores en el formato del CSV o rutas incorrectas pueden impedir la correcta lectura de los datos.

- **Construcción del Grafo:**  
  El grafo se genera exitosamente mediante Graphviz.  
  _Causas posibles de fallo:_ Datos incompletos o errores en las conexiones descritas en el CSV.

- **Interacción y Acciones del Jugador:**  
  Las funciones de recoger, descartar y avanzar se ejecutan descontando el tiempo conforme a la fórmula.  
  _Áreas de mejora:_ Mayor robustez en la validación de entrada por parte del jugador y el manejo de direcciones no válidas.

- **Reinicio y Salida:**  
  Ambas opciones operan de forma estable, permitiendo reiniciar la partida o finalizar el juego sin inconvenientes.

## Contribuciones

- **Integrante 1:**

  - Desarrollo del módulo de lectura y parseo del archivo CSV en C.
  - Implementación de la construcción del grafo explícito utilizando Graphviz.
  - Diseño y desarrollo del menú principal.

- **Integrante 2:**
  - Desarrollo de la lógica para la interacción del juego, incluyendo el manejo del inventario y el cálculo del tiempo consumido.
  - Implementación del menú de juego y la administración de las acciones del jugador (recoger/descartar ítems, avanzar).
  - Redacción de la documentación, ejemplos de uso, y descripción del proceso de compilación y ejecución.

## Conclusión

GraphQuest en C combina la estrategia y la gestión de recursos en un desafiante laberinto. Los jugadores deben equilibrar la toma de decisiones en la recolección de ítems y la rapidez en el avance para maximizar su puntaje antes de que se agote el tiempo. Si bien el juego cumple con las funcionalidades básicas, se vislumbran oportunidades de mejora en aspectos como la validación de entradas y la complejidad de los escenarios.

¡Explora GraphQuest, afronta el reto y demuestra tu habilidad para tomar decisiones estratégicas en un entorno lleno de desafíos!
