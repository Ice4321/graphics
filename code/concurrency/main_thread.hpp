#ifndef INCLUDED_CONCURRENCY_MAIN_THREAD_HPP
#define INCLUDED_CONCURRENCY_MAIN_THREAD_HPP

#include<thread>

namespace Concurrency {
    inline std::jthread::id main_thread_id{};
}

#endif
