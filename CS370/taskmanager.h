#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QString>

struct TaskInfo
{
    QString name;
    QString status;
    QString details;
};

class TaskManager
{
public:
    // Replace these implementations with the functions from your real taskManager.cpp.
    static TaskInfo currentTask();
    static void refresh();
};

#endif // TASKMANAGER_H
