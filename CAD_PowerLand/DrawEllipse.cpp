// DrawEllipse.cpp: implementation of the CDrawEllipse class.
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

CDrawEllipse::CDrawEllipse()
	:m_Begin(0,0),m_End(0,0),m_Second(0,0)
{
	m_iNumOfClick = 0;
}

CDrawEllipse::~CDrawEllipse()
{

}


int CDrawEllipse::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//���ǿո��س���Ҫ�������������Ϊ��
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num == -1)//��������ݲ���
			return InvalidContent();
		l_cPos.x = atof(cmd.Left(l_num));	//����ǰ������
		l_cPos.y = atof(cmd.Mid(l_num+1));	//���ź������
	}

	g_refresh = 0;
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
	case 1:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nָ�������һ���˵�:");
		else
			RemindContent("\r\nAnother endpoint of the specified axis��");			
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_Begin = l_cPos;
		break;
	case 2:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nָ��һ�㵽��Բ�ĵľ���Ϊ��һ�����᳤��:");
		else
			RemindContent("\r\nSpecify a bit to the elliptical heart another half shaft length��");			
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_Second = l_cPos;
		break;
	case 3:
		CDC *pDC = g_pView->GetDC();

		// ������һ����Ƥ��
		CCreateEllipse*	l_pEllipse = new CCreateEllipse(m_Begin,m_Second, m_End);	
		l_pEllipse->DrawMove(pDC,dmDrag);
		delete l_pEllipse;

		m_End = l_cPos;
		CCreateEllipse* l_pLine = new CCreateEllipse(m_Begin,m_Second,m_End);
		g_pDoc->m_CCreateBaseList.AddTail(l_pLine);
		l_pLine->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		//�Ѳ���������ӵ�����������
		AddUndoList((LONG)l_pLine);		
		g_pView->ReleaseDC(pDC);			
		m_iNumOfClick = 0;
		//������ͼԪ//��Ҫ����ͼԪɾ��complete
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;
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

int CDrawEllipse::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	// �õ��豸����ָ��
	CDC*	pDC = g_pView->GetDC(); 
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	if (m_iNumOfClick == 2)
	{
		Position	prePos, curPos;
		// ���������ڵ�ǰһ��λ��
		prePos = m_End; 
		curPos = pos;
		
		// ������ʱ���������һ����Ƥ��
		CCreateEllipse*	pTempLine1 = new CCreateEllipse(m_Begin,m_Second, prePos);
		if(g_refresh != 1)
			// ���Ӵ�û�б�ˢ��ʱ���ػ�ԭ������Ƥ��ʹ�䱻����
			pTempLine1->DrawMove(pDC,dmDrag);
		delete pTempLine1;
		// ������ʱ���󣬸��ݵ�ǰλ�û���һ����Ƥ��
		CCreateEllipse*	pTempLine2 = new CCreateEllipse(m_Begin,m_Second, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_End = curPos; 
	}
	//����׽
	FindPointAll(pDC,m_Second,pos,g_pView->m_iSwitch);
	// �ͷ��豸����ָ��
	g_pView->ReleaseDC(pDC); 		
	return 0;
}

int CDrawEllipse::OnRButtonDown(UINT nFlags,const Position &pos)
{

	return 0;
}


/*
 *	�����������ӵ���������
 */
void CDrawEllipse::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}