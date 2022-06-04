#include "mainwindow.h"
#include <manager.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->listWidget->itemDelegate(), &QAbstractItemDelegate::commitData, this, &MainWindow::list_commitData);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_addButton_clicked()
{
    manager.addNewItem();
}

void MainWindow::list_commitData()
{
    int nowRow = ui->listWidget->currentRow();
    QString listText = ui->listWidget->item(nowRow)->text();

    if (listText.isEmpty())
    {
        manager.removeItem(ui->listWidget->item(nowRow), nowRow);
        ui->listWidget->takeItem(nowRow);
    }
    else
    {
        manager.input(listText.toStdString(), ui->listWidget->item(nowRow), nowRow);
    }
}
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int nowRow = ui->listWidget->currentRow();
    manager.editItem(item,nowRow);
}


void MainWindow::on_pushButton_clicked()
{
    manager.calculate(5, "c");
}

