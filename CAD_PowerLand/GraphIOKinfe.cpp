// GraphIOKinfe.cpp: implementation of the CGraphIOKinfe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "GraphIOKinfe.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphIOKinfe::CGraphIOKinfe()
{
	m_pUndoList = NULL;
}

CGraphIOKinfe::~CGraphIOKinfe()
{

}

/*
 *	给图形加进退刀
 */
void CGraphIOKinfe::PutGraphIOKife()
{
	POSITION l_POSList;
	//是否有要处理的图形的标志
	BOOL l_bIsAdd = FALSE;	
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	for( int i = 0 ; i < l_iSize; i++ )
	{
		CCreateBase*	l_pCreateBase = (CCreateBase*)g_pDoc->m_CollectOfPicked[i] ;		
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
		{//多线段
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);
			ChangePolyBE(Polyline,l_POSList,TRUE);
			l_bIsAdd = TRUE;
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{//矩形
			CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			ChangePolyBE(Rectangle,l_POSList,FALSE);
			l_bIsAdd = TRUE;
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//圆
			CCreateRound *Round = (CCreateRound*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);
			ChangeRoundBE(Round,l_POSList);
			l_bIsAdd = TRUE;
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//椭圆
			CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);
			ChangeEllipseBE(Ellipse,l_POSList);
			l_bIsAdd = TRUE;
		}
	}	
	if (l_bIsAdd == TRUE)
		g_pDoc->m_Undo->AddUndoList(m_pUndoList);
	g_pDoc->m_CollectOfPicked.RemoveAll();
	g_pView->Invalidate();
}

//改变多段线起点和终点
void CGraphIOKinfe::ChangePolyBE(CCreateBase *line,POSITION POSCur,BOOL IsPolyline)
{
	CCreatePolyline *Polyline = (CCreatePolyline*)line;
	CArray<Position,Position&> l_aryPoint;//点集
	CArray<double,double&> l_aryTuDu;//存放凸度的集
	//		中间用的起点，终点，圆弧起点点，直线起点//圆弧的圆心; 这两个参数没用
	Position l_posBegin,l_posEnd,l_posArc,l_posLine,l_posArcCenter,l_posArc1,l_posLine1;;
	int l_iClock = 0;//圆弧方向
	//判断左右刀补，给每个图形用
	int l_iKnifeStyle = 41;
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iKnifeStyle = 41;
	else
		l_iKnifeStyle = 42;
	//凸度
	double l_dTudu,l_dUseTudu;
	l_posBegin = Polyline->m_point.GetAt(0);
	l_posEnd = Polyline->m_point.GetAt(1);
	//求进刀圆弧的起点和凸度
	if(Polyline->m_dTuDu.GetAt(0) == 0.0)//如果是直线的话
		CalcArcBgPos(l_posBegin,l_posEnd,l_posArc,l_posLine,l_dTudu);
	else//否则是圆弧
	{
		l_dUseTudu = Polyline->m_dTuDu.GetAt(0);
		if(l_dUseTudu >0) l_iClock = AD_COUNTERCLOCKWISE;
		else l_iClock = AD_CLOCKWISE;
		g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dUseTudu,l_posArcCenter);
		//求圆弧的点凸度还有直线的点，后面的三个参数 没有用
		CalcArcCenterForRound(l_iClock,l_posArcCenter,l_posBegin,l_posArc,l_posLine,l_dTudu,l_posArc1,l_posLine1,l_dUseTudu);	
	}
	//如果是不在中心
	if(g_pDoc->m_stcIOKnife.IsCenter == FALSE)
	{
		double tudu = 0.0;
		//加进刀
		if(!PosEqual(l_posLine,l_posArc))//看直线是否为零
		{
			l_aryPoint.Add(l_posLine);//加直线
			l_aryTuDu.Add(tudu);
		}
		l_aryPoint.Add(l_posArc);//加圆弧
		l_aryTuDu.Add(l_dTudu);
		int l_iSizeOfPoint = Polyline->m_point.GetSize()-2;//到数第二点
		//把原图加进点集和凸度集
		for(int i = 0;i<l_iSizeOfPoint+1;i++)
		{
			l_aryPoint.Add(Polyline->m_point.GetAt(i));
			tudu = Polyline->m_dTuDu.GetAt(i);
			l_aryTuDu.Add(tudu);
		}
		tudu = 0.0;
		//加退刀
		l_posBegin = Polyline->m_point.GetAt(l_iSizeOfPoint);//到数第二点
		l_posEnd = Polyline->m_point.GetAt(l_iSizeOfPoint+1);//到数第一点
		if(Polyline->m_dTuDu.GetAt(l_iSizeOfPoint) == 0.0)//如果是直线的话
			CalcArcEnPos(l_posBegin,l_posEnd,l_posArc,l_posLine,l_dTudu);
		else
		{
			l_dUseTudu = Polyline->m_dTuDu.GetAt(l_iSizeOfPoint);
			if(l_dUseTudu >0) l_iClock = AD_COUNTERCLOCKWISE;
			else l_iClock = AD_CLOCKWISE;
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dUseTudu,l_posArcCenter);
			//求圆弧的点凸度还有直线的点，后面的三个参数 没有用
			CalcArcCenterForRound(l_iClock,l_posArcCenter,l_posEnd,l_posArc1,l_posLine1,l_dUseTudu,l_posArc,l_posLine,l_dTudu);			
		}
		l_aryPoint.Add(l_posEnd);//加圆弧
		l_aryTuDu.Add(l_dTudu);
		l_aryPoint.Add(l_posArc);//加直线
		l_aryTuDu.Add(tudu);
		if(!PosEqual(l_posLine,l_posArc))//看直线是否为零
		{
			l_aryPoint.Add(l_posLine);//加直线
			l_aryTuDu.Add(tudu);
		}
		//更新前先存到点集里
		AddUndoList(Polyline->m_point,Polyline->m_dTuDu,(LONG)Polyline);	
		//把点集更新一下
		Polyline->m_point.Copy(l_aryPoint);
		Polyline->m_dTuDu.Copy(l_aryTuDu);
	}
	else//在中心
	{
	}	
	l_aryPoint.RemoveAll();
	l_aryTuDu.RemoveAll();
}

//改变圆起点和终点
void CGraphIOKinfe::ChangeRoundBE(CCreateRound *Round,POSITION POSCur)
{
	CArray<Position,Position&> l_aryPoint,l_aryPoint1;//点集
	CArray<double,double&> l_aryTuDu,l_aryTuDu1;//存放凸度的集
	//		中间用的起点，终点，圆弧起点点，直线起点
	Position l_posCenter,l_posEnd,l_posArc1,l_posLine1,l_posArc2,l_posLine2;
	//凸度
	double l_dTudu1,l_dTudu2;
	//判断左右刀补，给每个图形用
	int l_iKnifeStyle = 41;
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iKnifeStyle = 41;
	else
		l_iKnifeStyle = 42;
	l_posCenter = Round->m_Center;
	l_posEnd = Round->m_EndPos;
	int l_iClock = Round->m_iDirect;
	//求进刀圆弧的起点和凸度
	CalcArcCenterForRound(l_iClock,l_posCenter,l_posEnd,l_posArc1,l_posLine1,l_dTudu1,l_posArc2,l_posLine2,l_dTudu2);
	double tudu = 0.0;
	//加进刀
	if(!PosEqual(l_posLine1,l_posArc1))
	{
		l_aryPoint.Add(l_posLine1);//加直线
		l_aryTuDu.Add(tudu);
	}
	l_aryPoint.Add(l_posArc1);//加圆弧
	l_aryTuDu.Add(l_dTudu1);
	//把圆分两个半圆加
	Position l_posBegin(2*l_posCenter.x-l_posEnd.x,2*l_posCenter.y-l_posEnd.y);
	tudu = g_pDoc->RtnTudu(l_posEnd,l_posBegin,l_posCenter,Round->m_iDirect);
	l_aryPoint.Add(l_posEnd);
	l_aryTuDu.Add(tudu);
	l_aryPoint1.Add(l_posEnd);
	l_aryTuDu1.Add(tudu);
	tudu = g_pDoc->RtnTudu(l_posBegin,l_posEnd,l_posCenter,Round->m_iDirect);
	l_aryPoint.Add(l_posBegin);
	l_aryTuDu.Add(tudu);
	l_aryPoint1.Add(l_posBegin);
	l_aryTuDu1.Add(tudu);
	tudu = 0.0;
	l_aryPoint1.Add(l_posEnd);
	l_aryTuDu1.Add(tudu);
	//加退刀
	l_aryPoint.Add(l_posEnd);//加圆弧
	l_aryTuDu.Add(l_dTudu2);
	l_aryPoint.Add(l_posArc2);//加直线
	l_aryTuDu.Add(tudu);
	if(!PosEqual(l_posLine2,l_posArc2))
	{
		l_aryPoint.Add(l_posLine2);//加直线
		l_aryTuDu.Add(tudu);
	}
	CCreatePolyline *pTempPolyline2 = new CCreatePolyline(l_aryPoint,l_aryTuDu);
	pTempPolyline2->m_iKnifeStyle = l_iKnifeStyle;	
	pTempPolyline2->m_iIsBeginGraph = Round->m_iIsBeginGraph;
	pTempPolyline2->m_ColorOfLine = Round->m_ColorOfLine;
	pTempPolyline2->m_strCBM.Copy(Round->m_strCBM);	
	pTempPolyline2->m_strSBM.Copy(Round->m_strSBM);	
	g_pDoc->m_CCreateBaseList.SetAt(POSCur,pTempPolyline2);
	//更新前先存到点集里	
	AddUndoList(l_aryPoint1,l_aryTuDu1,(LONG)pTempPolyline2);	
	//释放内存
	l_aryPoint1.RemoveAll();
	l_aryTuDu1.RemoveAll();
	l_aryPoint.RemoveAll();
	l_aryTuDu.RemoveAll();
	//把原撤消链表里存的圆换成多段线
	PUNDOLIST pList = g_pDoc->m_Undo->m_pHeadList;
	while (pList)
	{
		if (pList->pGraphPos&&pList->pGraphPos->pCreateBasepos)
		{
			if (pList->pGraphPos->pCreateBasepos == (LONG)Round) 
				pList->pGraphPos->pCreateBasepos = (LONG)pTempPolyline2;
		}
		pList = pList->pNext;
	}
	delete Round;
	Round = NULL;
	
}

//求圆的进出刀圆心
void CGraphIOKinfe::CalcArcCenterForRound(int l_iClock,Position Center,Position End,Position& posArc1,Position& posLine1,
						double& tudu1,Position& posArc2,Position& posLine2,double& tudu2)
{
	//1，弧心与圆的切点的角度
	double l_dAngleBgEn = GetAngleToXAxis(End,Center);//顺时针左,逆时针右
	if ((g_pDoc->m_stcGlassGrind.DaoBuStyle&&l_iClock == AD_COUNTERCLOCKWISE)||((!g_pDoc->m_stcGlassGrind.DaoBuStyle&&l_iClock == AD_CLOCKWISE)))		//否则
		l_dAngleBgEn = MATH_PI + l_dAngleBgEn;
	//2，求弧心
	Position l_posCenter1,l_posCenter2;
	l_posCenter1.x = End.x - cos(l_dAngleBgEn)*g_pDoc->m_stcIOKnife.IArcRadius;
	l_posCenter1.y = End.y - sin(l_dAngleBgEn)*g_pDoc->m_stcIOKnife.IArcRadius;
	l_posCenter2.x = End.x - cos(l_dAngleBgEn)*g_pDoc->m_stcIOKnife.OArcRadius;
	l_posCenter2.y = End.y - sin(l_dAngleBgEn)*g_pDoc->m_stcIOKnife.OArcRadius;
	//3，计算进刀的起始角度和退刀的结束角度
	double l_dAngle1,l_dAngle2;	//进退刀的结束角度
	int l_iArcClock1,l_iArcClock2;//进退刀圆弧的方向
	if (g_pDoc->m_stcGlassGrind.DaoBuStyle)		//左
	{
		l_dAngle1 = l_dAngleBgEn - g_pDoc->m_stcIOKnife.IArcAngle/180*MATH_PI;
		l_dAngle2 = l_dAngleBgEn + g_pDoc->m_stcIOKnife.OArcAngle/180*MATH_PI;
		l_iArcClock1 = l_iArcClock2 = AD_COUNTERCLOCKWISE;
	}
	else	//右
	{
		l_dAngle1 = l_dAngleBgEn + g_pDoc->m_stcIOKnife.IArcAngle/180*MATH_PI;
		l_dAngle2 = l_dAngleBgEn - g_pDoc->m_stcIOKnife.OArcAngle/180*MATH_PI;
		l_iArcClock1 = l_iArcClock2 =  AD_CLOCKWISE;
	}

	//4，求进刀的起始点，退刀的末点
	CalcBeginPoint(l_posCenter1,g_pDoc->m_stcIOKnife.IArcRadius,l_dAngle1,posArc1);
	CalcBeginPoint(l_posCenter2,g_pDoc->m_stcIOKnife.OArcRadius,l_dAngle2,posArc2);
	//5,求进退刀的凸度
	tudu1 = g_pDoc->RtnTudu(posArc1,End,l_posCenter1,l_iArcClock1);
	tudu2 = g_pDoc->RtnTudu(End,posArc2,l_posCenter2,l_iArcClock2);
	//6,求进刀的直线起点退刀的直线末点
	double AngleEtC1,AngleEtC2;
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle)
	{
		AngleEtC1 = GetAngleToXAxis(l_posCenter1,posArc1)-MATH_PI/2;
		AngleEtC2 = GetAngleToXAxis(l_posCenter2,posArc2)+MATH_PI/2;		
	}
	else
	{
		AngleEtC1 = GetAngleToXAxis(l_posCenter1,posArc1)+MATH_PI/2;
		AngleEtC2 = GetAngleToXAxis(l_posCenter2,posArc2)-MATH_PI/2;		
	}
	posLine1.x = posArc1.x + g_pDoc->m_stcIOKnife.ILineLong * cos(AngleEtC1);
	posLine1.y = posArc1.y + g_pDoc->m_stcIOKnife.ILineLong * sin(AngleEtC1);
	posLine2.x = posArc2.x + g_pDoc->m_stcIOKnife.OLineLong * cos(AngleEtC2);
	posLine2.y = posArc2.y + g_pDoc->m_stcIOKnife.OLineLong * sin(AngleEtC2);
}

//改变椭圆起点和终点
void CGraphIOKinfe::ChangeEllipseBE(CCreateEllipse *Ellipse,POSITION POSCur)
{
	CArray<Position,Position&> l_aryPoint;//点集
	CArray<double,double&> l_aryTuDu;//存放凸度的集
	//		中间用的起点，终点，圆弧起点点，直线起点
	Position l_posBegin,l_posEnd,l_posArc,l_posLine;
	//凸度
	double l_dTudu;
	//判断左右刀补，给每个图形用
	int l_iKnifeStyle = 41;
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iKnifeStyle = 41;
	else
		l_iKnifeStyle = 42;
	
	l_posBegin = Ellipse->m_point.GetAt(0);
	l_posEnd = Ellipse->m_point.GetAt(1);
	//求进刀圆弧的起点和凸度
	CalcArcBgPos(l_posBegin,l_posEnd,l_posArc,l_posLine,l_dTudu);
	//如果是不在中心
	if(g_pDoc->m_stcIOKnife.IsCenter == FALSE)
	{
		double tudu = 0.0;
		//加进刀
		if(!PosEqual(l_posLine,l_posArc))
		{
			l_aryPoint.Add(l_posLine);//加直线
			l_aryTuDu.Add(tudu);	
		}
		l_aryPoint.Add(l_posArc);//加圆弧
		l_aryTuDu.Add(l_dTudu);
		//把原图加进点集和凸度集
		int l_iEllipseOfSize = Ellipse->m_point.GetSize();
		for(int i = 0;i<l_iEllipseOfSize-1;i++)
		{
			l_aryPoint.Add(Ellipse->m_point.GetAt(i));
			tudu = Ellipse->m_dTuDu.GetAt(i);
			l_aryTuDu.Add(tudu);
		}
		tudu = 0.0;
		//加退刀
		l_posBegin = Ellipse->m_point.GetAt(l_iEllipseOfSize-1);
		l_posEnd = Ellipse->m_point.GetAt(0);
		CalcArcEnPos(l_posBegin,l_posEnd,l_posArc,l_posLine,l_dTudu);
		l_aryPoint.Add(l_posEnd);//加圆弧
		l_aryTuDu.Add(l_dTudu);
		l_aryPoint.Add(l_posArc);//加直线
		l_aryTuDu.Add(tudu);
		if(!PosEqual(l_posLine,l_posArc))
		{
			l_aryPoint.Add(l_posLine);//加直线
			l_aryTuDu.Add(tudu);	
		}
		//把点集更新一下
		CCreatePolyline *pTempPolyline = new CCreatePolyline(l_aryPoint,l_aryTuDu);
		pTempPolyline->m_iKnifeStyle = l_iKnifeStyle;	
		pTempPolyline->m_iIsBeginGraph = Ellipse->m_iIsBeginGraph;
		pTempPolyline->m_ColorOfLine = Ellipse->m_ColorOfLine;
		pTempPolyline->m_strCBM.Copy(Ellipse->m_strCBM);	
		pTempPolyline->m_strSBM.Copy(Ellipse->m_strSBM);	
		g_pDoc->m_CCreateBaseList.SetAt(POSCur,pTempPolyline);
		//更新前先存到点集里
		AddUndoList(Ellipse->m_point,Ellipse->m_dTuDu,(LONG)pTempPolyline);		
		//把原撤消链表里存的圆换成多段线
		PUNDOLIST pList = g_pDoc->m_Undo->m_pHeadList;
		while (pList)
		{
			if (pList->pGraphPos&&pList->pGraphPos->pCreateBasepos)
			{
				if (pList->pGraphPos->pCreateBasepos == (LONG)Ellipse) 
					pList->pGraphPos->pCreateBasepos = (LONG)pTempPolyline;
			}
			pList = pList->pNext;
		}
		delete Ellipse;
		Ellipse = NULL;
	}
	else//在中心
	{
	}
	l_aryPoint.RemoveAll();
	l_aryTuDu.RemoveAll();
}



//计算进刀圆弧中心
void CGraphIOKinfe::CalcCenterInput(Position ptStart, Position ptEnd, Position& ptCenter, double radius)
{
	//计算垂直直线的向量
	double dVecX, dVecY;
	if (1 == g_pDoc->m_stcGlassGrind.DaoBuStyle)	//左
	{
		dVecX = -(ptEnd.y-ptStart.y) / ptStart.Distance(ptEnd) * radius;
		dVecY = (ptEnd.x-ptStart.x) / ptStart.Distance(ptEnd) * radius;
	}
	else					//右
	{
		dVecX = (ptEnd.y-ptStart.y) / ptStart.Distance(ptEnd) * radius;
		dVecY = -(ptEnd.x-ptStart.x) / ptStart.Distance(ptEnd) * radius;
	}
	//计算圆心坐标
	ptCenter.x = ptStart.x + dVecX;
	ptCenter.y = ptStart.y + dVecY;
}

//计算退刀圆弧中心
void CGraphIOKinfe::CalcCenterOutput(Position ptStart, Position ptEnd, Position& ptCenter, double radius)
{
	//计算垂直直线的向量
	double dVecX, dVecY;
	double l_dLong = ptStart.Distance(ptEnd);
	if (1 == g_pDoc->m_stcGlassGrind.DaoBuStyle)	//左
	{
		dVecX = -(ptEnd.y-ptStart.y) / l_dLong * radius;
		dVecY = (ptEnd.x-ptStart.x) / l_dLong * radius;
	}
	else					//右
	{
		dVecX = (ptEnd.y-ptStart.y) / l_dLong * radius;
		dVecY = -(ptEnd.x-ptStart.x) / l_dLong * radius;
	}
	//计算圆心坐标
	ptCenter.x = ptEnd.x + dVecX;
	ptCenter.y = ptEnd.y + dVecY;
}

//计算直线中点
void CGraphIOKinfe::CalcMidPoint(Position ptStart, Position ptEnd, Position& ptMid)
{
	ptMid.x = (ptStart.x + ptEnd.x) / 2.0;
	ptMid.y = (ptStart.y + ptEnd.y) / 2.0;
}

//计算圆弧起点
void CGraphIOKinfe::CalcBeginPoint(Position ptCenter, double radius, double angle, Position& ptBeg)
{
	ptBeg.x = ptCenter.x + cos(angle) * radius;
	ptBeg.y = ptCenter.y + sin(angle) * radius;
}

//功能：求进刀圆弧的起点和凸度
//参数：DOUBLEPOINT ptStart：已知直线起点 DOUBLEPOINT ptEnd：已知直线终点
//	    double radius：接入圆弧半径 double dAngleSweep：圆弧扫过角度 
void CGraphIOKinfe::CalcArcBgPos(Position ptStart, Position ptEnd,Position& posArc,Position& posLine,double& tudu)
{
	int l_iArcClock = 0;
	Position ptCenter, ptMid;
	if(g_pDoc->m_stcIOKnife.IsCenter == TRUE)//在中点进刀
	{
		//1.计算中点坐标
		CalcMidPoint(ptStart, ptEnd, ptMid);
		//2.计算圆心坐标
		CalcCenterInput(ptMid, ptEnd, ptCenter, g_pDoc->m_stcIOKnife.IArcRadius);
	}
	else//不在中点进刀
	{
		ptMid  = ptStart;
		CalcCenterInput(ptMid, ptEnd, ptCenter,g_pDoc->m_stcIOKnife.IArcRadius);
	}
	//3.计算终止角度
	double dBeginPointAngle, dEndPointAngle;
	dEndPointAngle = GetAngleToXAxis(ptCenter, ptMid);

	//4.计算起始角度
	if (1 == g_pDoc->m_stcGlassGrind.DaoBuStyle)		//左边，逆时针
	{
		dBeginPointAngle = dEndPointAngle - g_pDoc->m_stcIOKnife.IArcAngle/180*MATH_PI;
		l_iArcClock = AD_COUNTERCLOCKWISE;
	}
	else	//顺时针	右边
	{
		dBeginPointAngle = dEndPointAngle + g_pDoc->m_stcIOKnife.IArcAngle/180*MATH_PI;
		l_iArcClock = AD_CLOCKWISE;
	}	
	//5.计算圆弧起点坐标
	Position ptBegin;
	CalcBeginPoint(ptCenter, g_pDoc->m_stcIOKnife.IArcRadius, dBeginPointAngle, ptBegin);
	//求凸度
	posArc = ptBegin;
	tudu = g_pDoc->RtnTudu(ptBegin,ptStart,ptCenter,l_iArcClock);
	//6，求进刀直线的起点
	double AngleEtC =0.0;
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle)
		AngleEtC = GetAngleToXAxis(ptCenter,ptBegin)-MATH_PI/2;
	else
		AngleEtC = GetAngleToXAxis(ptCenter,ptBegin)+MATH_PI/2;
	posLine.x = ptBegin.x + g_pDoc->m_stcIOKnife.ILineLong * cos(AngleEtC);
	posLine.y = ptBegin.y + g_pDoc->m_stcIOKnife.ILineLong * sin(AngleEtC);
}

//求退刀圆弧的末点和凸度
void CGraphIOKinfe::CalcArcEnPos(Position ptStart,Position ptEnd,Position& posArc,Position& posLine,double& tudu)
{
	int l_iArcClock = 0;
	Position ptCenter, ptMid;
	if(g_pDoc->m_stcIOKnife.IsCenter == TRUE)//在中点进刀
	{
		//1.计算中点坐标
		CalcMidPoint(ptStart, ptEnd, ptMid);
		//2.计算圆心坐标
		CalcCenterOutput(ptStart,ptMid, ptCenter, g_pDoc->m_stcIOKnife.OArcRadius);
	}
	else//不在中点进刀
	{
		ptMid  = ptEnd;
		CalcCenterOutput(ptStart,ptMid , ptCenter, g_pDoc->m_stcIOKnife.OArcRadius);
	}
	//3.计算起始角度
	double dBeginPointAngle, dEndPointAngle;
	dBeginPointAngle = GetAngleToXAxis(ptCenter, ptMid);
	
	//4.计算终止角度
	if (1 == g_pDoc->m_stcGlassGrind.DaoBuStyle)		//左边，逆时针
	{
		dEndPointAngle = dBeginPointAngle + g_pDoc->m_stcIOKnife.OArcAngle/180*MATH_PI;
		l_iArcClock = AD_COUNTERCLOCKWISE;
	}
	else	//顺时针  右边
	{
		dEndPointAngle = dBeginPointAngle - g_pDoc->m_stcIOKnife.OArcAngle/180*MATH_PI;
		l_iArcClock = AD_CLOCKWISE;
	}
	//5.计算圆弧末点坐标
	Position l_posEnd;
	CalcBeginPoint(ptCenter, g_pDoc->m_stcIOKnife.OArcRadius, dEndPointAngle, l_posEnd);
	//求凸度
	posArc = l_posEnd;
	tudu = g_pDoc->RtnTudu(ptMid,l_posEnd,ptCenter,l_iArcClock);
	//6，求退刀直线的末点
	double AngleEtC =0.0;//直线末点与圆心的角度 
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle)
		AngleEtC = GetAngleToXAxis(ptCenter,l_posEnd)+MATH_PI/2;
	else
		AngleEtC = GetAngleToXAxis(ptCenter,l_posEnd)-MATH_PI/2;
	posLine.x = l_posEnd.x +g_pDoc-> m_stcIOKnife.OLineLong * cos(AngleEtC);
	posLine.y = l_posEnd.y + g_pDoc->m_stcIOKnife.OLineLong * sin(AngleEtC);	
}

/*
 *	向撤消链表里添加操作
 */
void CGraphIOKinfe::AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,LONG CreateBase)
{
	if (m_pUndoList == NULL)
	{
		m_pUndoList = m_UndoRedo.NewUndoList();
		m_pUndoList->OperatieWhat = ID_IO_KNIFE;
		m_pCurGraphPos = m_UndoRedo.NewGraphPos();
		m_pCurGraphPos->pCreateBasepos = CreateBase;
		m_pCurGraphPos->aryPoint.SetSize(aryPoint.GetSize());
		m_pCurGraphPos->aryPoint.Copy(aryPoint);
		m_pCurGraphPos->aryTudu.SetSize(aryTudu.GetSize());
		m_pCurGraphPos->aryTudu.Copy(aryTudu);
		m_pUndoList->pGraphPos = m_pCurGraphPos;
		
	}
	else
	{
		m_pGraphPos = m_UndoRedo.NewGraphPos();						
		m_pGraphPos->pCreateBasepos = CreateBase;
		m_pGraphPos->aryPoint.SetSize(aryPoint.GetSize());
		m_pGraphPos->aryPoint.Copy(aryPoint);
		m_pGraphPos->aryTudu.SetSize(aryTudu.GetSize());
		m_pGraphPos->aryTudu.Copy(aryTudu);
		m_pCurGraphPos->pNext = m_pGraphPos;
		m_pGraphPos->pPrev = m_pCurGraphPos;
		m_pCurGraphPos = m_pGraphPos;
	}
}


