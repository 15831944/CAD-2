// DeleteMoreLine.cpp: implementation of the CDeleteMoreLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "DeleteMoreLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeleteMoreLine::CDeleteMoreLine()
{

}

CDeleteMoreLine::~CDeleteMoreLine()
{

}

/*
 *	返回离原点最近的点和元素和斜率
 */
BOOL CDeleteMoreLine::RtnPosBaseSlope()
{
	POSITION cpyListPos,ListPos = m_cpyBaseList.GetHeadPosition();
	//两个端点与原点的距离;
	double l_dDisBegin,l_dDisEnd,l_dDisNear = 99999999999;
	//			零点
	Position l_posBegin,l_posEnd,l_posOrig(0,0);
	CCreateBase* l_pCreateBase = NULL;
	while (ListPos != NULL)
	{	
		cpyListPos = ListPos;
		m_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;//直线
			l_posBegin = line->m_Begin;
			l_posEnd = line->m_End;
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{
			CCreateRound *Round = (CCreateRound*)l_pCreateBase;//圆
			l_posBegin = Round->m_EndPos;
			l_posEnd = Round->m_EndPos;
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;//圆弧
			l_posBegin = Arc->m_Begin;
			l_posEnd = Arc->m_End;
		}
		l_dDisBegin = l_posOrig.Distance(l_posBegin);
		l_dDisEnd = l_posOrig.Distance(l_posEnd);
		//取离原点最近的元素
		if (l_dDisBegin < l_dDisNear)
		{
			l_dDisNear = l_dDisBegin;
			m_pCreateBase = l_pCreateBase;
		}
		if (l_dDisEnd < l_dDisNear)
		{
			l_dDisNear = l_dDisEnd;
			m_pCreateBase = l_pCreateBase;	
			//改变切割方向
			Changedir(); 
		}	
	}
	//存到新链表里
	g_pDoc->m_CCreateBaseList.AddTail(m_pCreateBase);
	//移除复制链表里的
	m_cpyBaseList.RemoveAt(cpyListPos);		
	return TRUE;
}

/*
 *	接口函数
 */
void CDeleteMoreLine::DeleteMoreLine()
{
	//复制一个所有图形的链表，并把图形分成直线和圆弧
	CopyList();
	//清空基础链表
	ClearBaseList();
	POSITION cpyListPos,ListPos = m_cpyBaseList.GetHeadPosition();
//	ListPos = NULL;
	//去重刀
	while (ListPos)
	{		
		cpyListPos = ListPos;
		m_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
		
		if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			if(CleckLine())
				ListPos = m_cpyBaseList.GetHeadPosition();
		}
		else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{
			if(CleckRound())
				ListPos = m_cpyBaseList.GetHeadPosition();	
		}
		else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
			if(CleckArc())
				ListPos = m_cpyBaseList.GetHeadPosition();
		}
	}
	RtnPosBaseSlope();
	
	
/*
	//是否从下一个连接图形开始的标志
	BOOL l_bIsNext = TRUE;
1
	//多段线中的点集
	int l_iSizeOfPoint;
	ListPos = m_cpyBaseList.GetHeadPosition();
	POSITION CurListPos;
	cpyListPos = ListPos;
	//剩余图形里的循环
	while (ListPos != NULL)
	{		
		if(l_bIsNext == TRUE)//进入一个不相连图形的起点，并把末点记下来
		{

		}
		//看是否有跟末点相同的吗
		while (ListPos != NULL)
		{
			cpyListPos = ListPos;
			CCreateBase* l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
			
			if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				CCreateLine *line = (CCreateLine*)l_pCreateBase;//直线
				l_PosBegin = line->m_Begin;
				l_PosEnd = line->m_End;
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
			{
				CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;//多线段
				l_iSizeOfPoint = Polyline->m_point.GetSize()-1;
				l_PosBegin = Polyline->m_point.GetAt(0);
				l_PosEnd = Polyline->m_point.GetAt(l_iSizeOfPoint);
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{
				CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;//矩形
				l_iSizeOfPoint = Rectangle->m_point.GetSize()-1;
				l_PosBegin = Rectangle->m_point.GetAt(0);
				l_PosEnd = Rectangle->m_point.GetAt(l_iSizeOfPoint);
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{
				CCreateRound *Round = (CCreateRound*)l_pCreateBase;//圆
				l_PosBegin = Round->m_EndPos;
				l_PosEnd = Round->m_EndPos;
			}
			
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
			{
				CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;//圆弧
				l_PosBegin = Arc->m_Begin;
				l_PosEnd = Arc->m_End;
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{
				CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;//椭圆
				l_PosBegin = Ellipse->m_Begin;
				l_PosEnd = Ellipse->m_End;
			}
			//判断是否在同一点上
			if(PosEqual(m_PosEnd,l_PosBegin))//如果末点与起点相接
			{
				m_PosEnd = l_PosEnd;
				g_pDoc->m_CCreateBaseList.AddTail(l_pCreateBase);
				l_bIsNext = FALSE;
				break;
			}
			else if(PosEqual(m_PosEnd,l_PosEnd))//如果末点与末点相接
			{
				m_ChangeDir = l_pCreateBase;
				Changedir();
				m_PosEnd = l_PosBegin;
				g_pDoc->m_CCreateBaseList.AddTail(l_pCreateBase);
				l_bIsNext = FALSE;
				break;
			}
			l_bIsNext = TRUE;			
		}
		//如果一末点没有相同的，看是否有跟起点相同的吗
		if(l_bIsNext == TRUE)
		{			
			ListPos = m_cpyBaseList.GetHeadPosition();	
			while (ListPos != NULL)
			{
				cpyListPos = ListPos;
				CCreateBase* l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
				
				if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
				{
					CCreateLine *line = (CCreateLine*)l_pCreateBase;//直线
					l_PosBegin = line->m_Begin;
					l_PosEnd = line->m_End;
				}
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
				{
					CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;//多线段
					l_iSizeOfPoint = Polyline->m_point.GetSize()-1;
					l_PosBegin = Polyline->m_point.GetAt(0);
					l_PosEnd = Polyline->m_point.GetAt(l_iSizeOfPoint);;
				}
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
				{
					CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;//矩形
					l_iSizeOfPoint = Rectangle->m_point.GetSize()-1;
					l_PosBegin = Rectangle->m_point.GetAt(0);
					l_PosEnd = Rectangle->m_point.GetAt(l_iSizeOfPoint);
				}
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
				{
					CCreateRound *Round = (CCreateRound*)l_pCreateBase;//圆
					l_PosBegin = Round->m_EndPos;
					l_PosEnd = Round->m_EndPos;
				}
				
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
				{
					CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;//圆弧
					l_PosBegin = Arc->m_Begin;
					l_PosEnd = Arc->m_End;
				}
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
				{
					CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;//椭圆
					l_PosBegin = Ellipse->m_Begin;
					l_PosEnd = Ellipse->m_End;
				}
				//判断是否在同一点上
				if(PosEqual(m_PosBegin,l_PosBegin))//如果起点与起点相接
				{
					m_ChangeDir = l_pCreateBase;
					Changedir();
					m_PosBegin = l_PosEnd;
					CurListPos = g_pDoc->m_CCreateBaseList.InsertBefore(CurListPos,l_pCreateBase);
					l_bIsNext = FALSE;
					break;
				}
				else if(PosEqual(m_PosBegin,l_PosEnd))//如果起点与末点相接
				{
					m_PosBegin = l_PosBegin;
					CurListPos = g_pDoc->m_CCreateBaseList.InsertBefore(CurListPos,l_pCreateBase);
					l_bIsNext = FALSE;
					break;
				}
				l_bIsNext = TRUE;			
			}
		}
		if (l_bIsNext == FALSE)
			m_cpyBaseList.RemoveAt(cpyListPos);
		ListPos = m_cpyBaseList.GetHeadPosition();			
	}		
	
	
	
	
	*/

	
	g_pDoc->m_CCreateBaseList.AddTail(&m_cpyBaseList);
	g_pView->Invalidate();
}

/*
 *	检测直线是否与其它线重合
 */
BOOL CDeleteMoreLine::CleckLine()
{
	POSITION ListPos = m_cpyBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase;
	//剩余图形里的循环
	while (ListPos != NULL)
	{	
		l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
		//不与自己比较
		if (l_pCreateBase != m_pCreateBase)
		{
			if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				CCreateLine* pLineOri = (CCreateLine*)m_pCreateBase;//直线
				CCreateLine *pLineCur = (CCreateLine*)l_pCreateBase;//直线
				//先排除直线的点在延长线上
				double angle1 = GetAngleThreePos(pLineOri->m_Begin,pLineCur->m_Begin,pLineCur->m_End);
				double angle2 = GetAngleThreePos(pLineOri->m_Begin,pLineCur->m_End,pLineCur->m_Begin);
				double angle3 = GetAngleThreePos(pLineOri->m_End,pLineCur->m_Begin,pLineCur->m_End);
				double angle4 = GetAngleThreePos(pLineOri->m_End,pLineCur->m_End,pLineCur->m_Begin);
				
				if(angle1 <= MATH_PI/2&&angle2 <= MATH_PI/2&&angle3 <= MATH_PI/2&&angle4 <= MATH_PI/2)
				{				
					//根据直线两点到线的距离看两条线重合吗
					double l_dDis1 = pLineOri->m_Begin.PotDisLine(pLineCur->m_Begin,pLineCur->m_End);
					double l_dDis2 = pLineOri->m_End.PotDisLine(pLineCur->m_Begin,pLineCur->m_End);
					if(l_dDis1<DISTANCE_ZERO&&l_dDis2<DISTANCE_ZERO)
					{
						POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
						m_cpyBaseList.RemoveAt(POS);
						return TRUE;
					}
				}
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{
				CCreateLine* pLineOri = (CCreateLine*)m_pCreateBase;//直线
				CCreatePolyline *pPolylineCur = (CCreatePolyline*)l_pCreateBase;//直线
				int l_iSize = pPolylineCur->m_point.GetSize()-1;
				//多段线上的两个点
				Position l_posBegin,l_posEnd,l_posMid,l_pos;
				double l_dtudu;
				for(int i = 0;i<l_iSize;i++)
				{
					l_dtudu = pPolylineCur->m_dTuDu.GetAt(i);
					if(l_dtudu == 0.0)//说明是直线
					{
						l_posBegin = pPolylineCur->m_point.GetAt(i);
						l_posEnd = pPolylineCur->m_point.GetAt(i+1);
						l_pos = l_posEnd;
						//如果多点在一条直线上
						while(i<l_iSize-1)
						{
							i++;
							l_posMid = pPolylineCur->m_point.GetAt(i+1);	
							//看下一点是否在前两点的延长线上吗
							double l_dDis = l_posMid.PotDisLine(l_posBegin,l_pos);
							if (l_dDis>DISTANCE_ZERO)
							{
								i--;
								break;
							}
							else
								l_posEnd = l_posMid;
						}	
						//先排除直线的点在延长线上
						double angle1 = GetAngleThreePos(pLineOri->m_Begin,l_posBegin,l_posEnd);
						double angle2 = GetAngleThreePos(pLineOri->m_Begin,l_posEnd,l_posBegin);
						double angle3 = GetAngleThreePos(pLineOri->m_End,l_posBegin,l_posEnd);
						double angle4 = GetAngleThreePos(pLineOri->m_End,l_posEnd,l_posBegin);
						if(angle1 <= MATH_PI/2&&angle2 <= MATH_PI/2&&angle3 <= MATH_PI/2&&angle4 <= MATH_PI/2)
						{				
							//根据直线两点到线的距离看两条线重合吗
							double l_dDis1 = pLineOri->m_Begin.PotDisLine(l_posBegin,l_posEnd);
							double l_dDis2 = pLineOri->m_End.PotDisLine(l_posBegin,l_posEnd);
							if(l_dDis1<DISTANCE_ZERO&&l_dDis2<DISTANCE_ZERO)
							{
								POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
								m_cpyBaseList.RemoveAt(POS);
								return TRUE;
							}
						}
					}					
				}
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{				
				CCreateLine* pLineOri = (CCreateLine*)m_pCreateBase;//直线
				CCreateRectangle *pPolylineCur = (CCreateRectangle*)l_pCreateBase;//直线
				int l_iSize = pPolylineCur->m_point.GetSize()-1;
				//多段线上的两个点
				Position l_posBegin,l_posEnd;
				double l_dtudu;
				for(int i = 0;i<l_iSize;i++)
				{
					l_dtudu = pPolylineCur->m_dTuDu.GetAt(i);
					if(l_dtudu == 0.0)//说明是直线
					{
						l_posBegin = pPolylineCur->m_point.GetAt(i);
						l_posEnd = pPolylineCur->m_point.GetAt(i+1);
						//先排除直线的点在延长线上
						double angle1 = GetAngleThreePos(pLineOri->m_Begin,l_posBegin,l_posEnd);
						double angle2 = GetAngleThreePos(pLineOri->m_Begin,l_posEnd,l_posBegin);
						double angle3 = GetAngleThreePos(pLineOri->m_End,l_posBegin,l_posEnd);
						double angle4 = GetAngleThreePos(pLineOri->m_End,l_posEnd,l_posBegin);
						if(angle1 <= MATH_PI/2&&angle2 <= MATH_PI/2&&angle3 <= MATH_PI/2&&angle4 <= MATH_PI/2)
						{				
							//根据直线两点到线的距离看两条线重合吗
							double l_dDis1 = pLineOri->m_Begin.PotDisLine(l_posBegin,l_posEnd);
							double l_dDis2 = pLineOri->m_End.PotDisLine(l_posBegin,l_posEnd);
							if(l_dDis1<DISTANCE_ZERO&&l_dDis2<DISTANCE_ZERO)
							{
								POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
								m_cpyBaseList.RemoveAt(POS);
								return TRUE;
							}
						}
					}					
				}
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{
				CCreateLine* pLineOri = (CCreateLine*)m_pCreateBase;//直线
				CCreateEllipse *pPolylineCur = (CCreateEllipse*)l_pCreateBase;//直线
				int l_iSize = pPolylineCur->m_point.GetSize()-1;
				//多段线上的两个点
				Position l_posBegin,l_posEnd;
				double l_dtudu;
				for(int i = 0;i<l_iSize;i++)
				{
					l_dtudu = pPolylineCur->m_dTuDu.GetAt(i);
					if(l_dtudu == 0.0)//说明是直线
					{
						l_posBegin = pPolylineCur->m_point.GetAt(i);
						l_posEnd = pPolylineCur->m_point.GetAt(i+1);
						//先排除直线的点在延长线上
						double angle1 = GetAngleThreePos(pLineOri->m_Begin,l_posBegin,l_posEnd);
						double angle2 = GetAngleThreePos(pLineOri->m_Begin,l_posEnd,l_posBegin);
						double angle3 = GetAngleThreePos(pLineOri->m_End,l_posBegin,l_posEnd);
						double angle4 = GetAngleThreePos(pLineOri->m_End,l_posEnd,l_posBegin);
						if(angle1 <= MATH_PI/2&&angle2 <= MATH_PI/2&&angle3 <= MATH_PI/2&&angle4 <= MATH_PI/2)
						{				
							//根据直线两点到线的距离看两条线重合吗
							double l_dDis1 = pLineOri->m_Begin.PotDisLine(l_posBegin,l_posEnd);
							double l_dDis2 = pLineOri->m_End.PotDisLine(l_posBegin,l_posEnd);
							if(l_dDis1<DISTANCE_ZERO&&l_dDis2<DISTANCE_ZERO)
							{
								POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
								m_cpyBaseList.RemoveAt(POS);
								return TRUE;
							}
						}
					}					
				}
			}
		}
	}
	return FALSE;
}

/*
 *	检测圆是否与其它线重合
 */
BOOL CDeleteMoreLine::CleckRound()
{
	POSITION ListPos = m_cpyBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase;
	//剩余图形里的循环
	while (ListPos != NULL)
	{	
		l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
		//不与自己比较
		if (l_pCreateBase != m_pCreateBase)
		{
			if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{
				CCreateRound* pLineOri = (CCreateRound*)m_pCreateBase;
				CCreateRound *pLineCur = (CCreateRound*)l_pCreateBase;
				if (pLineOri->m_Center == pLineCur->m_Center&&pLineCur->m_dRadius == pLineOri->m_dRadius)
				{
					POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
					m_cpyBaseList.RemoveAt(POS);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
	
}

/*
 *	检测圆弧是否与其它线重合
 */
BOOL CDeleteMoreLine::CleckArc()
{
	POSITION ListPos = m_cpyBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase;
	//剩余图形里的循环
	while (ListPos != NULL)
	{	
		l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
		//不与自己比较
		if (l_pCreateBase != m_pCreateBase)
		{
			if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
			{
				CCreateRoundArc* pLineOri = (CCreateRoundArc*)m_pCreateBase;
				CCreateRoundArc *pLineCur = (CCreateRoundArc*)l_pCreateBase;
				if (pLineOri->m_Center == pLineCur->m_Center&&fabs(pLineCur->m_dRadius-pLineOri->m_dRadius)<DISTANCE_ZERO)
				{
					BOOL l_bCover = FALSE;//标志是否重合
					//如果都是顺时针
					if (pLineOri->m_iDirect == AD_CLOCKWISE&&pLineCur->m_iDirect == AD_CLOCKWISE)
					{//都是顺时针
						if (pLineOri->m_dAngleStart<=pLineCur->m_dAngleStart&&pLineOri->m_dAngleEnd>=pLineCur->m_dAngleEnd)
							l_bCover = TRUE;							
					}
					else if (pLineOri->m_iDirect == AD_COUNTERCLOCKWISE&&pLineCur->m_iDirect == AD_COUNTERCLOCKWISE)
					{//都是逆时针
						if (pLineOri->m_dAngleStart>=pLineCur->m_dAngleStart&&pLineOri->m_dAngleEnd<=pLineCur->m_dAngleEnd)
							l_bCover = TRUE;							
					}
					else if (pLineOri->m_iDirect == AD_CLOCKWISE&&pLineCur->m_iDirect == AD_COUNTERCLOCKWISE)
					{//原图是顺时针,对比图是
						if (pLineOri->m_dAngleStart>=pLineCur->m_dAngleStart&&pLineOri->m_dAngleEnd<=pLineCur->m_dAngleEnd)
							l_bCover = TRUE;							
					}
					else if (pLineOri->m_iDirect == AD_COUNTERCLOCKWISE&&pLineCur->m_iDirect == AD_CLOCKWISE)
					{//
						if (pLineOri->m_dAngleStart>=pLineCur->m_dAngleStart&&pLineOri->m_dAngleEnd<=pLineCur->m_dAngleEnd)
							l_bCover = TRUE;							
					}
					if (l_bCover == TRUE)
					{
						POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
						m_cpyBaseList.RemoveAt(POS);
						return TRUE;
					}
				}
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{
				CCreateRoundArc* pLineOri = (CCreateRoundArc*)m_pCreateBase;
				CCreateRound *pLineCur = (CCreateRound*)l_pCreateBase;
				if (pLineOri->m_Center == pLineCur->m_Center&&pLineCur->m_dRadius == pLineOri->m_dRadius)
				{
					POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
					m_cpyBaseList.RemoveAt(POS);
					return TRUE;
				}
			}
		}
	}
	return FALSE;	
}


/*
 *	把所有的图形链表复制一下并分成只有直线和圆弧
 */
void CDeleteMoreLine::CopyList()
{
//	m_cpyBaseList.AddTail(&(g_pDoc->m_CCreateBaseList));
	POSITION cpyListPos,ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	//把所有图形都变成直线和圆弧的
	while (ListPos)
	{		
		cpyListPos = ListPos;
		m_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(ListPos);
		
		if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))||
			m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			AnlysPolyline();//分解多段线,并把多段线分成直线和圆弧
		else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			AnlysRect();//分解矩形,并把矩形分成直线和圆弧		
		else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			AnlysEllipse();//分解椭圆,并把椭圆分成直线
		else//如果是其它的就直接存进链表里了
			m_cpyBaseList.AddTail(m_pCreateBase);
	}
}

/*
 *	分解多段线
 */
void CDeleteMoreLine::AnlysPolyline()
{
	CCreatePolyline* pPolyline = (CCreatePolyline*)m_pCreateBase;
	//圆弧方向		点集的个数
	int	l_iDir,l_iSize = pPolyline->m_point.GetSize();
	//起点，终点，圆心
	Position l_posBegin,l_posEnd,l_posCenter;
	//凸度
	double l_dTudu;
	for(int i = 0;i <= l_iSize-2;i++)
	{
		l_posBegin = pPolyline->m_point.GetAt(i);
		l_posEnd = pPolyline->m_point.GetAt(i+1);
		l_dTudu = pPolyline->m_dTuDu.GetAt(i);
		//说明是直线
		if (l_dTudu == 0.0)
		{	//把直线存进新链表
			CCreateLine *pLine = new CCreateLine(l_posBegin,l_posEnd);
			pLine->m_ColorOfLine = m_pCreateBase->m_ColorOfLine;
			pLine->m_strCBM.Copy(m_pCreateBase->m_strCBM);	
			pLine->m_strSBM.Copy(m_pCreateBase->m_strSBM);	
			m_cpyBaseList.AddTail(pLine);			
		}
		else//说明是圆弧
		{
			//如果是顺时针
			if (l_dTudu<0.0) l_iDir = AD_CLOCKWISE;
			else l_iDir = AD_COUNTERCLOCKWISE;			
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);
			CCreateRoundArc* pArc = new CCreateRoundArc(l_posBegin,l_posEnd,l_posCenter,l_iDir);	
			pArc->m_ColorOfLine = m_pCreateBase->m_ColorOfLine;
			pArc->m_strCBM.Copy(m_pCreateBase->m_strCBM);	
			pArc->m_strSBM.Copy(m_pCreateBase->m_strSBM);	
			m_cpyBaseList.AddTail(pArc);						
		}
	}
}


/*
 *	分解矩形
 */
void CDeleteMoreLine::AnlysRect()
{
	CCreateRectangle* pPolyline = (CCreateRectangle*)m_pCreateBase;
	//圆弧方向		点集的个数
	int	l_iDir,l_iSize = pPolyline->m_point.GetSize();
	//起点，终点，圆心
	Position l_posBegin,l_posEnd,l_posCenter;
	//凸度
	double l_dTudu;
	for(int i = 0;i <= l_iSize-2;i++)
	{
		l_posBegin = pPolyline->m_point.GetAt(i);
		l_posEnd = pPolyline->m_point.GetAt(i+1);
		l_dTudu = pPolyline->m_dTuDu.GetAt(i);
		//说明是直线
		if (l_dTudu == 0.0)
		{	//把直线存进新链表
			CCreateLine *pLine = new CCreateLine(l_posBegin,l_posEnd);
			pLine->m_ColorOfLine = m_pCreateBase->m_ColorOfLine;
			pLine->m_strCBM.Copy(m_pCreateBase->m_strCBM);	
			pLine->m_strSBM.Copy(m_pCreateBase->m_strSBM);	
			m_cpyBaseList.AddTail(pLine);			
		}
		else//说明是圆弧
		{
			//如果是顺时针
			if (l_dTudu<0.0) l_iDir = AD_CLOCKWISE;
			else l_iDir = AD_COUNTERCLOCKWISE;			
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);
			CCreateRoundArc* pArc = new CCreateRoundArc(l_posBegin,l_posEnd,l_posCenter,l_iDir);
			pArc->m_ColorOfLine = m_pCreateBase->m_ColorOfLine;
			pArc->m_strCBM.Copy(m_pCreateBase->m_strCBM);	
			pArc->m_strSBM.Copy(m_pCreateBase->m_strSBM);	
			m_cpyBaseList.AddTail(pArc);						
		}
	}
}


/*
 *	分解椭圆
 */
void CDeleteMoreLine::AnlysEllipse()
{
	CCreateEllipse* pPolyline = (CCreateEllipse*)m_pCreateBase;
	//	点集的个数
	int	l_iSize = pPolyline->m_point.GetSize();
	//起点，终点
	Position l_posBegin,l_posEnd;
	for(int i = 0;i <= l_iSize-2;i++)
	{
		l_posBegin = pPolyline->m_point.GetAt(i);
		l_posEnd = pPolyline->m_point.GetAt(i+1);
		//把直线存进新链表
		CCreateLine *pLine = new CCreateLine(l_posBegin,l_posEnd);
		pLine->m_ColorOfLine = m_pCreateBase->m_ColorOfLine;
		pLine->m_strCBM.Copy(m_pCreateBase->m_strCBM);	
		pLine->m_strSBM.Copy(m_pCreateBase->m_strSBM);	
		m_cpyBaseList.AddTail(pLine);			
	}
}

/*
 *	清空基础链表
 */
void CDeleteMoreLine::ClearBaseList()
{
	//清空所有图元	
	g_pDoc->m_CCreateBaseList.RemoveAll();
	//清空选择集图元
	g_pDoc->m_CollectOfPicked.RemoveAll();
}


/*
 *	改变切割方向
 */
void CDeleteMoreLine::Changedir() 
{
	// TODO: Add your control notification handler code here
	if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		//把起点和终点换个位置
		Position pos = ((CCreateLine*)m_pCreateBase)->m_Begin;
		((CCreateLine*)m_pCreateBase)->m_Begin = ((CCreateLine*)m_pCreateBase)->m_End;
		((CCreateLine*)m_pCreateBase)->m_End = pos;
		//直线
	}
	else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))//圆弧
	{	//把起点和终点坐标位置换一下！
		Position pos = ((CCreateRoundArc*)m_pCreateBase)->m_Begin;
		((CCreateRoundArc*)m_pCreateBase)->m_Begin =	\
			((CCreateRoundArc*)m_pCreateBase)->m_End;
		((CCreateRoundArc*)m_pCreateBase)->m_End = pos;
		//改变圆的切割方向
		if(((CCreateRoundArc*)m_pCreateBase)->m_iDirect == AD_CLOCKWISE)
			((CCreateRoundArc*)m_pCreateBase)->m_iDirect = AD_COUNTERCLOCKWISE;
		else
			((CCreateRoundArc*)m_pCreateBase)->m_iDirect = AD_CLOCKWISE;
		((CCreateRoundArc*)m_pCreateBase)->m_dAngleStart = GetAngle(((CCreateRoundArc*)m_pCreateBase)->m_Center, ((CCreateRoundArc*)m_pCreateBase)->m_Begin);//
		((CCreateRoundArc*)m_pCreateBase)->m_dAngleEnd = GetAngle(((CCreateRoundArc*)m_pCreateBase)->m_Center, ((CCreateRoundArc*)m_pCreateBase)->m_End);
		
	}
}
