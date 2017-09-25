#ifndef _FAULT_TOLERANT__
#define _FAULT_TOLERANT__

#include "MSAABB.h"
//#include "globalFunctions.h"
#include "HE_mesh\Mesh3D.h"

/*
MeshOcNode作为八叉树的记录节点

child_node[8] 记录当前节点下属的八个子节点

ocn_aabb	  记录当前节点包含的范围

is_leaf		  记录当前节点是否是叶子结点

*/

class MeshOcNode
{

public:
	MeshOcNode*				child_node_[8]; // 安全删除MeshOctree析构函数
	AABB					ocn_aabb_;
	bool					is_leaf_;
	std::vector<int>*		ocn_face_list_;

public:
	MeshOcNode()
	{
		for (int i = 0; i < 8; i++)
			child_node_[i] = NULL;
		is_leaf_ = false;
	};
	~MeshOcNode()
	{
		for (int i = 0; i < 8; i++)
			SafeDelete(child_node_[i]);
	};

};

class MeshOctree
{
public:
	MeshOcNode*				oc_root_;
	std::vector<Triangle>*	oc_face_list_;//模型的所有三角面
	std::vector<Vec3f>		bary_center;//存储三角面的重心

	MeshOctree()
		: oc_root_(NULL)
		, oc_face_list_(NULL)
	{

	}
	~MeshOctree() { DeleteMeshOcTree(); };

	void DeleteMeshOcTree();

	void BuildOctree(Mesh3D* ptr_in);


private:
	//如果子节点等于它的父节点，停止递归（避免无限分割下去）
	void createOctree(MeshOcNode*& node_in, std::vector<int>* face_list_idx, AABB aabb, bool stopFlag);

};

#endif
