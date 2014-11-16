#include <QtWidgets>
#include <QtWebKitWidgets>
#include <QDebug>
#include "mainwindow.h"

const QString MainWindow::header = QString("<html><head></head><body>");
const QString MainWindow::footer = QString("</body></html>");

MainWindow::MainWindow(QStringList files, QUrl styleUrl)
{
	// setup webview
	view = new QWebView(this);
	// user css style sheet
	view->settings()->setUserStyleSheetUrl(styleUrl);
	// open links in external browser
	view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

	// setup markdown renderer
	renderer = hoedown_html_renderer_new(hoedown_html_flags(0), 0);
	// github style exensions
	hoedown_extensions ext = hoedown_extensions(HOEDOWN_EXT_TABLES |
			HOEDOWN_EXT_FENCED_CODE | HOEDOWN_EXT_AUTOLINK |
			HOEDOWN_EXT_STRIKETHROUGH | HOEDOWN_EXT_NO_INTRA_EMPHASIS);
	document = hoedown_document_new(renderer, ext, 16);

	watcher = new QFileSystemWatcher();
	connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(loadFile()));
	connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(loadFile()));
	connect(view, SIGNAL(linkClicked(QUrl)), this, SLOT(openExtern(QUrl)));

	setCentralWidget(view);

	if (files.size() == 0) {
		view->setHtml(header+"Open new file with <code>o</code>"+footer);
		return;
	}

	file = new QFile(files.at(0));
	info = new QFileInfo(files.at(0));
	if (!info->exists()) {
		view->setHtml(header+QString("File %1 doesn't exist!").arg(files.at(0))+footer);
		return;
	}

	for (int i = 0; i < files.size(); i++) {
		if (!watcher->addPath(files.at(i)))
			qWarning("Error watching %s.", qPrintable(files.at(i)));
	}

	this->setWindowTitle(files.at(0) + " - ViewDown");

	baseUrl = QUrl("file://"+info->canonicalPath()+"/");

	loadFile();
}

void MainWindow::loadFile() {
	info->refresh();
	if (!file->open(QFile::ReadOnly | QFile::Text)){
		view->setHtml("Failed");
		return;
	}
	QTextStream in(file);

	QByteArray ba = in.readAll().toLocal8Bit();
	file->close();

	hoebuf = hoedown_buffer_new(16);
	hoedown_document_render(document, hoebuf, (unsigned char*)ba.constData(), ba.size());

	QString md = QString::fromLocal8Bit((char*)hoebuf->data, hoebuf->size);
	hoedown_buffer_free(hoebuf);

	qDebug() << "Reload";
	// restore scroll position
	QPoint point = view->page()->mainFrame()->scrollPosition();
	view->setHtml(header+md+footer, baseUrl);
	view->page()->mainFrame()->setScrollPosition(point);
}

void MainWindow::openExtern(const QUrl &url) {
	QDesktopServices::openUrl(url);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
		case Qt::Key_Q:
			this->close();
			break;
		case Qt::Key_R:
			loadFile();
			break;
		default:
			QMainWindow::keyPressEvent(event);
	}
}

MainWindow::~MainWindow() {
	hoedown_document_free(document);
	hoedown_html_renderer_free(renderer);
}
