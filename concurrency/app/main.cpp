#include "lib/concurrency-util.h"
#include <chrono>
#include <iostream>

Spinlock spin;

std::atomic_bool atomiclock{false};
std::mutex mutexlock;

long long mutexsum{0};
long long spinsum{0};

void mutexWorker()
{
    auto start = std::chrono::steady_clock::now();
    for (uint i = 0; i < 10000000; ++i) {
        {
            CriticalSection<> lock(mutexlock);
            mutexsum += i;
        }
    }
    std::cout << "CriticalSection duration: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count() << std::endl;
}

void spinlockWorker()
{
    auto start = std::chrono::steady_clock::now();
    for (uint i = 0; i < 10000000; ++i) {
        spin.lock();
        spinsum += i;
        spin.unlock();
    }
    std::cout << "Spinlock duration: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count() << std::endl;
}

int main(int argc, char* argv[])
{
    std::thread t1(mutexWorker);
    std::thread t12(mutexWorker);
    std::thread t2(spinlockWorker);
    std::thread t22(spinlockWorker);

    t1.join();
    t12.join();
    t2.join();
    t22.join();

    std::cout << "mutexsum: " << mutexsum << ", spinsum: " << spinsum << std::endl;
    long long correct{0};
    for (uint i = 0; i < 10000000; ++i) {
        correct += i;
    }
    std::cout << "correct: " << 2*correct << std::endl;
    return 0;
}