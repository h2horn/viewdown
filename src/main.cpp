#include <QApplication>
#include <QCommandLineParser>
#include "mainwindow.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QApplication::setApplicationName("ViewDown");
	QApplication::setApplicationVersion("1.0");

	QCommandLineParser parser;
	parser.setApplicationDescription("Markdown viewer");
	parser.addPositionalArgument("file", "file to view and auto refresh");
	parser.addHelpOption();
	parser.addVersionOption();
	parser.process(app);
	const QString file = parser.positionalArguments().at(0);

	MainWindow *browser = new MainWindow(file);
	browser->show();
	return app.exec();
}
