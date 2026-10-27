#include "mainwindow.h"

#include "taskmanager.h"

#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Task Manager"));

    auto *container = new QWidget(this);
    auto *layout = new QVBoxLayout(container);
    auto *showButton = new QPushButton(tr("Show task information"), container);
    auto *refreshButton = new QPushButton(tr("Refresh"), container);
    showButton->setMinimumSize(220, 44);
    refreshButton->setMinimumSize(220, 44);
    layout->addWidget(showButton);
    layout->addWidget(refreshButton);
    setCentralWidget(container);

    connect(showButton, &QPushButton::clicked,
            this, &MainWindow::showTaskInfo);
    connect(refreshButton, &QPushButton::clicked,
            this, &MainWindow::refreshTaskInfo);

    resize(360, 220);
}

void MainWindow::refreshTaskInfo()
{
    TaskManager::refresh();
    showTaskInfo();
}

void MainWindow::showTaskInfo()
{
    const TaskInfo task = TaskManager::currentTask();

    QDialog dialog(this);
    dialog.setWindowTitle(tr("Task information"));
    dialog.setModal(true);

    auto *layout = new QVBoxLayout(&dialog);
    auto *form = new QFormLayout;
    form->addRow(tr("Name:"), new QLabel(task.name));
    form->addRow(tr("Status:"), new QLabel(task.status));
    form->addRow(tr("Details:"), new QLabel(task.details));
    layout->addLayout(form);

    auto *closeButton = new QPushButton(tr("Close"));
    closeButton->setDefault(true);
    layout->addWidget(closeButton);
    connect(closeButton, &QPushButton::clicked,
            &dialog, &QDialog::accept);

    dialog.adjustSize();
    dialog.exec();
}
