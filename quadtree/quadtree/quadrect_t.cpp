#include "quadrect_t.h"
#include <corecrt_malloc.h>



void QuadTree::QuadTreeBuild(int depth, quadrect_t rect)
{
	this->depth = depth;

	for(size_t i = 1;i < Tria.size();i++)      //遍历所有对象
	{
		QuadInsert(Tria[i], root);//将i对象插入四叉树
	}
	//剔除多余的节点；       //执行完上面循环后，四叉树中可能有数据为空的叶子节点需要剔除

	/*创建分支，root树的根，depth深度，rect根节点代表的矩形区域*/
	QuadCreateBranch(root, depth, rect);
}





void QuadTree::QuadCreateBranch(quadnode_t *root, int depth, quadrect_t rect)
{

	if (depth != 0)
	{
	
		root = (quadnode_t *)malloc(sizeof(quadnode_t));    //开辟新节点

		root->rect = rect; //将该节点所代表的矩形区域存储到该节点中
		
		double xMind = ((rect.xRight - rect.xLeft) / 2)>0 ? (rect.xRight - rect.xLeft) / 2 : -(rect.xRight - rect.xLeft) / 2;//计算节点一维上半边长
		double yMind = ((rect.yRight - rect.yLeft) / 2)>0 ? (rect.yRight - rect.yLeft) / 2 : -(rect.yRight - rect.yLeft) / 2;
		double zMind = ((rect.zRight - rect.zLeft) / 2)>0 ? (rect.zRight - rect.zLeft) / 2 : -(rect.zRight - rect.zLeft) / 2;

		quadrect_t rectUR(rect.xLeft, rect.xRight - xMind, rect.yRight - yMind, rect.yRight, rect.zRight - zMind, rect.zRight);
		quadrect_t rectUL(rect.xLeft, rect.xRight - xMind, rect.yLeft, rect.yRight - yMind, rect.zRight - zMind, rect.zRight);
		quadrect_t rectLL(rect.xRight - xMind, rect.xRight, rect.yRight - yMind, rect.yRight, rect.zRight - zMind, rect.zRight);
		quadrect_t rectLR(rect.xRight - xMind, rect.xRight, rect.yLeft, rect.yRight - yMind, rect.zRight - zMind, rect.zRight);

		quadrect_t rectHUR(rect.xLeft, rect.xRight - xMind, rect.yRight - yMind, rect.yRight, rect.zLeft, rect.zRight - zMind);
		quadrect_t rectHUL(rect.xLeft, rect.xRight - xMind, rect.yLeft, rect.yRight - yMind, rect.zLeft, rect.zRight - zMind);
		quadrect_t rectHLL(rect.xRight - xMind, rect.xRight, rect.yRight - yMind, rect.yRight, rect.zLeft, rect.zRight - zMind);
		quadrect_t rectHLR(rect.xRight - xMind, rect.xRight, rect.yLeft, rect.yRight - yMind, rect.zLeft, rect.zRight - zMind);



		/*创建各孩子分支 将rect划成四份 rect[UR], rect[UL], rect[LL], rect[LR];*/
		QuadCreateBranch(root->sub[UR], depth - 1, rectUR);
		QuadCreateBranch(root->sub[UL], depth - 1, rectUL);
		QuadCreateBranch(root->sub[LL], depth - 1, rectLL);
		QuadCreateBranch(root->sub[LR], depth - 1, rectLR);

		QuadCreateBranch(root->sub[HUR], depth - 1, rectHUR);
		QuadCreateBranch(root->sub[HUL], depth - 1, rectHUL);
		QuadCreateBranch(root->sub[HLL], depth - 1, rectHLL);
		QuadCreateBranch(root->sub[HLR], depth - 1, rectHLR);
	}
}

void QuadTree::QuadInsert(Triangle i, quadnode_t * root)//QuadInsert(i,n)      //该函数插入后四叉树中的每个节点所存储的对象数量不是1就是0
{
	if(root)//节点n有孩子
	{
		//通过划分区域判断i应该放置于n节点的哪一个孩子节点c；
		QuadInsert(i, c);
	}
	 else if(root)//节点n存储了一个对象
	{
		 //为n节点创建四个孩子；
		 //将n节点中的对象移到它应该放置的孩子节点中；
		 //通过划分区域判断i应该放置于n节点的哪一个孩子节点c；
		 QuadInsert(i, c);
	 }
	else if(root)//n节点数据为空
	 {
		 //将i存储到节点n中；
	 }

}


QuadTree::QuadTree()
{

}


QuadTree::~QuadTree()
{
}
