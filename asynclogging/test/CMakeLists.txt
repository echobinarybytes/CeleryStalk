project(log_test)
set(SRC
    log_test.cc
    ../asynclog.cc
    ../../timestamp/timestamp.cc
    ../largebuffer.cc)

set(THREAD_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_compile_options(-g)
add_executable(log_test ${SRC})
target_link_libraries(log_test Threads::Threads)
