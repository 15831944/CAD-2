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
 *	��ͼ�α�ΪҪϳ��ͼ��
 */
void CTurnMill::TurnGraphMill()
{
	POSITION pos = NULL ;
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize() ;
	//�Ƿ���Ҫ�����ͼ�εı�־
	BOOL l_bIsAdd = FALSE;
	//��ѡ�����ͼ�η���
	for( int i = 0 ; i < l_iSize; i++ )
	{
		m_pMillGraph = (CCreateBase*)g_pDoc->m_CollectOfPicked[i] ;
		if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			l_bIsAdd = TRUE;
			TurnLine();
		}
		
		//���߶�//////////////////////////////////////////
		else if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreatePolyline))||
			m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)m_pMillGraph;
			l_bIsAdd = TRUE;
			//������ǱպϵĻ� 			
			if (!PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))
				TurnOpenPolyline();
			else
				TurnClosePolyline();
		}
		
		//����//////////////////////////////////////////
		else if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			l_bIsAdd = TRUE;
			CCreatePolyline *Polyline = (CCreatePolyline*)m_pMillGraph;		
			TurnClosePolyline();
		}
		else if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//Բ
			l_bIsAdd = TRUE;
			TurnRound();
		}
		
		else if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{//Բ��
			l_bIsAdd = TRUE;
			TurnArc();
		}
		else if (m_pMillGraph->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//��Բ
			l_bIsAdd = TRUE;
			TurnEllipse();
		}
	}	
	if (l_bIsAdd == TRUE)
		g_pDoc->m_Undo->AddUndoList(m_pUndoList);
	//ԭͼ��ǳɫ�һ�һ��
	int l_iSizeCollect = g_pDoc->m_CollectOfPicked.GetSize() ;
	for( int j = 0 ; j < l_iSizeCollect; j++ )
	{
		CCreateBase* pSelEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[j] ;
		g_pDoc->m_CCreateBaselayer1.AddTail(pSelEntity);
	}
	//���ѡ��
	g_pDoc->m_CollectOfPicked.RemoveAll() ;
	g_pView->Invalidate();
}

/*
 *	�Ѳ��պ϶���߱��ϳ����Ҫ��ͼ��
 */
void CTurnMill::TurnOpenPolyline()
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pMillGraph;
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer;
	//����ߵĵ㼯�Ĵ�С
	int l_iSize = Polyline->m_point.GetSize()-1; 
	//�����������������������ĸ����    ����      ������ϵ�������
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin;
	//����ߵ�������������X��ĽǶȺ�͹��
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dTudu180 = TUDU_180;	
	//�µĵ㼯��͹��
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	//�����е㴦��
	for(int i = 0;i<l_iSize;i++)
	{
		l_posPoly1 = Polyline->m_point.GetAt(i);
		l_posPoly2 = Polyline->m_point.GetAt(i+1);
		//����ӵ����һ�㣬�˳�ѭ�������ؼӡ�
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			if (i ==0)//�������߾�һ��ֱ��
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
		//����������ĸ���
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
		if (i ==0)//�ӵ�һ��
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
		}
		//�����ƽ�У����ؽ���
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	//���ؼӵ�
	for(int j = l_iSize;j>0;j--)
	{
		l_posPoly1 = Polyline->m_point.GetAt(j);
		l_posPoly2 = Polyline->m_point.GetAt(j-1);
		//������һ�㣬�ٰ������ϣ�ʹ�պ�
		if (j-1 == 0) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			if (j ==l_iSize)//�������߾�һ��ֱ��
			{
				l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
				l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
				l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
				l_aryPoint1.Add(l_pos1);
				l_aryTudu1.Add(l_dTudu);
			}			
			l_aryPoint1.Add(l_pos2);
			g_pDoc->m_stcClothCut.iChamferEndStyle == 0?(l_aryTudu1.Add(l_dTudu180)):(l_aryTudu1.Add(l_dTudu));
			//����㣬ʹ�պ�
			l_aryPoint1.Add(l_posBegin);
			l_aryTudu1.Add(l_dTudu);	
			break;
		}
		l_posPoly3 = Polyline->m_point.GetAt(j-2);
		//��������ĵ�
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
		if (j == l_iSize)//�ӵ�һ��
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
		}
		//�����ƽ�У����ؽ���
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
 *	�������ߵĽ���
 */
BOOL CTurnMill::GetIntersectPoint(Position pos1,Position pos2,Position pos3,Position pos4,Position &pos)
{
	//�����ߵ�б�ʺͽؾ�
	double k1,k2,b1,b2;
	double xVal1 = (pos1.x - pos2.x);
	double xVal2 = (pos3.x - pos4.x);
	//�����һ������X��û�н���
	if (xVal1 == 0&&xVal2 != 0)
	{
		//��б��
		k2 = (pos3.y - pos4.y)/xVal2;
		//��ؾ�
		b2 = pos3.y - k2*pos3.x;
		//�󽻵�
		pos.x = pos1.x;
		pos.y = k2*pos1.x+b2;
		return TRUE;
		
	}
	//�����һ������X��û�н���
	else if (xVal2 == 0&&xVal1 != 0)
	{
		//��б��
		k1 = (pos1.y - pos2.y)/xVal1;
		//��ؾ�
		b1 = pos1.y - k1*pos1.x;
		//�󽻵�
		pos.x = pos3.x;
		pos.y = k1*pos3.x+b1;
		return TRUE;
	}
	else if (xVal2 == 0&&xVal1 == 0)
		return FALSE;
	//��б��
	k1 = (pos1.y - pos2.y)/xVal1;
	k2 = (pos3.y - pos4.y)/xVal2;
	//���б��һ��û�н���
	if (fabs(k1-k2)<DISTANCE_ZERO)
		return FALSE;
	//��ؾ�
	b1 = pos1.y - k1*pos1.x;
	b2 = pos3.y - k2*pos3.x;
	//�󽻵�
	pos.x = (b2-b1)/(k1-k2);
	pos.y = k1*pos.x+b1;
	return TRUE;
}

/*
 *	�ѱպϵĶ��߶�ת�� ��ϳ����Ҫ��ͼ��
 */
void CTurnMill::TurnClosePolyline()
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pMillGraph;
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer;
	//����ߵĵ㼯�Ĵ�С
	int l_iSize = Polyline->m_point.GetSize()-1; 
	//�����������������������ĸ����    ����      ������ϵ�������
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin,l_posEnd;
	//����ߵ�������������X��ĽǶȺ�͹��
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dAgl90 = MATH_PI/2;	
	//��������ֱ���¼�����ֱ��������
	//�µĵ㼯��͹��
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	//������һȦ
	for(int i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = Polyline->m_point.GetAt(i);
		l_posPoly2 = Polyline->m_point.GetAt(i+1);
		//����ӵ����һ�㣬�˳�ѭ�������������һ��
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//���һ��ֱ����ڶ���ֱ�ߵĽ���,���ƽ��
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//����ĩ���غ�
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
		//����������ĸ���
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
		if (i ==0)//�ӵ�һ��
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//�����ƽ�У����ؽ���
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
	//������һȦ
	for(i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = Polyline->m_point.GetAt(i);
		l_posPoly2 = Polyline->m_point.GetAt(i+1);
		//����ӵ����һ�㣬�˳�ѭ�������������һ��
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//���һ��ֱ����ڶ���ֱ�ߵĽ���,���ƽ��
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//����ĩ���غ�
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
		//����������ĸ���
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
		if (i ==0)//�ӵ�һ��
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//�����ƽ�У����ؽ���
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
 *	������������Ӳ����պϵ�
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
 *	��ֱ�߱�ϳ��ͼ��
 */
void CTurnMill::TurnLine()
{
	CCreateLine *pLine = (CCreateLine*)m_pMillGraph;
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer;
	//ֱ����������ĸ����                   ���	   ĩ��
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_posPoly1,l_posPoly2;
	//����ߵ�������������X��ĽǶ�
	double l_dAngle1,l_dAngle2,l_dTudu = 0.0,l_dTudu180 = TUDU_180;	
	//�µĵ㼯��͹��
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	l_posPoly1 = pLine->m_Begin;
	l_posPoly2 = pLine->m_End;
	//����������ĸ���
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
 *	��Բ����ϳ����
 */
void CTurnMill::TurnArc()
{
	CCreateRoundArc *pArc = (CCreateRoundArc*)m_pMillGraph;
	//ϳ�����
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer,l_dTudu180 = -TUDU_180;
	//ֱ����������ĸ����                   ���	   ĩ��
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_posPoly1,l_posPoly2;
	l_posPoly1 = pArc->m_Begin;
	l_posPoly2 = pArc->m_End;
	//�µĵ㼯��͹��
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	//Բ�����ĽǶ�
	double l_dAglBG = GetAngleToXAxis(pArc->m_Center,pArc->m_Begin);
	//Բ�����ĽǶ�
	double l_dAglEN = GetAngleToXAxis(pArc->m_Center,pArc->m_End);
	l_pos1.x = l_posPoly1.x - l_dLongChamfer*RetainValid(cos(l_dAglBG),8);
	l_pos1.y = l_posPoly1.y - l_dLongChamfer*RetainValid(sin(l_dAglBG),8);
	l_pos2.x = l_posPoly2.x - l_dLongChamfer*RetainValid(cos(l_dAglEN),8);
	l_pos2.y = l_posPoly2.y - l_dLongChamfer*RetainValid(sin(l_dAglEN),8);
	l_pos3.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAglEN),8);
	l_pos3.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAglEN),8);
	l_pos4.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAglBG),8);
	l_pos4.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAglBG),8);
	//͹��
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
 *	��Բ��ϳ����
 */
void CTurnMill::TurnRound()
{
	CCreateRound *pRound = (CCreateRound*)m_pMillGraph;
	//ϳ�����
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer;
	//Բ����ĩ��ĽǶ�
	double l_dAgl = GetAngleToXAxis(pRound->m_Center,pRound->m_EndPos);
	//��Բ��ĩ��
	Position posOut(pRound->m_Center.x + (l_dLongChamfer+pRound->m_dRadius)*cos(l_dAgl),
					pRound->m_Center.y + (l_dLongChamfer+pRound->m_dRadius)*sin(l_dAgl));
	//��Բ��ĩ��
	Position posIn(pRound->m_Center.x + (-l_dLongChamfer+pRound->m_dRadius)*cos(l_dAgl),
		pRound->m_Center.y + (-l_dLongChamfer+pRound->m_dRadius)*sin(l_dAgl));
	
	//��ϳ�����Բ
	CCreateRound *pRound1 = new CCreateRound(pRound->m_Center,posOut,pRound->m_iDirect);
	CCreateRound *pRound2 = new CCreateRound(pRound->m_Center,posIn,pRound->m_iDirect);
	//���뵽����
	POSITION POS = g_pDoc->m_CCreateBaseList.Find(pRound);
	g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound1);
	g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound2);	
	g_pDoc->m_CCreateBaseList.RemoveAt(POS);
	AddUndoList((long)pRound1,(long)pRound2);	
}

/*
 *	����Բ��ϳ����
 */
void CTurnMill::TurnEllipse()
{
	CCreateEllipse *pEllipse = (CCreateEllipse*)m_pMillGraph;
	//ϳ�����
	double l_dLongChamfer = g_pDoc->m_stcClothCut.m_dChamfer;
	//����ߵĵ㼯�Ĵ�С
	int l_iSize = pEllipse->m_point.GetSize()-1; 
	//�����������������������ĸ����    ����      ������ϵ�������
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin,l_posEnd;
	//����ߵ�������������X��ĽǶȺ�͹��
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dAgl90 = MATH_PI/2;	
	//��������ֱ���¼�����ֱ��������
	//�µĵ㼯��͹��
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	//������һȦ
	for(int i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = pEllipse->m_point.GetAt(i);
		l_posPoly2 = pEllipse->m_point.GetAt(i+1);
		//����ӵ����һ�㣬�˳�ѭ�������������һ��
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//���һ��ֱ����ڶ���ֱ�ߵĽ���,���ƽ��
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//����ĩ���غ�
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
		//����������ĸ���
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
		if (i ==0)//�ӵ�һ��
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//�����ƽ�У����ؽ���
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
	//������һȦ
	for(i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = pEllipse->m_point.GetAt(i);
		l_posPoly2 = pEllipse->m_point.GetAt(i+1);
		//����ӵ����һ�㣬�˳�ѭ�������������һ��
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//���һ��ֱ����ڶ���ֱ�ߵĽ���,���ƽ��
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//����ĩ���غ�
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
		//����������ĸ���
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
		if (i ==0)//�ӵ�һ��
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//�����ƽ�У����ؽ���
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


