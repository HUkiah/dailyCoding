#include "meshprint.h"
#include "maintenance.h"

Meshprint::Meshprint(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.openGLWidget->parent = this;

}

Meshprint::~Meshprint()
{

}
