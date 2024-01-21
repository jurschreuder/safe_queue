Thread Safe Queue in C++
===

Example application to get you started can be found in `src/safe_queue.cpp`.

# Header only library / snippet
```cpp
#include "safe_queue.hpp"
```

# Try out the examples and run the tests

Requirements:

**Cmake > 3.5**
**Catch2 > 3.0**

```bash
mkdir build
cd build
cmake ..
make
./safe_queue
./tests
```

# How to use it?

## Basic

```cpp
// some things you'll need
#include <iostream>

// new queue
SafeQueue<int> queue;

// put three in
queue.put(1);
queue.put(2);
queue.put(3);

// get three out
for(int i = 0; i < 3; ++i){
    int item = queue.get();
    std::cout << "Item: " << item << "\n";
}

```

## Thread

```cpp
// some things you'll need
#include <iostream>
#include <thread>
#include <chrono>

// new queue
std::shared_ptr<SafeQueue<int>> queue = std::make_shared<SafeQueue<int>>();

// listen to the queue in a thread
std::thread test_thread = std::thread([queue](){
  while(true){
    // get an item from the queue 
    int item = queue->get();
    std::cout << "Item: " << item << "\n";
  }
});
test_thread.detach(); 

// put three in, waiting a bit in between
for(int i = 0; i < 3; ++i){
    queue->put(i);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

```

## Maximum number of items

```cpp
// some things you'll need
#include <iostream>
#include <thread>
#include <chrono>

// new queue
std::shared_ptr<SafeQueue<int>> queue = std::make_shared<SafeQueue<int>>();

// listen to the queue in a thread
std::thread test_thread = std::thread([queue](){
  while(true){
    // get an item from the queue 
    int item = queue->get();
    std::cout<<"Item number: "<<item<<" queue size: "<<queue->size<<"\n";
    // wait 1ms bit for demonstration purposes
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
});
test_thread.detach(); 

// If the queue is processed too slow and we want to stop
// adding items if there are too many items already
for(int i = 0; i < 20; ++i){
  if(queue->size < 3){
    queue->put(i);
  }
}
// wait for this all to be over
std::this_thread::sleep_for(std::chrono::milliseconds(500));

```

## Faster with move()

If you're working with a lot of data, it's faster to create a unique pointer to the data you want in the queue, **move** it into the queue, and **move** it out of the queue again when you need it to gain back ownership again.

Run the tests to see a benchmark showing the performance increase.

Example:

```
for(int i = 0; i < 100; ++i){
  // make some big data
  auto big = std::make_unique<std::vector<int>>(1e6);
  // notice the move here 
  big_queue->put_move(std::move(big));
}
std::cout << "Added 100 to big_queue\n";

for(int i = 0; i < 100; ++i){
  // notice the move here
  auto big = big_queue->get_move();
}
std::cout << "Got 100 to big_queue\n";

```
