#include "taskManager.hpp"
#include <iostream>

int main(){
    taskManager manager;

    manager.refresh();
    manager.sortByPID();

    const std::vector<task>& tasks = manager.getTasks();

    std::cout << "PID UID NAME RAM(KB) VIRTUAL(KB)\n";

    for (std::size_t i = 0; i < tasks.size(); i++){
        std::cout << tasks[i].pid << ' '
                  << tasks[i].uid << ' '
                  << tasks[i].name << ' '
                  << tasks[i].residentMemKB << ' '
                  << tasks[i].virtMemKB << '\n';
    }

    return 0;
}
