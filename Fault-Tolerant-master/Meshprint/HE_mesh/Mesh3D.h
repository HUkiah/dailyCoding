#ifndef _MESH3D__
#define _MESH3D__

#include <vector>
#include <map>
#include "Vec.h"
#include <set>
#include "Triangle.h"

#define PARTTABLENUM 32

// forward declarations of mesh classes
class HE_vert;
class HE_edge;
class HE_face;
class Triangle;

using trimesh::point;

typedef trimesh::point point;
typedef trimesh::vec2 Vec2f;
typedef trimesh::vec3  Vec3f;
typedef trimesh::vec4  Vec4f;


enum SelectTag
{
    UNSELECTED = 0,		//
    SELECTED,			//
	OTHER				//
};

enum BoundaryTag
{
	BOUNDARY,
	INNER,
	TO_SPLIT
};

/*!
*	The basic vertex class for half-edge structure.
*/
class HE_vert
{
public:
	int		id_;
	point	position_;		//!< vertex position
	Vec3f	normal_;		//!< vertex normal
	Vec3f	texCoord_;		//!< texture coord
	Vec4f	color_;
	HE_edge		*pedge_;		//!< 表示一个半边(表示的是model的外边界，反右手定则的边)
	int			degree_;		//!<度. 表示这个点周围面的个数（点周围有多少个面）
	BoundaryTag	boundary_flag_;	//!< 边界标志
	int			selected_;		//!<表示点是否被选中
	std:: vector<HE_edge*> adjHEdges;//表示位于点pair侧的所有边的集合 仅供阅读对象使用，请勿在其他地方使用
	/*--------------------add by wang kang at 2013-10-12----------------------*/
	std::vector<size_t> neighborIdx;//表示此点周围的几个点
	//HE_vert* pprev;
	//HE_vert* pnext;
	//std::vector<Vec3f> helper;
public:
	HE_vert(const Vec3f& v)
		: id_(-1), position_(v), pedge_(NULL), degree_(0), boundary_flag_(INNER), selected_(UNSELECTED)
		, color_(255.f / 255.f, 215.f / 255.f, 0.f/ 255.f, 1.f),neighborIdx()
	{}

	~HE_vert(void) { neighborIdx.clear();}

	bool		isOnBoundary(void) {return boundary_flag_==BOUNDARY;}
	int			id(void) {return id_;}
	Vec3f&		normal(void) {return normal_;}
	Vec3f&		position(void) {return position_;}
	Vec3f&		texCoordinate(void) {return texCoord_;}
	Vec4f&		color(void) {return color_;};
	int			degree(void) {return degree_;}
	int			selected(void) {return selected_;}
	BoundaryTag boundary_flag(void) {return boundary_flag_;}

	void		set_normal(const Vec3f& n) {normal_=n;}
	void		set_position(const Vec3f& p) {position_=p;}
	void		set_id(int id) {id_=id;}
	void		set_color(const Vec4f& c) {color_=c;}
	void		set_seleted(int tag){selected_=tag;}
	void		set_boundary_flag(BoundaryTag bt) {boundary_flag_=bt;}
};

/*!
*	The basic edge class for half-edge structure.
*/
class HE_edge
{
public:
	int			id_;
	HE_vert		*pvert_;		//!< 顶点为半边的末端
	HE_vert		*start_;		//!< 顶点为半边的始端
	HE_edge		*ppair_;		//!< 相对相邻的半边
	HE_face		*pface_;		//!< 表示边指向那个面
	HE_edge		*pnext_;		//!< 下一条半边			pprev_         pnext_ 
	HE_edge		*pprev_;		//!< 半边的前一条半边  --------> half -------->
	Vec3f		texCoord_;		//!< 端点的纹理坐标
	BoundaryTag boundary_flag_;	//!< 边界标志
	bool		is_selected_;	//!< 拾取\选中标志

public:
	HE_edge()
		: id_(-1), pvert_(NULL), ppair_(NULL)
		, pface_(NULL), pnext_(NULL), pprev_(NULL), boundary_flag_(INNER), is_selected_(false)
	{}


	~HE_edge()
	{}

	int		id(void) {return id_;}
	bool	isBoundary(void) {return boundary_flag_==BOUNDARY;}
	BoundaryTag boundary_flag(){return boundary_flag_;}
	void    set_boundary_flag(BoundaryTag bt) {boundary_flag_ = bt;}
};

/*!
*	The basic face class for half-edge structure.
*/
class HE_face
{
public:
	int			id_;
	HE_edge		*pedge_;		//!< 表示面的那条边
	Vec3f		normal_;		//!< 面的法向
	int			valence_;		//!< 表示面上顶点的数量
	int			selected_;		//!< 一个标记:表示面是否选中
	Vec4f		color_;			//!< 这个面的颜色
	BoundaryTag boundary_flag_;	//!< 这标记是用来划分网格的
	int com_flag;				//!< 组件 编号
	std::vector<HE_vert* > vertices_;
public:
	HE_face()
		: id_(-1), pedge_(NULL), valence_(0), selected_(UNSELECTED), boundary_flag_(INNER), normal_(0,0,0),com_flag(-1)
	{}

	~HE_face()
	{}

	int				id(void) {return id_;}
	int				valence(void) {return valence_;}
	Vec3f&			normal(void) {return normal_;}
	int				selected(void) {return selected_;}
	Vec4f&			color(void) {return color_;}
	BoundaryTag		boundary_flag() {return boundary_flag_;}

	void			set_selected(int tag){selected_=tag;}
	void			set_color(const Vec4f& c) {color_=c;}
	void			set_boundary_flag(BoundaryTag bt) {boundary_flag_=bt;}

	/*-----------add by wang kang at 2013-10-13-------------*/
	/*-----------modified by shuai   2016-12-13------------*/
	//verts获取面片的三个顶点
	void face_verts(std::vector<HE_vert *>& verts)
	{
		verts.clear();
		HE_edge* pedge = pedge_;

		do
		{
			verts.push_back(pedge->pvert_);
			pedge = pedge->pnext_;
			if (verts.size() > 3)//孔洞，不规则的多边形
			{
				verts.clear();
				break;
			}
		} while (pedge != pedge_);
	}

	//计算面的几何中心
	point center() 
	{
		point center;
		HE_edge* pedge = pedge_;

		do 
		{
			center += pedge->pvert_->position();
			pedge = pedge->pnext_;

		} while (pedge != pedge_);

		center /= 3.0;
		return center;		
	}

public:
	//计算三角面的最大，最小值
	void getBoundingBox(Vec3f& max_out, Vec3f& min_out)
	{
		max_out = Vec3f(0, 0, 0);
		min_out = Vec3f(0, 0, 0);
		HE_edge* pedge = pedge_;

		do
		{
			Vec3f bb_temp = pedge->pvert_->position();
			max_out.x() = max_out.x() < bb_temp.x() ? bb_temp.x() : max_out.x();
			max_out.y() = max_out.y() < bb_temp.y() ? bb_temp.y() : max_out.y();
			max_out.z() = max_out.z() < bb_temp.z() ? bb_temp.z() : max_out.z();

			min_out.x() = min_out.x() > bb_temp.x() ? bb_temp.x() : min_out.x();
			min_out.y() = min_out.y() > bb_temp.y() ? bb_temp.y() : min_out.y();
			min_out.z() = min_out.z() > bb_temp.z() ? bb_temp.z() : min_out.z();

			pedge = pedge->pnext_;
		} while (pedge != pedge_);
	}
};

//set集合中定义排序规则，由 x->y->z
struct comVertex
{
	bool operator ()(HE_vert* a, HE_vert* b)const
	{
		if (a->position().x() - b->position().x() < -1e-4)
		{
			return true;
		}
		else if (a->position().x() - b->position().x() < 1e-4)
		{
			if (a->position().y()-b->position().y()<-1e-4)
			{
				return true;
			}
			else if (a->position().y() - b->position().y()<1e-4)
			{
				return a->position().z() - b->position().z() < -1e-4;
			}
			return false;
		}
		return false;
	}
};


class Mesh3D
{
	// type definitions
	typedef std::vector<HE_vert* >::iterator VERTEX_ITER;
	typedef std::vector<HE_face* >::iterator FACE_ITER;
	typedef std::vector<HE_edge* >::iterator EDGE_ITER;

	typedef std::vector<HE_vert* >::reverse_iterator VERTEX_RITER;
	typedef std::vector<HE_face* >::reverse_iterator FACE_RITER;
	typedef std::vector<HE_edge* >::reverse_iterator EDGE_RITER;
	typedef std::pair<HE_vert*, HE_vert* > PAIR_VERTEX;

private:
	// mesh data
	std::vector<HE_vert*>	*pvertices_list_;	//!< 存储顶点
	std::vector<HE_edge*>	*pedges_list_;		//!< 存储边
	std::vector<HE_face*>	*pfaces_list_;		//!< 存储面
	std::vector<HE_edge*>	*bheList;		// 外边界边集合 不指向任何面
	std::vector<HE_edge*>	*iheList;		// 内半边集合
	std::vector<std::vector<HE_edge*>>   bLoop;	//孔洞？
	int no_loop;
	std::set<HE_vert*,comVertex>   input_vertex_list_;// !< 存储STL输入顶点
	// mesh info
	int		num_components_;					//!< 组件的数量
	float	average_edge_length_;				//!< 平均边界长度

	//std::map<std::pair<HE_vert*, HE_vert* >, HE_vert* >    midPointMap_;
	//! 模型包围盒的值
	float xmax_, xmin_, ymax_, ymin_, zmax_, zmin_;
	std::vector<int> wro_Nor_facets_;//错误面？

public:
	//! 将两端顶点与它的边缘相关联:只用于创建网格
	std::map<std::pair<HE_vert*, HE_vert* >, HE_edge* >    edgemap_;
	
	std::vector<Triangle> Tria; //用来判断三角面的位置关系
	std::vector<Triangle> Tri; //用来判断三角面的位置关系
//	std::vector<Triangle *> partitionTable[PARTTABLENUM]; //分区检测某一坐标轴
	std::vector<Triangle *> partitionTable_Z[PARTTABLENUM]; //分区检测Z轴
//	std::vector<Triangle *> partitionTable_Y[PARTTABLENUM]; //分区检测Y轴
	//! constructor
	Mesh3D(void);

	//! destructor
	~Mesh3D(void);

	//输出一些点的信息zhulin.txt Debug example
	void exportNeighborId();

	//! get the pointer of vertex list
	inline std::vector<HE_vert* >* get_vertex_list(void) {return pvertices_list_;}
	
	//! get the pointer of edges list
	inline std::vector<HE_edge* >* get_edges_list(void) {return pedges_list_;}

	//! get the pointer of faces list
	inline std::vector<HE_face* >* get_faces_list(void) { return pfaces_list_; }

	//获取边界边列表
	inline std::vector<HE_edge* >* get_bedges_list(void) { return bheList; }

	//! 得到顶点的总数
	inline int num_of_vertex_list(void)	{return pvertices_list_ ? static_cast<int>(pvertices_list_->size()) : 0;}

	//! 得到半边的总数
	inline int num_of_half_edges_list(void)	{return pedges_list_ ? static_cast<int>(pedges_list_->size()) : 0;}

	//! 得到边的总数 /2
	inline int num_of_edge_list(void) {return num_of_half_edges_list()/2;}

	//! get the total number of faces
	inline int num_of_face_list(void) {return pfaces_list_ ? static_cast<int>(pfaces_list_->size()) : 0;}

	//! get the number of components
	inline int num_of_components(void) {return num_components_;}

	//! get the average edge length
	inline float average_edge_length(void) {return average_edge_length_;}

	//! get the pointer of the id-th vertex
	inline HE_vert* get_vertex(int id) {return id >= num_of_vertex_list() || id<0 ? NULL : (*pvertices_list_)[id];}

	//! get the pointer of the id-th half-edge
	inline HE_edge* get_half_edge(int id) {return id >= num_of_half_edges_list() || id<0 ? NULL : (*pedges_list_)[id];}

	//! get the pointer of the id-th face
	inline HE_face* get_face(int id) {return id >= num_of_face_list() || id<0 ? NULL : (*pfaces_list_)[id];}

	//! get the half-edge from vertex hv0 to hv1
	inline HE_edge* get_edge(HE_vert* hv0, HE_vert* hv1)
	{
		if (!hv0 || !hv1) return NULL;
		HE_edge* edge = hv0->pedge_;
		do 
		{
			if (edge->pvert_ == hv1)
			{
				return edge;
			}
			edge = edge->ppair_->pnext_;//此循环可以便利改点周围所有的边 （放射形）
		} while (edge!=hv0->pedge_);
		return NULL;
	}

	inline std::vector<int> getWrongNorFacets() { return wro_Nor_facets_; }

	//! 检查网格是否有效
	inline bool isValid(void)
	{
		if ( num_of_vertex_list()==0 || num_of_face_list()==0)
		{
			return false;
		}
		return true;
	}


	//! insert a vertex 
	/*!
	*	\param v a 3d point
	*	\return a pointer to the created vertex
	*	\返回创建的顶点的指针
	*/

	HE_vert* InsertVertex(const Vec3f& v);
	
	HE_vert* InsertVertex( HE_vert* & v);

	//HE_vert* SortInsertVertex(const Vec3f& v, int index, int start, int end);

	//! insert an edge
	HE_edge* InsertEdge(HE_vert* vstart, HE_vert* vend);

	//! insert a face
	/*!
	*	\param vec_hv the vertex list of a face
	*	\return a pointer to the created face
	*/
	HE_face* InsertFace(std::vector<HE_vert* >& vec_hv);

	//孔洞的修复过程中插入面片的处理
	//HE_face* InsertFace_FromLoop(std::vector<HE_vert* >& vec_hv);
	
	// FILE IO
	//! load a 3D mesh from an OBJ format file
	bool LoadFromOBJFile(const char* fins);
	//! export the current mesh to an OBJ format file
	void WriteToOBJFile(const char* fouts);



	//保存修改完成后的mesh
	void MntnMesh(const char* fouts);

	//保存文件采用半边数据结构中的pfaces_list_
	void WriteToStlFile(const char* fouts);

	//银行家舍入法
	float getRound(float num);
	//检测三角面是否相交
	void TriangleIntersect();
	//自动修复三角面片的相交
	bool Maintenance();
	//擦除标记
	void ClearMark();
	//Tria转到Tri 为了使标记相交button有效
	void TriaToTri();
	//新增三角面片,以连接点的方式
	void CreateTriangle();
	//修复孔洞
	void RepairHole();



	bool LoadFromSTLFile(const char * fins);


	//转录面片到Tria
	void transcriptionFaces(void);

	//! update mesh:
	/*! 
	*	call it when you have created the mesh
	*   创建网格时调用它
	*/
	void UpdateMesh(void);

	//! update normal
	/*!
	*	compute all the normals of vertex and faces
	*	计算所有顶点和面的法线
	*/
	void UpdateNormal(void);

	//! 计算包围盒
	void ComputeBoundingBox(void);

	//! get the face with id0, id1, id2 vertices
	HE_face* get_face(int vId0, int vId1, int vId2);

	//! get the face whose vertices are the ids vertices
	HE_face* get_face(const std::vector<unsigned int>& ids);

	int		GetFaceId(HE_face* face);

	//! reset all the selected flags for the vertices as the tag
	void ResetVertexSelectedTags(int tag=UNSELECTED);

	//! reset all the selected flags for the faces as the tag
	void ResetFaceSelectedTags(int tag=UNSELECTED);

	//! check the vertex v0 and v1 whether are neighborhoods
	// ! 检查顶点V0和V1是否相邻
	bool isNeighbors(HE_vert* v0, HE_vert* v1);

	//! get the selected "first" vertex, if no one is selected, return -1
	// 如果没有选择，则返回所选的“第一个”顶点（用-1进行的初始化），返回 -1 ....
	int GetSelectedVrtId();

	//! create a mesh by the vertices positions and triangle topology
	//通过顶点位置和三角形拓扑创建一个网格
	void CreateMesh(const std::vector<Vec3f>& verts, const std::vector<int>& triIdx);

	//!   the triIdx is defined as tri0_v0, tri0_v1, tri0_v2, tr1_v0, tr1_v1, tr1_v2, ...
	//三角面的顶点被定义为tri0_v0, tri0_v1, tri0_v2, tr1_v0, tr1_v1, tr1_v2, ...的样子
	void CreateMesh(const std::vector<double>& verts, const std::vector<unsigned>& triIdx);

	//获取边界点的数量
	int GetBoundaryVrtSize();


	void meshTranslate(float param1, float param2);

	//放大缩小网格比例
	void scalemesh(float scaleT);
public:
	//! clear all the data
	void ClearData(void);
private:
	//! clear vertex
	void ClearVertex(void);
	//! clear edges
	void ClearEdges(void);
	//! clear faces
	void ClearFaces(void);

	//normal computation

	//! compute all the normals of faces
	void ComputeFaceslistNormal(void);
	//! compute the normal of a face
	void ComputePerFaceNormal(HE_face* hf);

	//! compute all the normals of vertex
	void ComputeVertexlistNormal(void);
	//! compute the normal of a vertex
	void ComputePerVertexNormal(HE_vert* hv); 


	//! compute the number of components
	//void ComputeNumComponents(void);

	//! compute the average edge length
	void ComputeAvarageEdgeLength(void);

	//! set vertex and edge boundary flag
	void SetBoundaryFlag(void);

	//! for the boundary vertices, make sure the half-edge structure can find all of them
	void BoundaryCheck();

	void countBoundaryComponat();
	//! unify mesh
	void Unify(float size);

	//! check the face whether contains the vert
	bool isFaceContainVertex(HE_face* face, HE_vert* vert);


/*----------------------------------add by wang kang at 2013-10-12- -----------------------------------*/
	void get_neighborId(const size_t& vertid, std::vector<size_t>& neighbors)
	{
		HE_edge* edge = get_vertex(vertid)->pedge_;
		neighbors.clear();
		int num = 0;
		if (edge == NULL)
		{
			qDebug() << "vertid-pedge is null "<<num++;
			return;
		}
		
		do 
		{
			neighbors.push_back(edge->pvert_->id());
			edge = edge->ppair_->pnext_;
			/*if (num++==100)
			{
				break;
			}*/

		} while (edge!=get_vertex(vertid)->pedge_ && edge != NULL);

		std::vector<size_t> tmp = neighbors;
		for (size_t i=0; i!= tmp.size(); ++i)
			neighbors[i] = tmp[tmp.size()-i-1];
	}
private:
	void SetNeighbors()
	{
		for (size_t i=0; i!= num_of_vertex_list(); ++i)
			get_neighborId(i, pvertices_list_->at(i)->neighborIdx);
	}

public:
	void LinearTex()
	{
		for (size_t i=0; i != num_of_vertex_list(); ++i)
			pvertices_list_->at(i)->texCoord_ = pvertices_list_->at(i);
	}

	void SphereTex()
	{
		static const float pi = 3.1415926;
		for (size_t i=0; i != num_of_vertex_list(); ++i)
		{
			register HE_vert * vert = pvertices_list_->at(i);
			point p = vert->position().length()*vert->normal();


			float r = sqrt(p[1]*p[1]+p[0]*p[0]);

			vert->texCoord_[0] = asin(p[2])/pi + 0.5;
			vert->texCoord_[1] = acos(p[0])/(2*pi);
		}
	}
	/*---------------------------------------------------------*/


	// support operations
	std::vector<Vec3f> getBoundingBox(){
		std::vector<Vec3f> myboundingbox(1, Vec3f(xmax_, ymax_, zmax_));
		myboundingbox.push_back(Vec3f(xmin_, ymin_, zmin_));
		return myboundingbox;	
	}
	// slice-cutting operations
	std::map<std::pair<HE_vert*, HE_vert* >, HE_edge* >  getedgemap(){
		return edgemap_;
	}
	void Transformation(float * matrix);
	void SetDirection(int faceid);
		
private:
	void ClearSlice();
	//////////////////////////////////////////////////////////////////////////
public:
	HE_face*InsertFaceSup(std::vector<HE_vert* >& vec_hv);
	void UpdateMeshSup(void);
	std::vector<std::vector<HE_edge*>>  GetBLoop() { return bLoop; }
	void UpdateBList(void);

	//计算bloop 历史版本，bug大王
	void computeComponent();		

	//计算bloop 最新版本，正在测试(只是将bhelist中的边分一下类，edge之间没有关系)
	void setBloopFromBhelist();		

	void FaceDFS(HE_face* facet, int no);

	void VertexDFS(HE_vert * v);
	//DFS的方式，为每个面打上com_flag的标记，区分不同阵营
	
	std::vector<HE_edge*>* GetBhelist() { return bheList; }
};

#endif // _MESH3D__