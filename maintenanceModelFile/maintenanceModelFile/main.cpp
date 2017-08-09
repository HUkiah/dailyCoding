#include "mainwindow.h"
#include <QApplication>
#include "renderingwidget.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	RenderingWidget w;
	w.show();
	return app.exec();
}
