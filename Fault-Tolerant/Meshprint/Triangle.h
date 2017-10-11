#ifndef TRIANGLE_HAN
#define TRIANGLE_HAN
#include "HE_mesh/Vec.h"
#include "globalFunctions.h"

#include <QDebug>
#include "MSAABB.h"

/*
���������������ཻʱ˼·���£�
��������������Ĺ�ϵ�������Ǹ���������ƽ��Ϊ���У�
1.����һ������������һ������������ƽ���һ�࣬��ʱ����������һ�����ཻ��
ʵ���ϣ���һ���������ϵ�һ����ֱ�����һ�����������������ϵ��ĵ㣬�ж��Ķ����λ�ù�ϵ
2.�������ϵ�һ��������һ�����������ڵ�ƽ���ϣ����е��Ƿ����������ڵ��ж�
3.��������һ��������������ֱ�λ����һ������������ƽ������ࣨ������������ƽ���ϵ������
ʵ�������߶�����Ľ����Ƿ�����������
*/


typedef trimesh::vec3  Vec3f;

enum TopologicalStructure
{
	INTERSECT, NONINTERSECT
};
enum pointInTriangleStatus
{
	INTHEFACE, ONTHETOP, NOINTHETRIANGLE
};

struct InsFaceIdAndEdge
{
//	int ID = -1;				//��¼�ཻ���ID
	int insEdgeStartPoint = 0;	//�ཻ�ߵ�start�˵�
	int insEdgeEndPoint = 0;	//�ཻ�ߵ�End�˵�
	int insRedundantPoint = 0;  //ʣ��Ķ˵�
	Vec3f insPoint;				//�ཻ��
	bool operator < (const InsFaceIdAndEdge &a) const // ���ء�<�����������Զ����������  
	{
		//��ID�ɴ�С�������Ҫ��С��������ʹ�á�>�����ɡ�  
		return a.insPoint > insPoint;
	}
};

class Triangle
{
private:
	int triId;					//���ն�ȡ˳�򣬸�����������ID��

public:
	Vec3f normal;
	Vec3f Vertex_1, Vertex_2, Vertex_3;
	AABB _aabb;					//AABB��Χ��
	InsFaceIdAndEdge INS;

	Vec3f centerpoint;			//���������� -
	double radius;				//����������Բ�뾶 -
	int selected = 0;			//���������Ƭ�Ƿ��ཻ�ı�־λ
	int partitionNumber = 0;	//������ţ���¼�������洦���ĸ��� -
	std::vector<Triangle> triVar;		//��¼�����ཻ��������ID��
	std::set<InsFaceIdAndEdge> triPoint;	//��¼�������������еĽ���

	//std::vector<int> insEdgePoint; //�ཻ�ߵ�һ���˵�


public:
	int getTriId() {
		return triId;
	}
	void setTriId(int num) {
		triId = num;
	}
	AABB getAABB() {
		return _aabb;
	}

	Vec3f mappingValue(int value) {
		if (value == 1)
		{
			return Vertex_1;
		}
		else if (value == 2)
		{
			return Vertex_2;
		}
		else if (value == 3)
		{
			return Vertex_3;
		}
	}

	inline bool operator==(Triangle &v)
	{
		if (this->Vertex_1 == v.Vertex_1&&this->Vertex_2 == v.Vertex_2&&this->Vertex_3 == v.Vertex_3)
			return true;
		else
			return false;
	}
};

struct pointTri
{
	double x = 0, y = 0;
};

//inline void expandBoundary() {
//
//#define MAX_FLOAT_VALUE (static_cast<float>(10e10))
//#define MIN_FLOAT_VALUE	(static_cast<float>(-10e10))
//
//	_max.x() = _max.y() = _max.z() = MIN_FLOAT_VALUE;
//	_min.x() = _min.y() = _min.z() = MAX_FLOAT_VALUE;
//
//	_min.x() = std::min(_min.x(), (*viter)->position_.x());
//	_min.y() = min(_min.y(), (*viter)->position_.y());
//	_min.z() = min(_min.z(), (*viter)->position_.z());
//	_max.x() = max(_max.x(), (*viter)->position_.x());
//	_max.y() = max(_max.y(), (*viter)->position_.y());
//	_max.z() = max(_max.z(), (*viter)->position_.z());
//
//
//	//�����Χ�����ܣ�������ֻ��һ����
//	_min.x() = _min.x() - 0.01;
//	_min.y() = _min.y() - 0.01;
//	_min.z() = _min.z() - 0.01;
//
//	_max.x() = _max.x() + 0.01;
//	_max.y() = _max.y() + 0.01;
//	_max.z() = _max.z() + 0.01;
//}

inline void copy_pointXY(pointTri &a, Vec3f b) {
	a.x = b[0];
	a.y = b[1];
}

inline void copy_pointXZ(pointTri &a, Vec3f b) {
	a.x = b[0];
	a.y = b[2];
}

inline void copy_pointYZ(pointTri &a, Vec3f b) {
	a.x = b[1];
	a.y = b[2];
}

inline double twoPointDistance(Vec3f a, Vec3f b) {
	return sqrt(pow((a[0] - b[0]), 2) + pow((a[1] - b[1]), 2) + pow((a[2] - b[2]), 2));
}

//�ж������Ƿ����
inline bool is_equal_vertex(Vec3f pointTri1, Vec3f pointTri2) {
	//if (pointTri1.x() - pointTri2.x() < 0.000001&&pointTri1.y() - pointTri2.y() < 0.000001&& pointTri1.z() - pointTri2.z() < 0.000001)
	if (pointTri1==pointTri2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//�ĵ�����ʽ  
inline float get_vector4_det(Vec3f v1, Vec3f v2, Vec3f v3, Vec3f v4)
{
	float a[3][3];
	for (int i = 0; i != 3; ++i)
	{
		a[0][i] = v1[i] - v4[i];
		a[1][i] = v2[i] - v4[i];
		a[2][i] = v3[i] - v4[i];
	}

	return a[0][0] * a[1][1] * a[2][2]
		+ a[0][1] * a[1][2] * a[2][0]
		+ a[0][2] * a[1][0] * a[2][1]
		- a[0][2] * a[1][1] * a[2][0]
		- a[0][1] * a[1][0] * a[2][2]
		- a[0][0] * a[1][2] * a[2][1];

	///ƽ�淽��A*x+B*y+C*z+D=0;
	//float A = ((v2.y() - v1.y())*(v3.z() - v1.z()) - (v2.z() - v1.z())*(v3.y() - v1.y()));
	//float B = ((v2.z() - v1.z())*(v3.x() - v1.x()) - (v2.x() - v1.x())*(v3.z() - v1.z()));
	//float C = ((v2.x() - v1.x())*(v3.y() - v1.y()) - (v2.y() - v1.y())*(v3.x() - v1.x()));
	//float D = -(A * v1.x() + B * v1.y() + C * v1.z());

	//return (A*v4.x() + B*v4.y() + v4.z()*C + D);
}

//���ò�������p����߶�p1p2�ķ�λ  
inline double direction(pointTri p1, pointTri p2, pointTri p) {
	return  (p2.x - p1.x) * (p.y - p1.y) - (p.x - p1.x) * (p2.y - p1.y);
}

//ȷ�����߶�p1p2���ߵĵ�p�Ƿ����߶�p1p2��  
inline int on_segment(pointTri p1, pointTri p2, pointTri p) {
	double max = p1.x > p2.x ? p1.x : p2.x;
	double min = p1.x < p2.x ? p1.x : p2.x;
	double max1 = p1.y > p2.y ? p1.y : p2.y;
	double min1 = p1.y < p2.y ? p1.y : p2.y;

	if (p.x >= min && p.x <= max && p.y >= min1 && p.y <= max1)//�޸� ȥ����=��,��ȥ���ߵ㣬�붥����ͬ
	{
		//�ڶ�ά�ռ���ȥ���� �ߣ���Ĺ�ϵ���������뷨����������ע�͵��뷨��
		return 1;
	}
	else
	{
		return 0;
	}
}

inline void get_center_point_of_circle(Triangle* tri)
{
	Vec3f centerpoint_var;

	double a1, b1, c1, d1;
	double a2, b2, c2, d2;
	double a3, b3, c3, d3;

	double x1 = tri->Vertex_1[0], y1 = tri->Vertex_1[1], z1 = tri->Vertex_1[2];
	double x2 = tri->Vertex_2[0], y2 = tri->Vertex_2[1], z2 = tri->Vertex_2[2];
	double x3 = tri->Vertex_3[0], y3 = tri->Vertex_3[1], z3 = tri->Vertex_3[2];

	a1 = (y1*z2 - y2*z1 - y1*z3 + y3*z1 + y2*z3 - y3*z2);
	b1 = -(x1*z2 - x2*z1 - x1*z3 + x3*z1 + x2*z3 - x3*z2);
	c1 = (x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2);
	d1 = -(x1*y2*z3 - x1*y3*z2 - x2*y1*z3 + x2*y3*z1 + x3*y1*z2 - x3*y2*z1);

	a2 = 2 * (x2 - x1);
	b2 = 2 * (y2 - y1);
	c2 = 2 * (z2 - z1);
	d2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;

	a3 = 2 * (x3 - x1);
	b3 = 2 * (y3 - y1);
	c3 = 2 * (z3 - z1);
	d3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;

	centerpoint_var[0] = -(b1*c2*d3 - b1*c3*d2 - b2*c1*d3 + b2*c3*d1 + b3*c1*d2 - b3*c2*d1)
		/ (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
	centerpoint_var[1] = (a1*c2*d3 - a1*c3*d2 - a2*c1*d3 + a2*c3*d1 + a3*c1*d2 - a3*c2*d1)
		/ (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
	centerpoint_var[2] = -(a1*b2*d3 - a1*b3*d2 - a2*b1*d3 + a2*b3*d1 + a3*b1*d2 - a3*b2*d1)
		/ (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
	tri->centerpoint = centerpoint_var;
	tri->radius = sqrt(pow((x1 - centerpoint_var[0]), 2) + pow((x1 - centerpoint_var[1]), 2) + pow((x1 - centerpoint_var[2]), 2));
}

inline void get_central_point(Vec3f centralPoint, Triangle* tri)
{
	centralPoint[0] = (tri->Vertex_1[0] + tri->Vertex_2[0] + tri->Vertex_3[0]) / 3;

	centralPoint[1] = (tri->Vertex_1[1] + tri->Vertex_2[1] + tri->Vertex_3[1]) / 3;

	centralPoint[2] = (tri->Vertex_1[2] + tri->Vertex_2[2] + tri->Vertex_3[2]) / 3;
}

//����֮��  
inline void get_vector_diff(Vec3f& aimV, const Vec3f a, const Vec3f b)
{
	aimV[0] = b[0] - a[0];

	aimV[1] = b[1] - a[1];

	aimV[2] = b[2] - a[2];
}

//�����ڻ�  �� ���
inline double Dot(const Vec3f& v1, const Vec3f& v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

//�жϵ����߶�p3p4�Ƿ��ߵ�������  
inline int segments_collinear(Vec3f p1, Vec3f p2) {
	Vec3f p;
	if (p1.cross(p2) == p)
	{
		return 1;
	}
	return 0;
}

//�ж��߶�p1p2���߶�p3p4�Ƿ��ཻ��������  
inline int segments_intersert(pointTri p1, pointTri p2, pointTri p3, pointTri p4) {
	double d1, d2, d3, d4;
	d1 = direction(p3, p4, p1);
	d2 = direction(p3, p4, p2);
	d3 = direction(p1, p2, p3);
	d4 = direction(p1, p2, p4);
	if (d1 * d2 < 0 && d3 * d4 < 0)
	{
		return 1;
	}//���ߵ��ǵ��붥���غϣ��ж����ཻ(�����Ҷ����غϵ������棬�ж�Ϊ���ཻ)
	else if (d1 == 0 && on_segment(p3, p4, p1) == 1)
	{
		return 1;
	}
	else if (d2 == 0 && on_segment(p3, p4, p2) == 1)
	{
		return 1;
	}
	else if (d3 == 0 && on_segment(p1, p2, p3) == 1)
	{
		return 1;
	}
	else if (d4 == 0 && on_segment(p1, p2, p4) == 1)
	{
		return 1;
	}
	return 0;
}

//���ķ��жϵ��Ƿ����������ڲ�  �ų����ϵ���� ���������λ��ͬһ��ƽ���� -_-...
//inline pointInTriangleStatus is_pointTri_within_triangle(Triangle* tri, Vec3f pointTri)
//{
//	Vec3f v0;
//	get_vector_diff(v0, tri->Vertex_1, tri->Vertex_3);
//	Vec3f v1;
//	get_vector_diff(v1, tri->Vertex_1, tri->Vertex_2);
//	Vec3f v2;
//	get_vector_diff(v2, tri->Vertex_1, pointTri);
//	float dot00 = Dot(v0, v0);
//	float dot01 = Dot(v0, v1);
//	float dot02 = Dot(v0, v2);
//	float dot11 = Dot(v1, v1);
//	float dot12 = Dot(v1, v2);
//	float inverDeno = 1 / (dot00* dot11 - dot01* dot01);
//	float u = (dot11* dot02 - dot01* dot12) * inverDeno;
//	if (u < 0 || u > 1) // if u out of range, return directly  �����"="��ȥ���ڶ�������
//	{
//		return NOINTHETRIANGLE;
//	}
//	float v = (dot00* dot12 - dot01* dot02) * inverDeno;
//	if (v < 0 || v > 1) // if v out of range, return directly  
//	{
//		return NOINTHETRIANGLE;
//	}
//	if ((v == 0 && u == 1) || (u == 0 && v == 1) || (u == 0 && v == 0))
//	{
//		//qDebug() << "U+V==1";
//		return ONTHETOP;
//	}
//	else if (u + v <= 1)//����1�����Ӧ���ǵ���������
//	{
//		return INTHEFACE;
//	}
//	return NOINTHETRIANGLE;
//
//}

// Determine whether two vectors v1 and v2 point to the same direction
// v1 = Cross(AB, AC)
// v2 = Cross(AB, AP)
inline bool SameSide(Vec3f A, Vec3f B, Vec3f C, Vec3f P)
{
    Vec3f AB = B - A ;
    Vec3f AC = C - A ;
    Vec3f AP = P - A ;

    Vec3f v1 = AB.cross(AC) ;
    Vec3f v2 = AB.cross(AP) ;

    // v1 and v2 should point to the same direction
    return v1.dot(v2) >= 0 ;
}

// Same side method
// Determine whether point P in triangle ABC
inline bool PointinTriangle1(Triangle *tri, Vec3f P)
{
    return SameSide(tri->Vertex_1, tri->Vertex_2, tri->Vertex_3, P) &&
        SameSide(tri->Vertex_2, tri->Vertex_3, tri->Vertex_1, P) &&
        SameSide(tri->Vertex_3, tri->Vertex_1, tri->Vertex_2, P) ;
}

inline bool is_pointTri_within_triangle(Triangle* tri, Vec3f pointTri)
{
	//return SameSide(tri->Vertex_1, tri->Vertex_2, tri->Vertex_3, pointTri) &&
	//	SameSide(tri->Vertex_2, tri->Vertex_3, tri->Vertex_1, pointTri) &&
	//	SameSide(tri->Vertex_3, tri->Vertex_1, tri->Vertex_2, pointTri);

	Vec3f v0;
	get_vector_diff(v0, tri->Vertex_1, tri->Vertex_3);
	Vec3f v1;
	get_vector_diff(v1, tri->Vertex_1, tri->Vertex_2);
	Vec3f v2;
	get_vector_diff(v2, tri->Vertex_1, pointTri);
	double dot00 = Dot(v0, v0);
	double dot01 = Dot(v0, v1);
	double dot02 = Dot(v0, v2);
	double dot11 = Dot(v1, v1);
	double dot12 = Dot(v1, v2);
	double inverDeno = 1 / (dot00* dot11 - dot01* dot01);
	double u = (dot11* dot02 - dot01* dot12) * inverDeno;
	if (u < 0 || u > 1) // if u out of range, return directly  �����"="��ȥ���ڶ�������
	{
		return false;
	}
	double v = (dot00* dot12 - dot01* dot02) * inverDeno;
	if (v < 0 || v > 1) // if v out of range, return directly  
	{
		return false;
	}
	else if (u + v <= 1)//����1�����Ӧ���ǵ���������
	{
		return true;
	}
	return false;

}
inline bool is_pointTri_within_Line(Vec3f f1,Vec3f f2,Vec3f point) {
		if (((f1.x() - point.x())*(point.x() - f2.x()))>=0&& ((f1.y() - point.y())*(point.y() - f2.y())) >= 0&& ((f1.z() - point.z())*(point.z() - f2.z())) >= 0)
		{
			return true;
		}
		return false;
}

//�жϵ��Ƿ��������ε�һ������
inline bool is_pointTri_within_triangle_vectex(Triangle *tri, Vec3f pointTri) {
	if (is_equal_vertex(tri->Vertex_1, pointTri) || is_equal_vertex(tri->Vertex_2, pointTri) || is_equal_vertex(tri->Vertex_3, pointTri))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//�жϿռ������㹲��
inline bool is_three_point_within_SameLine(Vec3f f1, Vec3f f2, Vec3f f3) {
	Vec3f m1, m2;
	m1 = f2 - f1;
	m2 = f3 - f2;

	if (m1.x() / m2.x() == m1.y() / m2.y() && m1.y() / m2.y() == m1.z() / m2.z())
	{
		return true;
	}
	return false;
}


//�жϵ��Ƿ�������ε�һ�߹���(�ռ��еĹ�������ת����2ά�Ĺ��ߴ�����ʧ����)
inline bool is_pointTri_within_triangle_inSameEdgeLine(Triangle *tri, Vec3f f1, Vec3f f2) {

	if (is_three_point_within_SameLine(tri->Vertex_2, f1, f2) && is_three_point_within_SameLine(tri->Vertex_3, f1, f2))
	{
		return true;
	}
	else if (is_three_point_within_SameLine(tri->Vertex_1, f1, f2) && is_three_point_within_SameLine(tri->Vertex_3, f1, f2))
	{
		return true;
	}
	else if (is_three_point_within_SameLine(tri->Vertex_2, f1, f2) && is_three_point_within_SameLine(tri->Vertex_1, f1, f2))
	{
		return true;
	}
	return false;


	Vec3f pa1, pb1, pa2, pb2, pa3, pb3;

	pa1 = tri->Vertex_1 - f1;
	pb1 = tri->Vertex_2 - f2;

	pa2 = tri->Vertex_1 - f1;
	pb2 = tri->Vertex_3 - f2;

	pa3 = tri->Vertex_2 - f1;
	pb3 = tri->Vertex_3 - f2;

	//����
	//if ((segments_collinear(pa1, pb1) && segments_collinear(pc1,pd1))||(segments_collinear(pa2, pb2)&& segments_collinear(pc2, pd2))||(segments_collinear(pa3, pb3)&&segments_collinear(pc3, pd3)))
	if (segments_collinear(pa1, pb1) || segments_collinear(pa2, pb2) || segments_collinear(pa3, pa3))
	{
		pa1 = tri->Vertex_1 - f2;
		pb1 = tri->Vertex_2 - f1;

		pa2 = tri->Vertex_1 - f2;
		pb2 = tri->Vertex_3 - f1;

		pa3 = tri->Vertex_2 - f2;
		pb3 = tri->Vertex_3 - f1;
		if (segments_collinear(pa1, pb1) || segments_collinear(pa2, pb2) || segments_collinear(pa3, pa3)) {
			return true;
		}

	}
	return false;
}

//���model���յ�������Ƭ���ص�����ų�����,����û������Ҫ��ĵ㣬�Ҳ���˵�������ʣ���Ҫ������һ��
inline bool need_triangle_rule_out(Triangle *tri, Vec3f f1, Vec3f f2) {

	//if �߶����������ϵ�һ���߹���

	//if ���㶼���������ڣ����ʱ���Ѿ��ڶ���ͬһ�������ˣ�
	//return NOINTERSECT
	//else if ֻ��һ������������
	//if �Ƕ���
	//return NONINTERSECT

	if (is_pointTri_within_triangle(tri, f1) && is_pointTri_within_triangle(tri, f2))
	{
		return true;//NONINTERSECT
	}
	else if (is_pointTri_within_triangle(tri, f1))
	{
		if (is_pointTri_within_triangle_vectex(tri, f1))
		{
			return true;
		}
	}
	else if (is_pointTri_within_triangle(tri, f2))
	{
		if (is_pointTri_within_triangle_vectex(tri, f2))
		{
			return true;
		}
	}

	return false;


}

//�ж�ͬһƽ���ֱ�ߺ��������Ƿ��ཻ  
inline bool line_triangle_intersert_inSamePlane(Triangle* tri, Vec3f f1, Vec3f f2)
{

	//a:XY�� b:XZ�� c:YZ��
	pointTri pa1, pa2, pb1, pb2, pc1, pc2, pa3, pa4, pb3, pb4, pc3, pc4;

	copy_pointXY(pa1, f1);

	copy_pointXY(pa2, f2);


	copy_pointXZ(pb1, f1);

	copy_pointXZ(pb2, f2);


	copy_pointYZ(pc1, f1);

	copy_pointYZ(pc2, f2);



	copy_pointXY(pa3, tri->Vertex_1);

	copy_pointXY(pa4, tri->Vertex_2);


	copy_pointXZ(pb3, tri->Vertex_1);

	copy_pointXZ(pb4, tri->Vertex_2);


	copy_pointYZ(pc3, tri->Vertex_1);

	copy_pointYZ(pc4, tri->Vertex_2);

	if (segments_intersert(pa1, pa2, pa3, pa4) && segments_intersert(pb1, pb2, pb3, pb4) && segments_intersert(pc1, pc2, pc3, pc4))
	{
		return true;
	}

	copy_pointXY(pa3, tri->Vertex_2);

	copy_pointXY(pa4, tri->Vertex_3);


	copy_pointXZ(pb3, tri->Vertex_2);

	copy_pointXZ(pb4, tri->Vertex_3);


	copy_pointYZ(pc3, tri->Vertex_2);

	copy_pointYZ(pc4, tri->Vertex_3);

	if (segments_intersert(pa1, pa2, pa3, pa4) && segments_intersert(pb1, pb2, pb3, pb4) && segments_intersert(pc1, pc2, pc3, pc4))
	{
		return true;
	}

	copy_pointXY(pa3, tri->Vertex_1);

	copy_pointXY(pa4, tri->Vertex_3);


	copy_pointXZ(pb3, tri->Vertex_1);

	copy_pointXZ(pb4, tri->Vertex_3);


	copy_pointYZ(pc3, tri->Vertex_1);

	copy_pointYZ(pc4, tri->Vertex_3);

	if (segments_intersert(pa1, pa2, pa3, pa4) && segments_intersert(pb1, pb2, pb3, pb4) && segments_intersert(pc1, pc2, pc3, pc4))
	{
		return true;
	}

	return false;
}

//�ж�ͬһƽ���ڵ��������Ƿ��ཻ  
inline bool triangle_intersert_inSamePlane(Triangle* tri1, Triangle* tri2)
{
	//ͬһƽ���У���������棨Ψһһ�㣩�ų��Ƕ�����ڱ��ϵĵ�

	if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_1) && (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_2)  || is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_3)))
	{
		return false;
	}
	else if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_2) && is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_3))
	{
		return false;
	}

	if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_1) && !is_pointTri_within_triangle(tri1, tri2->Vertex_2)&& !is_pointTri_within_triangle(tri1, tri2->Vertex_3))
	{
		return false;
	}
	else if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_2) && !is_pointTri_within_triangle(tri1, tri2->Vertex_1)&& !is_pointTri_within_triangle(tri1, tri2->Vertex_3))
	{
		return false;
	}
	else if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_3) && !is_pointTri_within_triangle(tri1, tri2->Vertex_2)&& !is_pointTri_within_triangle(tri1, tri2->Vertex_1))
	{
		return false;
	}



	if (line_triangle_intersert_inSamePlane(tri2, tri1->Vertex_1, tri1->Vertex_2))
	{
		return true;
	}
	else if (line_triangle_intersert_inSamePlane(tri2, tri1->Vertex_2, tri1->Vertex_3))
	{
		return true;
	}
	else if (line_triangle_intersert_inSamePlane(tri2, tri1->Vertex_1, tri1->Vertex_3))
	{
		return true;
	}
	else
	{
		//�Ȳ����ص���

		//Vec3f centralPoint1, centralPoint2;
		//get_central_point(centralPoint1, tri1);
		//get_central_point(centralPoint2, tri2);
		//if (is_pointTri_within_triangle(tri2, centralPoint1) || is_pointTri_within_triangle(tri1, centralPoint2))
		//{
		//	return true;
		//}

		return false;
	}
}

inline bool Cal_Plane_Line_IntersectPoint(Triangle *tri,Vec3f f1,Vec3f f2,Vec3f& point)
{

	float p1_f1 = get_vector4_det(tri->Vertex_1, tri->Vertex_2, tri->Vertex_3, f1);

	float p1_f2 = get_vector4_det(tri->Vertex_1, tri->Vertex_2, tri->Vertex_3, f2);

	//qDebug() << p1_f1 << "    " << p1_f2 << "     " << "........";

	if (p1_f1*p1_f2>0)
	{
		return false;
	}

	Vec3f var0;
	get_vector_diff(var0, tri->Vertex_1, tri->Vertex_3);
	Vec3f var1;
	get_vector_diff(var1, tri->Vertex_1, tri->Vertex_2);

	Vec3f planeVector= var0.cross(var1);
	Vec3f planePoint = tri->Vertex_1;

	Vec3f lineVector;
	get_vector_diff(lineVector, f1, f2);

	Vec3f linePoint=f1;

	float vp1, vp2, vp3, n1, n2, n3, v1, v2, v3, m1, m2, m3, t, vpt;
	vp1 = planeVector.x();
	vp2 = planeVector.y();
	vp3 = planeVector.z();
	n1 = planePoint.x();
	n2 = planePoint.y();
	n3 = planePoint.z();
	v1 = lineVector.x();
	v2 = lineVector.y();
	v3 = lineVector.z();
	m1 = linePoint.x();
	m2 = linePoint.y();
	m3 = linePoint.z();
	vpt = v1 * vp1 + v2 * vp2 + v3 * vp3;

	/*if (abs(vpt) < 1e-3)
	{
		qDebug() << "..................";
		point = linePoint + lineVector;
		if (point < linePoint)
		{

			point = linePoint;
		}
		point.y() -= 0.01;
	}*/
	if(vpt!=0)
	{
		t = ((n1 - m1) * vp1 + (n2 - m2) * vp2 + (n3 - m3) * vp3) / vpt;
		point.x() = m1 + v1 * t;
		point.y() = m2 + v2 * t;
		point.z() = m3 + v3 * t;
		return true;
	}
	return false;
}


inline bool is_TriangleIntersect_within_NoSamePlane(Triangle *tri1,Triangle *tri2) {
	
	insStartPoint = 0;	//�ཻ�ߵ�start�˵㣬������¼
	insEndPoint = 0;	//�ཻ�ߵ�End�˵�
	insRedunPoint = 0;	//�ཻ�ߵ������

	INSPOINT_X = 0;
	INSPOINT_Y = 0;
	INSPOINT_Z = 0;

	Vec3f point;

	if (Cal_Plane_Line_IntersectPoint(tri2, tri1->Vertex_1, tri1->Vertex_2, point) && is_pointTri_within_triangle(tri2, point))
	{
		qDebug() << ".........";
		if (is_pointTri_within_triangle_vectex(tri2, point))
		{
			return false;
		}
		//�жϵ��Ƿ����߶��ϣ�����Ӧ���������
		if (is_pointTri_within_Line(tri1->Vertex_1, tri1->Vertex_2, point))
		{
			insStartPoint = 1;
			insEndPoint = 2;
			insRedunPoint = 3;
			INSPOINT_X = point.x();
			INSPOINT_Y = point.y();
			INSPOINT_Z = point.z();
		}
		return true;
	}

	if (Cal_Plane_Line_IntersectPoint(tri2, tri1->Vertex_1, tri1->Vertex_3, point) && is_pointTri_within_triangle(tri2, point))
	{
		qDebug() << "++++++++++++++++";
		if (is_pointTri_within_triangle_vectex(tri2, point))
		{
			return false;
		}
		//�жϵ��Ƿ����߶��ϣ�����Ӧ���������
		if (is_pointTri_within_Line(tri1->Vertex_1, tri1->Vertex_3, point))
		{
			insStartPoint = 1;
			insEndPoint = 3;
			insRedunPoint = 2;
			INSPOINT_X = point.x();
			INSPOINT_Y = point.y();
			INSPOINT_Z = point.z();
		}
		return true;
	}

	if (Cal_Plane_Line_IntersectPoint(tri2, tri1->Vertex_2, tri1->Vertex_3, point) && is_pointTri_within_triangle(tri2, point))
	{
		qDebug() << "-----------------";
		if (is_pointTri_within_triangle_vectex(tri2, point))
		{
			return false;
		}
		//�жϵ��Ƿ����߶��ϣ�����Ӧ���������
		if (is_pointTri_within_Line(tri1->Vertex_2, tri1->Vertex_3, point))
		{
			insStartPoint = 2;
			insEndPoint = 3;
			insRedunPoint = 1;
			INSPOINT_X = point.x();
			INSPOINT_Y = point.y();
			INSPOINT_Z = point.z();
		}
		return true;
	}

	if (Cal_Plane_Line_IntersectPoint(tri1, tri2->Vertex_1, tri2->Vertex_2, point)&&is_pointTri_within_triangle(tri1,point))
	{
		if (is_pointTri_within_triangle_vectex(tri1, point))
		{
			return false;
		}
		//�жϵ��Ƿ����߶��ϣ�����Ӧ���������
		if (is_pointTri_within_Line(tri2->Vertex_1, tri2->Vertex_2, point))
		{
			insStartPoint = 1;
			insEndPoint = 2;
			insRedunPoint = 3; 
			INSPOINT_X = point.x();
			INSPOINT_Y = point.y();
			INSPOINT_Z = point.z();
		}
		return true;
	}
	
	if (Cal_Plane_Line_IntersectPoint(tri1, tri2->Vertex_1, tri2->Vertex_3, point)&&is_pointTri_within_triangle(tri1, point))
	{
		if (is_pointTri_within_triangle_vectex(tri1, point))
		{
			return false;
		}
		if (is_pointTri_within_Line(tri2->Vertex_1, tri2->Vertex_3, point))
		{
			insStartPoint = 1;
			insEndPoint = 3;
			insRedunPoint = 2;
			INSPOINT_X = point.x();
			INSPOINT_Y = point.y();
			INSPOINT_Z = point.z();
		}
		return true;
	}
	
	if (Cal_Plane_Line_IntersectPoint(tri1, tri2->Vertex_2, tri2->Vertex_3, point)&&is_pointTri_within_triangle(tri1, point))
	{

		if (is_pointTri_within_triangle_vectex(tri1, point))
		{
			return false;
		}
		if (is_pointTri_within_Line(tri2->Vertex_2, tri2->Vertex_3, point))
		{
			insStartPoint = 2;
			insEndPoint = 3;
			insRedunPoint = 1;
			INSPOINT_X = point.x();
			INSPOINT_Y = point.y();
			INSPOINT_Z = point.z();
		}
		return true;
	}
	
	
	qDebug() << "0000000000";
	return false;
}


//Devillers�㷨������  
inline TopologicalStructure judge_triangle_topologicalStructure(Triangle* tri1, Triangle* tri2)
{
	////��tri1���ڵ�ƽ��Ϊp1,tri2���ڵ�ƽ��Ϊp2
	float p1_tri2_vertex1 = get_vector4_det(tri1->Vertex_1, tri1->Vertex_2, tri1->Vertex_3, tri2->Vertex_1);

	float p1_tri2_vertex2 = get_vector4_det(tri1->Vertex_1, tri1->Vertex_2, tri1->Vertex_3, tri2->Vertex_2);

	float p1_tri2_vertex3 = get_vector4_det(tri1->Vertex_1, tri1->Vertex_2, tri1->Vertex_3, tri2->Vertex_3);

	//qDebug() << p1_tri2_vertex1 << "   " << p1_tri2_vertex2 << "   " << p1_tri2_vertex3 << "\n";

	if (p1_tri2_vertex1 > 0 && p1_tri2_vertex2 > 0 && p1_tri2_vertex3 > 0)
	{
		return NONINTERSECT;
	}

	if (p1_tri2_vertex1 < 0 && p1_tri2_vertex2 < 0 && p1_tri2_vertex3 < 0)
	{
		return NONINTERSECT;
	}


	if (p1_tri2_vertex1 == 0 && p1_tri2_vertex2 == 0 && p1_tri2_vertex3 == 0)
	{
		if (triangle_intersert_inSamePlane(tri1, tri2))
		{
			return INTERSECT;/////////////////////////
		}
		else
		{
			return NONINTERSECT;
		}
	}

	////�����������������ڵ�ƽ����
	//if (p1_tri2_vertex1 == 0 && p1_tri2_vertex2 == 0 && p1_tri2_vertex3 != 0)
	//{
	//	if (is_pointTri_within_triangle(tri1, tri2->Vertex_1) == ONTHETOP&&is_pointTri_within_triangle(tri1, tri2->Vertex_2) == ONTHETOP)
	//	{
	//		return NONINTERSECT;
	//	}
	//}
	//else if (p1_tri2_vertex1 == 0 && p1_tri2_vertex3 == 0 && p1_tri2_vertex2 != 0)
	//{
	//	if (is_pointTri_within_triangle(tri1, tri2->Vertex_1) == ONTHETOP&&is_pointTri_within_triangle(tri1, tri2->Vertex_3) == ONTHETOP)
	//	{
	//		return NONINTERSECT;
	//	}
	//}
	//else if (p1_tri2_vertex2 == 0 && p1_tri2_vertex3 == 0 && p1_tri2_vertex1 != 0)
	//{
	//	if (is_pointTri_within_triangle(tri1, tri2->Vertex_2) == ONTHETOP&&is_pointTri_within_triangle(tri1, tri2->Vertex_3) == ONTHETOP)
	//	{
	//		return NONINTERSECT;
	//	}
	//}

	if (p1_tri2_vertex1 == 0 && p1_tri2_vertex2 == 0 && p1_tri2_vertex3 != 0)
	{
		if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_1)&& is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_2))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_1)&&!is_pointTri_within_triangle(tri1, tri2->Vertex_2))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_2) && !is_pointTri_within_triangle(tri1, tri2->Vertex_1))
		{
			return NONINTERSECT;
		}
		else if (line_triangle_intersert_inSamePlane(tri1, tri2->Vertex_1, tri2->Vertex_2))
		{
			return INTERSECT;
		}
	}
	else if (p1_tri2_vertex1 == 0 && p1_tri2_vertex3 == 0 && p1_tri2_vertex2 != 0)
	{
		if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_1)&&is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_3))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_1) && !is_pointTri_within_triangle(tri1, tri2->Vertex_3))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_3) && !is_pointTri_within_triangle(tri1, tri2->Vertex_1))
		{
			return NONINTERSECT;
		}
		else if (line_triangle_intersert_inSamePlane(tri1, tri2->Vertex_1, tri2->Vertex_3))
		{
			return INTERSECT;
		}
	}
	else if (p1_tri2_vertex2 == 0 && p1_tri2_vertex3 == 0 && p1_tri2_vertex1 != 0)
	{
		if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_2)&&is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_3))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_2) && !is_pointTri_within_triangle(tri1, tri2->Vertex_3))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_3) && !is_pointTri_within_triangle(tri1, tri2->Vertex_2))
		{
			return NONINTERSECT;
		}
		else if (line_triangle_intersert_inSamePlane(tri1, tri2->Vertex_2, tri2->Vertex_3))
		{
			return INTERSECT;
		}
	}
	

	////һ�������������ڵ�ƽ����
	if (p1_tri2_vertex1 == 0 && p1_tri2_vertex2 * p1_tri2_vertex3 != 0)
	{
		//��������ֻ��һ�����ų�
		if (is_pointTri_within_triangle(tri1, tri2->Vertex_1))
		{
			if (is_pointTri_within_triangle_vectex(tri1,tri2->Vertex_1))
			{
				return NONINTERSECT;
			}
			return INTERSECT; /////////////////////////////
		}
		else
		{
			return NONINTERSECT;
		}
	}
	else if (p1_tri2_vertex2 == 0 && p1_tri2_vertex1 * p1_tri2_vertex3 != 0)
	{
		//��������ֻ��һ�����ų�
		if (is_pointTri_within_triangle(tri1, tri2->Vertex_2))
		{
			if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_2))
			{
				return NONINTERSECT;
			}
			return INTERSECT;//////////////////////////
		}
		else
		{
			return NONINTERSECT;
		}
	}
	else if (p1_tri2_vertex3 == 0 && p1_tri2_vertex1 * p1_tri2_vertex2 != 0)
	{
		//��������ֻ��һ�����ų�

		if (is_pointTri_within_triangle(tri1, tri2->Vertex_3))
		{
			if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_3))
			{
				return NONINTERSECT;
			}
			return INTERSECT; ///////////////////////////
		}
		else
		{
			return NONINTERSECT;
		}
	}

	float p2_tri1_vertex1 = get_vector4_det(tri2->Vertex_1, tri2->Vertex_2, tri2->Vertex_3, tri1->Vertex_1);

	float p2_tri1_vertex2 = get_vector4_det(tri2->Vertex_1, tri2->Vertex_2, tri2->Vertex_3, tri1->Vertex_2);

	float p2_tri1_vertex3 = get_vector4_det(tri2->Vertex_1, tri2->Vertex_2, tri2->Vertex_3, tri1->Vertex_3);

	//qDebug() << p2_tri1_vertex1 << "   " << p2_tri1_vertex2 << "   " << p2_tri1_vertex3 <<"------"<< "\n";

	if (p2_tri1_vertex1 > 0 && p2_tri1_vertex2 > 0 && p2_tri1_vertex3 > 0)
	{
		return NONINTERSECT;
	}

	if (p2_tri1_vertex1 < 0 && p2_tri1_vertex2 < 0 && p2_tri1_vertex3 < 0)
	{
		return NONINTERSECT;
	}
	//���������������ڵ�ƽ����
	if (p2_tri1_vertex1 == 0 && p2_tri1_vertex2 == 0 && p2_tri1_vertex3 != 0)
	{
		if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_1)&&is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_2))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_1) && !is_pointTri_within_triangle(tri2, tri1->Vertex_2))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_2) && !is_pointTri_within_triangle(tri2, tri1->Vertex_1))
		{
			return NONINTERSECT;
		}
		else if (line_triangle_intersert_inSamePlane(tri2, tri1->Vertex_1, tri1->Vertex_2))
		{
			return INTERSECT;
		}
	}
	else if (p2_tri1_vertex1 == 0 && p2_tri1_vertex3 == 0 && p2_tri1_vertex2 != 0)
	{
		if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_1)&& is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_3))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_1) && !is_pointTri_within_triangle(tri2, tri1->Vertex_3))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_3) && !is_pointTri_within_triangle(tri2, tri1->Vertex_1))
		{
			return NONINTERSECT;
		}
		else if (line_triangle_intersert_inSamePlane(tri2, tri1->Vertex_1, tri1->Vertex_3))
		{
			return INTERSECT;
		}
	}
	else if (p2_tri1_vertex3 == 0 && p2_tri1_vertex2 == 0 && p2_tri1_vertex1 != 0)
	{
		if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_3)&& is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_2))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_3) && !is_pointTri_within_triangle(tri2, tri1->Vertex_2))
		{
			return NONINTERSECT;
		}
		else if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_2) && !is_pointTri_within_triangle(tri2, tri1->Vertex_3))
		{
			return NONINTERSECT;
		}
		else if (line_triangle_intersert_inSamePlane(tri2, tri1->Vertex_3, tri1->Vertex_2))
		{
			return INTERSECT;
		}
	}

	if (p2_tri1_vertex1 == 0 && p2_tri1_vertex2 * p2_tri1_vertex3 != 0)
	{
		//��������ֻ��һ�����ų�
		if (is_pointTri_within_triangle(tri2, tri1->Vertex_1))
		{
			if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_1))
			{
				return NONINTERSECT;
			}
			return INTERSECT; ////////////////////////////
		}
		else
		{
			return NONINTERSECT;
		}
	}

	if (p2_tri1_vertex2 == 0 && p2_tri1_vertex1 * p2_tri1_vertex3 != 0)
	{
		//��������ֻ��һ�����ų�
		if (is_pointTri_within_triangle(tri2, tri1->Vertex_2))
		{
			if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_2))
			{
				return NONINTERSECT;
			}
			return INTERSECT;
		}
		else
		{
			return NONINTERSECT;
		}
	}

	if (p2_tri1_vertex3 == 0 && p2_tri1_vertex1 * p2_tri1_vertex2 != 0)
	{
		//��������ֻ��һ�����ų�
		if (is_pointTri_within_triangle(tri2, tri1->Vertex_3))
		{
			if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_3))
			{
				return NONINTERSECT;
			}
			return INTERSECT;
		}
		else
		{
			return NONINTERSECT;
		}
	}

	//���һ��������־�����
	if (is_TriangleIntersect_within_NoSamePlane(tri1,tri2))
	{
		return INTERSECT;
	}
	else
	{
		return NONINTERSECT;
	}




	//float *tri1_a = tri1->Vertex_1, *tri1_b = tri1->Vertex_2, *tri1_c = tri1->Vertex_3
	//	, *tri2_a = tri2->Vertex_1, *tri2_b = tri2->Vertex_2, *tri2_c = tri2->Vertex_3;

	//float* m;

	//float im;

	//if (p2_tri1_vertex2 * p2_tri1_vertex3 >= 0 && p2_tri1_vertex1 != 0)
	//{
	//	if (p2_tri1_vertex1 < 0)
	//	{
	//		m = tri2_b;
	//		tri2_b = tri2_c;
	//		tri2_c = m;

	//		im = p1_tri2_vertex2;
	//		p1_tri2_vertex2 = p1_tri2_vertex3;
	//		p1_tri2_vertex3 = im;
	//	}
	//}
	//else if (p2_tri1_vertex1 * p2_tri1_vertex3 >= 0 && p2_tri1_vertex2 != 0)
	//{
	//	m = tri1_a;
	//	tri1_a = tri1_b;
	//	tri1_b = tri1_c;
	//	tri1_c = m;

	//	if (p2_tri1_vertex2 < 0)
	//	{
	//		m = tri2_b;
	//		tri2_b = tri2_c;
	//		tri2_c = m;

	//		im = p1_tri2_vertex2;
	//		p1_tri2_vertex2 = p1_tri2_vertex3;
	//		p1_tri2_vertex3 = im;
	//	}
	//}
	//else if (p2_tri1_vertex1 * p2_tri1_vertex2 >= 0 && p2_tri1_vertex3 != 0)
	//{
	//	m = tri1_a;
	//	tri1_a = tri1_c;
	//	tri1_c = tri1_b;
	//	tri1_b = m;

	//	if (p2_tri1_vertex3 < 0)
	//	{
	//		m = tri2_b;
	//		tri2_b = tri2_c;
	//		tri2_c = m;

	//		im = p1_tri2_vertex2;
	//		p1_tri2_vertex2 = p1_tri2_vertex3;
	//		p1_tri2_vertex3 = im;
	//	}
	//}

	//if (p1_tri2_vertex2 * p1_tri2_vertex3 >= 0 && p1_tri2_vertex1 != 0)
	//{
	//	if (p1_tri2_vertex1 < 0)
	//	{
	//		m = tri1_b;
	//		tri1_b = tri1_c;
	//		tri1_c = m;
	//	}
	//}
	//else if (p1_tri2_vertex1 * p1_tri2_vertex3 >= 0 && p1_tri2_vertex2 != 0)
	//{
	//	m = tri2_a;
	//	tri2_a = tri2_b;
	//	tri2_b = tri2_c;
	//	tri2_c = m;

	//	if (p1_tri2_vertex2 < 0)
	//	{
	//		m = tri1_b;
	//		tri1_b = tri1_c;
	//		tri1_c = m;
	//	}
	//}
	//else if (p1_tri2_vertex1 * p1_tri2_vertex2 >= 0 && p1_tri2_vertex3 != 0)
	//{
	//	m = tri2_a;
	//	tri2_a = tri2_c;
	//	tri2_c = tri2_b;
	//	tri2_b = m;

	//	if (p1_tri2_vertex3 < 0)
	//	{
	//		m = tri1_b;
	//		tri1_b = tri1_c;
	//		tri1_c = m;
	//	}
	//}
	////..������
	//if (get_vector4_det(tri1_a, tri1_b, tri2_a, tri2_b) <= 0 && get_vector4_det(tri1_a, tri1_c, tri2_c, tri2_a) <= 0)
	//{
	//	return INTERSECT;
	//}
	//else
	//{
	//	return NONINTERSECT;
	//}

}

//�ж������������Ƿ���ͬһ��������

#endif //TRIANGLE_HAN
