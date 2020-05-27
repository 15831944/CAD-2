// OverCut.h: interface for the COverCut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OVERCUT_H__8ECCD33A_20C9_426D_A5E2_057F05375A99__INCLUDED_)
#define AFX_OVERCUT_H__8ECCD33A_20C9_426D_A5E2_057F05375A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COverCut  //过切的类
{
public:
	COverCut();
	virtual ~COverCut();
	//是不是直线，然后才过切
	void NeedOverCut();
private:
	void ExecuteOverCut(CCreatePolyline* l_pIsPolyline,POSITION l_POSOverCut);//执行过切函数
	//功能：给直线加延长线
	//参数：CMyLine lnIn:原直线
	//	int nType: 0 表示正向延长 1表示反向延长
	//	double distance: 延长距离
	//	CMyLine& lnOut:延长后直线
	void ProcLine(Position ptStart1,Position ptEnd1, int nType, double distance,
						  Position& ptStart2,Position& ptEnd2);
	//把多段线根据过切的点分解为多个
	void PutPloyBreak(CCreatePolyline* l_pIsPolyline,CByteArray* l_carryNumPoint,POSITION l_POSOverCut);

	
	//撤消类的对象
	CUndoRedo m_UndoRedo;
	//向撤消链表里添加的一个结点	
	PUNDOLIST m_pUndoList;
	//存图形的指针
	PGRAPHPOS m_pCurGraphPos,m_pheadGraphPos,m_pGraphPos;


};

#endif // !defined(AFX_OVERCUT_H__8ECCD33A_20C9_426D_A5E2_057F05375A99__INCLUDED_)
