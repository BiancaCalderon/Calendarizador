cmake_minimum_required(VERSION 3.10)
project(Simulador)

# Configurar el estandar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Habilitar automoc para procesar Q_OBJECT
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Ruta absoluta a examples/
add_compile_definitions(EXAMPLES_DIR="${CMAKE_SOURCE_DIR}/examples")

# Opciones de compilacion
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra)
endif()

# Definir los directorios de codigo fuente
# set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Buscar todos los archivos fuente
# file(GLOB_RECURSE SOURCES
#    "${SRC_DIR}/*.cpp"
# )

# Agregar el directorio de encabezados
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Sólo los fuentes de GUI (excluimos main.cpp de consola)
file(GLOB_RECURSE GUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main_gui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/metrics.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/scheduling/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/synchronization/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/models/*.cpp"
)

# Crear el ejecutable
add_executable(simulador ${GUI_SOURCES})

# Buscar e incluir bibliotecas externas
# Por ejemplo, para Qt, SFML o ImGui, descomente las lu00edneas correspondientes:

# Qt
find_package(Qt5 COMPONENTS Widgets REQUIRED)
target_link_libraries(simulador Qt5::Widgets)

# SFML
# find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
# target_link_libraries(simulador sfml-graphics sfml-window sfml-system)

# ImGui con GLFW y OpenGL
# find_package(glfw3 REQUIRED)
# find_package(OpenGL REQUIRED)
# target_link_libraries(simulador glfw OpenGL::GL ${CMAKE_DL_LIBS})

# Agregar instrucciones de installaciu00f3n si es necesario
install(TARGETS simulador DESTINATION bin)

# Copiar los archivos de ejemplo durante la installaciu00f3n
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/ DESTINATION share/simulador/examples)
