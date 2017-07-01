#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <deque>
#include <vector>
#include <atomic>
#include <limits>
#include <condition_variable>
#include <mutex>
#include "sample.hpp"

namespace sn_test {

    class storage {
    public:
        const size_t BUF_SIZE = 4 * 10;
        const size_t BUF_MAX_ELEMS = BUF_SIZE / sizeof(sample);

        explicit storage();

        void add(sample sm);

        std::vector<sample> getLast(size_t n_samples) const;

        void flush();
        void exit();

        void runStorage();

    private:

        static void saveChunk(const std::deque<sample>& samples2save, const size_t total_samples);

        std::atomic<bool> quit = false;
        std::atomic<size_t> total_samples = 0;
        std::deque<sample> buff;
    };

}

#endif // !STORAGE_HPP
