#ifndef PRODUCER_HPP
#define PRODUCER_HPP

#include <atomic>
#include <boost/signals2.hpp>
#include <functional>
#include "storage.hpp"
#include "generator.hpp"

namespace sn_test {

    class producer {
    public:
        explicit producer(int freq);

        void threadFn( std::shared_ptr<std::promise<void>> pr );
        void exit();

        int getFreq() const;

        void connect(std::function<void(sample)>);

        producer(const producer&) = delete;
        producer& operator=(const producer&) = delete;

    private:
        typedef boost::signals2::signal<void(sample)> signal;
        signal sig;

        typedef std::chrono::steady_clock clock;
        const clock::duration period;
        clock::time_point last_value_time;

        generator gen;
        std::atomic<bool> quit = false;

    };

}

#endif // PRODUCER_HPP