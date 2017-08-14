#include "renderingwidget.h"
#include <gl\GLU.h>
#include "globalFunctions.h"
#include "QDebug"
#include <QKeyEvent>
#include <QTime>
#include "QFileDialog"
#include "QTextCodec"

RenderingWidget::RenderingWidget(QWidget *parent)
	: QOpenGLWidget(parent), has_lighting_(false), eye_distance_(5.0),
	is_draw_point_(false), is_draw_edge_(false), is_draw_face_(true)
{
	ptr_arcball_ = new CArcBall(width(), height());
	ptr_arcball_module_ = new CArcBall(width(), height());

	ptr_mesh_ = new Mesh3D();
	
	is_move_module_ = (false);
	is_load_texture_ = false;
	is_draw_axes_ = false;
	is_draw_texture_ = (false);
	is_draw_grid_ = (false);
	eye_goal_[0] = eye_goal_[1] = eye_goal_[2] = 0.0;
	eye_direction_[0] = eye_direction_[1] = 0.0;
	eye_direction_[2] = 1.0;
}

RenderingWidget::~RenderingWidget()
{
	SafeDelete(ptr_arcball_);
	SafeDelete(ptr_arcball_module_);
	SafeDelete(ptr_mesh_);
}

void RenderingWidget::initializeGL()
{
	glClearColor(.1, .1, .1, 0.0);
	glShadeModel(GL_SMOOTH);//平滑渐变 对应的GL_FLAT是单色方式（两点间取一点的颜色着色）
	glEnable(GL_DOUBLEBUFFER);//启用双缓冲

	//glEnable(GL_CULL_FACE);//开启剔除操作效果 避免不必要的渲染 

	glEnable(GL_COLOR_MATERIAL);//用颜色材质,使我们可以用颜色来贴物体
	glColorMaterial(GL_FRONT, GL_DIFFUSE);//最开始颜色材质对应的是ambient(环境)的。所以要给为diffuse(传播)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//则表示源颜色乘以自身的alpha 值，目标颜色乘以1.0减去源颜色的alpha值(不透明度)
	glEnable(GL_BLEND);//开启混合功能
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);//采样质量按照像素（GL_NICEST）进行。（GL_FASTEST）顶点、反走样
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);//只绘制最前面的一层
	glClearDepth(1);//功能为指定深度缓冲区的清除值（0-1）

	SetLight();
}

void RenderingWidget::resizeGL(int w, int h)
{
	h = (h == 0) ? 1 : h;

	ptr_arcball_->reSetBound(w, h);
	ptr_arcball_module_->reSetBound(w, h);


	glViewport(0, 0, w, h);//视口位置、大小
	glMatrixMode(GL_PROJECTION);//投影矩阵Model( 将要对投影矩阵操作)
	glLoadIdentity();//重置当前指定的矩阵为单位矩阵

	gluPerspective(45.0, GLdouble(w) / GLdouble(h), 0.1, 1000);//指定了观察的视景体

	glMatrixMode(GL_MODELVIEW);//Object space to Eyes space(将要设置模型空间位置)
	glLoadIdentity();//(设置其为单位矩阵)
}

void RenderingWidget::paintGL()
{
	glShadeModel(GL_SMOOTH);//设置着色模式

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除屏幕显示内容 深度和颜色都要清一下

	if (has_lighting_)
	{
		SetLight();
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

	glMatrixMode(GL_MODELVIEW);//(将要设置模型空间位置)
	glLoadIdentity();//(设置其为单位矩阵)

	register vec eyepos = eye_distance_*eye_direction_; 
	gluLookAt(eyepos[0], eyepos[1], eyepos[2],//eye position
		eye_goal_[0], eye_goal_[1], eye_goal_[2],//eye angel
		0.0, 1.0, 0.0);// Y coordinate

	glPushMatrix();

	glMultMatrixf(ptr_arcball_->GetBallMatrix());
	//用矩阵M 调用glMultMatrix 对顶点v的变换就从原来的C × v变成C × M × v
	Render();
	glPopMatrix();
}

void RenderingWidget::SetLight()
{
	/** 定义光源的属性值 */
	//GLfloat LightAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };    /**< 环境光参数 */
	GLfloat LightDiffuse[] = { 0.8f, 0.9f, 0.9f, 1.0f };    /**< 漫射光参数 */
	//GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   /**< 镜面光参数 */
	
	
	GLfloat LightPosition[] = { 0.0f, -1.414f, 1.0f, 0.0f };   /**< 光源位置 */

	GLfloat lmodel_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);//选择光照模型：其包括四项内容
														   ////全局环境光强度
														   ////观察点靠近场景还是位于无穷远处
														   ////对物体的正面和背面是否采用相同的光照计算
														   ////以及是否将镜面反射颜色同环境颜色和散射颜色分开，并在纹理操作后应用它
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);//把无限远的观察点改为局部观察点

	/** 设置光源的属性值 */
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);     /**< 设置环境光 */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);     /**< 设置漫射光 */
	//glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);   /**< 设置漫射光 */


	/** 启用光源 */
	glEnable(GL_LIGHTING);//启用光照 
	glEnable(GL_LIGHT0);//启用指定光源

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

//绘轴
void RenderingWidget::DrawAxes(bool bv)
{
	if (!bv)
		return;
	//x axis
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.7, 0.0, 0.0);
	glEnd();
	glPushMatrix();
	glTranslatef(0.7, 0, 0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glPopMatrix();

	//y axis
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.0, 0.7, 0.0);
	glEnd();

	glPushMatrix();
	glTranslatef(0.0, 0.7, 0);
	glRotatef(90, -1.0, 0.0, 0.0);
	glPopMatrix();

	//z axis
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0.0, 0.0, 0.7);
	glEnd();
	glPushMatrix();
	glTranslatef(0.0, 0, 0.7);
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

	if (ptr_mesh_->num_of_face_list() == 0)
	{
		return;
	}

	const std::vector<HE_edge *>& edges = *(ptr_mesh_->get_edges_list());
	const std::vector<HE_edge *>& bedges = *(ptr_mesh_->get_bedges_list());

	for (size_t i = 0; i != edges.size(); ++i)
	{
		glBegin(GL_LINES);
		glColor3f(0.0, 0.0, 0.0);
		glNormal3fv(edges[i]->start_->normal().data());
		glVertex3fv((edges[i]->start_->position()*scaleV).data());
		glNormal3fv(edges[i]->pvert_->normal().data());
		glVertex3fv((edges[i]->pvert_->position()*scaleV).data());
		glEnd();
	}

	for (size_t i = 0; i != bedges.size(); ++i)
	{
		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glNormal3fv(bedges[i]->start_->normal().data());
		glVertex3fv((bedges[i]->start_->position()*scaleV).data());
		glNormal3fv(bedges[i]->pvert_->normal().data());
		glVertex3fv((bedges[i]->pvert_->position()*scaleV).data());
		glEnd();
	}
	auto bl = ptr_mesh_->GetBLoop();
	for (size_t i = 0; i != bl.size(); i++)
	{
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 0.0);
		for (int j = 0; j < bl[i].size(); j++)
		{
			glNormal3fv(bl[i][j]->start_->normal().data());
			glVertex3fv((bl[i][j]->start_->position()*scaleV).data());
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

	glColor4f(.6, .5, .5, 0.9);
	for (size_t i = 0; i < faces.size(); ++i)
	{
		if (ptr_mesh_->Tria[i].selected==1)
		{
			glColor3f(0.8, 0, 0);
		}
		else
		{
			glColor3f(0, 0.8, 0);
		}

		HE_edge *pedge(faces.at(i)->pedge_);
		do
		{
			if (pedge == NULL)
			{
				break;
			}
			if (pedge == NULL || pedge->pface_->id() != faces.at(i)->id())
			{
				faces.at(i)->pedge_ = NULL;
				qDebug() << faces.at(i)->id() << "facet display wrong";
				break;
			}
			glNormal3fv(pedge->pvert_->normal().data());
			glVertex3fv((pedge->pvert_->position()*scaleV).data());
			pedge = pedge->pnext_;
		} while (pedge != faces.at(i)->pedge_);

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

void RenderingWidget::mousePressEvent(QMouseEvent *e)
{
	qDebug() << "Mouse Press!!!";

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
	qDebug() << "Mouse Move!!!";

	switch (e->buttons())
	{
		setCursor(Qt::ClosedHandCursor);//改变鼠标样式（close）

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

void RenderingWidget::ReadMesh()
{
	QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
	QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //设置显示格式
	ptr_arcball_->reSetBound(width(), height());
	ptr_arcball_module_->reSetBound(width(), height());
	ptr_mesh_->ClearData();
	is_draw_grid_ = true;
	is_draw_face_ = true;
	is_draw_cutpieces_ = true;
	is_draw_hatch_ = true;
	has_lighting_ = true;
	QString filename = QFileDialog::
		getOpenFileName(this, tr("Read Mesh"),
			"Resources/models", tr("Meshes (*.obj *.stl)"));

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


	//	m_pMesh->LoadFromOBJFile(filename.toLatin1().data());
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
	//ptr_arcball_->PlaceBall(scaleV);
	scaleT = scaleV;
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
			"..", tr("Meshes (*.txt)"));
	if (filename.isEmpty())
		return;
	QByteArray byfilename = filename.toLocal8Bit();
	ptr_mesh_->WriteToOBJFile(byfilename.data());
}

void RenderingWidget::CheckLight()
{
	has_lighting_ = !has_lighting_;
	update();
}

void RenderingWidget::CheckDrawEdge()
{
	is_draw_edge_ = !is_draw_edge_;
	update();
}

void RenderingWidget::MntnMesh() {
	qDebug() << "Maintenance Model !";
}


