Thread Safe Queue in C++
===

![tests](https://github.com/jurschreuder/safe_queue/actions/workflows/test-ubuntu.yml/badge.svg)

Example application to get you started can be found in `src/safe_queue.cpp`.

# Header only library / snippet
```cpp
#include "safe_queue.hpp"
```

# Try out the examples and run the tests

Requirements:

**Cmake > 3.5**

**Catch2 > 3.0**

Run:

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

#### Example:

```cpp
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

## shared\_ptr

If you want to do something else with the object after you put it into the queue, you'll need the reference counting of the std::shared\_ptr to keep track of the memory lifetime of the object in the queue. Like in this example:

```cpp
// new queue
auto queue = std::make_shared<SafeQueue<std::shared_ptr<std::vector<int>>>>();

// put 100 in
for(int i = 0; i < 100; ++i){	
  auto foo = std::make_shared<std::vector<int>>(1e6);
  queue->put(foo);
}

// get 100 out
for(int i = 0; i < 100; ++i){
  std::shared_ptr<std::vector<int>> foo = queue->get();
}

```


#### Benchmarks:

```
-------------------------------------------------------------------------------
PutGet
-------------------------------------------------------------------------------
benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
PutGet 1 item                                  100            18      2.133 ms 
                                         1.1953 us     1.1843 us    1.23831 us 
                                        96.1105 ns    17.3557 ns    221.307 ns 
                                                                               
PutGet 3 items                                 100             9     2.3292 ms 
                                        2.69148 us    2.64209 us    2.77551 us 
                                         321.14 ns    214.406 ns    463.596 ns 
                                                                               
PutGet 1000 items                              100             1    67.5246 ms 
                                        676.699 us    674.708 us    679.362 us 
                                        11.6007 us    9.23667 us     15.628 us 
                                                                               
PutGet 1000 items, with 1000                                                   
elements each                                  100             1     3.52502 s 
                                        35.2378 ms    35.2049 ms    35.3009 ms 
                                        224.446 us    135.443 us    369.356 us 
                                                                               
```

```
-------------------------------------------------------------------------------
PutGetMove
-------------------------------------------------------------------------------
benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
PutGetMove 1 item                              100            27     2.1546 ms 
                                        787.362 ns    782.206 ns    807.316 ns 
                                        46.9645 ns    7.65404 ns     110.72 ns 
                                                                               
PutGetMove 3 items                             100            15     2.1765 ms 
                                        1.45697 us    1.44714 us    1.49258 us 
                                         85.606 ns    22.4203 ns    197.675 ns 
                                                                               
PutGetMove 1000 items                          100             1    30.3233 ms 
                                        304.776 us    303.399 us    306.706 us 
                                        8.31293 us    6.35508 us    11.3026 us 
                                                                               
PutGetMove 1000 items, with 1000                                               
elements each                                  100             1     1.05036 s 
                                        10.4601 ms    10.4415 ms    10.4847 ms 
                                        108.646 us     82.545 us     154.92 us 
```
