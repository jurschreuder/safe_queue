// safe_queue example

#include <iostream>
#include <thread>
#include <chrono>

#include "safe_queue.hpp"


int main() {
  std::cout << "Simple (thread) Safe Queue example.\n";

  std::cout << "\n=========== Demo 1: Basic ===========\n";

	// new queue
	SafeQueue<int> basic_queue;

	// put three in
	basic_queue.put(1);
	basic_queue.put(2);
	basic_queue.put(3);

	// get three out
	for(int i = 0; i < 3; ++i){
    int item = basic_queue.get();
    std::cout << "Item: " << item << "\n";
	}

  std::cout << "\n=========== Demo 2: Thread ===========\n";

  // new queue
  std::shared_ptr<SafeQueue<int>> queue = std::make_shared<SafeQueue<int>>();

  // listen to the queue in a thread
  std::thread test_thread = std::thread([queue](){
      while(true){
        // get an item from the queue 
				int item = queue->get();
				std::cout << "Item in thread: " << item << 
            " << queue size: " << queue->size << "\n";
        // wait 1ms bit for demonstration purposes
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
  });
  test_thread.detach(); 
  
  // put three in, waiting a bit in between
  for(int i = 0; i < 3; ++i){
		queue->put(i);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

  std::cout << "\n=========== Demo 3: Max size ===========\n";

  // If the queue is processed too slow and we want to stop
  // adding items if there are too many items already
  for(int i = 0; i < 20; ++i){
    if(queue->size < 3){
      queue->put(i);
    }
	}
  // wait till completed
  std::this_thread::sleep_for(std::chrono::milliseconds(500));


  std::cout << "\n=== Demo 4: Pass ownership and get ownership from the queue ===\n";

  auto big_queue = std::make_shared<SafeQueue<std::unique_ptr<std::vector<int>>>>();

  for(int i = 0; i < 100; ++i){
    // make some big data
    auto big = std::make_unique<std::vector<int>>(1e6);
    big_queue->put_move(std::move(big));
  }
  std::cout << "Added 100 to big_queue\n";

  for(int i = 0; i < 100; ++i){
    auto big = big_queue->get_move();
  }
  std::cout << "Got 100 to big_queue\n";

	return 0;
}	
