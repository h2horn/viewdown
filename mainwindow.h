#include <QMainWindow>

class QWebView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    QWebView *view;
};

