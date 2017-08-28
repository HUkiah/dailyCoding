#pragma once

#include "Triangle.h"
#include <vector>

/* һ��������������޻��֣�:

UL(1)   |    UR(0)
--------|---------
LL(2)   |    LR(3)
���¶Ը��������͵�ö��
*/
//�˲���

typedef enum 
{
	UR=0,UL=1,
	LL=2,LR=3,

	HUR=4,HUL=5,
	HLL=6,HLR=7

}QuadrantEnum;

/* ���νṹ */
typedef struct quadrect_t
{
	double  xLeft, xRight,
		yLeft, yRight,
		zLeft, zRight;
	quadrect_t(double xleft, double xright, double yleft, double yright, double zleft, double zright) :
		xLeft(xleft), xRight(xright), yLeft(yright), yRight(yright), zLeft(zleft), zRight(zright) { }

	inline quadrect_t operator = (const quadrect_t &rect){
		this->xLeft = rect.xLeft;
		this->xRight = rect.xRight;
		this->yLeft = rect.yLeft;
		this->yRight = rect.yRight;
		this->zLeft = rect.zLeft;
		this->zRight = rect.zRight;
		return *this;
	}
}quadrect_t;


/* �˲����ڵ����ͽṹ */
typedef struct quadnode_t
{
	quadrect_t    rect;          //�ڵ������������������  
	int        *lst_object;   //�ڵ�����, �ڵ�����һ��Ϊ�����ɴ洢�������  ��������int replace
	struct  quadnode_t  *sub[8]; //ָ��ڵ�İ˸�����   
}quadnode_t;

/* �˲������ͽṹ */
typedef struct quadtree_t
{
	quadnode_t  *root;
	int         depth;           // �˲��������                      
}quadtree_t;

/* �˲������ͽṹ */
class QuadTree
{
public:
	quadnode_t  *root;
	int         depth;           // �˲��������     

public:
	void QuadTreeBuild(int depth, quadrect_t rect);//����QTree

	void QuadCreateBranch(quadnode_t *root,int depth, quadrect_t rect);/*������֧��root���ĸ���depth��ȣ�rect���ڵ����ľ�������*/

	void QuadInsert(Triangle i,quadnode_t *root);//�����ݶ������Tree

	std::vector<Triangle> Tria; //�����ж��������λ�ù�ϵ

public:
	QuadTree();
	~QuadTree();
};

