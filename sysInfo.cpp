#include "sysInfo.hpp"
namespace {
    /* Function that fulfills totalCpuCount from the struct. Using a unistd.h system call */
    void getCPUCount(systemInfo &sysInfo){
        long count = sysconf(_SC_NPROCESSORS_ONLN);

        if (count > 0)
            sysInfo.totalCpuCount = static_cast<std::size_t>(count);
        #ifdef PROC_DEBUG
        else
            std::cerr << "System call to retrieve cpu count returned an invalid number.\n";
        #endif
    }

    /* Gets the meminfo of the system */
    bool getMemInfo(systemInfo &sysInfo){
        std::uint64_t val = 0;

        bool hasTotal = false;
        bool hasAvail = false;
        bool hasTotalSwap = false;
        bool hasAvailSwap = false;

        std::ifstream file("/proc/meminfo");
        if (!file.is_open()){
            #ifdef PROC_DEBUG
            std::cerr << "WARNING: Could not open \""
                        "/proc/meminfo\"\n";
            #endif
            return false;
        }

        // Multi-use string to read stuff
        std::string tmp;
        while((file >> tmp) && (!hasTotal || !hasAvail || !hasTotalSwap || !hasAvailSwap)){
            // Checks each field we want
            if (tmp == "MemTotal:"){
                if (!(file >> val))
                    return false;
                
                sysInfo.totalMemKB = val;
                hasTotal = true;
                continue;
            } if (tmp == "MemAvailable:"){
                if (!(file >> val))
                    return false;

                sysInfo.availableMemKB = val;
                hasAvail = true;
                continue;
            } if (tmp == "SwapTotal:"){
                if (!(file >> val))
                    return false;

                sysInfo.totalSwapKB = val;
                hasTotalSwap = true;
                continue;
            } if (tmp == "SwapFree:"){
                if (!(file >> val))
                    return false;

                sysInfo.freeSwapKB = val;
                hasAvailSwap = true;
                continue;
            }
        };

        
        #ifdef PROC_DEBUG
        if (!hasTotal)
            std::cerr << "Could not get total memory.\n";
        if (!hasAvail)
            std::cerr << "Could not get available memory.\n";
        if (!hasTotalSwap)
            std::cerr << "Could not get total swap.\n";
        if (!hasAvailSwap)
            std::cerr << "Could not get available swap.\n";
        #endif

        file.close();

        return (hasTotal && hasAvail && hasTotalSwap && hasAvailSwap);
    }

    /* Reads the uptime from /proc/uptime */
    bool getUptime(systemInfo &sysInfo){
        std::ifstream file("/proc/uptime");
        if (!file.is_open()){
            #ifdef PROC_DEBUG
            std::cerr << "Could not open /proc/uptime\n";
            #endif
            return false;
        }

        if (!(file >> sysInfo.uptimeSeconds)){
            #ifdef PROC_DEBUG
            std::cerr << "Could not get total uptime seconds from /proc/uptime\n";
            #endif
            return false;
        }

        file.close();
        return true;
    }

    /* Gets the loadavg of a the system between 1, 5, and 15 minutes */
    bool getLoadAvg(systemInfo &sysInfo){
        std::ifstream file("/proc/loadavg");

        if (!file.is_open()){
            #ifdef PROC_DEBUG
            std::cerr << "Could not open /proc/loadavg\n";
            #endif
            return false;
        }

        // 3 reads that check /proc/loadavg for the 1 5 and 15 minute loads
        if (!(file >> sysInfo.load1))
            return false;
        if (!(file >> sysInfo.load5))
            return false;
        if (!(file >> sysInfo.load15))
            return false;

        file.close();
        return true;
    }

    /* Reads the scary stat file!! Ooooh! */
    bool readStat(systemInfo &sysInfo){
        std::ifstream file("/proc/stat");

        if (!file.is_open()){
            #ifdef PROC_DEBUG
            std::cerr << "Could not open /proc/stat\n";
            #endif
            return false;
        }

        std::string str;

        if (!(file >> str))
            return false;
        
        // Make sure it says "cpu"
        if (str != "cpu"){
            #ifdef PROC_DEBUG
            std::cerr << "First line does not contain cpu\n";
            #endif
            return false;
        }

        // Parse the next 8 numbers into associated sysInfo things
        if (!(file >> sysInfo.cpuUserTicks))
            return false;

        if (!(file >> sysInfo.cpuNiceTicks))
            return false;

        if (!(file >> sysInfo.cpuSystemTicks))
            return false;
            
        if (!(file >> sysInfo.cpuIdleTicks))
            return false;

        if (!(file >> sysInfo.cpuIoWaitTicks))
            return false;

        if (!(file >> sysInfo.cpuIrqTicks))
            return false;

        if (!(file >> sysInfo.cpuSoftIrqTicks))
            return false;
            
        if (!(file >> sysInfo.cpuStealTicks))
            return false;


        file.close();
        return true;
    }
}

/* Collects all the system info */
bool collectSysInfo(systemInfo &sysInfo){
    getCPUCount(sysInfo);
    bool mem = getMemInfo(sysInfo);
    bool uptime = getUptime(sysInfo);
    bool load = getLoadAvg(sysInfo);
    bool stat = readStat(sysInfo);

    #ifdef PROC_DEBUG
    if (!mem)
        std::cerr << "Failed to fill memory fields.\n";
    if (!uptime)
        std::cerr << "Failed to fill uptime field.\n";
    if (!load)
        std::cerr << "Failed to fill load fields.\n";
    if (!stat)
        std::cerr << "Failed to fill stat fields.\n";
    #endif
    
    return (mem && uptime && load && stat);
}

/* This returns percentages of the system */
percentageSystemInfo calcPercentages(const systemInfo &prev, const systemInfo& curr){
    std::uint64_t prevTotal = prev.cpuUserTicks + prev.cpuNiceTicks + prev.cpuSystemTicks + 
                            prev.cpuIdleTicks + prev.cpuIoWaitTicks + prev.cpuIrqTicks +
                            prev.cpuSoftIrqTicks + prev.cpuStealTicks;
    
    std::uint64_t currTotal =  curr.cpuUserTicks + curr.cpuNiceTicks + curr.cpuSystemTicks + 
                            curr.cpuIdleTicks + curr.cpuIoWaitTicks + curr.cpuIrqTicks +
                            curr.cpuSoftIrqTicks + curr.cpuStealTicks;

    std::int64_t totalDelta = static_cast<std::int64_t>(currTotal) - 
                                static_cast<std::int64_t>(prevTotal);
    std::int64_t idleTotal = (static_cast<std::int64_t>(curr.cpuIdleTicks) - static_cast<std::int64_t>(prev.cpuIdleTicks)) + 
                                (static_cast<std::int64_t>(curr.cpuIoWaitTicks) - static_cast<std::int64_t>(prev.cpuIoWaitTicks));

    double totalCpu;
    if ((totalDelta <= 0) || (idleTotal < 0) || (idleTotal > totalDelta))
        totalCpu = -1.0;
    else
        totalCpu = 100.0 * (static_cast<double>(totalDelta - idleTotal) /
                                static_cast<double>(totalDelta));


    double memPercent;
    if (curr.totalMemKB == 0)
        memPercent = -1.0;
    else
        memPercent = 100.0 * (1.0 - (static_cast<double>(curr.availableMemKB) /
                                        (static_cast<double>(curr.totalMemKB))));

    double swapPercent;
    if (curr.totalSwapKB == 0)
        swapPercent = 0.0;
    else
        swapPercent = 100.0 * (1.0 - (static_cast<double>(curr.freeSwapKB) /
                                        (static_cast<double>(curr.totalSwapKB))));

    return percentageSystemInfo{totalCpu, memPercent, swapPercent};
}
