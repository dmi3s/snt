#ifndef CHUMK_LIST_HPP_
#define CHUMK_LIST_HPP_


#include <deque>
#include <vector>
#include <queue>
#include <memory>
#include <boost/filesystem.hpp>
#include <mutex>
#include "sample.hpp"


namespace sn_test {

    class chunk_list {
    public:

        chunk_list(const std::string& working_directory);

        void save(std::shared_ptr<std::deque<sample>> buff, size_t last_sample);
        size_t loadLastSamples(std::vector<sample>& v, size_t n_samples) const;

    private:

        struct chunk {
            size_t last_sample;
            size_t nsamples;
        };

        size_t loadChunk(std::vector<sample>& dst, size_t last_sample, size_t samples_to_load) const;

        mutable std::mutex list_mutex;
        std::deque<chunk> list;
        const boost::filesystem::path working_dir;
    };

}
#endif //CHUMK_LIST_HPP_
