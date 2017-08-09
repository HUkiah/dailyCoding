#include "renderingwidget.h"  
#include <math.h>  
#include <gl\GLU.h>

const GLfloat PI = 3.1415926536f; 

RenderingWidget::RenderingWidget(QWidget *parent /* = 0 */, const QGLWidget* shareEidget /* = 0 */)
	: QGLWidget(parent, shareEidget)
{


	//setupUi(this);
	setMinimumSize(320, 240);

	resize(640, 480);
	setWindowTitle(tr("paint Model"));

	short angle = 18;
	for (short i = 0;i < 5;++i)
	{
		Point[i][0] = cos(angle * PI / 180);
		Point[i][1] = sin(angle * PI / 180);
		Point[i][2] = 0.0;      // Z值  
		angle += 72;
	}
}

RenderingWidget::~RenderingWidget()
{

}

void RenderingWidget::initializeGL()
{
	glShadeModel(GL_SMOOTH);

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, Point);
}
void RenderingWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, (GLint)width(), (GLint)height());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (GLfloat)width() / (GLfloat)height(), 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
}
void RenderingWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(-1.5, 0.0, -6.0);

	glBegin(GL_TRIANGLES);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glEnd();

	glTranslatef(3.0, 0.0, 0.0);

	glBegin(GL_LINE_LOOP);
	glArrayElement(1);
	glArrayElement(4);
	glArrayElement(2);
	glArrayElement(0);
	glArrayElement(3);
	glEnd();
}

void RenderingWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (windowState() & Qt::WindowFullScreen)
		showNormal();
	else
		showFullScreen();
}

void RenderingWidget::closeEvent(QCloseEvent *event)
{
	//QMessageBox box;
	//box.setWindowTitle(tr("退出"));
	//box.setIcon(QMessageBox::Warning);
	//box.setText(tr("确定要关闭OpenGL窗口吗？"));
	//box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	//if (QMessageBox::No == box.exec())
	//{
	//	event->ignore();
	//	return;
	//}

	QWidget::closeEvent(event);
}