// DrawSpline.cpp: implementation of the CDrawSpline class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "Draw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawSpline::CDrawSpline()
{
	m_refresh = 0;
}


CDrawSpline::~CDrawSpline()
{
}

int CDrawSpline::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	m_refresh = 0;
	m_iNumOfClick++;
	if(m_iNumOfClick == 1)
	{
		m_ControlPoints.Add(l_cPos);
		m_NumControl.Add(m_refresh);
	}
	if(m_iNumOfClick == 2)
	{
		if(m_posLast.Distance(l_cPos)<=1)//如果点击的两点距离很短就返回
			return 0;
		CDC* pDC = g_pView->GetDC();
		m_ControlPoints.RemoveAt(m_ControlPoints.GetSize()-1);
		m_ControlPoints.Add(l_cPos);
		if(m_ControlPoints.GetSize()>1)
		{
			//创建一个样条曲线对象
			Spline spline(m_ControlPoints, m_ControlPoints.GetSize());
			//根据控制点生成一个样条曲线
			spline.Generate();
			//设定曲线点数				得到控制点数
			m_CurvePoints.SetSize(spline.GetCurveCount());
			//得到曲线的轮廓点集
			int PointCount = 0;
			spline.GetCurve(m_CurvePoints, PointCount);
			m_NumControl.Add(PointCount);			
			//向图形链表里添加
			CCreateSpline* l_pTempSpline = new CCreateSpline(m_CurvePoints,m_ControlPoints,m_NumControl);
			l_pTempSpline->DrawMove(pDC,dmNormal);	
			delete l_pTempSpline;
			m_iNumOfClick--;
		}
		g_pView->ReleaseDC(pDC);
		g_pView->Invalidate();
	}
	m_posLast = l_cPos;
	return 0;
}

int CDrawSpline::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//改变鼠标形状	
	Position l_cPos(pos);
	// 得到设备环境指针
	CDC*	pDC = g_pView->GetDC(); 
	if (m_refresh == 1000)
		m_refresh = 100;
	m_refresh++;
	if (m_iNumOfClick == 1)
	{
		// 创建临时对象擦除上一条橡皮线
		if(m_refresh != 1&&g_refresh!=0)
		{
			// 创建临时对象擦除上一条橡皮线
			m_ControlPoints.RemoveAt(m_ControlPoints.GetSize()-1);
			m_ControlPoints.Add(m_prePos);
			//创建一个样条曲线对象
			Spline spline(m_ControlPoints, m_ControlPoints.GetSize());
			//根据控制点生成一个样条曲线
			spline.Generate();
			//设定曲线点数				得到控制点数
			m_CurvePoints.SetSize(spline.GetCurveCount());
			//得到曲线的轮廓点集
			int PointCount = 0;
			spline.GetCurve(m_CurvePoints, PointCount);
			CCreateSpline* l_pTempSpline = new CCreateSpline(m_CurvePoints,m_ControlPoints,m_NumControl);
			l_pTempSpline->DrawMove(pDC,dmDrag);
			delete l_pTempSpline;			
		}
		if(m_refresh != 1)
			m_ControlPoints.RemoveAt(m_ControlPoints.GetSize()-1);		
		m_ControlPoints.Add(l_cPos);
		//创建一个样条曲线对象
		Spline spline(m_ControlPoints, m_ControlPoints.GetSize());
		//根据控制点生成一个样条曲线
		spline.Generate();
		//设定曲线点数				得到控制点数
		m_CurvePoints.SetSize(spline.GetCurveCount());
		//得到曲线的轮廓点集
		int PointCount = 0;
		spline.GetCurve(m_CurvePoints, PointCount);
		CCreateSpline* l_pTempSpline = new CCreateSpline(m_CurvePoints,m_ControlPoints,m_NumControl);
		l_pTempSpline->DrawMove(pDC,dmDrag);
		delete l_pTempSpline;
		// 将当前位置设置为直线终点，以备下一次鼠标移动时用
		m_prePos = l_cPos; 
		g_refresh = 2;
	}
	//对象捕捉
	FindPointAll(pDC,m_prePos,pos,g_pView->m_iSwitch);
	// 释放设备环境指针
	g_pView->ReleaseDC(pDC); 
	return 0;
}



int CDrawSpline::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//如果只点击一个点就返回
	if(m_ControlPoints.GetSize() <= 2)
		return 0;
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if(l_cPos != m_posLast)//当鼠标左键点击完最后一点时，鼠标不再移动（也就是没有再向控制点集里加点），就不用删除最后一点了
		m_ControlPoints.RemoveAt(m_ControlPoints.GetSize()-1);				
	//创建一个样条曲线对象
	Spline spline(m_ControlPoints, m_ControlPoints.GetSize());
	//根据控制点生成一个样条曲线
	spline.Generate();
	//设定曲线点数				得到控制点数
	m_CurvePoints.SetSize(spline.GetCurveCount());
	//得到曲线的轮廓点集
	int PointCount = 0;
	spline.GetCurve(m_CurvePoints, PointCount);
	//向图形链表里添加
	CCreateSpline* l_pTempSpline = new CCreateSpline(m_CurvePoints,m_ControlPoints,m_NumControl);
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempSpline);
	//把操作步骤添加到撤消链表里
	AddUndoList((LONG)l_pTempSpline);				
	g_pView->Invalidate();
	//结束画图元//把要画的图元删除
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	return 0;
}

int CDrawSpline::IsInsideControlPoint(Position pos)
{
	int count = m_ControlPoints.GetSize();	
	CPoint point;
	g_pView->WorldToScreem(pos,point);	
	CPoint l_potClick;
	Position l_posCurve;
	if(count>0)	
	{
		for(int i=0; i<count; i++)
		{
			l_posCurve = m_ControlPoints.GetAt(i);
			g_pView->WorldToScreem(l_posCurve,l_potClick);
			if(Distance(l_potClick, point)<4)
				return i;
		}
	}
	
	return -1;
	
}

double CDrawSpline::Distance(CPoint p1, CPoint p2)
{
	int dx = abs(p1.x - p2.x);
	int dy = abs(p1.y - p2.y);
	
	return sqrt(dx*dx + dy*dy);
	
}

/*
 *	把这个操作添加到撤消链表
 */
void CDrawSpline::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}