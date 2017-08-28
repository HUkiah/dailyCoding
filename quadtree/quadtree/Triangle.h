#ifndef TRIANGLE_HAN
#define TRIANGLE_HAN

typedef float float3[3];

enum TopologicalStructure
{
	INTERSECT,NONINTERSECT
};

class Triangle
{
public:
	float3 Vertex_1, Vertex_2, Vertex_3;
	float3 centerpoint;
	float radius;
	int selected = 0;
};

struct pointTri
{
	float x = 0, y = 0;
};

inline void copy_pointXY(pointTri &a, float3 b) {
	a.x = b[0];
	a.y = b[1];
}

inline void copy_pointXZ(pointTri &a, float3 b) {
	a.x = b[0];
	a.y = b[2];
}

inline void copy_pointYZ(pointTri &a, float3 b) {
	a.x = b[1];
	a.y = b[2];
}


//判断两点是否相等
inline bool is_equal_vertex(float3 pointTri1, float3 pointTri2) {
	if (pointTri1[0] == pointTri2[0] && pointTri1[1] == pointTri2[1] && pointTri1[2] == pointTri2[2])
	//if (pointTri1[0] - pointTri2[0]<=0.00001 && pointTri1[1] - pointTri2[1]<=0.00001 && pointTri1[2] - pointTri2[2]<=0.00001)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

//inline bool is_equal_vertex(pointTri pointTri1, pointTri pointTri2) {
//	if (pointTri1.x == pointTri2.x && pointTri1.y == pointTri2.y)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//}

//四点行列式  
inline float get_vector4_det(float3 v1, float3 v2, float3 v3, float3 v4)
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

//利用叉积计算点p相对线段p1p2的方位  
inline double direction(pointTri p1, pointTri p2, pointTri p) {
	return  (p2.x - p1.x) * (p.y - p1.y)-(p.x - p1.x) * (p2.y - p1.y);
}

//确定与线段p1p2共线的点p是否在线段p1p2上  
inline int on_segment(pointTri p1, pointTri p2, pointTri p) {
	double max = p1.x > p2.x ? p1.x : p2.x;
	double min = p1.x < p2.x ? p1.x : p2.x;
	double max1 = p1.y > p2.y ? p1.y : p2.y;
	double min1 = p1.y < p2.y ? p1.y : p2.y;

	if (p.x >=min && p.x <= max && p.y >= min1 && p.y <= max1)//修改 去除“=”,除去共线点，与顶点相同
	{
		//在二维空间中去控制 边，点的关系，是糟糕的想法（就是上面注释的想法）
		return 1;
	}
	else
	{
		return 0;
	}
}

//求出了三角面的外接圆

//inline void get_center_point_of_circle(Triangle* tri)
//{
//	float3 centerpoint;
//	
//	double a1, b1, c1, d1;
//	double a2, b2, c2, d2;
//	double a3, b3, c3, d3;
//
//	double x1 = tri->Vertex_1[0], y1 = tri->Vertex_1[1], z1 = tri->Vertex_1[2];
//	double x2 = tri->Vertex_2[0], y2 = tri->Vertex_2[1], z2 = tri->Vertex_2[2];
//	double x3 = tri->Vertex_3[0], y3 = tri->Vertex_3[1], z3 = tri->Vertex_3[2];
//
//	a1 = (y1*z2 - y2*z1 - y1*z3 + y3*z1 + y2*z3 - y3*z2);
//	b1 = -(x1*z2 - x2*z1 - x1*z3 + x3*z1 + x2*z3 - x3*z2);
//	c1 = (x1*y2 - x2*y1 - x1*y3 + x3*y1 + x2*y3 - x3*y2);
//	d1 = -(x1*y2*z3 - x1*y3*z2 - x2*y1*z3 + x2*y3*z1 + x3*y1*z2 - x3*y2*z1);
//
//	a2 = 2 * (x2 - x1);
//	b2 = 2 * (y2 - y1);
//	c2 = 2 * (z2 - z1);
//	d2 = x1 * x1 + y1 * y1 + z1 * z1 - x2 * x2 - y2 * y2 - z2 * z2;
//
//	a3 = 2 * (x3 - x1);
//	b3 = 2 * (y3 - y1);
//	c3 = 2 * (z3 - z1);
//	d3 = x1 * x1 + y1 * y1 + z1 * z1 - x3 * x3 - y3 * y3 - z3 * z3;
//
//	centerpoint[0] = -(b1*c2*d3 - b1*c3*d2 - b2*c1*d3 + b2*c3*d1 + b3*c1*d2 - b3*c2*d1)
//		/ (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
//	centerpoint[1] = (a1*c2*d3 - a1*c3*d2 - a2*c1*d3 + a2*c3*d1 + a3*c1*d2 - a3*c2*d1)
//		/ (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
//	centerpoint[2] = -(a1*b2*d3 - a1*b3*d2 - a2*b1*d3 + a2*b3*d1 + a3*b1*d2 - a3*b2*d1)
//		/ (a1*b2*c3 - a1*b3*c2 - a2*b1*c3 + a2*b3*c1 + a3*b1*c2 - a3*b2*c1);
//
//	tri->radius = sqrt(pow((x1 - centerpoint[0]), 2) + pow((x1 - centerpoint[1]), 2) + pow((x1 - centerpoint[2]), 2));
//}

inline void get_central_point(float3 centralPoint, Triangle* tri)
{
	centralPoint[0] = (tri->Vertex_1[0] + tri->Vertex_2[0] + tri->Vertex_3[0]) / 3;

	centralPoint[1] = (tri->Vertex_1[1] + tri->Vertex_2[1] + tri->Vertex_3[1]) / 3;

	centralPoint[2] = (tri->Vertex_1[2] + tri->Vertex_2[2] + tri->Vertex_3[2]) / 3;
}

//向量之差  
inline void get_vector_diff(float3& aimV, const float3 a, const float3 b)
{
	aimV[0] = b[0] - a[0];

	aimV[1] = b[1] - a[1];

	aimV[2] = b[2] - a[2];
}

//向量内积  
inline float Dot(const float3& v1, const float3& v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

//判断线段p1p2与线段p3p4是否相交的主函数  
inline int segments_intersert(pointTri p1, pointTri p2, pointTri p3, pointTri p4) {
	double d1, d2, d3, d4;
	d1 = direction(p3, p4, p1);
	d2 = direction(p3, p4, p2);
	d3 = direction(p1, p2, p3);
	d4 = direction(p1, p2, p4);
	if (d1 * d2 < 0 && d3 * d4 < 0)
	{
		return 1;
	}//共线但是点与顶点重合，判定不相交(共面且顶点重合的两个面，判定为不相交)
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

//重心法判断点是否在三角形内部  
inline bool is_pointTri_within_triangle(Triangle* tri, float3 pointTri)
{
	float3 v0;
	get_vector_diff(v0, tri->Vertex_1, tri->Vertex_3);
	float3 v1;
	get_vector_diff(v1, tri->Vertex_1, tri->Vertex_2);
	float3 v2;
	get_vector_diff(v2, tri->Vertex_1, pointTri);
	float dot00 = Dot(v0, v0);
	float dot01 = Dot(v0, v1);
	float dot02 = Dot(v0, v2);
	float dot11 = Dot(v1, v1);
	float dot12 = Dot(v1, v2);
	float inverDeno = 1 / (dot00* dot11 - dot01* dot01);
	float u = (dot11* dot02 - dot01* dot12) * inverDeno;
	if (u < 0 || u > 1) // if u out of range, return directly  添加了"="，去除在顶点的情况
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

//判断点是否是三角形的一个顶点
inline bool is_pointTri_within_triangle_vectex(Triangle *tri, float3 pointTri) {
	if (is_equal_vertex(tri->Vertex_1, pointTri) || is_equal_vertex(tri->Vertex_2, pointTri) || is_equal_vertex(tri->Vertex_3, pointTri))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//判断同一平面的直线和三角形是否相交  
inline bool line_triangle_intersert_inSamePlane(Triangle* tri, float3 f1, float3 f2)
{
	//a:XY面 b:XZ面 c:YZ面
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

	if (segments_intersert(pa1, pa2, pa3, pa4)&& segments_intersert(pb1, pb2, pb3, pb4)&& segments_intersert(pc1, pc2, pc3, pc4))
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

//判断同一平面内的三角形是否相交  
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
		//先不看重叠面
		//float3 centralPoint1, centralPoint2;

		//get_central_point(centralPoint1, tri1);

		//get_central_point(centralPoint2, tri2);

		//if (is_pointTri_within_triangle(tri2, centralPoint1) || is_pointTri_within_triangle(tri1, centralPoint2))
		//{
		//	return true;
		//}

		return false;
	}
}

//Devillers算法主函数  
inline TopologicalStructure judge_triangle_topologicalStructure(Triangle* tri1, Triangle* tri2)
{
	//排除自身比较
	if (is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_1) && is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_2) && is_pointTri_within_triangle_vectex(tri1, tri2->Vertex_3))
	{
		return NONINTERSECT;
	}

	//设tri1所在的平面为p1,tri2所在的平面为p2  
	float p1_tri2_vertex1 = get_vector4_det(tri1->Vertex_1, tri1->Vertex_2, tri1->Vertex_3, tri2->Vertex_1);

	float p1_tri2_vertex2 = get_vector4_det(tri1->Vertex_1, tri1->Vertex_2, tri1->Vertex_3, tri2->Vertex_2);

	float p1_tri2_vertex3 = get_vector4_det(tri1->Vertex_1, tri1->Vertex_2, tri1->Vertex_3, tri2->Vertex_3);


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
			return INTERSECT;
		}
		else
		{
			return NONINTERSECT;
		}
	}


	if (p1_tri2_vertex1 == 0 && p1_tri2_vertex2 * p1_tri2_vertex3 > 0)
	{
		if (is_pointTri_within_triangle(tri1, tri2->Vertex_1))
		{
			return INTERSECT;
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
			return INTERSECT;
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
			return INTERSECT;
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


	if (p2_tri1_vertex1 == 0 && p2_tri1_vertex2 * p2_tri1_vertex3 > 0)
	{
		if (is_pointTri_within_triangle(tri2, tri1->Vertex_1))
		{
			return INTERSECT;
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
			return INTERSECT;
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
			return INTERSECT;
		}
		else
		{
			return NONINTERSECT;
		}
	}



	float* tri1_a = tri1->Vertex_1, *tri1_b = tri1->Vertex_2, *tri1_c = tri1->Vertex_3
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

	if (get_vector4_det(tri1_a, tri1_b, tri2_a, tri2_b) <= 0 && get_vector4_det(tri1_a, tri1_c, tri2_c, tri2_a) <= 0)
	{
		return INTERSECT;
	}
	else
	{
		return NONINTERSECT;
	}
}


//判断两个三角形是否是同一个三角形

#endif //TRIANGLE_HAN
