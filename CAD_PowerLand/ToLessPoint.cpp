// ToLessPoint.cpp: implementation of the CToLessPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "ToLessPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToLessPoint::CToLessPoint()
{
	m_dMaxRadius = g_pDoc->m_stcClothCut.dLessPointRadius;
	m_dPrecision = g_pDoc->m_stcClothCut.dSimplify;
	m_dMinDis = 0.0;
	m_dMaxDis = 0.0;
	m_pUndoList = NULL;
	m_aryPoint.RemoveAll();
	m_aryTuDu.RemoveAll();
}

CToLessPoint::~CToLessPoint()
{
	m_aryPoint.RemoveAll();
	m_aryTuDu.RemoveAll();
}

/*
 *	�Ѷ�����ٵ㣬˼��������֮����߶���һ���ߣ������е��ߵľ��룬Ҫ��С��һ����ֵ�������Ϳ���ȡ���ˡ�
 */
void CToLessPoint::ToLessPoint()
{
	CCreateBase* pSelEntity = NULL;	
	int l_isize = g_pDoc->m_CollectOfPicked.GetSize();
	//��ͼ���Ƿ��в�����
	BOOL l_bOpera = FALSE;
	for(int i = 0;i<l_isize;i++)
	{
		pSelEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(i);
		if (pSelEntity->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
		{
			l_bOpera = TRUE;
			m_pPolyline = (CCreatePolyline*)pSelEntity;
			//����ǰ�ȴ浽�㼯��
			AddUndoList(m_pPolyline->m_point,m_pPolyline->m_dTuDu,(LONG)m_pPolyline);		
			//ȥ����һ��ֱ���ϵĶ���ĵ�
			DelEqualSlope();
			//��С��ֱ����ϳ�Բ��
			LineToArc();
		}
	}
	if(l_bOpera == TRUE)
		g_pDoc->m_Undo->AddUndoList(m_pUndoList);	
	g_pView->Invalidate();
}

/*
 *	ȥ����һ��ֱ���ϵĶ���ĵ�
 */
void CToLessPoint::DelEqualSlope()
{
	Position pos1,pos2,pos3;
	double l_dzore = 0.0,l_dPotDis = 0.0;
	int l_iNum,i,j;
	//�㼯�Ķ���
	int l_iSizeOfPoint = m_pPolyline->m_point.GetSize();
	pos1 = m_pPolyline->m_point.GetAt(0);//���
	l_iNum = 0;
	m_aryPoint.Add(pos1);
	m_aryTuDu.Add(l_dzore);
	//�����µ�
	for(i=2;i<l_iSizeOfPoint;i++)
	{
		pos2 = m_pPolyline->m_point.GetAt(i);//ĩ��
		for(j = l_iNum+1;j<i;j++)
		{
			pos3 = m_pPolyline->m_point.GetAt(j);//�м��һЩ��					
			l_dPotDis = pos3.PotDisLine(pos1,pos2);
			if (l_dPotDis > DISTANCE_ZERO) //�м�㲻��ֱ����,Ҫ����
			{
				pos1 = m_pPolyline->m_point.GetAt(i-1);
				l_iNum = i-1;
				m_aryPoint.Add(pos1);
				m_aryTuDu.Add(l_dzore);
				break;
			}
			else //�м����ֱ����
			{//               ĩ��                   �е�						
				if((pos1.Distance(pos2)-pos1.Distance(pos3))<DISTANCE_ZERO)//����������ɨ��
				{
					pos1 = m_pPolyline->m_point.GetAt(i-1);
					l_iNum = i-1;
					m_aryPoint.Add(pos1);
					m_aryTuDu.Add(l_dzore);
					break;
				}
			}
		}
	}
	//�����һ��
	if (!PosEqual( pos1,m_pPolyline->m_point.GetAt(l_iSizeOfPoint-1)))
	{
		m_aryPoint.Add(m_pPolyline->m_point.GetAt(l_iSizeOfPoint-1));
		m_aryTuDu.Add(l_dzore);
	}
	//�ѵ㼯����
	UpdataPolyline();
}		

/*
 *	��С��ֱ����ϳ�Բ��
 */
void CToLessPoint::LineToArc()
{
	int i,j,l_iNumMid;
	double l_dTudu = 0.0;
	//�㼯�Ķ���
	int l_iSizeOfPoint = m_pPolyline->m_point.GetSize();
	//ÿ�λ�
	for(j=0;j<l_iSizeOfPoint;j++)
	{	
		m_posBegin = m_pPolyline->m_point.GetAt(j);
		if (j == l_iSizeOfPoint-2) 
		{
			m_posBegin = m_pPolyline->m_point.GetAt(j);
			m_aryPoint.Add(m_posBegin);
			m_aryTuDu.Add(l_dTudu);	
		}
		//ÿ����һ��������Բ�������龫��
		for(i=j;i<l_iSizeOfPoint-2;i++)
		{
			l_iNumMid = (i+2+j)/2;
			m_posMid = m_pPolyline->m_point.GetAt(l_iNumMid);
			m_posEnd = m_pPolyline->m_point.GetAt(i+2);
			//�����γɵ�Բ
			if(Pos3OutRound(m_posBegin,m_posMid,m_posEnd,m_posCenter,m_dRadius) == FALSE)
			{
				AddPoint(i,j);
				break;
			}			//�鿴Բ����ԭͼ�ľ���
			if(ClickPrecision(i,j) == FALSE)
				break;
			m_posPasCT = m_posCenter;
			m_posPasEN = m_posEnd;
			m_posPasMD = m_posMid;
			if (i == l_iSizeOfPoint-3)
			{
				AddPoint(i,j);
				break;
			}
		}
	}
	m_aryPoint.Add(m_pPolyline->m_point.GetAt(l_iSizeOfPoint-1));
	m_aryTuDu.Add(l_dTudu);
	//�ѵ㼯����
	UpdataPolyline();
}

/*
 *	�鿴Բ����ԭͼ�ľ���
 */
BOOL CToLessPoint::ClickPrecision(int& i,int& j)
{
	Position pos1,pos2,pos3,posFoot;
	//ԭͼ������Բ����������
	double l_dValMax,l_dRcdMax,l_dRcdMin;
	//���������С����ĵ㼯�е�λ��
	int l_iMaxPos,l_iMinPos;
	//�Ƿ����߶���Բ����Զ
	BOOL l_bLineFarest = FALSE;
	for(int k=j;k<i+2;k++)
	{		
		if (m_dRadius > m_dMaxRadius)
			return AddPoint(i,j);
		l_dRcdMin = m_dMinDis;
		l_dRcdMax = m_dMaxDis;
		pos1 = m_pPolyline->m_point.GetAt(k);
		pos2 = m_pPolyline->m_point.GetAt(k+1);
		posFoot = m_posCenter.DistancePosFoot(pos1,pos2);	//����
		//˵���������߶���,������������뻡��Զ�ĵ�
		if (fabs(posFoot.Distance(pos1)+posFoot.Distance(pos2) - pos2.Distance(pos1)) < DOUBLE_EQUAL)
		{
			//���߶���Բ����������
			l_dValMax = m_dRadius - posFoot.Distance(m_posCenter);
			m_dMaxDis = max(l_dValMax,m_dMaxDis);
			m_dMinDis = min(l_dValMax,m_dMinDis);
			//�����������С����ı�ʱҪ�������ĸ���			
			if(fabs(m_dMaxDis - l_dRcdMax) > DOUBLE_EQUALEST)
			{
				l_bLineFarest = TRUE;
				l_dRcdMax = m_dMaxDis;
				l_iMaxPos = k;
			}
		}
		////��һ��////////
		l_dValMax = m_dRadius - pos1.Distance(m_posCenter);
		if(l_dValMax > 0)
			m_dMaxDis = max(l_dValMax,m_dMaxDis);
		else
			m_dMinDis = min(l_dValMax,m_dMinDis);
		//�����������С����ı�ʱҪ�������ĸ���			
		if(fabs(m_dMaxDis - l_dRcdMax) > DOUBLE_EQUALEST)
		{
			l_bLineFarest = FALSE;	
			l_dRcdMax = m_dMaxDis;
			l_iMaxPos = k;
		}
		else if(fabs(m_dMinDis - l_dRcdMin) > DOUBLE_EQUALEST)
		{
			l_iMinPos = k;	
			l_dRcdMin = m_dMinDis;
		}
		////////////�ڶ���//////////////////////
		l_dValMax = m_dRadius - pos2.Distance(m_posCenter);
		if(l_dValMax > 0)
			m_dMaxDis = max(l_dValMax,m_dMaxDis);
		else
			m_dMinDis = min(l_dValMax,m_dMinDis);
		//�����������С����ı�ʱҪ�������ĸ���			
		if(fabs(m_dMaxDis - l_dRcdMax) > DOUBLE_EQUALEST)
		{
			l_bLineFarest = FALSE;	
			l_dRcdMax = m_dMaxDis;
			l_iMaxPos = k+1;
		}
		else if(fabs(m_dMinDis - l_dRcdMin) > DOUBLE_EQUALEST)
		{
			l_iMinPos = k+1;	
			l_dRcdMin = m_dMinDis;
		}
	}
	//�����С�����뾫�ȶԱ�
	return TurnArcClickPrecs(i,j,l_iMaxPos,l_iMinPos,l_bLineFarest);
}

/*
 *	�ı�Բ�����ٴβ鿴����
 */
BOOL CToLessPoint::TurnArcClickPrecs(int& i,int& j,int iMaxPos,int iMinPos,BOOL bLineFarest)
{
	double l_dLessPrecis = m_dPrecision -  m_dPrecision/10;
	Position posCenter1,posCenter2,pos1,pos2,posFoot;
	double Radius1,Radius2,angle;
	//�����������������
	if (m_dMaxDis >= m_dPrecision&&m_dMinDis <= -m_dPrecision)
	{	//��������ӵ�
		return AddPoint(i,j);
	}
	else if(m_dMaxDis >= m_dPrecision)//�����Բ����
	{
		pos1 = m_pPolyline->m_point.GetAt(iMaxPos);
		Pos2OnRdPosInRdOutRd(m_posEnd,m_posBegin,pos1,l_dLessPrecis,posCenter1,Radius1,posCenter2,Radius2);
		//�����һ��Բ���㾫��
		if (PosOnRound(i,j,posCenter1,Radius1)&&bLineFarest == FALSE)
		{
			m_posCenter = posCenter1;
			angle = GetAngleToXAxis(posCenter1,pos1);
			m_posMid.x = posCenter1.x + cos(angle)*Radius1;
			m_posMid.y = posCenter1.y + sin(angle)*Radius1;				
			m_dMaxDis = 0.0;
			m_dMinDis = 0.0;
			return TRUE;
		}
		else if (PosOnRound(i,j,posCenter2,Radius2)&&bLineFarest == FALSE) 
		{
			m_posCenter = posCenter2;
			angle = GetAngleToXAxis(posCenter2,pos1);
			m_posMid.x = posCenter2.x + cos(angle)*Radius2;
			m_posMid.y = posCenter2.y + sin(angle)*Radius2;				
			m_dMaxDis = 0.0;
			m_dMinDis = 0.0;
			return TRUE;
		}
		else if (bLineFarest == TRUE)//�߶���Բ���ľ�������
		{
			pos1 = m_pPolyline->m_point.GetAt(iMaxPos);
			pos2 = m_pPolyline->m_point.GetAt(iMaxPos+1);
			posFoot = m_posCenter.DistancePosFoot(pos1,pos2);
			angle = GetAngleToXAxis(m_posCenter,posFoot);
			pos1.x = pos1.x + l_dLessPrecis*cos(angle);
			pos1.y = pos1.y + l_dLessPrecis*sin(angle);
			pos2.x = pos2.x + l_dLessPrecis*cos(angle);
			pos2.y = pos2.y + l_dLessPrecis*sin(angle);
			if(Pos2LineOutRound(m_posBegin,m_posEnd,pos1,pos2,posCenter1,Radius1,posCenter2,Radius2))	
			{
				//�����һ��Բ���㾫��
				if (PosOnRound(i,j,posCenter1,Radius1))
				{
					m_posCenter = posCenter1;
					pos1 = (pos1+pos2)/2;
					angle = GetAngleToXAxis(posCenter1,pos1);
					m_posMid.x = posCenter1.x + cos(angle)*Radius1;
					m_posMid.y = posCenter1.y + sin(angle)*Radius1;				
					m_dMaxDis = 0.0;
					m_dMinDis = 0.0;
					return TRUE;
				}
				else if (PosOnRound(i,j,posCenter2,Radius2)) 
				{
					m_posCenter = posCenter2;
					pos1 = (pos1+pos2)/2;
					angle = GetAngleToXAxis(posCenter2,pos1);
					m_posMid.x = posCenter2.x + cos(angle)*Radius2;
					m_posMid.y = posCenter2.y + sin(angle)*Radius2;				
					m_dMaxDis = 0.0;
					m_dMinDis = 0.0;
					return TRUE;
				}
			}
			return AddPoint( i,j);			
		}
		else
			return AddPoint( i,j);

	}
	else if(m_dMinDis <= -m_dPrecision)
	{
		pos1 = m_pPolyline->m_point.GetAt(iMinPos);
		Pos2OnRdPosInRdOutRd(m_posBegin,m_posEnd,pos1,l_dLessPrecis,posCenter1,Radius1,posCenter2,Radius2);
		//�����һ��Բ���㾫��
		if (PosOnRound(i,j,posCenter1,Radius1)&&bLineFarest == FALSE)
		{
			m_posCenter = posCenter1;
			angle = GetAngleToXAxis(posCenter1,pos1);
			m_posMid.x = posCenter1.x + cos(angle)*Radius1;
			m_posMid.y = posCenter1.y + sin(angle)*Radius1;				
			m_dMaxDis = 0.0;
			m_dMinDis = 0.0;
			return TRUE;
		}
		else if (PosOnRound(i,j,posCenter2,Radius2)&&bLineFarest == FALSE) 
		{
			m_posCenter = posCenter2;
			angle = GetAngleToXAxis(posCenter2,pos1);
			m_posMid.x = posCenter2.x + cos(angle)*Radius2;
			m_posMid.y = posCenter2.y + sin(angle)*Radius2;				
			m_dMaxDis = 0.0;
			m_dMinDis = 0.0;
			return TRUE;
		}
		else
			return AddPoint( i,j);
	}
	return TRUE;
}

/*
 *	������ĵ��Ƿ���Բ����
 */
BOOL CToLessPoint::PosOnRound(int i,int j,Position posCenter,double dRadius)
{
	double l_dValMax,l_dValMax1;
	Position pos1,pos2,posFoot;
	for(int k=j;k<i+2;k++)
	{
		if (dRadius > m_dMaxRadius)
			return FALSE;
		pos1 = m_pPolyline->m_point.GetAt(k);
		pos2 = m_pPolyline->m_point.GetAt(k+1);
		posFoot = posCenter.DistancePosFoot(pos1,pos2);	//����
		//˵���������߶���,������������뻡��Զ�ĵ�
		if (fabs(posFoot.Distance(pos1)+posFoot.Distance(pos2) - pos2.Distance(pos1)) < DOUBLE_EQUAL)
		{
			//���߶���Բ����������
			l_dValMax = dRadius - posFoot.Distance(posCenter);
			//�����������
			if (fabs(l_dValMax) >= m_dPrecision)
				return FALSE;
		}
		l_dValMax = dRadius - pos1.Distance(posCenter);
		l_dValMax1 = dRadius - pos2.Distance(posCenter);
		//�����������
		if (fabs(l_dValMax) >= m_dPrecision||fabs(l_dValMax1) >= m_dPrecision)
			return FALSE;
	}
	return TRUE;
}

/*
 *	�ѵ㼯����
 */
void CToLessPoint::UpdataPolyline()
{
	m_pPolyline->m_point.Copy(m_aryPoint);
	m_pPolyline->m_dTuDu.Copy(m_aryTuDu);
	m_aryPoint.RemoveAll();
	m_aryTuDu.RemoveAll();
}

/*
 *	��������ӵ�
 */
BOOL CToLessPoint::AddPoint(int& i,int& j)
{
	double l_dTudu = 0.0;
	if (i == j)
	{
		m_aryPoint.Add(m_posBegin);
		m_aryTuDu.Add(l_dTudu);
		j = i;
		m_dMaxDis = 0.0;
		m_dMinDis = 0.0;
		return FALSE;
		
	}
	else
	{
		m_aryPoint.Add(m_posBegin);
		l_dTudu = g_pDoc->RtnTudu(m_posBegin,m_posPasEN,m_posPasCT,
			m_posPasEN.AnalysisClockDir(m_posBegin,m_posPasMD));
		m_aryTuDu.Add(l_dTudu);
	}
	j = i;
	m_dMaxDis = 0.0;
	m_dMinDis = 0.0;
	return FALSE;	
}

/*
 *	����Բ�������Բ��һ�㵽Բ�ϵľ�����Բ
 */
BOOL CToLessPoint::Pos2OnRdPosInRdOutRd(Position posRound1,Position posRound2,Position posInRound,double dDisToRound,Position& posCenter1,double& Radius1,Position& posCenter2,double& Radius2)
{
	//��Բ�����ڵ�ֱ�ߵĲ���
	double dAngle = GetAngleToXAxis(posRound1,posRound2)+MATH_PI/2;
	Position posMid = (posRound1+posRound2)/2;//�ҵ��е�
	double k = tan(dAngle);
	double b = posMid.y - k*posMid.x;
	double e = 2*k*posInRound.y + 2*posInRound.x -2*k*posRound1.y - 2*posRound1.x;
	double f = posRound1.y*posRound1.y +posRound1.x*posRound1.x + 2*b*posInRound.y - 2*b*posRound1.y - posInRound.y*posInRound.y -posInRound.x*posInRound.x -dDisToRound*dDisToRound;
	double AA = 4*dDisToRound*dDisToRound*(1+k*k) - e*e;
	double BB = 4*dDisToRound*dDisToRound*(2*k*(b-posInRound.y) - 2*posInRound.x) - 2*e*f;
	double CC = 4*dDisToRound*dDisToRound*((b-posInRound.y)*(b-posInRound.y)+posInRound.x*posInRound.x) - f*f;
	SolveEquation(AA,BB,CC,posCenter1.x,posCenter2.x);
	posCenter1.y = k*posCenter1.x + b;
	posCenter2.y = k*posCenter2.x + b;
	Radius1 = posCenter1.Distance(posRound1);
	Radius2 = posCenter2.Distance(posRound1);
	return TRUE;
}

/*
 *	������������Ӳ���
 */
void CToLessPoint::AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,LONG CreateBase)
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













