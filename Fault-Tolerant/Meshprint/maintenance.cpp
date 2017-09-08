#include "maintenance.h"  
#include "ui_maintenance.h"  
#include<QHBoxLayout>  


MyDialog::MyDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MyDialog)
{
	ui->setupUi(this);

}

MyDialog::~MyDialog()
{
	delete ui;
}

void MyDialog::off_pushButton_clicked()
{

	updateFlag = false;

	emit SendMsg();

}

void MyDialog::on_pushButton_clicked()
{
	
	updateFlag = true;

	emit SendMsg();

	//updateFlag = false;
}