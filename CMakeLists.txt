cmake_minimum_required(VERSION 3.8.0)

project(LoveGL)

if (MINGW AND WIN32)
    message("MINGW可能不支持filesystem")
endif()

if (MSVC AND WIN32)
    add_definitions(-D _CRT_SECURE_NO_WARNINGS -D _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
endif()

#C++ 17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D GLEW_STATIC)

include_directories("deps")
include_directories("deps/glew/include")
include_directories("deps/glfw/include")
include_directories("lovegl")

add_subdirectory(deps/glew)
add_subdirectory(deps/glfw)
add_subdirectory(lovegl)

####下面是可执行程序
link_libraries(lovegl)

#例子
add_executable(example_pbr example/pbr.cpp)