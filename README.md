# Simulador de Sistemas Operativos

Este proyecto es un simulador visual de algoritmos de planificación de procesos y mecanismos de sincronización desarrollado en C++. El simulador permite visualizar de manera dinámica diferentes algoritmos de calendarización y escenarios de sincronización.

## Características

### Simulador de Algoritmos de Calendarización

- **Algoritmos soportados:**
  - First In First Out (FIFO): Ejecuta los procesos en el orden en que llegan.
  - Shortest Job First (SJF): Ejecuta el proceso con el menor Burst Time (no preemptivo).
  - Shortest Remaining Time (SRT): Ejecuta el proceso con el menor tiempo restante (preemptivo de SJF).
  - Round Robin (con quantum configurable): Asigna a cada proceso un tiempo fijo (quantum) en la CPU de forma rotatoria.
  - Priority: Ejecuta los procesos según su prioridad (menor número = mayor prioridad, no preemptivo).

- **Visualización dinámica:**
  - Diagrama de Gantt que muestra la ejecución de procesos
  - Número de ciclo visible
  - Diferenciación por nombre y color de cada proceso
  - Métricas de eficiencia (Avg Waiting Time, Avg Turnaround Time)

### Simulador de Mecanismos de Sincronización

- **Mecanismos soportados:**
  - Mutex Locks
  - Semáforos

- **Visualización dinámica:**
  - Representación visual de acciones sobre recursos
  - Estados de procesos (ACCESSED o WAITING)
  - Diferenciación visual entre accesos exitosos y esperas

## Estructura del Proyecto

```
.
├── CMakeLists.txt                # Configuración de CMake
├── build/                        # Archivos generados durante la construcción
├── examples/                     # Archivos de ejemplo para pruebas
│   ├── actions/                  # Ejemplos de acciones de sincronización
│   │   ├── ejemplo_conflicto.txt # Ejemplo de conflicto de recursos
│   │   └── ejemplo_sync.txt      # Ejemplo básico de sincronización
│   ├── processes/                # Ejemplos de procesos
│   │   ├── ejemplo_conflicto.txt # Ejemplo de conflicto de procesos
│   │   ├── ejemplo_fifo.txt      # Ejemplo para algoritmo FIFO
│   │   ├── ejemplo_priority.txt  # Ejemplo para algoritmo Priority
│   │   ├── ejemplo_round_robin.txt # Ejemplo para algoritmo Round Robin
│   │   ├── ejemplo_sjf.txt       # Ejemplo para algoritmo SJF
│   │   ├── ejemplo_srt.txt       # Ejemplo para algoritmo SRT
│   │   └── ejemplo_sync.txt      # Ejemplo para sincronización
│   └── resources/                # Ejemplos de recursos
│       ├── ejemplo_conflicto.txt # Ejemplo de conflicto de recursos
│       └── ejemplo_sync.txt      # Ejemplo básico de recursos
└── src/                          # Código fuente del proyecto
    ├── gui/                      # Componentes de la interfaz gráfica
    │   ├── algorithm_config_widget.cpp # Widget de configuración de algoritmos
    │   ├── algorithm_config_widget.h
    │   ├── control_panel_widget.cpp    # Panel de control de simulación
    │   ├── control_panel_widget.h
    │   ├── file_loader_widget.cpp      # Widget para cargar archivos
    │   ├── file_loader_widget.h
    │   ├── gantt_chart_view.cpp        # Vista del diagrama de Gantt
    │   ├── gantt_chart_view.h
    │   ├── gantt_chart_widget.cpp      # Widget del diagrama de Gantt
    │   ├── gantt_chart_widget.h
    │   ├── main_window.cpp             # Ventana principal de la aplicación
    │   ├── main_window.h
    │   ├── metrics_panel_widget.cpp    # Panel de métricas
    │   ├── metrics_panel_widget.h
    │   ├── scheduling_view.cpp         # Vista de calendarización
    │   ├── scheduling_view.h
    │   ├── sync_view.cpp              # Vista de sincronización
    │   └── sync_view.h
    ├── main.cpp                        # Punto de entrada principal
    ├── main_gui.cpp                    # Inicialización de la GUI
    ├── models/                         # Modelos de datos
    │   ├── action.cpp                  # Modelo de acción
    │   ├── action.h
    │   ├── process.cpp                 # Modelo de proceso
    │   ├── process.h
    │   ├── resource.cpp                # Modelo de recurso
    │   └── resource.h
    ├── scheduling/                     # Algoritmos de calendarización
    │   ├── fifo.cpp                    # First In First Out
    │   ├── fifo.h
    │   ├── priority.cpp                # Priority Scheduling
    │   ├── priority.h
    │   ├── round_robin.cpp             # Round Robin
    │   ├── round_robin.h
    │   ├── scheduler.cpp               # Clase base para schedulers
    │   ├── scheduler.h
    │   ├── sjf.cpp                     # Shortest Job First
    │   ├── sjf.h
    │   ├── srt.cpp                     # Shortest Remaining Time
    │   └── srt.h
    ├── synchronization/                # Mecanismos de sincronización
    │   ├── mutex.cpp                   # Mutex Locks
    │   ├── mutex.h
    │   ├── semaphore.cpp               # Semáforos
    │   ├── semaphore.h
    │   ├── sync_mechanism.cpp          # Clase base para mecanismos
    │   ├── sync_mechanism.h
    │   ├── sync_sim.cpp                # Simulador de sincronización
    │   └── sync_sim.h
    └── utils/                          # Utilidades
        ├── file_loader.cpp             # Cargador de archivos
        ├── file_loader.h
        ├── metrics.cpp                 # Cálculo de métricas
        └── metrics.h
```

## Requisitos

- C++17 o superior
- **Biblioteca gráfica Qt5:** Este proyecto utiliza la biblioteca Qt5 para la interfaz gráfica.
- CMake 3.10 o superior

## Instalación

1.  **Actualizar el sistema e instalar dependencias (Sistemas basados en Debian/Ubuntu):**
    ```bash
    sudo apt update
    sudo apt install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
    ```
    *(Si utilizas otro sistema operativo o distribución, consulta la documentación de Qt para instalar las herramientas de desarrollo de Qt5.)*

2.  **Clonar el repositorio:**
    ```bash
    git clone https://github.com/usuario/calendarizador.git
    cd calendarizador
    ```

3.  **Crear y acceder al directorio de compilación:**
    ```bash
    mkdir build
    cd build
    ```

4.  **Configurar y compilar el proyecto:**
    ```bash
    cmake ..
    make
    ```

## Formato de archivos de entrada

### Procesos (para ambos simuladores)
```
<PID>, <BT>, <AT>, <Priority>
Ejemplo: P1, 8, 0, 1
```
- PID: Identificador del proceso
- BT: Burst Time (tiempo de ejecución)
- AT: Arrival Time (tiempo de llegada)
- Priority: Prioridad (número menor indica mayor prioridad)

### Recursos (para simulador de sincronización)
```
<NOMBRE_RECURSO>, <CONTADOR>
Ejemplo: R1, 1
```
- NOMBRE_RECURSO: Identificador del recurso
- CONTADOR: Cantidad de instancias disponibles

### Acciones (para simulador de sincronización)
```
<PID>, <ACCION>, <RECURSO>, <CICLO>
Ejemplo: P1, READ, R1, 0
```
- PID: Identificador del proceso
- ACCION: Tipo de operación (READ/WRITE)
- RECURSO: Identificador del recurso
- CICLO: Ciclo en el que se realiza la acción

## Uso

1. Ejecutar la aplicación:
   ```bash
   ./simulador
   ```

2. Seleccionar tipo de simulación (Calendarización o Sincronización)

3. Cargar archivos de procesos, recursos y acciones según corresponda

4. Configurar parámetros (algoritmo, quantum, etc.)

5. Ejecutar la simulación

## Autores

- Bianca Calderón
- Daniel Dubon
- Andrés Ortega
