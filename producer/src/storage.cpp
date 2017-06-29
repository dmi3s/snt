#include "storage.hpp"

#include <iostream>

namespace sn_test {

    void storage::add(const sample& sm)
    {
        buff.push_front(sm);
        std::cout << sm << "\n";
    }

    void storage::flush()
    {
        std::cout << "dequeue size = " << buff.size() << "\n";
    }


}