#pragma once;
#include "Order.hpp"
struct Command { //M2
    CommantType type;
    Side side;
    int price;
    int Quantity;
    int OrderID; };