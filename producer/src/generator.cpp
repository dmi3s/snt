#include "generator.hpp"

namespace sn_test {

    generator::generator() :
        gen(std::random_device()()),
        dist(std::numeric_limits<sample>::min(), std::numeric_limits<sample>::max())
    {}

    sample generator::newValue()
    {
        static sample sm{ 0 };
        return sm++;
        //return dist(gen);
    }


}