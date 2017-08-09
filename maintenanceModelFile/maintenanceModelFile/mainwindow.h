#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

class QTextEdit;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	private slots:
	void openFile();
	void saveFile();
	void mntnFile();

private:
	QAction *openAction;
	QAction *saveAction;
	QAction *mntnAction;

	QTextEdit *textEdit;
};

#endif // MAINWINDOW_H
