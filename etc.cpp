#include "etc.hpp"

/* Calculates the ram usage percen of a pid */
inline double calcRamPercent(const processInfo &pid, const systemInfo &sys){
    return 100 * (static_cast<double>(pid.residentMemKB) / 
                static_cast<double>(sys.totalMemKB));
}

/* Gets percentages over a duration, sleepFor is in milliseconds */
percentageSystemInfo getPercentages(systemInfo &sysInfo, const std::size_t sleepFor){
    systemInfo prev = sysInfo;

    // Sleep for duration
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepFor));

    collectSysInfo(sysInfo);
    

    return calcPercentages(prev, sysInfo);
}

/* Creates a new process vector of all the processes above the uint64 */
std::vector<processInfo> threshFunc(const std::vector<processInfo> &pids, const std::uint64_t minRamKB){
    std::vector<processInfo> cleaned;

    for (const processInfo& proc : pids)
        if (proc.residentMemKB > minRamKB)
            cleaned.push_back(proc);
    
    return cleaned;
}

/* Same as the other, just does a percentage threshhold of the system's max ram */
std::vector<processInfo> threshFunc(const std::vector<processInfo> &pids, const systemInfo &sys, const double minPercent){
    std::vector<processInfo> cleaned;

    for (const processInfo& proc : pids)
        if (calcRamPercent(proc, sys) > minPercent)
            cleaned.push_back(proc);
    
    return cleaned;
}
