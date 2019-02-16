#是否编译成动态库
option(DYNAMIC_LIB "dynamic lib" OFF)

include_directories("../glfw/deps")
include_directories("../glfw/include")
include_directories("../deps")
include_directories(".")

link_libraries(glfw)

#glad
set(GLAD "../glfw/deps/glad.c")

#源代码文件夹
aux_source_directory(. DIR_SRC)
aux_source_directory(core DIR_CORE)
aux_source_directory(entity DIR_ENTITY)
aux_source_directory(physics DIR_PHYSICS)
aux_source_directory(ui DIR_SRC_UI)

#添加静态库  
ADD_LIBRARY(lovegl STATIC ${GLAD} ${DIR_SRC} ${DIR_CORE} ${DIR_ENTITY} ${DIR_PHYSICS} ${DIR_SRC_UI})  

#将静态库重新命名
SET_TARGET_PROPERTIES(lovegl PROPERTIES OUTPUT_NAME "lovegl")