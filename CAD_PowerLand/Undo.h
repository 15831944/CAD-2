/*

	撤消时用的结构和一些定义

*/
//以下是只编译一次
#if !defined(_POWERLAND_CAD_UNDO_H)
#define _POWERLAND_CAD_UNDO_H

#if _MSC_VER > 1000 //如果MFC版本大于4.21
#pragma once	//只编译一次
#endif 








typedef struct _CADPOINT//点
{
	double		x;	
	double		y;
} CADPOINT;

typedef struct _GRAPH_POS//选择集里的图形
{
	LONG		pCreateBasepos;	//图形在链表中的数据指针
	LONG        pCreateBaseposSpare1;//图形在链表中的数据指针备用
	LONG        pCreateBaseposSpare2;//图形在链表中的数据指针备用
	CADPOINT potData1;	
	CADPOINT potData2;	
	int	  iData1;		
	int   iData2;
	double dData1;
	double dData2;		
	POSITION	pListPos;	//图形在链表中的结点指针	
	POSITION	pListPosSpare;//图形在链表中的结点指针备用
	CArray<Position,Position&> aryPoint;//点集
	CArray<double,double&> aryTudu;//凸度集
	_GRAPH_POS	*pNext;
	_GRAPH_POS	*pPrev;
	
} GRAPHPOS, *PGRAPHPOS;

typedef struct _OPERATE_PARAM//操作参数
{
	CADPOINT potBase;	//基点
	CADPOINT potDes;	//目地的点
	LONG pPosition;		//位置指针
	int direction;
	double distance;
	double angle;	
	
} OPERATE_PARAM, *POPERATE_PARAM;

typedef struct _UNDOLIST	
{
	int OperatieNum;	//操作步骤
	int OperatieWhat;	//操作项
	POPERATE_PARAM pOperatieParam;//操作参数
	PGRAPHPOS pGraphPos;	//图形的集
	PGRAPHPOS pGraphPosSpare;//图形的集备用
	_UNDOLIST		*pNext;
	_UNDOLIST		*pPrev;

} UNDOLIST, *PUNDOLIST;






#endif 