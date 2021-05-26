cmake_minimum_required(VERSION 3.18)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED) # 引入外部依赖包
add_executable(v_1 v_1.cc)
target_link_libraries(v_1 Threads::Threads) # 链接thread库
