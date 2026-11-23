#include "sysInfo.hpp"

#include <iostream>
#include <vector>

#include <thread>
#include <chrono>

int main(void){
    systemInfo a;
    std::cout << collectSysInfo(a) << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    systemInfo b;
    std::cout << collectSysInfo(b) << std::endl;

    percentageSystemInfo total= calcPercentages(a, b);

    std::cout << total.cpuUsage << '\n' << total.memoryUsage << std::endl;

    return 0;
}