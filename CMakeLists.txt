cmake_minimum_required(VERSION 3.16)
project(EscuelaLimpia VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Qml Quick)

add_executable(EscuelaLimpia
    main.cpp
)

target_link_libraries(EscuelaLimpia PRIVATE 
    Qt6::Qml
    Qt6::Quick
)
