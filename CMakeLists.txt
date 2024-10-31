cmake_minimum_required(VERSION 3.10)
project(Pikuma3D C)

set(CMAKE_C_STANDARD 11)

# Source files
set(SOURCES
        src/main.c
        # other source files
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# SDL2 paths
if(WIN32)
    set(SDL2_INCLUDE_DIR "W:/external_libs/SDL2/include")           # Path to SDL2 include directory
    set(SDL2_LIBRARY "W:/external_libs/SDL2/lib/x64/SDL2.lib")      # Full path to SDL2.lib file
elseif(APPLE)
    set(SDL2_INCLUDE_DIR "/usr/local/include/SDL2")                 # Default path for Homebrew
    set(SDL2_LIBRARY "/usr/local/lib/libSDL2.dylib")                # Default path for Homebrew
endif()

# Add SDL2 include directory
include_directories(${SDL2_INCLUDE_DIR})

# Link SDL2 and SDL2main libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} "W:/external_libs/SDL2/lib/x64/SDL2main.lib" ${SDL2_LIBRARY})
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
endif ()

# Specify subsystem for Windows applications
if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
endif()
