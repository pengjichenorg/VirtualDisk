#pragma once

#include <chrono>

using namespace std::chrono;

class Timer {
public:
    // constructor
    Timer() : m_begin(high_resolution_clock::now()) {

    }

    // default output milliseconds
    template <typename Duration = milliseconds>
    int64_t elapsed() const {
        return duration_cast<Duration>(high_resolution_clock::now() - m_begin).count();
    }

    // output microseconds
    int64_t elapsed_micro() const {
        return elapsed<microseconds>();
    }

    // output nanoseconds
    int64_t elapsed_nano() const {
        return elapsed<nanoseconds>();
    }

    // output seconds
    int64_t elapsed_seconds() const {
        return elapsed<seconds>();
    }

    // output minutes
    int64_t elapsed_minutes() const {
        return elapsed<minutes>();
    }

    // output hours
    int64_t elapsed_hours() const {
        return elapsed<hours>();
    }

private:
    time_point<high_resolution_clock> m_begin;
};
