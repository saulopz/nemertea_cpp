cmake_minimum_required(VERSION 3.21)
project(NemerteaSuite LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- 1. Binary Output ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- 2. Dependencies ---
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
    GIT_SHALLOW TRUE
)
FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
    GIT_TAG v3.3.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(nlohmann_json cxxopts)

find_package(Threads REQUIRED)

# --- 3. Cross-Platform Compilation Flags ---
add_library(project_configs INTERFACE)
target_compile_options(project_configs INTERFACE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
        -O3 -Wall -Wextra -Wpedantic -Wconversion -pthread
    >
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<NOT:$<CXX_COMPILER_ID:AppleClang>>>:
        -march=native
    >
    $<$<CXX_COMPILER_ID:MSVC>:
        /O2 /Oi /Ot /Gy /W4 /permissive- /utf-8
    >
)

# --- 3 Profiling Support (debug symbols + frame pointers alongside -O3) ---
option(ENABLE_PROFILING "Add -g and -fno-omit-frame-pointer for profiling" OFF)
if(ENABLE_PROFILING)
    target_compile_options(project_configs INTERFACE
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>: -g -fno-omit-frame-pointer>
    )
    target_link_options(project_configs INTERFACE
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>: -g>
    )
endif()

# LTO for maximum performance
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)

# --- 4. Common Library (The Foundation) ---
add_library(graph_common STATIC 
    src/common/graph.cpp
)
target_include_directories(graph_common PUBLIC src/common)
target_link_libraries(graph_common PUBLIC nlohmann_json::nlohmann_json project_configs)

# --- 5. Targets (Executables) ---

# Nemertea
file(GLOB_RECURSE NEMERTEA_SRCS src/nemertea/*.cpp)
add_executable(nemertea ${NEMERTEA_SRCS})
target_include_directories(nemertea PRIVATE src/nemertea)
target_link_libraries(nemertea PRIVATE graph_common cxxopts::cxxopts project_configs)

# Backtracking
add_executable(backtracking src/backtracking/main.cpp)
target_link_libraries(backtracking PRIVATE graph_common Threads::Threads project_configs)

# Warnsdorff
add_executable(warnsdorff src/warnsdorff/main.cpp)
target_link_libraries(warnsdorff PRIVATE graph_common Threads::Threads project_configs)

# Apply Link Time Optimization (LTO) if available (disabled for profiling builds
# because LTO replaces DWARF-carrying object files with bitcode, losing symbols).
if(ipo_supported AND NOT ENABLE_PROFILING)
    set_target_properties(graph_common nemertea backtracking warnsdorff
                          PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Generate dSYM bundles on macOS for profiling (samply needs them for symbols).
if(ENABLE_PROFILING AND APPLE)
    foreach(tgt nemertea backtracking warnsdorff)
        add_custom_command(TARGET ${tgt} POST_BUILD
            COMMAND dsymutil $<TARGET_FILE:${tgt}>
            COMMENT "Generating dSYM for ${tgt}")
    endforeach()
endif()