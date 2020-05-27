// DrawLine.cpp: implementation of the CDrawLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "Draw.h"

#include "MainFrm.h"
#include "CAD_PowerLandDoc.h"
#include "CAD_PowerLandView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawLine::CDrawLine()
	:m_Begin(0,0),m_End(0,0)
{
	m_iNumOfClick = 0;
}

CDrawLine::~CDrawLine()
{

}


int CDrawLine::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{		
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//���ǿո��س���Ҫ�������������Ϊ��
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num == -1)//������ǳ���
		{		
			if (m_iNumOfClick == 1)
			{
				double l_dLong = atof(cmd);
				double l_dAngle = GetAngleToXAxis(m_middle,l_cPos);
				if (l_dLong>DISTANCE_ZERO)
				{
					l_cPos.x = m_middle.x + l_dLong*cos(l_dAngle);
					l_cPos.y = m_middle.y + l_dLong*sin(l_dAngle);					
				}
				else
					return InvalidContent();//��������ʾ���������Ч�����
			}
		}
		else
		{
			if(cmd.FindOneOf("@") != -1)//����ҵ�@˵�������λ��
			{
				l_cPos.x = m_middle.x +  atof(cmd.Mid(1,l_num));	//����ǰ������
				l_cPos.y = m_middle.y +  atof(cmd.Mid(l_num+1));	//���ź������				
			}
			else
			{
				l_cPos.x = atof(cmd.Left(l_num));	//����ǰ������
				l_cPos.y = atof(cmd.Mid(l_num+1));	//���ź������
			}
		}
	}
	CDC *pDC = g_pView->GetDC();
	g_refresh = 0;
	//������Ĵ���
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
		case 1:
			m_Begin  = m_middle = l_cPos;
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n����ֱ�ߵ���һ��,�򳤶�,�������Ҽ����:");
			else
				RemindContent("\r\nInput point, or length, or the right mouse button to finish:");					
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			break;
		case 2:
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n����ֱ�ߵ���һ��,�򳤶�,�������Ҽ����:");
			else
				RemindContent("\r\nInput point, or length, or the right mouse button to finish:");					
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");

			//����MOUSEMOVE����󻭵���Щ
			CCreateBase* l_pEraseLine1 = new CCreateLine(m_middle,m_End);
			l_pEraseLine1->DrawMove(pDC,dmDrag);
			delete l_pEraseLine1;			
			m_End = l_cPos;	
			//����������µ�
			CCreateBase* l_pLine1 = new CCreateLine(m_middle,m_End);
			g_pDoc->m_CCreateBaseList.AddTail(l_pLine1);		
			l_pLine1->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
			m_iNumOfClick = 1;
			//�Ѳ���������ӵ�����������
			AddUndoList((LONG)l_pLine1);
			m_middle = l_cPos;		
			break;		
	}
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	
	g_pView->ReleaseDC(pDC);
	return 0;
}

int CDrawLine::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	curPos = pos;
	//����׽
	FindPointAll(pDC,m_middle,curPos,g_pView->m_iSwitch);	
	if (m_iNumOfClick >= 1)
	{	
		// ���������ڵ�ǰһ��λ��
		prePos = m_End; 
		// �õ��豸����ָ��
		
		// ������ʱ���������һ����Ƥ��
		CCreateBase*	pTempLine1 = new CCreateLine(m_middle, prePos);
		if(g_refresh != 1)	//����Ϊ�˵�һ��û�в���ʱ������
			// ���Ӵ�û�б�ˢ��ʱ���ػ�ԭ������Ƥ��ʹ�䱻����
			pTempLine1->DrawMove(pDC,dmDrag);
		delete pTempLine1;
		//�趨��׽���ĵ�
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);	
		// ������ʱ���󣬸��ݵ�ǰλ�û���һ����Ƥ��
		CCreateBase*	pTempLine2 = new CCreateLine(m_middle, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		// �ͷ��豸����ָ��		
		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_End = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	return 0;
}

int CDrawLine::OnRButtonDown(UINT nFlags,const Position &pos)
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
void CDrawLine::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}