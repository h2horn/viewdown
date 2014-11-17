#include <QtWidgets>
#include <QtWebKitWidgets>
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
	connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(reload()));
	connect(watcher, SIGNAL(directoryChanged(QString)), this, SLOT(reload()));
	connect(view, SIGNAL(linkClicked(QUrl)), this, SLOT(openExtern(QUrl)));

	setCentralWidget(view);

	if (files.size() == 0) {
		view->setHtml(header+"Open new file with <code>o</code>"+footer);
		return;
	}

	for (int i = 1; i < files.size(); i++) {
		if (!watcher->addPath(files.at(i)))
			qWarning("Error watching %s.", qPrintable(files.at(i)));
	}

	loadNewFile(files.at(0));
}

void MainWindow::requestNewFile() {
	QString dirName;
	if (info)
		dirName = info->canonicalPath();
	if (dirName.isEmpty())
		dirName = QDir::homePath();
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
			dirName,
			tr("Markdown (*.md);;All Files (*)"));

	if (fileName.isEmpty())
		return;

	loadNewFile(fileName);
}

void MainWindow::loadNewFile(const QString &fileName) {
	// remove old watcher
	if (info)
		watcher->removePath(info->fileName());

	file = new QFile(fileName);
	info = new QFileInfo(fileName);
	if (!info->exists()) {
		view->setHtml(header+QString("File %1 doesn't exist!").arg(fileName)+footer);
		return;
	}

	this->setWindowTitle(fileName + " - ViewDown");

	baseUrl = QUrl("file://"+info->canonicalPath()+"/");

	if (!watcher->addPath(fileName))
		qWarning("Error watching %s.", qPrintable(fileName));

	reload();
}

void MainWindow::reload() {
	info->refresh();
	if (!file->open(QFile::ReadOnly | QFile::Text)){
		view->setHtml(header+QString("Failed to open  %1!").arg(info->fileName())+footer);
		return;
	}
	QTextStream in(file);

	QByteArray ba = in.readAll().toLocal8Bit();
	file->close();

	hoebuf = hoedown_buffer_new(16);
	hoedown_document_render(document, hoebuf, (unsigned char*)ba.constData(), ba.size());

	QString md = QString::fromLocal8Bit((char*)hoebuf->data, hoebuf->size);
	hoedown_buffer_free(hoebuf);

	// restore scroll position
	QPoint point = view->page()->mainFrame()->scrollPosition();
	view->setHtml(header+md+footer, baseUrl);
	view->page()->mainFrame()->setScrollPosition(point);
}

void MainWindow::openExtern(const QUrl &url) {
	QDesktopServices::openUrl(url);
}

void MainWindow::toggleInspector() {
	if (!inspector) {
		view->page()->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
		inspector = new QWebInspector;
		inspector->setPage(view->page());
	}
	if (inspector->isVisible())
		inspector->setVisible(false);
	else
		inspector->setVisible(true);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
	switch (event->key()) {
		case Qt::Key_Q:
			close();
			break;
		case Qt::Key_R:
			reload();
			break;
		case Qt::Key_O:
			requestNewFile();
			break;
		case Qt::Key_I:
			toggleInspector();
			break;
		default:
			QMainWindow::keyPressEvent(event);
	}
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (inspector)
		inspector->close();
	QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow() {
	hoedown_document_free(document);
	hoedown_html_renderer_free(renderer);
}
