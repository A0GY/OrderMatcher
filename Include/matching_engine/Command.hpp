#pragma once
#include "Order.hpp"
struct Command { //M2
    CommandType type;
    Side side;
    int price;
    int Quantity;
    int OrderID; };