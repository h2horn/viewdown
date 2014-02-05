#include <QtWidgets>
#include <QtWebKitWidgets>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    view = new QWebView(this);

    setCentralWidget(view);
}

