#pragma once
#include "HE_mesh/Vec.h"
#include "HE_mesh/Mesh3D.h"
#include "globalFunctions.h"
//#include "SliceCut.h"
class Cubes
{
public:
	class Box;
	Cubes();
	~Cubes();
public:
	void StoreBox();
	Box insertToBox(Vec3f pos, HE_face * facet=NULL);
	void setUnit(float param1) { unit_ = param1; }

public:	
	struct sortBox
{
	bool operator ()(Box a, Box b)const
	{

		std::vector<int> aId = a.getID();
		std::vector<int> bId = b.getID();
		if (aId[0] < bId[0]) {
			return true;
		}
		else if (aId[0] == bId[0]) {

			if (aId[1] < bId[1]) {
				return true;
			}
			else if (aId[1] == bId[1])
			{
				if (aId[2] < bId[2])
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else {
				return false;
			}
		}
		else
		{
			return false;
		}

	}
};
	struct sortVec
		{
			bool operator ()(Vec3f a, Vec3f b)const
			{
				if (a[0] - b[0] < -1e-5) {
					return true;
				}
				else if (a[0] - b[0] < 1e-5) {

					if (a[1] - b[1] < -1e-5) {
						return true;
					}
					else if (a[1] - b[1] < 1e-5)
					{
						if (a[2] - b[2] < -1e-5)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
					else {
						return false;
					}
				}
				else
				{
					return false;
				}

			}
		};
	class Box
	{
	public:
		Box();
		Box(int x,int y, int z,float hei);
		void setCube(float x, float y, float z, float h);
		Vec4f GetCoordinate() const;
		std::vector<int> getID()const;
		void insertF(HE_face * facet);
		void insertP(Vec3f point);
	public:
		~Box();
	private:
		float xmin_, ymin_, zmin_,xlength,ywidth,zheight_;
		int idX_, idY, idZ_;
	public:
		std::vector<HE_face*> facets;
		std::set<Vec3f,sortVec>* crosspoint;
	};

	std::set<Box, sortBox>* GetBoxes() { return boxes_; }
	float GetUnit() { return unit_; }
private:
	std::set<Box, sortBox>* boxes_;
	float unit_;
	std::vector<HE_face*>* ptr_faces_;
	Mesh3D* mesh_in_;
	
};
