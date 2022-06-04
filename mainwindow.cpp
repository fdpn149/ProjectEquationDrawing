#include "mainwindow.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QValueAxis>
#include <QLineEdit>
#include <manager.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QLineSeries* series = new QLineSeries();
    //*series << QPointF(1.0, 1.0) << QPointF(2.0, 73.0) << QPointF(3.0, 268.0) << QPointF(4.0, 17.0)
    //    << QPointF(5.0, 4325.0) << QPointF(6.0, 723.0);
    //QChart* chart = new QChart();
    //chart->addSeries(series);
    //chart->legend()->hide();
    /////chart->setTitle("Logarithmic axis example");

    //QValueAxis* axisX = new QValueAxis();
    ////axisX->setTitleText("Data point");
    //axisX->setLabelFormat("%i");
    //axisX->setTickCount(series->count());
    //chart->addAxis(axisX, Qt::AlignBottom);
    //series->attachAxis(axisX);

    //QValueAxis* axisY = new QValueAxis();
    ////axisY->setTitleText("Data point");
    //axisY->setLabelFormat("%i");
    //axisY->setTickCount(series->count());
    //chart->addAxis(axisY, Qt::AlignLeft);
    //series->attachAxis(axisY);

    //QChartView* chartView = new QChartView(chart);
    //chartView->setRenderHint(QPainter::Antialiasing);
    //ui->horizontalLayout_2->addWidget(chartView);

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

