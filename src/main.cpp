#include <iostream>
#include <print>
#include <string>
#include <ostream>
#include <charconv>
#include "MatchingEngine.hpp"
int main () {
  auto live_ts = std::chrono::system_clock::now();
  
  std::cout << "time is : " << live_ts << "\n";
  
    MatchingEngine engine;
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

            command.type = CommandType::New;
            
            
            
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
            
            }
            
            else if (Token2 == "SELL")
            { 
            command.side = Side::SELL;
            command.price = Token3;
            command.Quantity = Token4;
            
            }
            
            else { std::cout << "Error your second statment must either be BUY/SELL \n" << std::endl;
            continue;}
           }
        
           else if (Token1 == "CANCEL")
        {
            int Token2{};
            command.type = CommandType::Cancel;
            size_t start2 = com_pos1 + 1;
            std::string Token2Str = CMD.substr(start2);
            auto T2temp = std::from_chars(Token2Str.data(), Token2Str.data() + Token2Str.size(), Token2); 
            if (T2temp.ec != std::errc{}){// check to see if ec that was made is the same as defult as would indicat no errors
                std::println("You have not formatted your input correctly \n Correct Example: CANCEL 2  ");
                continue;

            }
            command.OrderID = Token2;
            
            
             
        }
        
        else if (Token1 == "PRINTBOOK")
        {
            command.type = CommandType::PrintBook;
        }

        else if (Token1 == "QUIT")
        {
            command.type = CommandType::Quit;
            
        }

        else {
            command.type = CommandType::INVALID;
            std::println("You did not pick a valid option please refear to docs");
            continue;
        }

        //--------------------------------------------------------------------
        

     // Now we disptaach to the engine but we need to actually create the engine methods first we will use a switch case and then send off 
     
    switch (command.type)
    {
        case (CommandType::New):  
        engine.NewOrder(command);
        break;



        case (CommandType::Cancel): 
        engine.cancel_order(command.OrderID);
         break;


        case (CommandType::Quit):
        std::println("Progam is closing");
        std::exit(1);
        break;

        case (CommandType::PrintBook): 
        engine.print_book();
        break;
        
        case (CommandType::INVALID): std::cout << "You have not formatted an input correctly" << std::endl;
        
        break;

        default:
        std::println("You have not formatted an input correctly");
        break;

    } 
    
    



    }

    return 0;
}