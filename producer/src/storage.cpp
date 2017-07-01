#include "storage.hpp"

#include <iostream>
#include <fstream>
#include <cassert>

namespace sn_test {

    using namespace std;
    namespace fs = boost::filesystem;

    storage::storage(const std::string& dir) : 
        working_dir(dir),
        buff(make_shared<container>())
    {
        if (!fs::exists(dir))
            fs::create_directory(dir);
    }

    void storage::exit() {
        quit = true;
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
                const auto task = save_tasks_queue.front();
                lock.unlock();
                try {
                    saveChunk(task.buff, task.last_sample);
                }
                catch (const exception& e)
                {
                    cerr << "ERROR! Failed to save " << task.buff->size() << "samples from " << task.last_sample << ": " << e.what() << endl;
                }
                lock.lock();
                save_tasks_queue.pop();
            }
        }

    }

    void storage::add(sample sm)
    {
        assert(buff);
        buff->push_front(sm);
        ++total_samples;

        if (buff->size() == BUF_MAX_ELEMS)
        {
            container_ptr samples2save = make_shared<container>();
            std::swap(buff, samples2save);
            const save_task st{ samples2save, total_samples };
            unique_lock<mutex> lock(save_tasks_mutex);
            save_tasks_queue.push(st);
        }
        cout << sm << "\n";
    }

    void storage::saveChunk(container_ptr samples2save, const size_t nsamples)
    {
        const auto tmp_name = fs::path(working_dir) / fs::unique_path();
        fstream out(tmp_name.string(), fstream::out | fstream::trunc);
        if (!out.is_open())
            throw fstream::failure("Failed to open file " + tmp_name.string());
        out << samples2save->size() << "\n";
        for (auto s : *samples2save)
            out << s << "\n";
        out.close();
        const auto chunk_name = fs::path(working_dir) / (to_wstring(nsamples) + L".chunk");
        fs::rename(tmp_name, chunk_name);
    }

    std::vector<sample> storage::getLast(size_t n_samples) const
    {
        std::vector<sample> result;
        result.reserve(n_samples);
        return result;
    }


    void storage::flush()
    {
        std::cout << "total samples = " << total_samples << "\n";
        if (!buff->empty())
        {
            container_ptr samples2save = make_shared<container>();
            std::swap(*buff, *samples2save);
            saveChunk(samples2save, total_samples);
        }
    }


}