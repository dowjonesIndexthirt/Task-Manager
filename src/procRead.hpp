/*
    Proc read header file


    Goal:
        Make /proc reading for all PID's quick and efficiently exported

    Things to be read:
        status          <-- Start here DONE
        stat            // DONE
        exe             // DONE
        cmdline         // DONE
        io              // Optional
    
    System info:
        stat
        meminfo

    What this file will do:
        Read all processes in /proc and report a std::vector of all the processes
*/

#pragma once

#define PROC_DEBUG   // Comment this out for the full build, this displays diagnostic info such as failed symlink etc

#include <sys/types.h>
#include <iostream>
#include <optional>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

/* Struct used for basic process info */
struct processInfo {
    pid_t pid = 0;                      // Process ID
    pid_t parentPiD = 0;                // Parent process ID

    uid_t uid = 0;                      // User ID that owns the process

    std::string name;                   // Process name
    std::string command;                // Command that started the process
    std::string exec;                   // Executable location

    char state = 0;                     // State, e.g. 'S' sleeping etc

    std::uint64_t residentMemKB = 0;    // Resident memory usage in KB
    std::uint64_t virtMemKB = 0;        // Virtual memory usage in KB

    std::uint64_t userCpuTicks = 0;     // CPU ticks used in userspace
    std::uint64_t systemCpuTicks = 0;   // CPU ticks used in kernel space

    std::size_t threadCount = 0;        // Number of threads

    // Inbuilt functions
    /* Output stream helper to make printing info easier */
    friend std::ostream& operator<<(std::ostream& os, const processInfo& p){
        constexpr char nl = '\n';
        os  << p.pid << nl
            << p.parentPiD << nl
            << p.uid << nl
            << p.name << nl
            << p.command << nl
            << p.exec << nl
            << p.state << nl
            << p.residentMemKB << nl
            << p.virtMemKB << nl
            << p.userCpuTicks << nl
            << p.systemCpuTicks << nl
            << p.threadCount << nl;

        return os;
    };
};

/* External functions for upwards on the line */
void readAndPrintPID1();

/*
    This reads a specific PID from an id
    returns std::nulopt from std::optional if /proc/PID/status cannot be read
    Less important fields will be labeled with "Unknown" or be defaulted to 0
*/
std::optional<processInfo> readPID(const std::string& pid);

std::vector<processInfo> readAllPIDs();
