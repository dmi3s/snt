#ifndef PUBLISHER_HPP_
#define PUBLISHER_HPP_

#include <chrono>
#include <functional>
#include <boost/signals2.hpp>

#include "storage.hpp"

namespace sn_test {

    class publisher {
    public:
        typedef std::chrono::steady_clock clock;

        publisher(std::shared_ptr<istorage> store, int freq);

        publisher(const publisher&) = delete;
        publisher& operator=(const publisher&) = delete;

        void onExit(std::function<void()> exit_fn);

        void threadFn(std::shared_ptr<std::promise<void>> pr);

    private:
        void atExit();

        typedef boost::signals2::signal<void()> exit_signal;
        exit_signal on_exit;

        std::shared_ptr<istorage> store;
        const int freq;
    };

}

#endif //PUBLISHER_HPP_