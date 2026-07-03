#pragma once
#include <list>
#include <print>
enum class CommandType{
New,Cancel,PrintBook,Quit,INVALID};
enum class Side {//M1
    BUY = 0, SELL = 1 };
enum class OrderStatus { //M1
    New,
    PartiallyFilled,
    Filled,
    Cancelled,};

class Order {

private:
int ID {};
Side side {};
int Price {};
int O_Quantity {};
int N_Quantity {};
int TS {}; 
OrderStatus status;

public:
Order () = default;

Order (int Id, Side s, int price, int o_quantity, int n_quantity, int ts, OrderStatus OS) : ID{Id}, side{s}, Price{price}, O_Quantity{o_quantity}, N_Quantity{n_quantity}, TS{ts}, status{OS} {}

public:

int remaining() const{return N_Quantity;}

void reduce_remaining (int filled);
     


void Filled();

int id() const{
    return ID;

};
int price() const{
    return Price;
}

Side get_side() const{
    return side;
}
};
struct PriceLevel {std::list<Order> fifo;};


