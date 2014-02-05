#include <QApplication>
#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    MainWindow *browser = new MainWindow();
    browser->show();
    return app.exec();
}
