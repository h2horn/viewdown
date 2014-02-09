#include <QMainWindow>

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
};

