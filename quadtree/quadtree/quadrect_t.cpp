#include "quadrect_t.h"
#include <corecrt_malloc.h>



void QuadTree::QuadTreeBuild(int depth, quadrect_t rect)
{
	this->depth = depth;

	for(size_t i = 1;i < Tria.size();i++)      //�������ж���
	{
		QuadInsert(Tria[i], root);//��i��������Ĳ���
	}
	//�޳�����Ľڵ㣻       //ִ��������ѭ�����Ĳ����п���������Ϊ�յ�Ҷ�ӽڵ���Ҫ�޳�

	/*������֧��root���ĸ���depth��ȣ�rect���ڵ����ľ�������*/
	QuadCreateBranch(root, depth, rect);
}





void QuadTree::QuadCreateBranch(quadnode_t *root, int depth, quadrect_t rect)
{

	if (depth != 0)
	{
	
		root = (quadnode_t *)malloc(sizeof(quadnode_t));    //�����½ڵ�

		root->rect = rect; //���ýڵ�������ľ�������洢���ýڵ���
		
		double xMind = ((rect.xRight - rect.xLeft) / 2)>0 ? (rect.xRight - rect.xLeft) / 2 : -(rect.xRight - rect.xLeft) / 2;//����ڵ�һά�ϰ�߳�
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



		/*���������ӷ�֧ ��rect�����ķ� rect[UR], rect[UL], rect[LL], rect[LR];*/
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

void QuadTree::QuadInsert(Triangle i, quadnode_t * root)//QuadInsert(i,n)      //�ú���������Ĳ����е�ÿ���ڵ����洢�Ķ�����������1����0
{
	if(root)//�ڵ�n�к���
	{
		//ͨ�����������ж�iӦ�÷�����n�ڵ����һ�����ӽڵ�c��
		QuadInsert(i, c);
	}
	 else if(root)//�ڵ�n�洢��һ������
	{
		 //Ϊn�ڵ㴴���ĸ����ӣ�
		 //��n�ڵ��еĶ����Ƶ���Ӧ�÷��õĺ��ӽڵ��У�
		 //ͨ�����������ж�iӦ�÷�����n�ڵ����һ�����ӽڵ�c��
		 QuadInsert(i, c);
	 }
	else if(root)//n�ڵ�����Ϊ��
	 {
		 //��i�洢���ڵ�n�У�
	 }

}


QuadTree::QuadTree()
{

}


QuadTree::~QuadTree()
{
}
