#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "drewTaskMgr.hpp"

class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void showSelectedTaskInfo();
    void refreshTaskInfo();

private:
    void populateTaskTable();

    taskManager manager;
    QTableWidget *taskTable = nullptr;
};

#endif // MAINWINDOW_H