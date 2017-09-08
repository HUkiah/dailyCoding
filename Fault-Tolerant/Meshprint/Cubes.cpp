#include "Cubes.h"

class Box;

Cubes::Cubes()
{
	ptr_faces_ = NULL;
	boxes_ = new std::set<Box, sortBox>;
}




Cubes::Box Cubes::insertToBox(Vec3f pos, HE_face * facet)
{
	int id_x_ = round((pos.x() + GetUnit() * 10000) / GetUnit() - 0.5) - 10000;
	int id_y_ = round((pos.y() + GetUnit() * 10000) / GetUnit() - 0.5) - 10000;
	int id_z_ = round((pos.z() + GetUnit() * 10000) / GetUnit() - 0.5) - 10000;
	Box b_(id_x_, id_y_, id_z_, GetUnit());
	std::set<Box, sortBox>::iterator box_iter_ = boxes_->insert(b_).first;

	Box & b1 = const_cast<Box&>(*box_iter_);
	b1.facets.push_back(facet);
	return b1;
}


Cubes::~Cubes()
{
	SafeDelete(boxes_);
}

void Cubes::StoreBox()
{
//	setUnit(mesh_in_->average_edge_length());//5 triangles
	for (std::vector<HE_face*>::iterator iterF = ptr_faces_->begin(); iterF != ptr_faces_->end(); iterF++)
	{
		HE_edge* start_ = (*iterF)->pedge_;
		HE_edge* cur_ = start_;
		do
		{
			insertToBox(cur_->pvert_->position(), *iterF);
			cur_ = cur_->pnext_;
		} while (cur_ != start_&&cur_ != NULL);
	}

}

Cubes::Box::Box()
{
	idX_ = 0;	idY = 0;	idZ_ = 0;	zheight_ = 0;	xmin_ = 0.0;	ymin_ = 0.0;	zmin_ = 0.0;

}

Cubes::Box::Box(int x, int y, int z, float hei)
{
	idX_ = x;
	idY = y;
	idZ_ = z;
	xlength = ywidth = zheight_ = hei;
	xmin_ = x*xlength;
	ymin_ = y*ywidth;
	zmin_ = z*zheight_;
}

void Cubes::Box::setCube(float x, float y, float z, float h)
{
	xmin_ = x; ymin_ = y; zmin_ = z; zheight_ = h;
}

Vec4f Cubes::Box::GetCoordinate() const
{
	return Vec4f(xmin_, ymin_, zmin_, zheight_);
}
std::vector<int>  Cubes::Box::getID()const
{
	std::vector<int> id = { idX_,idY,idZ_ };
	return id;
}


void Cubes::Box::insertF(HE_face * facet) 
{
	facets.push_back(facet);
}

void Cubes::Box::insertP(Vec3f point)
{
	if (crosspoint==NULL)
	{
		crosspoint = new std::set<Vec3f, sortVec>;
	}
	crosspoint->insert(point);
}

Cubes::Box::~Box()
{
}
