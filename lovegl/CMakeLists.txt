#是否编译成动态库
option(DYNAMIC_LIB "dynamic lib" OFF)

link_libraries(glew_s)
link_libraries(glfw)

#源代码文件夹
aux_source_directory(. DIR_SRC)
aux_source_directory(core DIR_CORE)
aux_source_directory(entity DIR_ENTITY)
aux_source_directory(physics DIR_PHYSICS)
aux_source_directory(ui DIR_SRC_UI)
aux_source_directory(raytracing DIR_SRC_RAYTRACING)

#添加静态库  
ADD_LIBRARY(lovegl STATIC ${DIR_SRC} ${DIR_CORE} ${DIR_ENTITY} ${DIR_PHYSICS} ${DIR_SRC_UI} ${DIR_SRC_RAYTRACING})
#将静态库重新命名
SET_TARGET_PROPERTIES(lovegl PROPERTIES OUTPUT_NAME "lovegl")