# C++23 Limit Order Matching Engine

A single-instrument limit order book written in C++23.

The engine currently supports price-time priority matching, partial fills, maker-price execution, order cancellation by ID, trade recording, and a simple command-line interface.

This project is still in development.

## Commands

```text
NEW BUY <price> <qty>
NEW SELL <price> <qty>
CANCEL <order_id>
PRINTBOOK
QUIT
```

Example:

```text
NEW SELL 100 10
NEW BUY 100 10
NEW BUY 99 5
CANCEL 3
QUIT
```

## Implementation

The order book uses:

* `std::map<int, PriceLevel>` for bid and ask price levels
* `std::list<Order>` for FIFO ordering within each price level
* `std::unordered_map<int, Location>` as a cancellation index
* `std::chrono::system_clock` for trade timestamps
* `std::from_chars` for command input parsing

Cancellation uses the order ID index to locate an order without scanning the full book. The stored `std::list` iterator allows constant-time removal once the price level has been found.

The matching engine is built as a separate library. The CLI, tests, and benchmark link against it.

## Project Structure

```text
OrderMatcher/
├── CMakeLists.txt
├── Include/matching_engine/
│   ├── Order.hpp
│   ├── Command.hpp
│   ├── Trade.hpp
│   ├── Location.hpp
│   └── MatchingEngine.hpp
├── src/
│   ├── Order.cpp
│   ├── MatchingEngine.cpp
│   ├── benchmarking.cpp
│   └── main.cpp
└── tests/
    └── test_matching_engine.cpp
```

## Build

Requires a C++23 compiler and CMake 3.20+.

```bash
cmake -S . -B build
cmake --build build
```

Run the engine:

```bash
./build/engine
```

Run the tests:

```bash
./build/tests
```

## Benchmark

A standalone benchmark submits 1,000,000 generated orders directly to the matching engine.

On a Ryzen 5 5600X with an optimised GCC build, the current median result is approximately:

```text
2.50 million new-order operations per second
```

This measures batch throughput only and is not a per-order latency measurement.

## Roadmap

* Multi-instrument order books
* TCP server for submitting orders over the network
* Order book depth output
* Further testing and benchmarking
