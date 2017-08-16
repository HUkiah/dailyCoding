#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H

#include <QOpenGLWidget>
#include <QEvent>
#include "ArcBall.h"
#include "Vec.h"
//#include "MeshBase.h"
#include "Mesh3D.h"

using trimesh::vec;
using trimesh::point;

class MainWindow;

class RenderingWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	MainWindow *parent;

	RenderingWidget(QWidget *parent);
	~RenderingWidget();

public:
	CArcBall					*ptr_arcball_;
	CArcBall					*ptr_arcball_module_;

	Mesh3D						*ptr_mesh_;
	
	//Texture
	GLuint						texture_[1];
	bool						is_load_texture_;

	// eye
	GLfloat						eye_distance_;
	point						eye_goal_;
	vec							eye_direction_;
	QPoint						current_position_;


	// Render information

	bool						has_lighting_;//是否设置光源

	bool						is_draw_axes_;
	bool						is_draw_grid_;
	bool						is_draw_point_;
	bool						is_draw_edge_;
	bool						is_draw_face_;
	bool						is_draw_texture_;

	bool						is_move_module_;
	bool						is_draw_cutpieces_;
	bool						is_draw_hatch_;
private:
	void DrawAxes(bool bv);
	void DrawPoints(bool);
	void DrawEdge(bool);
	void DrawFace(bool);
	void DrawTexture(bool);
	void DrawGrid(bool bV);


protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	

	//mouse events
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseDoubleClickEvent(QMouseEvent *e);
	void wheelEvent(QWheelEvent *e);

private:
	void Render();
	void SetLight();

public slots:
	void MntnMesh();
	void ReadMesh();
	void WriteMesh();
	void CheckLight();
	void CheckDrawEdge();
	//void CheckDrawFace();
	//void CheckDrawPoint();
};

#endif // RENDERINGWIDGET_H
