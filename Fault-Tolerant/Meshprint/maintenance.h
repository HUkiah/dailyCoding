#ifndef MYDIALOG_H  
#define MYDIALOG_H 

#include "globalFunctions.h"
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
	void SendMsg();

private slots:  
    void on_pushButton_clicked();
	void off_pushButton_clicked();

private:  
    Ui::MyDialog *ui;  
};  
#endif // MYDIALOG_H  