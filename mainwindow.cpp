#include "mainwindow.h"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QValueAxis>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QLineSeries* series = new QLineSeries();
    *series << QPointF(1.0, 1.0) << QPointF(2.0, 73.0) << QPointF(3.0, 268.0) << QPointF(4.0, 17.0)
        << QPointF(5.0, 4325.0) << QPointF(6.0, 723.0);
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    ///chart->setTitle("Logarithmic axis example");

    QValueAxis* axisX = new QValueAxis();
    //axisX->setTitleText("Data point");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(series->count());
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    //axisY->setTitleText("Data point");
    axisY->setLabelFormat("%i");
    axisY->setTickCount(series->count());
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //![4]
    ui->horizontalLayout_2->addWidget(chartView);

    connect(ui->listWidget->itemDelegate(), &QAbstractItemDelegate::commitData, this, &MainWindow::OnLstItemsCommitData);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_addButton_clicked()
{
    QListWidgetItem* item = new QListWidgetItem("");
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    item->setFont(QFont("Microsoft JhengHei UI", 20));
    ui->listWidget->addItem(item);
}

void MainWindow::OnLstItemsCommitData(QWidget* pLineEdit)
{
    QString strNewText = reinterpret_cast<QLineEdit*>(pLineEdit)->text();
    int nRow = ui->listWidget->currentRow();
    if (strNewText.isEmpty())
    {
        ui->listWidget->takeItem(nRow);
    }
}