/**
 * @file threadpool.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-04-04
 * 
 * @copyright Copyright (c) 2023
 * 
 * @note 写一个简单的线程池
 */

#pragma once
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  ThreadPool(int nums = 8) : taskpool(std::make_shared<TaskPool>()) {
    for (int i = 0; i < nums; i++) {
      threads.emplace_back([taskPool = taskpool] {
        std::unique_lock<std::mutex> locker(taskPool->mtx);
        while (true) {
          if (!taskPool->tasks.empty()) {
            auto task = std::move(taskPool->tasks.front());
            taskPool->tasks.pop();
            taskPool->mtx.unlock();
            task();
            taskPool->mtx.lock();
          } else if (taskPool->isClosed) {
            break;
          }
          taskPool->cv.wait(locker);
        }
      });
    }
  }

  ThreadPool(ThreadPool&&) = default;

  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> locker(taskpool->mtx);
      taskpool->isClosed = true;
    }
    taskpool->cv.notify_all();
    for (auto&& thread : threads) {
      thread.join();
    }
  }

  template <class Callable>
  void addTask(Callable&& cb) {
    {
      std::unique_lock<std::mutex> locker(taskpool->mtx);
      taskpool->tasks.emplace(std::forward<Callable>(cb));
    }
    taskpool->cv.notify_one();
  }

 private:
  using CallBack = std::function<void(void)>;

  struct TaskPool {
    bool isClosed{false};
    std::queue<CallBack> tasks;
    std::mutex mtx;
    std::condition_variable cv;
  };

  std::shared_ptr<TaskPool> taskpool;
  std::vector<std::thread> threads;
};