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
	}

	if (!watcher->addPath(file)) {
		view->setHtml("Failed");
	}

	baseUrl = QUrl("file://"+info.canonicalPath()+"/");

	view->settings()->setUserStyleSheetUrl(QUrl("file://"+QFileInfo("github.css").absoluteFilePath()));
	qDebug() << view->settings()->userStyleSheetUrl();
	renderer = hoedown_html_renderer_new(hoedown_html_flags(0), 0);
	// github style exensions
	hoedown_extensions ext = hoedown_extensions(HOEDOWN_EXT_TABLES |
			HOEDOWN_EXT_FENCED_CODE | HOEDOWN_EXT_AUTOLINK |
			HOEDOWN_EXT_STRIKETHROUGH | HOEDOWN_EXT_NO_INTRA_EMPHASIS);
	document = hoedown_document_new(renderer, ext, 16);

	connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(loadFile(QString)));

	setCentralWidget(view);
	loadFile(file);
}

void MainWindow::loadFile(const QString &path) {
	QFile f(path);
	if (!f.open(QFile::ReadOnly | QFile::Text)){
		view->setHtml("Failed");
	}
	QTextStream in(&f);

	QByteArray ba = in.readAll().toLocal8Bit();
	f.close();

	hoebuf = hoedown_buffer_new(16);
	hoedown_document_render(document, hoebuf, (unsigned char*)ba.constData(), ba.size());

	QString md = QString::fromLocal8Bit((char*)hoebuf->data, hoebuf->size);
	hoedown_buffer_free(hoebuf);

	qDebug() << "Reload";
	view->setHtml(header+md+footer, baseUrl);
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
