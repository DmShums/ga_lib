//
// Created by Victor Muryn on 19.03.2024.
//

#ifndef GA_LIB_TREADSAFEQ_H
#define GA_LIB_TREADSAFEQ_H

#include <condition_variable>
#include <mutex>
#include <deque>

template<class T>
class ThreadSafeQ {
private:
    std::condition_variable cv;
    std::mutex mx;
    std::deque<T> deque;

public:
    ThreadSafeQ() = default;
    ~ThreadSafeQ() = default;

    void push(T value) {
        {
            std::lock_guard<std::mutex> lg{mx};
            deque.emplace_back(value);
        }

        cv.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock{mx};
        cv.wait(lock, [this](){ return !deque.empty(); });
        auto item = deque.front();
        deque.pop_front();

        return item;
    }
};

#endif //GA_LIB_TREADSAFEQ_H
