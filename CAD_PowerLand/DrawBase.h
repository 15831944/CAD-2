// DrawBase.h: interface for the CDrawBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWBASE_H__1719DA0B_4A01_4AE5_9689_5756BC40833E__INCLUDED_)
#define AFX_DRAWBASE_H__1719DA0B_4A01_4AE5_9689_5756BC40833E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Position.h"

#define END_POINT			1
#define NEAR_POINT			2
#define FOOT_POINT			4
#define ORTHOGONAL			8
#define MID_POINT			16

class CDrawBase  
{
public:
	CDrawBase();
	virtual ~CDrawBase();
	//                     参数nFlags为1鼠标点击，为2键盘点击
	virtual int OnLButtonDown(UINT nFlags, const Position& pos,CString cmd) = 0;   //纯虚函数 
	virtual int OnMouseMove(UINT nFlags, const Position& pos) = 0;	
	virtual int OnRButtonDown(UINT nFlags, const Position& pos)  = 0;	
	virtual int OnLButtonUp(UINT nFlags, const Position& pos) {return 0;};	
	
protected:
	
	//对象捕捉
	BOOL FindPointAll(CDC* pDC,Position posBase,Position posMove,int iCatch = 3);
	//设定鼠标点下的点
	void SetLButtonDownPot(Position& pos,int NumOfClick);
	//输入无效的内容
	int InvalidContent();
	//执行成功的善后工作
	void SuccessWord(CString str);


	//鼠标点击次数
	int m_iNumOfClick;	
	Position m_Begin;
	Position m_middle;
	Position m_End;
	//会点中的点
	Position m_posWillEnd,m_posPassEnd,m_posWillNear,m_posPassNear,m_posWillFoot,m_posPassFoot,m_posWillMid,m_posPassMid,m_posOrthogonal;
	//是否点到端点上
	BOOL m_bIsClickEnd,m_bIsClickNear,m_bIsClickMid,m_bIsClickFoot,m_bisClickOrthogonal;

private:
	//查找端点
	BOOL FindPointEnd(Position pos);
	//查找最近点
	BOOL FindPointNear(Position pos);
	//查找垂足点
	BOOL FindPointFoot(Position posBase,Position posMove);
	//查找中点
	BOOL FindPointMid(Position pos);
	//反回正交点
	Position RtnOrthogonalPos(Position posBase,Position posMove);
	


	//画最近点
	void DrawNearPoint(CDC* pDC,CPoint pot);
	//画垂足点
	void DrawFootPoint(CDC* pDC,CPoint pot);
	//画中点
	void DrawMidPoint(CDC* pDC,CPoint pot);

};




#endif // !defined(AFX_DRAWBASE_H__1719DA0B_4A01_4AE5_9689_5756BC40833E__INCLUDED_)
