cmake_minimum_required(VERSION 3.18.4)
project(celerystalk)

set(THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)
add_compile_options(-g)
set(SRC
    buffer/buffer.cc
    channel/channel.cc
    event_loop/event_loop.cc
    poller/epoll_poller.cc
    poller/poller.cc
    socket_addr/socket_addr.cc
    tcp_server/acceptor.cc
    tcp_server/connector.cc
    tcp_server/tcp_server.cc
    asynclogging/asynclog.cc
    asynclogging/largebuffer.cc
    timestamp/timestamp.cc
    event_loop_thread_pool/event_loop_thread_pool.cc
    )

add_executable(main ${SRC} main.cc )
target_link_libraries(main Threads::Threads)
