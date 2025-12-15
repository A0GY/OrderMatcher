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



enum class CommantType //M2
{
New,Cancle,PrintBook,Ouit
};

 enum class Side {//M1
    BUY = 0, SELL = 1 
};

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
std::map<int,PriceLevel> Bids;
std::map<int,PriceLevel> Ask;

int NewOrder(Command& EngineCommand)
{
    
    if (EngineCommand.side == Side::BUY){

        Order buy(nextID,Side::BUY,EngineCommand.price,EngineCommand.Quantity,EngineCommand.Quantity,0, OrderStatus::New);
        //Getters and setter for order because its private        
        auto& level = Bids[EngineCommand.price];
        level.fifo.push_back(buy);
        int NewID = nextID;
        nextID++;
        return nextID;
   
   
   
    }






    }




};




int main () {
  MachingEngine engine;
    while(true){
   
   // Code to spilt tokens for mapping object variables
   std::cout << "Please input youe chosen action (if unsure please check docs)" << std::endl;
   Command command;
   std::string CMD {""}; 
   std::getline(std::cin, CMD);
    size_t start {0};
        auto com_pos1 = CMD.find(" ",0);
        std::string Token1 = CMD.substr(start, com_pos1);
        
        if (Token1 == "NEW"){

            command.type = CommantType::New;
            std::cout << "Command was set to NEW successfully" << std::endl;
            
            
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

            std::println("You have made a {} order of {} {} shares at a price of {}", Token1,Token2,Token4,Token3 );

            
            }
            
            
            else if (Token2 == "SELL")
            { 
            command.side = Side::SELL;
            
            
            command.price = Token3;
            command.Quantity = Token4;

            std::println("You have made a {} order of {} {} shares at a price of {}", Token1,Token2,Token4,Token3 );
            
            }
            else { std::cout << "Error your second statment must either be BUY/SELL" << std::endl;
            continue;}
           }
        else if (Token1 == "CANCEL")
        {
            command.type = CommantType::Cancle;
            size_t start2 = com_pos1 + 1;
            std::string Token2 = CMD.substr(start2);
            command.OrderID = stoi(Token2);
            std::println("You have put a requrest to {} order {}",Token1,Token2 );
             
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
        std::println("command has now been formed");

     // Now we disptaach to the engine but we need to actually create the engine methods first we will use a switch case and then send off 
     
    switch (command.type)
    {
        case (CommantType::New): std::cout << "Test call working for new" << std::endl;
        case (CommantType::Cancle): std::cout << "Test call working for Cancel" << std::endl;
        case (CommantType::Ouit): std::cout << "Test call working for Quit" << std::endl;
        case (CommantType::PrintBook): std::cout << "Test call working for PB" << std::endl;

    } 
    
    



    }

    return 0;
}