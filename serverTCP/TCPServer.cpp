#include <sys/socket.h>
#include <netdb.h> //getaddrinfo, addrinfo, AF_*, AI_*
#include <unistd.h>   // close()
#include <cstring> //strlen
#include <string>
#include <print>
#include <stdio.h>
#include <iostream>
#include <array>
#include <charconv>
#include "MatchingEngine.hpp"
// TCP enigne to be atached to Matching engine 
int main(){
auto live_ts = std::chrono::system_clock::now();
//std::array<char,500> _buffer ;

std::string CommandLine;
MatchingEngine engine;
Command command;
std::cout << "time is : " << live_ts << "\n";

class Socket{
private:
int fd_ = -1; // defult set to error

public:
explicit Socket (int fd) : fd_(fd) {};
~Socket() {if (fd_ != -1){close(fd_);}}
Socket(const Socket&) = delete;
Socket& operator=(const Socket&) = delete;

int get() const noexcept{
return fd_;}
};

addrinfo hints{}; // no need to use c despicable memset -- THIS IS THE TYPE TO BUILT THE ADDRESSINFO TO GETADDRINFO "FUNCTION"
hints.ai_family = AF_UNSPEC; // AF = Address family, UNSPEC is just use both 
hints.ai_socktype = SOCK_STREAM; //CHOOSE STREAM TCP OR DGRAM UDP
hints.ai_flags = AI_PASSIVE; // inform that this is a server
addrinfo* res = nullptr; // store the address created obj of our hints object once GETADDRINFO takes in as it need to put restulys into somthing 

int rc =  getaddrinfo(nullptr, "8080", &hints, &res);

if (rc != 0){ std::cout << gai_strerror(rc);
    return 1;}

// res now contains the structre needed to create a sock, rc was there to identify if correct conditions/values to form a adress if not then report a error back
// we use gai_strerror to see error given by geraddrinfo

Socket sockfd(socket(res->ai_family,res->ai_socktype,res->ai_protocol));

if (sockfd.get() == -1){perror("socket");
return 1;}

int yes {1}; // not sure why 1 is needed maybe a tick/flag for the conditions to be enabeled?

setsockopt(sockfd.get(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

if (bind(sockfd.get(),res->ai_addr,res->ai_addrlen) == -1){
    perror("bind");
    return 1;
}
freeaddrinfo(res);
if (listen(sockfd.get(), 10) == -1) {perror("listen");
return 1;}

while (true){

Socket connfd ( accept(sockfd.get() ,nullptr,nullptr));
if (connfd.get() == -1){perror("accept");
return 1;}
const char* msg = "Hello, World! \n";
send (connfd.get(), msg, strlen(msg), 0);

bool keep_client = true;
// loop over to take input from client,, not all bytes may have been moved from kernel to buffer to we need to loop over 
while (keep_client == true){
std::array<char,500> _buffer ; // moving to within client lifecycle to clear buffer once a client has     
std::string stringBuffer;
ssize_t _recv = recv(connfd.get(),_buffer.data(),_buffer.size(),0);

if (_recv == -1){ perror("recv");
return 1;}

else if(_recv == 0){ std::cout << "client disconnected" ;
break;}

for (ssize_t i {}; i<_recv; i++){

    std::cout << _buffer[i];
}

stringBuffer.append(_buffer.data(),_recv); 
std::cout << "string buffer: " << stringBuffer;

auto subFind = stringBuffer.find('\n');
while ((subFind = stringBuffer.find('\n')) != std::string::npos) {
    std::string CommandLine = stringBuffer.substr(0, subFind);
    stringBuffer.erase(0, subFind + 1);
// parsing here, as we are out of inner while so test has been formed correctly
std::cout << "testing CommandLine out of inner while " << CommandLine << '\n' << '\n' ;

size_t start {0};
        auto com_pos1 = CommandLine.find(" ",0);
        std::string Token1 = CommandLine.substr(start, com_pos1);
        
        if (Token1 == "NEW"){

            command.type = CommandType::New;
            
            size_t start2 = com_pos1 + 1;
            auto com_pos2 = CommandLine.find(" ", start2);
            std::string Token2 = CommandLine.substr(start2, com_pos2 - start2);
            size_t start3 = com_pos2 + 1;
            auto com_pos3 = CommandLine.find(" ", start3);
            
            const std::string Token3Str = CommandLine.substr(start3, com_pos3 - start3);
            int Token3 {};
            auto T3temp = std::from_chars(Token3Str.data(), Token3Str.data() + Token3Str.size(), Token3);// Using from_chat is a better perfomance choice then stoi which was prev used, no memory alloc and no crash of error/throw
            if (T3temp.ec != std::errc{} || Token3 <= 0 ){// check to see if ec that was made is the same as defult as would indicat no errors
                std::println("You have not formatted your input correctly \n Correct Example: NEW BUY 10 100  \n Warning: No Negative numbers allowd ");
                continue;

            }
            
            size_t start4 = com_pos3 + 1;
            int  Token4 {};
            std::string Token4Str = CommandLine.substr(start4);
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
            std::string Token2Str = CommandLine.substr(start2);
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
        case (CommandType::New): {
        int i = engine.NewOrder(command);
        std::string msg = std::to_string(i);
        msg.append("\n");
       send(connfd.get(), msg.data(), msg.size(), 0); } // need to point at start of string and length for send()
        break;

        case (CommandType::Cancel): {
        std::string cancel_retun = engine.cancel_order(command.OrderID);
        send(connfd.get(), cancel_retun.data(), cancel_retun.size(), 0);
        }break;

        case (CommandType::Quit):{
        const char * msg ="Disconected \n" ;
        send(connfd.get(),msg, strlen(msg), 0);
      
        keep_client = false;
        break;}

        case (CommandType::PrintBook): {
        std::vector<std::string> book_retun = engine.print_book();
        
        for ( auto i : book_retun){
            send(connfd.get(), i.data(), i.size(), 0);

        }
    
    }

        break;
        
        case (CommandType::INVALID): std::cout << "You have not formatted an input correctly" << std::endl;
        
        break;

        default:
        std::println("You have not formatted an input correctly");
        break;

    } 
    
}

}

}}