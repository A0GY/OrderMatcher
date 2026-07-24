#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include <optional>
#include "Location.hpp"
#include "Command.hpp"
#include "Trade.hpp"
#include <string>

struct MatchingEngine{
int nextID{1};

private:
std::map<int,PriceLevel> Bids; // BUYERS
std::map<int,PriceLevel> Ask; //SELLERS
std::unordered_map<int,Location> order_index; // to cancel an order
std::vector<Trade> Trade_Record; 

public:
std::string cancel_order(int order_id_);
void remove_index(int index_id_);

std::vector<std::string> print_book();
std::optional<Trade> last_trade();
int NewOrder(Command& EngineCommand);
size_t tradeCount()const;
std::optional<int>best_bid()const;
std::optional<int> best_ask()const;
bool hasOrder(int order_id_);
std::optional<int> restingQuantityAt(Side side, int price);
};