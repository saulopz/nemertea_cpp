cmake_minimum_required(VERSION 3.21)
project(NemerteaSuite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- 1. Saída dos Binários ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- 2. Dependências (JSON) ---
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

find_package(Threads REQUIRED)

# --- 3. Flags de Compilação Multilateral ---
add_library(project_configs INTERFACE)
target_compile_options(project_configs INTERFACE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
        -O3 -march=native -Wall -Wextra -Wpedantic -Wconversion -pthread
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /O2 /Oi /Ot /Gy /W4 /permissive- /utf-8
    >
)

# LTO para performance máxima
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)

# --- 4. Biblioteca Comum (A Base de Tudo) ---
add_library(graph_common STATIC 
    src/common/graph.cpp
)
target_include_directories(graph_common PUBLIC src/common)
target_link_libraries(graph_common PUBLIC nlohmann_json::nlohmann_json project_configs)

# --- 5. Os Alvos (Executáveis) ---

# Nemertea
file(GLOB_RECURSE NEMERTEA_SRCS src/nemertea/*.cpp)
add_executable(nemertea ${NEMERTEA_SRCS})
target_include_directories(nemertea PRIVATE src/nemertea)
target_link_libraries(nemertea PRIVATE graph_common project_configs)

# Backtracking
add_executable(backtracking src/backtracking/main.cpp)
target_link_libraries(backtracking PRIVATE graph_common Threads::Threads project_configs)

# Warnsdorff
add_executable(warnsdorff src/warnsdorff/main.cpp)
target_link_libraries(warnsdorff PRIVATE graph_common Threads::Threads project_configs)

# Aplica LTO se disponível
if(ipo_supported)
    set_target_properties(graph_common nemertea backtracking warnsdorff 
                          PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()