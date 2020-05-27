// DrawRoundArc.cpp: implementation of the CDrawRoundArc class.
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

CDrawRoundArc::CDrawRoundArc()
	:m_Begin(0,0),m_End(0,0),m_Center(0,0)
{
	m_iNumOfClick = 0;
	m_iDirect = AD_CLOCKWISE;
}

CDrawRoundArc::~CDrawRoundArc()
{

}

int CDrawRoundArc::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	CDC*	pDC = g_pView->GetDC(); 
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
				RemindContent("\r\nָ��Բ����Բ��:");
			else
				RemindContent("\r\nSpecifies the center of the circle arc��");			
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			return 0;
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
			RemindContent("\r\nָ��Բ�������:");
		else
			RemindContent("\r\nSpecify the starting point of the arc��");			
		
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_Center = l_cPos;			
			break;
	case 2:
		{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nָ��Բ�����յ�:");
		else
			RemindContent("\r\nSpecify the end of the arc��");			
		
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		// �������϶�״̬ʱ��ʾ����Ƥ��
		CCreateLine*	pTempLine2 = new CCreateLine(m_Center, m_Begin);
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		
		m_Begin = l_cPos;
		break;
		}
	case 3:
		{
		// �������϶�״̬ʱ��ʾ����Ƥ��
		CCreateLine*	pTempLine2 = new CCreateLine(m_Center, m_End);
		CCreateRoundArc*	pTempArc = new CCreateRoundArc(m_Begin,m_Center, m_End,m_iDirect);
		pTempLine2->DrawMove(pDC,dmDrag);
		pTempArc->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		delete pTempArc;
		// �յ�Ϊ��굥��λ��
		m_End = l_cPos ;
		if(fabs(l_cPos.y - m_Begin.y)<DISTANCE_ZERO)//�������ĩ��һ���Ͳ���
		{
			g_pView->ReleaseDC(pDC); 		
			return 0;
		}		
		CCreateRoundArc*	pNewArc = new CCreateRoundArc(m_Begin,m_Center, m_End,m_iDirect);			
		// ��Բ��ָ����ӵ�ͼԪ����
		g_pDoc->m_CCreateBaseList.AddTail(pNewArc);
		pNewArc->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		//�Ѳ���������ӵ�����������
		AddUndoList((LONG)pNewArc);		
		// �ͷ��豸����ָ��
		// ������������Ϊ 0
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
	}
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	g_pView->ReleaseDC(pDC); 
	return 0;
}

int CDrawRoundArc::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	CDC*	pDC = g_pView->GetDC(); 
	//����׽
	FindPointAll(pDC,m_Center,pos,g_pView->m_iSwitch);
	//�趨��׽���ĵ�
	Position l_cPos(pos);
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	switch(m_iNumOfClick)		
	{
	case 1:
		{
			Position	prePos, curPos;
			prePos = m_Begin; 		
			curPos = l_cPos;
			//����
			CCreateLine* pTempLine1= new CCreateLine(m_Center,prePos);
			if(g_refresh != 1)
				pTempLine1->DrawMove(pDC,dmDrag);
			delete pTempLine1;
			//��ͼ��
			CCreateLine*  pTempLine2 = new CCreateLine(m_Center,curPos);
			pTempLine2->DrawMove(pDC,dmDrag);
			delete pTempLine2;			
			m_Begin = curPos; 
			break;
		}
		
	case 2:
		{
		Position	prePos, curPos;
		prePos = m_End; 		
		curPos = l_cPos;
		//����
		CCreateLine* pTempLine1= new CCreateLine(m_Center,prePos);
		CCreateRoundArc* ptemArc1 = new CCreateRoundArc(m_Begin,m_Center,prePos,m_iDirect);
		if(g_refresh != 1)
		{
			ptemArc1->DrawMove(pDC,dmDrag);
			pTempLine1->DrawMove(pDC,dmDrag);
		}
		delete pTempLine1;
		delete ptemArc1;
		//��ͼ��
		CCreateLine*  pTempLine2 = new CCreateLine(m_Center,curPos);
		CCreateRoundArc*	ptemArc2 = new CCreateRoundArc(m_Begin,m_Center,curPos,m_iDirect);	
		pTempLine2->DrawMove(pDC,dmDrag);
		ptemArc2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		delete ptemArc2;

		m_End = curPos; 
		break;
		}
	}
	g_pView->ReleaseDC(pDC); 		
	return 0;
}

int CDrawRoundArc::OnRButtonDown(UINT nFlags,const Position &pos)
{

	return 0;
}


/*
 *	�����������ӵ���������
 */
void CDrawRoundArc::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}