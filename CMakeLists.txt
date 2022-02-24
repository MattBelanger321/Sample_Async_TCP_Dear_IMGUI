cmake_minimum_required(VERSION 3.5)
project(Demo)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Boost 1.78.0 REQUIRED)
find_package(nlohmann_json 3.10.5 REQUIRED)
find_package(asio 1.21.0 REQUIRED)

# CONFIG option is important so that CMake doesnt search for modules into the default modules directory

find_package(imgui CONFIG)
find_package(glfw3 CONFIG)
find_package(glew CONFIG)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${imgui_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${glfw_INCLUDE_DIRS})
include_directories(${nlohmann_json_INCLUDE_DIRS})
include_directories(${asio_INCLUDE_DIRS})

add_executable( Demo
                src/main.cpp
                src/student.h
                src/student.cpp
                src/json_writer.h
                src/json_writer.cpp
                src/json_reader.h
                src/json_reader.cpp
                bindings/imgui_impl_glfw.cpp
                bindings/imgui_impl_opengl3.cpp
                bindings/imgui_impl_glfw.h
                bindings/imgui_impl_opengl3.h
                bindings/imgui_impl_glfw.h
                bindings/imgui_impl_opengl3.h
                   bindings/imgui_impl_win32.h
                bindings/imgui_impl_win32.cpp
                )

set_property(TARGET Demo PROPERTY CXX_STANDARD 17)

target_compile_definitions(Demo PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(Demo imgui::imgui GLEW::glew_s glfw::glfw)
target_link_libraries(Demo ${CONAN_LIBS})