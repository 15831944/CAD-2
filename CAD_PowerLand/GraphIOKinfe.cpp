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
 *	��ͼ�μӽ��˵�
 */
void CGraphIOKinfe::PutGraphIOKife()
{
	POSITION l_POSList;
	//�Ƿ���Ҫ�����ͼ�εı�־
	BOOL l_bIsAdd = FALSE;	
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	for( int i = 0 ; i < l_iSize; i++ )
	{
		CCreateBase*	l_pCreateBase = (CCreateBase*)g_pDoc->m_CollectOfPicked[i] ;		
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
		{//���߶�
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);
			ChangePolyBE(Polyline,l_POSList,TRUE);
			l_bIsAdd = TRUE;
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{//����
			CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			ChangePolyBE(Rectangle,l_POSList,FALSE);
			l_bIsAdd = TRUE;
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//Բ
			CCreateRound *Round = (CCreateRound*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);
			ChangeRoundBE(Round,l_POSList);
			l_bIsAdd = TRUE;
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//��Բ
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

//�ı����������յ�
void CGraphIOKinfe::ChangePolyBE(CCreateBase *line,POSITION POSCur,BOOL IsPolyline)
{
	CCreatePolyline *Polyline = (CCreatePolyline*)line;
	CArray<Position,Position&> l_aryPoint;//�㼯
	CArray<double,double&> l_aryTuDu;//���͹�ȵļ�
	//		�м��õ���㣬�յ㣬Բ�����㣬ֱ�����//Բ����Բ��; ����������û��
	Position l_posBegin,l_posEnd,l_posArc,l_posLine,l_posArcCenter,l_posArc1,l_posLine1;;
	int l_iClock = 0;//Բ������
	//�ж����ҵ�������ÿ��ͼ����
	int l_iKnifeStyle = 41;
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iKnifeStyle = 41;
	else
		l_iKnifeStyle = 42;
	//͹��
	double l_dTudu,l_dUseTudu;
	l_posBegin = Polyline->m_point.GetAt(0);
	l_posEnd = Polyline->m_point.GetAt(1);
	//�����Բ��������͹��
	if(Polyline->m_dTuDu.GetAt(0) == 0.0)//�����ֱ�ߵĻ�
		CalcArcBgPos(l_posBegin,l_posEnd,l_posArc,l_posLine,l_dTudu);
	else//������Բ��
	{
		l_dUseTudu = Polyline->m_dTuDu.GetAt(0);
		if(l_dUseTudu >0) l_iClock = AD_COUNTERCLOCKWISE;
		else l_iClock = AD_CLOCKWISE;
		g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dUseTudu,l_posArcCenter);
		//��Բ���ĵ�͹�Ȼ���ֱ�ߵĵ㣬������������� û����
		CalcArcCenterForRound(l_iClock,l_posArcCenter,l_posBegin,l_posArc,l_posLine,l_dTudu,l_posArc1,l_posLine1,l_dUseTudu);	
	}
	//����ǲ�������
	if(g_pDoc->m_stcIOKnife.IsCenter == FALSE)
	{
		double tudu = 0.0;
		//�ӽ���
		if(!PosEqual(l_posLine,l_posArc))//��ֱ���Ƿ�Ϊ��
		{
			l_aryPoint.Add(l_posLine);//��ֱ��
			l_aryTuDu.Add(tudu);
		}
		l_aryPoint.Add(l_posArc);//��Բ��
		l_aryTuDu.Add(l_dTudu);
		int l_iSizeOfPoint = Polyline->m_point.GetSize()-2;//�����ڶ���
		//��ԭͼ�ӽ��㼯��͹�ȼ�
		for(int i = 0;i<l_iSizeOfPoint+1;i++)
		{
			l_aryPoint.Add(Polyline->m_point.GetAt(i));
			tudu = Polyline->m_dTuDu.GetAt(i);
			l_aryTuDu.Add(tudu);
		}
		tudu = 0.0;
		//���˵�
		l_posBegin = Polyline->m_point.GetAt(l_iSizeOfPoint);//�����ڶ���
		l_posEnd = Polyline->m_point.GetAt(l_iSizeOfPoint+1);//������һ��
		if(Polyline->m_dTuDu.GetAt(l_iSizeOfPoint) == 0.0)//�����ֱ�ߵĻ�
			CalcArcEnPos(l_posBegin,l_posEnd,l_posArc,l_posLine,l_dTudu);
		else
		{
			l_dUseTudu = Polyline->m_dTuDu.GetAt(l_iSizeOfPoint);
			if(l_dUseTudu >0) l_iClock = AD_COUNTERCLOCKWISE;
			else l_iClock = AD_CLOCKWISE;
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dUseTudu,l_posArcCenter);
			//��Բ���ĵ�͹�Ȼ���ֱ�ߵĵ㣬������������� û����
			CalcArcCenterForRound(l_iClock,l_posArcCenter,l_posEnd,l_posArc1,l_posLine1,l_dUseTudu,l_posArc,l_posLine,l_dTudu);			
		}
		l_aryPoint.Add(l_posEnd);//��Բ��
		l_aryTuDu.Add(l_dTudu);
		l_aryPoint.Add(l_posArc);//��ֱ��
		l_aryTuDu.Add(tudu);
		if(!PosEqual(l_posLine,l_posArc))//��ֱ���Ƿ�Ϊ��
		{
			l_aryPoint.Add(l_posLine);//��ֱ��
			l_aryTuDu.Add(tudu);
		}
		//����ǰ�ȴ浽�㼯��
		AddUndoList(Polyline->m_point,Polyline->m_dTuDu,(LONG)Polyline);	
		//�ѵ㼯����һ��
		Polyline->m_point.Copy(l_aryPoint);
		Polyline->m_dTuDu.Copy(l_aryTuDu);
	}
	else//������
	{
	}	
	l_aryPoint.RemoveAll();
	l_aryTuDu.RemoveAll();
}

//�ı�Բ�����յ�
void CGraphIOKinfe::ChangeRoundBE(CCreateRound *Round,POSITION POSCur)
{
	CArray<Position,Position&> l_aryPoint,l_aryPoint1;//�㼯
	CArray<double,double&> l_aryTuDu,l_aryTuDu1;//���͹�ȵļ�
	//		�м��õ���㣬�յ㣬Բ�����㣬ֱ�����
	Position l_posCenter,l_posEnd,l_posArc1,l_posLine1,l_posArc2,l_posLine2;
	//͹��
	double l_dTudu1,l_dTudu2;
	//�ж����ҵ�������ÿ��ͼ����
	int l_iKnifeStyle = 41;
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iKnifeStyle = 41;
	else
		l_iKnifeStyle = 42;
	l_posCenter = Round->m_Center;
	l_posEnd = Round->m_EndPos;
	int l_iClock = Round->m_iDirect;
	//�����Բ��������͹��
	CalcArcCenterForRound(l_iClock,l_posCenter,l_posEnd,l_posArc1,l_posLine1,l_dTudu1,l_posArc2,l_posLine2,l_dTudu2);
	double tudu = 0.0;
	//�ӽ���
	if(!PosEqual(l_posLine1,l_posArc1))
	{
		l_aryPoint.Add(l_posLine1);//��ֱ��
		l_aryTuDu.Add(tudu);
	}
	l_aryPoint.Add(l_posArc1);//��Բ��
	l_aryTuDu.Add(l_dTudu1);
	//��Բ��������Բ��
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
	//���˵�
	l_aryPoint.Add(l_posEnd);//��Բ��
	l_aryTuDu.Add(l_dTudu2);
	l_aryPoint.Add(l_posArc2);//��ֱ��
	l_aryTuDu.Add(tudu);
	if(!PosEqual(l_posLine2,l_posArc2))
	{
		l_aryPoint.Add(l_posLine2);//��ֱ��
		l_aryTuDu.Add(tudu);
	}
	CCreatePolyline *pTempPolyline2 = new CCreatePolyline(l_aryPoint,l_aryTuDu);
	pTempPolyline2->m_iKnifeStyle = l_iKnifeStyle;	
	pTempPolyline2->m_iIsBeginGraph = Round->m_iIsBeginGraph;
	pTempPolyline2->m_ColorOfLine = Round->m_ColorOfLine;
	pTempPolyline2->m_strCBM.Copy(Round->m_strCBM);	
	pTempPolyline2->m_strSBM.Copy(Round->m_strSBM);	
	g_pDoc->m_CCreateBaseList.SetAt(POSCur,pTempPolyline2);
	//����ǰ�ȴ浽�㼯��	
	AddUndoList(l_aryPoint1,l_aryTuDu1,(LONG)pTempPolyline2);	
	//�ͷ��ڴ�
	l_aryPoint1.RemoveAll();
	l_aryTuDu1.RemoveAll();
	l_aryPoint.RemoveAll();
	l_aryTuDu.RemoveAll();
	//��ԭ������������Բ���ɶ����
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

//��Բ�Ľ�����Բ��
void CGraphIOKinfe::CalcArcCenterForRound(int l_iClock,Position Center,Position End,Position& posArc1,Position& posLine1,
						double& tudu1,Position& posArc2,Position& posLine2,double& tudu2)
{
	//1��������Բ���е�ĽǶ�
	double l_dAngleBgEn = GetAngleToXAxis(End,Center);//˳ʱ����,��ʱ����
	if ((g_pDoc->m_stcGlassGrind.DaoBuStyle&&l_iClock == AD_COUNTERCLOCKWISE)||((!g_pDoc->m_stcGlassGrind.DaoBuStyle&&l_iClock == AD_CLOCKWISE)))		//����
		l_dAngleBgEn = MATH_PI + l_dAngleBgEn;
	//2������
	Position l_posCenter1,l_posCenter2;
	l_posCenter1.x = End.x - cos(l_dAngleBgEn)*g_pDoc->m_stcIOKnife.IArcRadius;
	l_posCenter1.y = End.y - sin(l_dAngleBgEn)*g_pDoc->m_stcIOKnife.IArcRadius;
	l_posCenter2.x = End.x - cos(l_dAngleBgEn)*g_pDoc->m_stcIOKnife.OArcRadius;
	l_posCenter2.y = End.y - sin(l_dAngleBgEn)*g_pDoc->m_stcIOKnife.OArcRadius;
	//3�������������ʼ�ǶȺ��˵��Ľ����Ƕ�
	double l_dAngle1,l_dAngle2;	//���˵��Ľ����Ƕ�
	int l_iArcClock1,l_iArcClock2;//���˵�Բ���ķ���
	if (g_pDoc->m_stcGlassGrind.DaoBuStyle)		//��
	{
		l_dAngle1 = l_dAngleBgEn - g_pDoc->m_stcIOKnife.IArcAngle/180*MATH_PI;
		l_dAngle2 = l_dAngleBgEn + g_pDoc->m_stcIOKnife.OArcAngle/180*MATH_PI;
		l_iArcClock1 = l_iArcClock2 = AD_COUNTERCLOCKWISE;
	}
	else	//��
	{
		l_dAngle1 = l_dAngleBgEn + g_pDoc->m_stcIOKnife.IArcAngle/180*MATH_PI;
		l_dAngle2 = l_dAngleBgEn - g_pDoc->m_stcIOKnife.OArcAngle/180*MATH_PI;
		l_iArcClock1 = l_iArcClock2 =  AD_CLOCKWISE;
	}

	//4�����������ʼ�㣬�˵���ĩ��
	CalcBeginPoint(l_posCenter1,g_pDoc->m_stcIOKnife.IArcRadius,l_dAngle1,posArc1);
	CalcBeginPoint(l_posCenter2,g_pDoc->m_stcIOKnife.OArcRadius,l_dAngle2,posArc2);
	//5,����˵���͹��
	tudu1 = g_pDoc->RtnTudu(posArc1,End,l_posCenter1,l_iArcClock1);
	tudu2 = g_pDoc->RtnTudu(End,posArc2,l_posCenter2,l_iArcClock2);
	//6,�������ֱ������˵���ֱ��ĩ��
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

//�ı���Բ�����յ�
void CGraphIOKinfe::ChangeEllipseBE(CCreateEllipse *Ellipse,POSITION POSCur)
{
	CArray<Position,Position&> l_aryPoint;//�㼯
	CArray<double,double&> l_aryTuDu;//���͹�ȵļ�
	//		�м��õ���㣬�յ㣬Բ�����㣬ֱ�����
	Position l_posBegin,l_posEnd,l_posArc,l_posLine;
	//͹��
	double l_dTudu;
	//�ж����ҵ�������ÿ��ͼ����
	int l_iKnifeStyle = 41;
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iKnifeStyle = 41;
	else
		l_iKnifeStyle = 42;
	
	l_posBegin = Ellipse->m_point.GetAt(0);
	l_posEnd = Ellipse->m_point.GetAt(1);
	//�����Բ��������͹��
	CalcArcBgPos(l_posBegin,l_posEnd,l_posArc,l_posLine,l_dTudu);
	//����ǲ�������
	if(g_pDoc->m_stcIOKnife.IsCenter == FALSE)
	{
		double tudu = 0.0;
		//�ӽ���
		if(!PosEqual(l_posLine,l_posArc))
		{
			l_aryPoint.Add(l_posLine);//��ֱ��
			l_aryTuDu.Add(tudu);	
		}
		l_aryPoint.Add(l_posArc);//��Բ��
		l_aryTuDu.Add(l_dTudu);
		//��ԭͼ�ӽ��㼯��͹�ȼ�
		int l_iEllipseOfSize = Ellipse->m_point.GetSize();
		for(int i = 0;i<l_iEllipseOfSize-1;i++)
		{
			l_aryPoint.Add(Ellipse->m_point.GetAt(i));
			tudu = Ellipse->m_dTuDu.GetAt(i);
			l_aryTuDu.Add(tudu);
		}
		tudu = 0.0;
		//���˵�
		l_posBegin = Ellipse->m_point.GetAt(l_iEllipseOfSize-1);
		l_posEnd = Ellipse->m_point.GetAt(0);
		CalcArcEnPos(l_posBegin,l_posEnd,l_posArc,l_posLine,l_dTudu);
		l_aryPoint.Add(l_posEnd);//��Բ��
		l_aryTuDu.Add(l_dTudu);
		l_aryPoint.Add(l_posArc);//��ֱ��
		l_aryTuDu.Add(tudu);
		if(!PosEqual(l_posLine,l_posArc))
		{
			l_aryPoint.Add(l_posLine);//��ֱ��
			l_aryTuDu.Add(tudu);	
		}
		//�ѵ㼯����һ��
		CCreatePolyline *pTempPolyline = new CCreatePolyline(l_aryPoint,l_aryTuDu);
		pTempPolyline->m_iKnifeStyle = l_iKnifeStyle;	
		pTempPolyline->m_iIsBeginGraph = Ellipse->m_iIsBeginGraph;
		pTempPolyline->m_ColorOfLine = Ellipse->m_ColorOfLine;
		pTempPolyline->m_strCBM.Copy(Ellipse->m_strCBM);	
		pTempPolyline->m_strSBM.Copy(Ellipse->m_strSBM);	
		g_pDoc->m_CCreateBaseList.SetAt(POSCur,pTempPolyline);
		//����ǰ�ȴ浽�㼯��
		AddUndoList(Ellipse->m_point,Ellipse->m_dTuDu,(LONG)pTempPolyline);		
		//��ԭ������������Բ���ɶ����
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
	else//������
	{
	}
	l_aryPoint.RemoveAll();
	l_aryTuDu.RemoveAll();
}



//�������Բ������
void CGraphIOKinfe::CalcCenterInput(Position ptStart, Position ptEnd, Position& ptCenter, double radius)
{
	//���㴹ֱֱ�ߵ�����
	double dVecX, dVecY;
	if (1 == g_pDoc->m_stcGlassGrind.DaoBuStyle)	//��
	{
		dVecX = -(ptEnd.y-ptStart.y) / ptStart.Distance(ptEnd) * radius;
		dVecY = (ptEnd.x-ptStart.x) / ptStart.Distance(ptEnd) * radius;
	}
	else					//��
	{
		dVecX = (ptEnd.y-ptStart.y) / ptStart.Distance(ptEnd) * radius;
		dVecY = -(ptEnd.x-ptStart.x) / ptStart.Distance(ptEnd) * radius;
	}
	//����Բ������
	ptCenter.x = ptStart.x + dVecX;
	ptCenter.y = ptStart.y + dVecY;
}

//�����˵�Բ������
void CGraphIOKinfe::CalcCenterOutput(Position ptStart, Position ptEnd, Position& ptCenter, double radius)
{
	//���㴹ֱֱ�ߵ�����
	double dVecX, dVecY;
	double l_dLong = ptStart.Distance(ptEnd);
	if (1 == g_pDoc->m_stcGlassGrind.DaoBuStyle)	//��
	{
		dVecX = -(ptEnd.y-ptStart.y) / l_dLong * radius;
		dVecY = (ptEnd.x-ptStart.x) / l_dLong * radius;
	}
	else					//��
	{
		dVecX = (ptEnd.y-ptStart.y) / l_dLong * radius;
		dVecY = -(ptEnd.x-ptStart.x) / l_dLong * radius;
	}
	//����Բ������
	ptCenter.x = ptEnd.x + dVecX;
	ptCenter.y = ptEnd.y + dVecY;
}

//����ֱ���е�
void CGraphIOKinfe::CalcMidPoint(Position ptStart, Position ptEnd, Position& ptMid)
{
	ptMid.x = (ptStart.x + ptEnd.x) / 2.0;
	ptMid.y = (ptStart.y + ptEnd.y) / 2.0;
}

//����Բ�����
void CGraphIOKinfe::CalcBeginPoint(Position ptCenter, double radius, double angle, Position& ptBeg)
{
	ptBeg.x = ptCenter.x + cos(angle) * radius;
	ptBeg.y = ptCenter.y + sin(angle) * radius;
}

//���ܣ������Բ��������͹��
//������DOUBLEPOINT ptStart����ֱ֪����� DOUBLEPOINT ptEnd����ֱ֪���յ�
//	    double radius������Բ���뾶 double dAngleSweep��Բ��ɨ���Ƕ� 
void CGraphIOKinfe::CalcArcBgPos(Position ptStart, Position ptEnd,Position& posArc,Position& posLine,double& tudu)
{
	int l_iArcClock = 0;
	Position ptCenter, ptMid;
	if(g_pDoc->m_stcIOKnife.IsCenter == TRUE)//���е����
	{
		//1.�����е�����
		CalcMidPoint(ptStart, ptEnd, ptMid);
		//2.����Բ������
		CalcCenterInput(ptMid, ptEnd, ptCenter, g_pDoc->m_stcIOKnife.IArcRadius);
	}
	else//�����е����
	{
		ptMid  = ptStart;
		CalcCenterInput(ptMid, ptEnd, ptCenter,g_pDoc->m_stcIOKnife.IArcRadius);
	}
	//3.������ֹ�Ƕ�
	double dBeginPointAngle, dEndPointAngle;
	dEndPointAngle = GetAngleToXAxis(ptCenter, ptMid);

	//4.������ʼ�Ƕ�
	if (1 == g_pDoc->m_stcGlassGrind.DaoBuStyle)		//��ߣ���ʱ��
	{
		dBeginPointAngle = dEndPointAngle - g_pDoc->m_stcIOKnife.IArcAngle/180*MATH_PI;
		l_iArcClock = AD_COUNTERCLOCKWISE;
	}
	else	//˳ʱ��	�ұ�
	{
		dBeginPointAngle = dEndPointAngle + g_pDoc->m_stcIOKnife.IArcAngle/180*MATH_PI;
		l_iArcClock = AD_CLOCKWISE;
	}	
	//5.����Բ���������
	Position ptBegin;
	CalcBeginPoint(ptCenter, g_pDoc->m_stcIOKnife.IArcRadius, dBeginPointAngle, ptBegin);
	//��͹��
	posArc = ptBegin;
	tudu = g_pDoc->RtnTudu(ptBegin,ptStart,ptCenter,l_iArcClock);
	//6�������ֱ�ߵ����
	double AngleEtC =0.0;
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle)
		AngleEtC = GetAngleToXAxis(ptCenter,ptBegin)-MATH_PI/2;
	else
		AngleEtC = GetAngleToXAxis(ptCenter,ptBegin)+MATH_PI/2;
	posLine.x = ptBegin.x + g_pDoc->m_stcIOKnife.ILineLong * cos(AngleEtC);
	posLine.y = ptBegin.y + g_pDoc->m_stcIOKnife.ILineLong * sin(AngleEtC);
}

//���˵�Բ����ĩ���͹��
void CGraphIOKinfe::CalcArcEnPos(Position ptStart,Position ptEnd,Position& posArc,Position& posLine,double& tudu)
{
	int l_iArcClock = 0;
	Position ptCenter, ptMid;
	if(g_pDoc->m_stcIOKnife.IsCenter == TRUE)//���е����
	{
		//1.�����е�����
		CalcMidPoint(ptStart, ptEnd, ptMid);
		//2.����Բ������
		CalcCenterOutput(ptStart,ptMid, ptCenter, g_pDoc->m_stcIOKnife.OArcRadius);
	}
	else//�����е����
	{
		ptMid  = ptEnd;
		CalcCenterOutput(ptStart,ptMid , ptCenter, g_pDoc->m_stcIOKnife.OArcRadius);
	}
	//3.������ʼ�Ƕ�
	double dBeginPointAngle, dEndPointAngle;
	dBeginPointAngle = GetAngleToXAxis(ptCenter, ptMid);
	
	//4.������ֹ�Ƕ�
	if (1 == g_pDoc->m_stcGlassGrind.DaoBuStyle)		//��ߣ���ʱ��
	{
		dEndPointAngle = dBeginPointAngle + g_pDoc->m_stcIOKnife.OArcAngle/180*MATH_PI;
		l_iArcClock = AD_COUNTERCLOCKWISE;
	}
	else	//˳ʱ��  �ұ�
	{
		dEndPointAngle = dBeginPointAngle - g_pDoc->m_stcIOKnife.OArcAngle/180*MATH_PI;
		l_iArcClock = AD_CLOCKWISE;
	}
	//5.����Բ��ĩ������
	Position l_posEnd;
	CalcBeginPoint(ptCenter, g_pDoc->m_stcIOKnife.OArcRadius, dEndPointAngle, l_posEnd);
	//��͹��
	posArc = l_posEnd;
	tudu = g_pDoc->RtnTudu(ptMid,l_posEnd,ptCenter,l_iArcClock);
	//6�����˵�ֱ�ߵ�ĩ��
	double AngleEtC =0.0;//ֱ��ĩ����Բ�ĵĽǶ� 
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle)
		AngleEtC = GetAngleToXAxis(ptCenter,l_posEnd)+MATH_PI/2;
	else
		AngleEtC = GetAngleToXAxis(ptCenter,l_posEnd)-MATH_PI/2;
	posLine.x = l_posEnd.x +g_pDoc-> m_stcIOKnife.OLineLong * cos(AngleEtC);
	posLine.y = l_posEnd.y + g_pDoc->m_stcIOKnife.OLineLong * sin(AngleEtC);	
}

/*
 *	������������Ӳ���
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


