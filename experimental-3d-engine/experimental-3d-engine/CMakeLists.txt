# CMakeList.txt: проект CMake для experimental-3d-engine; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)

include_directories (experimental-3d-engine "include" "include/GL" "glfw/include")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (experimental-3d-engine "experimental-3d-engine.cpp" "experimental-3d-engine.h")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

target_link_libraries(experimental-3d-engine opengl32 glfw)