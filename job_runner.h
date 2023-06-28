#pragma once

#include "rate_limiter.h"

#include <queue>

class JobRunner
{
public:
    JobRunner( std::unique_ptr< RateLimiter > rateLimiter )
        : 

    void addJob( Job job )
    {
        {
            mutex lock;
            m_jobs.push(job);
        }
        cv.notify();
    }

    void jobDone()
    {
        cv.notify();
    }

private:
    void runner() //worker_thread
    {
        while( cv.wait( ) && m_rateLimiter.consume() )
        {
            auto job = m_jobs.front();
            m_jobs.pop;
            job();
        }
    }

private:
    std::queue<Job> m_jobs;
    std::unique_ptr< RateLimiter > m_rateLimiter;
};

// #1 event loop https://godbolt.org/z/4Ge37ef1W
// #2 Timer: try_consume(), whenNextTimeCallConsume()
// #3 +1 thread inside of RateLimiter
// #4 sleep_until(start_time + interval * n) inside of consume()
//    n - (now() - start_time) / interval + 1 - double check formula

// we want to limit number of requests for time interver
// or number of request in execution
// for example, what if interval is 1 min, response time is 20 min,
// allowed 4 requests for 1
// 10 requests come in how to process

// or response time is 1 sec



        if (cv.wait_for(lock, std::chrono::seconds(1), [&]() { return ready; terminate_ || !running_ })) {
            std::cout << "Worker thread finished its work." << std::endl;
        } else {
            std::cout << "Timeout occurred, worker thread still running." << std::endl;
        }




#include <iostream>
#include <thread>
#include <condition_variable>

class WorkerThread {
public:
    WorkerThread() : running_(false), terminate_(false) {}

    ~WorkerThread() {
        Stop(); // Stop the thread if it's still running
    }

    void Start() {
        if (!running_) {
            running_ = true;
            thread_ = std::thread(&WorkerThread::WorkerLoop, this);
        }
    }

    void Stop() {
        if (running_) {
            {
                std::unique_lock<std::mutex> lock(mutex_);
                terminate_ = true; // Set the termination flag
                condition_.notify_all(); // Notify the worker thread to stop
            }

            thread_.join(); // Wait for the worker thread to exit
            running_ = false;
        }
    }

private:
    void WorkerLoop() {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex_);
            // Wait for notification or termination request
            condition_.wait(lock, [this]() { return terminate_ || !running_; });

            if (terminate_) {
                // Termination requested, exit the loop
                break;
            }

            // Perform some work here
            std::cout << "Worker thread is running!" << std::endl;
        }
    }
  
  

    std::thread thread_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool running_;
    bool terminate_;
};

int main() {
    WorkerThread worker;
    worker.Start();

    // Do some other work here...

    // Terminate the worker thread after 5 seconds
    std::this_thread::sleep_for(std::chrono::seconds(5));
    worker.Stop();

    return 0;
}
























