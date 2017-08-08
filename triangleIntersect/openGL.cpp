#include<GLUT.H>    
#include <windows.h>        
#include <math.h>        
#include <gl/GL.h>
#include"triangle.h"


typedef float float3[3];

void copy_Vertex(float3 _Vertex, vec3f _ver);

enum TopologicalStructure
{
	INTERSECT, NONINTERSECT
};

struct Triangle
{
	//float3 Normal_0;
	float3 Vertex_1, Vertex_2, Vertex_3;

	inline Triangle operator =(const face &temp)
	{
		copy_Vertex(Vertex_1, temp.verts[0]);
		copy_Vertex(Vertex_2, temp.verts[1]);
		copy_Vertex(Vertex_3, temp.verts[2]);
		return *this;
	}
};
vector<Triangle> Tria;

struct point
{
	float x=0, y=0;
};
void copy_Vertex(float3 _Vertex, vec3f _ver) {
	_Vertex[0] = _ver.x;
	_Vertex[1] = _ver.y;
	_Vertex[2] = _ver.z;
}
void copy_point(point a, float3 b) {
	a.x = b[0];
	a.y = b[1];
}

float twoPointDistance(vec3f a, vec3f b) {
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
}

float minEdge(face temp) {

	float a, b, c;

	a = sqrt(pow((temp.verts[0].x - temp.verts[1].x), 2) + pow((temp.verts[0].y - temp.verts[1].y), 2) + pow((temp.verts[0].z - temp.verts[1].z), 2));
	b = sqrt(pow((temp.verts[0].x - temp.verts[2].x), 2) + pow((temp.verts[0].y - temp.verts[2].y), 2) + pow((temp.verts[0].z - temp.verts[2].z), 2));
	c = sqrt(pow((temp.verts[1].x - temp.verts[2].x), 2) + pow((temp.verts[1].y - temp.verts[2].y), 2) + pow((temp.verts[1].z - temp.verts[2].z), 2));

	if (a < b)
	{
		if (a < c)return a;
		else if (b < c)
		{
			return b;
		}
		else
			return c;
	}
	else if (b < c)
	{
		return b;
	}
	else
		return c;

}



//�ĵ�����ʽ  
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


//���ò�������p����߶�p1p2�ķ�λ  
inline double direction(point p1, point p2, point p) {
	return (p.x - p1.x) * (p2.y - p1.y) - (p2.x - p1.x) * (p.y - p1.y);
}


//ȷ�����߶�p1p2���ߵĵ�p�Ƿ����߶�p1p2��  
inline int on_segment(point p1, point p2, point p) {
	double max = p1.x > p2.x ? p1.x : p2.x;
	double min = p1.x < p2.x ? p1.x : p2.x;
	double max1 = p1.y > p2.y ? p1.y : p2.y;
	double min1 = p1.y < p2.y ? p1.y : p2.y;
	if (p.x >= min && p.x <= max && p.y >= min1 && p.y <= max1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


//�ж��߶�p1p2���߶�p3p4�Ƿ��ཻ��������  
inline int segments_intersert(point p1, point p2, point p3, point p4) {
	double d1, d2, d3, d4;
	d1 = direction(p3, p4, p1);
	d2 = direction(p3, p4, p2);
	d3 = direction(p1, p2, p3);
	d4 = direction(p1, p2, p4);
	if (d1 * d2 < 0 && d3 * d4 < 0)
	{
		return 1;
	}
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


//�ж�ͬһƽ���ֱ�ߺ��������Ƿ��ཻ  
inline bool line_triangle_intersert_inSamePlane(Triangle* tri, float3 f1, float3 f2)
{
	point p1, p2, p3, p4;

	copy_point(p1, f1);

	copy_point(p2, f2);

	copy_point(p3, tri->Vertex_1);

	copy_point(p4, tri->Vertex_2);

	if (segments_intersert(p1, p2, p3, p4))
	{
		return true;
	}

	copy_point(p3, tri->Vertex_2);

	copy_point(p4, tri->Vertex_3);

	if (segments_intersert(p1, p2, p3, p4))
	{
		return true;
	}

	copy_point(p3, tri->Vertex_1);

	copy_point(p4, tri->Vertex_3);

	if (segments_intersert(p1, p2, p3, p4))
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
		return false;
	}
}

//����֮��  
inline void get_vector_diff(float3& aimV, const float3 a, const float3 b)
{
	aimV[0] = b[0] - a[0];

	aimV[1] = b[1] - a[1];

	aimV[2] = b[2] - a[2];
}

//�����ڻ�  
inline float Dot(const float3& v1, const float3& v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

//���ķ��жϵ��Ƿ����������ڲ�  
inline bool is_point_within_triangle(Triangle* tri, float3 point)
{
	float3 v0;
	get_vector_diff(v0, tri->Vertex_1, tri->Vertex_3);
	float3 v1;
	get_vector_diff(v1, tri->Vertex_1, tri->Vertex_2);
	float3 v2;
	get_vector_diff(v2, tri->Vertex_1, point);
	float dot00 = Dot(v0, v0);
	float dot01 = Dot(v0, v1);
	float dot02 = Dot(v0, v2);
	float dot11 = Dot(v1, v1);
	float dot12 = Dot(v1, v2);
	float inverDeno = 1 / (dot00* dot11 - dot01* dot01);
	float u = (dot11* dot02 - dot01* dot12) * inverDeno;
	if (u < 0 || u > 1) // if u out of range, return directly  
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

//Devillers�㷨������  
inline TopologicalStructure judge_triangle_topologicalStructure(Triangle* tri1, Triangle* tri2)
{
	//��tri1���ڵ�ƽ��Ϊp1,tri2���ڵ�ƽ��Ϊp2  
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
		if (is_point_within_triangle(tri1, tri2->Vertex_1))
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
		if (is_point_within_triangle(tri1, tri2->Vertex_2))
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
		if (is_point_within_triangle(tri1, tri2->Vertex_3))
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
		if (is_point_within_triangle(tri2, tri1->Vertex_1))
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
		if (is_point_within_triangle(tri2, tri1->Vertex_2))
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
		if (is_point_within_triangle(tri2, tri1->Vertex_3))
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

//static const GLfloat vertex_list[][3] = {
//	-0.5f, -0.5f, -0.5f,//0
//	0.5f, -0.5f, -0.5f,//1
//	0.5f, 0.5f, -0.5f,//2
//	-0.5f, 0.5f, -0.5f,//3
//	-0.5f, -0.5f, 0.5f,//4
//	0.5f, -0.5f, 0.5f,//5
//
//};
//typedef GLfloat GLfloat3[3];
//static const GLfloat3 vertex_listSolid[]= {
//
//	0.5f, -0.5f, -0.5f,//1
//	0.5f, 0.5f, -0.5f,//2
//	-0.5f, -0.5f, 0.5f,//4
//
//	-0.5f, -0.5f, -0.5f,//0
//	-0.5f, 0.5f, -0.5f,//3
//	0.5f, -0.5f, 0.5f,//5
//
//};
//
//static const GLint line_list[][2] = {
//	1,2,//0
//	1,4,//1
//	2,4,//2
//	0,5,//3
//	0,3,//4
//	3,5,//5
//
//};
//
//static const GLint face_list[][3] = {
//	0,1,2,//0
//	3,4,5//1
//};
//
//template <class T>
//
//int getArrayLen(T& array)
//
//{
//
//	return (sizeof(array) / sizeof(array[0]));
//
//}
//
//void copy_vrtex(float3 a,const float3 b) {
//	a[0] = b[0];
//	a[1] = b[1];
//	a[2] = b[2];
//}
//
//void myDisplay(void) {
//	
//	Triangle a, b;
//	copy_vrtex(a.Vertex_1, vertex_listSolid[0]);
//	copy_vrtex(a.Vertex_2, vertex_listSolid[1]);
//	copy_vrtex(a.Vertex_3, vertex_listSolid[2]);
//
//	copy_vrtex(b.Vertex_1, vertex_listSolid[3]);
//	copy_vrtex(b.Vertex_2, vertex_listSolid[4]);
//	copy_vrtex(b.Vertex_3, vertex_listSolid[5]);
//
//	glClear(GL_COLOR_BUFFER_BIT);
//	glRotatef(45, 1, 1, 1);
//	glFrontFace(GL_CCW);
//
//	if (judge_triangle_topologicalStructure(&a,&b)==0)//�ཻ���������Σ����ཻ����������
//	{
//		int L = getArrayLen(line_list);
//		for (int i = 0; i < L; ++i)      // ��L���ߣ�ѭ��L��  
//		{
//			glBegin(GL_LINES);
//			{glVertex3fv(vertex_list[line_list[i][0]]);//����ÿ�������������
//			glVertex3fv(vertex_list[line_list[i][1]]);
//			}
//			glEnd();
//
//		}
//		glFlush();
//	}
//	else {
//		glBegin(GL_TRIANGLES);
//		{
//			glVertex3f(0.5f, -0.5f, -0.5f);
//			glVertex3f(0.5f, 0.5f, -0.5f);
//			glVertex3f(-0.5f, -0.5f, 0.5f);
//
//			glVertex3f(-0.5f, -0.5f, -0.5f);
//			glVertex3f(-0.5f, 0.5f, -0.5f);
//			glVertex3f(0.5f, -0.5f, 0.5f);
//		}
//		glEnd();
//		glFlush();
//	}
//
//
//
//
//}

#define  YU 100

vector<face> loadStlFromFlie(char * fins) {
	int start = clock();

	face temp;
	string  tmp;
	int n = 0;
	float minValue = 0.0;//edge min value
	vector<vec3f> vec1, vec2;
	vector<face> myvector;

	char * buffer;
	long size;
	float xx, yy, zz;
	ifstream myfile;
	myfile.open(fins, fstream::ate);//Exercise02.stl

	size = (long)myfile.tellg();
	myfile.seekg(0, ios::beg);
	buffer = new char[size];

	myfile.read(buffer, size);
	tmp = buffer;

	myfile.close();

	cout << "read complete " << double(clock() - start) / CLOCKS_PER_SEC << "\n";

	std::string pattern("vertex[\\s]+[+-]?\\d\\.\\d{6}e[+-]\\d{3}[\\s]+[+-]?\\d\\.\\d{6}e[+-]\\d{3}[\\s]+[+-]?\\d\\.\\d{6}e[+-]\\d{3}");
	std::regex r(pattern);

	for (std::sregex_iterator it(tmp.begin(), tmp.end(), r), end; it != end; ++it, ++n) //end��β���������regex_iterator��regex_iterator��string���͵İ汾
	{
		std::string ret = it->str().replace(it->str().find("vertex"), 6, string(""));
		if (n == 3) {
			float m = minEdge(temp);
			if (minValue == 0.0) {
				minValue = m;
			}
			else if (minValue > m) {
				minValue = m;
			}
			n = 0;
		}

		sscanf(ret.c_str(), "%f %f %f", &xx, &yy, &zz);
		temp.verts[n].x = xx;
		temp.verts[n].y = yy;
		temp.verts[n].z = zz;
		vec1.push_back(temp.verts[n]);//�ڶ�ȡʱ,������index�Ĺ���.
	}
	cout << "# ������ = " << vec1.size() << "  ��С��ֵ = " << minValue << "\n";
	cout << "# read file complete " << double(clock() - start) / CLOCKS_PER_SEC << "\n";

	for (int i = 0;i < vec1.size();) {

		for (int k = 0;k < 3;k++, i++)
		{
			temp.verts[k].x = vec1[i].x;
			temp.verts[k].y = vec1[i].y;
			temp.verts[k].z = vec1[i].z;
			int w = 0;
			if (vec2.size() == 0)
			{
				temp.verts[k].index = w + 1;
			}
			else
			{
				for (w = 0;w < vec2.size();w++)//vectex index���ļ��еĵڼ���
				{
					if (temp.verts[k] == vec2[w])
					{
						temp.verts[k].index = w + 1;
						break;
					}
				}
			}

			if (w == vec2.size())//new vectex
			{
				temp.verts[k].index = w + 1;
				vec2.push_back(temp.verts[k]);
			}

		}
		myvector.push_back(temp);

	}
	cout << "face = " << myvector.size() << "\n";
	cout << "face������� " << double(clock() - start) / CLOCKS_PER_SEC << "\n";

	//ofstream myfile2;
	//myfile2.open("han.txt");
	//myfile2 << "# face " << myvector.size() << "\n";
	//for (int j = 0;j < myvector.size();j++)
	//	myfile2 << "f " << myvector[j].verts[0].index << " " << myvector[j].verts[1].index << " " << myvector[j].verts[2].index << "\n";
	//myfile2.close();

	return myvector;
}
void myDisplay() {
	glPointSize(5);
	glColor3f(0.0f, 1.0f, 0.0f);//������ɫ
	{
		
		glBegin(GL_LINES);
		for (int i=0;i<Tria.size();++i)
		{
			glVertex3f(Tria[i].Vertex_1[0] /YU, Tria[i].Vertex_1[1] / YU, Tria[i].Vertex_1[2] / YU);
			glVertex3f(Tria[i].Vertex_2[0] /YU, Tria[i].Vertex_2[1] / YU, Tria[i].Vertex_2[2] / YU);
			glVertex3f(Tria[i].Vertex_3[0] /YU, Tria[i].Vertex_3[1] / YU, Tria[i].Vertex_3[2] / YU);

		}
		glEnd();

	}
	glFlush();
}
int main(int argc, char *argv[]) 
{
	vector<int> interInsect;
	vector<face> FF = loadStlFromFlie("e2.stl");

	Triangle triangle1;
	for (int i = 0;i < FF.size();i++)
	{
		triangle1 = FF[i];
		Tria.push_back(triangle1);
	}
	for (int i = 0;i < Tria.size();i++)
	{
		for (int j = i + 1;j < Tria.size();j++)
		{
			if (judge_triangle_topologicalStructure(&Tria[i], &Tria[j]) == INTERSECT)
			{
				interInsect.push_back(i);
				interInsect.push_back(j);
				break;
			}

		}

	}

	cout << "�ཻ��ĸ�����" << interInsect.size() << "\n";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("opengl1");
	glutDisplayFunc(&myDisplay);
	glutMainLoop();
	return 0;
}