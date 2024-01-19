// safe_queue example

#include <iostream>
#include <thread>
#include <chrono>

#include "safe_queue.hpp"


int main() {
  std::cout << "Simple (thread) Safe Queue example.\n";

  // new queue
  std::shared_ptr<SafeQueue<int>> queue = std::make_shared<SafeQueue<int>>();

  std::cout << "=========== Demo 1 ===========\n";

  // put three in
  queue->put(1);
  queue->put(2);
  queue->put(3);

  // get three out
  for(int i = 0; i < 3; ++i){
    int item = queue->get();
    std::cout << "Item: " << item << "\n";
  }

  std::cout << "=========== Demo 2 ===========\n";

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

  std::cout << "=========== Demo 3 ===========\n";

  // If the queue is processed too slow and we want to stop
  // adding items if there are too many items already
  for(int i = 0; i < 20; ++i){
    if(queue->size < 3){
      queue->put(i);
    }
	}
  // wait till completed
  std::this_thread::sleep_for(std::chrono::milliseconds(500));


	return 0;
}	
