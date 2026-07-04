#include "MatchingEngine.hpp"



void MatchingEngine::cancel_order(int order_id_)
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

void MatchingEngine::remove_index(int index_id_){
    auto find_index = order_index.find(index_id_);
    if (find_index != order_index.end()){
    order_index.erase(index_id_);
    std::println("Order {} was removed from the order index map", index_id_);
    }
    else {
        std::println("error index {} was not in the order index", index_id_);
    }

}

void MatchingEngine::print_book(){
    
    for (size_t i{}; i < Trade_Record.size(); i++){
        auto obj = Trade_Record.at(i);
        std::println("Buy Order {} \n Sell Order {} \n Quantity {} \n Price: {} \n Time of order {}", obj.OrderB_ID, obj.OrderS_ID, obj.price, obj.Quantity, obj.TS);

    }

}

int MatchingEngine::NewOrder(Command& EngineCommand)
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
                    
                    //add record to trade.
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
                    //FIX logical issue as this will make a Parital fill even though it has 0 filles for there not being a mached price which isnt logically correct it should just form an order not partial order
                    // we need to turn this into a partial order as it has not been fully filled and there are no asks/sells open 
                    Order buy(NewID,Side::BUY,EngineCommand.price,incoming,incoming,0, OrderStatus::PartiallyFilled);
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
                    return NewID;}

            




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
                    Order sell(NewID,Side::SELL,EngineCommand.price,incoming,incoming,0, OrderStatus::PartiallyFilled);
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

size_t MatchingEngine::tradeCount() const{


    size_t TC = Trade_Record.size();
    return TC;
};

 std::optional<int> MatchingEngine::best_bid () const{ // optional useage when bids/ask empty
    if (Bids.empty()){
    std::println("Bids is currently empty");
    return std::nullopt;
}
    
    else {
    int bd = std::prev(Bids.end())->first;
    return bd;
    }
};

 std::optional<int> MatchingEngine::best_ask () const{

    if (!Ask.empty()){ int ba = Ask.begin()->first;
    return ba;}

    else {
        std::println("Asks is currently empty");
        return std::nullopt;

    }
    

}

bool MatchingEngine::hasOrder(int order_id_){
std::println("Order {} is being located ", order_id_);
auto find_order = order_index.find(order_id_);
    if (find_order != order_index.end()){
       std::println("This order exists and is resting");
        return true;
         }

    else {std::println("This is not a valid order ");
    return false;
    }
}

std::optional<int> MatchingEngine::restingQuantityAt(Side side, int price){

int sum {};
if (side == Side::BUY){
    
    if (Bids.find(price) == Bids.end()){
        return std::nullopt;

    }
    
    else {
    auto& priceLoc = Bids.at(price).fifo;
    for (auto i = priceLoc.begin(); i != priceLoc.end(); i++){
        sum += i->remaining();
    }}
    }
else 
{

    if (Ask.find(price) == Ask.end()){
        return std::nullopt;

    }
    
    else {
    auto& priceLoc = Ask.at(price).fifo;
    for (auto i = priceLoc.begin(); i != priceLoc.end(); i++){
        sum += i->remaining();
    }}
}
return sum;

}

std::optional<Trade> MatchingEngine::last_trade(){
if (Trade_Record.empty())
{
    return std::nullopt;
    std::println("Trade is currently empty");
}
else {
Trade &vecLast {Trade_Record.back()};
return vecLast;

}
}