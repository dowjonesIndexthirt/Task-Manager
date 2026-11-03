#include "taskmanager.h"

namespace {
TaskInfo task{
    QStringLiteral("Example task"),
    QStringLiteral("In progress"),
    QStringLiteral("This information is coming from the temporary TaskManager stub.")
};
}

TaskInfo TaskManager::currentTask()
{
    return task;
}

void TaskManager::refresh()
{
    // Temporary implementation. Replace this with your real refresh() function.
    task.status = QStringLiteral("Refreshed");
    task.details = QStringLiteral("The task information was refreshed.");
}
