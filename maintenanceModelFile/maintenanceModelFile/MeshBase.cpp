#include "MeshBase.h"
#include "qlogging.h"
#include "QString"
#include "QFile"
#include "QTextStream"
#include <iostream>
#include "QDataStream"
#include <winsock.h>

MeshBase::MeshBase(void)
{
	// intialization
	pvertices_list_ = new std::vector<HE_vert*>;
	pfaces_list_ = new std::vector<HE_face*>;
	pedges_list_ = new std::vector<HE_edge*>;

	xmax_ = ymax_ = zmax_ = 1.f;
	xmin_ = ymin_ = zmin_ = -1.f;
}

MeshBase::~MeshBase(void)
{
	ClearData();
}

void MeshBase::ClearData(void)
{
	ClearVertex();
	ClearEdges();
	ClearFaces();
}

void MeshBase::ClearVertex(void)
{

	if (pvertices_list_ == NULL)
	{
		return;
	}
	else
	{
		for (VERTEX_ITER viter = pvertices_list_->begin(); viter != pvertices_list_->end(); viter++)
		{
			if (*viter != NULL)
			{
				delete *viter;
				*viter = NULL;
			}
			else
			{
				// ERROR
			}
		}
		delete pvertices_list_;
		pvertices_list_ = NULL;
	}
}

void MeshBase::ClearEdges(void)
{
	if (pedges_list_ == NULL)
	{
		return;
	}
	else
	{
		for (EDGE_ITER eiter = pedges_list_->begin(); eiter != pedges_list_->end(); eiter++)
		{
			if (*eiter != NULL)
			{
				delete *eiter;
				*eiter = NULL;
			}
			else
			{
				// ERROR
			}
		}
		delete pedges_list_;
		pedges_list_ = NULL;
	}
}

void MeshBase::ClearFaces(void)
{
	if (pfaces_list_ == NULL)
	{
		return;
	}
	else
	{
		for (FACE_ITER fiter = pfaces_list_->begin(); fiter != pfaces_list_->end(); fiter++)
		{
			if (*fiter != NULL)
			{
				delete *fiter;
				*fiter = NULL;
			}
			else
			{
				// ERROR
			}
		}
		delete pfaces_list_;
		pfaces_list_ = NULL;
	}
}

HE_edge* MeshBase::InsertEdge(HE_vert* vstart, HE_vert* vend)
{
	if (vstart == NULL || vend == NULL)
	{
		return NULL;
	}

	if (pedges_list_ == NULL)
	{
		pedges_list_ = new std::vector<HE_edge*>;
	}

	if (edgemap_[PAIR_VERTEX(vstart, vend)] != NULL)
	{
		return edgemap_[PAIR_VERTEX(vstart, vend)];
	}
	//  if  has not find  an  exist edge

	HE_edge* pedge = new HE_edge;
	pedge->vert_ = vend;
	vstart->edge = pedge;
	edgemap_[PAIR_VERTEX(vstart, vend)] = pedge;
	pedge->id_ = static_cast<int>(pedges_list_->size());
	pedges_list_->push_back(pedge);

	return pedge;
}

HE_face* MeshBase::InsertFace(std::vector<HE_vert* >& vec_hv)
{
	int vsize = static_cast<int>(vec_hv.size());

	if (pfaces_list_ == NULL)
	{
		pfaces_list_ = new std::vector<HE_face*>;
	}

	HE_face *pface = new HE_face;

	HE_edge *he1 = NULL, *he2 = NULL, *he3 = NULL, *he1_pair_ = NULL, *he2_pair_ = NULL, *he3_pair_ = NULL;
	//std::vector<HE_edge*> vec_edges;
	he1 = InsertEdge(vec_hv[0], vec_hv[1]);
	he2 = InsertEdge(vec_hv[1], vec_hv[2]);
	he3 = InsertEdge(vec_hv[2], vec_hv[0]);
	he1_pair_ = InsertEdge(vec_hv[1], vec_hv[0]);
	he2_pair_ = InsertEdge(vec_hv[2], vec_hv[1]);
	he3_pair_ = InsertEdge(vec_hv[0], vec_hv[2]);
	he1->pair_ = he1_pair_;
	he1_pair_->pair_ = he1;
	he2->pair_ = he2_pair_;
	he2_pair_->pair_ = he2;
	he3->pair_ = he3_pair_;
	he3_pair_->pair_ = he3;

	if (he1->face_ != NULL || he2->face_ != NULL || he3->face_ != NULL)//if has one been used, mean the triangle is stored in cw direction.
	{
		pface->pedge_ = he1_pair_;
		he1_pair_->pnext_ = he3_pair_;
		he1_pair_->face_ = pface;
		he2_pair_->pnext_ = he1_pair_;
		he2_pair_->face_ = pface;
		he3_pair_->pnext_ = he2_pair_;
		he3_pair_->face_ = pface;

// 		vec_hv[0]->adjHEdges.push_back(he1);
// 		vec_hv[1]->adjHEdges.push_back(he2);
// 		vec_hv[2]->adjHEdges.push_back(he3);
	}
	else
	{
		pface->pedge_ = he1;
		he1->pnext_ = he2;
		he1->face_ = pface;
		he2->pnext_ = he3;
		he2->face_ = pface;
		he3->pnext_ = he1;
		he3->face_ = pface;
// 		vec_hv[0]->adjHEdges.push_back(he3_pair_);
// 		vec_hv[1]->adjHEdges.push_back(he1_pair_);
// 		vec_hv[2]->adjHEdges.push_back(he2_pair_);
	}

	pface->id_ = static_cast<int>(pfaces_list_->size());
	pfaces_list_->push_back(pface);
	return pface;

}

bool MeshBase::LoadFromSTLFile(const char* fins)
{
	//支持中文
	QString filename = QString::fromLocal8Bit(fins);
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{

		std::cerr << "Cannot open file for reading:" << qPrintable(file.errorString()) << std::endl;
		return false;
	}

	ClearData();
	// read ASCII .stl file
	if (file.read(1) == "s")
	{
		//qDebug() << "s" << file.readLine();
		QTextStream inASCII(file.readAll());
		ClearData();//为什么又执行了一次Clear操作
		int NUm_vertex = 0, num_facet = 0;
		std::vector<HE_vert* > s_faceid;

		float3 normal;
		while (!inASCII.atEnd())
		{
			QString temp;
			inASCII >> temp;

			if (temp == "vertex")
			{
				//qDebug() << NUm_vertex;
				//NUm_vertex++;
				HE_vert* hv;
				float3 nvv;
				inASCII >> nvv[0] >> nvv[1] >> nvv[2];
				//now we have a new vertex
				hv = new HE_vert(nvv);
				if (pvertices_list_ == NULL)
				{
					pvertices_list_ = new std::vector<HE_vert*>;
					//pvertices_list_->push_back(hv);
				}
				else
				{
					hv->set_id(static_cast<int>(pvertices_list_->size()));
				}
				//下面的10行 代码实现 等同于 for循环遍历判断结果相同 时所做的操作
				std::set<HE_vert*, comVertex>::iterator  iterVert = input_vertex_list_.insert(hv).first;//
				if ((*iterVert)->id() == pvertices_list_->size())
				{
					pvertices_list_->push_back(hv);
				}
				else
				{
					hv = *iterVert;
				}
				s_faceid.push_back(hv);

			}
			else if (temp == "endfacet")
			{
				if (s_faceid.size() >= 3)
				{
					InsertFace(s_faceid)/*->normal_=normal*/;
					num_facet++;
				}

				s_faceid.clear();
			}
			else if (temp == "normal")
			{
				inASCII >> normal[0] >> normal[1] >> normal[2];
			}
		}

	}

	// read Binary .stl file
	else
	{

		quint32 num_of_face;
		file.seek(80);		//前80bit为文件名
		QDataStream inBinary(file.readAll());
		//qDebug() << file.pos();
		inBinary.setVersion(QDataStream::Qt_5_5);
		inBinary.setFloatingPointPrecision(QDataStream::SinglePrecision);
		inBinary >> num_of_face;
		//qDebug() << num_of_face;
		num_of_face = htonl(num_of_face);
		while (!inBinary.atEnd())
		{
			float normalvector;
			inBinary >> normalvector >> normalvector >> normalvector;//can confirm its correctness
			float3 nvv;
			//qDebug() << sizeof(nvv[0]);
			quint16 info;
			//qDebug() << sizeof(temp);
			std::vector<HE_vert* > s_faceid;
			for (int i = 0; i < 3; i++)
			{
				quint32 temp[3];
				inBinary >> temp[0] >> temp[1] >> temp[2];
				//qDebug() << nvv[0] << nvv[1] << nvv[2];
				//下面的htonl函数的功能,颠倒高低位的存储方式，在机器内部其实还是以小尾顺序存储的，但相对于原来的数会有（小尾->大尾，大尾->小尾）的改变
				temp[0] = htonl(temp[0]);
				temp[1] = htonl(temp[1]);
				temp[2] = htonl(temp[2]);
				nvv[0] = *(float*)&temp[0];
				nvv[1] = *(float*)&temp[1];
				nvv[2] = *(float*)&temp[2];
				//	qDebug() << nvv[0] << nvv[1] << nvv[2];
				//s_faceid.push_back(InsertVertex(nvv));
				HE_vert* hv = new HE_vert(nvv);
				if (pvertices_list_ == NULL)
				{
					pvertices_list_ = new std::vector<HE_vert*>;
					hv->set_id(0);
					//pvertices_list_->push_back(hv);
				}
				else
				{
					hv->set_id(static_cast<int>(pvertices_list_->size()));
				}

				std::set<HE_vert*, comVertex>::iterator  iterVert = input_vertex_list_.insert(hv).first;//get the 
				if ((*iterVert)->id() == pvertices_list_->size())
				{
					pvertices_list_->push_back(hv);
				}
				else
				{
					hv = *iterVert;
				}
				s_faceid.push_back(hv);

			}
			inBinary >> info;
			InsertFace(s_faceid);
			s_faceid.clear();
		}
	}

	file.close();

	return isValid();;
}
