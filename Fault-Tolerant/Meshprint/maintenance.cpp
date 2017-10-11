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

//控制maintenance只产生一个Dialog
void MyDialog::DialogClose() {
	actionMaintenanceFlag = true;
}