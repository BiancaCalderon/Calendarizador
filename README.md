# Simulador de Sistemas Operativos

Este proyecto es un simulador visual de algoritmos de planificación de procesos y mecanismos de sincronización desarrollado en C++. El simulador permite visualizar de manera dinámica diferentes algoritmos de calendarización y escenarios de sincronización.

## Características

### Simulador de Algoritmos de Calendarización

- **Algoritmos soportados:**
  - First In First Out (FIFO)
  - Shortest Job First (SJF)
  - Shortest Remaining Time (SRT)
  - Round Robin (con quantum configurable)
  - Priority

- **Visualización dinámica:**
  - Diagrama de Gantt que muestra la ejecución de procesos
  - Número de ciclo visible
  - Diferenciación por nombre y color de cada proceso
  - Métricas de eficiencia (Avg Waiting Time)

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
simulador/
├── src/
│   ├── main.cpp                  # Punto de entrada principal
│   ├── gui/                      # Interfaz gráfica
│   │   ├── main_window.cpp       # Ventana principal 
│   │   ├── main_window.h
│   │   ├── scheduling_view.cpp   # Vista para calendarización
│   │   ├── scheduling_view.h
│   │   ├── sync_view.cpp         # Vista para sincronización
│   │   └── sync_view.h
│   ├── scheduling/               # Algoritmos de calendarización
│   │   ├── scheduler.cpp         # Clase base para schedulers
│   │   ├── scheduler.h
│   │   ├── fifo.cpp              # First In First Out
│   │   ├── fifo.h
│   │   ├── sjf.cpp               # Shortest Job First
│   │   ├── sjf.h
│   │   ├── srt.cpp               # Shortest Remaining Time
│   │   ├── srt.h
│   │   ├── round_robin.cpp       # Round Robin
│   │   ├── round_robin.h
│   │   ├── priority.cpp          # Priority
│   │   └── priority.h
│   ├── synchronization/          # Mecanismos de sincronización
│   │   ├── sync_mechanism.cpp    # Clase base para mecanismos
│   │   ├── sync_mechanism.h  
│   │   ├── mutex.cpp             # Mutex Locks
│   │   ├── mutex.h
│   │   ├── semaphore.cpp         # Semáforos
│   │   └── semaphore.h
│   ├── models/                   # Modelos de datos
│   │   ├── process.cpp           # Modelo para procesos
│   │   ├── process.h
│   │   ├── resource.cpp          # Modelo para recursos
│   │   ├── resource.h
│   │   ├── action.cpp            # Modelo para acciones
│   │   └── action.h
│   └── utils/                    # Utilidades
│       ├── file_loader.cpp       # Carga de archivos
│       ├── file_loader.h
│       ├── metrics.cpp           # Cálculo de métricas
│       └── metrics.h
├── include/                      # Librerías externas
├── tests/                        # Pruebas unitarias
├── examples/                     # Archivos de ejemplo
│   ├── processes/                # Ejemplos de procesos
│   ├── resources/                # Ejemplos de recursos
│   └── actions/                  # Ejemplos de acciones
├── build/                        # Archivos de construcción
└── CMakeLists.txt                # Configuración de CMake
```

## Requisitos

- C++17 o superior
- Biblioteca gráfica (Qt/SFML/ImGui)
- CMake 3.10 o superior

## Instalación

1. Clonar el repositorio:
   ```bash
   git clone https://github.com/usuario/calendarizador.git
   cd calendarizador
   ```

2. Crear directorio de compilación:
   ```bash
   mkdir build && cd build
   ```

3. Configurar y compilar:
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
