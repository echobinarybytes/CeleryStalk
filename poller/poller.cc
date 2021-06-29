#include "poller.h"
#include "epoll_poller.h"

Poller::Poller(std::shared_ptr<EventLoop> eventloop) : _eventloop(eventloop) {  

}

Poller::~Poller() {  }

std::shared_ptr<Poller> Poller::NewDefaultPoller(std::shared_ptr<EventLoop> eventloop) {
    return std::make_shared<EpollPoller>(eventloop);
}
