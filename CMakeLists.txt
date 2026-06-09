cmake_minimum_required(VERSION 3.16)
project(EscuelaLimpiaProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Qml Quick Sql)

# Creamos el ejecutable móvil nativo
qt_add_executable(escuelamovil
    main.cpp
)

# Mapeamos el archivo de interfaz táctil
qt_add_qml_module(escuelamovil
    URI escuelamovil
    VERSION 1.0
    QML_FILES
        main.qml
)

target_link_libraries(escuelamovil PRIVATE 
    Qt6::Qml
    Qt6::Quick
    Qt6::Sql
)

# INYECCIÓN CRUCIAL: Forzamos a Qt a leer la ruta del SDK de los servidores de GitHub de forma interna
if(DEFINED ENV{ANDROID_HOME})
    set(QT_ANDROID_SDK_ROOT "$ENV{ANDROID_HOME}" CACHE STRING "Android SDK Root")
    set(ANDROID_SDK_ROOT "$ENV{ANDROID_HOME}" CACHE STRING "Android SDK Root")
endif()

# Configura automáticamente el paquete de Android (Manifiesto de permisos)
set_property(TARGET escuelamovil PROPERTY
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
)
