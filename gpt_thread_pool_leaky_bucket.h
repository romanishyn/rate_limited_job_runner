#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t threadCount, size_t bucketCapacity, size_t tokenRate) :
            stop(false), bucket(bucketCapacity), rate(tokenRate) {
        for (size_t i = 0; i < threadCount; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        condition.wait(lock, [this] {
                            return stop || !tasks.empty();
                        });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    // Rate limiting: acquire tokens from the bucket
                    {
                        std::unique_lock<std::mutex> lock(bucketMutex);
                        conditionToken.wait(lock, [this] {
                            return stop || bucket > 0;
                        });

                        if (stop) {
                            return;
                        }

                        --bucket;
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }

        condition.notify_all();
        for (std::thread& thread : threads) {
            thread.join();
        }
    }

    template<class F>
    void enqueue(F&& task) {
        {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.emplace(std::forward<F>(task));
        }
        condition.notify_one();
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable condition;
    bool stop;

    size_t bucket;
    std::mutex bucketMutex;
    std::condition_variable conditionToken;
    size_t rate;
};

int main() {
    // Example usage
    ThreadPool pool(4, 10, 2); // 4 threads, bucket capacity of 10, 2 tokens per second

    // Enqueue some tasks
    for (int i = 0; i < 20; ++i) {
        pool.enqueue([i] {
            std::cout << "Task " << i << " executed." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }

    // Sleep to allow tasks to finish
    std::this_thread::sleep_for(std::chrono::seconds(25));

    return 0;
}
