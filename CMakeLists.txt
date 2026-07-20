
cmake_minimum_required(VERSION 3.20)
project(OrderMatcher LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)



add_library(matching_engine_lib src/Order.cpp src/MatchingEngine.cpp)
target_include_directories(matching_engine_lib PUBLIC Include/matching_engine)
target_link_libraries(matching_engine_lib PUBLIC stdc++exp)

add_executable(engine src/main.cpp)
target_link_libraries(engine PRIVATE matching_engine_lib)

 add_executable(bench src/benchmarking.cpp)
target_link_libraries(bench PRIVATE matching_engine_lib)

add_executable(tests tests/test_matching_engine.cpp)
target_link_libraries(tests PRIVATE matching_engine_lib Catch2::Catch2WithMain)


add_executable(server serverTCP/TCPServer.cpp)
target_link_libraries(server PRIVATE matching_engine_lib)