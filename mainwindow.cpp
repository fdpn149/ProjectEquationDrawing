#include "mainwindow.h"
#include <manager.h>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	connect(ui->listWidget->itemDelegate(), &QAbstractItemDelegate::commitData, this, &MainWindow::on_listWidget_commitData);
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

void MainWindow::on_listWidget_commitData()
{
	int nowRow = ui->listWidget->currentRow();  //得到現在的列
	QListWidgetItem* item = ui->listWidget->item(nowRow);  //得到現在的列表物品(ListItem)

	if (item->text().isEmpty())
	{
		manager.removeItem(nowRow);
		ui->listWidget->takeItem(nowRow);  //從列表中移除物品
		manager.checkError(-1);
	}
	else
	{
		manager.input(item->text().toStdString(), item, nowRow);
		manager.checkError(nowRow);
	}
	manager.showGraph();
}
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
	int nowRow = ui->listWidget->currentRow();
	manager.editItem(item, nowRow);
}

void MainWindow::on_listWidget_itemChanged(QListWidgetItem* item)
{
	int nowRow = ui->listWidget->indexFromItem(item).row();  //得到現在的列

	//若該項從未打勾變已打勾
	if (item->checkState() == Qt::Checked && Storage::graphs.at(nowRow)->status == 0)
	{
		Storage::graphs.at(nowRow)->status = 1;  //將狀態設為顯示

		manager.showGraph();  //顯示函數
	}
	//若該項從已打勾變未打勾
	else if(item->checkState() == Qt::Unchecked && Storage::graphs.at(nowRow)->status == 1)
	{
		Storage::graphs.at(nowRow)->status = 0;  //將狀態設為隱藏
		if (Storage::graphs.at(nowRow)->graph != nullptr)  //若儲存函數圖形的地方不為空
		{
			ui->graphicsView->scene->removeItem(Storage::graphs.at(nowRow)->graph);  //刪除畫面中的圖形
			delete Storage::graphs.at(nowRow)->graph;
			Storage::graphs.at(nowRow)->graph = nullptr;  //刪除保存的圖形
		}
	}
}

