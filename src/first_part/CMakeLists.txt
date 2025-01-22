cmake_minimum_required(VERSION 3.20)
project(CONNECT_POOL)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

aux_source_directory(${CMAKE_SOURCE_DIR}/include SOURCE_FILES)

message(${CMAKE_SOURCE_DIR}/include)
# 查找 MySQL 客户端库路径
link_directories(/usr/lib/x86_64-linux-gnu)
include_directories(${CMAKE_SOURCE_DIR}/include)


add_executable(${PROJECT_NAME} ${SOURCE_FILES} main.cpp)
target_link_libraries(CONNECT_POOL  jsoncpp mysqlclient)  # 链接 libmuduo_net.a 和 libmuduo_base.a
