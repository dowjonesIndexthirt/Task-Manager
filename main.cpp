#include <iostream>
#include <vector>

#include "etc.hpp"

#define PROC_DEBUG
#include "sysInfo.hpp"
#include "pidRead.hpp"

int main(void){
    systemInfo sysInfo;
    collectSysInfo(sysInfo);

    percentageSystemInfo perc = getPercentages(sysInfo, 500);
    std::cout << "CPU Usage: " << perc.cpuUsage << "%\n"
                "Ram Usage: " << perc.memoryUsage << "%\n"
                "Swap Usage: " << perc.swapUsage << "%\n";
    
    std::vector<processInfo> pids = readAllPIDs();

    // Weed out all processes using less than 1% of max ram
    std::vector<processInfo> culled = threshFunc(pids, sysInfo, 1.0);

    for (const processInfo& proc : culled)
        std::cout << proc << "\n";

    return 0;
}
