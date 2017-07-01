#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <deque>
#include <vector>
#include <atomic>
#include <limits>
#include <condition_variable>
#include <queue>
#include <mutex>
#include <memory>
#include <boost/filesystem.hpp>
#include "sample.hpp"

namespace sn_test {

    class storage {
    public:
        const size_t BUF_SIZE = 4 * 10;
        const size_t BUF_MAX_ELEMS = BUF_SIZE / sizeof(sample);

        explicit storage(const std::string& dir);

        void add(sample sm);

        std::vector<sample> getLast(size_t n_samples) const;

        void exit();

        void threadFn();

    private:

        const boost::filesystem::path working_dir;

        std::atomic<bool> quit = false;
        std::atomic<size_t> total_samples = 0;

        typedef std::deque<sample> container;
        typedef std::shared_ptr<container> container_ptr;

        container_ptr buff;

        void saveChunk(container_ptr samples2save, const size_t total_samples);
        void flush();

    private:

        struct save_task {
            container_ptr buff;
            size_t last_sample;
        };

        typedef std::queue<save_task, std::deque<save_task>> queue_t;
        queue_t save_tasks_queue;
        mutable std::mutex save_tasks_mutex;
        std::condition_variable save_task_cv;

    };

}

#endif // !STORAGE_HPP
