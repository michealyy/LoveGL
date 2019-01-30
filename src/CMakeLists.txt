cmake_minimum_required(VERSION 3.8.0)

#C++ 17标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#编译成动态库
option(DYNAMIC_LIB "dynamic lib" OFF)

#set(GLFW_DIR "../glfw")

#glew3
#glad
set(GLAD "deps/glad.c")

include_directories("deps")

#源代码文件夹
aux_source_directory(. DIR_SRC)
aux_source_directory(example DIR_SRC_EXAMPLE)
aux_source_directory(ui DIR_SRC_UI)

#添加静态库  
ADD_LIBRARY(renderer STATIC ${GLAD} ${DIR_SRC} ${DIR_SRC_EXAMPLE} ${DIR_SRC_UI})  

#将静态库重新命名为renderer
SET_TARGET_PROPERTIES(renderer PROPERTIES OUTPUT_NAME "renderer")