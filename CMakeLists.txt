# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)
project(Interrup_deboun C CXX ASM)

# Inicialize o SDK do Raspberry Pi Pico
pico_sdk_init()

# Adicione o executável com o nome atualizado
add_executable(Interrup_deboun Interrup_deboun.c)

# Crie o diretório para arquivos gerados
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)

# Gere o cabeçalho PIO, se necessário. Se o arquivo .pio for necessário, mantenha ou ajuste o nome.
pico_generate_pio_header(Interrup_deboun ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

# Se não houver mais o arquivo ws2812.c, remova esta linha
target_sources(Interrup_deboun PRIVATE Interrup_deboun.c)

# Vincule as bibliotecas padrão e hardware_pio, se necessário
target_link_libraries(Interrup_deboun PRIVATE pico_stdlib hardware_pio)

# Gere os arquivos de saída adicionais
pico_add_extra_outputs(Interrup_deboun)



