#pragma once

#include <thread>
#include <iostream>
#include <cstdint>
#include <chrono>


#include "pidRead.hpp"
#include "sysInfo.hpp"

/*

Sleep for like 0.01 seconds:
```cpp
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "Starting...\n";

    // Wait for 0.01 seconds (10 milliseconds)
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    std::cout << "Done waiting!\n";
    return 0;
}
```

TODO: threshhold function, do the reads for percentages
*/
inline double calcRamPercent(const processInfo& pid, const systemInfo& sys);

percentageSystemInfo getPercentages(systemInfo& sysInfo, const std::size_t sleepFor);
std::vector<processInfo> threshFunc(const std::vector<processInfo>& pids, const std::uint64_t minRamKB = 1024);
std::vector<processInfo> threshFunc(const std::vector<processInfo>& pids, const systemInfo& sys, const double minPercent = 10.0);