#include <iostream>
#include <vector>
#include <unordered_map>
#include <deque>
#include <chrono>
#include <optional>
#include <print>
#include <string>
#include <map>
#include <unordered_map>
#include <list>
#include <algorithm>
#include <ostream>
#include <charconv>



enum class CommantType //M2
{
New,Cancle,PrintBook,Ouit,INVALID};

enum class Side {//M1
    BUY = 0, SELL = 1 };

struct Command { //M2
    CommantType type;
    Side side;
    int price;
    int Quantity;
    int OrderID; // If to cancel a Order

// New Order: NEW SIDE 100 10
// Cancel: Cancel 7
// PrintBook/Quit 

};

enum class OrderStatus { //M1
    New,
    PatiallyFilled,
    Filled,
    Cannclled,

};

class  Order { //M1
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

int remaining() const{
    return N_Quantity;
}

void reduce_remaining (int filled) {

    if (filled > 0 && filled <= N_Quantity){
    N_Quantity -= filled;
    // this would make sense? OrderStatus::Filled;
   
}
     
}

void Filled() 
{
std::println("Order {} has been filled", ID);
status = OrderStatus::Filled;
}

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

struct Trade{ //M1
int OrderB_ID {};
int OrderS_ID {};
int price {};
int Quantity {};
std::time_t TS {};
};

struct PriceLevel {std::list<Order> fifo;};

struct Location //To help cancel a order 
{
    Side side;          
    int Price;
    std::list<Order>::iterator it;

};

struct MachingEngine 
{
int nextID {1};
std::map<int,PriceLevel> Bids; // BUYERS
std::map<int,PriceLevel> Ask; //SELLERS
std::unordered_map<int,Location> order_index; // to cancel an order
std::vector<Trade> Trade_Record; //trade recorded

// member function to access order_index to cancel an order
void cancel_order(int order_id_)
{
    std::println("Attempting to cancel order number {}", order_id_);

    auto find_order = order_index.find(order_id_);
    if (find_order != order_index.end()){
        // map out data
        auto order_loc = find_order->second.it;
        auto order_side = find_order->second.side;
        auto order_price = find_order->second.Price;

        if (order_side == Side::BUY){
            auto Bids_Order = Bids.find(order_price);
            if(Bids_Order == Bids.end()){
                std::println("This order was not found");
                return;
            }
            auto& Price_found = Bids_Order->second.fifo;
            Price_found.erase(order_loc);
            std::println("Order {} has now been erased", order_id_);
            order_index.erase(order_id_);
        }

        else if (order_side == Side::SELL){
            auto Asks_Order = Ask.find(order_price);
            if(Asks_Order == Ask.end()){
                std::println("This order was not found");
                return;
            }
            auto& Price_found = Asks_Order->second.fifo;
            Price_found.erase(order_loc);
            std::println("Order {} has now been erased", order_id_);
            order_index.erase(order_id_);
        }

    }

    else {std::println("This is not a valid order ");
    
    }

    





}

void remove_index(int index_id_){
    auto find_index = order_index.find(index_id_);
    if (find_index != order_index.end()){
    order_index.erase(index_id_);
    std::println("Order {} was removed from the order index map", index_id_);
    }
    else {
        std::println("error index {} was not in the order index", index_id_);
    }

}

void print_book(){
    
    for (size_t i{}; i < Trade_Record.size(); i++){
        auto obj = Trade_Record.at(i);
        std::println("Buy Order {} \n Sell Order {} \n Quantity {} \n Price: {} \n Time of order {}", obj.OrderB_ID, obj.OrderS_ID, obj.price, obj.Quantity, obj.TS);

    }

}


int NewOrder(Command& EngineCommand)
{
int NewID = nextID;  
nextID++;
    if (EngineCommand.side == Side::BUY){
        if (Ask.empty()){
            
            Order buy(NewID,Side::BUY,EngineCommand.price,EngineCommand.Quantity,EngineCommand.Quantity,0, OrderStatus::New);
            //Getters and setter for order because its private        
            auto& level = Bids[EngineCommand.price];
            level.fifo.push_back(buy);
            std::println("Your Buy order has been added to the book \n \n");
            
            Location location;
            location.side = Side::BUY;
            location.Price = EngineCommand.price;
            auto indexP = std::prev(level.fifo.end());
            location.it = indexP;
            order_index.insert({NewID,location}); // lovley syntax 
            return NewID;}
            
        else if (!Ask.empty()){
            int incoming = EngineCommand.Quantity;
            for (auto i = Ask.begin(); i != Ask.end(); i++ ){
                if (i->first <= EngineCommand.price && incoming > 0){
                    auto& fifo = i->second.fifo;
                    while (!fifo.empty() && incoming > 0){
                    
                    auto& maker = fifo.front(); // access the oldest order at price lvl
                    // need to get the quant and then subtract it from the buys quant.
                    int Ask_qant = maker.remaining();
                    auto fill = std::min(incoming, Ask_qant);
                    incoming -= fill;
                    maker.reduce_remaining(fill);
                    
                    //add record to trade 
                    Trade trade_ex;
                    trade_ex.OrderS_ID = maker.id();
                    trade_ex.OrderB_ID = NewID;
                    trade_ex.price = maker.price();
                    auto live_ts = std::chrono::system_clock::now();
                    trade_ex.TS = std::chrono::duration_cast<std::chrono::microseconds>(live_ts.time_since_epoch()).count(); // Look over this syntax and use of auto 
                    trade_ex.Quantity = fill;
                    Trade_Record.push_back(trade_ex);
                    
                    if (maker.remaining() == 0){
                        maker.Filled();
                        remove_index(maker.id());
                        fifo.pop_front();}        
                }               
        }            
     }
            //ADD logic for > 0 incoming to make parital order if fifio empty 
                if (incoming > 0){
                    
                    // we need to turn this into a partial order as it has not been fully filled and there are no asks/sells open 
                    Order buy(NewID,Side::BUY,EngineCommand.price,incoming,incoming,0, OrderStatus::PatiallyFilled);
                     std::println("Partial order was made to fill quant of {} of order {}",incoming,nextID);
                    // maybe just turn this into a case to either make partial or remove object/add it to orders tracker
                    auto& level = Bids[EngineCommand.price];
                    level.fifo.push_back(buy);
                     
            Location location;
            location.side = Side::BUY;
            location.Price = EngineCommand.price;
            auto indexP = std::prev(level.fifo.end());
            location.it = indexP;
            order_index.insert({NewID,location}); // lovley syntax     
            return NewID;

                        
        }
    }
}
    
    if (EngineCommand.side == Side::SELL){
    if (Bids.empty()){

          
            Order sell(NewID,Side::SELL,EngineCommand.price,EngineCommand.Quantity,EngineCommand.Quantity,0, OrderStatus::New);
            //Getters and setter for order because its private        
            auto& level = Ask[EngineCommand.price];
            level.fifo.push_back(sell);
             
            Location location;
            location.side = Side::SELL;
            location.Price = EngineCommand.price;
            auto indexP = std::prev(level.fifo.end());
            location.it = indexP;
            order_index.insert({NewID,location});
            return NewID;
        }
        else if (!Bids.empty()){
             int incoming = EngineCommand.Quantity;
            for (auto i = Bids.rbegin(); i != Bids.rend(); i++ ){
                if (i->first >= EngineCommand.price && incoming > 0){
                    auto& fifo = i->second.fifo;
                    while (!fifo.empty() && incoming > 0){
                    
                    auto& maker = fifo.front(); // access the oldest order at price lvl
                    // need to get the quant and then subtract it from the buys quant
                    int Ask_qant = maker.remaining();
                    auto fill = std::min(incoming, Ask_qant);
                    incoming -= fill;
                    maker.reduce_remaining(fill);
                    
                    //add record to trade 
                    Trade trade_ex;
                    trade_ex.OrderB_ID = maker.id();
                    trade_ex.OrderS_ID = NewID;
                    trade_ex.price = maker.price();
                    auto live_ts = std::chrono::system_clock::now();
                    trade_ex.TS = std::chrono::duration_cast<std::chrono::microseconds>(live_ts.time_since_epoch()).count(); // Look over this syntax and use of auto 
                    trade_ex.Quantity = fill;
                    Trade_Record.push_back(trade_ex);
                    
                    if (maker.remaining() == 0){
                        //return maker id/not
                        maker.Filled();
                        remove_index(maker.id());
                        fifo.pop_front();}
                    }    
                }

            
            }
            
            //ADD logic for > 0 incoming to make parital order if fifio empty 
                if (incoming > 0){
                    
                    // we need to turn this into a partial order as it has not been fully filled and there are no asks/sells open 
                    Order sell(NewID,Side::SELL,EngineCommand.price,incoming,incoming,0, OrderStatus::PatiallyFilled);
                     std::println("Partial order was made to fill quant of {} of order {}",incoming,nextID);
                    // maybe just turn this into a case to either make partial or remove object/add it to orders tracker
                    auto& level = Ask[EngineCommand.price];
                    level.fifo.push_back(sell);
                     
                    Location location;
                    location.side = Side::SELL;
                    location.Price = EngineCommand.price;
                    auto indexP = std::prev(level.fifo.end());
                    location.it = indexP;
                    order_index.insert({NewID,location});
                    return NewID;
                    
        }
    }
}
return 0;
}
};

int main () {
  auto live_ts = std::chrono::system_clock::now();
  
  std::cout << "time is : " << live_ts << "\n";
  
    MachingEngine engine;
    while(true){
   
   // Code to spilt tokens for mapping object variables
   std::cout << "Please input youe chosen action (if unsure please check docs) \n"  << std::endl;
   Command command;
   std::string CMD {""}; 
   std::getline(std::cin, CMD);
    size_t start {0};
        auto com_pos1 = CMD.find(" ",0);
        std::string Token1 = CMD.substr(start, com_pos1);
        
        if (Token1 == "NEW"){

            command.type = CommantType::New;
            std::cout << "Command was set to NEW successfully \n"  << std::endl;
            
            
            size_t start2 = com_pos1 + 1;
            auto com_pos2 = CMD.find(" ", start2);
            std::string Token2 = CMD.substr(start2, com_pos2 - start2);
            size_t start3 = com_pos2 + 1;
            auto com_pos3 = CMD.find(" ", start3);
            
            const std::string Token3Str = CMD.substr(start3, com_pos3 - start3);
            int Token3 {};
            auto T3temp = std::from_chars(Token3Str.data(), Token3Str.data() + Token3Str.size(), Token3);// Using from_chat is a better perfomance choice then stoi which was prev used, no memory alloc and no crash of error/throw
            if (T3temp.ec != std::errc{} || Token3 <= 0 ){// check to see if ec that was made is the same as defult as would indicat no errors
                std::println("You have not formatted your input correctly \n Correct Example: NEW BUY 10 100  \n Warning: No Negative numbers allowd ");
                continue;

            }
            
            size_t start4 = com_pos3 + 1;
            int  Token4 {};
            std::string Token4Str = CMD.substr(start4);
            auto T4temp = std::from_chars(Token4Str.data(), Token4Str.data() + Token4Str.size(), Token4);            
            if (T4temp.ec != std::errc{}  ||  Token4 <= 0){// check to see if ec that was made is the same as defult as would indicat no errors
                std::println("You have not formatted your input correctly \n Correct Example: NEW BUY 10 100 \n Warning: No Negative numbers allowd  ");
                continue;

            }
            if (Token2 == "BUY"){
            command.side = Side::BUY;
            command.price = Token3;
            command.Quantity = Token4;
            std::println("You have made a {} order of {} {} shares at a price of {} \n ",  Token1,Token2,Token4,Token3 );
            }
            
            else if (Token2 == "SELL")
            { 
            command.side = Side::SELL;
            command.price = Token3;
            command.Quantity = Token4;
            std::println("You have made a {} order of {} {} shares at a price of {} \n" , Token1,Token2,Token4,Token3 );
            }
            
            else { std::cout << "Error your second statment must either be BUY/SELL \n" << std::endl;
            continue;}
           }
        
           else if (Token1 == "CANCEL")
        {
            int Token2{};
            command.type = CommantType::Cancle;
            size_t start2 = com_pos1 + 1;
            std::string Token2Str = CMD.substr(start2);
            auto T2temp = std::from_chars(Token2Str.data(), Token2Str.data() + Token2Str.size(), Token2); 
            if (T2temp.ec != std::errc{}){// check to see if ec that was made is the same as defult as would indicat no errors
                std::println("You have not formatted your input correctly \n Correct Example: CANCEL 2  ");
                continue;

            }
            command.OrderID = Token2;
            std::println("You have put a requrest to {} order {} \n",Token1,Token2 );
            
             
        }
        
        else if (Token1 == "PRINTBOOK")
        {
            command.type = CommantType::PrintBook;
        }

        else if (Token1 == "QUIT")
        {
            command.type = CommantType::Ouit;
            
        }

        else {
            command.type = CommantType::INVALID;
            std::println("You did not pick a valid option please refear to docs");
            continue;
        }

        //--------------------------------------------------------------------
        std::println("command has now been formed \n \n ");

     // Now we disptaach to the engine but we need to actually create the engine methods first we will use a switch case and then send off 
     
    switch (command.type)
    {
        case (CommantType::New): std::cout << "Test call working for new \n " << std::endl; 
        engine.NewOrder(command);
        break;



        case (CommantType::Cancle): 
        engine.cancel_order(command.OrderID);
         break;


        case (CommantType::Ouit):
        std::println("Progam is closing");
        std::exit(1);
        break;

        case (CommantType::PrintBook): std::cout << "Test call working for PB" << std::endl;
        engine.print_book();
        break;
        
        case (CommantType::INVALID): std::cout << "You have not formatted an input correctly" << std::endl;
        
        break;

        default:
        std::println("You have not formatted an input correctly");
        break;

    } 
    
    



    }

    return 0;
}