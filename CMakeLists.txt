cmake_minimum_required(VERSION 3.16)
project(EscuelaLimpiaProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Qml Quick)

# Macro oficial de Qt6 que gestiona los ejecutables móviles sin colisión de directorios
qt_add_executable(appecologia
    main.cpp
)

qt_add_qml_module(appecologia
    URI appecologia
    VERSION 1.0
    QML_FILES
        main.qml
)

target_link_libraries(appecologia PRIVATE 
    Qt6::Qml
    Qt6::Quick
)
