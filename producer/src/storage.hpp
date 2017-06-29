#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <deque>
#include "sample.hpp"

namespace sn_test {

    class storage {
    public:
        void add(const sample& sm);
        void flush();
    private:
        std::deque<sample> buff;
    };

}

#endif // !STORAGE_HPP
