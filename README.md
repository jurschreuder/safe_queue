Thread Safe Queue in C++
===

# Header only library / snippet
```cpp
#include "safe_queue.hpp"
```

# Try out some examples

```bash
mkdir build
cmake ..
make
./safe_queue
```

# How to use it?

## Basic

```cpp
#include <iostream>

// new queue
std::shared_ptr<SafeQueue<int>> queue = std::make_shared<SafeQueue<int>>();

// put three in
queue->put(1);
queue->put(2);
queue->put(3);

// get three out
for(int i = 0; i < 3; ++i){
    int item = queue->get();
    std::cout << "Item: " << item << "\n";
}

```

## Thread

```cpp
#include <iostream>
#include <thread>
#include <chrono>

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
#include <iostream>
#include <thread>
#include <chrono>

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
