#include "mainwindow.h"

#include <QAbstractItemView>
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
    buttonLayout->addWidget(showButton);
    buttonLayout->addWidget(refreshButton);

    layout->addWidget(taskTable);
    layout->addLayout(buttonLayout);
    setCentralWidget(container);

    connect(showButton, &QPushButton::clicked,
            this, &MainWindow::showSelectedTaskInfo);
    connect(refreshButton, &QPushButton::clicked,
            this, &MainWindow::refreshTaskInfo);
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
