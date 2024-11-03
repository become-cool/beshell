#pragma once

#include <cstdint>
#include <functional>


void run_wait_on_core(std::function<void()> func, uint8_t core_id) ;