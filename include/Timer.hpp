#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer {
public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        end_time = std::chrono::high_resolution_clock::now();
    }

    double elapsed() const {
        return std::chrono::duration<double>(end_time - start_time).count();
    }

private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
};

#endif // TIMER_HPP
