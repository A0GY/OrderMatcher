Task List — From Here Onward
Current position: CMake builds all three targets (library, engine, tests). Catch2 v3 wired in and working. Test executable runs (empty — "No tests ran"). The hard tooling is done.

Phase 1 — Prepare the engine for testing
Task 1.1 — Add accessor methods to MatchingEngine
Add these read-only (const) methods so tests can inspect state through a clean interface:

tradeCount() → returns number of trades (Trade_Record.size())
bestBid() → highest bid price on the book
bestAsk() → lowest ask price on the book
hasOrder(int id) → true/false if that order ID is still resting
restingQuantityAt(Side side, int price) → total quantity resting at that price level

Task 1.2 — Make data members private
Move Bids, Ask, order_index, Trade_Record, nextID to private. The accessors from 1.1 are now the only way in. (Convert the struct to a class, or add explicit private:.)
Task 1.3 — Remove debug prints
Strip every std::println("Test call working..."), "Your Buy order added", "Partial order was made", etc. from NewOrder and the matching loops. They'll flood test output and block the stress test.
Task 1.4 — Rebuild, confirm engine still works
cmake --build build then ./build/engine — confirm the CLI still runs after the refactor. Don't move on until the engine itself still builds and behaves.

Phase 2 — Prove the test pipeline
Task 2.1 — Write one dummy test
In tests/test_matching_engine.cpp:
cpp#include <catch2/catch_test_macros.hpp>
TEST_CASE("pipeline works") { REQUIRE(1 + 1 == 2); }
Build, run ./build/tests, see it pass.
Task 2.2 — Make it fail on purpose
Change to REQUIRE(1 + 1 == 3), rebuild, run, confirm it reports the failure. Then fix it back. Now you trust the setup actually checks things.

Phase 3 — Core matching tests (simplest → hardest)
Each test: Arrange (set up engine + orders) → Act (the one operation) → Assert (REQUIRE). One behaviour per test. Build and run after each.
Task 3.1 — Empty book: place one order, REQUIRE(tradeCount() == 0) and it's resting.
Task 3.2 — No-cross: buy below an existing sell, assert no trade, both rest.
Task 3.3 — Crossing produces a trade: sell, then crossing buy, REQUIRE(tradeCount() == 1) with correct quantity.
Task 3.4 — Maker's price: buy crosses a cheaper resting sell; assert trade price = resting order's price.
Task 3.5 — Full fill rests nothing: exact match; trade happens, nothing resting.
Task 3.6 — Partial fill: order bigger than available; filled part trades, restingQuantityAt(...) shows correct remainder.
Task 3.7 — Price-time priority (FIFO): two orders same price; crossing order fills the earlier one first.
Task 3.8 — Price priority: orders at different prices; best price fills first.

Phase 4 — Cancellation tests
Task 4.1 — Cancel removes order: place, cancel, REQUIRE(hasOrder(id) == false).
Task 4.2 — Cancelled order won't match: place, cancel, send a would-cross order, assert no trade.
Task 4.3 — Cancel non-existent ID: cancel_order(999), assert no crash, no state change.

Phase 5 — Refactor tests with SECTION
Task 5.1 — Once tests pass standalone, group those sharing setup (e.g. all "start with a resting sell, then...") into one TEST_CASE with multiple SECTIONs. Only do this after they work individually.

Phase 6 — Cleanups for publishing
Task 6.1 — Fix typo identifiers: Cancle→Cancel, Ouit→Quit, PatiallyFilled→PartiallyFilled, Cannclled→Cancelled, CommantType→CommandType. (Engine class is already MatchingEngine — good.)
Task 6.2 — Clean quit path: std::exit(1) → exit(0) or a clean loop break.
Task 6.3 — Rename print_book→print_trades (it prints trade history, not the book).
Task 6.4 — Add .gitignore: at minimum build/, so generated files never get committed.
Task 6.5 — Delete old_main.cpp (leftover clutter in src/).
Task 6.6 — (Optional) empty price-level cleanup: erase a price key when its FIFO empties.

Phase 7 — README
Task 7.1 — Write the README with:

One-line description
Build & run instructions (the exact cmake commands)
Design decisions with the WHY — std::map+std::list for sorted price levels with FIFO; unordered_map+stable iterators for O(1) cancel; chrono UTC timestamps; validation at the input gateway separate from matching
Note: requires GCC/libstdc++ for std::print
Future work (self-trade prevention, order-modify, multi-instrument)


Phase 8 — Stretch
Task 8.1 — Stress test: fire 100k–1M orders, assert correctness, optionally wrap in Catch2's BENCHMARK to measure throughput. (Needs Task 1.3 done — debug prints gone.)
Task 8.2 — Commit & push the finished state to GitHub with a clear history.