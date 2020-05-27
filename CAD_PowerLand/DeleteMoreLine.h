// DeleteMoreLine.h: interface for the CDeleteMoreLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELETEMORELINE_H__ED0089F6_6A10_409D_B569_8F1F591CC6A6__INCLUDED_)
#define AFX_DELETEMORELINE_H__ED0089F6_6A10_409D_B569_8F1F591CC6A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDeleteMoreLine  //删除重刀
{
public:
	CDeleteMoreLine();
	virtual ~CDeleteMoreLine();
	
	//接口函数
	void DeleteMoreLine();
private:
	
	
	/////////////////////////函数////////////////////////////////////////////
	
	//把所有的图形链表复制一下
	void CopyList();
	//清空基础链表
	void ClearBaseList();
	//检测直线是否与其它线重合
	BOOL CleckLine();
	//检测圆是否与其它线重合
	BOOL CleckRound();
	//检测圆弧是否与其它线重合
	BOOL CleckArc();
	//分解多段线
	void AnlysPolyline();
	//分解矩形
	void AnlysRect();
	//分解椭圆
	void AnlysEllipse();
	//改变切割方向
	void Changedir() ;
	//返回离原点最近的点和元素和斜率
	BOOL RtnPosBaseSlope();
	///////////////////变量//////////////////////////////////////////////////
	
	//			复制链表
	CObList m_cpyBaseList;
	//链表里的图元
	CCreateBase* m_pCreateBase;
	//离原点最近的点	,起点		终点
	Position m_posOriger,m_PosBegin,m_posEnd;
	//每个线的斜率
	double l_dSlope;
};

#endif // !defined(AFX_DELETEMORELINE_H__ED0089F6_6A10_409D_B569_8F1F591CC6A6__INCLUDED_)
