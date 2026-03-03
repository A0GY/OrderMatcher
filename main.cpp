#include <iostream>
#include <print>
#include <string>
#include <map>
#include <unordered_map>
#include <list>
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

        const int order_id = nextID++;
        int incoming = EngineCommand.Quantity;

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
