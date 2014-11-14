#include <QMainWindow>
#include "hoedown/src/html.h"
#include "hoedown/src/document.h"

class QWebView;
class QFileSystemWatcher;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected slots:
    void loadFile(const QString &path);

private:
    void loadCss(const QString &path);

    QWebView *view;
    QFileSystemWatcher *watcher;
    hoedown_renderer *renderer;
    hoedown_document *document;
    hoedown_buffer *hoebuf;
    QString css;
    static const QString header;
    static const QString body;
    static const QString footer;
};

