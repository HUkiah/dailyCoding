#include "maintenance.h"  
#include "ui_maintenance.h"  
#include "globalFunctions.h"

MyDialog::MyDialog(QWidget *parent) :
	QDialog(parent),ui(new Ui::MyDialog)
{
	ui->setupUi(this);
}

MyDialog::~MyDialog()
{
	delete ui;
}

void MyDialog::on_updateButton_clicked()
{
	emit SendMsg(tr("update"));
}

//����maintenanceֻ����һ��Dialog
void MyDialog::DialogClose() {
	actionMaintenanceFlag = true;
}