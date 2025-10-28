add_library(backend_lib
    Computador.cpp
    InterfaceDoDispositivo.cpp
    InterfaceDoDispositivo.h
    Pacote.cpp
    Roteador.cpp
    Switch.cpp
    IDispositivo.h
    Computador.h
    Roteador.h
    Switch.h

)

target_include_directories(backend_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
