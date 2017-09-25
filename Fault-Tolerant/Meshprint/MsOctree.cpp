#include "MsOctree.h"

void MeshOctree::BuildOctree(Mesh3D* ptr_in)
{
	oc_face_list_ = new std::vector<Triangle>;

	for (int i=0;i<ptr_in->Tria.size();i++)
	{
		oc_face_list_->push_back(ptr_in->Tria[i]);
	}
	std::vector<int>* face_list_idx = new std::vector < int >;
	for (int i = 0; i < oc_face_list_->size(); i++)
	{
		face_list_idx->push_back(i);
		Vec3f tmp_bary_center;
		//取得该三角形的重心
		tmp_bary_center += oc_face_list_->at(i).Vertex_1;
		tmp_bary_center += oc_face_list_->at(i).Vertex_2;
		tmp_bary_center += oc_face_list_->at(i).Vertex_3;

		tmp_bary_center /= 3;
		bary_center.push_back(tmp_bary_center);
	}

	

	AABB aabb = AABB(ptr_in->getBoundingBox()[0], ptr_in->getBoundingBox()[1]);//取得包围整个model的最大包围盒
//	DeleteMeshOcTree();
	createOctree(oc_root_, face_list_idx,aabb, false);
}

//stopFlag赋值为true时，会停止创建节点，立方体空间即不会再分
void MeshOctree::createOctree(MeshOcNode*& node_in, std::vector<int>* face_list_idx, AABB aabb, bool stopFlag)
{
	if (face_list_idx->size() == 0)
		return;
	node_in = new MeshOcNode();
	//node_in->ocn_aabb_ = aabb;

	//控制深度
	//if (--MsOctreeDeep == 0)
	//{
	//	stopFlag = true;
	//}
	if (face_list_idx->size()<=8||stopFlag) {
		node_in->is_leaf_ = true;
		node_in->ocn_face_list_ = new std::vector < int >;
		for (std::vector<int>::iterator i = face_list_idx->begin(); i != face_list_idx->end(); i++)
			node_in->ocn_face_list_->push_back(*i);

		// compute the bounding box
		AABB cur_aabb;
		cur_aabb = oc_face_list_->at(0).getAABB();//构建三角面的包围盒
		for (std::vector<int>::iterator i = face_list_idx->begin(); i != face_list_idx->end(); i++)
		{
			AABB tmp_aabb;
			tmp_aabb=oc_face_list_->at(*i).getAABB();
			cur_aabb.merge(tmp_aabb);
		}
		node_in->ocn_aabb_ = cur_aabb;

		return;

	}


	std::vector<int>* tmp_face_list_idx[8];
	for (int i = 0; i < 8; i++)
		tmp_face_list_idx[i] = new std::vector < int >;

	Vec3f split_center = (aabb._max + aabb._min) / 2.0f;//该层晶格的中心点

	for (std::vector<int>::iterator i = face_list_idx->begin(); i != face_list_idx->end(); i++)
	{
		float bcx = bary_center[*i].x();
		float bcy = bary_center[*i].y();
		float bcz = bary_center[*i].z();
		if (bcy < split_center.y())
		{
			if (bcx < split_center.x() && bcz >= split_center.z())
				tmp_face_list_idx[0]->push_back(*i);
			else if (bcx >= split_center.x() && bcz >= split_center.z())
				tmp_face_list_idx[1]->push_back(*i);
			else if (bcx >= split_center.x() && bcz < split_center.z())
				tmp_face_list_idx[2]->push_back(*i);
			else
				tmp_face_list_idx[3]->push_back(*i);
		}
		else
		{
			if (bcx < split_center.x() && bcz >= split_center.z())
				tmp_face_list_idx[4]->push_back(*i);
			else if (bcx >= split_center.x() && bcz >= split_center.z())
				tmp_face_list_idx[5]->push_back(*i);
			else if (bcx >= split_center.x() && bcz < split_center.z())
				tmp_face_list_idx[6]->push_back(*i);
			else
				tmp_face_list_idx[7]->push_back(*i);
		}
	}
	

	Vec3f hab = (aabb._max - aabb._min) / 2.0f;//分割成的小晶格的宽度


	createOctree(node_in->child_node_[0], tmp_face_list_idx[0], AABB(split_center, aabb._min), stopFlag);
	createOctree(node_in->child_node_[1], tmp_face_list_idx[1], AABB(split_center + Vec3f(hab.x(), 0, 0), aabb._min + Vec3f(hab.x(), 0, 0)), stopFlag);
	createOctree(node_in->child_node_[2], tmp_face_list_idx[2], AABB(aabb._max - Vec3f(0, hab.y(), 0), split_center - Vec3f(0, hab.y(), 0)), stopFlag);
	createOctree(node_in->child_node_[3], tmp_face_list_idx[3], AABB(split_center - Vec3f(0, 0, hab.z()), aabb._min - Vec3f(0, 0, hab.z())), stopFlag);

	createOctree(node_in->child_node_[4], tmp_face_list_idx[4], AABB(split_center + Vec3f(0, hab.y(), 0), aabb._min + Vec3f(0, hab.y(), 0)), stopFlag);
	createOctree(node_in->child_node_[5], tmp_face_list_idx[5], AABB(aabb._max + Vec3f(0, 0, hab.z()), split_center + Vec3f(0, 0, hab.z())), stopFlag);
	createOctree(node_in->child_node_[6], tmp_face_list_idx[6], AABB(aabb._max, split_center), stopFlag);
	createOctree(node_in->child_node_[7], tmp_face_list_idx[7], AABB(aabb._max - Vec3f(hab.x(), 0, 0), split_center - Vec3f(hab.x(), 0, 0)), stopFlag);

	//合并大的包围盒
	int i = 0;
	for (; i < 8; i++) {
		if (node_in->child_node_[i] != NULL) {
			node_in->ocn_aabb_ = node_in->child_node_[i]->ocn_aabb_;
			break;
		}
	}

	for (; i < 8; i++) {
		if (node_in->child_node_[i] != NULL) {
			node_in->ocn_aabb_.merge(node_in->child_node_[i]->ocn_aabb_);
		}
	}
}

//清零操作
void MeshOctree::DeleteMeshOcTree()
{
	SafeDelete(this->oc_root_);
}