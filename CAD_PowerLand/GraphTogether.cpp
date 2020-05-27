// GraphTogether.cpp: implementation of the CGraphTogether class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "GraphTogether.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphTogether::CGraphTogether()
{

}

CGraphTogether::~CGraphTogether()
{
	m_cpyBaseList.RemoveAll();
}

/*
 *	使单位图形合并
 */
void CGraphTogether::PutGraphTogether()
{
	//复制一个所有图形的链表
	CopyList();
	//清空基础链表
	ClearBaseList();
	//是否从下一个连接图形开始的标志
	BOOL l_bIsNext = TRUE;
	//每个图形的起末点
	Position l_PosBegin,l_PosEnd;
	//多段线中的点集
	int l_iSizeOfPoint;
	//以下是从新排序//
	POSITION ListPos = m_cpyBaseList.GetHeadPosition();
	POSITION CurListPos,cpyListPos = ListPos;
	//剩余图形里的循环
	while (ListPos != NULL)
	{		
		if(l_bIsNext == TRUE)//进入一个不相连图形的起点，并把末点记下来
		{
			cpyListPos = ListPos;
			CCreateBase* l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
			if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				CCreateLine *line = (CCreateLine*)l_pCreateBase;//直线
				m_PosBegin = line->m_Begin;
				m_PosEnd = line->m_End;
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{
				CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;//多线段
				l_iSizeOfPoint = Polyline->m_point.GetSize()-1;
				m_PosBegin = Polyline->m_point.GetAt(0);
				m_PosEnd = Polyline->m_point.GetAt(l_iSizeOfPoint);
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{
				CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;//矩形
				l_iSizeOfPoint = Rectangle->m_point.GetSize()-1;
				m_PosBegin = Rectangle->m_point.GetAt(0);
				m_PosEnd = Rectangle->m_point.GetAt(l_iSizeOfPoint);
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{
				CCreateRound *Round = (CCreateRound*)l_pCreateBase;//圆
				m_PosBegin = Round->m_EndPos;
				m_PosEnd = Round->m_EndPos;
			}
			
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
			{
				CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;//圆弧
				m_PosBegin = Arc->m_Begin;
				m_PosEnd = Arc->m_End;
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{
				CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;//椭圆
				m_PosBegin = Ellipse->m_Begin;
				m_PosEnd = Ellipse->m_End;
			}
			//把起点存到新链表里
			g_pDoc->m_CCreateBaseList.AddTail(l_pCreateBase);
			CurListPos = g_pDoc->m_CCreateBaseList.GetTailPosition();
			//移除复制链表里的
			m_cpyBaseList.RemoveAt(cpyListPos);		
			ListPos = m_cpyBaseList.GetHeadPosition();
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
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
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
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
					||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
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
}

/*
 *	把所有的图形链表复制一下
 */
void CGraphTogether::CopyList()
{
	//向撤消链表里添加操作
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_TOGETHER;

	PGRAPHPOS CurGraphPos,GraphPos;
	CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = CurGraphPos;
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	while(position != NULL)
	{
		CCreateBase* l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
		m_cpyBaseList.AddTail(l_pCreateBase);	
		//向撤消链表里添加
		if (CurGraphPos->pCreateBasepos == NULL)
			CurGraphPos->pCreateBasepos = (LONG)l_pCreateBase;
		else
		{
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (LONG)l_pCreateBase;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		} 
	}
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}

/*
 *	清空基础链表
 */
void CGraphTogether::ClearBaseList()
{
	//清空所有图元	
	g_pDoc->m_CCreateBaseList.RemoveAll();
	//清空选择集图元
	g_pDoc->m_CollectOfPicked.RemoveAll();
}

/*
 *	改变切割方向
 */
void CGraphTogether::Changedir() 
{
	// TODO: Add your control notification handler code here
	if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		//把起点和终点换个位置
		Position pos = ((CCreateLine*)m_ChangeDir)->m_Begin;
		((CCreateLine*)m_ChangeDir)->m_Begin = ((CCreateLine*)m_ChangeDir)->m_End;
		((CCreateLine*)m_ChangeDir)->m_End = pos;
		//直线
	}
	else if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateSpline)))//多线段
	{	
		Position l_ScreemPos;
		double l_dTude;
		CArray<Position,Position&> l_ListPoint; 
		CArray<double,double&> l_ListTuDu;//存放凸度的
		for(int i=((CCreatePolyline*)m_ChangeDir)->m_point.GetSize();  i>1;  i--)
		{
			l_ScreemPos = ((CCreatePolyline*)m_ChangeDir)->m_point.GetAt(i-1); 
			l_ListPoint.Add(l_ScreemPos);
			l_dTude = -(((CCreatePolyline*)m_ChangeDir)->m_dTuDu.GetAt(i-2)); 
			l_ListTuDu.Add(l_dTude);
		}
		l_ScreemPos = ((CCreatePolyline*)m_ChangeDir)->m_point.GetAt(0); 
		l_ListPoint.Add(l_ScreemPos);
		l_dTude = 0.0;
		l_ListTuDu.Add(l_dTude);
		((CCreatePolyline*)m_ChangeDir)->m_point.Copy(l_ListPoint);
		((CCreatePolyline*)m_ChangeDir)->m_dTuDu.Copy(l_ListTuDu);
	}
	else if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))//矩形
	{
		Position l_ScreemPos;
		double l_dTude;
		CArray<Position,Position&> l_ListPoint; 
		CArray<double,double&> l_ListTuDu;//存放凸度的
		for(int i=((CCreateRectangle*)m_ChangeDir)->m_point.GetSize();  i>1;  i--)
		{
			l_ScreemPos = ((CCreateRectangle*)m_ChangeDir)->m_point.GetAt(i-1); 
			l_ListPoint.Add(l_ScreemPos);
			l_dTude = -(((CCreateRectangle*)m_ChangeDir)->m_dTuDu.GetAt(i-2)); 
			l_ListTuDu.Add(l_dTude);
		}
		l_ScreemPos = ((CCreateRectangle*)m_ChangeDir)->m_point.GetAt(0); 
		l_ListPoint.Add(l_ScreemPos);
		l_dTude = 0.0;
		l_ListTuDu.Add(l_dTude);
		((CCreateRectangle*)m_ChangeDir)->m_point.Copy(l_ListPoint);
		((CCreateRectangle*)m_ChangeDir)->m_dTuDu.Copy(l_ListTuDu);	}
	else if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateRound)))//圆
	{	//改变圆的切割方向
		if(((CCreateRound*)m_ChangeDir)->m_iDirect == AD_CLOCKWISE)
			((CCreateRound*)m_ChangeDir)->m_iDirect = AD_COUNTERCLOCKWISE;
		else
			((CCreateRound*)m_ChangeDir)->m_iDirect = AD_CLOCKWISE;
	}
	
	else if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))//圆弧
	{	//把起点和终点坐标位置换一下！
		Position pos = ((CCreateRoundArc*)m_ChangeDir)->m_Begin;
		((CCreateRoundArc*)m_ChangeDir)->m_Begin =	\
			((CCreateRoundArc*)m_ChangeDir)->m_End;
		((CCreateRoundArc*)m_ChangeDir)->m_End = pos;
		//改变圆的切割方向
		if(((CCreateRoundArc*)m_ChangeDir)->m_iDirect == AD_CLOCKWISE)
			((CCreateRoundArc*)m_ChangeDir)->m_iDirect = AD_COUNTERCLOCKWISE;
		else
			((CCreateRoundArc*)m_ChangeDir)->m_iDirect = AD_CLOCKWISE;
		((CCreateRoundArc*)m_ChangeDir)->m_dAngleStart = GetAngle(((CCreateRoundArc*)m_ChangeDir)->m_Center, ((CCreateRoundArc*)m_ChangeDir)->m_Begin);//
		((CCreateRoundArc*)m_ChangeDir)->m_dAngleEnd = GetAngle(((CCreateRoundArc*)m_ChangeDir)->m_Center, ((CCreateRoundArc*)m_ChangeDir)->m_End);
		
	}
	else if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))//椭圆
	{
		Position l_ScreemPos;
		CArray<Position,Position&> l_ListPoint; 
		for(int i=((CCreateEllipse*)m_ChangeDir)->m_point.GetSize();  i>0;  i--)
		{
			l_ScreemPos = ((CCreateEllipse*)m_ChangeDir)->m_point.GetAt(i-1); 
			l_ListPoint.Add(l_ScreemPos);			
		}
		((CCreateEllipse*)m_ChangeDir)->m_point.Copy(l_ListPoint);
	}
}
