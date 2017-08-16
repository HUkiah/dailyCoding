#ifndef MESHBASE_MAINTENANCE
#define MESHBASE_MAINTENANCE

#include <vector>
#include <set>
#include <map>
#include "Vec.h"

class HE_vert;
class HE_edge;
class HE_face;

using trimesh::point;

typedef trimesh::point point;
typedef trimesh::vec3  Vec3f;

typedef float float3[3];

/*!
*	The basic vertex class for half-edge structure.
*/
class HE_vert
{
public:
	int id_; //还记得obj文件的格式吗，记录构成face的vertex index
	Vec3f	normal_;		//!< vertex normal

	point position_;
	HE_edge *edge;
public:
	int id(void) { return id_; }
	void set_id(int id) { id_ = id; }
	Vec3f&		position(void) { return position_; }
	Vec3f&		normal(void) { return normal_; }
public:
	HE_vert(const float3& v):position_(v)
	{}

	~HE_vert(void) {}
};

/*!
*	The basic edge class for half-edge structure.
*/
class HE_edge
{
public:
	int id_;
	HE_vert		*vert_;		//Half Edge end vertex
	HE_edge		*pair_;		//相对相邻的半边
	HE_face		*face_;		//哪一个face的 edge
	HE_edge		*pnext_;	//下一条半边
public:
	int id(void) { return id_; }
	void set_id(int id) { id_ = id; }

};

/*!
*	The basic face class for half-edge structure.
*/
class HE_face
{
public:
	int id_;
	HE_edge		*pedge_;		//one of the half-edges_list bordering the face
public:
	int id(void) { return id_; }
	void set_id(int id) { id_ = id; }

	void getBoundingBox(Vec3f& max_out, Vec3f& min_out)
	{
		max_out = Vec3f(0, 0, 0);
		min_out = Vec3f(0, 0, 0);
		HE_edge* pedge = pedge_;

		do
		{
			Vec3f bb_temp = pedge->vert_->position();
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
			if (a->position().y() - b->position().y() < -1e-4)
			{
				return true;
			}
			else if (a->position().y() - b->position().y() < 1e-4)
			{
				return a->position().z() - b->position().z() < -1e-4;
			}
			return false;
		}
		return false;
	}
};

class MeshBase {
	// type definitions
	typedef std::vector<HE_vert* >::iterator VERTEX_ITER;
	typedef std::vector<HE_face* >::iterator FACE_ITER;
	typedef std::vector<HE_edge* >::iterator EDGE_ITER;
	typedef std::pair<HE_vert*, HE_vert* > PAIR_VERTEX;



private:
	// mesh data
	std::vector<HE_vert*>	*pvertices_list_;	//!< store vertex
	std::vector<HE_edge*>	*pedges_list_;		//!< store edges
	std::vector<HE_face*>	*pfaces_list_;		//!< store faces

	std::set<HE_vert*, comVertex>   input_vertex_list_;// !< strore STL input vertex
    
	//! associate two end vertex with its edge: only useful in creating mesh
	std::map<std::pair<HE_vert*, HE_vert* >, HE_edge* > edgemap_;

	//! values for the bounding box
	float xmax_, xmin_, ymax_, ymin_, zmax_, zmin_;

public:
	//! constructor
	MeshBase(void);

	//! destructor
	~MeshBase(void);
#pragma region keke



public:
	//! get the pointer of vertex list
	inline std::vector<HE_vert* >* get_vertex_list(void) { return pvertices_list_; }

	//! get the pointer of edges list
	inline std::vector<HE_edge* >* get_edges_list(void) { return pedges_list_; }

	//! get the pointer of faces list
	inline std::vector<HE_face* >* get_faces_list(void) { return pfaces_list_; }

	//! get the total number of vertex
	inline int num_of_vertex_list(void) { return pvertices_list_ ? static_cast<int>(pvertices_list_->size()) : 0; }

	//! get the total number of half-edges
	inline int num_of_half_edges_list(void) { return pedges_list_ ? static_cast<int>(pedges_list_->size()) : 0; }

	//! get the total number of edges
	inline int num_of_edge_list(void) { return num_of_half_edges_list() / 2; }

	//! get the total number of faces
	inline int num_of_face_list(void) { return pfaces_list_ ? static_cast<int>(pfaces_list_->size()) : 0; }

public:

	//! check whether the mesh id valid
	inline bool isValid(void)
	{
		if (num_of_vertex_list() == 0 || num_of_face_list() == 0)
		{
			return false;
		}
		return true;
	}

	//! insert an edge
	HE_edge* InsertEdge(HE_vert* vstart, HE_vert* vend);

	//! insert a face
	/*!
	*	\param vec_hv the vertex list of a face
	*	\return a pointer to the created face
	*/
	HE_face* InsertFace(std::vector<HE_vert* >& vec_hv);

public:

	//! unify mesh
	void Unify(float size);

	bool LoadFromSTLFile(const char * fins);

	//! clear all the data
	void ClearData(void);

	void SphereTex()
	{
		static const float pi = 3.1415926;
		for (size_t i = 0; i != num_of_vertex_list(); ++i)
		{
			register HE_vert * vert = pvertices_list_->at(i);
			point p = vert->position().length()*vert->normal();


			float r = sqrt(p[1] * p[1] + p[0] * p[0]);

		//	vert->texCoord_[0] = asin(p[2]) / pi + 0.5;
		//	vert->texCoord_[1] = acos(p[0]) / (2 * pi);
		}
	}

	// support operations
	std::vector<Vec3f> getBoundingBox() {
		std::vector<Vec3f> myboundingbox(1, Vec3f(xmax_, ymax_, zmax_));
		myboundingbox.push_back(Vec3f(xmin_, ymin_, zmin_));
		return myboundingbox;
	}

	
private:
	//! clear vertex
	void ClearVertex(void);
	//! clear edges
	void ClearEdges(void);
	//! clear faces
	void ClearFaces(void);
};

#endif
