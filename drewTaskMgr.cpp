#include "drewTaskMgr.hpp"

#include <algorithm>

void taskManager::refresh(){
    /* procRead opens and reads the /proc files for the processes. */
    std::vector<processInfo> procs = readAllPIDs();

    /* Remove the old snapshot so repeated refreshes do not add duplicates. */
    tasks.clear();

    for (std::size_t i = 0; i < procs.size(); i++){
        task current;

        current.pid = procs[i].pid;
        current.parentPiD = procs[i].parentPiD;
        current.uid = procs[i].uid;

        current.name = procs[i].name;
        current.command = procs[i].command;
        current.exec = procs[i].exec;

        current.state = procs[i].state;

        current.residentMemKB = procs[i].residentMemKB;
        current.virtMemKB = procs[i].virtMemKB;

        current.userCpuTicks = procs[i].userCpuTicks;
        current.systemCpuTicks = procs[i].systemCpuTicks;

        current.threadCount = procs[i].threadCount;

        tasks.push_back(current);
    }
}

const std::vector<task>& taskManager::getTasks() const {
    return tasks;
}

void taskManager::sortByPID(bool ascending){
    std::sort(tasks.begin(), tasks.end(),
        [ascending](const task& a, const task& b){
            if (ascending)
                return a.pid < b.pid;

            return a.pid > b.pid;
        });
}

void taskManager::sortByUID(bool ascending){
    std::sort(tasks.begin(), tasks.end(),
        [ascending](const task& a, const task& b){
            if (ascending)
                return a.uid < b.uid;

            return a.uid > b.uid;
        });
}

void taskManager::sortByName(bool ascending){
    std::sort(tasks.begin(), tasks.end(),
        [ascending](const task& a, const task& b){
            if (ascending)
                return a.name < b.name;

            return a.name > b.name;
        });
}

void taskManager::sortByUserCpuTicks(bool ascending){
    std::sort(tasks.begin(), tasks.end(),
        [ascending](const task& a, const task& b){
            if (ascending)
                return a.userCpuTicks < b.userCpuTicks;

            return a.userCpuTicks > b.userCpuTicks;
        });
}

void taskManager::sortBySystemCpuTicks(bool ascending){
    std::sort(tasks.begin(), tasks.end(),
        [ascending](const task& a, const task& b){
            if (ascending)
                return a.systemCpuTicks < b.systemCpuTicks;

            return a.systemCpuTicks > b.systemCpuTicks;
        });
}

void taskManager::sortByResidentMemKB(bool ascending){
    std::sort(tasks.begin(), tasks.end(),
        [ascending](const task& a, const task& b){
            if (ascending)
                return a.residentMemKB < b.residentMemKB;

            return a.residentMemKB > b.residentMemKB;
        });
}

void taskManager::sortByVirtMemKB(bool ascending){
    std::sort(tasks.begin(), tasks.end(),
        [ascending](const task& a, const task& b){
            if (ascending)
                return a.virtMemKB < b.virtMemKB;

            return a.virtMemKB > b.virtMemKB;
        });
}
