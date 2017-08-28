#pragma once

#include "Triangle.h"
#include <vector>

/* 一个矩形区域的象限划分：:

UL(1)   |    UR(0)
--------|---------
LL(2)   |    LR(3)
以下对该象限类型的枚举
*/
//八叉树

typedef enum 
{
	UR=0,UL=1,
	LL=2,LR=3,

	HUR=4,HUL=5,
	HLL=6,HLR=7

}QuadrantEnum;

/* 矩形结构 */
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


/* 八叉树节点类型结构 */
typedef struct quadnode_t
{
	quadrect_t    rect;          //节点所代表的立方体区域  
	int        *lst_object;   //节点数据, 节点类型一般为链表，可存储多个对象  这里先用int replace
	struct  quadnode_t  *sub[8]; //指向节点的八个孩子   
}quadnode_t;

/* 八叉树类型结构 */
typedef struct quadtree_t
{
	quadnode_t  *root;
	int         depth;           // 八叉树的深度                      
}quadtree_t;

/* 八叉树类型结构 */
class QuadTree
{
public:
	quadnode_t  *root;
	int         depth;           // 八叉树的深度     

public:
	void QuadTreeBuild(int depth, quadrect_t rect);//构建QTree

	void QuadCreateBranch(quadnode_t *root,int depth, quadrect_t rect);/*创建分支，root树的根，depth深度，rect根节点代表的矩形区域*/

	void QuadInsert(Triangle i,quadnode_t *root);//把数据对象插入Tree

	std::vector<Triangle> Tria; //用来判断三角面的位置关系

public:
	QuadTree();
	~QuadTree();
};

