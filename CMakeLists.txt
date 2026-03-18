cmake_minimum_required(VERSION 3.21)
project(nemertea LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gera compile_commands.json para clangd/LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Dependências ────────────────────────────────────────────────────────────
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
    GIT_SHALLOW    TRUE   # clona só o tag, sem histórico completo
)
FetchContent_MakeAvailable(nlohmann_json)

# ── Fontes ──────────────────────────────────────────────────────────────────
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(nemertea ${SOURCES})

target_include_directories(nemertea PRIVATE src)

target_link_libraries(nemertea PRIVATE nlohmann_json::nlohmann_json)

# ── Flags de compilação ─────────────────────────────────────────────────────
target_compile_options(nemertea PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
        -Wall -Wextra -Wpedantic -Wconversion
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4
    >
)