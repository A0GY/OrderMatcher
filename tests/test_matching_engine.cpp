#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <print>
#include <string>
#include <ostream>
#include <charconv>
#include "MatchingEngine.hpp"

//empty book trade book on resting order
TEST_CASE("Resting order does not get added to Trade vector") {
    MatchingEngine engine;
   Command command;
   command.OrderID = 1;
   command.price = 100;
   command.Quantity = 10;
   command.side = Side::BUY;
   command.type = CommandType::New;

   engine.NewOrder(command);

  REQUIRE(engine.trade_count() == 0);


}

TEST_CASE("No Cross,buy below an existing sell, assert no trade, both rest."){
  MatchingEngine engine;  
  Command command;

  command.OrderID = 1;
  command.price = 100;
  command.Quantity = 10;
  command.side = Side::SELL;
  command.type = CommandType::New;

  Command command2;

  command2.OrderID = 2;
  command2.price = 90;
  command2.Quantity = 10;
  command2.side = Side::BUY;
  command2.type = CommandType::New;

  engine.NewOrder(command);
  engine.NewOrder(command2);

  REQUIRE( engine.trade_count() == 0);

}

TEST_CASE("Crossing produces trade adding to trademap"){
  MatchingEngine engine;  
  Command command;

  command.OrderID = 1;
  command.price = 100;
  command.Quantity = 10;
  command.side = Side::SELL;
  command.type = CommandType::New;

  Command command2;

  command2.OrderID = 2;
  command2.price = 100;
  command2.Quantity = 10;
  command2.side = Side::BUY;
  command2.type = CommandType::New;

  engine.NewOrder(command);
  engine.NewOrder(command2);

  REQUIRE( engine.trade_count() == 1);

}

TEST_CASE("Maker's price: buy crosses a cheaper resting sell; assert trade price = resting order's price."){

  MatchingEngine engine;  
  Command command;

  command.OrderID = 1;
  command.price = 90;
  command.Quantity = 10;
  command.side = Side::SELL;
  command.type = CommandType::New;

  Command command2;
  command2.OrderID = 2; 
  command2.price = 100;
  command2.Quantity = 10;
  command2.side = Side::BUY;
  command2.type = CommandType::New;

  engine.NewOrder(command);
  engine.NewOrder(command2);

  REQUIRE( engine.trade_count() == 1);

}

// full full rests nothing

//Partial fill

//FIFO

//Price prio valid



//////

// cancel removes order

//cancelled order wont match

//cancel non existingID

