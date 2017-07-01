#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <chrono>
#include <random>
#include "sample.hpp"


namespace sn_test {

    class generator {
    public:
        explicit generator();
        sample newValue();
    private:
        std::mt19937 gen;
        std::uniform_int_distribution<sample> dist;

    };

}

#endif // !GENERATOR_HPP
