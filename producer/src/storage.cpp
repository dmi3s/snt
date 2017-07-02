#include "storage.hpp"

#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <algorithm>

namespace sn_test {

    using namespace std;
    namespace fs = boost::filesystem;

    storage::storage(const std::string& dir) : 
        chunks(dir),
        buff(make_shared<container>())
    {
        if (!fs::exists(dir))
            fs::create_directory(dir);
    }

    void storage::exit() {
        quit = true;
        flush();
        save_task_cv.notify_all();
    }

    void storage::threadFn()
    {
        while (!quit)
        {
            unique_lock<mutex> lock(save_tasks_mutex);
            save_task_cv.wait(lock, [&] () { return quit || !save_tasks_queue.empty(); });
            while (!save_tasks_queue.empty())
            {
                const auto task = *save_tasks_queue.begin();
                lock.unlock();
                try {
                    chunks.save(task.buff, task.last_sample);
                    lock.lock();
                    save_tasks_queue.pop_front();
                }
                catch (const exception& e)
                {
                    cerr << "ERROR! Failed to save " << task.buff->size() << "samples from " << task.last_sample << ": " << e.what() << endl;
                }
            }
        }

    }

    void storage::enqueBuffer2Chunk(unique_lock<mutex>&& buff_lock)
    {
        container_ptr samples2save = make_shared<container>();
        std::swap(buff, samples2save);
        buff_lock.unlock();
        unique_lock<mutex> lock(save_tasks_mutex);
        save_tasks_queue.push_back(save_task{ samples2save, total_samples });
        lock.unlock();
        save_task_cv.notify_one();
    }

    void storage::add(sample sm)
    {
        unique_lock<mutex> buff_lock(buff_access);
        assert(buff);
        buff->push_front(sm);
        ++total_samples;

        if (buff->size() == BUF_MAX_ELEMS)
        {
            enqueBuffer2Chunk(move(buff_lock));
        }
        //cout << sm << "\n";
    }


    size_t storage::collect(std::vector<sample>& r, const storage::container& q, size_t rest)
    {
        const size_t nelems = min(q.size(), rest);
        auto b = q.begin(), e = b;
        advance(e, nelems);
        copy(b, e, back_inserter(r));
        return nelems;
    }

    std::vector<sample> storage::getLast(size_t n_samples) const
    {
        std::vector<sample> result;
        result.reserve(n_samples);
        auto rest = n_samples;

        // first copy from buffer
        unique_lock<mutex> buff_lock(buff_access);
        assert(buff);
        rest -= collect(result, *buff, rest);
        if (rest == 0)
            return result;

        unique_lock<mutex> task_lock(save_tasks_mutex);
        buff_lock.unlock();

        // then process qued tasks

        for (auto& t : save_tasks_queue)
        {
            rest -= collect(result, *(t.buff), rest);
            if (rest == 0)
                return result;
        }
        task_lock.unlock();

        // and finally go through chunks
        chunks.loadLastSamples(result, rest);

        return result;
    }


    void storage::flush()
    {
        std::cout << "total samples = " << total_samples << "\n";
        unique_lock<mutex> buff_lock(buff_access);
        if (!buff->empty())
        {
            enqueBuffer2Chunk(move(buff_lock));
        }
    }


}