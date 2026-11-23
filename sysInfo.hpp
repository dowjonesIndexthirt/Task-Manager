/*

Reads /proc/stat, /proc/meminfo and related files to
build a struct that contains basic sytem info

*/

#pragma once

#define PROC_DEBUG

#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <cstdint>
#include <string>

/* Basic system info struct that contains all relevant data */
struct systemInfo{
    std::uint64_t totalMemKB = 0;       // Total memory in KB
    std::uint64_t availableMemKB = 0;   // Available memory in KB
    std::uint64_t totalSwapKB = 0;      // Total swap memory in KB
    std::uint64_t freeSwapKB = 0;       // Free swap in KB

    std::uint64_t cpuUserTicks = 0;     // Ticks spent running user-space code.
    std::uint64_t cpuNiceTicks = 0;     // Ticks spent running user-space programs where priorities have been changed by the "nice" command
    std::uint64_t cpuSystemTicks = 0;   // Ticks spent running kernel code for the system
    std::uint64_t cpuIdleTicks = 0;     // Ticks spent doing nothing
    std::uint64_t cpuIoWaitTicks = 0;   // Ticks spent waiting for I/O work
    std::uint64_t cpuIrqTicks = 0;      // Ticks spent managing hardware interrrupts
    std::uint64_t cpuSoftIrqTicks = 0;  // Ticks spent handling software interrupts (Happens during hardware interrupt)
    std::uint64_t cpuStealTicks = 0;    // VM ONLY -- Ticks thate were meant 

    double uptimeSeconds = 0.0;         // Time computer has been booted in seconds

    double load1 = 0.0;                 // Load average over 1-minute  
    double load5 = 0.0;                 // Load average over 5-minutes 
    double load15 = 0.0;                // Load average over 15-minutes

    std::size_t totalCpuCount = 0;      // The pcount of logical CPUs the system can use
};

/* Struct that holds the percentages of all */
struct percentageSystemInfo{
    double cpuUsage = 0.0;
    double memoryUsage = 0.0;
    double swapUsage = 0.0;
};

bool collectSysInfo(systemInfo& sysInfo);
percentageSystemInfo calcPercentages(const systemInfo &a, const systemInfo& b);
