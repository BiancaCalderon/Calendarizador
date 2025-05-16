cmake_minimum_required(VERSION 3.10)
project(Simulador)

# Configurar el estu00e1ndar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Opciones de compilaciu00f3n
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra)
endif()

# Definir los directorios de cu00f3digo fuente
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Buscar todos los archivos fuente
file(GLOB_RECURSE SOURCES
    "${SRC_DIR}/*.cpp"
)

# Agregar el directorio de encabezados
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Crear el ejecutable
add_executable(simulador ${SOURCES})

# Buscar e incluir bibliotecas externas
# Por ejemplo, para Qt, SFML o ImGui, descomente las lu00edneas correspondientes:

# Qt
# find_package(Qt5 COMPONENTS Widgets REQUIRED)
# target_link_libraries(simulador Qt5::Widgets)

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
