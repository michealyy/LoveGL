cmake_minimum_required(VERSION 3.8.0)

project(LoveGL)

if (MINGW AND WIN32)
    message("MINGW可能不支持filesystem")
endif()

#C++ 17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(glfw)
#lovegl为单独库
add_subdirectory(src)

#例子
include_directories("glfw/include")
include_directories("glfw/deps")
include_directories("deps")
include_directories("src")
link_libraries(lovegl)
aux_source_directory(example DIR_SRC_EXAMPLE)
add_executable(loveglexample ${DIR_SRC_EXAMPLE})