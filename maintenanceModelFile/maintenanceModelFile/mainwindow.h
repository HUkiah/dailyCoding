#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"



class MainWindow :public QMainWindow
{
public:
	Ui::MainWindowClass ui;

	MainWindow(QWidget *parent = 0);
	~MainWindow();

};

#endif // MAINWINDOW_H
