#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <thread>
#include <chrono>

#include "safe_queue.hpp"

// Most basic usage
int PutGetBasic( int items_n ) {
	// new queue
	SafeQueue<int> queue;

	// put items_n in
  for(int i = 0; i < items_n; ++i){	
    queue.put(1);
  }

  int total{0};

	// get items_n out
	for(int i = 0; i < items_n; ++i){
    total += queue.get();
	}
  
  return total;
}

TEST_CASE( "PutGetBasic", "[items_n]" ) {
    REQUIRE( PutGetBasic(1) == 1 );
    BENCHMARK("PutGetBasic 1 item") {
        return PutGetBasic(1);
    };

    REQUIRE( PutGetBasic(3) == 3 );
    BENCHMARK("PutGetBasic 3 items") {
        return PutGetBasic(3);
    };

    REQUIRE( PutGetBasic(1e3) == 1e3 );
    BENCHMARK("PutGetBasic 1000 items") {
        return PutGetBasic(1e3);
    };
}

// Usage when using a simple shared pointer
int PutGet( int items_n, int vec_size ) {
	// new queue
  auto queue = std::make_shared<SafeQueue<std::vector<int>>>();

	// put items_n in
  for(int i = 0; i < items_n; ++i){	
    std::vector<int> foo(vec_size);
    foo.at(vec_size-1) = 1;
    queue->put(foo);
  }

  int total{0};

	// get items_n out
	for(int i = 0; i < items_n; ++i){
    auto foo = queue->get();
    total += foo.at(vec_size-1);
	}
  
  return total;
}

TEST_CASE( "PutGet", "[items_n, vec_size]" ) {
    REQUIRE( PutGet(1, 1) == 1 );
    BENCHMARK("PutGet 1 item") {
        return PutGet(1, 1);
    };

    REQUIRE( PutGet(3, 1) == 3 );
    BENCHMARK("PutGet 3 items") {
        return PutGet(3, 1);
    };

    REQUIRE( PutGet(1e3, 1) == 1e3 );
    BENCHMARK("PutGet 1000 items") {
        return PutGet(1e3, 1);
    };

    REQUIRE( PutGet(1e3, 1) == 1e3 );
    BENCHMARK("PutGet 1000 items, with 1000 elements each") {
        return PutGet(1e3, 1e3);
    };
}

// Usage when using a simple unique pointer and move
int PutGetMove( int items_n, int vec_size ) {
	// new queue
  auto queue = std::make_shared<SafeQueue<std::unique_ptr<std::vector<int>>>>();

	// put items_n in
  for(int i = 0; i < items_n; ++i){	
    auto foo = std::make_unique<std::vector<int>>(vec_size);
    foo->at(vec_size-1) = 1;
    queue->put_move(std::move(foo));
  }

  int total{0};

	// get items_n out
	for(int i = 0; i < items_n; ++i){
    std::unique_ptr<std::vector<int>> foo = queue->get_move();
    total += foo->at(vec_size-1);
	}
  
  return total;
}

TEST_CASE( "PutGetMove", "[items_n, vec_size]" ) {
    REQUIRE( PutGetMove(1, 1) == 1 );
    BENCHMARK("PutGetMove 1 item") {
        return PutGetMove(1, 1);
    };

    REQUIRE( PutGetMove(3, 1) == 3 );
    BENCHMARK("PutGetMove 3 items") {
        return PutGetMove(3, 1);
    };

    REQUIRE( PutGetMove(1e3, 1) == 1e3 );
    BENCHMARK("PutGetMove 1000 items") {
        return PutGetMove(1e3, 1);
    };

    REQUIRE( PutGetMove(1e3, 1) == 1e3 );
    BENCHMARK("PutGetMove 1000 items, with 1000 elements each") {
        return PutGetMove(1e3, 1e3);
    };
}

