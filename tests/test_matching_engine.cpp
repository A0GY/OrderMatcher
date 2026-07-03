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

  REQUIRE(engine.tradeCount() == 0);


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

  REQUIRE( engine.tradeCount() == 0);

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

  REQUIRE( engine.tradeCount() == 1);

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

  REQUIRE( engine.tradeCount() == 1);
  REQUIRE(engine.last_trade()->price == 90);

}


TEST_CASE("Full fill rests nothing"){
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

  REQUIRE(engine.hasOrder(1) == false);
  REQUIRE(engine.hasOrder(2) == false);


}
//Partial fill
TEST_CASE("Partial fill is made"){

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
  command2.Quantity = 11;
  command2.side = Side::BUY;
  command2.type = CommandType::New;

  engine.NewOrder(command);
  engine.NewOrder(command2);

  REQUIRE(engine.restingQuantityAt(Side::BUY, 100) == 1 );

}
//FIFO
TEST_CASE("Price prio valid"){

MatchingEngine engine;  
  Command command;

  command.OrderID = 1;
  command.price = 100;
  command.Quantity = 10;
  command.side = Side::BUY;
  command.type = CommandType::New;

  Command command2;
  command2.OrderID = 2; 
  command2.price = 100;
  command2.Quantity = 10;
  command2.side = Side::BUY;
  command2.type = CommandType::New;

  Command command3;
  command3.OrderID = 3; 
  command3.price = 100;
  command3.Quantity = 11;
  command3.side = Side::SELL;
  command3.type = CommandType::New;


  engine.NewOrder(command);
  engine.NewOrder(command2);
  engine.NewOrder(command3);

  REQUIRE(engine.hasOrder(1) == false);
  REQUIRE(engine.last_trade()->OrderB_ID == 2);


}




//////

// cancel removes order
TEST_CASE("Cancel removes order"){
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
  command2.Quantity = 11;
  command2.side = Side::BUY;
  command2.type = CommandType::New;

  engine.NewOrder(command);
  engine.cancel_order(1);
  REQUIRE(engine.hasOrder(1) == false);


}
//cancelled order wont match
TEST_CASE("Cancelled order wont match"){
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
  command2.Quantity = 11;
  command2.side = Side::BUY;
  command2.type = CommandType::New;

  engine.NewOrder(command);
  engine.cancel_order(1);
  REQUIRE(engine.hasOrder(1) == false);
  engine.NewOrder(command2);
  REQUIRE(engine.last_trade() == std::nullopt);
}
//cancel non existingID

TEST_CASE("Attemp to cancel non existing ID"){
  MatchingEngine engine;   
  Command command;

  command.OrderID = 1;
  command.price = 90;
  command.Quantity = 10;
  command.side = Side::SELL;
  command.type = CommandType::New;
  engine.NewOrder(command);
  REQUIRE_NOTHROW(engine.cancel_order(12));
  REQUIRE(engine.hasOrder(1) == true);
  REQUIRE(engine.tradeCount()==0);


}

