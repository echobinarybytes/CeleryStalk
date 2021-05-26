#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>

std::mutex mtx;
class Child {
public:
    // ignore
public:
    bool IsCry() { return cry; }

    void WakeUp() { 
        std::cout << "wake up! Wuuuuu" << std::endl;
        cry = true;
    }

    void Sleep() {
        std::cout << "go to sleep" << std::endl;
        cry = false;
    }

private:
    bool cry = false;
};

void ChildLive(Child& child) {
    while (1) {
        sleep(1);
        std::unique_lock<std::mutex> ulock(mtx);
        child.WakeUp();
    }
}


int main() {
    srand(time(NULL));
    Child child;

    std::thread th1(ChildLive, std::ref(child));

    while (1) {
        if (!child.IsCry()) {
        } else {
            std::unique_lock<std::mutex> ulock(mtx);
            std::cout << "feed child" << std::endl;
            child.Sleep();
        }

    }

    th1.detach();
}
