cmake_minimum_required(VERSION 3.10)
project(ATI)
set(CMAKE_BUILD_TYPE "Debug")
# g++编译选项 -O3优化代码
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -O3 -std=c++14 -Wall")

# 添加头文件路径 -I
include_directories(${CMAKE_SOURCE_DIR}/include    
)

# 添加源文件路径
set(SRC 
    src/ATI_reading.cpp
    src/periodic_rt_task.cpp
)

# 输出可执行文件 
add_executable(main main.cpp ${SRC})
# 链接库 -l
target_link_libraries(main PRIVATE 
                    pthread                  # C线程
)

