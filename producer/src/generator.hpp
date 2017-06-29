#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <chrono>
#include <random>
#include "sample.hpp"


namespace sn_test {

    class generator {
    public:
        explicit generator(int hz);
        sample newValue();
    private:
        std::mt19937 gen;
        std::uniform_int_distribution<sample> dist;

        typedef std::chrono::steady_clock clock;

        const clock::duration period;
        clock::time_point last_value_time;
    };

}

#endif // !GENERATOR_HPP
