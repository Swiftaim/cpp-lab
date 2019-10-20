#include "concurrency-util.h"

void Spinlock::lock()
{
    while (flag.test_and_set());
}

void Spinlock::unlock()
{
    flag.clear();
}
