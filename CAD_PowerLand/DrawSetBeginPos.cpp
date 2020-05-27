// DrawSetBeginPos.cpp: implementation of the CDrawSetBeginPos class.
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

CDrawSetBeginPos::CDrawSetBeginPos()
{
	m_pUndoList = NULL;
}

CDrawSetBeginPos::~CDrawSetBeginPos()
{

}

int CDrawSetBeginPos::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	long Pointer;
	//�㵽������ϵ��Ķ���
	int l_iNumPick = g_pDoc->NumPicked(l_cPos,Pointer);
	if(l_iNumPick == -1)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû�е��ж��߶Σ����µ����ESC��ȡ��",2);				
		else
			RemindContent("\r\nNo point line, click or press the ESC key to cancel",2);	
				
		return FALSE;
	}
	m_pCreateBase = (CCreatePolyline*)Pointer;	
	//���߶�//////////////////////////////////////////
	if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
	{
		CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
		//����ǱպϵĻ�
		if (Polyline->m_point.GetAt(0) == Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1))
		{
			if(PolylineChoiceBegin(l_cPos,l_iNumPick))
				SuccessBreak();
		}
	}	
	//����//////////////////////////////////////////
	else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		if(RectChoiceBegin(l_cPos,l_iNumPick))
			SuccessBreak();
		else
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû�е��о��Σ����µ����ESC��ȡ��",2);					
			else
				RemindContent("\r\nThere is no point rectangle, click or press the ESC key to cancel",2);			
		}
								
	}
	else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{//Բ			
		if(RoundChoiceBegin(l_cPos))
			SuccessBreak();
		else
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû�е���Բ�����µ����ESC��ȡ��",2);
			else
				RemindContent("\r\nThere is no point circle, click or press the ESC key to cancel",2);			
		}
	}
	else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{//��Բ
		if(EllipseChoiceBegin(l_cPos,l_iNumPick))
			SuccessBreak();
		else
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû�е�����Բ�����µ����ESC��ȡ��",2);			
			else
				RemindContent("\r\nThere is no point ellipse, click or press the ESC key to cancel",2);			
		}
	}
	return 0;
}

int CDrawSetBeginPos::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	CDC *pDC = g_pView->GetDC();
	Position posBase(0,0);
	//����׽
	FindPointAll(pDC,posBase,pos,(g_pView->m_iSwitch&NEAR_POINT)|(g_pView->m_iSwitch&END_POINT)|(g_pView->m_iSwitch&MID_POINT));	
	g_pView->ReleaseDC(pDC);	
	return TRUE;
}

int CDrawSetBeginPos::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n�˳�ѡ����㹦�ܣ�",2);
	else
		RemindContent("\r\nExit select start function",2);			
	//�˳�ѡ����㹦��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	return 0;
}

 /*
  *	��ϳɹ�
  */
void CDrawSetBeginPos::SuccessBreak()
{
	g_pView->Invalidate();
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n����ѱ�:");
	else
		RemindContent("\r\nStarting point has changed:");
}

/*
 *	�պϵĶ����ѡ�����
 */
BOOL CDrawSetBeginPos::PolylineChoiceBegin(Position ClickPos,int iNumPick)
{
	CCreatePolyline *pTempPolyLine = (CCreatePolyline*)m_pCreateBase;
	//ÿ��ֱ�ߵ������˵�              �������ϵ�         ����
	Position  l_cBegin,l_cEnd,l_posCenter;
	double l_dTudu=0.0,l_dTudu1 = 0.0;
	//�������ĵ�ĸ���
	int l_iCountOfPoint = pTempPolyLine->m_point.GetSize();
	CArray<Position,Position&> l_AryPoint;//�㼯
	CArray<double,double&> l_AryTuDu;//���͹�ȵ�

	l_cBegin = pTempPolyLine->m_point.GetAt(iNumPick);
	l_cEnd = pTempPolyLine->m_point.GetAt(iNumPick+1);
	if(l_cBegin == ClickPos&&iNumPick == 0)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\nѡ�������㣬��ԭͼһ������Ч��",2);
		else
		RemindContent("\r\nChoose the starting point, just like the original, is invalid��",2);
		return FALSE;	
	}
	else if(l_cEnd == ClickPos)//�ڶ˵���
	{
		//�ѵ㼯������һ�£��ı����
		for(int i=iNumPick;;)
		{
			l_cBegin = pTempPolyLine->m_point.GetAt(i+1);
			l_dTudu = pTempPolyLine->m_dTuDu.GetAt(i+1);
			l_AryPoint.Add(l_cBegin);
			l_AryTuDu.Add(l_dTudu);
			i++;
			if(i == iNumPick)//���һ��
			{
				l_cBegin = pTempPolyLine->m_point.GetAt(iNumPick+1);
				l_dTudu = 0.0;
				l_AryPoint.Add(l_cBegin);
				l_AryTuDu.Add(l_dTudu);
				break;
			}
			if(i == l_iCountOfPoint-2)//�����
				i=-1;
		}	
	}
	else//���߶���
	{
		if(pTempPolyLine->m_dTuDu.GetAt(iNumPick) != 0.0)//�������Բ����
		{
			g_pDoc->polylineToArc(l_cBegin,l_cEnd,pTempPolyLine->m_dTuDu.GetAt(iNumPick),l_posCenter);
			l_dTudu = g_pDoc->RtnTudu(ClickPos,l_cEnd,l_posCenter,RtnArcDir(pTempPolyLine->m_dTuDu.GetAt(iNumPick)));
			l_dTudu1 = g_pDoc->RtnTudu(l_cBegin,ClickPos,l_posCenter,RtnArcDir(pTempPolyLine->m_dTuDu.GetAt(iNumPick)));		
		}
		l_AryPoint.Add(ClickPos);
		l_AryTuDu.Add(l_dTudu);
		int i=iNumPick;
		if(iNumPick == l_iCountOfPoint-2)
			i = -1;
		//�ѵ㼯������һ�£��ı����
		while (1)
		{
			l_cBegin = pTempPolyLine->m_point.GetAt(i+1);
			l_dTudu = pTempPolyLine->m_dTuDu.GetAt(i+1);
			l_AryPoint.Add(l_cBegin);
			l_AryTuDu.Add(l_dTudu);
			i++;
			if(i == iNumPick)//���һ��
			{
				l_AryPoint.RemoveAt(l_AryPoint.GetSize()-1);
				l_AryTuDu.RemoveAt(l_AryTuDu.GetSize()-1);
				l_AryPoint.Add(pTempPolyLine->m_point.GetAt(iNumPick));
				l_AryTuDu.Add(l_dTudu1);
				l_dTudu = 0.0;
				l_AryPoint.Add(ClickPos);
				l_AryTuDu.Add(l_dTudu);
				break;
			}
			if(i == l_iCountOfPoint-2)//�����
				i=-1;
		}
	}
	AddUndoList(pTempPolyLine->m_point,pTempPolyLine->m_dTuDu);
	pTempPolyLine->m_point.Copy(l_AryPoint);
	pTempPolyLine->m_dTuDu.Copy(l_AryTuDu);
	return TRUE;
}

//����ѡ�����
BOOL CDrawSetBeginPos::RectChoiceBegin(Position ClickPos,int iNumPick)
{
	CCreateRectangle *pTempPolyLine = (CCreateRectangle*)m_pCreateBase;
	//ÿ��ֱ�ߵ������˵�              �������ϵ�         ����
	Position  l_cBegin,l_cEnd,l_posCenter;
	double l_dTudu=0.0,l_dTudu1 = 0.0;
	//�������ĵ�ĸ���
	int l_iCountOfPoint = pTempPolyLine->m_point.GetSize();
	CArray<Position,Position&> l_AryPoint;//�㼯
	CArray<double,double&> l_AryTuDu;//���͹�ȵ�
	
	l_cBegin = pTempPolyLine->m_point.GetAt(iNumPick);
	l_cEnd = pTempPolyLine->m_point.GetAt(iNumPick+1);
	if(l_cBegin == ClickPos&&iNumPick == 0)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nѡ�������㣬��ԭͼһ������Ч��",2);
		else
			RemindContent("\r\nChoose the starting point, just like the original, is invalid��",2);
		return FALSE;	
	}
	else if(l_cEnd == ClickPos)//�ڶ˵���
	{
		//�ѵ㼯������һ�£��ı����
		for(int i=iNumPick;;)
		{
			l_cBegin = pTempPolyLine->m_point.GetAt(i+1);
			l_dTudu = pTempPolyLine->m_dTuDu.GetAt(i+1);
			l_AryPoint.Add(l_cBegin);
			l_AryTuDu.Add(l_dTudu);
			i++;
			if(i == iNumPick)//���һ��
			{
				l_cBegin = pTempPolyLine->m_point.GetAt(iNumPick+1);
				l_dTudu = 0.0;
				l_AryPoint.Add(l_cBegin);
				l_AryTuDu.Add(l_dTudu);
				break;
			}
			if(i == l_iCountOfPoint-2)//�����
				i=-1;
		}	
	}
	else//���߶���
	{
		if(pTempPolyLine->m_dTuDu.GetAt(iNumPick) != 0.0)//�������Բ����
		{
			g_pDoc->polylineToArc(l_cBegin,l_cEnd,pTempPolyLine->m_dTuDu.GetAt(iNumPick),l_posCenter);
			l_dTudu = g_pDoc->RtnTudu(ClickPos,l_cEnd,l_posCenter,RtnArcDir(pTempPolyLine->m_dTuDu.GetAt(iNumPick)));
			l_dTudu1 = g_pDoc->RtnTudu(l_cBegin,ClickPos,l_posCenter,RtnArcDir(pTempPolyLine->m_dTuDu.GetAt(iNumPick)));		
		}
		l_AryPoint.Add(ClickPos);
		l_AryTuDu.Add(l_dTudu);
		int i=iNumPick;
		if(iNumPick == l_iCountOfPoint-2)
			i = -1;
		//�ѵ㼯������һ�£��ı����
		while (1)
		{
			
			l_cBegin = pTempPolyLine->m_point.GetAt(i+1);
			l_dTudu = pTempPolyLine->m_dTuDu.GetAt(i+1);
			l_AryPoint.Add(l_cBegin);
			l_AryTuDu.Add(l_dTudu);
			i++;
			if(i == iNumPick)//���һ��
			{
				l_AryPoint.RemoveAt(l_AryPoint.GetSize()-1);
				l_AryTuDu.RemoveAt(l_AryTuDu.GetSize()-1);				
				l_AryPoint.Add(pTempPolyLine->m_point.GetAt(iNumPick));
				l_AryTuDu.Add(l_dTudu1);
				l_dTudu = 0.0;
				l_AryPoint.Add(ClickPos);
				l_AryTuDu.Add(l_dTudu);
				break;
			}
			if(i == l_iCountOfPoint-2)//�����
				i=-1;
		}
	}
	AddUndoList(pTempPolyLine->m_point,pTempPolyLine->m_dTuDu);
	pTempPolyLine->m_point.Copy(l_AryPoint);
	pTempPolyLine->m_dTuDu.Copy(l_AryTuDu);
	return TRUE;
}

//Բѡ�����
BOOL CDrawSetBeginPos::RoundChoiceBegin(Position ClickPos)
{
	CCreateRound *pTempRound = (CCreateRound*)m_pCreateBase;
	//���Ƿ����Բ��
	CPoint ScrClickPos;
	g_pView->WorldToScreem(ClickPos,ScrClickPos);
	if(-1 == pTempRound->IsPick(ScrClickPos))
		return FALSE;
	//������������Ӳ���
	m_pUndoList = m_UndoRedo.NewUndoList();
	m_pUndoList->OperatieWhat = ID_IO_KNIFE;
	m_pUndoList->pGraphPos = m_UndoRedo.NewGraphPos();
	m_pUndoList->pGraphPos->pCreateBasepos = (LONG)m_pCreateBase;
	m_pUndoList->pOperatieParam = m_UndoRedo.NewOperateParam();
	g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potBase,pTempRound->m_EndPos);
	g_pDoc->m_Undo->AddUndoList(m_pUndoList);
	//���¶���ĩ��
	double l_dAngle = GetAngleToXAxis(pTempRound->m_Center,ClickPos);
	double l_dRadius = pTempRound->m_dRadius;
	pTempRound->m_EndPos.x = pTempRound->m_Center.x+l_dRadius*cos(l_dAngle);
	pTempRound->m_EndPos.y = pTempRound->m_Center.y+l_dRadius*sin(l_dAngle);	
	return TRUE;
}

//��Բѡ�����
BOOL CDrawSetBeginPos::EllipseChoiceBegin(Position ClickPos,int iNumPick)
{
	CCreateEllipse *pTempPolyLine = (CCreateEllipse*)m_pCreateBase;
	//ÿ��ֱ�ߵ������˵�              �������ϵ�         ����
	Position  l_cBegin,l_cEnd,l_posCenter;
	double l_dTudu=0.0,l_dTudu1 = 0.0;
	//�������ĵ�ĸ���
	int l_iCountOfPoint = pTempPolyLine->m_point.GetSize();
	CArray<Position,Position&> l_AryPoint;//�㼯
	CArray<double,double&> l_AryTuDu;//���͹�ȵ�
	
	l_cBegin = pTempPolyLine->m_point.GetAt(iNumPick);
	l_cEnd = pTempPolyLine->m_point.GetAt(iNumPick+1);
	if(l_cBegin == ClickPos&&iNumPick == 0)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nѡ�������㣬��ԭͼһ������Ч��",2);
		else
			RemindContent("\r\nChoose the starting point, just like the original, is invalid��",2);
		return FALSE;	
	}
	else if(l_cEnd == ClickPos)//�ڶ˵���
	{
		//�ѵ㼯������һ�£��ı����
		for(int i=iNumPick;;)
		{
			l_cBegin = pTempPolyLine->m_point.GetAt(i+1);
			l_dTudu = pTempPolyLine->m_dTuDu.GetAt(i+1);
			l_AryPoint.Add(l_cBegin);
			l_AryTuDu.Add(l_dTudu);
			i++;
			if(i == iNumPick)//���һ��
			{
				l_cBegin = pTempPolyLine->m_point.GetAt(iNumPick+1);
				l_dTudu = 0.0;
				l_AryPoint.Add(l_cBegin);
				l_AryTuDu.Add(l_dTudu);
				break;
			}
			if(i == l_iCountOfPoint-2)//�����
				i=-1;
		}	
	}
	else//���߶���
	{
		l_AryPoint.Add(ClickPos);
		l_AryTuDu.Add(l_dTudu);
		int i=iNumPick;
		if(iNumPick == l_iCountOfPoint-2)
			i = -1;
		//�ѵ㼯������һ�£��ı����
		while (1)
		{
			
			l_cBegin = pTempPolyLine->m_point.GetAt(i+1);
			l_dTudu = pTempPolyLine->m_dTuDu.GetAt(i+1);
			l_AryPoint.Add(l_cBegin);
			l_AryTuDu.Add(l_dTudu);
			i++;
			if(i == iNumPick)//���һ��
			{
				l_AryPoint.RemoveAt(l_AryPoint.GetSize()-1);
				l_AryTuDu.RemoveAt(l_AryTuDu.GetSize()-1);				
				l_AryPoint.Add(pTempPolyLine->m_point.GetAt(iNumPick));
				l_AryTuDu.Add(l_dTudu1);
				l_dTudu = 0.0;
				l_AryPoint.Add(ClickPos);
				l_AryTuDu.Add(l_dTudu);
				break;
			}
			if(i == l_iCountOfPoint-2)//�����
				i=-1;
		}
	}
	AddUndoList(pTempPolyLine->m_point,pTempPolyLine->m_dTuDu);
	pTempPolyLine->m_point.Copy(l_AryPoint);
	pTempPolyLine->m_dTuDu.Copy(l_AryTuDu);
	return TRUE;
}

/*
 *	������������Ӳ���
 */
void CDrawSetBeginPos::AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	m_pUndoList = m_UndoRedo.NewUndoList();
	m_pUndoList->OperatieWhat = ID_IO_KNIFE;
	m_pCurGraphPos = m_UndoRedo.NewGraphPos();
	m_pCurGraphPos->pCreateBasepos = (LONG)m_pCreateBase;
	m_pCurGraphPos->aryPoint.SetSize(aryPoint.GetSize());
	m_pCurGraphPos->aryPoint.Copy(aryPoint);
	m_pCurGraphPos->aryTudu.SetSize(aryTudu.GetSize());
	m_pCurGraphPos->aryTudu.Copy(aryTudu);
	m_pUndoList->pGraphPos = m_pCurGraphPos;
	g_pDoc->m_Undo->AddUndoList(m_pUndoList);
}




