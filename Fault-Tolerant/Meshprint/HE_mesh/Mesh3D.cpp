#include "Mesh3D.h"
#include <string>
#include <fstream>
#include <iostream>
#include <strstream>
#include <xutility>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QTextCodec>
#include <fstream>
#include <iostream>
#include "Mesh3D.h"
#include <winsock.h>
#include <QMatrix4x4>
#include "QQuaternion"
#include "qvector.h"
#include <qmath.h>
#include <QTime>
#include "globalFunctions.h"

#include <iomanip>
#pragma comment(lib,"ws2_32.lib")
#define SWAP(a,b,T) {T tmp=(a); (a)=(b); (b)=tmp;}
#define min(a,b) a<b?a:b
#define max(a,b) a>b?a:b

Mesh3D::Mesh3D(void)
{
	// intialization
	pvertices_list_ = new std::vector<HE_vert*>;
	pfaces_list_ = new std::vector<HE_face*>;
	pedges_list_ = new std::vector<HE_edge*>;
	bheList = new std::vector<HE_edge*>;
	iheList = new std::vector<HE_edge*>;


	//input_vertex_list_ = NULL;
	xmax_ = ymax_ = zmax_ = 1.f;
	xmin_ = ymin_ = zmin_ = -1.f;
	no_loop = 0;
	num_components_ = 0;
	average_edge_length_ = 1.f;
}

void Mesh3D::ClearData(void)
{
	ClearVertex();
	ClearEdges();
	ClearFaces();
	input_vertex_list_.clear();
	edgemap_.clear();
	xmax_ = ymax_ = zmax_ = 1.f;
	xmin_ = ymin_ = zmin_ = -1.f;
	bLoop.clear();
	bheList->clear();
	Tria.clear();
	num = 0;
	for (int i = 0;i < PARTTABLENUM;i++)
	{
		partitionTable_Z[i].clear();
	}
	
}

void Mesh3D::ClearVertex(void)
{

	if (pvertices_list_==NULL)
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

void Mesh3D::ClearEdges(void)
{
	if (pedges_list_ == NULL)
	{
		return;
	}
	else
	{
		for (EDGE_ITER eiter = pedges_list_->begin(); eiter!=pedges_list_->end(); eiter++)
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

void Mesh3D::ClearFaces(void)
{
	if (pfaces_list_==NULL)
	{
		return;
	}
	else
	{
		for (FACE_ITER fiter = pfaces_list_->begin(); fiter!=pfaces_list_->end(); fiter++)
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

HE_vert* Mesh3D::InsertVertex(const Vec3f& v)
{
	HE_vert* pvert = new HE_vert(v);
	if (pvertices_list_ == NULL)
	{
		pvertices_list_ = new std::vector<HE_vert*>;
	}
	pvert->id_ = static_cast<int>(pvertices_list_->size());
	pvertices_list_->push_back(pvert);
	return pvert;
}

HE_vert* Mesh3D::InsertVertex(HE_vert* & v)
{
	
	if (pvertices_list_ == NULL)
	{
		pvertices_list_ = new std::vector<HE_vert*>;
	}
	v->id_ = static_cast<int>(pvertices_list_->size());
	pvertices_list_->push_back(v);
	return v;
}

HE_edge* Mesh3D::InsertEdge(HE_vert* vstart, HE_vert* vend)
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
	pedge->pvert_ = vend;
	pedge->start_ = vstart;
	pedge->pvert_->degree_++;
	vstart->pedge_ = pedge;
	edgemap_[PAIR_VERTEX(vstart, vend)] = pedge;
	pedge->id_ = static_cast<int>(pedges_list_->size());
	pedges_list_->push_back(pedge);

	return pedge;
}

HE_face* Mesh3D::InsertFace(std::vector<HE_vert* >& vec_hv)
{
	int vsize = static_cast<int>(vec_hv.size());
	//if (vsize != 3)
	//{
	//	return NULL;
	//}

	if (pfaces_list_ == NULL)
	{
		pfaces_list_ = new std::vector<HE_face*>;
	}

	HE_face *pface = new HE_face;
	pface->valence_ = vsize;
	//HE_edge *bhe[3];
	HE_edge *he1 = NULL, *he2 = NULL, *he3 = NULL, *he1_pair_ = NULL, *he2_pair_ = NULL, *he3_pair_ = NULL;
	//std::vector<HE_edge*> vec_edges;
	he1 = InsertEdge(vec_hv[0], vec_hv[1]);
	he2 = InsertEdge(vec_hv[1], vec_hv[2]);
	he3 = InsertEdge(vec_hv[2], vec_hv[0]);
	he1_pair_ = InsertEdge(vec_hv[1], vec_hv[0]);
	he2_pair_ = InsertEdge(vec_hv[2], vec_hv[1]);
	he3_pair_ = InsertEdge(vec_hv[0], vec_hv[2]);
	he1->ppair_ = he1_pair_;
	he1_pair_->ppair_ = he1;
	he2->ppair_ = he2_pair_;
	he2_pair_->ppair_ = he2;
	he3->ppair_ = he3_pair_;
	he3_pair_->ppair_ = he3;

	if (he1->pface_ != NULL || he2->pface_ != NULL || he3->pface_ != NULL)//if has one been used, mean the triangle is stored in cw direction.
	{
		//qDebug() << "insert #" << (pfaces_list_->size()) << "facet,he1 has been used in facet:";
		pface->set_selected(SELECTED);
		sss++;
		pface->pedge_ = he1_pair_;
		he1_pair_->pnext_ = he3_pair_;
		he1_pair_->pprev_ = he2_pair_;
		he1_pair_->pface_ = pface;
		he2_pair_->pnext_ = he1_pair_;
		he2_pair_->pprev_ = he3_pair_;
		he2_pair_->pface_ = pface;
		he3_pair_->pnext_ = he2_pair_;
		he3_pair_->pprev_ = he1_pair_;
		he3_pair_->pface_ = pface;
		//qDebug() << pface->pedge_->id() << pface->pedge_->pnext_->id() << pface->pedge_->pnext_->pnext_->id();
		HE_edge *current = pface->pedge_->pnext_->pnext_;
	//	qDebug() << current->pnext_->id();
		vec_hv[0]->adjHEdges.push_back(he1);
		vec_hv[1]->adjHEdges.push_back(he2);
		vec_hv[2]->adjHEdges.push_back(he3);
		//he1->set_boundary_flag(BOUNDARY);
		//he2->set_boundary_flag(BOUNDARY);
		//he3->set_boundary_flag(BOUNDARY);
		//bhe[0] = he1;
		//bhe[1] = he2;
		//bhe[2] = he3;
	}

	else

	{
		pface->pedge_ = he1;
		he1->pnext_ = he2;
		he1->pprev_ = he3;
		he1->pface_ = pface;
		he2->pnext_ = he3;
		he2->pprev_ = he1;
		he2->pface_ = pface;
		he3->pnext_ = he1;
		he3->pprev_ = he2;
		he3->pface_ = pface;
		vec_hv[0]->adjHEdges.push_back(he3_pair_);
		vec_hv[1]->adjHEdges.push_back(he1_pair_);
		vec_hv[2]->adjHEdges.push_back(he2_pair_);
		//he1_pair_->set_boundary_flag(BOUNDARY);
		//he2_pair_->set_boundary_flag(BOUNDARY);
		//he3_pair_->set_boundary_flag(BOUNDARY);
		//bhe[0] = he1_pair_;
		//bhe[1] = he2_pair_;
		//bhe[2] = he3_pair_;
	}

	pface->id_ = static_cast<int>(pfaces_list_->size());
	pfaces_list_->push_back(pface);
	return pface;
	
}

bool Mesh3D::LoadFromOBJFile(const char* fins)//读取obj文件
{
	FILE *pfile = fopen(fins, "r");

	char *tok;
	//char *tok_tok;
	char temp[128];
	
	num = 0;

	try
	{
		ClearData();
		//read vertex
		fseek(pfile, 0, SEEK_SET);
		char pLine[512];

		while (fgets(pLine, 512, pfile))
		{
			if (pLine[0] == 'v' && pLine[1] == ' ')
			{
				Vec3f nvv;
				tok = strtok(pLine, " ");
				for (int i = 0; i < 3; i++)
				{
					tok = strtok(NULL, " ");
					strcpy(temp, tok);
					temp[strcspn(temp, " ")] = 0;
					nvv[i] = (float)atof(temp);
				}
				InsertVertex(nvv);
			}
		}

		//read facets
		fseek(pfile, 0, SEEK_SET);

		while (fgets(pLine, 512, pfile))
		{
			char *pTmp = pLine;
			if (pTmp[0] == 'f')
			{
				std::vector<HE_vert* > s_faceid;

				tok = strtok(pLine, " ");
				while ((tok = strtok(NULL, " ")) != NULL)
				{
					strcpy(temp, tok);
					temp[strcspn(temp, "/")] = 0;
					int id = (int)strtol(temp, NULL, 10) - 1;
					HE_vert* hv = get_vertex(id);
					bool findit = false;
					for (int i = 0; i < (int)s_faceid.size(); i++)
					{
						if (hv == s_faceid[i])	//remove redundant vertex id if it exists
						{
							//	cout << "remove redundant vertex" << endl;
							findit = true;
							break;
						}
					}
					if (findit == false && hv != NULL)
					{
						s_faceid.push_back(hv);
					}
				}
				if ((int)s_faceid.size() >= 3)
				{

					Triangle tri;
					tri._aabb.reset();

					tri.Vertex_1[0] = s_faceid[0]->position().x();
					tri.Vertex_1[1] = s_faceid[0]->position().y();
					tri.Vertex_1[2] = s_faceid[0]->position().z();
					tri._aabb.updateMinMax(tri.Vertex_1);

					tri.Vertex_2[0] = s_faceid[1]->position().x();
					tri.Vertex_2[1] = s_faceid[1]->position().y();
					tri.Vertex_2[2] = s_faceid[1]->position().z();
					tri._aabb.updateMinMax(tri.Vertex_2);

					tri.Vertex_3[0] = s_faceid[2]->position().x();
					tri.Vertex_3[1] = s_faceid[2]->position().y();
					tri.Vertex_3[2] = s_faceid[2]->position().z();
					tri._aabb.updateMinMax(tri.Vertex_3);

					//tri._aabb.expandBoundary();
					tri.setTriId(num);

					Tria.push_back(tri);
					++num;

					InsertFace(s_faceid);
				}
			}
		}
		// 		if (pedges_list_!=NULL)
		// 		{
		// 			for (int i = 0; i < pedges_list_->size(); i++)
		// 			{
		// 				qDebug() << pedges_list_->at(i)->id() << pedges_list_->at(i)->pface_;
		// 			}
		// 		}

		//read texture coords
		fseek(pfile, 0, SEEK_SET);
		std::vector<Vec3f> texCoordsTemp;
		while (fscanf(pfile, "%s", pLine) != EOF)
		{
			if (pLine[0] == 'v' && pLine[1] == 't')
			{
				Vec3f texTemp(0.f, 0.f, 0.f);
				fscanf(pfile, "%f %f", &texTemp[0], &texTemp[1]);
				texCoordsTemp.push_back(texTemp);
			}
		}
		//read texture index

		if (texCoordsTemp.size() > 0)
		{
			fseek(pfile, 0, SEEK_SET);

			int faceIndex = 0;
			while (fscanf(pfile, "%s", pLine) != EOF)
			{

				if (pLine[0] == 'f')
				{
					int v, t;
					fscanf(pfile, "%s", pLine);
					if (sscanf(pLine, "%d/%d", &v, &t) == 2)
					{
						std::map<int, int> v2tex;
						v2tex[v - 1] = t - 1;

						fscanf(pfile, "%s", pLine);
						sscanf(pLine, "%d/%d", &v, &t);
						v2tex[v - 1] = t - 1;

						fscanf(pfile, "%s", pLine);
						sscanf(pLine, "%d/%d", &v, &t);
						v2tex[v - 1] = t - 1;

						HE_edge* edgeTemp = pfaces_list_->at(faceIndex)->pedge_;
						edgeTemp->texCoord_ = texCoordsTemp.at(v2tex[edgeTemp->pvert_->id_]);
						edgeTemp->pvert_->texCoord_ = edgeTemp->texCoord_;
						edgeTemp = edgeTemp->pnext_;
						edgeTemp->texCoord_ = texCoordsTemp.at(v2tex[edgeTemp->pvert_->id_]);
						edgeTemp->pvert_->texCoord_ = edgeTemp->texCoord_;
						edgeTemp = edgeTemp->pnext_;
						edgeTemp->texCoord_ = texCoordsTemp.at(v2tex[edgeTemp->pvert_->id_]);
						edgeTemp->pvert_->texCoord_ = edgeTemp->texCoord_;
						faceIndex++;
					}
				}
			}
		}



		UpdateMesh();
		//Unify(2.f);


		float x_box, y_box, z_box, min_box, max_box, len_z, len_x, len_y;
		Vec3f max = getBoundingBox().at(0);
		Vec3f min = getBoundingBox().at(1);

		x_box = max.x() - min.x();
		y_box = max.y() - min.y();
		z_box = max.z() - min.z();

		//if (mid_box == mif_box)
		//{
		//	len = z_box;
		//}
		//else
		//{
		//	len = y_box;
		//}
		//////////////////////////////////////分割密度
		len_z = z_box / static_cast<float>(PARTTABLENUM);
		len_x = x_box / static_cast<float>(PARTTABLENUM);
		len_y = y_box / static_cast<float>(PARTTABLENUM);

		for (int i = 0;i < Tria.size();i++)
		{
			min_box = min(Tria[i]._aabb._min.z(), Tria[i]._aabb._max.z());
			max_box = max(Tria[i]._aabb._min.z(), Tria[i]._aabb._max.z());

			if (i < 0)
			{
				qDebug() << "ModelMin:" << min.z() << "ModelMax:" << max.z();
				qDebug() << "TriaMin:" << min_box << "TriaMax:" << max_box;
				qDebug() << "min.z() + j*len_z:" << min.z() + i*len_z;
			}


			for (int j = 1;j <= PARTTABLENUM;j++)
			{
				//qDebug() << Tria[i]._aabb._min.z() + 0.01 << Tria[i]._aabb._max.z() + 0.01 << min.z() + j*len;
				//partitionTable_Z[j - 1].push_back(Tria[i]);

				if (min_box - 0.1 < (min.z() + j*len_z))
				{
					partitionTable_Z[j - 1].push_back(&Tria[i]);
					if (max_box + 0.1 < (min.z() + j*len_z))
					{
						break;
					}
				}
			}

		}

		qDebug() << "partitionTable_Z_Z :" << "parttable[0]" << partitionTable_Z[0].size() << "parttable[1]" << partitionTable_Z[1].size();

		num = 0;
		for (int n = 0;n < PARTTABLENUM;n++)
		{
			for (int i = 0;i < partitionTable_Z[n].size();i++)
			{
				for (int j = 0;j < partitionTable_Z[n].size();j++)
				{
					if (i == j)
					{
						continue;
					}
					if (judge_triangle_topologicalStructure(partitionTable_Z[n][i], partitionTable_Z[n][j]) == INTERSECT)
					{
						partitionTable_Z[n][i]->selected = 1;
						partitionTable_Z[n][j]->selected = 1;
						
					//	partitionTable_Z[n][j]->INS.ID = partitionTable_Z[n][j]->getTriId();
						partitionTable_Z[n][j]->INS.insEdgeStartPoint = insStartPoint;
						partitionTable_Z[n][j]->INS.insEdgeEndPoint = insEndPoint;
						partitionTable_Z[n][j]->INS.insPoint.x() = INSPOINT_X;
						partitionTable_Z[n][j]->INS.insPoint.y() = INSPOINT_Y;
						partitionTable_Z[n][j]->INS.insPoint.z() = INSPOINT_Z;

					//	partitionTable_Z[n][i]->triVar.insert(partitionTable_Z[n][j]->INS);
						//break;	INS = new InsFaceIdAndEdge();
					}
				}
			}
		}


		for (int i = 0;i < Tria.size();i++)
		{
			if (Tria[i].selected == 1)
			{
				num++;
			}
			if (i < 20)
			{
				qDebug() << "Triangle ID:" << Tria[i].getTriId();
				//std::set<InsFaceIdAndEdge>::iterator it; //定义前向迭代器  
				//							//中序遍历集合中的所有元素  
				//for (it = Tria[i].triVar.begin(); it != Tria[i].triVar.end(); it++)
				//{
				//	qDebug() << it->ID << " ";
				//	//->ID << " "<<it->insEdgeStartPoint<<"-"<<it->insEdgeEndPoint;
				//}
			}
		}

		qDebug() << "w Tria=" << Tria.size() << "Num" << num << "\n";

		num = Tria.size();


	}
	catch (...)
	{
		ClearData();
		xmax_ = ymax_ = zmax_ = 1.f;
		xmin_ = ymin_ = zmin_ = -1.f;

		fclose(pfile);
		return false;
	}

	fclose(pfile);
	// 	for (EDGE_ITER iter = pedges_list_->begin(); iter != pedges_list_->end(); iter++)
	// 	{
	// 		qDebug() << (*iter)->id() << (*iter)->pface_;
	// 	}
	return isValid();
}

void Mesh3D::MntnMesh(const char* fouts) {
	//qDebug() << "Maintenance Model !";
	std::ofstream fout(fouts);

	//fout << "g object\n";
	//fout.precision(8);
	fout << "solid 3" << "\n";
	for (int i = 0; i < Tri.size(); i++)
	{
		//if (Tria[i].partitionNumber==1)
		//{
		//	continue;
		//}
		//	fout.setf(std::ios::showpoint);
		//科学计数法
		fout << "  facet normal " << std::fixed << std::setprecision(4)<< Tri[i].normal.x()<< "  "<< Tri[i].normal.y() << "  " << Tri[i].normal.z() << "\n";
		fout << "      outer loop" << "\n";
		fout << "         vertex " << std::fixed << std::setprecision(4)<< Tri[i].Vertex_1.x() << "  " << Tri[i].Vertex_1.y() << "  " << Tri[i].Vertex_1.z() << "\n";
		fout << "         vertex " << std::fixed << std::setprecision(4)<< Tri[i].Vertex_2.x() << "  " << Tri[i].Vertex_2.y() << "  " << Tri[i].Vertex_2.z() << "\n";
		fout << "         vertex " << std::fixed << std::setprecision(4)<< Tri[i].Vertex_3.x() << "  " << Tri[i].Vertex_3.y() << "  " << Tri[i].Vertex_3.z() << "\n";
		fout << "      endloop" << "\n";
		fout << "  endfacet" << "\n";
			
	}
	fout << "endsolid" << "\n";
	fout.close();
}

void Mesh3D::TriangleIntersect()
{
	if (Tria.size()==0)
	{
		return ;
	}
	Tri.clear();

	float x_box, y_box, z_box, min_box, max_box, len_z, len_x, len_y;
	Vec3f max = getBoundingBox().at(0);
	Vec3f min = getBoundingBox().at(1);

	x_box = max.x() - min.x();
	y_box = max.y() - min.y();
	z_box = max.z() - min.z();

	//////////////////////////////////////分割密度
	len_z = z_box / static_cast<float>(PARTTABLENUM);
	len_x = x_box / static_cast<float>(PARTTABLENUM);
	len_y = y_box / static_cast<float>(PARTTABLENUM);

	for (int i = 0;i < Tria.size();i++)
	{
		min_box = min(Tria[i]._aabb._min.z(), Tria[i]._aabb._max.z());
		max_box = max(Tria[i]._aabb._min.z(), Tria[i]._aabb._max.z());

		if (i < 0)
		{
			qDebug() << "ModelMin:" << min.z() << "ModelMax:" << max.z();
			qDebug() << "TriaMin:" << min_box << "TriaMax:" << max_box;
			qDebug() << "min.z() + j*len_z:" << min.z() + i*len_z;
		}


		for (int j = 1;j <= PARTTABLENUM;j++)
		{
			//qDebug() << Tria[i]._aabb._min.z() + 0.01 << Tria[i]._aabb._max.z() + 0.01 << min.z() + j*len;
			//partitionTable_Z[j - 1].push_back(Tria[i]);

			if (min_box - 0.1 < (min.z() + j*len_z))
			{
				partitionTable_Z[j - 1].push_back(&Tria[i]);
				if (max_box + 0.1 < (min.z() + j*len_z))
				{
					break;
				}
			}
		}

	}

	//qDebug() << "partitionTable_Z_Z :" << "parttable[0]" << partitionTable_Z[0].size() << "parttable[1]" << partitionTable_Z[1].size();

	num = 0;
	for (int n = 0;n < PARTTABLENUM;n++)
	{
		for (int i = 0;i < partitionTable_Z[n].size();i++)
		{
			for (int j = 0;j < partitionTable_Z[n].size();j++)
			{
				if (i == j)
				{
					continue;
				}
				if (judge_triangle_topologicalStructure(partitionTable_Z[n][i], partitionTable_Z[n][j]) == INTERSECT)
				{
					partitionTable_Z[n][i]->selected = 1;
					partitionTable_Z[n][j]->selected = 1;

					//partitionTable_Z[n][j]->INS.ID = partitionTable_Z[n][j]->getTriId();
					partitionTable_Z[n][j]->INS.insEdgeStartPoint = insStartPoint;
					partitionTable_Z[n][j]->INS.insEdgeEndPoint = insEndPoint;
					partitionTable_Z[n][j]->INS.insRedundantPoint = insRedunPoint;
					partitionTable_Z[n][j]->INS.insPoint.x() = INSPOINT_X;
					partitionTable_Z[n][j]->INS.insPoint.y() = INSPOINT_Y;
					partitionTable_Z[n][j]->INS.insPoint.z() = INSPOINT_Z;

					if (INSPOINT_X != 0)
					{
						partitionTable_Z[n][i]->triPoint.insert(partitionTable_Z[n][j]->INS);//记录此面所有的交点
					}
					//2017/9/26 由此处理问题
					//partitionTable_Z[n][i]->triVar.push_back(*partitionTable_Z[n][j]);
					//break;
				}
			}
		}
	}

	
}

void Mesh3D::WriteToOBJFile(const char* fouts)
{
	std::ofstream fout(fouts);


	fout<<"g object\n";
	fout.precision(8);
	//output coordinates of each vertex
	VERTEX_ITER viter = pvertices_list_->begin();
	for (;viter!=pvertices_list_->end(); viter++) 
	{
		//科学计数法
		fout<<"v "<< std::scientific <<(*viter)->position_.x() 
			<<" "<<(*viter)->position_.y() <<" "<< (*viter)->position_.z() <<"\n";
	}

	// 		for (viter = pvertices_list_->begin();viter!=pvertices_list_->end(); viter++) 
	// 		{
	// 			fout<<"vn "<< std::scientific <<(*viter)->normal_.x() 
	// 				<<" "<<(*viter)->normal_.y() <<" "<<(*viter)->normal_.z() <<"\n";
	// 		}
	//output the valence of each face and its vertices_list' id

	FACE_ITER fiter = pfaces_list_->begin();

	for (;fiter!=pfaces_list_->end(); fiter++) 
	{
		fout<<"f";

		HE_edge* edge = (*fiter)->pedge_; 
		//half data structrue 构建的问题，导致会有缺陷，所以如果没有修复完成，save会引发异常
		do {
			fout<<" "<<edge->ppair_->pvert_->id_+1;
			edge = edge->pnext_;

		} while (edge != (*fiter)->pedge_);
		fout<<"\n";
	}

	qDebug() << pvertices_list_->size();
	fout.close();
}

bool Mesh3D::LoadFromSTLFile(const char* fins)
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
		//ClearData();//为什么又执行了一次Clear操作
		int NUm_vertex = 0, num_facet = 0;
		std::vector<HE_vert* > s_faceid;
		Vec3f normal;
		while (!inASCII.atEnd())
		{
			QString temp,strVar;
			inASCII >> temp;
			if (temp == "normal")
			{
				inASCII >> normal[0] >> normal[1] >> normal[2];
				//数据格式化，保留小数点后4位
				normal[0] = strVar.setNum(normal[0], 'f', 4).toFloat();
				normal[1] = strVar.setNum(normal[1], 'f', 4).toFloat();
				normal[2] = strVar.setNum(normal[2], 'f', 4).toFloat();
			}
			else if (temp == "vertex")
			{
				//qDebug() << NUm_vertex;
				NUm_vertex++;
				HE_vert* hv;
				Vec3f nvv; 
				//inASCII.setRealNumberNotation(QTextStream::FixedNotation);
				//inASCII.setRealNumberPrecision(4);

				inASCII >> nvv[0] >> nvv[1] >> nvv[2];

				//数据格式化，保留小数点后4位
				nvv[0] = strVar.setNum(nvv[0], 'f', 4).toFloat();
				nvv[1] = strVar.setNum(nvv[1], 'f', 4).toFloat();
				nvv[2] = strVar.setNum(nvv[2], 'f', 4).toFloat();

				//nvv[0] = (int)((nvv[0] * 10000) + 0.5) / 10000.0;
				//nvv[1] = (int)((nvv[1] * 10000) + 0.5) / 10000.0;
				//nvv[2] = (int)((nvv[2] * 10000) + 0.5) / 10000.0;

				//qDebug() <<scientific<< nvv[0] << nvv[1] << nvv[2];
				
				//QString::number(nvv[0], 'f', 4);
				
				//now we have a new vertex
				hv = new HE_vert(nvv);
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
				//下面的10行 代码实现 等同于 for循环遍历判断结果相同 时所做的操作
				std::set<HE_vert*, comVertex>::iterator  iterVert = input_vertex_list_.insert(hv).first;//get the 
				if ((*iterVert)->id() == pvertices_list_->size())
					//if (hv->id() != (*iterVert)->id() || hv->id() == pvertices_list_->size())
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
					Triangle tri;
					tri.normal[0] = normal[0];
					tri.normal[1] = normal[1];
					tri.normal[2] = normal[2];

					tri._aabb.reset();

					tri.Vertex_1[0] = s_faceid[0]->position().x();
					tri.Vertex_1[1] = s_faceid[0]->position().y();
					tri.Vertex_1[2] = s_faceid[0]->position().z();
					tri._aabb.updateMinMax(tri.Vertex_1);

					tri.Vertex_2[0] = s_faceid[1]->position().x();
					tri.Vertex_2[1] = s_faceid[1]->position().y();
					tri.Vertex_2[2] = s_faceid[1]->position().z();
					tri._aabb.updateMinMax(tri.Vertex_2);

					tri.Vertex_3[0] = s_faceid[2]->position().x();
					tri.Vertex_3[1] = s_faceid[2]->position().y();
					tri.Vertex_3[2] = s_faceid[2]->position().z();
					tri._aabb.updateMinMax(tri.Vertex_3);

					//tri._aabb.expandBoundary();
					tri.setTriId(num);

					Tria.push_back(tri);
					++num;
					InsertFace(s_faceid)/*->normal_=normal*/;
					num_facet++;
				}
				s_faceid.clear();
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
			Vec3f nvv;
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
			//qDebug() << pfaces_list_->size() << pvertices_list_->size();
		}
	}

	file.close();
	UpdateMesh();
//	Unify(2.0f);
	float x_box, y_box, z_box, min_box, max_box, len_z, len_x, len_y;
	Vec3f max = getBoundingBox().at(0);
	Vec3f min = getBoundingBox().at(1);

	x_box = max.x() - min.x();
	y_box = max.y() - min.y();
	z_box = max.z() - min.z();

	//if (mid_box == mif_box)
	//{
	//	len = z_box;
	//}
	//else
	//{
	//	len = y_box;
	//}
	//////////////////////////////////////分割密度
	len_z = z_box / static_cast<float>(PARTTABLENUM);
	len_x = x_box / static_cast<float>(PARTTABLENUM);
	len_y = y_box / static_cast<float>(PARTTABLENUM);

	for (int i = 0;i < Tria.size();i++)
	{
		min_box = min(Tria[i]._aabb._min.z(), Tria[i]._aabb._max.z());
		max_box = max(Tria[i]._aabb._min.z(), Tria[i]._aabb._max.z());

		if (i < 0)
		{
			qDebug() << "ModelMin:" << min.z() << "ModelMax:" << max.z();
			qDebug() << "TriaMin:" << min_box << "TriaMax:" << max_box;
			qDebug() << "min.z() + j*len_z:" << min.z() + i*len_z;
		}


		for (int j = 1;j <= PARTTABLENUM;j++)
		{
			//qDebug() << Tria[i]._aabb._min.z() + 0.01 << Tria[i]._aabb._max.z() + 0.01 << min.z() + j*len;
			//partitionTable_Z[j - 1].push_back(Tria[i]);

			if (min_box - 0.1 < (min.z() + j*len_z))
			{
				partitionTable_Z[j - 1].push_back(&Tria[i]);
				if (max_box + 0.1 < (min.z() + j*len_z))
				{
					break;
				}
			}
		}

	}

	qDebug() << "partitionTable_Z_Z :" << "parttable[0]" << partitionTable_Z[0].size() << "parttable[1]" << partitionTable_Z[1].size();

	num = 0;
	for (int n = 0;n < PARTTABLENUM;n++)
	{
		for (int i = 0;i < partitionTable_Z[n].size();i++)
		{
			for (int j = 0;j < partitionTable_Z[n].size();j++)
			{
				if (i == j)
				{
					continue;
				}
				if (judge_triangle_topologicalStructure(partitionTable_Z[n][i], partitionTable_Z[n][j]) == INTERSECT)
				{
					partitionTable_Z[n][i]->selected = 1;
					partitionTable_Z[n][j]->selected = 1;

					//partitionTable_Z[n][j]->INS.ID = partitionTable_Z[n][j]->getTriId();
					partitionTable_Z[n][j]->INS.insEdgeStartPoint = insStartPoint;
					partitionTable_Z[n][j]->INS.insEdgeEndPoint = insEndPoint;
					partitionTable_Z[n][j]->INS.insRedundantPoint = insRedunPoint;
					partitionTable_Z[n][j]->INS.insPoint.x() = INSPOINT_X;
					partitionTable_Z[n][j]->INS.insPoint.y() = INSPOINT_Y;
					partitionTable_Z[n][j]->INS.insPoint.z() = INSPOINT_Z;

					if (INSPOINT_X != 0)
					{
						partitionTable_Z[n][i]->triPoint.insert(partitionTable_Z[n][j]->INS);//记录此面所有的交点
					}
					//2017/9/26 由此处理问题
					//partitionTable_Z[n][i]->triVar.push_back(*partitionTable_Z[n][j]);
					//break;
				}
			}
		}
	}

	num = Tria.size();
	for (int i = 0;i < num;i++)
	{
		//if (Tria[i].selected == 1)
		//{
		//	num++;
		//}
		if (i >= 0)
		{
			if (Tria[i].triPoint.size() == 0)
			{
				continue;
			}
			else
			{
				Tria[i].partitionNumber = 1;//标记，后续处理时应该删除该面片
			}
			qDebug() << "Triangle ID:" << Tria[i].getTriId();

			std::set<InsFaceIdAndEdge>::iterator it; //定义前向迭代器  
													 //中序遍历集合中的所有元素  
			std::vector<Triangle>::iterator it_var;

			int TriNum = 0;
			Triangle tri;
			for (it = Tria[i].triPoint.begin();it != Tria[i].triPoint.end();it++)
			{
				qDebug() << it->insEdgeStartPoint << "->" << it->insEdgeEndPoint << " | " << it->insRedundantPoint << "(" << it->insPoint.x() << "," << it->insPoint.y() << "," << it->insPoint.z() << ")";
				if (it == Tria[i].triPoint.begin())
				{
					qDebug() << "(" << Tria[i].mappingValue(it->insEdgeStartPoint).x() << "," << Tria[i].mappingValue(it->insEdgeStartPoint).y() << "," << Tria[i].mappingValue(it->insEdgeStartPoint).z() << ")" << "->" << "(" << Tria[i].mappingValue(it->insEdgeEndPoint).x() << "," << Tria[i].mappingValue(it->insEdgeEndPoint).y() << "," << Tria[i].mappingValue(it->insEdgeEndPoint).z() << ")" << " | " << "(" << Tria[i].mappingValue(it->insRedundantPoint).x() << "," << Tria[i].mappingValue(it->insRedundantPoint).y() << "," << Tria[i].mappingValue(it->insRedundantPoint).z() << ")";

					//自身三角面片分割
					tri.Vertex_1 = Tria[i].mappingValue(it->insRedundantPoint);
					tri.Vertex_2 = Tria[i].mappingValue(it->insEdgeStartPoint);
					tri.Vertex_3 = it->insPoint;
					Tria[i].triVar.push_back(tri);
					qDebug() << "---------------begin";
					qDebug() << "tri_Vertex_1:(x,y,z)" << "(" << tri.Vertex_1.x() << "," << tri.Vertex_1.y() << "," << tri.Vertex_1.z() << ")";
					qDebug() << "tri_Vertex_2:(x,y,z)" << "(" << tri.Vertex_2.x() << "," << tri.Vertex_2.y() << "," << tri.Vertex_2.z() << ")";
					qDebug() << "tri_Vertex_3:(x,y,z)" << "(" << tri.Vertex_3.x() << "," << tri.Vertex_3.y() << "," << tri.Vertex_3.z() << ")";
					tri.Vertex_1 = Tria[i].mappingValue(it->insRedundantPoint);
					tri.Vertex_2 = it->insPoint;
					tri.Vertex_3 = Tria[i].mappingValue(it->insEdgeEndPoint);
					Tria[i].triVar.push_back(tri);
					qDebug() << "tri_Vertex_1:(x,y,z)" << "(" << tri.Vertex_1.x() << "," << tri.Vertex_1.y() << "," << tri.Vertex_1.z() << ")";
					qDebug() << "tri_Vertex_2:(x,y,z)" << "(" << tri.Vertex_2.x() << "," << tri.Vertex_2.y() << "," << tri.Vertex_2.z() << ")";
					qDebug() << "tri_Vertex_3:(x,y,z)" << "(" << tri.Vertex_3.x() << "," << tri.Vertex_3.y() << "," << tri.Vertex_3.z() << ")";
					qDebug() << "-----------End begin";
					continue;
				}

				for (int j = 0;j < Tria[i].triVar.size();j++)
				{
					qDebug() << "(" << Tria[i].triVar[j].mappingValue(it->insEdgeStartPoint).x() << "," << Tria[i].triVar[j].mappingValue(it->insEdgeStartPoint).y() << "," << Tria[i].triVar[j].mappingValue(it->insEdgeStartPoint).z() << ")" << "->" << "(" << Tria[i].triVar[j].mappingValue(it->insEdgeEndPoint).x() << "," << Tria[i].triVar[j].mappingValue(it->insEdgeEndPoint).y() << "," << Tria[i].triVar[j].mappingValue(it->insEdgeEndPoint).z() << ")" << " | " << "(" << Tria[i].triVar[j].mappingValue(it->insRedundantPoint).x() << "," << Tria[i].triVar[j].mappingValue(it->insRedundantPoint).y() << "," << Tria[i].triVar[j].mappingValue(it->insRedundantPoint).z() << ")";

					if (is_pointTri_within_triangle(&Tria[i].triVar[j], it->insPoint))
					{

						//自身三角面片分割
						tri.Vertex_1 = Tria[i].triVar[j].mappingValue(it->insRedundantPoint);
						tri.Vertex_2 = Tria[i].triVar[j].mappingValue(it->insEdgeStartPoint);
						tri.Vertex_3 = it->insPoint;
						Tria[i].triVar.push_back(tri);
						qDebug() << "---------------for--triVar";

						qDebug() << "tri_Vertex_1:(x,y,z)" << "(" << tri.Vertex_1.x() << "," << tri.Vertex_1.y() << "," << tri.Vertex_1.z() << ")";
						qDebug() << "tri_Vertex_2:(x,y,z)" << "(" << tri.Vertex_2.x() << "," << tri.Vertex_2.y() << "," << tri.Vertex_2.z() << ")";
						qDebug() << "tri_Vertex_3:(x,y,z)" << "(" << tri.Vertex_3.x() << "," << tri.Vertex_3.y() << "," << tri.Vertex_3.z() << ")";

						tri.Vertex_1 = Tria[i].triVar[j].mappingValue(it->insRedundantPoint);
						tri.Vertex_2 = it->insPoint;
						tri.Vertex_3 = Tria[i].triVar[j].mappingValue(it->insEdgeEndPoint);
						Tria[i].triVar.push_back(tri);
						//delete it_var
						qDebug() << "tri_Vertex_1:(x,y,z)" << "(" << tri.Vertex_1.x() << "," << tri.Vertex_1.y() << "," << tri.Vertex_1.z() << ")";
						qDebug() << "tri_Vertex_2:(x,y,z)" << "(" << tri.Vertex_2.x() << "," << tri.Vertex_2.y() << "," << tri.Vertex_2.z() << ")";
						qDebug() << "tri_Vertex_3:(x,y,z)" << "(" << tri.Vertex_3.x() << "," << tri.Vertex_3.y() << "," << tri.Vertex_3.z() << ")";
						//it_var = Tria[i].triVar.erase(it_var);

						qDebug() << "-----------End----for--triVar";

						Tria[i].triVar[j].partitionNumber = 1;//标记，后续处理时应该删除该面片
						break;
					}
				}


				//for (it_var=Tria[i].triVar.begin();it_var!=Tria[i].triVar.end();it_var++)
				//{
				//	qDebug() << "(" << Tria[i].mappingValue(it->insEdgeStartPoint).x() << "," << Tria[i].mappingValue(it->insEdgeStartPoint).y() << "," << Tria[i].mappingValue(it->insEdgeStartPoint).z() << ")" << "->" << "(" << Tria[i].mappingValue(it->insEdgeEndPoint).x() << "," << Tria[i].mappingValue(it->insEdgeEndPoint).y() << "," << Tria[i].mappingValue(it->insEdgeEndPoint).z() << ")" << " | " << "(" << Tria[i].mappingValue(it->insRedundantPoint).x() << "," << Tria[i].mappingValue(it->insRedundantPoint).y() << "," << Tria[i].mappingValue(it->insRedundantPoint).z() << ")";

				//	if (is_pointTri_within_triangle(&*it_var,it->insPoint))
				//	{
				//		//自身三角面片分割
				//		tri.Vertex_1 = Tria[i].mappingValue(it->insRedundantPoint);
				//		tri.Vertex_2 = Tria[i].mappingValue(it->insEdgeStartPoint);
				//		tri.Vertex_3 = it->insPoint;
				//		Tria[i].triVar.push_back(tri);
				//		qDebug() << "---------------for--triVar";

				//		qDebug() << "tri_Vertex_1:(x,y,z)" << "(" << tri.Vertex_1.x() << "," << tri.Vertex_1.y() << "," << tri.Vertex_1.z() << ")";
				//		qDebug() << "tri_Vertex_2:(x,y,z)" << "(" << tri.Vertex_2.x() << "," << tri.Vertex_2.y() << "," << tri.Vertex_2.z() << ")";
				//		qDebug() << "tri_Vertex_3:(x,y,z)" << "(" << tri.Vertex_3.x() << "," << tri.Vertex_3.y() << "," << tri.Vertex_3.z() << ")";

				//		tri.Vertex_1 = Tria[i].mappingValue(it->insRedundantPoint);
				//		tri.Vertex_2 = it->insPoint;
				//		tri.Vertex_3 = Tria[i].mappingValue(it->insEdgeEndPoint);
				//		Tria[i].triVar.push_back(tri);
				//		//delete it_var
				//		qDebug() << "tri_Vertex_1:(x,y,z)" << "(" << tri.Vertex_1.x() << "," << tri.Vertex_1.y() << "," << tri.Vertex_1.z() << ")";
				//		qDebug() << "tri_Vertex_2:(x,y,z)" << "(" << tri.Vertex_2.x() << "," << tri.Vertex_2.y() << "," << tri.Vertex_2.z() << ")";
				//		qDebug() << "tri_Vertex_3:(x,y,z)" << "(" << tri.Vertex_3.x() << "," << tri.Vertex_3.y() << "," << tri.Vertex_3.z() << ")";
				//		//it_var = Tria[i].triVar.erase(it_var);

				//		qDebug() << "-----------End----for--triVar";
				//		break;
				//	}
				//}

			}


			//for (it = Tria[i].triVar.begin(); it != Tria[i].triVar.end(); it++)
			//{
			//	qDebug() << it->getTriId();
			//	
			//	for (int j=0;j<Tria[i].triPoint.size();j++)
			//	{
			//		qDebug()<<"(" << Tria[i].triPoint[j].x() << "," << Tria[i].triPoint[j].y() << "," << Tria[i].triPoint[j].z() << ")";
			//	}

			//	//if (it->insEdgeStartPoint == 0) //排除不是两边相交的点
			//	//{
			//	//	continue;
			//	//}
			//	//for (it_var=Tria[it->ID].triVar.begin();it_var!=Tria[i].triVar.end();it_var++)
			//	//{
			//	//	if (it_var->ID!=i)
			//	//	{
			//	//		continue;
			//	//	}
			//	//	Triangle tri;
			//	//	////左上角 小三角面片
			//	//	//tri.setTriId(Tria.size());
			//	//	//tri.Vertex_1 = Tria[it_var->ID].mappingValue(it_var->insEdgeStartPoint);
			//	//	//tri.Vertex_2 = Tria[it->ID].mappingValue(it->insEdgeStartPoint);
			//	//	//tri.Vertex_3 = it_var->insPoint;
			//	//	//Tria.push_back(tri);

			//	//	////右下角 小三角面片
			//	//	//tri.setTriId(Tria.size());
			//	//	//tri.Vertex_1 = Tria[it_var->ID].mappingValue(it_var->insEdgeEndPoint);
			//	//	//tri.Vertex_2 = Tria[it->ID].mappingValue(it->insEdgeEndPoint);
			//	//	//tri.Vertex_3 = it_var->insPoint;
			//	//	//Tria.push_back(tri);

			//	//	//自身三角面片分割
			//	//	tri.setTriId(TriNum++);
			//	//	tri.Vertex_1 = Tria[it->ID].mappingValue(it->insEdgeStartPoint);
			//	//	tri.Vertex_2 = Tria[it->ID].mappingValue(it->insRedundantPoint);
			//	//	tri.Vertex_3 = it_var->insPoint;
			//	//	Tri.push_back(tri);

			//	//	tri.setTriId(TriNum++);
			//	//	tri.Vertex_1 = Tria[it->ID].mappingValue(it->insEdgeEndPoint);
			//	//	tri.Vertex_2 = Tria[it->ID].mappingValue(it->insRedundantPoint);
			//	//	tri.Vertex_3 = it_var->insPoint;
			//	//	Tri.push_back(tri);

			//	//	//上面的小三角面片分割
			//	//	tri.setTriId(TriNum++);
			//	//	tri.Vertex_1 = Tria[it_var->ID].mappingValue(it_var->insEdgeStartPoint);
			//	//	tri.Vertex_2 = Tria[it_var->ID].mappingValue(it_var->insRedundantPoint);
			//	//	tri.Vertex_3 = it_var->insPoint;
			//	//	Tri.push_back(tri);

			//	//	tri.setTriId(TriNum);
			//	//	tri.Vertex_1 = Tria[it_var->ID].mappingValue(it_var->insEdgeEndPoint);
			//	//	tri.Vertex_2 = Tria[it_var->ID].mappingValue(it_var->insRedundantPoint);
			//	//	tri.Vertex_3 = it_var->insPoint;
			//	//	Tri.push_back(tri);
			//	//	break;
			//	//}
			//	//Triangle tri;
			//	//tri.setTriId(Tria.size());
			//	//tri.Vertex_1 = Tria[it->ID].mappingValue(it->insEdgeStartPoint);
			//	//tri.Vertex_2 = Tria[it->ID].mappingValue(it->insEdgeEndPoint);
			//	//tri.Vertex_3 = it->insPoint;
			//	//Tria.push_back(tri);
			//}

			//for (int j=0;j<Tria.size();j++)
			//{
			//	if (!Tria[j].triVar.empty())
			//	{
			//		Tria[j].partitionNumber = 1;
			//	}
			//}

		}
	}

	for (int i = 0;i < Tria.size();i++)
	{
		if (Tria[i].triVar.size() != 0)
		{
			for (int j = 0;j < Tria[i].triVar.size();j++)
			{
				if (Tria[i].triVar[j].partitionNumber != 1)
				{
					Tri.push_back(Tria[i].triVar[j]);
				}
			}
		}

		if (Tria[i].partitionNumber != 1)
		{
			Tri.push_back(Tria[i]);
		}
	}


	qDebug() << "w Tria=" << Tria.size() << "Num" << num << "\n";
	num = Tria.size();

	return isValid();
}

void Mesh3D::UpdateMesh(void)
{
	ComputeBoundingBox();//对vertices做出一些限制10E10,10E-10，取得 max,min
	SetBoundaryFlag();//遍历所有面，对不表示任何面的边打上标记，记录到bheList中
	BoundaryCheck();
	countBoundaryComponat();
	UpdateNormal();
	ComputeAvarageEdgeLength();
	if (input_vertex_list_.size() == 0)
		SetNeighbors();
	//Unify(1.5);
}

void Mesh3D::SetBoundaryFlag(void)
{
	if (bheList==NULL)
	{
		bheList = new std::vector<HE_edge *>;
	}
	else
	{
		bheList->clear();
	}
	for (EDGE_ITER eiter = pedges_list_->begin(); eiter != pedges_list_->end(); eiter++)
	{
		if ((*eiter)->pface_ == NULL)
		{
			(*eiter)->set_boundary_flag(BOUNDARY);
			(*eiter)->ppair_->set_boundary_flag(BOUNDARY);
			(*eiter)->pvert_->set_boundary_flag(BOUNDARY);
			(*eiter)->ppair_->pvert_->set_boundary_flag(BOUNDARY);
			(*eiter)->ppair_->pface_->set_boundary_flag(BOUNDARY);
			bheList->push_back(*eiter);
		}
	}
}

//将处在边界上的点，指到边界边上
void Mesh3D::BoundaryCheck()
{
	for (VERTEX_ITER viter=pvertices_list_->begin(); viter!=pvertices_list_->end(); viter++)
	{
		if ((*viter)->isOnBoundary())
		{
			HE_edge* edge = (*viter)->pedge_;
			int deg = 0;
			while (edge->pface_!=NULL && deg<(*viter)->degree())
			{
				edge = edge->pprev_->ppair_;
				deg ++;
			}
			(*viter)->pedge_ = edge;//将处在边界上的点，指到边界边上
		}
	}
}

void Mesh3D::countBoundaryComponat()
{
	if (bLoop.size()!=0)
	{
		return;
		bLoop.clear();
	}
	else
	{
		bLoop.resize(no_loop + 1);//initial the vectro bloop
	}
	//count the number of boundary loops
	size_t i;
	for (i = 0; i < bheList->size(); i++)
	{
		HE_edge *cur = bheList->at(i);
		HE_edge *nex = cur;
		while (nex->start_->selected() != SELECTED)
		{
			bLoop[no_loop].push_back(nex);
			nex->start_->set_seleted(SELECTED);
			nex = nex->pvert_->pedge_;
			if (nex == cur)
			{
				no_loop++;
				bLoop.resize(no_loop+1);
				break;
			}
		}
	}
	for (i = 0; i < bheList->size(); i++)
	{
		bheList->at(i)->start_->set_seleted(UNSELECTED);
	}
	bLoop.resize(no_loop);
	bLoop;
}

void Mesh3D::UpdateNormal(void)
{
	ComputeFaceslistNormal();
	ComputeVertexlistNormal();
}

void Mesh3D::ComputeFaceslistNormal(void)
{
	for (FACE_ITER fiter = pfaces_list_->begin(); fiter != pfaces_list_->end(); fiter++)
	{
		//if ((*fiter)->normal().x() == 0 && (*fiter)->normal().y() == 0 && (*fiter)->normal().z() == 0)
		{	
			ComputePerFaceNormal(*fiter);
		}
	
	}
}

void Mesh3D::ComputePerFaceNormal(HE_face* hf)
{
	HE_edge *pedge = hf->pedge_;
	HE_edge *nedge = hf->pedge_->pnext_;

	HE_vert *p = pedge->pvert_;
	HE_vert *c = pedge->pnext_->pvert_;
	HE_vert *n = nedge->pnext_->pvert_;

	Vec3f pc, nc;
	pc = p->position_ - c->position_;
	nc = n->position_ - c->position_;

	hf->normal_ = nc ^ pc;	// cross prodoct
	hf->normal_.normalize();
	//qDebug() << "normal compute";
}

void Mesh3D::ComputeVertexlistNormal(void)
{
	for (VERTEX_ITER viter = pvertices_list_->begin(); viter!=pvertices_list_->end(); viter++) 
	{
		ComputePerVertexNormal(*viter);
	}
}

void Mesh3D::ComputePerVertexNormal(HE_vert* hv)
{
	if (hv->degree_ < 2)
	{
		// ERROR: the degree of the vertex is less than 2
		hv->normal_ = Vec3f(1.f,0.f,0.f);
		return;
	}

	HE_edge *edge = hv->pedge_;
	if (edge == NULL)
	{
		// ERROR: the edge attached to the vertex is NULL
		hv->normal_ = Vec3f(1.f,0.f,0.f);
		return;
	}

	hv->normal_ = Vec3f(0.f,0.f,0.f);
	if (hv->boundary_flag_ == INNER)
	{
		int iterNum = 0;
		do
		{
			iterNum++;
			if (iterNum > hv->degree())
			{
				/*hv->set_position(hv->position() * 1.1f);*/
				//std::cout << "    iterNum > hv->degree : " << hv->id() << "\n";
				break;
			}
			//hv->normal_ = hv->normal_ + edge->pface_->normal_;
			Vec3f  p = edge->pvert_->position(),
				q = edge->pnext_->pvert_->position(),
				r = edge->pprev_->pvert_->position();
			Vec3f  n = (q-p) ^ (r-p);
			hv->normal_ = hv->normal_ + n;
			edge = edge->ppair_->pnext_;
		} while (edge != hv->pedge_ && edge != NULL);
	}
	else
	{
		// NOTE: for the boundary vertices, this part may be something wrong
		//	     Up to now, define the normals all as unity
		hv->normal_ = Vec3f(1.f, 0.f, 0.f);

		//int degree_flag = 0;
		//for (int i=0; i<hv->degree_-1; i++)
		//{
		//	edge = edge->ppair_->pnext_;
		//	if (edge == NULL)
		//	{
		//		// ERROR: the algorithm of computing boundary vertices has errors!
		//		break;
		//	}
		//	if (edge->pface_ != NULL)
		//	{
		//		hv->normal_ = hv->normal_ + edge->pface_->normal_;
		//	}
		//}
	}
	hv->normal_.normalize();
}
//构建model的边界点（包围盒 2 point）
void Mesh3D::ComputeBoundingBox(void)
{
	if (pvertices_list_->size() < 3)
	{
		return;
	}

#define MAX_FLOAT_VALUE (static_cast<float>(10e10))
#define MIN_FLOAT_VALUE	(static_cast<float>(-10e10))
	
	xmax_ = ymax_ = zmax_ = MIN_FLOAT_VALUE;
	xmin_ = ymin_ = zmin_ = MAX_FLOAT_VALUE;

	VERTEX_ITER viter = pvertices_list_->begin();
	for (; viter!=pvertices_list_->end(); viter++)
	{
		xmin_ = min(xmin_, (*viter)->position_.x());
		ymin_ = min(ymin_, (*viter)->position_.y());
		zmin_ = min(zmin_, (*viter)->position_.z());
		xmax_ = max(xmax_, (*viter)->position_.x());
		ymax_ = max(ymax_, (*viter)->position_.y());
		zmax_ = max(zmax_, (*viter)->position_.z());
	}
}

//重置模型位置
void Mesh3D::Unify(float size)
{
	//qDebug() << "z position" << zmax_;
	float scaleX = xmax_ - xmin_;
	float scaleY = ymax_ - ymin_;
	float scaleZ = zmax_ - zmin_;
	float scaleMax;

	if (scaleX < scaleY)
	{
		scaleMax = scaleY;
	}
	else
	{
		scaleMax = scaleX;
	}
	if (scaleMax < scaleZ)
	{
		scaleMax = scaleZ;
	}
 	//scaleV = size / scaleMax;
 	//scaleT = scaleV;
 	//scaleV = 1;
	Vec3f centerPos((xmin_ +xmax_)/2.0, (ymin_+ymax_)/2.0, (zmin_));
	//Vec3f centerPos((xmin_ + xmax_) / 2.0, (ymin_ + ymax_) / 2.0, (zmin_+zmax_)/2.0);
	for (size_t i = 0; i != pvertices_list_->size(); i++)
	{
		pvertices_list_->at(i)->position_ = (pvertices_list_->at(i)->position_ - centerPos)*scaleT;
	}
	//	update bounding box and average edge length
	ComputeBoundingBox();
	ComputeAvarageEdgeLength();
}

void Mesh3D::ComputeAvarageEdgeLength(void)
{
	if(!isValid())
	{
		average_edge_length_ = 0.f;
		return;
	}
	float aveEdgeLength = 0.f;
	for (int i=0; i<num_of_half_edges_list(); i++)
	{
		HE_edge* edge = get_edges_list()->at(i);
		HE_vert* v0 = edge->pvert_;
		HE_vert* v1 = edge->ppair_->pvert_;
		aveEdgeLength += (v0->position() - v1->position()).length();
	}
	average_edge_length_ = aveEdgeLength/num_of_half_edges_list();
	//std::cout << "Average_edge_length = " << average_edge_length_ << "\n";
}

HE_face* Mesh3D::get_face(int vId0, int vId1, int vId2)
{
	HE_vert *v0 = get_vertex(vId0);
	HE_vert *v1 = get_vertex(vId1);
	HE_vert *v2 = get_vertex(vId2);
	if (!v0 || !v1 || !v2)
	{
		return NULL;
	}

	HE_face* face=NULL;

	// 由于对边界点的邻域遍历有bug，所以找到非边界点进行邻域遍历
	if (v0->isOnBoundary())
	{
		if (!v1->isOnBoundary())
		{
			SWAP(v0, v1, HE_vert*);
		}
		else if (!v2->isOnBoundary())
		{
			SWAP(v0, v2, HE_vert*);
		}
		else
		{
			// v0, v1, v2 都是边界点
			// 暂时先不处理
			return NULL;
		}
	}

	if (!v0->isOnBoundary())	// 对边界点的遍历有bug
	{
		HE_edge* edge=v0->pedge_;
		bool inFace = true;
		do 
		{
			bool b1 = isFaceContainVertex(edge->pface_, v1);
			bool b2 = isFaceContainVertex(edge->pface_, v2);
			if (!b1 && !b1)
			{
				edge = edge->ppair_->pnext_;
			}
			else if(b1 && b2)
			{
				face = edge->pface_;
				break;
			}
			else
			{
				inFace = false;
				break;
			}
		} while (edge!=v0->pedge_ && edge!=NULL);
	}

	return face;
}

HE_face* Mesh3D::get_face(const std::vector<unsigned int>& ids)
{
	if (ids.size()<3)
	{
		std::cout << "查询点数过少，无法返回面\n";
		return NULL;
	}
	// 首先找到一个非边界点
	HE_vert* v = NULL;
	for (unsigned int i=0; i<ids.size(); i++)
	{
		if (!get_vertex(ids[i])->isOnBoundary())
		{
			v = get_vertex(ids[i]);
			break;
		}
	}
	if (!v)
	{
		// 所有点都是边界点
		// 暂不处理
		return NULL;
	}

	HE_edge *edge = v->pedge_;
	HE_face *face = NULL;
	do 
	{
		face = edge->pface_;
		edge = edge->ppair_->pnext_;
		bool bInFace = isFaceContainVertex(face, get_vertex(ids[0]));
		if (!bInFace)
		{
			continue;
		}
		for (unsigned int i=1; i<ids.size(); i++)
		{
			bool b = isFaceContainVertex(face, get_vertex(ids[i]));
			if (b!=bInFace)
			{
				bInFace = false;
				break;
			}
		}
		if (bInFace)
		{
			return face;
		}
	} while (edge!=v->pedge_ && edge!=NULL);
	return NULL;
}

bool Mesh3D::isFaceContainVertex(HE_face* face, HE_vert* vert)
{
	HE_edge* edge = face->pedge_;
	do 
	{
		if (edge->pvert_==vert)
		{
			return true;
		}
		edge = edge->pnext_;
	} while (edge!=face->pedge_ && edge!=NULL);
	return false;
}

void Mesh3D::Transformation(float * matrix)
{
	if (pvertices_list_ != NULL)
	{
		for (VERTEX_ITER iter = pvertices_list_->begin(); iter != pvertices_list_->end(); iter++)
		{
			Vec4f temp((*iter)->position().x(), (*iter)->position().y(), (*iter)->position().z(), 1);
			(*iter)->position_[0] = matrix[0] * temp[0] + matrix[1] * temp[1] + matrix[2] * temp[2] + matrix[3] * temp[3];
			(*iter)->position_[1] = matrix[4] * temp[0] + matrix[5] * temp[1] + matrix[6] * temp[2] + matrix[7] * temp[3];
			(*iter)->position_[2] = matrix[8] * temp[0] + matrix[9] * temp[1] + matrix[10] * temp[2] + matrix[11] * temp[3];
					
		}
	}
	ComputeBoundingBox();
	//Unify(2.0);

}

void Mesh3D::SetDirection(int faceid)
{
	if (faceid==-1)
	{
		return;
	}
	QVector3D normal_(pfaces_list_->at(faceid)->normal().x(), pfaces_list_->at(faceid)->normal().y(), pfaces_list_->at(faceid)->normal().z());
	QVector3D center_(pfaces_list_->at(faceid)->center().x(), pfaces_list_->at(faceid)->center().y(), pfaces_list_->at(faceid)->center().z());
	QVector3D print_dir_(0.0,0.0,-1.0);

	
	float cosTheta =QVector3D::dotProduct(normal_, print_dir_);
	if (fabs(cosTheta - 1.0) <= 1.0e-6)
		cosTheta =1;//0
	else if (fabs(cosTheta + 1.0) <= 1.0e-6)
		cosTheta = -1;//180
	
	//qDebug() << normal_ << print_dir_<<qAcos(cosTheta) * 180 / qAcos(-1) <<qAcos(-1) * 180 / qAcos(-1);
	QMatrix4x4  matrix_;
	QVector3D rotationAxis;
	if (cosTheta == -1)
	{
		rotationAxis = QVector3D(1.0, 0.0, 0.0);
	}
	else if (cosTheta==1)
	{
		rotationAxis = QVector3D(1.0, 0.0, 0.0);
	}
	else
	{
		rotationAxis = QVector3D::crossProduct(normal_, print_dir_);
	}
	matrix_.setToIdentity();
	matrix_.rotate(qAcos(cosTheta) * 180 / qAcos(-1), rotationAxis);
	// 		QVector3D temp = matrix_*normal_;
	// 		if ((temp-print_dir_).length()> 1.0e-6)
	// 		{
	// 			matrix_.setToIdentity();
	// 			matrix_.rotate((2*qAcos(-1) - qAcos(cosTheta)) * 180 / qAcos(-1), rotationAxis);
	// 		}
	// 		temp = matrix_*normal_;


	for (VERTEX_ITER iter=pvertices_list_->begin();iter!=pvertices_list_->end();iter++)
	{
		
		QVector4D position_((*iter)->position().x(), (*iter)->position().y(), (*iter)->position().z(), 1.0);
		position_ = matrix_*position_;
		(*iter)->position().x() = position_[0];
		(*iter)->position().y() = position_[1];
		(*iter)->position().z() = position_[2];
	}
}

void Mesh3D::ClearSlice()
{
	throw std::logic_error("The method or operation is not implemented.");
}


int Mesh3D::GetFaceId(HE_face* face)
{
	return !face ? -1 : face->id();
}

void Mesh3D::ResetFaceSelectedTags(int tag)
{
	for (int i=0; i<num_of_face_list(); i++)
	{
		get_face(i)->set_selected(tag);
	}
}

void Mesh3D::ResetVertexSelectedTags(int tag)
{
	for (int i=0; i<num_of_vertex_list(); i++)
	{
		get_vertex(i)->set_seleted(tag);
	}
}

bool Mesh3D::isNeighbors(HE_vert* v0, HE_vert* v1)
{
	if (!v0 || !v1)
	{
		return false;
	}

	HE_edge *edge = v0->pedge_;
	do 
	{
		if (edge->pvert_==v1)
		{
			return true;
		}
		edge = edge->ppair_->pnext_;
	} while (edge!=v0->pedge_ && edge);
	return false;
}

int Mesh3D::GetSelectedVrtId()
{
	if (!isValid())
	{
		return -1;
	}
	for (int i=0; i<num_of_vertex_list(); i++)
	{
		if (get_vertex(i)->selected()==SELECTED)
		{
			return i;
		}
	}
	return -1;
}

void Mesh3D::CreateMesh(const std::vector<Vec3f>& verts, const std::vector<int>& triIdx)
{
	ClearData();
	for (unsigned int i=0; i<verts.size(); i++)
	{
		InsertVertex(verts[i]);
	}
	for (unsigned int i=0; i<triIdx.size(); i=i+3)
	{
		std::vector<HE_vert*> tri;
		HE_vert *v0 = get_vertex(triIdx[i]);
		HE_vert *v1 = get_vertex(triIdx[i+1]);
		HE_vert *v2 = get_vertex(triIdx[i+2]);
		if (!v0 || !v1 || !v2) continue;
		tri.push_back(v0);
		tri.push_back(v1);
		tri.push_back(v2);
		InsertFace(tri);
	}
	UpdateMesh();
}

void Mesh3D::CreateMesh(const std::vector<double>& verts, const std::vector<unsigned>& triIdx)
{
	ClearData();
	for (unsigned int i=0; i<verts.size(); i=i+3)
	{
		InsertVertex(Vec3f(verts[i], verts[i+1], verts[i+2]));
	}
	for (unsigned int i=0; i<triIdx.size(); i=i+3)
	{
		std::vector<HE_vert*> tri;
		HE_vert *v0 = get_vertex(triIdx[i]);
		HE_vert *v1 = get_vertex(triIdx[i+1]);
		HE_vert *v2 = get_vertex(triIdx[i+2]);
		if (!v0 || !v1 || !v2) continue;
		tri.push_back(v0);
		tri.push_back(v1);
		tri.push_back(v2);
		InsertFace(tri);
	}
	UpdateMesh();
}

int Mesh3D::GetBoundaryVrtSize()
{
	int count = 0;
	for (int i=0; i<num_of_vertex_list(); i++)
	{
		if (get_vertex(i)->isOnBoundary())
		{
			count ++;
		}
	}
	return count;
}

void Mesh3D::meshTranslate(float param1, float param2)
{
	if (pvertices_list_ != NULL)
	{
		for (VERTEX_ITER iter = pvertices_list_->begin(); iter != pvertices_list_->end(); iter++)
		{
			

		}
	}
}


void Mesh3D::scalemesh(float size)
{
	Unify(size);
}

Mesh3D::~Mesh3D(void)
{
	ClearData();
}

void Mesh3D::exportNeighborId()
{
	//FILE outfile("C:\\Users\\Dell\\Desktop\\zhulin.txt");
	std::fstream out;
	out.open("C:\\Users\\h\\Desktop\\zhulin.txt");
	out.clear();
	if (!out.is_open())
	{
		return;
	}
	{
  		for (VERTEX_ITER iter = pvertices_list_->begin(); iter != pvertices_list_->end(); iter++)
  		{	
  			//out << "v "<<(*iter)->id()<<" ";
  			//out << (*iter)->position().x() << " " << (*iter)->position().y() << " " << (*iter)->position().z();
  			for (int i=0;i<(*iter)->neighborIdx.size();i++)
  			{
  				out<<(*iter)->neighborIdx[i]<<" ";
  			}
  			out << "\r\n";
  		}
		for (FACE_ITER iterface = pfaces_list_->begin(); iterface != pfaces_list_->end(); iterface++)
		{
			
			HE_edge * cur = (*iterface)->pedge_; 
			HE_edge *last = cur;
			do 
			{
				out << "f " << (*iterface)->id() << " ";
				out << cur->id() << " " <<cur->ppair_->id() << " " << cur->pprev_->pvert_->id() << " " << cur->pvert_->id()<<"\r\n";
				cur = cur->pnext_;
			} while (last!=cur);
		}
	}
	out.close();
}

//////////////////////////////////////////////////////////////////////////
HE_face* Mesh3D::InsertFaceSup(std::vector<HE_vert* >& vec_hv)
{
	if (pfaces_list_ == NULL)
	{
		pfaces_list_ = new std::vector<HE_face*>;
	}

	int i = 0;
	HE_edge *he[3], *bhe[3];
	HE_vert *v[3];
	HE_face *f;

	// obtain objects
	for (i = 0; i < 3; i++) he[i] = new HE_edge();
	for (i = 0; i < 3; i++) {
		bhe[i] = new HE_edge();
		bhe[i]->set_boundary_flag(BOUNDARY);
	}
	v[0] = vec_hv[0];
	v[1] = vec_hv[1];
	v[2] = vec_hv[2];
	f = new HE_face();

	// connect prev-next pointers
	he[0]->pnext_ = he[1];
	he[1]->pprev_ = he[0];
	he[1]->pnext_ = he[2];
	he[2]->pprev_ = he[1];
	he[2]->pnext_ = he[0];
	he[0]->pprev_ = he[2];
	bhe[0]->pnext_ = bhe[1];
	bhe[1]->pprev_ = bhe[0];
	bhe[1]->pnext_ = bhe[2];
	bhe[2]->pprev_ = bhe[1];
	bhe[2]->pnext_ = bhe[0];
	bhe[0]->pprev_ = bhe[2];
	// connect twin pointers
	he[0]->ppair_ = bhe[0];
	bhe[0]->ppair_ = he[0];
	he[1]->ppair_ = bhe[2];
	bhe[2]->ppair_ = he[1];
	he[2]->ppair_ = bhe[1];
	bhe[1]->ppair_ = he[2];

	// connect start pointers for bhe
	bhe[0]->start_ = v[1];
	bhe[0]->pvert_ = v[0];
	bhe[1]->start_ = v[0];
	bhe[1]->pvert_ = v[2];
	bhe[2]->start_ = v[2];
	bhe[2]->pvert_ = v[1];
	he[0]->start_ = v[0];
	he[0]->pvert_ = v[1];
	he[1]->start_ = v[1];
	he[1]->pvert_ = v[2];
	he[2]->start_ = v[2];
	he[2]->pvert_ = v[0];
	// connect start pointers
	// connect face-hedge pointers
	for (i = 0; i < 3; i++) {
		v[i]->pedge_ = (he[i]);
		he[i]->pface_ = f;
		f->pedge_ = he[i];
	}

	if (pedges_list_ == NULL)
	{
		pedges_list_ = new std::vector<HE_edge*>;
	}
	if (bheList == NULL)
	{
		bheList = new std::vector<HE_edge *>;
	}
	for (i = 0; i < 3; i++)
	{

		he[i]->id_ = static_cast <int>(pedges_list_->size());
		pedges_list_->push_back(he[i]);
	}
	for (i = 0; i < 3; i++) bheList->push_back(bhe[i]);
	// merge boundary if needed
	for (i = 0; i < 3; i++) {
		HE_vert *start = bhe[i]->start_;
		HE_vert *end = bhe[i]->pvert_;
		for (size_t j = 0; j < end->adjHEdges.size(); j++) {
			HE_edge *curr = end->adjHEdges[j];
			if (curr->boundary_flag() == BOUNDARY && curr->pvert_ == start) {

				bhe[i]->pprev_->pnext_ = curr->pnext_;
				curr->pnext_->pprev_ = bhe[i]->pprev_;
				curr->pprev_->pnext_ = bhe[i]->pnext_;
				bhe[i]->pnext_->pprev_ = curr->pprev_;
				bhe[i]->ppair_->ppair_ = curr->ppair_;
				curr->ppair_->ppair_ = bhe[i]->ppair_;
				bhe[i]->start_ = NULL;	// mark as unused
				curr->start_ = NULL;	// mark as unused
				break;
			}
		}
	}
	v[0]->adjHEdges.push_back(he[0]);
	v[0]->adjHEdges.push_back(he[2]);
	v[0]->adjHEdges.push_back(bhe[0]);
	v[0]->adjHEdges.push_back(bhe[1]);
	v[1]->adjHEdges.push_back(he[1]);
	v[1]->adjHEdges.push_back(he[0]);
	v[1]->adjHEdges.push_back(bhe[0]);
	v[1]->adjHEdges.push_back(bhe[2]);
	v[2]->adjHEdges.push_back(he[1]);
	v[2]->adjHEdges.push_back(he[2]);
	v[2]->adjHEdges.push_back(bhe[1]);
	v[2]->adjHEdges.push_back(bhe[2]);

	// finally add hedges and faces to list
	f->id_ = static_cast<int>(pfaces_list_->size());
	pfaces_list_->push_back(f);
	return f;
}
void Mesh3D::UpdateMeshSup(void)
{
	
	UpdateBList();
	ComputeBoundingBox();
	//Unify(1.0);
	SetNeighbors();
	SetBoundaryFlag();
	computeComponent();
	UpdateNormal();
}
void Mesh3D::UpdateBList(void)
{
	std::vector<HE_edge*>* list = new std::vector<HE_edge*>;
	for (int i = 0; i < bheList->size(); i++)
	{
		if ((*bheList)[i]->start_ != NULL)
		{
			list->push_back(bheList->at(i));
			bheList->at(i)->id_ = static_cast<int> (pedges_list_->size());
			pedges_list_->push_back(bheList->at(i));
		}
		else
		{
			delete bheList->at(i);
			bheList->at(i) = NULL;
		}
	}
	bheList = list;
}
void Mesh3D::computeComponent()
{
	if (bheList == NULL)
	{
		return;
	}
	bLoop.clear();
	num_components_ = 0;
	for (size_t i = 0; i < bheList->size(); i++)
	{
		bheList->at(i)->is_selected_=false;
	}
	for (size_t i = 0; i < bheList->size(); i++)
	{
		HE_edge *sta = bheList->at(i);
		HE_edge *cur = sta;
		if (cur->is_selected_)
		{
			continue;
		}

		bLoop.resize(++num_components_);

		do
		{
			cur->is_selected_=true;
			bLoop[num_components_ - 1].push_back(cur);
			cur = cur->pnext_;
		} while (cur != sta);
	}
	for (size_t i = 0; i < bheList->size(); i++)
	{
		bheList->at(i)->start_->set_seleted(UNSELECTED);
	}
	return;
	for (int i = 0; i < num_components_; i++)
	{
		HE_face* facet = bLoop[i].at(1)->ppair_->pface_;

		FaceDFS(facet, i);
	}
	for (int i = 0; i < num_of_face_list(); i++)
	{
		pfaces_list_->at(i)->set_selected(UNSELECTED);
	}
}
void Mesh3D::FaceDFS(HE_face* facet, int no)
{
	facet->set_selected(SELECTED);
	facet->com_flag= no;
	HE_edge* sta = facet->pedge_;
	HE_edge* cur = sta;
	do
	{
		facet = cur->ppair_->pface_;
		if (facet != NULL&&facet->selected() == UNSELECTED)
		{
			FaceDFS(cur->ppair_->pface_, no);
		}
		cur = cur->pnext_;
	} while (cur != sta&&cur != NULL);

}




