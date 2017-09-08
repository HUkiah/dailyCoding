#include "meshprint.h"
#include "maintenance.h"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Meshprint w;
	MyDialog dlg;
	w.show();
	QObject::connect(&dlg, SIGNAL(SendMsg()), &w, SLOT(RecvMsg()));

	return a.exec();
}
