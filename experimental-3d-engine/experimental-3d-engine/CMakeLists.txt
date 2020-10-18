# CMakeList.txt: проект CMake для experimental-3d-engine; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

include_directories (experimental-3d-engine include include/GL)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (experimental-3d-engine WIN32 "experimental-3d-engine.cpp" "experimental-3d-engine.h")

# TODO: Добавьте тесты и целевые объекты, если это необходимо.

target_link_libraries(experimental-3d-engine opengl32)