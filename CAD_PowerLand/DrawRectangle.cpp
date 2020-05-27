// DrawRectangle.cpp: implementation of the CDrawRectangle class.
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

CDrawRectangle::CDrawRectangle()
	: m_Begin(0,0), m_End(0,0),m_BeginDown(0,0),m_BeginRight(0,0)
{
	m_iNumOfClick = 0; // ��ʼ��������Ϊ 0
	m_bFlagRoundRect = FALSE;
	m_bSizeRect = FALSE;
	m_dRadius = 0;
	m_dRectWide = 0.0;
	m_dRectLong = 0.0;
}

CDrawRectangle::~CDrawRectangle()
{

}

int CDrawRectangle::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//���ǿո��س���Ҫ�������������Ϊ��
	{
		if (cmd.Find(",") == -1&&m_iNumOfClick == 0) //��һ�����뿴�ǲ���Բ�Ǿ���
		{
			if(cmd == "f"||cmd == "F")
			{
				m_bFlagRoundRect = TRUE;
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nָ�����ε�Բ�ǰ뾶:");
				else
					RemindContent("\r\nThe radius of the specified rectangle:");
				((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
				return 0;				
			}
			else
			{
				if (m_bFlagRoundRect == FALSE)
					return InvalidContent();//��������ʾ���������Ч�����
				m_dRadius = atof(cmd);
				if(m_dRadius == 0.0)
					return InvalidContent();//��������ʾ���������Ч�����
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nָ����һ���ǵ�:");
				else
					RemindContent("\r\nSpecify the first corner:");
				((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
				return 0;
			}
		}
		else if((m_bFlagRoundRect == FALSE||(m_bFlagRoundRect == TRUE&&m_dRadius != 0.0))&&cmd.Find(",") != -1)
		{
			int l_num = cmd.FindOneOf(",");
			if(l_num == -1)//��������ݲ���
				return InvalidContent();//��������ʾ���������Ч�����
			l_cPos.x = atof(cmd.Left(l_num));	//����ǰ������
			l_cPos.y = atof(cmd.Mid(l_num+1));	//���ź������
		}
		else if(m_iNumOfClick == 1)//���������D�ߴ�ʱ
		{
			if(cmd == "d"||cmd == "D")
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nָ�����εĳ���:");
				else
					RemindContent("\r\nThe long of the specified rectangle:");
				((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
				return 0;				
			}
			else
			{
				if (m_dRectLong == 0.0)
				{
					m_dRectLong = atof(cmd);
					//���Ļ���Ӣ��
					if (g_pDoc->m_stcWorkFor.IsChinese)
						RemindContent("\r\nָ�����εĿ��:");
					else
						RemindContent("\r\nThe wide of the specified rectangle:");
				}
				else
				{
					m_dRectWide = atof(cmd);
					if(m_dRectWide == 0.0)
						return InvalidContent();//��������ʾ���������Ч�����
					else
					{
						//���Ļ���Ӣ��
						if (g_pDoc->m_stcWorkFor.IsChinese)
							RemindContent("\r\n������ѡ����:");
						else
							RemindContent("\r\nClick mouse to sure direction:");						
						m_bSizeRect = TRUE;
					}
				}
				((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");			
				return 0;
			}
			
		}
		else
			return InvalidContent();//��������ʾ���������Ч�����		
	}
	g_refresh = 0;
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
	case 1:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nָ����һ���ǵ�[��ߴ�D]:");
		else
			RemindContent("\r\nSpecify another corner[or size D]��");			
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_Begin = l_cPos;
		break;
	case 2:

		CDC *pDC = g_pView->GetDC();
		//����MOUSEMOVE����󻭵���Щ
		CCreateRectangle* l_EraseRect = new CCreateRectangle(m_Begin,m_End,m_bFlagRoundRect,m_dRadius);
		l_EraseRect->DrawMove(pDC,dmDrag,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum );
		delete l_EraseRect;

		//���������ǳ��Ϳ�
		if(m_bSizeRect == FALSE)		
			m_End = l_cPos;
		CCreateRectangle* l_pLine = new CCreateRectangle(m_Begin,m_End,m_bFlagRoundRect,m_dRadius);
		g_pDoc->m_CCreateBaseList.AddTail(l_pLine);
		l_pLine->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		//�Ѳ���������ӵ�����������
		AddUndoList((LONG)l_pLine);		
		g_pView->ReleaseDC(pDC);			
		m_iNumOfClick = 0;
		//������ͼԪ//��Ҫ����ͼԪɾ��
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

int CDrawRectangle::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	// �õ��豸����ָ��
	CDC*	pDC = g_pView->GetDC(); 
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	//����׽
	if ((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL) 
		g_pView->m_iSwitch = g_pView->m_iSwitch^ORTHOGONAL;
	FindPointAll(pDC,m_Begin,pos,g_pView->m_iSwitch);
	//�趨��׽���ĵ�
	Position l_cPos(pos);
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (m_iNumOfClick == 1)
	{
		if(m_bSizeRect == TRUE)//���������ǳ��Ϳ�
		{
			if (m_Begin.x <= pos.x&& m_Begin.y <= pos.y) 
			{
				l_cPos.x = m_Begin.x + m_dRectLong;
				l_cPos.y = m_Begin.y + m_dRectWide;
			}
			else if (m_Begin.x <= pos.x&& m_Begin.y >= pos.y) 
			{
				l_cPos.x = m_Begin.x + m_dRectLong;
				l_cPos.y = m_Begin.y - m_dRectWide;
			}
			else if (m_Begin.x >= pos.x&& m_Begin.y <= pos.y) 
			{
				l_cPos.x = m_Begin.x - m_dRectLong;
				l_cPos.y = m_Begin.y + m_dRectWide;
			}
			else if (m_Begin.x >= pos.x&& m_Begin.y >= pos.y) 
			{
				l_cPos.x = m_Begin.x - m_dRectLong;
				l_cPos.y = m_Begin.y - m_dRectWide;
			}
		}
		Position	prePos, curPos;
		// ���������ڵ�ǰһ��λ��
		prePos = m_End; 
		curPos = l_cPos;
		
		// ������ʱ���������һ����Ƥ��
		CCreateRectangle*	pTempLine1 = new CCreateRectangle(m_Begin,prePos,m_bFlagRoundRect,m_dRadius);
		if(g_refresh != 1)
			// ���Ӵ�û�б�ˢ��ʱ���ػ�ԭ������Ƥ��ʹ�䱻����
			pTempLine1->DrawMove(pDC,dmDrag,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		delete pTempLine1;
		// ������ʱ���󣬸��ݵ�ǰλ�û���һ����Ƥ��
		CCreateRectangle*	pTempLine2 = new CCreateRectangle(m_Begin,curPos,m_bFlagRoundRect,m_dRadius);	
		pTempLine2->DrawMove(pDC,dmDrag,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		delete pTempLine2;
		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_End = curPos; 
	}
	// �ͷ��豸����ָ��
	g_pView->ReleaseDC(pDC); 		
	return 0;	
}

int CDrawRectangle::OnRButtonDown(UINT nFlags,const Position &pos)
{

	return 0;
}


/*
 *	�����������ӵ���������
 */
void CDrawRectangle::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}