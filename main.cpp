#include "mainwindow.h"
#include "manager.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

QT_USE_NAMESPACE

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    manager.start();
    return app.exec();
}
