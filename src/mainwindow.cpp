#include <QtWidgets>
#include <QtWebKitWidgets>
#include <QDebug>
#include "mainwindow.h"

const QString MainWindow::header = QString("<html><head></head><body>");
const QString MainWindow::footer = QString("</body></html>");

MainWindow::MainWindow(const QString file)
{
	view = new QWebView(this);
	watcher = new QFileSystemWatcher();

	QFileInfo info = QFileInfo(file);
	if (!info.exists()) {
		view->setHtml("Failed");
		return;
	}

	if (!watcher->addPath(file)) {
		view->setHtml("Failed");
		return;
	}

	this->setWindowTitle(file + " - ViewDown");

	baseUrl = QUrl("file://"+info.canonicalPath()+"/");

	// user css style sheet
	view->settings()->setUserStyleSheetUrl(QUrl("qrc:///github.css"));
	// open links in external browser
	view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

	renderer = hoedown_html_renderer_new(hoedown_html_flags(0), 0);
	// github style exensions
	hoedown_extensions ext = hoedown_extensions(HOEDOWN_EXT_TABLES |
			HOEDOWN_EXT_FENCED_CODE | HOEDOWN_EXT_AUTOLINK |
			HOEDOWN_EXT_STRIKETHROUGH | HOEDOWN_EXT_NO_INTRA_EMPHASIS);
	document = hoedown_document_new(renderer, ext, 16);

	connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(loadFile(QString)));
	connect(view, SIGNAL(linkClicked(QUrl)), this, SLOT(openExtern(QUrl)));

	setCentralWidget(view);
	loadFile(file);
}

void MainWindow::loadFile(const QString &path) {
	QFile f(path);
	if (!f.open(QFile::ReadOnly | QFile::Text)){
		view->setHtml("Failed");
		return;
	}
	QTextStream in(&f);

	QByteArray ba = in.readAll().toLocal8Bit();
	f.close();

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
		default:
			QMainWindow::keyPressEvent(event);
	}
}

MainWindow::~MainWindow() {
	hoedown_document_free(document);
	hoedown_html_renderer_free(renderer);
}
