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

enum class CommantType //M2
{
New,Cancle,PrintBook,Ouit};

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
    std::println("Order {} has now been filled ", ID);
}
     
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
int TS {};
};

struct PriceLevel {std::list<Order> fifo;};

struct MachingEngine 
{
int nextID {1};
std::map<int,PriceLevel> Bids; // BUYERS
std::map<int,PriceLevel> Ask; //SELLERS

int NewOrder(Command& EngineCommand)
{
    if (EngineCommand.side == Side::BUY){


        if (Ask.empty()){
        int NewID = nextID;    
            Order buy(nextID,Side::BUY,EngineCommand.price,EngineCommand.Quantity,EngineCommand.Quantity,0, OrderStatus::New);
            //Getters and setter for order because its private        
            auto& level = Bids[EngineCommand.price];
            level.fifo.push_back(buy);
            std::println("Your Buy order has been added to the book \n \n");
            nextID++;
            return NewID;}
            
        else if (!Ask.empty()){
            int incoming = EngineCommand.Quantity;
            for (auto i = Ask.begin(); i != Ask.end(); i++ ){
                if (i->first <= EngineCommand.price && incoming > 0){
                    auto& fifo = i->second.fifo;
                    while (!fifo.empty() && incoming > 0){
                    
                    auto& maker = fifo.front(); // access the oldest order at price lvl
                    // need to get the quant and then subtract it from the buys quant
                    int Ask_qant = maker.remaining();
                    auto fill = std::min(incoming, Ask_qant);
                    incoming -= fill;
                    maker.reduce_remaining(fill);
                    if (maker.remaining() == 0){
                        fifo.pop_front();}
                        
                    }
                    std::println("logic check ");
                    // add logic for checking if the incoming is still > 0 and fifo may become empty making partial 
                }

            
            }
        }
    }
    
    if (EngineCommand.side == Side::SELL){
    if (Bids.empty()){

        int NewID = nextID;    
            Order buy(NewID,Side::SELL,EngineCommand.price,EngineCommand.Quantity,EngineCommand.Quantity,0, OrderStatus::New);
            //Getters and setter for order because its private        
            auto& level = Ask[EngineCommand.price];
            level.fifo.push_back(buy);
            
            nextID++;
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
                    if (maker.remaining() == 0){
                        //return maker id
                        fifo.pop_front();}
                    }    
                }

            
            }
        }
    }

return 0;}


};

int main () {
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
            int Token3 = stod(CMD.substr(start3, com_pos3 - start3));
            size_t start4 = com_pos3 + 1;
            int  Token4 = stoi(CMD.substr(start4));
            
            
            if (Token2 == "BUY")
            {command.side = Side::BUY;
          
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
            command.type = CommantType::Cancle;
            size_t start2 = com_pos1 + 1;
            std::string Token2 = CMD.substr(start2);
            command.OrderID = stoi(Token2);
            std::println("You have put a requrest to {} order {} \n",Token1,Token2 );
             
        }
        
        else if (Token1 == "PRINT BOOK")
        {
            std::cout << "TBC" << std::endl;
        }

        else if (Token1 == "QUIT")
        {
            std::cout << "TBC" << std::endl;
            
        }

        //--------------------------------------------------------------------
        std::println("command has now been formed \n \n ");

     // Now we disptaach to the engine but we need to actually create the engine methods first we will use a switch case and then send off 
     
    switch (command.type)
    {
        case (CommantType::New): std::cout << "Test call working for new \n " << std::endl; 
        engine.NewOrder(command);
        break;



        case (CommantType::Cancle): std::cout << "Test call working for Cancel" << std::endl; break;
        case (CommantType::Ouit): std::cout << "Test call working for Quit" << std::endl; break;
        case (CommantType::PrintBook): std::cout << "Test call working for PB" << std::endl; break;

    } 
    
    



    }

    return 0;
}