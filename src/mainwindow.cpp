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

    renderer = hoedown_html_renderer_new(0, 0);
    document = hoedown_document_new(renderer, 0, 16);

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

    QByteArray ba = in.readAll().toLocal8Bit();

    hoebuf = hoedown_buffer_new(64);
    hoedown_document_render(document, hoebuf, (unsigned char*)ba.constData(), ba.size());

    view->setHtml(QString::fromLocal8Bit((char*)hoebuf->data), QUrl(""));
    hoedown_buffer_free(hoebuf);
    f.close();
}

