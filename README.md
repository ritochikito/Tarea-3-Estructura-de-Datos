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

## Requisitos

- **Lenguaje de Programación:** C
- **Compilador:** GCC o cualquier compilador estándar de C
- **Sistema Operativo:** Compatible con Windows (MinGW o Visual Studio), Linux y macOS.

## Compilación y Ejecución

1. **Descarga o clona el repositorio.**

2. **Compila el código fuente:**

   Navega a la carpeta del proyecto y ejecuta:

   ```bash
   gcc main.c -o graphquest
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
    Permite eliminar ítems del inventario para reducir el peso y facilitar el avance, consumiendo también 1 unidad de tiempo. Al descartar un ítem, este queda en la habitación donde fue descartado.
  - **Avanzar en una Dirección:**  
    El jugador elige una dirección válida. Al avanzar:
    - Se actualiza el escenario actual.
    - Se conserva el inventario.
    - Se descuenta el tiempo según el pesos total de los ítems de tu inventario.
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

   ![Grafico del archivo graphquest.csv](https://file.notion.so/f/f/4f8bebe4-a843-44d2-b6ee-51e2006a90d1/b66b234d-5152-4626-aa1f-b79623e418fc/graphviz.png?table=block&id=1f1d965d-c59e-8035-9afb-e3b9348f5dfd&spaceId=4f8bebe4-a843-44d2-b6ee-51e2006a90d1&expirationTimestamp=1748253600000&signature=A2zQBIZuL6Gxup-c8nBgl0h3JauE-osltmFWFbnDMQo&downloadName=graphviz.png)

   Grafico del archivo graphquest.csv de ejemplo generado por [Graphviz](<https://dreampuf.github.io/GraphvizOnline/?engine=dot#graph%20Laberinto%20%7B%0D%0A%20%20%20%20node%20%5Bshape%3Dbox%2C%20width%3D2%2C%20height%3D1%5D%3B%0D%0A%0D%0A%20%20%20%20%2F%2F%20Primera%20fila%0D%0A%20%20%20%20%7B%20rank%3Dsame%3B%20N1%20%5Blabel%3D%221%3A%20Entrada%20principal%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N2%20%5Blabel%3D%222%3A%20Librer%C3%ADa%5CnLibro%20antiguo%20(6%20pts%2C%202%20kg)%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N3%20%5Blabel%3D%223%3A%20Caverna%20oscura%5CnLinterna%20(15%20pts%2C%205%20kg)%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N4%20%5Blabel%3D%224%3A%20Estudio%22%5D%3B%20%7D%0D%0A%0D%0A%20%20%20%20%2F%2F%20Segunda%20fila%0D%0A%20%20%20%20%7B%20rank%3Dsame%3B%20N5%20%5Blabel%3D%225%3A%20Cocina%5CnCuchillo%20(3%20pts%2C%201%20kg)%5CnPan%20(2%20pts%2C%201%20kg)%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N6%20%5Blabel%3D%226%3A%20Comedor%5CnCopa%20dorada%20(8%20pts%2C%203%20kg)%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N7%20%5Blabel%3D%227%3A%20S%C3%B3tano%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N8%20%5Blabel%3D%228%3A%20Taller%5CnMartillo%20(5%20pts%2C%203%20kg)%5CnClavos%20(1%20pt%2C%201%20kg)%22%5D%3B%20%7D%0D%0A%0D%0A%20%20%20%20%2F%2F%20Tercera%20fila%0D%0A%20%20%20%20%7B%20rank%3Dsame%3B%20N9%20%5Blabel%3D%229%3A%20Jard%C3%ADn%5CnMoneda%20(2%20pts%2C%201%20kg)%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N10%20%5Blabel%3D%2210%3A%20Pasillo%5CnLlave%20(5%20pts%2C%201%20kg)%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N11%20%5Blabel%3D%2211%3A%20Sala%20del%20tesoro%5CnCorona%20(50%20pts%2C%2010%20kg)%5CnCollar%20(20%20pts%2C%204%20kg)%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N12%20%5Blabel%3D%2212%3A%20Almac%C3%A9n%22%5D%3B%20%7D%0D%0A%0D%0A%20%20%20%20%2F%2F%20Cuarta%20fila%0D%0A%20%20%20%20%7B%20rank%3Dsame%3B%20N13%20%5Blabel%3D%2213%3A%20Dormitorio%5CnRub%C3%AD%20(20%20pts%2C%205%20kg)%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N14%20%5Blabel%3D%2214%3A%20Corredor%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N15%20%5Blabel%3D%2215%3A%20C%C3%A1mara%20secreta%5CnMapa%20(10%20pts%2C%201%20kg)%22%5D%3B%0D%0A%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20%20N16%20%5Blabel%3D%2216%3A%20Salida%22%5D%3B%20%7D%0D%0A%0D%0A%20%20%20%20%2F%2F%20Conexiones%20horizontales%20(izq%20--%20der)%0D%0A%0D%0A%20%20%20%20N2%20--%20N3%3B%20N3%20--%20N4%3B%0D%0A%20%20%20%20N5%20--%20N6%3B%20N6%20--%20N7%3B%20%0D%0A%20%20%20%20N10%20--%20N11%3B%20N11--N7%0D%0A%20%20%20%20N13%20--%20N14%3B%20N14%20--%20N15%3B%20N15%20--%20N16%3B%0D%0A%0D%0A%20%20%20%20%2F%2F%20Conexiones%20verticales%20(arriba%20--%20abajo)%0D%0A%20%20%20%20N1%20--%20N5%3B%20N2%20--%20N6%3B%20N3%20--%20N7%3B%20N4%20--%20N8%3B%0D%0A%20%20%20%20N5%20--%20N9%3B%20N6%20--%20N10%3B%20N8%20--%20N12%3B%0D%0A%20%20%20%20N9%20--%20N13%3B%20N10%20--%20N14%3B%20N12%20--%20N16%3B%0D%0A%7D%0D%0A>)

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

- **Uso y Acciones del Jugador:**  
  Las funciones de recoger, descartar y avanzar se ejecutan descontando el tiempo conforme a la fórmula.  
  _Áreas de mejora:_ Mayor robustez en la validación de entrada por parte del jugador y el manejo de direcciones no válidas.

- **Reinicio y Salida:**  
  Ambas opciones operan de forma estable, permitiendo reiniciar la partida o finalizar el juego sin inconvenientes.
