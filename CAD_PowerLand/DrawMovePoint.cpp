// DrawMovePoint.cpp: implementation of the CDrawMovePoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "Draw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawMovePoint::CDrawMovePoint()
{
	m_pUndoList = NULL;
	m_pGraphPos = NULL;
	m_pCurGraphPos = NULL;
}

CDrawMovePoint::~CDrawMovePoint()
{
	ClearList();
}


int CDrawMovePoint::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));//�ı������״	
	g_refresh = 0;
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if(m_iNumOfClick == 0)//ѡ��
	{
		m_aryMovePoint.RemoveAll();
		ClearList();
		if(FindPointEnd(l_cPos))//ȷ������ͼ�κ�����ĵ�
		{
			m_posFirst = l_cPos;
			m_iNumOfClick++;
			CopyList();
		}
		return 0;
	}
	if(m_iNumOfClick == 1)
	{
		DoMovePoint(m_aryMovePoint,l_cPos,TRUE);
		m_iNumOfClick = 0;
		g_pView->Invalidate();
	}
	return 0;
}

int CDrawMovePoint::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));//�ı������״	
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	curPos = pos;
	if((m_iNumOfClick == 0))	//����׽
		FindPointAll(pDC,m_middle,pos,END_POINT);	
	else
		FindPointAll(pDC,m_posFirst,pos,g_pView->m_iSwitch);
	if (m_iNumOfClick == 1)
	{	
		//ͼ�δ�С
		int l_iSize = m_aryDrawMouseMove.GetSize();		
		stcMovePoint l_stcMovePoint;
		// ���������ڵ�ǰһ��λ��
		prePos = m_posPre; 
		// ������ʱ���������һ����Ƥ��
		CCreateBase*	pTempLine1 = new CCreateLine(m_posFirst, prePos);
		if(g_refresh != 1)	//����Ϊ�˵�һ��û�в���ʱ������
		{
			pTempLine1->DrawMove(pDC,dmDrag);
			DoMovePoint(m_aryDrawMouseMove,prePos,FALSE);
			for(int i=0;i<l_iSize;i++)
			{
				l_stcMovePoint = m_aryDrawMouseMove.GetAt(i);
				l_stcMovePoint.pCreateBase->DrawMove(pDC,dmDrag);
			}
		}
		delete pTempLine1;
		//�趨��׽���ĵ�
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);	
		// ������ʱ���󣬸��ݵ�ǰλ�û���һ����Ƥ��
		CCreateBase*	pTempLine2 = new CCreateLine(m_posFirst, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		DoMovePoint(m_aryDrawMouseMove,curPos,FALSE);
		for(int i=0;i<l_iSize;i++)
		{
			l_stcMovePoint = m_aryDrawMouseMove.GetAt(i);
			l_stcMovePoint.pCreateBase->DrawMove(pDC,dmDrag);
		}
		
		// �ͷ��豸����ָ��		
		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_posPre = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	
	return 0;
}



int CDrawMovePoint::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//������ͼԪ//��Ҫ����ͼԪɾ��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	return 0;
}

/*
 *	����һ������ƶ�ʱҪ��������
 */
void CDrawMovePoint::CopyList()
{
	int l_iSize = m_aryMovePoint.GetSize();
	stcMovePoint l_stcMovePoint,l_stcDrawMovePoint;
	for(int i=0;i<l_iSize;i++)
	{
		l_stcMovePoint = m_aryMovePoint.GetAt(i);
		CCreateBase* pCopy  = l_stcMovePoint.pCreateBase->Copy();
		l_stcDrawMovePoint = l_stcMovePoint;
		l_stcDrawMovePoint.pCreateBase = pCopy;
		m_aryDrawMouseMove.Add(l_stcDrawMovePoint);
	}				
}

/*
 *	�ͷ�m_aryDrawMouseMove
 */
void CDrawMovePoint::ClearList()
{
	int l_iSize = m_aryDrawMouseMove.GetSize();
	stcMovePoint l_stcDrawMovePoint;
	for(int i=0;i<l_iSize;i++)
	{
		l_stcDrawMovePoint = m_aryDrawMouseMove.GetAt(i);
		delete l_stcDrawMovePoint.pCreateBase;
		l_stcDrawMovePoint.pCreateBase = NULL;
	}				
	m_aryDrawMouseMove.RemoveAll();
}

/*
 *	���Ҷ˵�
 */
BOOL CDrawMovePoint::FindPointEnd(Position pos)
{
	//�����˵�
	Position l_cBegin,l_cEnd;
	//�������ĵ�ĸ���
	int l_iCountOfPoint;
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase;
	while (POS != NULL)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		//��//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePoint)))
		{
			CCreatePoint *pPoint = (CCreatePoint*)l_pCreateBase;
			//���Ƿ���ֱ�ߵ�����
			if(pPoint->m_Begin == pos)
			{
				stcMovePoint l_stcMovePoint;
				l_stcMovePoint.iStyleGraph = MY_POINT;
				l_stcMovePoint.pCreateBase = l_pCreateBase;
				m_aryMovePoint.Add(l_stcMovePoint);
			}
		}
		//ֱ��//////////////////////////////////////////
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			//���Ƿ���ֱ�ߵ�����
			if(line->m_Begin == pos)
			{
				stcMovePoint l_stcMovePoint;
				l_stcMovePoint.iStyleGraph = MY_LINE;
				l_stcMovePoint.bIsBeginPos =TRUE;
				l_stcMovePoint.pCreateBase = l_pCreateBase;
				m_aryMovePoint.Add(l_stcMovePoint);
			}
			if(line->m_End == pos)
			{
				stcMovePoint l_stcMovePoint;
				l_stcMovePoint.iStyleGraph = MY_LINE;
				l_stcMovePoint.bIsBeginPos = FALSE;
				l_stcMovePoint.pCreateBase = l_pCreateBase;
				m_aryMovePoint.Add(l_stcMovePoint);
			}
		}		
		//���߶�//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				//���Ƿ���ֱ�ߵ�����
				if(l_cBegin == pos)
				{
					stcMovePoint l_stcMovePoint;
					l_stcMovePoint.iStyleGraph = MY_POLYLINE;
					l_stcMovePoint.iNumPolyMovePoint = j;
					l_stcMovePoint.pCreateBase = l_pCreateBase;
					m_aryMovePoint.Add(l_stcMovePoint);
					break;
				}
			}
		}
		//��������//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreateSpline *pSpline = (CCreateSpline*)l_pCreateBase;
			
			l_iCountOfPoint = pSpline->m_aryControlPoints.GetSize();
			for(int j=0;j<l_iCountOfPoint;j++)
			{
				l_cBegin = pSpline->m_aryControlPoints.GetAt(j);
				//���Ƿ���ֱ�ߵ�����
				if(l_cBegin == pos)
				{
					stcMovePoint l_stcMovePoint;
					l_stcMovePoint.iStyleGraph = MY_SPLINE;
					l_stcMovePoint.iNumPolyMovePoint = j;
					l_stcMovePoint.pCreateBase = l_pCreateBase;
					m_aryMovePoint.Add(l_stcMovePoint);
					break;
				}			
			}
		}
		
		//����//////////////////////////////////////////
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Polyline = (CCreateRectangle*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				//���Ƿ���ֱ�ߵ�����
				if(l_cBegin == pos)
				{
					stcMovePoint l_stcMovePoint;
					l_stcMovePoint.iStyleGraph = MY_RECTANGLE;
					l_stcMovePoint.iNumPolyMovePoint = j;
					l_stcMovePoint.pCreateBase = l_pCreateBase;
					m_aryMovePoint.Add(l_stcMovePoint);
					break;
				}
			}			
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{//Բ

		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{//Բ��
			
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{//��Բ
			CCreateEllipse *Polyline = (CCreateEllipse*)l_pCreateBase;
			l_iCountOfPoint = Polyline->m_point.GetSize();
			for(int j=0;j<l_iCountOfPoint;j++)
			{
				l_cBegin = Polyline->m_point.GetAt(j);
				//���Ƿ���ֱ�ߵ�����
				if(l_cBegin == pos)
				{
					stcMovePoint l_stcMovePoint;
					l_stcMovePoint.iStyleGraph = MY_ELLIPSE;
					l_stcMovePoint.iNumPolyMovePoint = j;
					l_stcMovePoint.pCreateBase = l_pCreateBase;
					m_aryMovePoint.Add(l_stcMovePoint);
					break;
				}
			}
		}
	}
	if(m_aryMovePoint.GetSize() == 0)
		return FALSE;
	return TRUE;
}

/*
 *	ִ�е��ƶ�
 */
void CDrawMovePoint::DoMovePoint(CArray<stcMovePoint,stcMovePoint&>& aryMovePoint,Position pos,BOOL bAddUndo /*= TRUE*/)
{
	//���ƶ���ͼ�εĴ�С
	int l_iSize = aryMovePoint.GetSize();
	if(l_iSize == 0)
		return;
	//ÿ��ͼ�ε����ݽṹ
	stcMovePoint l_stcMovePoint;
	for(int i=0;i<l_iSize;i++)
	{
		l_stcMovePoint = aryMovePoint.GetAt(i);
		if(bAddUndo == TRUE)//����ƶ�ʱ��������ڴ�
			AddUndoList(l_stcMovePoint.pCreateBase);			
		if(l_stcMovePoint.iStyleGraph == MY_POINT)//����ǵ����͵�
		{
			CCreatePoint* pPoint = (CCreatePoint*)l_stcMovePoint.pCreateBase;
			pPoint->m_Begin = pos;
		}
		else if(l_stcMovePoint.iStyleGraph == MY_LINE)//�����ֱ�����͵�
		{
			CCreateLine* pLine = (CCreateLine*)l_stcMovePoint.pCreateBase;
			if(l_stcMovePoint.bIsBeginPos == TRUE)
				pLine->m_Begin = pos;
			else
				pLine->m_End = pos;
		}
		else if(l_stcMovePoint.iStyleGraph == MY_POLYLINE)//����Ƕ�������͵�
		{
			CCreatePolyline* pPolyline = (CCreatePolyline*)l_stcMovePoint.pCreateBase;
			//����㵽������ʱ������ĩ��ӵ�ͼ��
			if(l_stcMovePoint.iNumPolyMovePoint == 0)
			{
				Position l_posFirst = pPolyline->m_point.GetAt(0);
				Position l_posLast = pPolyline->m_point.GetAt(pPolyline->m_point.GetSize()-1);
				if(l_posFirst == l_posLast)
					pPolyline->m_point.SetAt(pPolyline->m_point.GetSize()-1,pos);				
			}
			pPolyline->m_point.SetAt(l_stcMovePoint.iNumPolyMovePoint,pos);
		}
		else if(l_stcMovePoint.iStyleGraph == MY_RECTANGLE)//����Ǿ������͵�
		{
			CCreateRectangle* pPolyline = (CCreateRectangle*)l_stcMovePoint.pCreateBase;
			pPolyline->m_point.SetAt(l_stcMovePoint.iNumPolyMovePoint,pos);
			if(l_stcMovePoint.iNumPolyMovePoint == 0)
				pPolyline->m_point.SetAt(4,pos);
			
		}
		else if(l_stcMovePoint.iStyleGraph == MY_ELLIPSE)//�������Բ���͵�
		{
			CCreateEllipse* pPolyline = (CCreateEllipse*)l_stcMovePoint.pCreateBase;
			pPolyline->m_point.SetAt(l_stcMovePoint.iNumPolyMovePoint,pos);
			if(l_stcMovePoint.iNumPolyMovePoint == 0)
				pPolyline->m_point.SetAt(pPolyline->m_point.GetSize()-1,pos);
			
		}
		else if(l_stcMovePoint.iStyleGraph == MY_SPLINE)//����������������͵�
		{
			CCreateSpline* pSpline = (CCreateSpline*)l_stcMovePoint.pCreateBase;
			//����㵽������ʱ������ĩ��ӵ�ͼ��
			if(l_stcMovePoint.iNumPolyMovePoint == 0)
			{
				Position l_posFirst = pSpline->m_aryControlPoints.GetAt(0);
				Position l_posLast = pSpline->m_aryControlPoints.GetAt(pSpline->m_aryControlPoints.GetSize()-1);
				if(l_posFirst == l_posLast)
					pSpline->m_aryControlPoints.SetAt(pSpline->m_aryControlPoints.GetSize()-1,pos);				
			}
			pSpline->m_aryControlPoints.SetAt(l_stcMovePoint.iNumPolyMovePoint,pos);
			pSpline->RefreshSpline();
		}
	
	}
	if(bAddUndo == TRUE)		
		g_pDoc->m_Undo->AddUndoList(m_pUndoList);	
	m_pUndoList = NULL;
	m_pGraphPos = NULL;
	m_pCurGraphPos = NULL;
}

/*
 *	������������Ӳ���
 */
void CDrawMovePoint::AddUndoList(CCreateBase* pCreateBase)
{
	PGRAPHPOS pGraphPos = NULL;
	if(m_pUndoList == NULL)//�����һ�δ�
	{
		m_pUndoList = m_UndoRedo.NewUndoList();
		m_pUndoList->OperatieWhat = ID_MOVE_POINT;
		m_pCurGraphPos = m_UndoRedo.NewGraphPos();
		pGraphPos = m_pCurGraphPos;
		m_pCurGraphPos->pCreateBasepos = (LONG)pCreateBase;
		m_pUndoList->pGraphPos = m_pCurGraphPos;		
	}
	else//���ǵ�һ�δ������
	{
		m_pGraphPos = m_UndoRedo.NewGraphPos();						
		pGraphPos = m_pGraphPos;
		m_pGraphPos->pCreateBasepos = (LONG)pCreateBase;
		m_pCurGraphPos->pNext = m_pGraphPos;
		m_pGraphPos->pPrev = m_pCurGraphPos;
		m_pCurGraphPos = m_pGraphPos;
	}
	//�����ֱ�ߵĻ�
	if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*) pCreateBase;
		g_pDoc->PosBeEqualed(pGraphPos->potData1,pLine->m_Begin);
		g_pDoc->PosBeEqualed(pGraphPos->potData2,pLine->m_End);		
	}
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePoint)))
	{
		CCreatePoint* pPoint = (CCreatePoint*) pCreateBase;
		g_pDoc->PosBeEqualed(pGraphPos->potData1,pPoint->m_Begin);
	}	
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))//�����
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*) pCreateBase;
		pGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		pGraphPos->aryPoint.Copy(pPolyline->m_point);
		pGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		pGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))//����
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*) pCreateBase;
		pGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		pGraphPos->aryPoint.Copy(pPolyline->m_point);
		pGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		pGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))//��Բ
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*) pCreateBase;
		pGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		pGraphPos->aryPoint.Copy(pPolyline->m_point);
		pGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		pGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))//��������
	{
		CCreateSpline* pSpline = (CCreateSpline*) pCreateBase;
		pGraphPos->aryPoint.SetSize(pSpline->m_aryControlPoints.GetSize());
		pGraphPos->aryPoint.Copy(pSpline->m_aryControlPoints);
	}	
	else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))//Բ��
	{
		CCreateRoundArc* pArc = (CCreateRoundArc*) pCreateBase;
		m_pUndoList->pOperatieParam = m_UndoRedo.NewOperateParam();
		g_pDoc->PosBeEqualed(pGraphPos->potData1,pArc->m_Begin);
		g_pDoc->PosBeEqualed(pGraphPos->potData2,pArc->m_End);	
		pGraphPos->dData1 = pArc->m_dAngleStart;
		pGraphPos->dData2 = pArc->m_dAngleEnd;
	}
}





















