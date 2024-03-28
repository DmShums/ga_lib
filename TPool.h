#ifndef GA_LIB_TPOOL_H
#define GA_LIB_TPOOL_H


#include <vector>
#include <iostream>
#include <functional>
#include <thread>
#include <future>
#include <deque>
#include <memory>
#include "TreadSafeQ.h"

class function_wrapper
{
    struct impl_base {
        virtual void call() = 0;
        virtual ~impl_base() {}
    };
    std::unique_ptr<impl_base> impl;
    template<typename F>
    struct impl_type: impl_base
    {
        F f;
        impl_type(F&& f_): f(std::move(f_)) {}
        void call() { f(); }
    };
public:
    template<typename F>
    function_wrapper(F&& f):
            impl(new impl_type<F>(std::move(f)))
    {}
    void operator()() { impl->call(); }
    function_wrapper() = default;
    function_wrapper(function_wrapper&& other):
            impl(std::move(other.impl))
    {}
    function_wrapper& operator= (function_wrapper&& other) {
        impl=std::move(other.impl);
        return *this;
    }

    function_wrapper(const function_wrapper&) = delete;
    function_wrapper(function_wrapper&)=delete;
    function_wrapper& operator=(const function_wrapper&) = delete;
};

class join_threads{
    std::vector<std::thread> &threads;
public:
    explicit join_threads(std::vector<std::thread> &threads_) :
            threads(threads_) {}

    ~join_threads() {
        for (std::thread& thread : threads) {
            if (thread.joinable())
                thread.join();
        }
    }
};

class thread_pool {
    std::atomic_bool done;
    ThreadSafeQ<function_wrapper> work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;
    void worker_thread();

public:
    explicit thread_pool(size_t thr_n);
    ~thread_pool();

    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type> submit(FunctionType f){
        typedef typename std::result_of<FunctionType()>::type result_type;

        std::packaged_task<result_type()> task(std::move(f));
        auto future = task.get_future();

        work_queue.push(std::move(task));

        return future;
    }
};


#endif //GA_LIB_TPOOL_H
