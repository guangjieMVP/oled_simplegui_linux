cmake_minimum_required(VERSION 3.18.4)

project(display VERSION 1.0)

set(exec_name display)

include_directories(. 3rd/simplegui/GUI/inc 3rd/simplegui/HMI/inc)
include_directories(3rd/log)
include_directories(./simplegui/inc)

#设置C的交叉编译工具
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc) 

# link_directories("../ubus/usr/local/lib")
# link_libraries(pthread)
aux_source_directory(./3rd/simplegui/HMI/src SOURCES)
aux_source_directory(./3rd/simplegui/GUI/src SOURCES)
aux_source_directory(./3rd/log SOURCES)
aux_source_directory(./simplegui/src SOURCES)
aux_source_directory(. SOURCES)

add_executable(${exec_name} ${SOURCES})