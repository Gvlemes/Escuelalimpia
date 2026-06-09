cmake_minimum_required(VERSION 3.16)
project(EscuelaLimpiaProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Qml Quick Sql)

qt_add_executable(escuelamovil
    main.cpp
)

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

# Instrucción obligatoria para que el empaquetador inyecte el manifiesto corregido 👇
set_property(TARGET escuelamovil PROPERTY
    QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
)
