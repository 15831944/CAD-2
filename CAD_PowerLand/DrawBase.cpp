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
  *	ִ�гɹ����ƺ���
  */
void CDrawBase::SuccessWord(CString str)
{
	//ִ�гɹ���Ҫ��ָ�����
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	g_pView->Invalidate();
	RemindContent(str);
}

/*
 *	���Ҷ˵�
 */
BOOL CDrawBase::FindPointEnd(Position pos)
{
	//�����˵�
	Position l_cBegin,l_cEnd;
	//�������ĵ�ĸ���
	int l_iCountOfPoint;
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase = NULL;
	while (POS != NULL)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		//��//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePoint)))
		{
			CCreatePoint *pPoint = (CCreatePoint*)l_pCreateBase;
			//���Ƿ���ֱ�ߵ�����
			if(pPoint->m_Begin.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
			{
				m_posWillEnd = pPoint->m_Begin;
				return TRUE;
			}
		}
		//ֱ��//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			//���Ƿ���ֱ�ߵ�����
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
		//���߶�//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				l_cEnd = Polyline->m_point.GetAt(j+1);
				//���Ƿ���ֱ�ߵ�����
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
		//��������//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreateSpline *Polyline = (CCreateSpline*)l_pCreateBase;
			
			l_iCountOfPoint = Polyline->m_aryControlPoints.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_aryControlPoints.GetAt(j);
				l_cEnd = Polyline->m_aryControlPoints.GetAt(j+1);
				//���Ƿ���ֱ�ߵ�����
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
		
		//����//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Polyline = (CCreateRectangle*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				l_cEnd = Polyline->m_point.GetAt(j+1);
				//���Ƿ���ֱ�ߵ�����
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
		{//Բ

		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{//Բ��
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			//���Ƿ���ֱ�ߵ�����
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
		{//��Բ
			CCreateEllipse *Polyline = (CCreateEllipse*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				l_cEnd = Polyline->m_point.GetAt(j+1);
				//���Ƿ���ֱ�ߵ�����
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
 *	���������
 */
BOOL CDrawBase::FindPointNear(Position pos)
{
	//�����˵�
	Position l_posBegin,l_posEnd,l_posCenter;
	//��Ļ�ϵĵ�
	CPoint pot;
	//�������ĵ���
	int l_iNumClick;
	//��������͹��
	double l_dTudu=0.0,l_dAgl,l_dRadius;
	BOX2D pBox;
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase = NULL;
	while (POS != NULL)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		//ֱ��//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			line->GetBox(&pBox,TRUE);
			if (IsBelongBox(pos,pBox))
			{
				//�㵽�ߵľ��� 
				if(pos.PotDisLine(line->m_Begin,line->m_End)/g_pView->ReturnProportion()<= PRECISION)
				{
					m_posWillNear = pos.DistancePosFoot(line->m_Begin,line->m_End);
					return TRUE;
				}
			}
		}		
		//���߶�//////////////////////////////////////////
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
					if (l_dTudu == 0.0)//ֱ��
					{
						m_posWillNear = pos.DistancePosFoot(l_posBegin,l_posEnd);
					}
					else//Բ��
					{	
						g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//��Բ��
						l_dRadius = l_posBegin.Distance(l_posCenter);//��뾶
						l_dAgl = GetAngleToXAxis(l_posCenter,pos);
						m_posWillNear.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
						m_posWillNear.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
					}
					return TRUE;
				}
			}
		}
		
		//����//////////////////////////////////////////
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
					if (l_dTudu == 0.0)//ֱ��
					{
						m_posWillNear = pos.DistancePosFoot(l_posBegin,l_posEnd);
					}
					else//Բ��
					{	
						g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//��Բ��
						l_dRadius = l_posBegin.Distance(l_posCenter);//��뾶
						l_dAgl = GetAngleToXAxis(l_posCenter,pos);
						m_posWillNear.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
						m_posWillNear.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
					}
					return TRUE;
				}
			}			
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//Բ
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
		{//Բ��
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
			//���Ƿ���ֱ�ߵ�����
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//��Բ
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
					if (l_dTudu == 0.0)//ֱ��
					{
						m_posWillNear = pos.DistancePosFoot(l_posBegin,l_posEnd);
					}
					else//Բ��
					{	
						g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//��Բ��
						l_dRadius = l_posBegin.Distance(l_posCenter);//��뾶
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

//�������
void CDrawBase::DrawNearPoint(CDC* pDC,CPoint pot)
{
	pDC->MoveTo(pot.x-5,pot.y-5);
	pDC->LineTo(pot.x+5,pot.y-5);
	pDC->LineTo(pot.x-5,pot.y+5);
	pDC->LineTo(pot.x+5,pot.y+5);
	pDC->LineTo(pot.x-5,pot.y-5);
}


/*
 *	���Ҵ����
 */
BOOL CDrawBase::FindPointFoot(Position posBase,Position posMove)
{
	//�����˵�
	Position l_posBegin,l_posEnd,l_posCenter,l_posFoot;
	//��Ļ�ϵĵ�
	CPoint pot;
	//�������ĵ���
	int l_iNumClick;
	//��������͹��
	double l_dTudu=0.0,l_dAgl,l_dAgl1,l_dAgl2,l_dRadius;
	BOX2D pBox;
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase = NULL;
	while (POS != NULL)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		//ֱ��//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			line->GetBox(&pBox,TRUE);
			if (IsBelongBox(posMove,pBox))
			{
				l_posBegin = posBase.DistancePosFoot(line->m_Begin,line->m_End);
				//�㵽��ľ��� 
				if(posMove.Distance(l_posBegin)/g_pView->ReturnProportion()<= PRECISION)
				{
					m_posWillFoot = l_posBegin;
					return TRUE;
				}
			}
		}		
		//���߶�//////////////////////////////////////////
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
					if (l_dTudu == 0.0)//ֱ��
					{
						l_posFoot = posBase.DistancePosFoot(l_posBegin,l_posEnd);
						//�㵽��ľ��� 
						if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
						{
							m_posWillFoot = l_posFoot;
							return TRUE;
						}
					}
					else//Բ��
					{	
						g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//��Բ��
						l_dAgl = GetAngleToXAxis(posBase,l_posCenter);
						l_dAgl1 = GetAngleToXAxis(l_posCenter,l_posBegin);
						l_dAgl2 = GetAngleToXAxis(l_posCenter,l_posEnd);						
						if (l_dTudu<0.0)//�����˳ʱ��
						{
							if (l_dAgl1<l_dAgl2)//�������ʱ������X��������
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
						else//��ʱ��
						{
							if (l_dAgl1>l_dAgl2)//�������ʱ������X��������
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
						l_dRadius = l_posBegin.Distance(l_posCenter);//��뾶
						l_posFoot.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
						l_posFoot.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
						//�㵽��ľ��� 
						if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
						{
							m_posWillFoot = l_posFoot;
							return TRUE;
						}
					}
				}
			}
		}
		
		//����//////////////////////////////////////////
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
					if (l_dTudu == 0.0)//ֱ��
					{
						l_posFoot = posBase.DistancePosFoot(l_posBegin,l_posEnd);
						//�㵽��ľ��� 
						if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
						{
							m_posWillFoot = l_posFoot;
							return TRUE;
						}
					}
					else//Բ��
					{	
						g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//��Բ��
						l_dAgl = GetAngleToXAxis(posBase,l_posCenter);
						l_dAgl1 = GetAngleToXAxis(l_posCenter,l_posBegin);
						l_dAgl2 = GetAngleToXAxis(l_posCenter,l_posEnd);	
						if (l_dTudu<0.0)//�����˳ʱ��
						{
							if (l_dAgl1<l_dAgl2)//�������ʱ������X��������
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
						else//��ʱ��
						{
							if (l_dAgl1>l_dAgl2)//�������ʱ������X��������
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
						l_dRadius = l_posBegin.Distance(l_posCenter);//��뾶
						l_posFoot.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
						l_posFoot.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
						//�㵽��ľ��� 
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
		{//Բ
			CCreateRound *Arc = (CCreateRound*)l_pCreateBase;
			g_pView->WorldToScreem(posMove,pot);
			l_iNumClick = Arc->IsPick(pot);
			if (l_iNumClick != -1)
			{			
				l_posCenter = Arc->m_Center;
				l_dAgl = GetAngleToXAxis(posBase,l_posCenter);
				l_dAgl1 = GetAngleToXAxis(l_posCenter,posBase);	
				l_dRadius = Arc->m_dRadius;//��뾶
				//һ������Բ���������㣬
				l_posFoot.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
				l_posFoot.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
				l_posEnd.x = l_posCenter.x + l_dRadius*cos(l_dAgl1);
				l_posEnd.y = l_posCenter.y + l_dRadius*sin(l_dAgl1);
				//�㵽��ľ��� �������ĸ������
				if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
				{
					m_posWillFoot = l_posFoot;//Բ�ĵ�һ������
					return TRUE;
				}
				else if (posMove.Distance(l_posEnd)/g_pView->ReturnProportion()<= PRECISION)
				{
					m_posWillFoot = l_posEnd;//Բ�ĵڶ�������
					return TRUE;
				}
			}
		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{//Բ��
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
				if (Arc->m_iDirect == AD_CLOCKWISE)//�����˳ʱ��
				{
					if (l_dAgl1<l_dAgl2)//�������ʱ������X��������
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
				else//��ʱ��
				{
					if (l_dAgl1>l_dAgl2)//�������ʱ������X��������
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
				l_dRadius = l_posBegin.Distance(l_posCenter);//��뾶
				l_posFoot.x = l_posCenter.x + l_dRadius*cos(l_dAgl);
				l_posFoot.y = l_posCenter.y + l_dRadius*sin(l_dAgl);
				//�㵽��ľ��� 
				if(posMove.Distance(l_posFoot)/g_pView->ReturnProportion()<= PRECISION)
				{
					m_posWillFoot = l_posFoot;
					return TRUE;
				}
			}
			//���Ƿ���ֱ�ߵ�����
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//��Բ
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
					if (l_dTudu == 0.0)//ֱ��
					{
						l_posFoot = posBase.DistancePosFoot(l_posBegin,l_posEnd);
						//�㵽��ľ��� 
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

//�������
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
 *	�����е�
 */
BOOL CDrawBase::FindPointMid(Position pos)
{
	//�����˵�
	Position l_cBegin,l_cEnd,l_cMid,l_cCenter;
	double l_dAngle,l_dAngle1,l_dAngle2,l_dTudu,l_dRadius;
	//�������ĵ�ĸ���
	int l_iCountOfPoint;
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase = NULL;
	while (POS != NULL)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		//ֱ��//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			l_cMid = (line->m_Begin + line->m_End)/2;
			//���Ƿ���ֱ�ߵ��е�
			if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
			{
				m_posWillMid = l_cMid;
				return TRUE;
			}
		}		
		//���߶�//////////////////////////////////////////
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
				//���Ƿ����е���
				if (l_dTudu == 0)
				{
					if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
					{
						m_posWillMid = l_cMid;
						return TRUE;
					}				
				}
				else if (l_dTudu < 0)//˳ʱ��
				{
					g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_cCenter);
					l_dAngle1 = GetAngleToXAxis(l_cCenter,l_cBegin);
					l_dAngle2 = GetAngleToXAxis(l_cCenter,l_cEnd);
					if (l_dAngle1<l_dAngle2)//�������ʱ������X��������
						l_dAngle = l_dAngle1 + MATH_2PI - l_dAngle2;
					else
						l_dAngle = l_dAngle1 - l_dAngle2;
						
				}
				else//��ʱ��
				{
					g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_cCenter);
					l_dAngle1 = GetAngleToXAxis(l_cCenter,l_cBegin);
					l_dAngle2 = GetAngleToXAxis(l_cCenter,l_cEnd);	
					if (l_dAngle2<l_dAngle1)//�������ʱ������X��������
						l_dAngle = l_dAngle2 + MATH_2PI - l_dAngle1;
					else
						l_dAngle = l_dAngle2 - l_dAngle1;
				}	
				//���е�
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
				//���Ƿ���Բ�����е�
				if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillMid = l_cMid;
					return TRUE;
				}
			}
		}
		
		//����//////////////////////////////////////////
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
				//���Ƿ����е���
				if (l_dTudu == 0)
				{
					if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
					{
						m_posWillMid = l_cMid;
						return TRUE;
					}				
				}
				else if (l_dTudu < 0)//˳ʱ��
				{
					g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_cCenter);
					l_dAngle1 = GetAngleToXAxis(l_cCenter,l_cBegin);
					l_dAngle2 = GetAngleToXAxis(l_cCenter,l_cEnd);
					if (l_dAngle1<l_dAngle2)//�������ʱ������X��������
						l_dAngle = l_dAngle1 + MATH_2PI - l_dAngle2;
					else
						l_dAngle = l_dAngle1 - l_dAngle2;
					
				}
				else//��ʱ��
				{
					g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_cCenter);
					l_dAngle1 = GetAngleToXAxis(l_cCenter,l_cBegin);
					l_dAngle2 = GetAngleToXAxis(l_cCenter,l_cEnd);	
					if (l_dAngle2<l_dAngle1)//�������ʱ������X��������
						l_dAngle = l_dAngle2 + MATH_2PI - l_dAngle1;
					else
						l_dAngle = l_dAngle2 - l_dAngle1;
				}	
				//���е�
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
				//���Ƿ���Բ�����е�
				if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
				{
					m_posWillMid = l_cMid;
					return TRUE;
				}				
			}			
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//Բ

		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{//Բ��
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			//���Ƿ���ֱ�ߵ�����
			l_cMid = (Arc->m_Begin+Arc->m_End)/2;
			l_dAngle1 = GetAngleToXAxis(Arc->m_Center,Arc->m_Begin);
			l_dAngle2 = GetAngleToXAxis(Arc->m_Center,Arc->m_End);
			if (Arc->m_iDirect == AD_CLOCKWISE)
			{
				if (l_dAngle1<l_dAngle2)//�������ʱ������X��������
					l_dAngle = l_dAngle1 + MATH_2PI - l_dAngle2;
				else
					l_dAngle = l_dAngle1 - l_dAngle2;
			}
			else
			{
				if (l_dAngle2<l_dAngle1)//�������ʱ������X��������
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
			//���Ƿ���Բ�����е�
			if(l_cMid.Distance(pos)/g_pView->ReturnProportion()<=PRECISION)
			{
				m_posWillMid = l_cMid;
				return TRUE;
			}
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//��Բ
			CCreateEllipse *Polyline = (CCreateEllipse*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint-1;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				l_cEnd = Polyline->m_point.GetAt(j+1);
				l_cMid = (l_cBegin+l_cEnd)/2;
				//���Ƿ���ֱ�ߵ�����
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
 *	���е�
 */
void CDrawBase::DrawMidPoint(CDC* pDC,CPoint pot)
{
	pDC->MoveTo(pot.x,pot.y-6);
	pDC->LineTo(pot.x-5,pot.y+3);
	pDC->LineTo(pot.x+5,pot.y+3);
	pDC->LineTo(pot.x,pot.y-6);
}

/*
 *	�趨�����µĵ�
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
 *	����׽
 */
BOOL CDrawBase::FindPointAll(CDC* pDC,Position posBase,Position pos,int iCatch /* = 3 */)
{
	//ת����Ļ����
	CPoint l_potRound;
	//������
	CPen	pen; 	
	pen.CreatePen(PS_SOLID,1,RGB(0,127,255));
	CPen* pOldPen = pDC->SelectObject(&pen); // �õ�ԭ���Ļ���
	pDC->SelectStockObject(NULL_BRUSH) ;	//��Բ�м�͸��
	pDC->SetROP2(R2_XORPEN);
	/////////////////////////////////�Ҷ˵�
	if ((iCatch&END_POINT) == END_POINT)
	{
		BOOL l_bIsInEnd = FindPointEnd(pos);	
		if(!l_bIsInEnd)	//���û���ڶ˵��ϣ���Ҫ����
		{	
			if(m_bIsClickEnd == TRUE)
			{
				g_pView->WorldToScreem(m_posPassEnd,l_potRound);
				CRect rect(l_potRound.x-4,l_potRound.y-4,l_potRound.x+4,l_potRound.y+4);
				pDC->Rectangle(rect);
				m_bIsClickEnd = FALSE;
			}
		}
		else//�����ڶ˵��Ͼ�Ҫ����Բ
		{
			if (m_bIsClickEnd == TRUE)
			{
				g_pView->WorldToScreem(m_posPassEnd,l_potRound);			
				CRect rect(l_potRound.x-4,l_potRound.y-4,l_potRound.x+4,l_potRound.y+4);
				pDC->Rectangle(rect);
			}
			//��֮ǰ������㻭û��Ҫ������������Ͷ˵�
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
			// �ָ�ԭ���Ļ��� 
			pDC->SelectObject(pOldPen); 
			pen.DeleteObject(); 	
			return END_POINT;
		}
	}
	/////////////////////////////////�Ҵ����
	if ((iCatch&FOOT_POINT) == FOOT_POINT&&m_iNumOfClick >0)
	{
		BOOL l_bIsInFoot = FindPointFoot(posBase,pos);	
		if(!l_bIsInFoot)	//���û���ڴ���㣬��Ҫ����
		{	
			if(m_bIsClickFoot == TRUE)
			{
				g_pView->WorldToScreem(m_posPassFoot,l_potRound);
				DrawFootPoint(pDC,l_potRound);
				m_bIsClickFoot = FALSE;
			}
		}
		else//�����ڴ�����Ͼ�Ҫ��
		{
			if(m_bIsClickFoot == TRUE)
			{
				g_pView->WorldToScreem(m_posPassFoot,l_potRound);
				DrawFootPoint(pDC,l_potRound);
			}
			//��֮ǰ������㻭û��Ҫ������������Ͷ˵�
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
			// �ָ�ԭ���Ļ��� 
			pDC->SelectObject(pOldPen); 
			pen.DeleteObject(); 	
			return FOOT_POINT;
		}
	}
	/////////////////////////////////���е�
	if ((iCatch&MID_POINT) == MID_POINT)
	{
		BOOL l_bIsInMid = FindPointMid(pos);
		if(!l_bIsInMid)	//���û�����е㣬��Ҫ����
		{	
			if(m_bIsClickMid == TRUE)
			{
				g_pView->WorldToScreem(m_posPassMid,l_potRound);
				DrawMidPoint(pDC,l_potRound);
				m_bIsClickMid = FALSE;
			}
		}
		else//�������е��Ͼ�Ҫ��
		{
			if(m_bIsClickMid == TRUE)
			{
				g_pView->WorldToScreem(m_posPassMid,l_potRound);
				DrawMidPoint(pDC,l_potRound);
			}
			//��֮ǰ������㻭û��Ҫ������������Ͷ˵�
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
			// �ָ�ԭ���Ļ��� 
			pDC->SelectObject(pOldPen); 
			pen.DeleteObject(); 	
			return MID_POINT;
		}
	}
	//////////////////////////////////////////�������
	if (NEAR_POINT == (iCatch&NEAR_POINT))
	{
		BOOL l_bIsInNear = FindPointNear(pos);
		if(!l_bIsInNear)	//���û����������ϣ���Ҫ����
		{	
			if(m_bIsClickNear == TRUE)
			{
				g_pView->WorldToScreem(m_posPassNear,l_potRound);
				DrawNearPoint(pDC,l_potRound);
				m_bIsClickNear = FALSE;
			}
		}
		else//�����ڶ˵��Ͼ�Ҫ����Բ
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
			// �ָ�ԭ���Ļ��� 
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
 *	����������
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
 *	������Ч������
 */
int CDrawBase::InvalidContent()
{
				//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n������Ч�����ݣ�",2);
	else
		RemindContent("\r\nThe content of the input is invalid��",2);			
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	return 0;
}