#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.openGLWidget->parent = this;//parent should let`s complier know it`s a variable, not a function
}
MainWindow::~MainWindow()
{
}

//void MainWindow::openFile()
//{
//	QString path = QFileDialog::getOpenFileName(this,
//		tr("Open File"),
//		".",
//		tr("Text Files(*.txt)"));
//	if (!path.isEmpty()) {
//		QFile file(path);
//		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//			QMessageBox::warning(this, tr("Read File"),
//				tr("Cannot open file:\n%1").arg(path));
//			return;
//		}
//		QTextStream in(&file);
//		textEdit->setText(in.readAll());
//		file.close();
//	}
//	else {
//		QMessageBox::warning(this, tr("Path"),
//			tr("You did not select any file."));
//	}
//}
//
//void MainWindow::saveFile()
//{
//	QString path = QFileDialog::getSaveFileName(this,
//		tr("Open File"),
//		".",
//		tr("Text Files(*.txt)"));
//	if (!path.isEmpty()) {
//		QFile file(path);
//		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
//			QMessageBox::warning(this, tr("Write File"),
//				tr("Cannot open file:\n%1").arg(path));
//			return;
//		}
//		QTextStream out(&file);
//		out << textEdit->toPlainText();
//		file.close();
//	}
//	else {
//		QMessageBox::warning(this, tr("Path"),
//			tr("You did not select any file."));
//	}
//}
//
//void MainWindow::mntnFile()
//{
//	qDebug() << "wait coding...";
//}
