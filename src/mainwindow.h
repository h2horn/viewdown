#include <QMainWindow>
#include <QKeyEvent>
#include <hoedown/html.h>
#include <hoedown/document.h>

class QWebView;
class QFileInfo;
class QFile;
class QFileSystemWatcher;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QStringList files, QUrl styleUrl);
	~MainWindow();

protected slots:
	void loadFile();
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
	QFile *file;
	QFileInfo *info;
	static const QString header;
	static const QString footer;
};

