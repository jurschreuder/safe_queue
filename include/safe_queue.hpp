#ifndef SAFE_QUEUE
#define SAFE_QUEUE

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

// A threadsafe-queue.
// Based on ChewOnThis_Trident StackOverflow example.

template <class T>
class SafeQueue
{
public:
  SafeQueue(void)
    : q() // queue
    , m() // mutex
    , c() // condition
  {}

  ~SafeQueue(void)
  {}

  // Keeps track of the size of the queue 
  std::atomic<std::uint64_t> size;

  // Add an element to the queue.
  void put(T t)
  {
    std::lock_guard<std::mutex> lock(m);
    q.push(t);
    size++;
    c.notify_one();
  }

  // Get the "front"-element.
  // If the queue is empty, wait till a element is avaiable.
  T get(void)
  {
    std::unique_lock<std::mutex> lock(m);
    while(q.empty())
    {
      // release lock as long as the wait and reaquire it afterwards.
      c.wait(lock);
    }
    T val = q.front();
    q.pop();
    size--;
    return val;
  }

private:
  std::queue<T> q;
  mutable std::mutex m;
  std::condition_variable c;
};
#endif
