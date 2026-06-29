#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include <optional>
#include "Location.hpp"
#include "Command.hpp"
#include "Trade.hpp"

struct MatchingEngine{
int nextID{1};

private:
std::map<int,PriceLevel> Bids; // BUYERS
std::map<int,PriceLevel> Ask; //SELLERS
std::unordered_map<int,Location> order_index; // to cancel an order
std::vector<Trade> Trade_Record; 

public:
void cancel_order(int order_id_);
void remove_index(int index_id_);
int NewOrder(Command& EngineCommand);
void print_book();

size_t trade_count()const;
std::optional<int>best_bid()const;
std::optional<int> best_ask()const;
bool hasOrder(int order_id_);
std::optional<int> restingQuantityAt(Side side, int price);
};