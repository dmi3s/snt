#ifndef SAMPLE_HPP_
#define SAMPLE_HPP_

#include <string>

namespace sn_test {

    typedef int sample;

    using std::to_string;
    sample stosm(const std::string& s)
    {
        return std::stoi(s);
    }

}

#endif