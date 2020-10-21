cmake_minimum_required (VERSION 3.8)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

include_directories (experimental-3d-engine
    include
    include/GL
    glfw/include
    imgui
)

add_executable (experimental-3d-engine
    experimental-3d-engine.cpp
    experimental-3d-engine.h

    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_glfw.cpp
)

target_link_libraries(experimental-3d-engine opengl32 glfw)