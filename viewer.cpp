#include "viewer.h"
#include "mainwindow.h"

Viewer::Viewer()
{
	window.resize(987, 600);
	window.show();
}

void Viewer::changeButtonText(string str)
{
	window.ui->addButton->setText(QString::fromStdString(str));
}
