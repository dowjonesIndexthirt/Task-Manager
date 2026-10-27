/*
    Stores raw process information for the frontend.
    procRead reads the files; taskManager puts the results into tasks.
*/

#pragma once

#include "procRead.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

/* One process and the information read from its /proc files. */
struct task {
    pid_t pid = 0;
    pid_t parentPiD = 0;
    uid_t uid = 0;

    std::string name;
    std::string command;
    std::string exec;

    char state = 0;

    std::uint64_t residentMemKB = 0;    // Resident RAM, from VmRSS.
    std::uint64_t virtMemKB = 0;        // Virtual memory, from VmSize.

    std::uint64_t userCpuTicks = 0;
    std::uint64_t systemCpuTicks = 0;

    std::size_t threadCount = 0;
};

class taskManager {
private:
    std::vector<task> tasks;

public:
    /* Replace the current list with the latest processes from procRead. */
    void refresh();

    /* Let the frontend read the list without copying or changing it. */
    const std::vector<task>& getTasks() const;

    /* true = smallest first; false = largest first. */
    void sortByPID(bool ascending = true);
    void sortByUID(bool ascending = true);
    void sortByName(bool ascending = true);
    void sortByUserCpuTicks(bool ascending = true);
    void sortBySystemCpuTicks(bool ascending = true);
    void sortByResidentMemKB(bool ascending = true);
    void sortByVirtMemKB(bool ascending = true);
};

