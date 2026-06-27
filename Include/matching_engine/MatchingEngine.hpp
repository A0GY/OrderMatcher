#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include "Order.hpp"
#include "Location.hpp"
#include "Command.hpp"

struct MachingEngine{
int nextID{1};
std::map<int,PriceLevel> Bids; // BUYERS
std::map<int,PriceLevel> Ask; //SELLERS
std::unordered_map<int,Location> order_index; // to cancel an order
std::vector<Trade> Trade_Record; 

void cancel_order(int order_id_);
void remove_index(int index_id_);
int NewOrder(Command& EngineCommand);

};