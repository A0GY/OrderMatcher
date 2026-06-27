#pragma once
#include "Order.hpp"
struct Location //To help cancel a order 
{
    Side side;          
    int Price;
    std::list<Order>::iterator it;

};