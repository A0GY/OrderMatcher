#include <iostream>
#include <print>
#include <string>
#include <map>
#include <unordered_map>
#include <list>
<<<<<<< HEAD
#include <algorithm>

enum class CommantType //M2
{
New,Cancle,PrintBook,Ouit};

enum class Side {//M1
    BUY = 0, SELL = 1 };
=======
#include <vector>
#include <algorithm>
#include <sstream>

enum class CommantType //M2
{
    New, Cancle, PrintBook, Ouit, Unknown
};

enum class Side {//M1
    BUY = 0, SELL = 1
};
>>>>>>> b39362db7d9c6b18fe8902aea36012d8c39bec63

struct Command { //M2
    CommantType type {CommantType::Unknown};
    Side side {Side::BUY};
    int price {0};
    int Quantity {0};
    int OrderID {0}; // If to cancel a Order
};

enum class OrderStatus { //M1
    New,
    PatiallyFilled,
    Filled,
    Cannclled,
};

class Order { //M1
private:
    int ID {};
    Side side {};
    int Price {};
    int O_Quantity {};
    int N_Quantity {};
    int TS {};
    OrderStatus status {OrderStatus::New};

public:
    Order() = default;

<<<<<<< HEAD
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

=======
    Order(int Id, Side s, int price, int o_quantity, int n_quantity, int ts, OrderStatus OS)
        : ID{Id}, side{s}, Price{price}, O_Quantity{o_quantity}, N_Quantity{n_quantity}, TS{ts}, status{OS} {}

public:
    int remaining() const { return N_Quantity; }
    int id() const { return ID; }
    int price() const { return Price; }
    Side get_side() const { return side; }
    int ts() const { return TS; }
    OrderStatus get_status() const { return status; }

    void reduce_remaining(int filled) {
        if (filled > 0 && filled <= N_Quantity) {
            N_Quantity -= filled;
            if (N_Quantity == 0) status = OrderStatus::Filled;
            else status = OrderStatus::PatiallyFilled;
        }
    }

    void cancel() { status = OrderStatus::Cannclled; }
>>>>>>> b39362db7d9c6b18fe8902aea36012d8c39bec63
};

struct Trade { //M1
    int OrderB_ID {};
    int OrderS_ID {};
    int price {};
    int Quantity {};
    int TS {};
};

struct PriceLevel {
    std::list<Order> fifo;
};

struct MachingEngine
{
<<<<<<< HEAD
int nextID {1};
std::map<int,PriceLevel> Bids; // BUYERS
std::map<int,PriceLevel> Ask; //SELLERS

int NewOrder(Command& EngineCommand)
{
    if (EngineCommand.side == Side::BUY){
=======
    struct Locator {
        Side side {};
        int price {};
        std::list<Order>::iterator it;
    };

    int nextID {1};
    int seqTS {0};

    std::map<int, PriceLevel> Bids; // ascending; best bid is rbegin()
    std::map<int, PriceLevel> Ask;  // ascending; best ask is begin()

    std::unordered_map<int, Locator> index; // orderId -> (side, price, list iterator)
    std::vector<Trade> last_trades;

    int next_ts() { return ++seqTS; }

    const std::vector<Trade>& get_last_trades() const { return last_trades; }

    int NewOrder(const Command& EngineCommand)
    {
        last_trades.clear();
>>>>>>> b39362db7d9c6b18fe8902aea36012d8c39bec63

        const int order_id = nextID++;
        int incoming = EngineCommand.Quantity;

<<<<<<< HEAD
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
=======
        if (EngineCommand.side == Side::BUY)
        {
            // Match against asks while crossing: best ask <= buy limit
            while (incoming > 0 && !Ask.empty())
            {
                auto level_it = Ask.begin();              // best ask
                const int level_price = level_it->first;
                if (level_price > EngineCommand.price) break; // no longer crossing

                auto& fifo = level_it->second.fifo;

                while (incoming > 0 && !fifo.empty())
                {
                    auto& maker = fifo.front(); // oldest maker at this price

                    const int maker_id = maker.id();
                    const int maker_rem = maker.remaining();
                    const int fill = std::min(incoming, maker_rem);

                    incoming -= fill;
                    maker.reduce_remaining(fill);

                    last_trades.push_back(Trade{order_id, maker_id, level_price, fill, next_ts()});

                    if (maker.remaining() == 0)
                    {
                        index.erase(maker_id);
                        fifo.pop_front();
                    }
                }

                if (fifo.empty())
                {
                    Ask.erase(level_it);
                }
            }

            // Rest leftover as bid
            if (incoming > 0)
            {
                Order rest(order_id, Side::BUY, EngineCommand.price,
                           EngineCommand.Quantity, incoming, next_ts(), OrderStatus::New);

                auto& level = Bids[EngineCommand.price];
                level.fifo.push_back(rest);
                auto it = std::prev(level.fifo.end());

                index[order_id] = Locator{Side::BUY, EngineCommand.price, it};
            }

            return order_id;
        }
>>>>>>> b39362db7d9c6b18fe8902aea36012d8c39bec63

        if (EngineCommand.side == Side::SELL)
        {
            // Match against bids while crossing: best bid >= sell limit
            while (incoming > 0 && !Bids.empty())
            {
                auto level_it = std::prev(Bids.end());     // best bid (highest price)
                const int level_price = level_it->first;
                if (level_price < EngineCommand.price) break; // no longer crossing

                auto& fifo = level_it->second.fifo;

                while (incoming > 0 && !fifo.empty())
                {
                    auto& maker = fifo.front(); // oldest maker at this price

                    const int maker_id = maker.id();
                    const int maker_rem = maker.remaining();
                    const int fill = std::min(incoming, maker_rem);

                    incoming -= fill;
                    maker.reduce_remaining(fill);

                    last_trades.push_back(Trade{maker_id, order_id, level_price, fill, next_ts()});

                    if (maker.remaining() == 0)
                    {
                        index.erase(maker_id);
                        fifo.pop_front();
                    }
                }

                if (fifo.empty())
                {
                    Bids.erase(level_it);
                }
            }

            // Rest leftover as ask
            if (incoming > 0)
            {
                Order rest(order_id, Side::SELL, EngineCommand.price,
                           EngineCommand.Quantity, incoming, next_ts(), OrderStatus::New);

                auto& level = Ask[EngineCommand.price];
                level.fifo.push_back(rest);
                auto it = std::prev(level.fifo.end());

                index[order_id] = Locator{Side::SELL, EngineCommand.price, it};
            }

            return order_id;
        }
<<<<<<< HEAD
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
=======

        return order_id;
    }

    bool Cancel(int order_id)
    {
        auto it = index.find(order_id);
        if (it == index.end()) return false;

        const Locator loc = it->second;

        if (loc.side == Side::BUY)
        {
            auto level_it = Bids.find(loc.price);
            if (level_it != Bids.end())
            {
                level_it->second.fifo.erase(loc.it);
                if (level_it->second.fifo.empty()) Bids.erase(level_it);
            }
        }
        else
        {
            auto level_it = Ask.find(loc.price);
            if (level_it != Ask.end())
            {
                level_it->second.fifo.erase(loc.it);
                if (level_it->second.fifo.empty()) Ask.erase(level_it);
            }
        }

        index.erase(it);
        return true;
    }
>>>>>>> b39362db7d9c6b18fe8902aea36012d8c39bec63

    void PrintBook(int depth = 10) const
    {
        std::println("---- BOOK (top {} levels) ----", depth);

        // Asks (ascending)
        std::println("ASKS:");
        int shown = 0;
        for (auto it = Ask.begin(); it != Ask.end() && shown < depth; ++it, ++shown)
        {
            int total = 0;
            for (const auto& o : it->second.fifo) total += o.remaining();
            std::println("  price {} qty {}", it->first, total);
        }

        // Bids (descending)
        std::println("BIDS:");
        shown = 0;
        for (auto it = Bids.rbegin(); it != Bids.rend() && shown < depth; ++it, ++shown)
        {
            int total = 0;
            for (const auto& o : it->second.fifo) total += o.remaining();
            std::println("  price {} qty {}", it->first, total);
        }

        std::println("------------------------------");
    }
};

<<<<<<< HEAD
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
    
    

=======
static std::vector<std::string> split_ws(const std::string& s)
{
    std::vector<std::string> out;
    std::istringstream iss(s);
    for (std::string tok; iss >> tok; ) out.push_back(tok);
    return out;
}

static Command parse_command(const std::string& line)
{
    Command cmd;
    auto t = split_ws(line);
    if (t.empty()) return cmd;

    if (t[0] == "NEW" && t.size() == 4)
    {
        cmd.type = CommantType::New;
>>>>>>> b39362db7d9c6b18fe8902aea36012d8c39bec63

        if (t[1] == "BUY") cmd.side = Side::BUY;
        else cmd.side = Side::SELL;

        cmd.price = std::stoi(t[2]);
        cmd.Quantity = std::stoi(t[3]);
        return cmd;
    }

    if (t[0] == "CANCEL" && t.size() == 2)
    {
        cmd.type = CommantType::Cancle;
        cmd.OrderID = std::stoi(t[1]);
        return cmd;
    }

    if ((t[0] == "PRINTBOOK") || (t[0] == "PRINT") || (t.size() == 2 && t[0] == "PRINT" && t[1] == "BOOK"))
    {
        cmd.type = CommantType::PrintBook;
        return cmd;
    }

    if (t[0] == "QUIT")
    {
        cmd.type = CommantType::Ouit;
        return cmd;
    }

    cmd.type = CommantType::Unknown;
    return cmd;
}

int main()
{
    MachingEngine engine;

    while (true)
    {
        std::cout << "Input: NEW BUY 100 10 | NEW SELL 100 5 | CANCEL 7 | PRINTBOOK | QUIT\n> ";

        std::string line;
        std::getline(std::cin, line);

        Command command = parse_command(line);

        switch (command.type)
        {
            case CommantType::New:
            {
                const int id = engine.NewOrder(command);
                std::println("Order accepted: id {}", id);

                const auto& trades = engine.get_last_trades();
                for (const auto& tr : trades)
                {
                    std::println("TRADE: buy_id {} sell_id {} price {} qty {} ts {}",
                                 tr.OrderB_ID, tr.OrderS_ID, tr.price, tr.Quantity, tr.TS);
                }
                break;
            }

            case CommantType::Cancle:
            {
                const bool ok = engine.Cancel(command.OrderID);
                if (ok) std::println("Cancel ok: id {}", command.OrderID);
                else std::println("Cancel reject: unknown id {}", command.OrderID);
                break;
            }

            case CommantType::PrintBook:
            {
                engine.PrintBook();
                break;
            }

            case CommantType::Ouit:
            {
                std::println("Bye.");
                return 0;
            }

            default:
            {
                std::println("Unknown command.");
                break;
            }
        }
    }
}
