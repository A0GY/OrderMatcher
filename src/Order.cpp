#include "Order.hpp"

void  Order::reduce_remaining (int filled) {

    if (filled > 0 && filled <= N_Quantity){
    N_Quantity -= filled;
    // this would make sense? OrderStatus::Filled;
   
}
     
}

void  Order::Filled() 
{
std::println("Order {} has been filled", ID);
status = OrderStatus::Filled;
}

