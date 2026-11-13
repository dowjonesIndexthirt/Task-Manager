#include "mainwindow.h"

#include <QAbstractItemView>
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Task Manager"));

    auto *container = new QWidget(this);
    auto *layout = new QVBoxLayout(container);

    taskTable = new QTableWidget(container);
    taskTable->setColumnCount(6);
    taskTable->setHorizontalHeaderLabels({
        tr("PID"), tr("Name"), tr("State"), tr("RAM (KB)"),
        tr("Virtual (KB)"), tr("Threads")
    });
    taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    taskTable->setSelectionMode(QAbstractItemView::SingleSelection);
    taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    taskTable->horizontalHeader()->setStretchLastSection(true);
    taskTable->setSortingEnabled(false);

    auto *buttonLayout = new QHBoxLayout;
    auto *showButton = new QPushButton(tr("Show selected task"), container);
    auto *refreshButton = new QPushButton(tr("Refresh"), container);
    sortField = new QComboBox(container);
    sortField->addItem(tr("PID"));
    sortField->addItem(tr("User ID"));
    sortField->addItem(tr("Name"));
    sortField->addItem(tr("User CPU ticks"));
    sortField->addItem(tr("System CPU ticks"));
    sortField->addItem(tr("RAM (KB)"));
    sortField->addItem(tr("Virtual memory (KB)"));

    sortOrder = new QComboBox(container);
    sortOrder->addItem(tr("Ascending"));
    sortOrder->addItem(tr("Descending"));
    auto *sortButton = new QPushButton(tr("Sort"), container);

    buttonLayout->addWidget(showButton);
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(sortField);
    buttonLayout->addWidget(sortOrder);
    buttonLayout->addWidget(sortButton);

    layout->addWidget(taskTable);
    layout->addLayout(buttonLayout);
    setCentralWidget(container);

    connect(showButton, &QPushButton::clicked,
            this, &MainWindow::showSelectedTaskInfo);
    connect(refreshButton, &QPushButton::clicked,
            this, &MainWindow::refreshTaskInfo);
    connect(sortButton, &QPushButton::clicked,
            this, &MainWindow::sortTasks);
    connect(taskTable, &QTableWidget::cellDoubleClicked,
            this, [this](int, int) { showSelectedTaskInfo(); });

    resize(850, 520);
    refreshTaskInfo();
}

void MainWindow::refreshTaskInfo()
{
    manager.refresh();
    populateTaskTable();
}

void MainWindow::refreshTaskInfoPeriodically()
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::refreshTaskInfo);
    timer->start(5000); 
}

void MainWindow::sortTasks()
{
    const bool ascending = sortOrder->currentIndex() == 0;

    switch (sortField->currentIndex()) {
    case 0:
        manager.sortByPID(ascending);
        break;
    case 1:
        manager.sortByUID(ascending);
        break;
    case 2:
        manager.sortByName(ascending);
        break;
    case 3:
        manager.sortByUserCpuTicks(ascending);
        break;
    case 4:
        manager.sortBySystemCpuTicks(ascending);
        break;
    case 5:
        manager.sortByResidentMemKB(ascending);
        break;
    case 6:
        manager.sortByVirtMemKB(ascending);
        break;
    default:
        return;
    }

    populateTaskTable();
}

void MainWindow::populateTaskTable()
{
    const auto &tasks = manager.getTasks();
    taskTable->setRowCount(0);
    taskTable->setRowCount(static_cast<int>(tasks.size()));

    for (int row = 0; row < taskTable->rowCount(); ++row) {
        const task &process = tasks[static_cast<std::size_t>(row)];
        taskTable->setItem(row, 0, new QTableWidgetItem(QString::number(process.pid)));
        taskTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(process.name)));
        taskTable->setItem(row, 2, new QTableWidgetItem(QString(QChar::fromLatin1(process.state))));
        taskTable->setItem(row, 3, new QTableWidgetItem(QString::number(process.residentMemKB)));
        taskTable->setItem(row, 4, new QTableWidgetItem(QString::number(process.virtMemKB)));
        taskTable->setItem(row, 5, new QTableWidgetItem(QString::number(process.threadCount)));
    }

    taskTable->resizeColumnsToContents();
}

void MainWindow::showSelectedTaskInfo()
{
    const int row = taskTable->currentRow();
    const auto &tasks = manager.getTasks();

    if (row < 0 || row >= static_cast<int>(tasks.size())) {
        QMessageBox::information(this, tr("Task information"),
                                 tr("Select a task first."));
        return;
    }

    const task &process = tasks[static_cast<std::size_t>(row)];
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Task information"));
    dialog.setModal(true);

    auto *layout = new QVBoxLayout(&dialog);
    auto *form = new QFormLayout;
    form->addRow(tr("PID:"), new QLabel(QString::number(process.pid)));
    form->addRow(tr("Parent PID:"), new QLabel(QString::number(process.parentPiD)));
    form->addRow(tr("User ID:"), new QLabel(QString::number(process.uid)));
    form->addRow(tr("Name:"), new QLabel(QString::fromStdString(process.name)));
    form->addRow(tr("Command:"), new QLabel(QString::fromStdString(process.command)));
    form->addRow(tr("Executable:"), new QLabel(QString::fromStdString(process.exec)));
    form->addRow(tr("State:"), new QLabel(QString(QChar::fromLatin1(process.state))));
    form->addRow(tr("User CPU ticks:"), new QLabel(QString::number(process.userCpuTicks)));
    form->addRow(tr("System CPU ticks:"), new QLabel(QString::number(process.systemCpuTicks)));
    form->addRow(tr("Resident memory (KB):"), new QLabel(QString::number(process.residentMemKB)));
    form->addRow(tr("Virtual memory (KB):"), new QLabel(QString::number(process.virtMemKB)));
    form->addRow(tr("Threads:"), new QLabel(QString::number(process.threadCount)));
    layout->addLayout(form);

    auto *closeButton = new QPushButton(tr("Close"));
    closeButton->setDefault(true);
    layout->addWidget(closeButton);
    connect(closeButton, &QPushButton::clicked,
            &dialog, &QDialog::accept);

    dialog.adjustSize();
    dialog.exec();
}
