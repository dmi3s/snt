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
#include "chunk_list.hpp"

namespace sn_test {

    class istorage {
    public:
        virtual void threadFn() = 0;
        virtual void add(sample sm) =0;
        virtual std::vector<sample> getLast(size_t n_samples) const = 0;
        virtual void exit() = 0;
        virtual ~istorage() {}
    };


    class storage final : public istorage {
    public:
        const size_t BUF_SIZE = sizeof(sample) * 10;
        const size_t BUF_MAX_ELEMS = BUF_SIZE / sizeof(sample);

        explicit storage(const std::string& dir);

        void add(sample sm) override;

        std::vector<sample> getLast(size_t n_samples) const override;

        void exit() override;

        void threadFn() override;

    private:

        std::atomic<bool> quit = false;
        std::atomic<size_t> total_samples = 0;

        typedef std::deque<sample> container;
        typedef std::shared_ptr<container> container_ptr;

        mutable std::mutex buff_access;
        container_ptr buff;

    private:

        void flush();

        chunk_list chunks;
        void enqueBuffer2Chunk(std::unique_lock<std::mutex>&& buff_lock);
    private:

        struct save_task {
            container_ptr buff;
            size_t last_sample;
        };

        typedef std::deque<save_task> queue;
        queue save_tasks_queue;
        mutable std::mutex save_tasks_mutex;
        std::condition_variable save_task_cv;

    private:
        static size_t collect(std::vector<sample>& r, const container& q, size_t rest);

    };

}

#endif // !STORAGE_HPP
