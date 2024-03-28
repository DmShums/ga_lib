// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "TPool.h"

thread_pool::thread_pool(size_t thr_n):done(false), joiner(threads) {
    try {
        for(size_t i = 0; i < thr_n; ++i) {
            threads.emplace_back(&thread_pool::worker_thread, this);
        }
    } catch(...) {
        done=true;
        throw;
    }
}

void thread_pool::worker_thread() {
    while (!done) {
        function_wrapper task;
        if (work_queue.try_pop(task)) {
            task();
        } else {
            std::this_thread::yield();
        }
    }
}

thread_pool::~thread_pool() {
    done = true;
}
