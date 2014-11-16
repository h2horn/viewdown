#include <QApplication>
#include <QFileInfo>
#include <QCommandLineParser>
#include "mainwindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QApplication::setApplicationName("ViewDown");
	QApplication::setApplicationVersion("1.0");

	QCommandLineParser parser;
	parser.setApplicationDescription("Markdown viewer, watching the file for saved changes.");
	parser.addPositionalArgument("file", "markdown file to view and reload on change.");
	parser.addPositionalArgument("watch", "files or directories to watch as trigger for reload.", "[watch...]");
	const QCommandLineOption styleOption("s", "css file to use as stylesheet.", "css");
	parser.addOption(styleOption);
	parser.addHelpOption();
	parser.addVersionOption();

	parser.process(app);

	QUrl styleUrl;
	if (parser.isSet(styleOption)) {
		const QString path = parser.value(styleOption);
		QFileInfo info = QFileInfo(path);
		if (info.exists())
			styleUrl = QUrl("file://"+info.canonicalPath()+"/");
		else
			qWarning("No such file: %s", qPrintable(path));
	}
	if (styleUrl.isEmpty())
		styleUrl = QUrl("qrc:///github.css");

	MainWindow *browser = new MainWindow(parser.positionalArguments(), styleUrl);
	browser->show();
	return app.exec();
}
