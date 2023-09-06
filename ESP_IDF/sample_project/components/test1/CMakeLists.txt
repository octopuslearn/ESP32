file(GLOB_RECURSE SOURCES 	./*.c)	#components目录下的所有.c文件

set(include_dirs	#给变量include_dirs赋值为 test1_1
   	test1_1			#头文件test1_1文件夹下有头文件
	)

idf_component_register(SRCS  ${SOURCES}
						INCLUDE_DIRS ${include_dirs} 
						)