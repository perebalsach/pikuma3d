cmake_minimum_required(VERSION 3.10)
project(Pikuma3D C)

set(CMAKE_C_STANDARD 11)

# Source files
file(GLOB SOURCE_FILES src/*.c src/*.h)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Specify different directories for Debug and Release configurations (optional)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release
)

# SDL2 paths
if(WIN32)
    set(SDL2_INCLUDE_DIR "C:/dev/external_libs/SDL2/include")           # Path to SDL2 include directory
    set(SDL2_LIBRARY "C:/dev/external_libs/SDL2/lib/x64/SDL2.lib")      # Full path to SDL2.lib file
    set(SDL2_DLL "C:/dev/external_libs/SDL2/lib/x64/SDL2.dll")
elseif(APPLE)
    set(SDL2_INCLUDE_DIR "/usr/local/include/SDL2")                 # Default path for Homebrew
    set(SDL2_LIBRARY "/usr/local/lib/libSDL2.dylib")                # Default path for Homebrew
endif()

# Add SDL2 include directory
include_directories(${SDL2_INCLUDE_DIR})

# Link SDL2 and SDL2main libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} "C:/dev/external_libs/SDL2/lib/x64/SDL2main.lib" ${SDL2_LIBRARY})
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
endif ()

# Specify subsystem for Windows applications
if(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows)

    # Copy SDL2.dll to the output directory after building
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_DLL}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)

    # Copy the res folder to the output directory after building
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/res
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
endif()
