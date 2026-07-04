C++23 Limit-Order Matching Engine

A single-instrument limit-order-book matching engine written in modern C++23. It implements
price-time priority matching with O(1) order cancellation, driven by a simple
command-line interface.

This is a learning-oriented but production-shaped project: the matching logic is separated into a
reusable library, the CLI is a thin executable on top of it, and the engine is covered by a
Catch2 unit-test suite.

Roadmap: next steps are multi-ticker order books, followed by a TCP wrapper so orders can be
fed to the engine over the network.


Commands

The engine reads space-separated commands from standard input:

CommandDescriptionNEW BUY <price> <qty>Submit a buy orderNEW SELL <price> <qty>Submit a sell orderCANCEL <order_id>Cancel a resting order by its IDPRINTBOOKPrint recorded tradesQUITExit the engine

Example session:

NEW SELL 100 10
NEW BUY 100 10      # crosses the resting sell -> one trade at price 100
NEW BUY 99 5        # rests on the book (no cross)
CANCEL 3            # cancels the resting buy
QUIT


Behaviour


Price-time priority (FIFO): incoming orders match the best price first, and within a price
level the oldest resting order fills first.
Execution at the maker's price: a crossing order trades at the resting (maker's) order's
price, giving price improvement to the aggressor.
Partial fills: an order larger than the available liquidity fills what it can; the unfilled
remainder rests on the book with its reduced quantity.
Cancellation: any resting order can be removed by ID in constant time.
Input validation: malformed, non-numeric, or non-positive orders are rejected at the input
layer before reaching the matching logic.



Design

The core data structures are chosen so that every operation the engine performs is efficient and,
just as importantly, straightforward to reason about.

Order book — std::map<int, PriceLevel> per side.
Bids and asks are each held in a std::map keyed by price. Because std::map keeps its keys
sorted, the best bid (highest) and best ask (lowest) are always at a known end of the map, and
walking price levels in matching order is a simple in-order traversal. Price lookup, insertion, and
removal are all O(log n) in the number of price levels.

Price levels — std::list<Order> FIFO queue.
Each price level holds its resting orders in a std::list, preserving arrival order so the oldest
order at a price is always at the front. std::list is chosen deliberately over std::vector
because its iterators are stable: inserting or erasing an order only rewires neighbouring node
pointers and never invalidates iterators to other orders. That stability is what makes the
cancellation index below possible.

Cancellation index — std::unordered_map<int, Location>.
To cancel by order ID without scanning the book, each live order ID maps to a Location holding its
side, price, and a stable std::list iterator pointing directly at the order's node. Cancelling
is then an O(1) hash lookup followed by an O(1) list erase. The index is kept in sync with the book
on every path that removes an order — both explicit cancels and orders consumed by matching — so it
never holds a dangling iterator.

Trade timestamps — std::chrono UTC microseconds.
Executions are stamped with a UTC microsecond timestamp captured from std::chrono::system_clock,
stored as an integer count since the epoch for exact, comparable ordering.

Validation separated from matching.
Input parsing and validation (well-formedness, numeric conversion via std::from_chars, and
positive-value "fat-finger" checks) live at the CLI boundary. The matching engine assumes it
receives clean, validated orders — mirroring how real exchanges separate an order-gateway layer
from the matching core.


Project layout

OrderMatcher/
├── CMakeLists.txt
├── Include/matching_engine/     # public headers (the interface)
│   ├── Order.hpp                #   Order, Side, OrderStatus, PriceLevel
│   ├── Command.hpp              #   Command, CommandType
│   ├── Trade.hpp                #   Trade
│   ├── Location.hpp             #   Location (cancel index entry)
│   └── MatchingEngine.hpp       #   the engine
├── src/                         # implementation
│   ├── Order.cpp
│   ├── MatchingEngine.cpp
│   └── main.cpp                 #   CLI entry point
└── tests/
    └── test_matching_engine.cpp # Catch2 unit tests

The engine compiles once into a library (matching_engine_lib); the CLI executable and the test
executable both link against it.


Building

Requirements:


A C++23 toolchain. Developed with GCC 15 (uses std::print / std::println and
std::from_chars).
CMake 3.20 or newer.



Note: the build links libstdc++exp for std::print support, which ties it to GCC's
libstdc++. Building with a different standard library may require adjusting the
target_link_libraries line in CMakeLists.txt.



bash# configure
cmake -S . -B build

# build (produces the engine and the test runner)
cmake --build build

The build produces two executables inside build/: engine (the CLI) and tests (the test suite).


Running

bash./build/engine

Then type commands at the prompt (see Commands).


Tests

The suite covers the core matching guarantees — price-time priority, crossing and non-crossing
orders, execution at the maker's price, full and partial fills, and the three cancellation cases
(cancel removes an order, a cancelled order no longer matches, and cancelling an unknown ID is a
safe no-op).

bash./build/tests            # run all tests
./build/tests --success  # also list passing assertions


Roadmap


 Multiple instruments (one book per ticker)
 TCP server so orders can be submitted over the network
 A true order-book depth view (resting quantity per level, both sides)



Reference

Primary language reference: Beginning C++23: From Beginner to Pro (Horton & Van Weert).