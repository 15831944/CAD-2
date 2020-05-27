// DrawVerticalLine.cpp: implementation of the CDrawVerticalLine class.
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

CDrawVerticalLine::CDrawVerticalLine()
	:m_Begin(0,0),m_End(0,0)
{
	m_aryAngleVertline.RemoveAll();
	m_dLongLine = 0.0;
}

CDrawVerticalLine::~CDrawVerticalLine()
{

}

int CDrawVerticalLine::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{		
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	//����ǵ�һ�����룬����û��ѡ�б���ֱ��
	if (m_aryAngleVertline.GetSize() == 0 &&m_iNumOfClick == 0)
	{
		if(!GetAngleVert(l_cPos))//����㵽����
			return FALSE;
	}		
	if (nFlags == 2&&m_iNumOfClick == 1)	//���ǿո��س���Ҫ�������������Ϊ��
	{
		m_dLongLine= atof(cmd);
		if(m_dLongLine < DOUBLE_EQUAL)
			return InvalidContent();//��������ʾ���������Ч�����
	}
	CDC *pDC = g_pView->GetDC();
	g_refresh = 0;
	//������Ĵ���
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
		case 1:
			m_Begin  = l_cPos;
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n����ֱ�ߵĳ���:");
			else
				RemindContent("\r\nEnter the length of the straight line:");					
			break;
		case 2:
			//����MOUSEMOVE����󻭵���Щ
			CCreateBase* l_pEraseLine1 = new CCreateLine(m_Begin,m_End);
			l_pEraseLine1->DrawMove(pDC,dmDrag);
			delete l_pEraseLine1;	

			//���ߵĽǶ�
			if(m_dLongLine != 0.0)
			{
				double l_dAngleVertLine = GetAngleToXAxis(m_Begin,m_End);
				m_End.x = m_Begin.x + m_dLongLine*cos(l_dAngleVertLine);
				m_End.y = m_Begin.y + m_dLongLine*sin(l_dAngleVertLine);			
			}
			//����������µ�
			CCreateBase* l_pLine1 = new CCreateLine(m_Begin,m_End);
			g_pDoc->m_CCreateBaseList.AddTail(l_pLine1);		
			l_pLine1->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
			m_iNumOfClick = 0;
			m_aryAngleVertline.RemoveAll();
			//�Ѳ���������ӵ�����������
			AddUndoList((LONG)l_pLine1);
			break;		
	}
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");	
	g_pView->ReleaseDC(pDC);
	return 0;
}

int CDrawVerticalLine::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	curPos = pos;
	//����׽
	FindPointAll(pDC,m_Begin,curPos,g_pView->m_iSwitch);	
	if (m_iNumOfClick >= 1)
	{	
		//��Ӧ�ô��ߵĵ�
		double l_dAngleMouse = GetAngleToXAxis(m_Begin,pos);
		//��������ĽǶ���Ƕȼ������С�н�
		double l_dAngleMin = fabs(m_aryAngleVertline.GetAt(0) - l_dAngleMouse);
		//���Ӧ�û��ĽǶ�
		double l_dAngleDraw = m_aryAngleVertline.GetAt(0);
		//������Ӧ�û��ĽǶ�
		for(int i=1;i<m_aryAngleVertline.GetSize();i++)
		{
			//�н�
			double l_dAngle = fabs(m_aryAngleVertline.GetAt(i) - l_dAngleMouse);
			if(l_dAngle >=MATH_PI)//����нǴ���180��
				l_dAngle = MATH_2PI - l_dAngle;
			if(l_dAngleMin > l_dAngle)
			{
				l_dAngleMin = l_dAngle;
				l_dAngleDraw = m_aryAngleVertline.GetAt(i);
			}
		}
		//Ӧ�û����ߵ�����
		double l_dLongDrawLine = cos(fabs(l_dAngleDraw - l_dAngleMouse))*m_Begin.Distance(pos);
		curPos.x = m_Begin.x + l_dLongDrawLine*cos(l_dAngleDraw);
		curPos.y = m_Begin.y + l_dLongDrawLine*sin(l_dAngleDraw);
		// ���������ڵ�ǰһ��λ��
		prePos = m_End; 
		// ������ʱ���������һ����Ƥ��
		CCreateBase*	pTempLine1 = new CCreateLine(m_Begin, prePos);
		if(g_refresh != 1)	//����Ϊ�˵�һ��û�в���ʱ������
			// ���Ӵ�û�б�ˢ��ʱ���ػ�ԭ������Ƥ��ʹ�䱻����
			pTempLine1->DrawMove(pDC,dmDrag);
		delete pTempLine1;
		//�趨��׽���ĵ�
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);	
		// ������ʱ���󣬸��ݵ�ǰλ�û���һ����Ƥ��
		CCreateBase*	pTempLine2 = new CCreateLine(m_Begin, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		// �ͷ��豸����ָ��		
		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_End = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	return 0;
}

int CDrawVerticalLine::OnRButtonDown(UINT nFlags,const Position &pos)
{
	CDC *pDC = g_pView->GetDC();
	//����MOUSEMOVE����󻭵���Щ
	CCreateBase* l_pEraseLine1 = new CCreateLine(m_middle,m_End);
	l_pEraseLine1->DrawMove(pDC,dmDrag);
	delete l_pEraseLine1;
	//������ͼԪ//��Ҫ����ͼԪɾ��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	g_pView->ReleaseDC(pDC);
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n���:");
	else
		RemindContent("\r\nComplete:");		
	
	CPoint pot;
	g_pView->WorldToScreem(pos,pot);
	CRect rect(pot.x-10,pot.y-10,pot.x+10,pot.y+10);
	g_pView->InvalidateRect(rect);
	
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	return 0;
}

/*
 *	�����������ӵ���������
 */
void CDrawVerticalLine::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}

/*
 *	���������ߵĴ��ߵĽǶ�
 */
BOOL CDrawVerticalLine::GetAngleVert(Position pos)
{
	//��Ļ���ĵ�
	CPoint point;
	g_pView->WorldToScreem(pos,point);
	POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//���ͷָ�룬������ѭ�����á�
	CCreateBase* l_pCreateBase = NULL;
	//�鿴�Ƿ�ѡ������
	while(POS != NULL)//����ɨ������
	{
		l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);
		// ����������ͼԪ��ѡ��
		int l_iPick = l_pCreateBase->IsPick(point);
		if( l_iPick != -1)
		{
			if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				CCreateLine* pLine = (CCreateLine*)l_pCreateBase;
				//				��ֱ���ߵĽǶȣ����Ҽӵ�������
				AddAngleToAry(GetAngleToXAxis(pLine->m_Begin,pLine->m_End));
			}
			else if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
			{
				CCreateRoundArc* pArc = (CCreateRoundArc*)l_pCreateBase;
				//				��ֱ���ߵĽǶȣ����Ҽӵ�������
				AddAngleToAry(GetAngleToXAxis(pArc->m_Center,pos)+MATH_PI/2);
			}
			else if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{
				CCreateRound* pRound = (CCreateRound*)l_pCreateBase;
				//				��ֱ���ߵĽǶȣ����Ҽӵ�������
				AddAngleToAry(GetAngleToXAxis(pRound->m_Center,pos)+MATH_PI/2);
			}
			else if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{
				CCreatePolyline* pPolyline = (CCreatePolyline*)l_pCreateBase;
				GetPolylineAngleVert(l_iPick,pos,pPolyline->m_point,pPolyline->m_dTuDu);
			}
			else if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{
				CCreateRectangle* pRect = (CCreateRectangle*)l_pCreateBase;
				GetPolylineAngleVert(l_iPick,pos,pRect->m_point,pRect->m_dTuDu);
			}
			else if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{
				CCreateEllipse* pEllipse = (CCreateEllipse*)l_pCreateBase;
				GetPolylineAngleVert(l_iPick,pos,pEllipse->m_point,pEllipse->m_dTuDu);
			}
			
		}
	}
	if (m_aryAngleVertline.GetSize() == 0)	//���û�е㵽ͼ��
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû�е����ͼ����:",2);
		else
			RemindContent("\r\nNot click on the graphic:",2);						
		return FALSE;
	}			
	return TRUE;
}

/*
 *	��Ƕȼ�����ӽǶ�
 */
void CDrawVerticalLine::AddAngleToAry(double dAngle)
{
	double l_dAngleVertline;//�ߵĴ���
	//����߽Ƕ��ڵ�������
	if(dAngle+MATH_PI/2 >= MATH_2PI)
	{
		l_dAngleVertline = dAngle - MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);
		l_dAngleVertline = dAngle-MATH_PI- MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);
	}
	else if(dAngle-MATH_PI/2 <= 0)//����߽Ƕ��ڵ�һ����
	{
		l_dAngleVertline = dAngle + MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);
		l_dAngleVertline = dAngle+MATH_PI+ MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);
		
	}
	else
	{
		l_dAngleVertline = dAngle + MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);
		l_dAngleVertline = dAngle - MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);		
	}

}

/*
 *	���㵽������ϻ���λ���Բ��ʱ��ֱ�ĽǶ�
 */
void CDrawVerticalLine::GetPolylineAngleVert(int iPick,Position posClick,CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	//				��ֱ���ߵĽǶȣ����Ҽӵ�������
	Position pos1,pos2,pos3(0,0);
	double dtudu;
	//����㵽����ߵĵ���
	int l_iSizePoint = aryPoint.GetSize();
	//�Ƿ�㵽�˵�����
	BOOL l_bPickPoint = FALSE;
	for(int i=0;i<l_iSizePoint;i++)
	{
		if(posClick == aryPoint.GetAt(i))//����㵽�Ķ˵�Ҫ���ĸ�
		{
			l_bPickPoint = TRUE;
			break;
		}
	}
	if(l_bPickPoint == TRUE)//�����������ǵ�
	{
		if(i!=0)//��ֱ�ڵ�ǰ����
		{
			pos1 = aryPoint.GetAt(i-1);
			pos2 = aryPoint.GetAt(i);
			dtudu = aryTudu.GetAt(i-1);
			if(dtudu == 0.0)
				AddAngleToAry(GetAngleToXAxis(pos1,pos2));
			else
			{
				Position l_posCenter;//Բ��
				g_pDoc->polylineToArc(pos1,pos2,dtudu,l_posCenter);
				AddAngleToAry(GetAngleToXAxis(l_posCenter,posClick)+MATH_PI/2);
			}			
		}
		//��ֱ�ڵ�����
		if(i!= l_iSizePoint-1)
		{
			pos1 = aryPoint.GetAt(i);
			pos2 = aryPoint.GetAt(i+1);
			dtudu = aryTudu.GetAt(i);
			if(dtudu == 0.0)
				AddAngleToAry(GetAngleToXAxis(pos1,pos2));
			else
			{
				Position l_posCenter;//Բ��
				g_pDoc->polylineToArc(pos1,pos2,dtudu,l_posCenter);
				AddAngleToAry(GetAngleToXAxis(l_posCenter,posClick)+MATH_PI/2);
			}
		}
	}
	else//����㵽����
	{
		pos1 = aryPoint.GetAt(iPick);
		pos2 = aryPoint.GetAt(iPick+1);
		dtudu = aryTudu.GetAt(iPick);
		if(dtudu == 0.0)
			AddAngleToAry(GetAngleToXAxis(pos1,pos2));
		else
		{
			Position l_posCenter;//Բ��
			g_pDoc->polylineToArc(pos1,pos2,dtudu,l_posCenter);
			AddAngleToAry(GetAngleToXAxis(l_posCenter,posClick)+MATH_PI/2);
		}
	}
}





