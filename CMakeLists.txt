cmake_minimum_required(VERSION 3.8.0)

project(kdRenderStudio)

if (MINGW AND WIN32)
    message("MINGW可能不支持filesystem")
endif()

#C++ 17标准
set(CMAKE_CXX_STANDARD 17)

option(OPTION_TEST "option test" OFF)

#依赖glfw
add_subdirectory(glfw)
include_directories("glfw/deps")
include_directories("glfw/include")
link_libraries(glfw)

#glad
set(GLAD "glfw/deps/glad/glad.h"
         "glfw/deps/glad.c")

include_directories("deps")

#源代码文件夹
aux_source_directory(src DIR_SRC)
aux_source_directory(src/scene DIR_SRC_SCENE)
aux_source_directory(src/ui DIR_SRC_UI)

add_executable(kdRenderStudio ${GLAD} ${DIR_SRC} ${DIR_SRC_SCENE} ${DIR_SRC_UI})