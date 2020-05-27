// DrawMovePoint.cpp: implementation of the CDrawMovePoint class.
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

CDrawMovePoint::CDrawMovePoint()
{
	m_pUndoList = NULL;
	m_pGraphPos = NULL;
	m_pCurGraphPos = NULL;
}

CDrawMovePoint::~CDrawMovePoint()
{
	ClearList();
}


int CDrawMovePoint::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));//改变鼠标形状	
	g_refresh = 0;
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if(m_iNumOfClick == 0)//选点
	{
		m_aryMovePoint.RemoveAll();
		ClearList();
		if(FindPointEnd(l_cPos))//确定哪类图形和上面的点
		{
			m_posFirst = l_cPos;
			m_iNumOfClick++;
			CopyList();
		}
		return 0;
	}
	if(m_iNumOfClick == 1)
	{
		DoMovePoint(m_aryMovePoint,l_cPos,TRUE);
		m_iNumOfClick = 0;
		g_pView->Invalidate();
	}
	return 0;
}

int CDrawMovePoint::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));//改变鼠标形状	
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	curPos = pos;
	if((m_iNumOfClick == 0))	//对象捕捉
		FindPointAll(pDC,m_middle,pos,END_POINT);	
	else
		FindPointAll(pDC,m_posFirst,pos,g_pView->m_iSwitch);
	if (m_iNumOfClick == 1)
	{	
		//图形大小
		int l_iSize = m_aryDrawMouseMove.GetSize();		
		stcMovePoint l_stcMovePoint;
		// 获得鼠标所在的前一个位置
		prePos = m_posPre; 
		// 创建临时对象擦除上一条橡皮线
		CCreateBase*	pTempLine1 = new CCreateLine(m_posFirst, prePos);
		if(g_refresh != 1)	//这是为了第一次没有擦除时所做的
		{
			pTempLine1->DrawMove(pDC,dmDrag);
			DoMovePoint(m_aryDrawMouseMove,prePos,FALSE);
			for(int i=0;i<l_iSize;i++)
			{
				l_stcMovePoint = m_aryDrawMouseMove.GetAt(i);
				l_stcMovePoint.pCreateBase->DrawMove(pDC,dmDrag);
			}
		}
		delete pTempLine1;
		//设定捕捉到的点
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);	
		// 创建临时对象，根据当前位置绘制一条橡皮线
		CCreateBase*	pTempLine2 = new CCreateLine(m_posFirst, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		DoMovePoint(m_aryDrawMouseMove,curPos,FALSE);
		for(int i=0;i<l_iSize;i++)
		{
			l_stcMovePoint = m_aryDrawMouseMove.GetAt(i);
			l_stcMovePoint.pCreateBase->DrawMove(pDC,dmDrag);
		}
		
		// 释放设备环境指针		
		// 将当前位置设置为直线终点，以备下一次鼠标移动时用
		m_posPre = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	
	return 0;
}



int CDrawMovePoint::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//结束画图元//把要画的图元删除
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	return 0;
}

/*
 *	复制一个鼠标移动时要画的链表
 */
void CDrawMovePoint::CopyList()
{
	int l_iSize = m_aryMovePoint.GetSize();
	stcMovePoint l_stcMovePoint,l_stcDrawMovePoint;
	for(int i=0;i<l_iSize;i++)
	{
		l_stcMovePoint = m_aryMovePoint.GetAt(i);
		CCreateBase* pCopy  = l_stcMovePoint.pCreateBase->Copy();
		l_stcDrawMovePoint = l_stcMovePoint;
		l_stcDrawMovePoint.pCreateBase = pCopy;
		m_aryDrawMouseMove.Add(l_stcDrawMovePoint);
	}				
}

/*
 *	释放m_aryDrawMouseMove
 */
void CDrawMovePoint::ClearList()
{
	int l_iSize = m_aryDrawMouseMove.GetSize();
	stcMovePoint l_stcDrawMovePoint;
	for(int i=0;i<l_iSize;i++)
	{
		l_stcDrawMovePoint = m_aryDrawMouseMove.GetAt(i);
		delete l_stcDrawMovePoint.pCreateBase;
		l_stcDrawMovePoint.pCreateBase = NULL;
	}				
	m_aryDrawMouseMove.RemoveAll();
}

/*
 *	查找端点
 */
BOOL CDrawMovePoint::FindPointEnd(Position pos)
{
	//两个端点
	Position l_cBegin,l_cEnd;
	//多段线里的点的个数
	int l_iCountOfPoint;
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase;
	while (POS != NULL)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		//点//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePoint)))
		{
			CCreatePoint *pPoint = (CCreatePoint*)l_pCreateBase;
			//看是否在直线的两端
			if(pPoint->m_Begin == pos)
			{
				stcMovePoint l_stcMovePoint;
				l_stcMovePoint.iStyleGraph = MY_POINT;
				l_stcMovePoint.pCreateBase = l_pCreateBase;
				m_aryMovePoint.Add(l_stcMovePoint);
			}
		}
		//直线//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			//看是否在直线的两端
			if(line->m_Begin == pos)
			{
				stcMovePoint l_stcMovePoint;
				l_stcMovePoint.iStyleGraph = MY_LINE;
				l_stcMovePoint.bIsBeginPos =TRUE;
				l_stcMovePoint.pCreateBase = l_pCreateBase;
				m_aryMovePoint.Add(l_stcMovePoint);
			}
			if(line->m_End == pos)
			{
				stcMovePoint l_stcMovePoint;
				l_stcMovePoint.iStyleGraph = MY_LINE;
				l_stcMovePoint.bIsBeginPos = FALSE;
				l_stcMovePoint.pCreateBase = l_pCreateBase;
				m_aryMovePoint.Add(l_stcMovePoint);
			}
		}		
		//多线段//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				//看是否在直线的两端
				if(l_cBegin == pos)
				{
					stcMovePoint l_stcMovePoint;
					l_stcMovePoint.iStyleGraph = MY_POLYLINE;
					l_stcMovePoint.iNumPolyMovePoint = j;
					l_stcMovePoint.pCreateBase = l_pCreateBase;
					m_aryMovePoint.Add(l_stcMovePoint);
					break;
				}
			}
		}
		//样条曲线//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreateSpline *pSpline = (CCreateSpline*)l_pCreateBase;
			
			l_iCountOfPoint = pSpline->m_aryControlPoints.GetSize();
			for(int j=0;j<l_iCountOfPoint;j++)
			{
				l_cBegin = pSpline->m_aryControlPoints.GetAt(j);
				//看是否在直线的两端
				if(l_cBegin == pos)
				{
					stcMovePoint l_stcMovePoint;
					l_stcMovePoint.iStyleGraph = MY_SPLINE;
					l_stcMovePoint.iNumPolyMovePoint = j;
					l_stcMovePoint.pCreateBase = l_pCreateBase;
					m_aryMovePoint.Add(l_stcMovePoint);
					break;
				}			
			}
		}
		
		//矩形//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Polyline = (CCreateRectangle*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				//看是否在直线的两端
				if(l_cBegin == pos)
				{
					stcMovePoint l_stcMovePoint;
					l_stcMovePoint.iStyleGraph = MY_RECTANGLE;
					l_stcMovePoint.iNumPolyMovePoint = j;
					l_stcMovePoint.pCreateBase = l_pCreateBase;
					m_aryMovePoint.Add(l_stcMovePoint);
					break;
				}
			}			
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//圆

		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{//圆弧
			
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//椭圆
			CCreateEllipse *Polyline = (CCreateEllipse*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				//看是否在直线的两端
				if(l_cBegin == pos)
				{
					stcMovePoint l_stcMovePoint;
					l_stcMovePoint.iStyleGraph = MY_ELLIPSE;
					l_stcMovePoint.iNumPolyMovePoint = j;
					l_stcMovePoint.pCreateBase = l_pCreateBase;
					m_aryMovePoint.Add(l_stcMovePoint);
					break;
				}
			}
		}
	}
	if(m_aryMovePoint.GetSize() == 0)
		return FALSE;
	return TRUE;
}

/*
 *	执行点移动
 */
void CDrawMovePoint::DoMovePoint(CArray<stcMovePoint,stcMovePoint&>& aryMovePoint,Position pos,BOOL bAddUndo /*= TRUE*/)
{
	//被移动的图形的大小
	int l_iSize = aryMovePoint.GetSize();
	if(l_iSize == 0)
		return;
	//每个图形的数据结构
	stcMovePoint l_stcMovePoint;
	for(int i=0;i<l_iSize;i++)
	{
		l_stcMovePoint = aryMovePoint.GetAt(i);
		if(bAddUndo == TRUE)//鼠标移动时不用添加内存
			AddUndoList(l_stcMovePoint.pCreateBase);			
		if(l_stcMovePoint.iStyleGraph == MY_POINT)//如果是点类型的
		{
			CCreatePoint* pPoint = (CCreatePoint*)l_stcMovePoint.pCreateBase;
			pPoint->m_Begin = pos;
		}
		else if(l_stcMovePoint.iStyleGraph == MY_LINE)//如果是直线类型的
		{
			CCreateLine* pLine = (CCreateLine*)l_stcMovePoint.pCreateBase;
			if(l_stcMovePoint.bIsBeginPos == TRUE)
				pLine->m_Begin = pos;
			else
				pLine->m_End = pos;
		}
		else if(l_stcMovePoint.iStyleGraph == MY_POLYLINE)//如果是多段线类型的
		{
			CCreatePolyline* pPolyline = (CCreatePolyline*)l_stcMovePoint.pCreateBase;
			//如果点到的是首时并且首末相接的图形
			if(l_stcMovePoint.iNumPolyMovePoint == 0)
			{
				Position l_posFirst = pPolyline->m_point.GetAt(0);
				Position l_posLast = pPolyline->m_point.GetAt(pPolyline->m_point.GetSize()-1);
				if(l_posFirst == l_posLast)
					pPolyline->m_point.SetAt(pPolyline->m_point.GetSize()-1,pos);				
			}
			pPolyline->m_point.SetAt(l_stcMovePoint.iNumPolyMovePoint,pos);
		}
		else if(l_stcMovePoint.iStyleGraph == MY_RECTANGLE)//如果是矩形类型的
		{
			CCreateRectangle* pPolyline = (CCreateRectangle*)l_stcMovePoint.pCreateBase;
			pPolyline->m_point.SetAt(l_stcMovePoint.iNumPolyMovePoint,pos);
			if(l_stcMovePoint.iNumPolyMovePoint == 0)
				pPolyline->m_point.SetAt(4,pos);
			
		}
		else if(l_stcMovePoint.iStyleGraph == MY_ELLIPSE)//如果是椭圆类型的
		{
			CCreateEllipse* pPolyline = (CCreateEllipse*)l_stcMovePoint.pCreateBase;
			pPolyline->m_point.SetAt(l_stcMovePoint.iNumPolyMovePoint,pos);
			if(l_stcMovePoint.iNumPolyMovePoint == 0)
				pPolyline->m_point.SetAt(pPolyline->m_point.GetSize()-1,pos);
			
		}
		else if(l_stcMovePoint.iStyleGraph == MY_SPLINE)//如果是样条曲线类型的
		{
			CCreateSpline* pSpline = (CCreateSpline*)l_stcMovePoint.pCreateBase;
			//如果点到的是首时并且首末相接的图形
			if(l_stcMovePoint.iNumPolyMovePoint == 0)
			{
				Position l_posFirst = pSpline->m_aryControlPoints.GetAt(0);
				Position l_posLast = pSpline->m_aryControlPoints.GetAt(pSpline->m_aryControlPoints.GetSize()-1);
				if(l_posFirst == l_posLast)
					pSpline->m_aryControlPoints.SetAt(pSpline->m_aryControlPoints.GetSize()-1,pos);				
			}
			pSpline->m_aryControlPoints.SetAt(l_stcMovePoint.iNumPolyMovePoint,pos);
			pSpline->RefreshSpline();
		}
	
	}
	if(bAddUndo == TRUE)		
		g_pDoc->m_Undo->AddUndoList(m_pUndoList);	
	m_pUndoList = NULL;
	m_pGraphPos = NULL;
	m_pCurGraphPos = NULL;
}

/*
 *	向撤消链表里添加操作
 */
void CDrawMovePoint::AddUndoList(CCreateBase* pCreateBase)
{
	PGRAPHPOS pGraphPos = NULL;
	if(m_pUndoList == NULL)//如果第一次存
	{
		m_pUndoList = m_UndoRedo.NewUndoList();
		m_pUndoList->OperatieWhat = ID_MOVE_POINT;
		m_pCurGraphPos = m_UndoRedo.NewGraphPos();
		pGraphPos = m_pCurGraphPos;
		m_pCurGraphPos->pCreateBasepos = (LONG)pCreateBase;
		m_pUndoList->pGraphPos = m_pCurGraphPos;		
	}
	else//不是第一次存进链表
	{
		m_pGraphPos = m_UndoRedo.NewGraphPos();						
		pGraphPos = m_pGraphPos;
		m_pGraphPos->pCreateBasepos = (LONG)pCreateBase;
		m_pCurGraphPos->pNext = m_pGraphPos;
		m_pGraphPos->pPrev = m_pCurGraphPos;
		m_pCurGraphPos = m_pGraphPos;
	}
	//如果是直线的话
	if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*) pCreateBase;
		g_pDoc->PosBeEqualed(pGraphPos->potData1,pLine->m_Begin);
		g_pDoc->PosBeEqualed(pGraphPos->potData2,pLine->m_End);		
	}
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePoint)))
	{
		CCreatePoint* pPoint = (CCreatePoint*) pCreateBase;
		g_pDoc->PosBeEqualed(pGraphPos->potData1,pPoint->m_Begin);
	}	
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))//多段线
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*) pCreateBase;
		pGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		pGraphPos->aryPoint.Copy(pPolyline->m_point);
		pGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		pGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))//矩形
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*) pCreateBase;
		pGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		pGraphPos->aryPoint.Copy(pPolyline->m_point);
		pGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		pGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))//椭圆
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*) pCreateBase;
		pGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		pGraphPos->aryPoint.Copy(pPolyline->m_point);
		pGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		pGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))//样条曲线
	{
		CCreateSpline* pSpline = (CCreateSpline*) pCreateBase;
		pGraphPos->aryPoint.SetSize(pSpline->m_aryControlPoints.GetSize());
		pGraphPos->aryPoint.Copy(pSpline->m_aryControlPoints);
	}	
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))//圆弧
	{
		CCreateRoundArc* pArc = (CCreateRoundArc*) pCreateBase;
		m_pUndoList->pOperatieParam = m_UndoRedo.NewOperateParam();
		g_pDoc->PosBeEqualed(pGraphPos->potData1,pArc->m_Begin);
		g_pDoc->PosBeEqualed(pGraphPos->potData2,pArc->m_End);	
		pGraphPos->dData1 = pArc->m_dAngleStart;
		pGraphPos->dData2 = pArc->m_dAngleEnd;
	}
}





















