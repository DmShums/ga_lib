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
            deque.emplace_back(std::move(value));
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

    bool try_pop(T& dst) {
        std::unique_lock<std::mutex> lock{mx};
        if (!deque.empty()){
            dst = std::move(deque.front());
            deque.pop_front();
            return true;
        }
        return false;
    }
};

#endif //GA_LIB_TREADSAFEQ_H
