#include <QtWidgets>
#include <QtWebKitWidgets>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    view = new QWebView(this);
    watcher = new QFileSystemWatcher();

    if (!watcher->addPath("test.md")) {
        view->setHtml("Failed", QUrl(""));
    }

    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(loadFile(QString)));

    setCentralWidget(view);
    loadFile("test.md");
}

void MainWindow::loadFile(const QString &path) {
    QFile f(path);
    if (!f.open(QFile::ReadOnly | QFile::Text)){
        view->setHtml("Failed", QUrl(""));
    }
    QTextStream in(&f);
    view->setHtml(in.readAll(), QUrl(""));
    f.close();
}

