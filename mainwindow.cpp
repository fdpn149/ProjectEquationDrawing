#include "mainwindow.h"
#include <manager.h>

MainWindow::MainWindow(QWidget* parent)
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

QListWidgetItem* MainWindow::getListItem(int index)
{
	return ui->listWidget->item(index);
}


void MainWindow::on_addButton_clicked()
{
	manager.addNewItem();
}

void MainWindow::list_commitData()
{
	int nowRow = ui->listWidget->currentRow();  //得到現在的列
	QListWidgetItem* item = ui->listWidget->item(nowRow);  //得到現在的列表物品(ListItem)

	if (item->text().isEmpty())
	{
		manager.removeItem(item, nowRow);
		ui->listWidget->takeItem(nowRow);  //從列表中移除物品
	}
	else
	{
		manager.input(item->text().toStdString(), item, nowRow);
	}
	manager.showGraph();
}
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
	int nowRow = ui->listWidget->currentRow();
	manager.editItem(item, nowRow);
}


void MainWindow::on_pushButton_clicked()
{

}


void MainWindow::on_listWidget_itemChanged(QListWidgetItem* item)
{
	int nowRow = ui->listWidget->indexFromItem(item).row();

	if (item->checkState() == Qt::Checked && Storage::graphs.at(nowRow)->status == 0)
	{
		Storage::graphs.at(nowRow)->status = 1;
		manager.showGraph();
	}
	else if(item->checkState() == Qt::Unchecked && Storage::graphs.at(nowRow)->status == 1)
	{
		Storage::graphs.at(nowRow)->status = 0;
		if (Storage::graphs.at(nowRow)->graph != nullptr)
		{
			ui->graphicsView->scene->removeItem(Storage::graphs.at(nowRow)->graph);
			Storage::graphs.at(nowRow)->graph = nullptr;
		}
	}
}

