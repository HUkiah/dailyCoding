#ifndef TRIANGLE_HAN
#define TRIANGLE_HAN
#include "Vec.h"
#include "globalFunctions.h"

#include <QDebug>

typedef trimesh::vec3  Vec3f;

enum TopologicalStructure
{
	INTERSECT, NONINTERSECT
};

class Triangle
{
public:
	Vec3f Vertex_1;
	Vec3f Vertex_2;
	Vec3f Vertex_3;
	Vec3f centerpoint;//����������
	float radius;//����������Բ�뾶
	int selected = 0;//���������Ƭ�Ƿ��ཻ�ı�־λ
};

struct pointTri
{
	float x = 0, y = 0;
};

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

inline float twoPointDistance(Vec3f a, Vec3f b) {
	return sqrt(pow((a[0] - b[0]), 2) + pow((a[1] - b[1]), 2) + pow((a[2] - b[2]), 2));
}

//�ж������Ƿ����
inline bool is_equal_vertex(Vec3f pointTri1, Vec3f pointTri2) {
	if (pointTri1 == pointTri2)
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

//�����ڻ�  
inline float Dot(const Vec3f& v1, const Vec3f& v2)
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

//���ķ��жϵ��Ƿ����������ڲ�  
inline bool is_pointTri_within_triangle(Triangle* tri, Vec3f pointTri)
{
	Vec3f v0;
	get_vector_diff(v0, tri->Vertex_1, tri->Vertex_3);
	Vec3f v1;
	get_vector_diff(v1, tri->Vertex_1, tri->Vertex_2);
	Vec3f v2;
	get_vector_diff(v2, tri->Vertex_1, pointTri);
	float dot00 = Dot(v0, v0);
	float dot01 = Dot(v0, v1);
	float dot02 = Dot(v0, v2);
	float dot11 = Dot(v1, v1);
	float dot12 = Dot(v1, v2);
	float inverDeno = 1 / (dot00* dot11 - dot01* dot01);
	float u = (dot11* dot02 - dot01* dot12) * inverDeno;
	if (u < 0 || u > 1) // if u out of range, return directly  �����"="��ȥ���ڶ�������
	{
		return false;
	}
	float v = (dot00* dot12 - dot01* dot02) * inverDeno;
	if (v < 0 || v > 1) // if v out of range, return directly  
	{
		return false;
	}
	return u + v <= 1;
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

//�жϵ��Ƿ�������ε�һ�߹���
inline bool is_pointTri_within_triangle_inSameEdgeLine(Triangle *tri, Vec3f f1, Vec3f f2) {

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
	//���жϵ��������ε��ڲ��������������εı���

	//if �߶����������ϵ�һ���߹���
	if (is_pointTri_within_triangle_inSameEdgeLine(tri, f1, f2))
	{
		if (need_triangle_rule_out(tri, f1, f2))
		{
			return false;//NONINTERSECT
		}

	}



	//���ж϶�Ӧ���������ϵĵ��ڲ��������������

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

//Devillers�㷨������  
inline TopologicalStructure judge_triangle_topologicalStructure(Triangle* tri1, Triangle* tri2)
{
	//�ų�����Ƚ�
	if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_1) && is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_2) && is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_3))
	{
		return NONINTERSECT;
	}

	////��tri1���ڵ�ƽ��Ϊp1,tri2���ڵ�ƽ��Ϊp2
	float p1_tri2_vertex1 = get_vector4_det(tri1->Vertex_1, tri1->Vertex_2, tri1->Vertex_3, tri2->Vertex_1);

	float p1_tri2_vertex2 = get_vector4_det(tri1->Vertex_1, tri1->Vertex_2, tri1->Vertex_3, tri2->Vertex_2);

	float p1_tri2_vertex3 = get_vector4_det(tri1->Vertex_1, tri1->Vertex_2, tri1->Vertex_3, tri2->Vertex_3);

	////qDebug() << p1_tri2_vertex1 << "   "<<p1_tri2_vertex2<<"   "<< p1_tri2_vertex3<<"\n";

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
			return NONINTERSECT;/////////////////////////
		}
		else
		{
			return NONINTERSECT;
		}
	}

	//���������棬����
	if (p1_tri2_vertex1 == 0 && p1_tri2_vertex2 == 0)
	{


		//if �߶����������ϵ�һ���߹���
		if (is_pointTri_within_triangle_inSameEdgeLine(tri1, tri2->Vertex_1, tri2->Vertex_2))
		{
			intersectNum++;
			if (need_triangle_rule_out(tri1, tri2->Vertex_1, tri2->Vertex_2))
			{
				return NONINTERSECT;
			}

		}

	}
	//���������棬����
	if (p1_tri2_vertex1 == 0 && p1_tri2_vertex3 == 0)
	{
		//if �߶����������ϵ�һ���߹���
		if (is_pointTri_within_triangle_inSameEdgeLine(tri1, tri2->Vertex_1, tri2->Vertex_3))
		{
			intersectNum++;
			if (need_triangle_rule_out(tri1, tri2->Vertex_1, tri2->Vertex_3))
			{
				return NONINTERSECT;
			}
		}
	}
	//���������棬����
	if (p1_tri2_vertex2 == 0 && p1_tri2_vertex3 == 0)
	{
		//if �߶����������ϵ�һ���߹���
		if (is_pointTri_within_triangle_inSameEdgeLine(tri1, tri2->Vertex_2, tri2->Vertex_3))
		{
			intersectNum++;
			if (need_triangle_rule_out(tri1, tri2->Vertex_2, tri2->Vertex_3))
			{
				return NONINTERSECT;
			}
		}
	}

	if (p1_tri2_vertex1 == 0 && p1_tri2_vertex2 * p1_tri2_vertex3 > 0)
	{
		if (is_pointTri_within_triangle(tri1, tri2->Vertex_1))
		{
			if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_1))
			{
				return NONINTERSECT;//�ų�һ�������ϵ���������һ�����غ�
			}
			else
			{
				return NONINTERSECT; /////////////////////////////
			}

		}
		else
		{
			return NONINTERSECT;
		}
	}
	else if (p1_tri2_vertex2 == 0 && p1_tri2_vertex1 * p1_tri2_vertex3 > 0)
	{
		if (is_pointTri_within_triangle(tri1, tri2->Vertex_2))
		{
			if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_2))
			{
				return NONINTERSECT;//һ�������ϵ���������һ�����غ�
			}
			else
			{
				return NONINTERSECT;//////////////////////////
			}
		}
		else
		{
			return NONINTERSECT;
		}
	}
	else if (p1_tri2_vertex3 == 0 && p1_tri2_vertex1 * p1_tri2_vertex2 > 0)
	{
		if (is_pointTri_within_triangle(tri1, tri2->Vertex_3))
		{
			if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_3))
			{
				return NONINTERSECT;//һ�������ϵ���������һ�����غ�
			}
			else
			{
				return NONINTERSECT; ///////////////////////////
			}
		}
		else
		{
			return NONINTERSECT;
		}
	}

	float p2_tri1_vertex1 = get_vector4_det(tri2->Vertex_1, tri2->Vertex_2, tri2->Vertex_3, tri1->Vertex_1);

	float p2_tri1_vertex2 = get_vector4_det(tri2->Vertex_1, tri2->Vertex_2, tri2->Vertex_3, tri1->Vertex_2);

	float p2_tri1_vertex3 = get_vector4_det(tri2->Vertex_1, tri2->Vertex_2, tri2->Vertex_3, tri1->Vertex_3);


	if (p2_tri1_vertex1 > 0 && p2_tri1_vertex2 > 0 && p2_tri1_vertex3 > 0)
	{
		return NONINTERSECT;
	}

	if (p2_tri1_vertex1 < 0 && p2_tri1_vertex2 < 0 && p2_tri1_vertex3 < 0)
	{
		return NONINTERSECT;
	}


	//���������棬����
	if (p2_tri1_vertex1 == 0 && p2_tri1_vertex2 == 0)
	{

		//if �߶����������ϵ�һ���߹���
		if (is_pointTri_within_triangle_inSameEdgeLine(tri2, tri1->Vertex_1, tri1->Vertex_2))
		{
			if (need_triangle_rule_out(tri2, tri1->Vertex_1, tri1->Vertex_2))
			{
				return NONINTERSECT;
			}
		}

	}
	//���������棬����
	if (p2_tri1_vertex1 == 0 && p2_tri1_vertex3 == 0)
	{

		//if �߶����������ϵ�һ���߹���
		if (is_pointTri_within_triangle_inSameEdgeLine(tri2, tri1->Vertex_1, tri1->Vertex_3))
		{
			if (need_triangle_rule_out(tri2, tri1->Vertex_1, tri1->Vertex_3))
			{
				return NONINTERSECT;
			}
		}
	}
	//���������棬����
	if (p2_tri1_vertex2 == 0 && p2_tri1_vertex3 == 0)
	{
		//if (!is_pointTri_within_triangle(tri2, tri1->Vertex_2) && !is_pointTri_within_triangle(tri2, tri1->Vertex_3))
		//{
		//	return NONINTERSECT;
		//}

		//if �߶����������ϵ�һ���߹���
		if (is_pointTri_within_triangle_inSameEdgeLine(tri2, tri1->Vertex_2, tri1->Vertex_3))
		{
			if (need_triangle_rule_out(tri2, tri1->Vertex_2, tri1->Vertex_3))
			{
				return NONINTERSECT;
			}

		}
	}

	if (p2_tri1_vertex1 == 0 && p2_tri1_vertex2 * p2_tri1_vertex3 > 0)
	{
		if (is_pointTri_within_triangle(tri2, tri1->Vertex_1))
		{
			if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_1))
			{
				return NONINTERSECT;//һ�������ϵ���������һ�����غ�
			}
			else
			{

				return NONINTERSECT; ////////////////////////////
			}
		}
		else
		{
			return NONINTERSECT;
		}
	}

	if (p2_tri1_vertex2 == 0 && p2_tri1_vertex1 * p2_tri1_vertex3 > 0)
	{
		if (is_pointTri_within_triangle(tri2, tri1->Vertex_2))
		{
			if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_2))
			{
				return NONINTERSECT;//һ�������ϵ���������һ�����غ�
			}
			else
			{

				//return INTERSECT;
			}
		}
		else
		{
			return NONINTERSECT;
		}
	}

	if (p2_tri1_vertex3 == 0 && p2_tri1_vertex1 * p2_tri1_vertex2 > 0)
	{
		if (is_pointTri_within_triangle(tri2, tri1->Vertex_3))
		{
			if (is_pointTri_within_triangle_vectex(tri2, tri1->Vertex_3))
			{
				return NONINTERSECT;//һ�������ϵ���������һ�����غ�
			}
			else
			{

				//return INTERSECT;
			}
		}
		else
		{
			return NONINTERSECT;
		}
	}



	float *tri1_a = tri1->Vertex_1, *tri1_b = tri1->Vertex_2, *tri1_c = tri1->Vertex_3
		, *tri2_a = tri2->Vertex_1, *tri2_b = tri2->Vertex_2, *tri2_c = tri2->Vertex_3;

	float* m;

	float im;

	if (p2_tri1_vertex2 * p2_tri1_vertex3 >= 0 && p2_tri1_vertex1 != 0)
	{
		if (p2_tri1_vertex1 < 0)
		{
			m = tri2_b;
			tri2_b = tri2_c;
			tri2_c = m;

			im = p1_tri2_vertex2;
			p1_tri2_vertex2 = p1_tri2_vertex3;
			p1_tri2_vertex3 = im;
		}
	}
	else if (p2_tri1_vertex1 * p2_tri1_vertex3 >= 0 && p2_tri1_vertex2 != 0)
	{
		m = tri1_a;
		tri1_a = tri1_b;
		tri1_b = tri1_c;
		tri1_c = m;

		if (p2_tri1_vertex2 < 0)
		{
			m = tri2_b;
			tri2_b = tri2_c;
			tri2_c = m;

			im = p1_tri2_vertex2;
			p1_tri2_vertex2 = p1_tri2_vertex3;
			p1_tri2_vertex3 = im;
		}
	}
	else if (p2_tri1_vertex1 * p2_tri1_vertex2 >= 0 && p2_tri1_vertex3 != 0)
	{
		m = tri1_a;
		tri1_a = tri1_c;
		tri1_c = tri1_b;
		tri1_b = m;

		if (p2_tri1_vertex3 < 0)
		{
			m = tri2_b;
			tri2_b = tri2_c;
			tri2_c = m;

			im = p1_tri2_vertex2;
			p1_tri2_vertex2 = p1_tri2_vertex3;
			p1_tri2_vertex3 = im;
		}
	}

	if (p1_tri2_vertex2 * p1_tri2_vertex3 >= 0 && p1_tri2_vertex1 != 0)
	{
		if (p1_tri2_vertex1 < 0)
		{
			m = tri1_b;
			tri1_b = tri1_c;
			tri1_c = m;
		}
	}
	else if (p1_tri2_vertex1 * p1_tri2_vertex3 >= 0 && p1_tri2_vertex2 != 0)
	{
		m = tri2_a;
		tri2_a = tri2_b;
		tri2_b = tri2_c;
		tri2_c = m;

		if (p1_tri2_vertex2 < 0)
		{
			m = tri1_b;
			tri1_b = tri1_c;
			tri1_c = m;
		}
	}
	else if (p1_tri2_vertex1 * p1_tri2_vertex2 >= 0 && p1_tri2_vertex3 != 0)
	{
		m = tri2_a;
		tri2_a = tri2_c;
		tri2_c = tri2_b;
		tri2_b = m;

		if (p1_tri2_vertex3 < 0)
		{
			m = tri1_b;
			tri1_b = tri1_c;
			tri1_c = m;
		}
	}
	//..������
	if (get_vector4_det(tri1_a, tri1_b, tri2_a, tri2_b) <= 0 && get_vector4_det(tri1_a, tri1_c, tri2_c, tri2_a) <= 0)
	{
		return INTERSECT;
	}

	return NONINTERSECT;//Ĭ�Ϸ���ֵ
}

//�ж������������Ƿ���ͬһ��������

#endif //TRIANGLE_HAN
