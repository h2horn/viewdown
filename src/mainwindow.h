#include <QMainWindow>
#include <QUrl>
#include <hoedown/html.h>
#include <hoedown/document.h>

class QWebView;
class QWebInspector;
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
	void reload();
	void openExtern(const QUrl &url);
	void requestNewFile();
	void toggleInspector();
	void print();

private:
	void loadCss(const QString &path);
	void loadNewFile(const QString &file);
	void closeEvent(QCloseEvent *event);
	void parseMarkdown(QString &data);

	QWebView *view;
	QWebInspector *inspector = NULL;
	QFileSystemWatcher *watcher;
	hoedown_renderer *renderer;
	hoedown_document *document;
	hoedown_buffer *hoebuf;
	QUrl baseUrl;
	QUrl styleUrl;
	QFile *file = NULL;
	QFileInfo *info = NULL;
	static const QString header;
	static const QString footer;
};

