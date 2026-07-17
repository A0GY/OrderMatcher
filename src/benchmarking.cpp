#include <iostream>
#include <print>
#include <string>
#include <ostream>
#include <charconv>
#include <random>
#include <chrono>
#include <vector>
#include "MatchingEngine.hpp"

int main(){
std::mt19937 gen(55);
std::uniform_int_distribution<> price(1,100);
std::bernoulli_distribution side(0.5);
std::vector<Command> CommandVec;
CommandVec.reserve(1000000);
MatchingEngine engine;
int ID {1};
for (int i {}; i!= 1000000; ++i ){


Command command;
command.OrderID = ID;
command.price = price(gen);
command.Quantity = price(gen); // same var just for simplicty it doesnt hurt
command.side = side(gen) ? Side::BUY : Side::SELL;
command.type = CommandType::New;
ID++;
CommandVec.push_back(command);}

auto start = std::chrono::steady_clock::now();

for (auto &e : CommandVec){

engine.NewOrder(e);
}
auto end = std::chrono::steady_clock::now();
std:: cout << "Command count: " << static_cast<int>(CommandVec.size()) << "\n"; 
std::chrono::duration<double> elapsed = end - start;
std:: cout <<"elapsed seconds: " << elapsed.count() << "\n" ;
const double oper_per_sec = static_cast<double>(CommandVec.size()) / elapsed.count();
std:: cout <<"operation per second: " << oper_per_sec;


return 0;

}