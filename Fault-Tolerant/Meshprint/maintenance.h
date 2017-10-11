#ifndef MYDIALOG_H  
#define MYDIALOG_H 

#include <QDialog>  
namespace Ui {
	class MyDialog;
}

class MyDialog : public QDialog  
{  
    Q_OBJECT  
public:
	explicit MyDialog(QWidget *parent = 0);
	~MyDialog();

signals:
	void SendMsg(QString);
	//void sendUpdate();
public slots:	
	void on_updateButton_clicked();
	void DialogClose();


public:
	Ui::MyDialog *ui;

};  
#endif // MYDIALOG_H  