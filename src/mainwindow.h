#include <QMainWindow>
#include <QKeyEvent>
#include "hoedown/src/html.h"
#include "hoedown/src/document.h"

class QWebView;
class QFileSystemWatcher;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(const QString file);
	~MainWindow();

protected slots:
	void loadFile(const QString &path);
	void openExtern(const QUrl &url);

private:
	void loadCss(const QString &path);
	void keyPressEvent(QKeyEvent *event);

	QWebView *view;
	QFileSystemWatcher *watcher;
	hoedown_renderer *renderer;
	hoedown_document *document;
	hoedown_buffer *hoebuf;
	QUrl baseUrl;
	static const QString header;
	static const QString body;
	static const QString footer;
};

