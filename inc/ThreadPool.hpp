
#include <queue>
#include <semaphore>
#include <thread>
#include <atomic>
#include <future>
#include <iostream>
#include <stop_token>
#include <mutex>

namespace com::thread
{
    constexpr unsigned int MAX_SEMS = 20;

    class Pool
    {
    public:

        /**
         * @brief Construct a new Thread Pool object
         * 
         * @param count Number of threads to create in the pool
         */
        explicit Pool(const std::size_t count)
            : m_threads()
            , m_futures()
            , m_sem(0)
            , m_jobs()
            , m_mutex()
        {
            for (size_t i = 0; i < count; ++i)
            {
                std::promise<void> p;
                std::shared_future<void> f = p.get_future();
                m_futures.push_back(f);
                m_threads.emplace_back(std::jthread([this, p = std::move(p)](std::stop_token stop) mutable
                {
                    while(!stop.stop_requested())
                    {
                        m_sem.acquire();

                        if (stop.stop_requested())
                        {
                            // time to stop thread
                            break;
                        }

                        std::function<void()> f;
                        {
                            std::lock_guard<std::mutex> lock(m_mutex);
                            f = m_jobs.front();
                            m_jobs.pop();
                        }

                        f();
                    }

                    p.set_value();
                }));
            }
        }

        ~Pool(void)
        {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                while(m_jobs.size() > 0)
                {
                    m_jobs.pop();
                }
            }

            // release all the semaphores
            m_sem.release(MAX_SEMS);
        }

        /**
         * @brief Add a new job to the thread pool queue
         * 
         * @param f Job to add to the job queue
         */
        void add(std::function<void()> f)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_jobs.emplace(f);
            m_sem.release();
        }

        /**
         * @brief Check to see if any of the threads has stopped running
         * 
         * @return true all threads running
         * @return false at least one thread has stopped
         */
        bool isHealthy(void) const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            for (auto& i : m_futures)
            {
                auto status = i.wait_for(std::chrono::microseconds(0));
                if (status == std::future_status::ready)
                {
                    // some thread has exited
                    return false;
                }
            }

            return true;
        }

        /**
         * @brief Get the current number of jobs left to run
         * 
         * @return size_t Number of jobs left to run
         */
        size_t getsize(void) const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_jobs.size();
        }

    private:

        std::vector<std::jthread> m_threads;
        std::vector<std::shared_future<void>> m_futures;
        std::counting_semaphore<MAX_SEMS> m_sem;
        std::queue<std::function<void()>> m_jobs;
        mutable std::mutex m_mutex;

        Pool& operator=(const Pool&) = delete;
        Pool(const Pool&) = delete;
        Pool(Pool&&) = delete;
        Pool& operator=(Pool&&) = delete;
      };
}
