#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H  

#include <QtGui>
#include <QtWidgets>
#include <QtOpenGL/QtOpenGL>  
#include "ui_mainwindow.h"  
#include "mainwindow.h"

class RenderingWidget : public QGLWidget, public Ui::MainWindowClass
{
	Q_OBJECT

public:
	RenderingWidget(QWidget *parent = 0, const QGLWidget* shareEidget = 0);
	~RenderingWidget();

protected:
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void closeEvent(QCloseEvent *event);

private:
	GLfloat Point[5][3];    ///< Îå½ÇÐÇ×ø±ê  

};

#endif // RENDERINGWIDGET_H  