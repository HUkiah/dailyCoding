#include <QtGui>
#include <QtWidgets>
#include "mainwindow.h"
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent)
{

	openAction = new QAction(QIcon(":/images/file-open"), tr("&Open..."), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an existing file"));
	connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

	saveAction = new QAction(QIcon(":/images/file-save"), tr("&Save..."), this);
	saveAction->setShortcuts(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save a new file"));
	connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

	mntnAction = new QAction(QIcon(":/images/file-maintenance"), tr("&Maintenance..."), this);
	mntnAction->setShortcuts(QKeySequence::Save);
	mntnAction->setStatusTip(tr("Maintenance Model file"));
	connect(mntnAction, &QAction::triggered, this, &MainWindow::mntnFile);

	QMenu *file = menuBar()->addMenu(tr("&File"));
	file->addAction(openAction);
	file->addAction(saveAction);
	file->addAction(mntnAction);

	QToolBar *toolBar = addToolBar(tr("&File"));
	toolBar->addAction(openAction);
	toolBar->addAction(saveAction);
	toolBar->addAction(mntnAction);

	textEdit = new QTextEdit(this);
	setCentralWidget(textEdit);
}

MainWindow::~MainWindow()
{
}

//void MainWindow::open()
//{
//    QMessageBox::information(this, tr("Information"), tr("Open"));
//}
void MainWindow::openFile()
{
	QString path = QFileDialog::getOpenFileName(this,
		tr("Open File"),
		".",
		tr("Text Files(*.txt)"));
	if (!path.isEmpty()) {
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QMessageBox::warning(this, tr("Read File"),
				tr("Cannot open file:\n%1").arg(path));
			return;
		}
		QTextStream in(&file);
		textEdit->setText(in.readAll());
		file.close();
	}
	else {
		QMessageBox::warning(this, tr("Path"),
			tr("You did not select any file."));
	}
}

void MainWindow::saveFile()
{
	QString path = QFileDialog::getSaveFileName(this,
		tr("Open File"),
		".",
		tr("Text Files(*.txt)"));
	if (!path.isEmpty()) {
		QFile file(path);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(this, tr("Write File"),
				tr("Cannot open file:\n%1").arg(path));
			return;
		}
		QTextStream out(&file);
		out << textEdit->toPlainText();
		file.close();
	}
	else {
		QMessageBox::warning(this, tr("Path"),
			tr("You did not select any file."));
	}
}

void MainWindow::mntnFile()
{
	qDebug() << "wait coding...";
}
