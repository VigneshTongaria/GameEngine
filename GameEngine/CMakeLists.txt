cmake_minimum_required(VERSION 3.20)

project(GameEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------------------------
# Source Files
# --------------------------------------------

file(GLOB_RECURSE ENGINE_SRC
    src/*.cpp
    lib/glad.c
    lib/stb.cpp

    Linking/include/imgui/imgui.cpp
    Linking/include/imgui/imgui_demo.cpp
    Linking/include/imgui/imgui_draw.cpp
    Linking/include/imgui/imgui_tables.cpp
    Linking/include/imgui/imgui_widgets.cpp
    Linking/include/imgui/imgui_impl_glfw.cpp
    Linking/include/imgui/imgui_impl_opengl3.cpp
)

add_executable(GameEngine ${ENGINE_SRC})

# --------------------------------------------
# Include Directories
# --------------------------------------------

target_include_directories(GameEngine PRIVATE

    src

    lib

    Linking/include

    Linking/include/glad

    Linking/include/GLFW

    Linking/include/imgui

)

# --------------------------------------------
# Windows Libraries
# --------------------------------------------

if(MSVC)

    target_link_directories(GameEngine PRIVATE

        Linking/lib

        Linking/lib/GLFW

        Linking/lib/assimp

    )

    target_link_libraries(GameEngine

        glfw3

        assimp-vc143-mtd

        opengl32

    )

endif()

# --------------------------------------------
# Emscripten
# --------------------------------------------

if(EMSCRIPTEN)

    target_link_options(GameEngine PRIVATE

        "-sUSE_GLFW=3"

        "-sFULL_ES3=1"

        "-sWASM=1"

        "-sALLOW_MEMORY_GROWTH=1"

        "-sUSE_WEBGL2=1"

    )

    set_target_properties(GameEngine PROPERTIES
        SUFFIX ".html"
    )

endif()

# --------------------------------------------
# Copy Assets
# --------------------------------------------

add_custom_command(TARGET GameEngine POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_directory

    ${CMAKE_SOURCE_DIR}/Assets

    $<TARGET_FILE_DIR:GameEngine>/Assets

)

target_include_directories(GameEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/Linking/include
)

if(EMSCRIPTEN)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

add_subdirectory(external/assimp)

target_link_libraries(GameEngine PRIVATE assimp)

endif()