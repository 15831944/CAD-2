// OverCut.cpp: implementation of the COverCut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "OverCut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COverCut::COverCut()
{

}

COverCut::~COverCut()
{

}

/*
 *	看是否是直线，然后过切
 */
void COverCut::NeedOverCut()
{
	g_pDoc->RemoveAllCollected();
	POSITION pos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	POSITION l_POSOverCut;
	while (pos != NULL)
	{	//从所有图元里找多段线
		l_POSOverCut = pos;
		CCreateBase* l_pIsPolyline = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(pos);
		if(l_pIsPolyline->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
		{	//找到后执行过切函数
			CCreatePolyline *l_pPolyline = (CCreatePolyline*)l_pIsPolyline;
			//只对所选的刀类型进行切割
			if(
	(g_pDoc->m_stcClothCut.m_bOverCutKnife1==1 && l_pIsPolyline->m_ColorOfLine==g_pDoc->m_stcColorToM.m_color1)||
	(g_pDoc->m_stcClothCut.m_bOverCutKnife2==1 && l_pIsPolyline->m_ColorOfLine==g_pDoc->m_stcColorToM.m_color2)||
	(g_pDoc->m_stcClothCut.m_bOverCutKnife3==1 && l_pIsPolyline->m_ColorOfLine==g_pDoc->m_stcColorToM.m_color3)||
	(g_pDoc->m_stcClothCut.m_bOverCutKnife4==1 && l_pIsPolyline->m_ColorOfLine==g_pDoc->m_stcColorToM.m_color4)||
	(g_pDoc->m_stcClothCut.m_bOverCutKnife5==1 && l_pIsPolyline->m_ColorOfLine==g_pDoc->m_stcColorToM.m_color5)
				)
			{
				//添加撤消操作
				m_pUndoList = m_UndoRedo.NewUndoList();
				m_pUndoList->OperatieWhat = ID_OVER_CUT;	
				m_pheadGraphPos = m_pCurGraphPos = m_UndoRedo.NewGraphPos();
				m_pUndoList->pGraphPos = m_pheadGraphPos;
				m_pUndoList->pOperatieParam = m_UndoRedo.NewOperateParam();
				//执行过切
				ExecuteOverCut(l_pPolyline,l_POSOverCut);	
			}
		}
	}
	g_pView->Invalidate();
}

/*
 *	执行过切函数
 */
void COverCut::ExecuteOverCut(CCreatePolyline* l_pIsPolyline,POSITION l_POSOverCut)
{
	if(l_pIsPolyline->m_point.GetSize() < 3)
	{	//如果没有过切的图形，把开始申请的内存释放掉
		if (m_pheadGraphPos->pCreateBasepos == NULL)
		{
			delete m_pUndoList->pGraphPos;
			m_pUndoList->pGraphPos = NULL;
			delete m_pUndoList->pOperatieParam;
			m_pUndoList->pOperatieParam = NULL;
			delete m_pUndoList;
			m_pUndoList = NULL;
		}
		return;
	}
	CByteArray l_carryNumPoint;
	//一个角度由两条直线组成上的三个点
	Position l_posBegin,l_posEnd,l_posMiddle;
	//凸度								两条线与X轴的角度
	double l_dTude1=0.0,l_dTude2=0.0,l_dAngle1,l_dAngle2;
	//点集的大小
	int l_iSize = l_pIsPolyline->m_point.GetSize();
	//看起点是不是过切角
	l_posBegin = l_pIsPolyline->m_point.GetAt(l_iSize-1);
	l_posMiddle = l_pIsPolyline->m_point.GetAt(0);
	//这两个等于零说明是直线
	l_dTude1 = l_pIsPolyline->m_dTuDu.GetAt(l_iSize-2);
	l_dTude2 = l_pIsPolyline->m_dTuDu.GetAt(0);
	//如果是闭合的图形且是直线	
	if(PosEqual(l_posBegin,l_posMiddle)&&l_dTude1 == 0.0&&l_dTude2 == 0.0)
	{
		l_posBegin = l_pIsPolyline->m_point.GetAt(l_iSize-2);
		l_posEnd = l_pIsPolyline->m_point.GetAt(1);
		//求两条线与X轴的角度
		l_dAngle1 = GetAngle(l_posMiddle,l_posBegin);
		l_dAngle2 = GetAngle(l_posMiddle,l_posEnd);
		l_dAngle1 = fabs(l_dAngle1-l_dAngle2);//两条线的夹角
		if(l_dAngle1 > 180) l_dAngle1 = 360 - l_dAngle1;
		//看点集里每两个直线之间角度是否满足条件
		if(l_dAngle1<g_pDoc->m_stcClothCut.m_dOverCutAngle)
			l_carryNumPoint.Add(TRUE);
		else
			l_carryNumPoint.Add(FALSE);	
	}
	else
		l_carryNumPoint.Add(FALSE);//记住这点是要过切的
	//看点集里每两个直线之间角度是否满足条件
	for(int i=0;i<l_iSize-2;i++)
	{
		//提取一个角度中的三个点和两个凸度
		l_posBegin = l_pIsPolyline->m_point.GetAt(i);
		l_posMiddle = l_pIsPolyline->m_point.GetAt(i+1);
		l_posEnd = l_pIsPolyline->m_point.GetAt(i+2);
		l_dTude1 = l_pIsPolyline->m_dTuDu.GetAt(i);
		l_dTude2 = l_pIsPolyline->m_dTuDu.GetAt(i+1);
		//三个点之间的两条线是否是直线
		if(l_dTude1==0.0&&l_dTude2==0.0)
		{
			//求两条线与X轴的角度
			l_dAngle1 = GetAngle(l_posBegin,l_posMiddle);
			l_dAngle2 = GetAngle(l_posEnd,l_posMiddle);
			l_dAngle1 = fabs(l_dAngle1-l_dAngle2);//两条线的夹角
			if(l_dAngle1 > 180) l_dAngle1 = 360 - l_dAngle1;
			//看点集里每两个直线之间角度是否满足条件
			if(l_dAngle1<g_pDoc->m_stcClothCut.m_dOverCutAngle)
				l_carryNumPoint.Add(TRUE);//记住这点是要过切的
			else
				l_carryNumPoint.Add(FALSE);//否则不用过切
		}
		else
			l_carryNumPoint.Add(FALSE);//否则不用过切
	}
	//把多段线根据过切的点分解为多个
	PutPloyBreak(l_pIsPolyline,&l_carryNumPoint,l_POSOverCut);
}

/*
 *	把多段线根据过切的点分解为多个
 */
void COverCut::PutPloyBreak(CCreatePolyline* l_pIsPolyline,CByteArray* l_carryNumPoint,POSITION l_POSOverCut)
{
	CArray<Position,Position&> l_carrayPoint;//点集
	CArray<double,double&> l_carrayTuDu;//存放凸度的
	//直线上的两个点
	Position l_posBegin,l_posEnd,l_posOutBegin,l_posOutEnd,l_posNext;
	//凸度					
	double l_dTude=0.0;
	//点集的大小									看是不是过切点
	int l_iSize = l_carryNumPoint->GetSize(),l_iBool;
	for(int i=0;i<l_iSize;i++)
	{
		l_iBool = l_carryNumPoint->GetAt(i);
		//获得两个点和凸度
		l_posBegin = l_pIsPolyline->m_point.GetAt(i);
		l_posEnd = l_pIsPolyline->m_point.GetAt(i+1);
		l_dTude = l_pIsPolyline->m_dTuDu.GetAt(i);
		if(l_iBool == TRUE)//如果这点需要过切
		{	
			ProcLine(l_posNext,l_posBegin,0,g_pDoc->m_stcClothCut.m_dOverCutLong,l_posOutBegin,l_posOutEnd);//出末点
			l_carrayPoint.Add(l_posOutEnd);
			l_carrayTuDu.Add(l_dTude);	
			//存进链表
			if(i!=0)
			{
				CCreatePolyline* pTemp = new CCreatePolyline(l_carrayPoint,l_carrayTuDu);
				pTemp->m_ColorOfLine = l_pIsPolyline->m_ColorOfLine;
				pTemp->m_strCBM.Copy(l_pIsPolyline->m_strCBM);	
				pTemp->m_strSBM.Copy(l_pIsPolyline->m_strSBM);	
				g_pDoc->m_CCreateBaseList.InsertBefore(l_POSOverCut,pTemp);
				//向撤消链表里添加元素
				if (m_pheadGraphPos->pCreateBasepos == NULL)	
				{
					m_pCurGraphPos->pCreateBasepos = (long)pTemp;
					POSITION pPOSITION  = g_pDoc->m_CCreateBaseList.Find(pTemp);
					m_pUndoList->pOperatieParam->pPosition = (LONG)pPOSITION;
					
				}
				else
				{
					m_pGraphPos = m_UndoRedo.NewGraphPos();						
					m_pGraphPos->pCreateBasepos = (long)pTemp;
					m_pCurGraphPos->pNext = m_pGraphPos;
					m_pGraphPos->pPrev = m_pCurGraphPos;
					m_pCurGraphPos = m_pGraphPos;
				}
			}
			l_carrayPoint.RemoveAll();
			l_carrayTuDu.RemoveAll();
			//加末点
			ProcLine(l_posBegin,l_posEnd,1,g_pDoc->m_stcClothCut.m_dOverCutLong,l_posOutBegin,l_posOutEnd);//出起点
			l_carrayPoint.Add(l_posOutBegin);
			l_carrayTuDu.Add(l_dTude);
			l_posNext = l_posBegin;
		}
		else //不需要
		{
			l_posNext = l_posBegin;			
			//加点
			l_carrayPoint.Add(l_posBegin);
			l_carrayTuDu.Add(l_dTude);
		}
	}

	//把末点加进去	
	l_posBegin = l_pIsPolyline->m_point.GetAt(l_iSize-1);
	l_posEnd = l_pIsPolyline->m_point.GetAt(l_iSize);
	l_dTude = l_pIsPolyline->m_dTuDu.GetAt(i);
	l_carrayTuDu.Add(l_dTude);	
	if(l_carryNumPoint->GetAt(0) == TRUE)//首尾相接的并且要过切的
	{
		ProcLine(l_posBegin,l_posEnd,0,g_pDoc->m_stcClothCut.m_dOverCutLong,l_posOutBegin,l_posOutEnd);//出末点
		l_carrayPoint.Add(l_posOutEnd);
	}
	else
		l_carrayPoint.Add(l_posEnd);


	//向撤消链表里添加元素,如果这个多段线不用过切
	if (m_pheadGraphPos->pCreateBasepos != NULL)
	{	//存进图形链表
		CCreatePolyline* pTemp = new CCreatePolyline(l_carrayPoint,l_carrayTuDu);
		pTemp->m_ColorOfLine = l_pIsPolyline->m_ColorOfLine;
		pTemp->m_strCBM.Copy(l_pIsPolyline->m_strCBM);	
		pTemp->m_strSBM.Copy(l_pIsPolyline->m_strSBM);	
		g_pDoc->m_CCreateBaseList.InsertBefore(l_POSOverCut,pTemp);
		//向撤消链表里加个图形
		m_pGraphPos = m_UndoRedo.NewGraphPos();						
		m_pGraphPos->pCreateBasepos = (long)pTemp;
		m_pCurGraphPos->pNext = m_pGraphPos;
		m_pGraphPos->pPrev = m_pCurGraphPos;
		m_pCurGraphPos = m_pGraphPos;
		m_pheadGraphPos->pCreateBaseposSpare1 = (LONG)l_pIsPolyline;
	}
	//如果没有过切的图形，把开始申请的内存释放掉
	if (m_pheadGraphPos->pCreateBasepos == NULL)
	{
		delete m_pUndoList->pGraphPos;
		m_pUndoList->pGraphPos = NULL;
		delete m_pUndoList->pOperatieParam;
		m_pUndoList->pOperatieParam = NULL;
		delete m_pUndoList;
		m_pUndoList = NULL;
	}
	else//如果有就把这个操作添加到里撤消里面
	{
		g_pDoc->m_Undo->AddUndoList(m_pUndoList);
		g_pDoc->m_CCreateBaseList.RemoveAt(l_POSOverCut);
	}
}

//功能：给直线加延长线
//参数：CMyLine lnIn:原直线
//	int nType: 0 表示正向延长 1表示反向延长
//	double distance: 延长距离
//	CMyLine& lnOut:延长后直线
void COverCut::ProcLine(Position ptStart1,Position ptEnd1, int nType, double distance,
						  Position& ptStart2,Position& ptEnd2)
{ 
	if ( fabs(distance-0.0)<DOUBLE_EQUAL )	//延长距离为0
	{
		ptStart2.x = ptStart1.x;
		ptStart2.y = ptStart1.y;
		ptEnd2.x = ptEnd1.x;
		ptEnd2.y = ptEnd1.y;
		return;
	}

	//计算斜率
	if ( fabs(ptEnd1.x-ptStart1.x)<DOUBLE_EQUAL )		//斜率不存在的情况
	{
		if (0 == nType)			//正向延长
		{
			ptStart2.x = ptStart1.x;
			ptStart2.y = ptStart1.y;
			ptEnd2.x = ptEnd1.x;

			if (ptEnd1.y > ptStart1.y)
				ptEnd2.y = ptEnd1.y + distance;
			else
				ptEnd2.y = ptEnd1.y - distance;
		}
		else					//反向延长
		{
			ptStart2.x = ptStart1.x;
			ptEnd2.x = ptEnd1.x;
			ptEnd2.y = ptEnd1.y;
			
			if (ptEnd1.y > ptStart1.y)
				ptStart2.y = ptStart1.y - distance;
			else
				ptStart2.y = ptStart1.y + distance;
		}
	}
	else														//斜率存在的情况
	{
		//斜率存在，则斜率为
		double K = (ptEnd1.y-ptStart1.y) / (ptEnd1.x-ptStart1.x);
		
		if ( fabs(K-0.0)<DOUBLE_EQUAL )		//斜率为0
		{
			if (0 == nType)			//正向延长
			{
				ptStart2.x = ptStart1.x;
				ptStart2.y = ptStart1.y;
				ptEnd2.y = ptEnd1.y;
				if (ptEnd1.x > ptStart1.x)
					ptEnd2.x = ptEnd1.x + distance;
				else
					ptEnd2.x = ptEnd1.x - distance;
			}
			else					//反向延长
			{
				ptEnd2.x = ptEnd1.x;
				ptEnd2.y = ptEnd1.y;
				ptStart2.y = ptStart1.y;
				if (ptEnd1.x > ptStart1.x)
					ptStart2.x = ptStart1.x - distance;
				else
					ptStart2.x = ptStart1.x + distance;
				
			}
		}
		else if (K > 0)					//斜率>0
		{
			double angle = atan(K);		//与X的夹角
			if (0 == nType)
			{
				ptStart2.x = ptStart1.x;
				ptStart2.y = ptStart1.y;

				if (ptEnd1.x > ptStart1.x)
				{
					ptEnd2.x = ptEnd1.x + cos(angle) * distance;
					ptEnd2.y = ptEnd1.y + sin(angle) * distance;
				}
				else
				{
					ptEnd2.x = ptEnd1.x - cos(angle) * distance;
					ptEnd2.y = ptEnd1.y - sin(angle) * distance;
				}
			}
			else
			{
				ptEnd2.x = ptEnd1.x;
				ptEnd2.y = ptEnd1.y;

				if (ptEnd1.x > ptStart1.x)
				{
					ptStart2.x = ptStart1.x - cos(angle) * distance;
					ptStart2.y = ptStart1.y - sin(angle) * distance;
				}
				else
				{
					ptStart2.x = ptStart1.x + cos(angle) * distance;
					ptStart2.y = ptStart1.y + sin(angle) * distance;
				}
			}
		}
		else							//斜率小于0
		{
			double angle = atan(fabs(K));	//与X轴夹角
			if (0 == nType)
			{
				ptStart2.x = ptStart1.x;
				ptStart2.y = ptStart1.y;

				if (ptEnd1.x > ptStart1.x)
				{
					ptEnd2.x = ptEnd1.x + cos(angle) * distance;
					ptEnd2.y = ptEnd1.y - sin(angle) * distance;
				}
				else
				{
					ptEnd2.x = ptEnd1.x - cos(angle) * distance;
					ptEnd2.y = ptEnd1.y + sin(angle) * distance;
				}
			}
			else
			{
				ptEnd2.x = ptEnd1.x;
				ptEnd2.y = ptEnd1.y;

				if (ptEnd1.x > ptStart1.x)
				{
					ptStart2.x = ptStart1.x - cos(angle) * distance;
					ptStart2.y = ptStart1.y + sin(angle) * distance;
				}
				else
				{
					ptStart2.x = ptStart1.x + cos(angle) * distance;
					ptStart2.y = ptStart1.y - sin(angle) * distance;
				}
			}
		}
		
	}
}