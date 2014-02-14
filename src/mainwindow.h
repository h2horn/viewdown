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

protected slots:
    void loadFile(const QString &path);

private:
    QWebView *view;
    QFileSystemWatcher *watcher;
    hoedown_renderer *renderer;
    hoedown_document *document;
    hoedown_buffer *hoebuf;
};

