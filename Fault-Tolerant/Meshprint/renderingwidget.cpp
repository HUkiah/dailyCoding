#include "renderingwidget.h"
#include <QKeyEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <iostream>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QTextCodec>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <algorithm>
#include <queue>
#include "ArcBall.h"
#include "globalFunctions.h"
#include "HE_mesh/Mesh3D.h"
#include "openGLProjector.h"
#include "QDebug"
#include "meshprint.h"
#include <fstream>
#include <QTime>
#include "maintenance.h"

class Support;

RenderingWidget::RenderingWidget(QWidget *parent, MainWindow* mainwindow)
	: QOpenGLWidget(parent), ptr_mainwindow_(mainwindow), eye_distance_(5.0),
	has_lighting_(true), is_draw_point_(false), is_draw_edge_(false), is_draw_face_(true)
{
	ptr_arcball_ = new CArcBall(width(), height());
	ptr_arcball_module_ = new CArcBall(width(), height());
	ptr_mesh_ = new Mesh3D();
	is_select_face = false;
	is_draw_hatch_ = false;
	is_load_texture_ = false;
	is_draw_axes_ = true;
	is_draw_texture_ = (false);
	is_draw_grid_ = (false);
	is_draw_cutpieces_ = (false);
	is_move_module_ = (false);
	is_show_all = false;
	eye_goal_[0] = eye_goal_[1] = eye_goal_[2] = 0.0;
	eye_direction_[0] = eye_direction_[1] = 0.0;
	eye_direction_[2] = 1.0;
}

RenderingWidget::~RenderingWidget()
{
	SafeDelete(ptr_arcball_);
	SafeDelete(ptr_arcball_module_);
	SafeDelete(ptr_mesh_);

//	SafeDelete(ptr_octree_);
}

void RenderingWidget::initializeGL()
{
	glClearColor(.1, .1, .1, 0.0);
//	glClearColor(0.7333, 0.8706,0.9643, 0.0);

	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);

	glEnable(GL_DOUBLEBUFFER);
	//glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1);

	SetLight();
}

void RenderingWidget::resizeGL(int w, int h)
{
	h = (h == 0) ? 1 : h;

	ptr_arcball_->reSetBound(w, h);
	ptr_arcball_module_->reSetBound(w, h);


	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, GLdouble(w) / GLdouble(h), 0.1, 10000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderingWidget::paintGL()
{
	glShadeModel(GL_SMOOTH);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (has_lighting_)
	{
		SetLight();
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	register vec eyepos = eye_distance_*eye_direction_;
	gluLookAt(eyepos[0], eyepos[1], eyepos[2],
		eye_goal_[0], eye_goal_[1], eye_goal_[2],
		0.0, 1.0, 0.0);
	glPushMatrix();

	glMultMatrixf(ptr_arcball_->GetBallMatrix());

	Render();
	glPopMatrix();
}

void RenderingWidget::timerEvent(QTimerEvent * e)
{
	update();
}

void RenderingWidget::mousePressEvent(QMouseEvent *e)
{

	switch (e->button())
	{
	case Qt::LeftButton:
	{
		makeCurrent();
		ptr_arcball_->MouseDown(e->pos());
		update();
	}
	break;
	case Qt::MidButton:
		current_position_ = e->pos();
		break;
	case  Qt::RightButton:
	{
		break;
	}

	default:
		break;
	}
}
void RenderingWidget::mouseMoveEvent(QMouseEvent *e)
{
	switch (e->buttons())
	{
		setCursor(Qt::ClosedHandCursor);

	case Qt::LeftButton:
		ptr_arcball_->MouseMove(e->pos());
		break;

	case Qt::MidButton:

		if (ptr_mesh_ != NULL)
		{
			eye_goal_[0] -= ptr_mesh_->getBoundingBox().at(0).at(2)*scaleV*GLfloat(e->x() - current_position_.x()) / GLfloat(width());
			eye_goal_[1] += ptr_mesh_->getBoundingBox().at(0).at(2)*scaleV*GLfloat(e->y() - current_position_.y()) / GLfloat(height());
		}
		current_position_ = e->pos();

		break;
	default:
		break;
	}
	update();
}
void RenderingWidget::mouseReleaseEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	case Qt::LeftButton:
		if (is_move_module_)
		{
			ptr_arcball_module_->MouseUp(e->pos());
		}
		else
		{
			ptr_arcball_->MouseUp(e->pos());
		}

		setCursor(Qt::ArrowCursor);

		ptr_arcball_->MouseUp(e->pos());
		setCursor(Qt::ArrowCursor);

		break;
	case Qt::RightButton:
		break;
	default:
		break;
	}
}
void RenderingWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
	switch (e->button())
	{
	default:
		break;
	}
	update();
}
void RenderingWidget::wheelEvent(QWheelEvent *e)
{
	if (ptr_mesh_ != NULL)
	{
		eye_distance_ -= e->delta()*ptr_mesh_->getBoundingBox().at(0).at(2)*scaleV / 1000;
	}
	eye_distance_ = eye_distance_ < 0 ? 0 : eye_distance_;
	update();
}

void RenderingWidget::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_A:
		break;
	default:
		break;
	}
}

void RenderingWidget::keyReleaseEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case Qt::Key_A:
		break;
	default:
		break;
	}
}

void RenderingWidget::Render()
{
	DrawAxes(is_draw_axes_);
	DrawGrid(is_draw_grid_);
	DrawPoints(is_draw_point_);
	DrawEdge(is_draw_edge_);
	DrawFace(is_draw_face_);
	DrawTexture(is_draw_texture_);
}

void RenderingWidget::SetLight()
{
	//return;
	static GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static GLfloat mat_shininess[] = { 50.0f };
	static GLfloat light_position0[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	static GLfloat light_position1[] = { -1.0f, -1.0f, 0.5f, 0.0f };
	static GLfloat light_position2[] = { -.0f, -.0f, -0.5f, 0.0f };
	static GLfloat bright[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	static GLfloat dim_light[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	static GLfloat lmodel_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };

	//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_specular);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, bright);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, bright);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, bright);
	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, dim_light);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
}

bool booladsad = true;
void RenderingWidget::SetBackground()
{
	QColor color = QColorDialog::getColor(Qt::white, this, tr("background color"));
	GLfloat r = (color.red()) / 255.0f;
	GLfloat g = (color.green()) / 255.0f;
	GLfloat b = (color.blue()) / 255.0f;
	GLfloat alpha = color.alpha() / 255.0f;
	makeCurrent();
	glClearColor(r, g, b, alpha);

	//updateGL();
	update();
}
//Reset Model View
void RenderingWidget::ResetView() {

	ptr_arcball_->InitBall();

	update();
}

//Menterance button
void RenderingWidget::RecvMsg(QString str) {

	if (str=="update")
	{
		//执行检测方法
		//发送信号，更新Dialog
	}
	//else
	//{
	//	updateFlag = true;
	//}
	int n = 0;
	for (int i = 0;i < ptr_mesh_->Tria.size();i++)
	{
		if (ptr_mesh_->Tria[i].selected == 1)
		{
			++n;
		}
	}
	//设置dialog上界面变化

	update();
}


void RenderingWidget::ApplyMaintenance() {
	qDebug() << "Maintenance!!!" << "\n";
	if (actionMaintenanceFlag)
	{
		actionMaintenanceFlag = false;

		MyDialog *dialog = new MyDialog(this);//当用WA_DeleteOnClose时，会不会存在二次析构的问题呢，不会，对象析构时，会在父对象的children table delete.父对象销毁时children也销毁，否则nodestory
		dialog->setAttribute(Qt::WA_DeleteOnClose);//在Dialog关闭时，进行析构，防止内存泄漏
		dialog->setWindowTitle(tr("Maintenance Dialog"));
		dialog->show();
		qDebug() << "over!!!" << dialog->result();
	}

	
}

void RenderingWidget::ReadMesh()
{
	QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
	QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
	ptr_arcball_->reSetBound(width(), height());
	ptr_arcball_module_->reSetBound(width(), height());
	ptr_mesh_->ClearData();
	is_draw_grid_ = false;
	is_draw_face_ = true;
	is_draw_cutpieces_ = true;
	is_draw_hatch_ = true;
	has_lighting_ = true;
	QString filename = QFileDialog::
		getOpenFileName(this, tr("Read Mesh"),
			"C:/Users/h/Desktop/Library/planeCollection/testCollection/", tr("Meshes (*.obj *.stl)"));

	if (filename.isEmpty())
	{
		//emit(operatorInfo(QString("Read Mesh Failed!")));
		return;
	}
	//中文路径支持
	QTextCodec *code = QTextCodec::codecForName("gd18030");
	QTextCodec::setCodecForLocale(code);

	//mycut->clearcut();

	QByteArray byfilename = filename.toLocal8Bit();
	QFileInfo fileinfo = QFileInfo(filename);
	//qDebug() << "read Mesh start time" << str;
	//qDebug() << byfilename.data();
	qDebug() << "load model time at " << time;

	if (fileinfo.suffix() == "obj")
	{
		ptr_mesh_->LoadFromOBJFile(byfilename.data());
	}
	else if (fileinfo.suffix() == "stl" || fileinfo.suffix() == "STL")
	{
		ptr_mesh_->LoadFromSTLFile(byfilename.data());
	}


	//m_pMesh->LoadFromOBJFile(filename.toLatin1().data());
	//emit(operatorInfo(QString("Read Mesh from") + filename + QString(" Done")));
	//emit(meshInfo(ptr_mesh_->num_of_vertex_list(), ptr_mesh_->num_of_edge_list(), ptr_mesh_->num_of_face_list()));


	float max_ = ptr_mesh_->getBoundingBox().at(0).at(0);
	max_ = max_ > ptr_mesh_->getBoundingBox().at(0).at(1) ? max_ : ptr_mesh_->getBoundingBox().at(0).at(1);
	max_ = max_ > ptr_mesh_->getBoundingBox().at(0).at(2) ? max_ : ptr_mesh_->getBoundingBox().at(0).at(2);


	//updateGL();
	update();
	time = QDateTime::currentDateTime();//获取系统现在的时间
	str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
													//qDebug() << "read mesh end time :" << str;

													// 	qDebug() << "孔洞个数为：" << ptr_mesh_->GetBLoop().size();
													// 	qDebug() << "法向面片错误" <<sss;
													//qDebug() << "法向错误面片个数："<<sss;
	qDebug() << "load model end at" << time;
	qDebug() << ptr_mesh_->get_faces_list()->size();
	qDebug() << ptr_mesh_->getBoundingBox().at(0)[0] * 2 << ptr_mesh_->getBoundingBox().at(0)[1] * 2 << ptr_mesh_->getBoundingBox().at(0)[2];
	 //ptr_octree_ = new MeshOctree();
	//构建AABB,Octree line support 
	// ptr_octree_->BuildOctree(ptr_mesh_);
	//FindRegion();
	//BuildSmartGrid();

	//ptr_arcball_->PlaceBall(scaleV);
	//scaleT = scaleV;
	eye_distance_ = 2 * max_;
}

void RenderingWidget::WriteMesh()
{
	if (ptr_mesh_->num_of_vertex_list() == 0)
	{
		emit(QString("The Mesh is Empty !"));
		return;
	}
	QString filename = QFileDialog::
		getSaveFileName(this, tr("Write Mesh"),
			"..", tr("Meshes (*.stl)"));
	if (filename.isEmpty())
		return;
	QByteArray byfilename = filename.toLocal8Bit();
	//ptr_mesh_->WriteToOBJFile(byfilename.data());
	ptr_mesh_->MntnMesh(byfilename.data());
}


void RenderingWidget::CheckDrawPoint()
{
	is_draw_point_ = !is_draw_point_;
	//updateGL();
	update();

}
void RenderingWidget::CheckDrawEdge()
{
	is_draw_edge_ = !is_draw_edge_;
	update();

}
void RenderingWidget::CheckDrawFace()
{
	is_draw_face_ = !is_draw_face_;
	update();

}
void RenderingWidget::CheckLight()
{
	has_lighting_ = !has_lighting_;
	update();

}
void RenderingWidget::CheckGrid()
{
	is_draw_grid_ = !is_draw_grid_;
	update();

}
void RenderingWidget::CheckDrawTexture()
{
	is_draw_texture_ = !is_draw_texture_;
	if (is_draw_texture_)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
	update();

}
void RenderingWidget::CheckDrawAxes()
{
	is_draw_axes_ = !is_draw_axes_;
	update();

}

void RenderingWidget::DrawAxes(bool bv)
{
	if (!bv)
		return;
	float max_x, max_y, max_z;
	float min_x, min_y, min_z;

	max_x = ptr_mesh_->getBoundingBox().at(0).at(0);
	max_y = ptr_mesh_->getBoundingBox().at(0).at(1);
	max_z = ptr_mesh_->getBoundingBox().at(0).at(2);

	min_x = ptr_mesh_->getBoundingBox().at(1).at(0) - 0.2;
	min_y = ptr_mesh_->getBoundingBox().at(1).at(1) - 0.2;
	min_z = ptr_mesh_->getBoundingBox().at(1).at(2) - 0.2;
	//x axis
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(min_x, min_y, min_z);
	glVertex3f(min_x+(max_x-min_x)/3.0, min_y, min_z);
	glEnd();
	glPushMatrix();
	glTranslatef(min_x + (max_x - min_x) / 3.0, min_y, min_z);
	glRotatef(90, 0.0, 1.0, 0.0);
	glutSolidCone(0.02, 0.06, 20, 10);
	glPopMatrix();

	//y axis
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(min_x, min_y, min_z);
	glVertex3f(min_x, min_y + (max_y - min_y) / 3.0, min_z);
	glEnd();

	glPushMatrix();
	glTranslatef(min_x, min_y + (max_y - min_y) / 3.0, min_z);
	glRotatef(90, -1.0, 0.0, 0.0);
	glutSolidCone(0.02, 0.06, 20, 10);
	glPopMatrix();

	//z axis
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(min_x, min_y, min_z);
	glVertex3f(min_x, min_y, min_z + (max_z - min_z) / 3.0);
	glEnd();
	glPushMatrix();
	glTranslatef(min_x, min_y, min_z + (max_z - min_z) / 3.0);
	glutSolidCone(0.02, 0.06, 20, 10);
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
}
void RenderingWidget::DrawPoints(bool bv)
{
	if (!bv || ptr_mesh_ == NULL)
		return;
	if (ptr_mesh_->num_of_vertex_list() == 0)
	{
		return;
	}

	const std::vector<HE_vert*>& verts = *(ptr_mesh_->get_vertex_list());

	glBegin(GL_POINTS);
	for (size_t i = 0; i != ptr_mesh_->num_of_vertex_list(); ++i)
	{
		glNormal3fv(verts[i]->normal().data());
		glVertex3fv((verts[i]->position()*scaleV).data());
	}
	glEnd();
}
void RenderingWidget::DrawEdge(bool bv)
{
	if (!bv || ptr_mesh_ == NULL)
		return;

	//if (ptr_mesh_->num_of_face_list() == 0)
	//{
	//	return;
	//}

	//const std::vector<HE_edge *>& edges = *(ptr_mesh_->get_edges_list());
	//const std::vector<HE_edge *>& bedges = *(ptr_mesh_->get_bedges_list());

	//for (size_t i = 0; i != edges.size(); ++i)
	//{
	//	glBegin(GL_LINES);
	//	glColor3f(0.0, 0.0, 0.0);
	//	glNormal3fv(edges[i]->start_->normal().data());
	//	glVertex3fv((edges[i]->start_->position()*scaleV).data());
	//	glNormal3fv(edges[i]->pvert_->normal().data());
	//	glVertex3fv((edges[i]->pvert_->position()*scaleV).data());
	//	glEnd();
	//}

	//for (size_t i = 0; i != bedges.size(); ++i)
	//{
	//	glBegin(GL_LINES);
	//	glColor3f(1.0, 0.0, 0.0);
	//	glNormal3fv(bedges[i]->start_->normal().data());
	//	glVertex3fv((bedges[i]->start_->position()*scaleV).data());
	//	glNormal3fv(bedges[i]->pvert_->normal().data());
	//	glVertex3fv((bedges[i]->pvert_->position()*scaleV).data());
	//	glEnd();
	//}
	//auto bl = ptr_mesh_->GetBLoop();
	//for (size_t i = 0; i != bl.size(); i++)
	//{
	//	glBegin(GL_LINE_LOOP);
	//	glColor3f(1.0, 0.0, 0.0);
	//	for (int j = 0; j < bl[i].size(); j++)
	//	{
	//		glNormal3fv(bl[i][j]->start_->normal().data());
	//		glVertex3fv((bl[i][j]->start_->position()*scaleV).data());
	//	}
	//	glEnd();
	//}
	for (size_t i = 0; i != ptr_mesh_->Tri.size(); ++i)
	{
		glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3fv((ptr_mesh_->Tri[i].Vertex_1*scaleV).data());
		glVertex3fv((ptr_mesh_->Tri[i].Vertex_2*scaleV).data());

		glVertex3fv((ptr_mesh_->Tri[i].Vertex_3*scaleV).data());//顺序不能调换
		glVertex3fv((ptr_mesh_->Tri[i].Vertex_1*scaleV).data());

		glVertex3fv((ptr_mesh_->Tri[i].Vertex_2*scaleV).data());
		glVertex3fv((ptr_mesh_->Tri[i].Vertex_3*scaleV).data());
		glEnd();
	}

	float x_box,y_box,z_box,mid_box, mif_box,len;
	Vec3f max = ptr_mesh_->getBoundingBox().at(0);
	Vec3f min = ptr_mesh_->getBoundingBox().at(1);

	x_box = max.x() - min.x();
	y_box = max.y() - min.y();
	z_box = max.z() - min.z();

	mid_box = std::max(x_box, y_box);
	mif_box = std::max(y_box, z_box);
	if (mid_box == mif_box)
	{
		len = z_box;
	}
	else
	{
		len = y_box;
	}
	len = len / 8.0;
//	for (size_t i=0;i<ptr_mesh_->Tria.size();i++)
	//for (int i=1;i<9;i++)
	{
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		{
			//x_box = ptr_mesh_->Tria[i]._aabb._max.x() - ptr_mesh_->Tria[i]._aabb._min.x();
			//y_box = ptr_mesh_->Tria[i]._aabb._max.y() - ptr_mesh_->Tria[i]._aabb._min.y();
			//z_box = ptr_mesh_->Tria[i]._aabb._max.z() - ptr_mesh_->Tria[i]._aabb._min.z();
			Vec3f boxPoint;
			Vec3f boxPoint_1, boxPoint_2, boxPoint_3, boxPoint_4, boxPoint_5, boxPoint_6, boxPoint_7, boxPoint_8;
			//boxPoint_1 = ptr_mesh_->Tria[i]._aabb._max;
			//boxPoint_8 = ptr_mesh_->Tria[i]._aabb._min;
			boxPoint_1 = max;
			boxPoint_8 = min;

			boxPoint = boxPoint_1;
			boxPoint.x() = boxPoint.x() - x_box;
			boxPoint_2 = boxPoint;

			boxPoint = boxPoint_1;
			boxPoint.y() = boxPoint.y() - y_box;
			boxPoint_6 = boxPoint;

			boxPoint = boxPoint_1;
			boxPoint.z() = boxPoint.z() - z_box;
			boxPoint_4 = boxPoint;

			boxPoint = boxPoint_8;
			boxPoint.x() = boxPoint.x() + x_box;
			boxPoint_5 = boxPoint;

			boxPoint = boxPoint_8;
			boxPoint.y() = boxPoint.y() + y_box;
			boxPoint_3 = boxPoint;

			boxPoint = boxPoint_8;
			boxPoint.z() = boxPoint.z() + z_box;
			boxPoint_7 = boxPoint;

			glVertex3fv(boxPoint_1.data());
			glVertex3fv(boxPoint_2.data());

			glVertex3fv(boxPoint_1.data());
			glVertex3fv(boxPoint_4.data());

			glVertex3fv(boxPoint_1.data());
			glVertex3fv(boxPoint_6.data());

			glVertex3fv(boxPoint_2.data());
			glVertex3fv(boxPoint_3.data());

			glVertex3fv(boxPoint_2.data());
			glVertex3fv(boxPoint_7.data());

			glVertex3fv(boxPoint_8.data());
			glVertex3fv(boxPoint_3.data());

			glVertex3fv(boxPoint_8.data());
			glVertex3fv(boxPoint_7.data());

			glVertex3fv(boxPoint_8.data());
			glVertex3fv(boxPoint_5.data());

			glVertex3fv(boxPoint_5.data());
			glVertex3fv(boxPoint_4.data());

			glVertex3fv(boxPoint_5.data());
			glVertex3fv(boxPoint_6.data());

			glVertex3fv(boxPoint_3.data());
			glVertex3fv(boxPoint_4.data());

			glVertex3fv(boxPoint_6.data());
			glVertex3fv(boxPoint_7.data());
		}
		glEnd();

	}

}
void RenderingWidget::DrawFace(bool bv)
{
	if (!bv || ptr_mesh_ == NULL)
		return;

	if (ptr_mesh_->num_of_face_list() == 0)
	{
		return;
	}

	const std::vector<HE_face *>& faces = *(ptr_mesh_->get_faces_list());
	glBegin(GL_TRIANGLES);

	glColor4f(.5, .5, 1.0, 0.9);
	//for (size_t i = 0; i < faces.size(); ++i)
	//{
	//	glColor3f(0.8, 0.8, 0.8);
	//	//glColor3f(0.3647, 0.3647, 0.3647);
	//	
	//	//if (updateFlag) {
	//	if (ptr_mesh_->Tria[i].selected == 1)
	//	{
	//		glColor3f(0.8, 0, 0);
	//	//	continue;
	//	}
	//	//}

	//	HE_edge *pedge(faces.at(i)->pedge_);
	//	do
	//	{
	//		if (pedge == NULL)
	//		{
	//			break;
	//		}
	//		if (pedge == NULL || pedge->pface_->id() != faces.at(i)->id())
	//		{
	//			faces.at(i)->pedge_ = NULL;
	//			qDebug() << faces.at(i)->id() << "facet display wrong";
	//			break;
	//		}
	//		glNormal3fv(pedge->pvert_->normal().data());
	//		glVertex3fv((pedge->pvert_->position()*scaleV).data());
	//		pedge = pedge->pnext_;
	//	} while (pedge != faces.at(i)->pedge_);


	//}
	for (int i = 0; i < ptr_mesh_->Tri.size(); ++i)
	{
		//if (ptr_mesh_->Tria[i].partitionNumber == 1)
		//{
		//	continue;
		//}

		glColor3f(0.8, 0.8, 0.8);
		//glColor3f(0.3647, 0.3647, 0.3647);

		//if (updateFlag) {
		if (ptr_mesh_->Tri[i].selected == 1)
		{
			glColor3f(0.8, 0, 0);
			//	continue;
		}
		//}


		glVertex3fv((ptr_mesh_->Tri[i].Vertex_1*scaleV).data());
		glVertex3fv((ptr_mesh_->Tri[i].Vertex_2*scaleV).data());

		glVertex3fv((ptr_mesh_->Tri[i].Vertex_3*scaleV).data());//顺序不能调换
		glVertex3fv((ptr_mesh_->Tri[i].Vertex_1*scaleV).data());

		glVertex3fv((ptr_mesh_->Tri[i].Vertex_2*scaleV).data());
		glVertex3fv((ptr_mesh_->Tri[i].Vertex_3*scaleV).data());


	}
	glEnd();
	//return;

}
void RenderingWidget::DrawTexture(bool bv)
{
	if (!bv)
		return;
	if (ptr_mesh_->num_of_face_list() == 0 || !is_load_texture_)
		return;

	//默认使用球面纹理映射，效果不好
	ptr_mesh_->SphereTex();

	const std::vector<HE_face *>& faces = *(ptr_mesh_->get_faces_list());

	glBindTexture(GL_TEXTURE_2D, texture_[0]);
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i != faces.size(); ++i)
	{
		HE_edge *pedge(faces.at(i)->pedge_);
		do
		{
			/* 请在此处绘制纹理，添加纹理坐标即可 */
			glTexCoord2fv(pedge->pvert_->texCoord_.data());
			glNormal3fv(pedge->pvert_->normal().data());
			glVertex3fv((pedge->pvert_->position()*scaleV).data());

			pedge = pedge->pnext_;

		} while (pedge != faces.at(i)->pedge_);
	}

	glEnd();
}

void RenderingWidget::DrawGrid(bool bv)
{
	if (!bv)
		return;
	//x axis
	//glDisable(GL_LIGHTING);

	glColor3f(0.9, 0.9, 0.9);
	glBegin(GL_LINES);


	Vec3f box(ptr_mesh_->getBoundingBox().at(0)*scaleV - ptr_mesh_->getBoundingBox().at(1)*scaleV);
	for (int i = 1; i < 16; i++)
	{
		glVertex2f(-box[0], -box[1] + i*box[1] / 8);
		glVertex2f(box[0], -box[1] + i*box[1] / 8);

		glVertex2f(-box[0] + i*box[0] / 8, -box[1]);
		glVertex2f(-box[0] + i*box[0] / 8, box[1]);
	}

	glEnd();

	//glEnable(GL_LIGHTING);
}
