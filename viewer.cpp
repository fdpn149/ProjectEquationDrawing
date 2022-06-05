#include "viewer.h"
#include "mainwindow.h"
#include <iostream>
#include <QPainter>
using std::cout;
using std::endl;

Viewer::Viewer()
{
	window.resize(987, 600);
	window.show();
}

void Viewer::changeText(string str)
{
	system("cls");
	cout << str + " ";
}

void Viewer::addText(string str)
{
	cout << str + " ";
}

void Viewer::addItem(QColor color)
{
	QPixmap pixmap(40, 40);
	pixmap.fill(color);
	QPainter painter(&pixmap);
	painter.setFont(QFont("Consolas", 25));
	painter.drawText(QPoint(10, 30), " ");
	QListWidgetItem* item = new QListWidgetItem("");
	item->setIcon(pixmap);
	item->setFont(QFont("Microsoft JhengHei UI", 20));
    window.ui->listWidget->addItem(item);
}

void Viewer::editItem(QListWidgetItem* item, QColor color)
{
	item->setText("");  //這裡只能把原有的輸入清掉
	QPixmap pixmap(40, 40);
	pixmap.fill(color);
	QPainter painter(&pixmap);
	painter.setFont(QFont("Consolas", 25));
	painter.drawText(QPoint(10, 30), " ");
	item->setIcon(pixmap);
}

void Viewer::changeItemIcon(QListWidgetItem* item, int type, QColor color)
{
	QPixmap pixmap(40, 40);
	pixmap.fill(color);
	QPainter painter(&pixmap);
	painter.setFont(QFont("Consolas",25));
	painter.setPen(QColor("white"));
	
	if(type == 0)
		painter.drawText(QPoint(10, 30), " ");
	else if(type == -1)
		painter.drawText(QPoint(10, 30), "E");

	item->setIcon(pixmap);
}

void Viewer::changeItemIcon(int index, int type, QColor color)
{
	QPixmap pixmap(40, 40);
	pixmap.fill(color);
	QPainter painter(&pixmap);
	painter.setFont(QFont("Consolas", 25));
	painter.setPen(QColor("white"));

	if (type == 0)
		painter.drawText(QPoint(10, 30), " ");
	else if (type == -1)
		painter.drawText(QPoint(10, 30), "E");

	window.getListItem(index)->setIcon(pixmap);
}
