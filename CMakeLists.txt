cmake_minimum_required(VERSION 3.16)
project(RedeDePacotes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Adiciona o backend
add_subdirectory(backend)

add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    mainwindow.h


)

target_link_libraries(${PROJECT_NAME}
    Qt6::Widgets
    backend_lib
)

qt_add_resources(RESOURCES resources.qrc)
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCES})


