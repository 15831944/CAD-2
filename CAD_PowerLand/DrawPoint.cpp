// DrawPoint.cpp: implementation of the CDrawPoint class.
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

CDrawPoint::CDrawPoint()
{

}

CDrawPoint::~CDrawPoint()
{

}

int CDrawPoint::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	CDC *pDC = g_pView->GetDC();
	CCreatePoint* l_pLine1;


	if (nFlags == 2)
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num != -1)
		{
			l_cPos.x = atof(cmd.Left(l_num));	//����ǰ������
			l_cPos.y = atof(cmd.Mid(l_num+1));	//���ź������
		}
		else
		{
			double l_dLongLine = atof(cmd);
			int l_iInvali;
			m_pCreateBase->RtnLongPos(m_posRight,l_dLongLine,l_cPos,l_iInvali);
		}
	}
	l_pLine1 = new CCreatePoint(l_cPos);
	g_pDoc->m_CCreateBaseList.AddTail(l_pLine1);		
	l_pLine1->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
	g_pView->ReleaseDC(pDC); 
	//�Ѳ���������ӵ�����������
	AddUndoList((LONG)l_pLine1);
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	
	return 0;
}

int CDrawPoint::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	CDC*	pDC = g_pView->GetDC(); 
	//����׽
	FindPointAll(pDC,pos,pos,g_pView->m_iSwitch);	
	g_pView->ReleaseDC(pDC); 
	return 0;
}



int CDrawPoint::OnRButtonDown(UINT nFlags,const Position &pos)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	
	//��Ļ���ĵ�
	CPoint point;
	g_pView->WorldToScreem(l_cPos,point);
	POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//���ͷָ�룬������ѭ�����á�
	while(POS != NULL)//����ɨ������
	{
		m_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);
		// ����������ͼԪ��ѡ��
		if( m_pCreateBase->IsPick(point) != -1)
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n�������˵�ĳ��Ȼ�ESC��ȡ��");
			else
				RemindContent("\r\nEnter the length of the distance from this point or press the ESC key to cancel��");			
			
			m_posRight = l_cPos;
			//�ѽ�����������������
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
			break;
		}
	}
	return 0;
}

void CDrawPoint::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}