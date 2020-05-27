// DrawBase.cpp: implementation of the CDrawBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "DrawBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawBase::CDrawBase()
{
	m_iNumOfClick = 0;
	m_bIsClickEnd = FALSE;
	m_bIsClickNear = FALSE;
	m_bIsClickFoot = FALSE;
	m_bIsClickMid = FALSE;
	m_bisClickOrthogonal = FALSE;
}

CDrawBase::~CDrawBase()
{

}

 /*
  *	执行成功的善后工作
  */
void CDrawBase::SuccessWord(CString str)
{
	//执行成功后要把指针清空
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	g_pView->Invalidate();
	RemindContent(str);
}

/*
 *	查找端点
 */
BOOL CDrawBase::FindPointEnd(Position pos)
{
	//两个端点
	Position l_cBegin,l_cEnd;
	//多段线里的点的个数
	int l_iCountOfPoint;
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase = NULL;
	while (POS != NULL)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		//点//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePoint)))
		{
			CCreatePoint *pPoint = (CCreatePoint*)l_pCreateBase;
			//看是否在直线的两端
			if(pPoint->m_Begin.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
			{
				m_posWillEnd = pPoint->m_Begin;
				return TRUE;
			}
		}
		//直线//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			//看是否在直线的两端
			if(line->m_Begin.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
			{
				m_posWillEnd = line->m_Begin;
				return TRUE;
			}
			if(line->m_End.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
			{
				m_posWillEnd = line->m_End;
				return TRUE;
			}
		}		
		//多线段//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				l_cEnd = Polyline->m_point.GetAt(j+1);
				//看是否在直线的两端
				if(l_cBegin.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillEnd = l_cBegin;
					return TRUE;
				}
				else if(l_cEnd.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillEnd = l_cEnd;
					return TRUE;
				}				
			}
		}
		//样条曲线//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreateSpline *Polyline = (CCreateSpline*)l_pCreateBase;
			
			l_iCountOfPoint = Polyline->m_aryControlPoints.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_aryControlPoints.GetAt(j);
				l_cEnd = Polyline->m_aryControlPoints.GetAt(j+1);
				//看是否在直线的两端
				if(l_cBegin.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillEnd = l_cBegin;
					return TRUE;
				}
				else if(l_cEnd.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillEnd = l_cEnd;
					return TRUE;
				}				
			}
		}
		
		//矩形//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Polyline = (CCreateRectangle*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				l_cEnd = Polyline->m_point.GetAt(j+1);
				//看是否在直线的两端
				if(l_cBegin.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillEnd = l_cBegin;
					return TRUE;
				}
				else if(l_cEnd.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillEnd = l_cEnd;
					return TRUE;
				}				
			}			
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//圆

		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{//圆弧
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			//看是否在直线的两端
			if(Arc->m_Begin.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
			{
				m_posWillEnd = Arc->m_Begin;
				return TRUE;
			}
			if(Arc->m_End.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
			{
				m_posWillEnd = Arc->m_End;
				return TRUE;
			}
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//椭圆
			CCreateEllipse *Polyline = (CCreateEllipse*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				l_cEnd = Polyline->m_point.GetAt(j+1);
				//看是否在直线的两端
				if(l_cBegin.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillEnd = l_cBegin;
					return TRUE;
				}
				else if(l_cEnd.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillEnd = l_cEnd;
					return TRUE;
				}				
			}
		}
	}
	return FALSE;
}

/*
 *	查找最近点
 */
BOOL CDrawBase::FindPointNear(Position pos)
{
	//两个端点
	Position l_posBegin,l_posEnd,l_posCenter;
	//屏幕上的点
	CPoint pot;
	//多段线里的点数
	int l_iNumClick;
	//多段线里的凸度
	double l_dTudu=0.0,l_dAgl,l_dRadius;
	BOX2D pBox;
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase = NULL;
	while (POS != NULL)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		//直线//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			line->GetBox(&pBox,TRUE);
			if (IsBelongBox(pos,pBox))
			{
				//点到线的距离 
				if(pos.PotDisLine(line->m_Begin,line->m_End)/g_pView->ReturnProportion()<= PRECISION)
				{
					m_posWillNear = pos.DistancePosFoot(line->m_Begin,line->m_End);
					return TRUE;
				}
			}
		}		
		//多线段//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
			||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			Polyline->GetBox(&pBox,TRUE);
			if (IsBelongBox(pos,pBox))
			{
				g_pView->WorldToScreem(pos,pot);
				l_iNumClick = Polyline->IsPick(pot);
				if (l_iNumClick != -1)
				{
					l_posBegin = Polyline->m_point.GetAt(l_iNumClick);
					l_posEnd = Polyline->m_point.GetAt(l_iNumClick+1);
					l_dTudu = Polyline->m_dTuDu.GetAt(l_iNumClick);
					if (l_dTudu == 0.0)//直线
					{
						m_posWillNear = pos.DistancePosFoot(l_posBegin,l_posEnd);
					}
					else//圆弧
					{	
						g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//求圆心
						l_dRadius = l_posBegin.Distance(l_posCenter);//求半径
						l_dAgl = GetAngleToXAxis(l_posCenter,pos);
						m_posWillNear.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
						m_posWillNear.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
					}
					return TRUE;
				}
			}
		}
		
		//矩形//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Polyline = (CCreateRectangle*)l_pCreateBase;
			Polyline->GetBox(&pBox,TRUE);
			if (IsBelongBox(pos,pBox))
			{
				g_pView->WorldToScreem(pos,pot);
				l_iNumClick = Polyline->IsPick(pot);
				if (l_iNumClick != -1)
				{
					l_posBegin = Polyline->m_point.GetAt(l_iNumClick);
					l_posEnd = Polyline->m_point.GetAt(l_iNumClick+1);
					l_dTudu = Polyline->m_dTuDu.GetAt(l_iNumClick);
					if (l_dTudu == 0.0)//直线
					{
						m_posWillNear = pos.DistancePosFoot(l_posBegin,l_posEnd);
					}
					else//圆弧
					{	
						g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//求圆心
						l_dRadius = l_posBegin.Distance(l_posCenter);//求半径
						l_dAgl = GetAngleToXAxis(l_posCenter,pos);
						m_posWillNear.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
						m_posWillNear.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
					}
					return TRUE;
				}
			}			
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//圆
			CCreateRound *Arc = (CCreateRound*)l_pCreateBase;
			g_pView->WorldToScreem(pos,pot);
			l_iNumClick = Arc->IsPick(pot);
			if (l_iNumClick != -1)
			{
				l_dAgl = GetAngleToXAxis(Arc->m_Center,pos);
				m_posWillNear.x = Arc->m_Center.x + Arc->m_dRadius*cos(l_dAgl);
				m_posWillNear.y = Arc->m_Center.y + Arc->m_dRadius*sin(l_dAgl);
				return TRUE;
			}
		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{//圆弧
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			g_pView->WorldToScreem(pos,pot);
			l_iNumClick = Arc->IsPick(pot);
			if (l_iNumClick != -1)
			{
				l_dAgl = GetAngleToXAxis(Arc->m_Center,pos);
				m_posWillNear.x = Arc->m_Center.x + Arc->m_dRadius*cos(l_dAgl);
				m_posWillNear.y = Arc->m_Center.y + Arc->m_dRadius*sin(l_dAgl);
				return TRUE;
			}
			//看是否在直线的两端
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//椭圆
			CCreateEllipse *Polyline = (CCreateEllipse*)l_pCreateBase;
			Polyline->GetBox(&pBox,TRUE);
			if (IsBelongBox(pos,pBox))
			{
				g_pView->WorldToScreem(pos,pot);
				l_iNumClick = Polyline->IsPick(pot);
				if (l_iNumClick != -1)
				{
					l_posBegin = Polyline->m_point.GetAt(l_iNumClick);
					l_posEnd = Polyline->m_point.GetAt(l_iNumClick+1);
					l_dTudu = Polyline->m_dTuDu.GetAt(l_iNumClick);
					if (l_dTudu == 0.0)//直线
					{
						m_posWillNear = pos.DistancePosFoot(l_posBegin,l_posEnd);
					}
					else//圆弧
					{	
						g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//求圆心
						l_dRadius = l_posBegin.Distance(l_posCenter);//求半径
						l_dAgl = GetAngleToXAxis(l_posCenter,pos);
						m_posWillNear.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
						m_posWillNear.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

//画最近点
void CDrawBase::DrawNearPoint(CDC* pDC,CPoint pot)
{
	pDC->MoveTo(pot.x-5,pot.y-5);
	pDC->LineTo(pot.x+5,pot.y-5);
	pDC->LineTo(pot.x-5,pot.y+5);
	pDC->LineTo(pot.x+5,pot.y+5);
	pDC->LineTo(pot.x-5,pot.y-5);
}


/*
 *	查找垂足点
 */
BOOL CDrawBase::FindPointFoot(Position posBase,Position posMove)
{
	//两个端点
	Position l_posBegin,l_posEnd,l_posCenter,l_posFoot;
	//屏幕上的点
	CPoint pot;
	//多段线里的点数
	int l_iNumClick;
	//多段线里的凸度
	double l_dTudu=0.0,l_dAgl,l_dAgl1,l_dAgl2,l_dRadius;
	BOX2D pBox;
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase = NULL;
	while (POS != NULL)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		//直线//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			line->GetBox(&pBox,TRUE);
			if (IsBelongBox(posMove,pBox))
			{
				l_posBegin = posBase.DistancePosFoot(line->m_Begin,line->m_End);
				//点到点的距离 
				if(posMove.Distance(l_posBegin)/g_pView->ReturnProportion()<= PRECISION)
				{
					m_posWillFoot = l_posBegin;
					return TRUE;
				}
			}
		}		
		//多线段//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
			||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			Polyline->GetBox(&pBox,TRUE);
			if (IsBelongBox(posMove,pBox))
			{
				g_pView->WorldToScreem(posMove,pot);
				l_iNumClick = Polyline->IsPick(pot);
				if (l_iNumClick != -1)
				{
					l_posBegin = Polyline->m_point.GetAt(l_iNumClick);
					l_posEnd = Polyline->m_point.GetAt(l_iNumClick+1);
					l_dTudu = Polyline->m_dTuDu.GetAt(l_iNumClick);
					if (l_dTudu == 0.0)//直线
					{
						l_posFoot = posBase.DistancePosFoot(l_posBegin,l_posEnd);
						//点到点的距离 
						if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
						{
							m_posWillFoot = l_posFoot;
							return TRUE;
						}
					}
					else//圆弧
					{	
						g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//求圆心
						l_dAgl = GetAngleToXAxis(posBase,l_posCenter);
						l_dAgl1 = GetAngleToXAxis(l_posCenter,l_posBegin);
						l_dAgl2 = GetAngleToXAxis(l_posCenter,l_posEnd);						
						if (l_dTudu<0.0)//如果是顺时针
						{
							if (l_dAgl1<l_dAgl2)//过零度线时，就是X轴正方向
							{
								if (!(l_dAgl<l_dAgl1||l_dAgl>l_dAgl2))
									l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
							}
							else
							{
								if (!(l_dAgl<l_dAgl1&&l_dAgl>l_dAgl2))
									l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
							}
						}
						else//逆时针
						{
							if (l_dAgl1>l_dAgl2)//过零度线时，就是X轴正方向
							{
								if (!(l_dAgl>l_dAgl1||l_dAgl<l_dAgl2))
									l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
							}
							else
							{
								if (!(l_dAgl>l_dAgl1&&l_dAgl<l_dAgl2))
									l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
							}	
						}
						l_dRadius = l_posBegin.Distance(l_posCenter);//求半径
						l_posFoot.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
						l_posFoot.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
						//点到点的距离 
						if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
						{
							m_posWillFoot = l_posFoot;
							return TRUE;
						}
					}
				}
			}
		}
		
		//矩形//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Polyline = (CCreateRectangle*)l_pCreateBase;
			Polyline->GetBox(&pBox,TRUE);
			if (IsBelongBox(posMove,pBox))
			{
				g_pView->WorldToScreem(posMove,pot);
				l_iNumClick = Polyline->IsPick(pot);
				if (l_iNumClick != -1)
				{
					l_posBegin = Polyline->m_point.GetAt(l_iNumClick);
					l_posEnd = Polyline->m_point.GetAt(l_iNumClick+1);
					l_dTudu = Polyline->m_dTuDu.GetAt(l_iNumClick);
					if (l_dTudu == 0.0)//直线
					{
						l_posFoot = posBase.DistancePosFoot(l_posBegin,l_posEnd);
						//点到点的距离 
						if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
						{
							m_posWillFoot = l_posFoot;
							return TRUE;
						}
					}
					else//圆弧
					{	
						g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//求圆心
						l_dAgl = GetAngleToXAxis(posBase,l_posCenter);
						l_dAgl1 = GetAngleToXAxis(l_posCenter,l_posBegin);
						l_dAgl2 = GetAngleToXAxis(l_posCenter,l_posEnd);	
						if (l_dTudu<0.0)//如果是顺时针
						{
							if (l_dAgl1<l_dAgl2)//过零度线时，就是X轴正方向
							{
								if (!(l_dAgl<l_dAgl1||l_dAgl>l_dAgl2))
									l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
							}
							else
							{
								if (!(l_dAgl<l_dAgl1&&l_dAgl>l_dAgl2))
									l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
							}
						}
						else//逆时针
						{
							if (l_dAgl1>l_dAgl2)//过零度线时，就是X轴正方向
							{
								if (!(l_dAgl>l_dAgl1||l_dAgl<l_dAgl2))
									l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
							}
							else
							{
								if (!(l_dAgl>l_dAgl1&&l_dAgl<l_dAgl2))
									l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
							}	
						}						
						l_dRadius = l_posBegin.Distance(l_posCenter);//求半径
						l_posFoot.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
						l_posFoot.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
						//点到点的距离 
						if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
						{
							m_posWillFoot = l_posFoot;
							return TRUE;
						}
					}
				}
			}			
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//圆
			CCreateRound *Arc = (CCreateRound*)l_pCreateBase;
			g_pView->WorldToScreem(posMove,pot);
			l_iNumClick = Arc->IsPick(pot);
			if (l_iNumClick != -1)
			{			
				l_posCenter = Arc->m_Center;
				l_dAgl = GetAngleToXAxis(posBase,l_posCenter);
				l_dAgl1 = GetAngleToXAxis(l_posCenter,posBase);	
				l_dRadius = Arc->m_dRadius;//求半径
				//一个点与圆有两个垂点，
				l_posFoot.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
				l_posFoot.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
				l_posEnd.x = l_posCenter.x + l_dRadius*cos(l_dAgl1);
				l_posEnd.y = l_posCenter.y + l_dRadius*sin(l_dAgl1);
				//点到点的距离 ，看离哪个垂点近
				if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
				{
					m_posWillFoot = l_posFoot;//圆的第一个垂点
					return TRUE;
				}
				else if (posMove.Distance(l_posEnd)/g_pView->ReturnProportion()<= PRECISION)
				{
					m_posWillFoot = l_posEnd;//圆的第二个垂点
					return TRUE;
				}
			}
		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{//圆弧
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			g_pView->WorldToScreem(posMove,pot);
			l_iNumClick = Arc->IsPick(pot);
			if (l_iNumClick != -1)
			{
				l_posBegin = Arc->m_Begin;
				l_posEnd = Arc->m_End;
				l_posCenter = Arc->m_Center;
				l_dAgl = GetAngleToXAxis(posBase,l_posCenter);
				l_dAgl1 = GetAngleToXAxis(l_posCenter,l_posBegin);
				l_dAgl2 = GetAngleToXAxis(l_posCenter,l_posEnd);	
				if (Arc->m_iDirect == AD_CLOCKWISE)//如果是顺时针
				{
					if (l_dAgl1<l_dAgl2)//过零度线时，就是X轴正方向
					{
						if (!(l_dAgl<l_dAgl1||l_dAgl>l_dAgl2))
							l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
					}
					else
					{
						if (!(l_dAgl<l_dAgl1&&l_dAgl>l_dAgl2))
							l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
					}
				}
				else//逆时针
				{
					if (l_dAgl1>l_dAgl2)//过零度线时，就是X轴正方向
					{
						if (!(l_dAgl>l_dAgl1||l_dAgl<l_dAgl2))
							l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
					}
					else
					{
						if (!(l_dAgl>l_dAgl1&&l_dAgl<l_dAgl2))
							l_dAgl = GetAngleToXAxis(l_posCenter,posBase);
					}	
				}				
				l_dRadius = l_posBegin.Distance(l_posCenter);//求半径
				l_posFoot.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
				l_posFoot.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
				//点到点的距离 
				if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
				{
					m_posWillFoot = l_posFoot;
					return TRUE;
				}
			}
			//看是否在直线的两端
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//椭圆
			CCreateEllipse *Polyline = (CCreateEllipse*)l_pCreateBase;
			Polyline->GetBox(&pBox,TRUE);
			if (IsBelongBox(posBase,pBox))
			{
				g_pView->WorldToScreem(posBase,pot);
				l_iNumClick = Polyline->IsPick(pot);
				if (l_iNumClick != -1)
				{
					l_posBegin = Polyline->m_point.GetAt(l_iNumClick);
					l_posEnd = Polyline->m_point.GetAt(l_iNumClick+1);
					l_dTudu = Polyline->m_dTuDu.GetAt(l_iNumClick);
					if (l_dTudu == 0.0)//直线
					{
						l_posFoot = posBase.DistancePosFoot(l_posBegin,l_posEnd);
						//点到点的距离 
						if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
						{
							m_posWillFoot = l_posFoot;
							return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}

//画垂足点
void CDrawBase::DrawFootPoint(CDC* pDC,CPoint pot)
{
	pDC->MoveTo(pot.x-5,pot.y-5);
	pDC->LineTo(pot.x-5,pot.y+5);
	pDC->LineTo(pot.x+5,pot.y+5);

	pDC->MoveTo(pot.x,pot.y+5);
	pDC->LineTo(pot.x,pot.y);
	pDC->LineTo(pot.x-5,pot.y);
	
}

/*
 *	查找中点
 */
BOOL CDrawBase::FindPointMid(Position pos)
{
	//两个端点
	Position l_cBegin,l_cEnd,l_cMid,l_cCenter;
	double l_dAngle,l_dAngle1,l_dAngle2,l_dTudu,l_dRadius;
	//多段线里的点的个数
	int l_iCountOfPoint;
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase = NULL;
	while (POS != NULL)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		//直线//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			l_cMid = (line->m_Begin + line->m_End)/2;
			//看是否在直线的中点
			if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
			{
				m_posWillMid = l_cMid;
				return TRUE;
			}
		}		
		//多线段//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				l_cEnd = Polyline->m_point.GetAt(j+1);
				l_cMid = (l_cBegin+l_cEnd)/2;
				l_dTudu = Polyline->m_dTuDu.GetAt(j);
				//看是否在中点上
				if (l_dTudu == 0)
				{
					if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
					{
						m_posWillMid = l_cMid;
						return TRUE;
					}				
				}
				else if (l_dTudu < 0)//顺时针
				{
					g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_cCenter);
					l_dAngle1 = GetAngleToXAxis(l_cCenter,l_cBegin);
					l_dAngle2 = GetAngleToXAxis(l_cCenter,l_cEnd);
					if (l_dAngle1<l_dAngle2)//过零度线时，就是X轴正方向
						l_dAngle = l_dAngle1 + MATH_2PI - l_dAngle2;
					else
						l_dAngle = l_dAngle1 - l_dAngle2;
						
				}
				else//逆时针
				{
					g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_cCenter);
					l_dAngle1 = GetAngleToXAxis(l_cCenter,l_cBegin);
					l_dAngle2 = GetAngleToXAxis(l_cCenter,l_cEnd);	
					if (l_dAngle2<l_dAngle1)//过零度线时，就是X轴正方向
						l_dAngle = l_dAngle2 + MATH_2PI - l_dAngle1;
					else
						l_dAngle = l_dAngle2 - l_dAngle1;
				}	
				//求中点
				l_dAngle1 = GetAngleToXAxis(l_cCenter,l_cMid);
				l_dRadius = l_cCenter.Distance(l_cBegin);
				if (l_dAngle <= MATH_PI)
				{
					l_cMid.x = l_cCenter.x + l_dRadius*cos(l_dAngle1);
					l_cMid.y = l_cCenter.y + l_dRadius*sin(l_dAngle1);
				}
				else
				{
					l_cMid.x = l_cCenter.x + l_dRadius*cos(l_dAngle1+MATH_PI);
					l_cMid.y = l_cCenter.y + l_dRadius*sin(l_dAngle1+MATH_PI);				
				}
				//看是否在圆弧的中点
				if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillMid = l_cMid;
					return TRUE;
				}
			}
		}
		
		//矩形//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Polyline = (CCreateRectangle*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				l_cEnd = Polyline->m_point.GetAt(j+1);
				l_cMid = (l_cBegin+l_cEnd)/2;
				l_dTudu = Polyline->m_dTuDu.GetAt(j);
				//看是否在中点上
				if (l_dTudu == 0)
				{
					if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
					{
						m_posWillMid = l_cMid;
						return TRUE;
					}				
				}
				else if (l_dTudu < 0)//顺时针
				{
					g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_cCenter);
					l_dAngle1 = GetAngleToXAxis(l_cCenter,l_cBegin);
					l_dAngle2 = GetAngleToXAxis(l_cCenter,l_cEnd);
					if (l_dAngle1<l_dAngle2)//过零度线时，就是X轴正方向
						l_dAngle = l_dAngle1 + MATH_2PI - l_dAngle2;
					else
						l_dAngle = l_dAngle1 - l_dAngle2;
					
				}
				else//逆时针
				{
					g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_cCenter);
					l_dAngle1 = GetAngleToXAxis(l_cCenter,l_cBegin);
					l_dAngle2 = GetAngleToXAxis(l_cCenter,l_cEnd);	
					if (l_dAngle2<l_dAngle1)//过零度线时，就是X轴正方向
						l_dAngle = l_dAngle2 + MATH_2PI - l_dAngle1;
					else
						l_dAngle = l_dAngle2 - l_dAngle1;
				}	
				//求中点
				l_dAngle1 = GetAngleToXAxis(l_cCenter,l_cMid);
				l_dRadius = l_cCenter.Distance(l_cBegin);
				if (l_dAngle <= MATH_PI)
				{
					l_cMid.x = l_cCenter.x + l_dRadius*cos(l_dAngle1);
					l_cMid.y = l_cCenter.y + l_dRadius*sin(l_dAngle1);
				}
				else
				{
					l_cMid.x = l_cCenter.x + l_dRadius*cos(l_dAngle1+MATH_PI);
					l_cMid.y = l_cCenter.y + l_dRadius*sin(l_dAngle1+MATH_PI);				
				}
				//看是否在圆弧的中点
				if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillMid = l_cMid;
					return TRUE;
				}				
			}			
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//圆

		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{//圆弧
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			//看是否在直线的两端
			l_cMid = (Arc->m_Begin+Arc->m_End)/2;
			l_dAngle1 = GetAngleToXAxis(Arc->m_Center,Arc->m_Begin);
			l_dAngle2 = GetAngleToXAxis(Arc->m_Center,Arc->m_End);
			if (Arc->m_iDirect == AD_CLOCKWISE)
			{
				if (l_dAngle1<l_dAngle2)//过零度线时，就是X轴正方向
					l_dAngle = l_dAngle1 + MATH_2PI - l_dAngle2;
				else
					l_dAngle = l_dAngle1 - l_dAngle2;
			}
			else
			{
				if (l_dAngle2<l_dAngle1)//过零度线时，就是X轴正方向
					l_dAngle = l_dAngle2 + MATH_2PI - l_dAngle1;
				else
					l_dAngle = l_dAngle2 - l_dAngle1;				
			}
			l_dAngle1 = GetAngleToXAxis(Arc->m_Center,l_cMid);
			if (l_dAngle <= MATH_PI)
			{
				l_cMid.x = Arc->m_Center.x + Arc->m_dRadius*cos(l_dAngle1);
				l_cMid.y = Arc->m_Center.y + Arc->m_dRadius*sin(l_dAngle1);
			}
			else
			{
				l_cMid.x = Arc->m_Center.x + Arc->m_dRadius*cos(l_dAngle1+MATH_PI);
				l_cMid.y = Arc->m_Center.y + Arc->m_dRadius*sin(l_dAngle1+MATH_PI);				
			}
			//看是否在圆弧的中点
			if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
			{
				m_posWillMid = l_cMid;
				return TRUE;
			}
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//椭圆
			CCreateEllipse *Polyline = (CCreateEllipse*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				l_cEnd = Polyline->m_point.GetAt(j+1);
				l_cMid = (l_cBegin+l_cEnd)/2;
				//看是否在直线的两端
				if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillMid = l_cMid;
					return TRUE;
				}				
			}
		}
	}
	return FALSE;
}

/*
 *	画中点
 */
void CDrawBase::DrawMidPoint(CDC* pDC,CPoint pot)
{
	pDC->MoveTo(pot.x,pot.y-6);
	pDC->LineTo(pot.x-5,pot.y+3);
	pDC->LineTo(pot.x+5,pot.y+3);
	pDC->LineTo(pot.x,pot.y-6);
}

/*
 *	设定鼠标点下的点
 */
void CDrawBase::SetLButtonDownPot(Position& pos,int NumOfClick)
{
	if (m_bIsClickEnd == TRUE)
		pos = m_posWillEnd;	
	else if (m_bIsClickNear == TRUE)
		pos = m_posWillNear;
	else if (m_bIsClickFoot == TRUE)
		pos = m_posWillFoot;
	else if (m_bIsClickMid == TRUE)
		pos = m_posWillMid;
	else if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL&&m_iNumOfClick != 0)
		pos = m_posOrthogonal;
}

/*
 *	对象捕捉
 */
BOOL CDrawBase::FindPointAll(CDC* pDC,Position posBase,Position pos,int iCatch /* = 3 */)
{
	//转成屏幕坐标
	CPoint l_potRound;
	//建画笔
	CPen	pen; 	
	pen.CreatePen(PS_SOLID,1,RGB(0,127,255));
	CPen* pOldPen = pDC->SelectObject(&pen); // 得到原来的画笔
	pDC->SelectStockObject(NULL_BRUSH) ;	//把圆中间透明
	pDC->SetROP2(R2_XORPEN);
	/////////////////////////////////找端点
	if ((iCatch&END_POINT) == END_POINT)
	{
		BOOL l_bIsInEnd = FindPointEnd(pos);	
		if(!l_bIsInEnd)	//如果没有在端点上，就要擦除
		{	
			if(m_bIsClickEnd == TRUE)
			{
				g_pView->WorldToScreem(m_posPassEnd,l_potRound);
				CRect rect(l_potRound.x-4,l_potRound.y-4,l_potRound.x+4,l_potRound.y+4);
				pDC->Rectangle(rect);
				m_bIsClickEnd = FALSE;
			}
		}
		else//否则在端点上就要画个圆
		{
			if (m_bIsClickEnd == TRUE)
			{
				g_pView->WorldToScreem(m_posPassEnd,l_potRound);			
				CRect rect(l_potRound.x-4,l_potRound.y-4,l_potRound.x+4,l_potRound.y+4);
				pDC->Rectangle(rect);
			}
			//把之前的最近点画没，要不会出现最近点和端点
			if(m_bIsClickNear == TRUE)
			{
				g_pView->WorldToScreem(m_posPassNear,l_potRound);
				DrawNearPoint(pDC,l_potRound);
				m_bIsClickNear = FALSE;
			}
			g_pView->WorldToScreem(m_posWillEnd,l_potRound);		
			pDC->Rectangle(l_potRound.x-4, l_potRound.y-4, l_potRound.x+4, l_potRound.y+4) ;
			m_posPassEnd = m_posWillEnd;
			m_bIsClickEnd = TRUE;	
			// 恢复原来的画笔 
			pDC->SelectObject(pOldPen); 
			pen.DeleteObject(); 	
			return END_POINT;
		}
	}
	/////////////////////////////////找垂足点
	if ((iCatch&FOOT_POINT) == FOOT_POINT&&m_iNumOfClick >0)
	{
		BOOL l_bIsInFoot = FindPointFoot(posBase,pos);	
		if(!l_bIsInFoot)	//如果没有在垂足点，就要擦除
		{	
			if(m_bIsClickFoot == TRUE)
			{
				g_pView->WorldToScreem(m_posPassFoot,l_potRound);
				DrawFootPoint(pDC,l_potRound);
				m_bIsClickFoot = FALSE;
			}
		}
		else//否则在垂足点上就要画
		{
			if(m_bIsClickFoot == TRUE)
			{
				g_pView->WorldToScreem(m_posPassFoot,l_potRound);
				DrawFootPoint(pDC,l_potRound);
			}
			//把之前的最近点画没，要不会出现最近点和端点
			if(m_bIsClickNear == TRUE)
			{
				g_pView->WorldToScreem(m_posPassNear,l_potRound);
				DrawNearPoint(pDC,l_potRound);
				m_bIsClickNear = FALSE;
			}
			g_pView->WorldToScreem(m_posWillFoot,l_potRound);		
			DrawFootPoint(pDC,l_potRound);
			m_posPassFoot = m_posWillFoot;
			m_bIsClickFoot = TRUE;	
			// 恢复原来的画笔 
			pDC->SelectObject(pOldPen); 
			pen.DeleteObject(); 	
			return FOOT_POINT;
		}
	}
	/////////////////////////////////找中点
	if ((iCatch&MID_POINT) == MID_POINT)
	{
		BOOL l_bIsInMid = FindPointMid(pos);
		if(!l_bIsInMid)	//如果没有在中点，就要擦除
		{	
			if(m_bIsClickMid == TRUE)
			{
				g_pView->WorldToScreem(m_posPassMid,l_potRound);
				DrawMidPoint(pDC,l_potRound);
				m_bIsClickMid = FALSE;
			}
		}
		else//否则在中点上就要画
		{
			if(m_bIsClickMid == TRUE)
			{
				g_pView->WorldToScreem(m_posPassMid,l_potRound);
				DrawMidPoint(pDC,l_potRound);
			}
			//把之前的最近点画没，要不会出现最近点和端点
			if(m_bIsClickNear == TRUE)
			{
				g_pView->WorldToScreem(m_posPassNear,l_potRound);
				DrawNearPoint(pDC,l_potRound);
				m_bIsClickNear = FALSE;
			}
			g_pView->WorldToScreem(m_posWillMid,l_potRound);		
			DrawMidPoint(pDC,l_potRound);
			m_posPassMid = m_posWillMid;
			m_bIsClickMid = TRUE;	
			// 恢复原来的画笔 
			pDC->SelectObject(pOldPen); 
			pen.DeleteObject(); 	
			return MID_POINT;
		}
	}
	//////////////////////////////////////////找最近点
	if (NEAR_POINT == (iCatch&NEAR_POINT))
	{
		BOOL l_bIsInNear = FindPointNear(pos);
		if(!l_bIsInNear)	//如果没有在最近点上，就要擦除
		{	
			if(m_bIsClickNear == TRUE)
			{
				g_pView->WorldToScreem(m_posPassNear,l_potRound);
				DrawNearPoint(pDC,l_potRound);
				m_bIsClickNear = FALSE;
			}
		}
		else//否则在端点上就要画个圆
		{
			if (m_bIsClickNear == TRUE)
			{
				g_pView->WorldToScreem(m_posPassNear,l_potRound);	
				DrawNearPoint(pDC,l_potRound);
			}
			g_pView->WorldToScreem(m_posWillNear,l_potRound);		
			DrawNearPoint(pDC,l_potRound);
			m_posPassNear = m_posWillNear;
			m_bIsClickNear = TRUE;	
			// 恢复原来的画笔 
			pDC->SelectObject(pOldPen); 
			pen.DeleteObject();	
			return NEAR_POINT;
		}
	}
	if (ORTHOGONAL == (iCatch&ORTHOGONAL)&&m_iNumOfClick >0)
		RtnOrthogonalPos(posBase,pos);
	return FALSE;
}

/*
 *	反回正交点
 */
Position CDrawBase::RtnOrthogonalPos(Position posBase,Position posMove)
{
	double l_dAngle = GetAngleToXAxis(posBase,posMove);
	if (l_dAngle<=MATH_PI/4||l_dAngle>=MATH_PI*7/4||(l_dAngle<=MATH_PI*5/4&&l_dAngle>=MATH_PI*3/4))
		posMove.y = posBase.y;
	else if ((l_dAngle<=MATH_PI*3/4&&l_dAngle>=MATH_PI/4)||(l_dAngle<=MATH_PI*7/4&&l_dAngle>=MATH_PI*5/4))
		posMove.x = posBase.x;
	m_posOrthogonal = posMove;
	return m_posOrthogonal;
}

/*
 *	输入无效的内容
 */
int CDrawBase::InvalidContent()
{
				//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n输入无效的内容：",2);
	else
		RemindContent("\r\nThe content of the input is invalid：",2);			
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	return 0;
}