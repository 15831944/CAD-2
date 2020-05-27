// TurnMill.cpp: implementation of the CTurnMill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "TurnMill.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTurnMill::CTurnMill()
{
	m_pMillGraph = NULL ;
	m_pUndoList = NULL;
}

CTurnMill::~CTurnMill()
{

}

/*
 *	把图形变为要铣的图形
 */
void CTurnMill::TurnGraphMill()
{
	POSITION pos = NULL ;
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize() ;
	//是否有要处理的图形的标志
	BOOL l_bIsAdd = FALSE;
	//把选择集里的图形分类
	for( int i = 0 ; i < l_iSize; i++ )
	{
		m_pMillGraph = (CCreateBase*)g_pDoc->m_CollectOfPicked[i] ;
		if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			l_bIsAdd = TRUE;
			TurnLine();
		}
		
		//多线段//////////////////////////////////////////
		else if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreatePolyline))||
			m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)m_pMillGraph;
			l_bIsAdd = TRUE;
			//如果不是闭合的话 			
			if (!PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))
				TurnOpenPolyline();
			else
				TurnClosePolyline();
		}
		
		//矩形//////////////////////////////////////////
		else if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			l_bIsAdd = TRUE;
			CCreatePolyline *Polyline = (CCreatePolyline*)m_pMillGraph;		
			TurnClosePolyline();
		}
		else if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//圆
			l_bIsAdd = TRUE;
			TurnRound();
		}
		
		else if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{//圆弧
			l_bIsAdd = TRUE;
			TurnArc();
		}
		else if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//椭圆
			l_bIsAdd = TRUE;
			TurnEllipse();
		}
	}	
	if (l_bIsAdd == TRUE)
		g_pDoc->m_Undo->AddUndoList(m_pUndoList);
	//原图用浅色灰画一次
	int l_iSizeCollect = g_pDoc->m_CollectOfPicked.GetSize() ;
	for( int j = 0 ; j < l_iSizeCollect; j++ )
	{
		CCreateBase* pSelEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[j] ;
		g_pDoc->m_CCreateBaselayer1.AddTail(pSelEntity);
	}
	//清空选择集
	g_pDoc->m_CollectOfPicked.RemoveAll() ;
	g_pView->Invalidate();
}

/*
 *	把不闭合多段线变成铣刀需要的图形
 */
void CTurnMill::TurnOpenPolyline()
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pMillGraph;
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer;
	//多段线的点集的大小
	int l_iSize = Polyline->m_point.GetSize()-1; 
	//多段线外括后的两个向量的四个点和    交点      多段线上的三个点
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin;
	//多段线的两个向量的与X轴的角度和凸度
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dTudu180 = TUDU_180;	
	//新的点集与凸度
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	//对所有点处理
	for(int i = 0;i<l_iSize;i++)
	{
		l_posPoly1 = Polyline->m_point.GetAt(i);
		l_posPoly2 = Polyline->m_point.GetAt(i+1);
		//如果加到最后一点，退出循环，往回加。
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			if (i ==0)//如果多段线就一条直线
			{
				l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
				l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
				l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
				l_posBegin = l_pos1;				
				l_aryPoint1.Add(l_pos1);
				l_aryTudu1.Add(l_dTudu);
			}	
			l_aryPoint1.Add(l_pos2);
			g_pDoc->m_stcClothCut.iChamferEndStyle == 0?(l_aryTudu1.Add(l_dTudu180)):(l_aryTudu1.Add(l_dTudu));
			break;
		}
		l_posPoly3 = Polyline->m_point.GetAt(i+2);
		//求外括后的四个点
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+MATH_PI/2;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
		if (i ==0)//加第一点
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
		}
		//如果不平行，返回交点
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	//往回加点
	for(int j = l_iSize;j>0;j--)
	{
		l_posPoly1 = Polyline->m_point.GetAt(j);
		l_posPoly2 = Polyline->m_point.GetAt(j-1);
		//如果最后一点，再把起点加上，使闭合
		if (j-1 == 0) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			if (j ==l_iSize)//如果多段线就一条直线
			{
				l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
				l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
				l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
				l_aryPoint1.Add(l_pos1);
				l_aryTudu1.Add(l_dTudu);
			}			
			l_aryPoint1.Add(l_pos2);
			g_pDoc->m_stcClothCut.iChamferEndStyle == 0?(l_aryTudu1.Add(l_dTudu180)):(l_aryTudu1.Add(l_dTudu));
			//加起点，使闭合
			l_aryPoint1.Add(l_posBegin);
			l_aryTudu1.Add(l_dTudu);	
			break;
		}
		l_posPoly3 = Polyline->m_point.GetAt(j-2);
		//求外括后的点
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+MATH_PI/2;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
		if (j == l_iSize)//加第一点
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
		}
		//如果不平行，返回交点
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	POSITION POS = g_pDoc->m_CCreateBaseList.Find(m_pMillGraph);
	CCreatePolyline *pPolyline1  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	pPolyline1->m_ColorOfLine = Polyline->m_ColorOfLine;
	pPolyline1->m_strCBM.Copy(Polyline->m_strCBM);	
	pPolyline1->m_strSBM.Copy(Polyline->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline1);
	g_pDoc->m_CCreateBaseList.RemoveAt(POS);
	AddUndoList((long)pPolyline1);	
}

/*
 *	求两条线的交点
 */
BOOL CTurnMill::GetIntersectPoint(Position pos1,Position pos2,Position pos3,Position pos4,Position &pos)
{
	//两条线的斜率和截距
	double k1,k2,b1,b2;
	double xVal1 = (pos1.x - pos2.x);
	double xVal2 = (pos3.x - pos4.x);
	//如果有一个线与X轴没有交点
	if (xVal1 == 0&&xVal2 != 0)
	{
		//求斜率
		k2 = (pos3.y - pos4.y)/xVal2;
		//求截距
		b2 = pos3.y - k2*pos3.x;
		//求交点
		pos.x = pos1.x;
		pos.y = k2*pos1.x+b2;
		return TRUE;
		
	}
	//如果有一个线与X轴没有交点
	else if (xVal2 == 0&&xVal1 != 0)
	{
		//求斜率
		k1 = (pos1.y - pos2.y)/xVal1;
		//求截距
		b1 = pos1.y - k1*pos1.x;
		//求交点
		pos.x = pos3.x;
		pos.y = k1*pos3.x+b1;
		return TRUE;
	}
	else if (xVal2 == 0&&xVal1 == 0)
		return FALSE;
	//求斜率
	k1 = (pos1.y - pos2.y)/xVal1;
	k2 = (pos3.y - pos4.y)/xVal2;
	//如果斜率一样没有交点
	if (fabs(k1-k2)<DISTANCE_ZERO)
		return FALSE;
	//求截距
	b1 = pos1.y - k1*pos1.x;
	b2 = pos3.y - k2*pos3.x;
	//求交点
	pos.x = (b2-b1)/(k1-k2);
	pos.y = k1*pos.x+b1;
	return TRUE;
}

/*
 *	把闭合的多线段转化 成铣刀需要的图形
 */
void CTurnMill::TurnClosePolyline()
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pMillGraph;
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer;
	//多段线的点集的大小
	int l_iSize = Polyline->m_point.GetSize()-1; 
	//多段线外括后的两个向量的四个点和    交点      多段线上的三个点
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin,l_posEnd;
	//多段线的两个向量的与X轴的角度和凸度
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dAgl90 = MATH_PI/2;	
	//它来决定直线新加线在直线左还是右
	//新的点集与凸度
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	//加外面一圈
	for(int i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = Polyline->m_point.GetAt(i);
		l_posPoly2 = Polyline->m_point.GetAt(i+1);
		//如果加到最后一点，退出循环，并加上最后一点
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//求第一条直线与第二条直线的交点,如果平行
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//让首末点重合
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
				l_aryPoint1.SetAt(0,l_pos);
			}
			else
			{
				l_aryPoint1.Add(l_posBegin);
				l_aryTudu1.Add(l_dTudu);
			}
			break;
		}
		l_posPoly3 = Polyline->m_point.GetAt(i+2);
		//求外括后的四个点
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
		if (i ==0)//加第一点
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//如果不平行，返回交点
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	CCreatePolyline *pPolyline1  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	pPolyline1->m_ColorOfLine = Polyline->m_ColorOfLine;
	pPolyline1->m_strCBM.Copy(Polyline->m_strCBM);	
	pPolyline1->m_strSBM.Copy(Polyline->m_strSBM);	
	POSITION CreatePos = g_pDoc->m_CCreateBaseList.Find(Polyline);
	g_pDoc->m_CCreateBaseList.InsertBefore(CreatePos,pPolyline1);
	l_aryPoint1.RemoveAll();
	l_aryTudu1.RemoveAll();
	l_dAgl90 = -l_dAgl90;
	//加里面一圈
	for(i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = Polyline->m_point.GetAt(i);
		l_posPoly2 = Polyline->m_point.GetAt(i+1);
		//如果加到最后一点，退出循环，并加上最后一点
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//求第一条直线与第二条直线的交点,如果平行
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//让首末点重合
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
				l_aryPoint1.SetAt(0,l_pos);
			}
			else
			{
				l_aryPoint1.Add(l_posBegin);
				l_aryTudu1.Add(l_dTudu);
			}
			break;
		}
		l_posPoly3 = Polyline->m_point.GetAt(i+2);
		//求外括后的四个点
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
		if (i ==0)//加第一点
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//如果不平行，返回交点
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	CCreatePolyline *pPolyline2  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	pPolyline2->m_ColorOfLine = Polyline->m_ColorOfLine;
	pPolyline2->m_strCBM.Copy(Polyline->m_strCBM);	
	pPolyline2->m_strSBM.Copy(Polyline->m_strSBM);	
	POSITION CreatePos2 = g_pDoc->m_CCreateBaseList.Find(Polyline);
	g_pDoc->m_CCreateBaseList.InsertBefore(CreatePos2,pPolyline2);
	g_pDoc->m_CCreateBaseList.RemoveAt(CreatePos2);
	l_aryPoint1.RemoveAll();
	l_aryTudu1.RemoveAll();	
	AddUndoList((long)pPolyline1,(long)pPolyline2);
}

/*
 *	向撤消链表里添加操作闭合的
 */
void CTurnMill::AddUndoList(long pCreatebase1,long pCreatebase2)
{
	if (m_pUndoList == NULL)
	{
		m_pUndoList = m_UndoRedo.NewUndoList();
		m_pUndoList->OperatieWhat = ID_MILL_GROOVES;
		m_pCurGraphPos = m_UndoRedo.NewGraphPos();
		m_pCurGraphPos->pCreateBasepos = (LONG)m_pMillGraph;
		m_pCurGraphPos->pCreateBaseposSpare1 = pCreatebase1;
		m_pCurGraphPos->pCreateBaseposSpare2 = pCreatebase2;
		m_pUndoList->pGraphPos = m_pCurGraphPos;
		
	}
	else
	{
		m_pGraphPos = m_UndoRedo.NewGraphPos();						
		m_pGraphPos->pCreateBasepos = (LONG)m_pMillGraph;
		m_pGraphPos->pCreateBaseposSpare1 = pCreatebase1;
		m_pGraphPos->pCreateBaseposSpare2 = pCreatebase2;
		m_pCurGraphPos->pNext = m_pGraphPos;
		m_pGraphPos->pPrev = m_pCurGraphPos;
		m_pCurGraphPos = m_pGraphPos;
	}
}

/*
 *	把直线变铣刀图形
 */
void CTurnMill::TurnLine()
{
	CCreateLine *pLine = (CCreateLine*)m_pMillGraph;
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer;
	//直线外括后的四个点和                   起点	   末点
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_posPoly1,l_posPoly2;
	//多段线的两个向量的与X轴的角度
	double l_dAngle1,l_dAngle2,l_dTudu = 0.0,l_dTudu180 = TUDU_180;	
	//新的点集与凸度
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	l_posPoly1 = pLine->m_Begin;
	l_posPoly2 = pLine->m_End;
	//求外括后的四个点
	l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
	l_dAngle2 = l_dAngle1-MATH_PI;
	l_pos1.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
	l_pos1.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
	l_pos2.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
	l_pos2.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
	l_pos3.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
	l_pos3.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
	l_pos4.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
	l_pos4.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
	l_aryPoint1.Add(l_pos1);
	l_aryPoint1.Add(l_pos2);
	l_aryPoint1.Add(l_pos3);
	l_aryPoint1.Add(l_pos4);
	l_aryPoint1.Add(l_pos1);	
	l_aryTudu1.Add(l_dTudu);
	g_pDoc->m_stcClothCut.iChamferEndStyle == 0?(l_aryTudu1.Add(l_dTudu180)):(l_aryTudu1.Add(l_dTudu));
	l_aryTudu1.Add(l_dTudu);
	g_pDoc->m_stcClothCut.iChamferEndStyle == 0?(l_aryTudu1.Add(l_dTudu180)):(l_aryTudu1.Add(l_dTudu));
	l_aryTudu1.Add(l_dTudu);
	POSITION POS = g_pDoc->m_CCreateBaseList.Find(m_pMillGraph);
	CCreatePolyline *pPolyline1  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	pPolyline1->m_ColorOfLine = pLine->m_ColorOfLine;
	pPolyline1->m_strCBM.Copy(pLine->m_strCBM);	
	pPolyline1->m_strSBM.Copy(pLine->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline1);
	g_pDoc->m_CCreateBaseList.RemoveAt(POS);
	AddUndoList((long)pPolyline1);	
}

/*
 *	把圆弧变铣刀线
 */
void CTurnMill::TurnArc()
{
	CCreateRoundArc *pArc = (CCreateRoundArc*)m_pMillGraph;
	//铣刀宽度
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer,l_dTudu180 = -TUDU_180;
	//直线外括后的四个点和                   起点	   末点
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_posPoly1,l_posPoly2;
	l_posPoly1 = pArc->m_Begin;
	l_posPoly2 = pArc->m_End;
	//新的点集与凸度
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	//圆心与点的角度
	double l_dAglBG = GetAngleToXAxis(pArc->m_Center,pArc->m_Begin);
	//圆心与点的角度
	double l_dAglEN = GetAngleToXAxis(pArc->m_Center,pArc->m_End);
	l_pos1.x = l_posPoly1.x - l_dLongChamfer*RetainValid(cos(l_dAglBG),8);
	l_pos1.y = l_posPoly1.y - l_dLongChamfer*RetainValid(sin(l_dAglBG),8);
	l_pos2.x = l_posPoly2.x - l_dLongChamfer*RetainValid(cos(l_dAglEN),8);
	l_pos2.y = l_posPoly2.y - l_dLongChamfer*RetainValid(sin(l_dAglEN),8);
	l_pos3.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAglEN),8);
	l_pos3.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAglEN),8);
	l_pos4.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAglBG),8);
	l_pos4.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAglBG),8);
	//凸度
	double l_dTudu = g_pDoc->RtnTudu(l_posPoly1,l_posPoly2,pArc->m_Center,pArc->m_iDirect),l_dzore = 0.0;
	double l_dfuTudu = -l_dTudu;
	l_aryPoint1.Add(l_pos1);	
	l_aryTudu1.Add(l_dTudu);	
	l_aryPoint1.Add(l_pos2);	
	g_pDoc->m_stcClothCut.iChamferEndStyle == 0?(l_aryTudu1.Add(l_dTudu180)):(l_aryTudu1.Add(l_dzore));
	l_aryPoint1.Add(l_pos3);	
	l_aryTudu1.Add(l_dfuTudu);	
	l_aryPoint1.Add(l_pos4);	
	g_pDoc->m_stcClothCut.iChamferEndStyle == 0?(l_aryTudu1.Add(l_dTudu180)):(l_aryTudu1.Add(l_dzore));
	l_aryPoint1.Add(l_pos1);	
	l_aryTudu1.Add(l_dzore);	
	POSITION POS = g_pDoc->m_CCreateBaseList.Find(m_pMillGraph);
	CCreatePolyline *pPolyline1  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	pPolyline1->m_ColorOfLine = pArc->m_ColorOfLine;
	pPolyline1->m_strCBM.Copy(pArc->m_strCBM);	
	pPolyline1->m_strSBM.Copy(pArc->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline1);
	g_pDoc->m_CCreateBaseList.RemoveAt(POS);	
	AddUndoList((long)pPolyline1);	
}

/*
 *	把圆变铣刀线
 */
void CTurnMill::TurnRound()
{
	CCreateRound *pRound = (CCreateRound*)m_pMillGraph;
	//铣刀宽度
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer;
	//圆心与末点的角度
	double l_dAgl = GetAngleToXAxis(pRound->m_Center,pRound->m_EndPos);
	//外圆的末点
	Position posOut(pRound->m_Center.x + (l_dLongChamfer+pRound->m_dRadius)*cos(l_dAgl),
					pRound->m_Center.y + (l_dLongChamfer+pRound->m_dRadius)*sin(l_dAgl));
	//内圆的末点
	Position posIn(pRound->m_Center.x + (-l_dLongChamfer+pRound->m_dRadius)*cos(l_dAgl),
		pRound->m_Center.y + (-l_dLongChamfer+pRound->m_dRadius)*sin(l_dAgl));
	
	//变铣刀后的圆
	CCreateRound *pRound1 = new CCreateRound(pRound->m_Center,posOut,pRound->m_iDirect);
	CCreateRound *pRound2 = new CCreateRound(pRound->m_Center,posIn,pRound->m_iDirect);
	//插入到链表
	POSITION POS = g_pDoc->m_CCreateBaseList.Find(pRound);
	g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound1);
	g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound2);	
	g_pDoc->m_CCreateBaseList.RemoveAt(POS);
	AddUndoList((long)pRound1,(long)pRound2);	
}

/*
 *	把椭圆变铣刀线
 */
void CTurnMill::TurnEllipse()
{
	CCreateEllipse *pEllipse = (CCreateEllipse*)m_pMillGraph;
	//铣刀宽度
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer;
	//多段线的点集的大小
	int l_iSize = pEllipse->m_point.GetSize()-1; 
	//多段线外括后的两个向量的四个点和    交点      多段线上的三个点
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin,l_posEnd;
	//多段线的两个向量的与X轴的角度和凸度
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dAgl90 = MATH_PI/2;	
	//它来决定直线新加线在直线左还是右
	//新的点集与凸度
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	//加外面一圈
	for(int i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = pEllipse->m_point.GetAt(i);
		l_posPoly2 = pEllipse->m_point.GetAt(i+1);
		//如果加到最后一点，退出循环，并加上最后一点
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//求第一条直线与第二条直线的交点,如果平行
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//让首末点重合
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
				l_aryPoint1.SetAt(0,l_pos);
			}
			else
			{
				l_aryPoint1.Add(l_posBegin);
				l_aryTudu1.Add(l_dTudu);
			}
			break;
		}
		l_posPoly3 = pEllipse->m_point.GetAt(i+2);
		//求外括后的四个点
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
		if (i ==0)//加第一点
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//如果不平行，返回交点
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	CCreatePolyline *pPolyline1  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	pPolyline1->m_ColorOfLine = pEllipse->m_ColorOfLine;
	pPolyline1->m_strCBM.Copy(pEllipse->m_strCBM);	
	pPolyline1->m_strSBM.Copy(pEllipse->m_strSBM);	
	POSITION CreatePos = g_pDoc->m_CCreateBaseList.Find(pEllipse);
	g_pDoc->m_CCreateBaseList.InsertBefore(CreatePos,pPolyline1);
	l_aryPoint1.RemoveAll();
	l_aryTudu1.RemoveAll();
	l_dAgl90 = -l_dAgl90;
	//加里面一圈
	for(i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = pEllipse->m_point.GetAt(i);
		l_posPoly2 = pEllipse->m_point.GetAt(i+1);
		//如果加到最后一点，退出循环，并加上最后一点
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//求第一条直线与第二条直线的交点,如果平行
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//让首末点重合
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
				l_aryPoint1.SetAt(0,l_pos);
			}
			else
			{
				l_aryPoint1.Add(l_posBegin);
				l_aryTudu1.Add(l_dTudu);
			}
			break;
		}
		l_posPoly3 = pEllipse->m_point.GetAt(i+2);
		//求外括后的四个点
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
		if (i ==0)//加第一点
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//如果不平行，返回交点
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	CCreatePolyline *pPolyline2  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	pPolyline2->m_ColorOfLine = pEllipse->m_ColorOfLine;
	pPolyline2->m_strCBM.Copy(pEllipse->m_strCBM);	
	pPolyline2->m_strSBM.Copy(pEllipse->m_strSBM);	
	POSITION CreatePos2 = g_pDoc->m_CCreateBaseList.Find(pEllipse);
	g_pDoc->m_CCreateBaseList.InsertBefore(CreatePos2,pPolyline2);
	l_aryPoint1.RemoveAll();
	l_aryTudu1.RemoveAll();	
	g_pDoc->m_CCreateBaseList.RemoveAt(CreatePos2);
	AddUndoList((long)pPolyline1,(long)pPolyline2);
}


