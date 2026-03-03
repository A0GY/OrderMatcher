````markdown
# C++23 Matching Engine (Project 2)

Single-instrument limit-order matching engine in C++23. Next: multi-ticker books, then a TCP wrapper so orders can be fed over the network.

## Input

Space-separated commands:

- `NEW BUY <price> <qty>`
- `NEW SELL <price> <qty>`
- `CANCEL <order_id>`
- `PRINTBOOK`
- `QUIT`

Example:
- `NEW BUY 100 10`

## Behaviour

- Price-time priority: best price first, FIFO at each price level
- Partial fills supported
- Unfilled remainder rests on the book
- `PRINTBOOK` shows aggregated depth by price level

## Build

Requires a C++23 toolchain (uses `<print>` / `std::println`):

```bash
g++ -std=c++23 -O2 -Wall -Wextra -pedantic main.cpp -o engine
# or
clang++ -std=c++23 -O2 -Wall -Wextra -pedantic main.cpp -o engine
````

## Run

```bash
./engine
```

## Notes

Primary reference: "Beginning C++23: From Beginner to Pro" (Horton & Van Weert).

```
```
