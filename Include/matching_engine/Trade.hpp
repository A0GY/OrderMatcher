#pragma once
#include <chrono>
struct Trade{ //M1
int OrderB_ID {};
int OrderS_ID {};
int price {};
int Quantity {};
std::time_t TS {};
};