#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "drewTaskMgr.hpp"

class QTableWidget;
class QComboBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void showSelectedTaskInfo();
    void refreshTaskInfo();
    void sortTasks();

private:
    void populateTaskTable();

    taskManager manager;
    QTableWidget *taskTable = nullptr;
    QComboBox *sortField = nullptr;
    QComboBox *sortOrder = nullptr;
};

#endif // MAINWINDOW_H
