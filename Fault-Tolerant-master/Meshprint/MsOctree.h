#ifndef _FAULT_TOLERANT__
#define _FAULT_TOLERANT__

#include "MSAABB.h"
//#include "globalFunctions.h"
#include "HE_mesh\Mesh3D.h"

/*
MeshOcNode��Ϊ�˲����ļ�¼�ڵ�

child_node[8] ��¼��ǰ�ڵ������İ˸��ӽڵ�

ocn_aabb	  ��¼��ǰ�ڵ�����ķ�Χ

is_leaf		  ��¼��ǰ�ڵ��Ƿ���Ҷ�ӽ��

*/

class MeshOcNode
{

public:
	MeshOcNode*				child_node_[8]; // ��ȫɾ��MeshOctree��������
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
	std::vector<Triangle>*	oc_face_list_;//ģ�͵�����������
	std::vector<Vec3f>		bary_center;//�洢�����������

	MeshOctree()
		: oc_root_(NULL)
		, oc_face_list_(NULL)
	{

	}
	~MeshOctree() { DeleteMeshOcTree(); };

	void DeleteMeshOcTree();

	void BuildOctree(Mesh3D* ptr_in);


private:
	//����ӽڵ�������ĸ��ڵ㣬ֹͣ�ݹ飨�������޷ָ���ȥ��
	void createOctree(MeshOcNode*& node_in, std::vector<int>* face_list_idx, AABB aabb, bool stopFlag);

};

#endif
