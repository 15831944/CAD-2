// DrawRound.cpp: implementation of the CDrawRound class.
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

CDrawRound::CDrawRound()
	:m_Center(0,0),m_EndPos(0,0)
{
	m_iNumOfClick = 0;
	m_iDirect = AD_CLOCKWISE;
}

CDrawRound::~CDrawRound()
{

}

int CDrawRound::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//���ǿո��س���Ҫ�������������Ϊ��
	{
		if (cmd.Find(",") == -1&&m_iNumOfClick == 0) //��һ�����뷽��ʱ��������
		{
			if(cmd == "s"||cmd == "S")
				m_iDirect = AD_CLOCKWISE;
			else if (cmd == "n"||cmd == "N")
				m_iDirect = AD_COUNTERCLOCKWISE;
			else
				return InvalidContent();//��������ʾ���������Ч�����
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nָ��Բ��Բ��:");
			else
				RemindContent("\r\nSpecifies the center of the circle��");			
			
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			return 0;
		}
		else if (cmd.Find(",") == -1&&m_iNumOfClick == 1) //�ڶ�������뾶
		{
			double l_dRadius = atof(cmd);
			l_cPos.y = m_Center.y;
			l_cPos.x = m_Center.x + l_dRadius;
		}
		else	//����������
		{
			int l_num = cmd.FindOneOf(",");
			if(l_num == -1)//��������ݲ���
				return InvalidContent();//��������ʾ���������Ч�����
			l_cPos.x = atof(cmd.Left(l_num));	//����ǰ������
			l_cPos.y = atof(cmd.Mid(l_num+1));	//���ź������
		}
	}
	
	m_iNumOfClick++;
	g_refresh = 0;
	switch(m_iNumOfClick)
	{
	case 1:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nָ���뾶����Բ�ܵ�����һ��:");
		else
			RemindContent("\r\nSpecify the radius or circumference of a circle at any point��");			
		
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_Center = l_cPos;
		break;
	case 2:
		
		CDC *pDC = g_pView->GetDC();
		//����MOUSEMOVE����󻭵���Щ
		CCreateRound* l_pEraseRound = new CCreateRound(m_Center,m_EndPos,m_iDirect);
		CCreateLine* l_pEraseLine = new CCreateLine(m_Center,m_EndPos);
		l_pEraseRound->DrawMove(pDC,dmDrag);
		l_pEraseLine->DrawMove(pDC,dmDrag);
		delete l_pEraseRound;
		delete l_pEraseLine;

		m_EndPos = l_cPos;
		CCreateRound* l_pLine = new CCreateRound(m_Center,l_cPos,m_iDirect);
		g_pDoc->m_CCreateBaseList.AddTail(l_pLine);
		l_pLine->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		//�Ѳ���������ӵ�����������
		AddUndoList((LONG)l_pLine);		
		g_pView->ReleaseDC(pDC);			
		m_iNumOfClick = 0;
		//������ͼԪ//��Ҫ����ͼԪɾ��
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;	//�����ѡ���ͼԪ
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���:");
		else
			RemindContent("\r\nComplete:");		
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		break;
	}
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	return 0;
}

int CDrawRound::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	CDC*	pDC = g_pView->GetDC(); 
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	Position	prePos, curPos;
	prePos = m_EndPos; 
	curPos = pos;
	//����׽
	FindPointAll(pDC,m_Center,curPos,g_pView->m_iSwitch);
	if (m_iNumOfClick == 1)
	{
		
		CCreateLine* ptem = new CCreateLine(m_Center,prePos);
		CCreateRound*	pTempLine1 = new CCreateRound(m_Center, prePos,m_iDirect);
		if(g_refresh != 1)
		{
			ptem->DrawMove(pDC,dmDrag);
			pTempLine1->DrawMove(pDC,dmDrag);
		}
		delete pTempLine1;
		delete ptem;
		//�趨��׽���ĵ�
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);
		
		CCreateLine* ptem2 = new CCreateLine(m_Center,curPos);
		CCreateRound*	pTempLine2 = new CCreateRound(m_Center, curPos,m_iDirect);	
		pTempLine2->DrawMove(pDC,dmDrag);
		ptem2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		delete ptem2;
		m_EndPos = curPos; 
	}
	g_pView->ReleaseDC(pDC); 		
	return 0;
}

int CDrawRound::OnRButtonDown(UINT nFlags,const Position &pos)
{

	return 0;
}


/*
 *	�����������ӵ���������
 */
void CDrawRound::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}