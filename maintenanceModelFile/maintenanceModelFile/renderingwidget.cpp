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
	glShadeModel(GL_SMOOTH);//ƽ������ ��Ӧ��GL_FLAT�ǵ�ɫ��ʽ�������ȡһ�����ɫ��ɫ��
	glEnable(GL_DOUBLEBUFFER);//����˫����

	//glEnable(GL_CULL_FACE);//�����޳�����Ч�� ���ⲻ��Ҫ����Ⱦ 

	glEnable(GL_COLOR_MATERIAL);//����ɫ����,ʹ���ǿ�������ɫ��������
	glColorMaterial(GL_FRONT, GL_DIFFUSE);//�ʼ��ɫ���ʶ�Ӧ����ambient(����)�ġ�����Ҫ��Ϊdiffuse(����)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//���ʾԴ��ɫ���������alpha ֵ��Ŀ����ɫ����1.0��ȥԴ��ɫ��alphaֵ(��͸����)
	glEnable(GL_BLEND);//������Ϲ���
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);//���������������أ�GL_NICEST�����С���GL_FASTEST�����㡢������
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);//ֻ������ǰ���һ��
	glClearDepth(1);//����Ϊָ����Ȼ����������ֵ��0-1��

	SetLight();
}

void RenderingWidget::resizeGL(int w, int h)
{
	h = (h == 0) ? 1 : h;

	ptr_arcball_->reSetBound(w, h);
	ptr_arcball_module_->reSetBound(w, h);


	glViewport(0, 0, w, h);//�ӿ�λ�á���С
	glMatrixMode(GL_PROJECTION);//ͶӰ����Model( ��Ҫ��ͶӰ�������)
	glLoadIdentity();//���õ�ǰָ���ľ���Ϊ��λ����

	gluPerspective(45.0, GLdouble(w) / GLdouble(h), 0.1, 1000);//ָ���˹۲���Ӿ���

	glMatrixMode(GL_MODELVIEW);//Object space to Eyes space(��Ҫ����ģ�Ϳռ�λ��)
	glLoadIdentity();//(������Ϊ��λ����)
}

void RenderingWidget::paintGL()
{
	glShadeModel(GL_SMOOTH);//������ɫģʽ

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����Ļ��ʾ���� ��Ⱥ���ɫ��Ҫ��һ��

	if (has_lighting_)
	{
		SetLight();
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

	glMatrixMode(GL_MODELVIEW);//(��Ҫ����ģ�Ϳռ�λ��)
	glLoadIdentity();//(������Ϊ��λ����)

	register vec eyepos = eye_distance_*eye_direction_; 
	gluLookAt(eyepos[0], eyepos[1], eyepos[2],//eye position
		eye_goal_[0], eye_goal_[1], eye_goal_[2],//eye angel
		0.0, 1.0, 0.0);// Y coordinate

	glPushMatrix();

	glMultMatrixf(ptr_arcball_->GetBallMatrix());
	//�þ���M ����glMultMatrix �Զ���v�ı任�ʹ�ԭ����C �� v���C �� M �� v
	Render();
	glPopMatrix();
}

void RenderingWidget::SetLight()
{
	/** �����Դ������ֵ */
	//GLfloat LightAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };    /**< ��������� */
	GLfloat LightDiffuse[] = { 0.8f, 0.9f, 0.9f, 1.0f };    /**< �������� */
	//GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   /**< �������� */
	
	
	GLfloat LightPosition[] = { 0.0f, -1.414f, 1.0f, 0.0f };   /**< ��Դλ�� */

	GLfloat lmodel_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);//ѡ�����ģ�ͣ��������������
														   ////ȫ�ֻ�����ǿ��
														   ////�۲�㿿����������λ������Զ��
														   ////�����������ͱ����Ƿ������ͬ�Ĺ��ռ���
														   ////�Լ��Ƿ񽫾��淴����ɫͬ������ɫ��ɢ����ɫ�ֿ����������������Ӧ����
	
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);//������Զ�Ĺ۲���Ϊ�ֲ��۲��

	/** ���ù�Դ������ֵ */
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);     /**< ���û����� */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);     /**< ��������� */
	//glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);   /**< ��������� */


	/** ���ù�Դ */
	glEnable(GL_LIGHTING);//���ù��� 
	glEnable(GL_LIGHT0);//����ָ����Դ

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

//����
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

	//Ĭ��ʹ����������ӳ�䣬Ч������
	ptr_mesh_->SphereTex();

	const std::vector<HE_face *>& faces = *(ptr_mesh_->get_faces_list());

	glBindTexture(GL_TEXTURE_2D, texture_[0]);
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i != faces.size(); ++i)
	{
		HE_edge *pedge(faces.at(i)->pedge_);
		do
		{
			/* ���ڴ˴�������������������꼴�� */
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
		setCursor(Qt::ClosedHandCursor);//�ı������ʽ��close��

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
	QDateTime time = QDateTime::currentDateTime();//��ȡϵͳ���ڵ�ʱ��
	QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //������ʾ��ʽ
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
	//����·��֧��
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
	time = QDateTime::currentDateTime();//��ȡϵͳ���ڵ�ʱ��
	str = time.toString("yyyy-MM-dd hh:mm:ss ddd"); //������ʾ��ʽ
													//qDebug() << "read mesh end time :" << str;

													// 	qDebug() << "�׶�����Ϊ��" << ptr_mesh_->GetBLoop().size();
													// 	qDebug() << "������Ƭ����" <<sss;
													//qDebug() << "���������Ƭ������"<<sss;
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


