#pragma once

#include <atomic>
#include <mutex>
#include <thread>

class Spinlock {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    void lock();
    void unlock();
};

template<class Mutex = std::mutex>
class CriticalSection
{
    Mutex& lock_;
public:
    CriticalSection(Mutex& lock) : lock_(lock) { lock_.lock(); }
    ~CriticalSection() { lock_.unlock(); }
};
