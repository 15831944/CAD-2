// DrawPolyline.cpp: implementation of the CDrawPolyline class.
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

CDrawPolyline::CDrawPolyline()
{
	//��ʼ�����������
	m_iNumOfClick = 0;
	//��ʼ��m_point������
	//m_poin
	m_PolylineLast = NULL;
}

CDrawPolyline::~CDrawPolyline()
{
	m_point.RemoveAll();
}

int CDrawPolyline::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	CPoint pt_begin;	//����ߵĵ�һ��
	CRect rect;	//����ߵĵ�һ��ķ���
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
		if (g_pView->m_stcSwitch.bShowArrow)	//����һ��ķ���
		{
			g_pView->WorldToScreem(l_cPos,pt_begin);
			rect.left = pt_begin.x-5;
			rect.top = pt_begin.y-5;
			rect.right = pt_begin.x+5;
			rect.bottom = pt_begin.y+5;		
			pDC->SelectStockObject(NULL_BRUSH);
			pDC->Rectangle(rect);
		}
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n����ֱ�ߵ���һ��,�������Ҽ����:");
		else
			RemindContent("\r\nInput next, or the right mouse button to finish:");
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		break;
	case 2:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n����ֱ�ߵ���һ��,�������Ҽ����:");
		else
			RemindContent("\r\nInput next, or the right mouse button to finish:");
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		//����MOUSEMOVE����󻭵���Щ
		CCreateLine* l_pEraseLine1 = new CCreateLine(m_middle,m_End);
		l_pEraseLine1->DrawMove(pDC,dmDrag);
		delete l_pEraseLine1;
		//����������µ�
		m_End = l_cPos;	
		CCreateLine* l_pLine1 = new CCreateLine(m_middle,m_End);
		l_pLine1->DrawMove(pDC,dmNormal);
		delete l_pLine1;
		m_iNumOfClick = 1;
		m_middle = l_cPos;		
		break;
		
	}
	g_pView->ReleaseDC(pDC);
	Position l_cListPos = l_cPos;
	m_point.Add(l_cListPos);
	//����㼯������������Ҫ��һ�������
	if(m_point.GetSize()==2)
	{
		CCreatePolyline* l_pTempPolyline = new CCreatePolyline(m_point);
		g_pDoc->m_CCreateBaseList.AddTail(l_pTempPolyline);
		m_PolylineLast = l_pTempPolyline;
		g_pView->Invalidate();
	}
	else if (m_point.GetSize()>2)//�������2����Ҫɾ��ǰ���������ڵ�
	{
		delete g_pDoc->m_CCreateBaseList.GetTail();//�����ǰ���ڴ�
		g_pDoc->m_CCreateBaseList.RemoveTail();//��������ɾ��
		CCreatePolyline* l_pTempPolyline = new CCreatePolyline(m_point);
		g_pDoc->m_CCreateBaseList.AddTail(l_pTempPolyline);
		m_PolylineLast = l_pTempPolyline;
	}
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	return 0;
}

int CDrawPolyline::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	// �õ��豸����ָ��
	CDC*	pDC = g_pView->GetDC(); 
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	Position	prePos, curPos;
	// ���������ڵ�ǰһ��λ��
	prePos = m_End; 
	curPos = pos;
	//����׽
	FindPointAll(pDC,m_middle,pos,g_pView->m_iSwitch);
	if (m_iNumOfClick >= 1)
	{	
		// ������ʱ���������һ����Ƥ��
		CCreateLine*	pTempLine1 = new CCreateLine(m_middle, prePos);
		if(g_refresh != 1)	//����Ϊ�˵�һ��û�в���ʱ������
			// ���Ӵ�û�б�ˢ��ʱ���ػ�ԭ������Ƥ��ʹ�䱻����
			pTempLine1->DrawMove(pDC,dmDrag);
		delete pTempLine1;
		//�趨��׽���ĵ�
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);
		// ������ʱ���󣬸��ݵ�ǰλ�û���һ����Ƥ��
		CCreateLine*	pTempLine2 = new CCreateLine(m_middle, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_End = curPos; 
	}
	// �ͷ��豸����ָ��
	g_pView->ReleaseDC(pDC); 		
	return 0;
}
/*
 *	ֻ�е���Ҽ�ʱ�ű���
 */
int CDrawPolyline::OnRButtonDown(UINT nFlags,const Position &pos)
{
	CDC *pDC = g_pView->GetDC();	
	//���߶εĻ�����������һ��
	//����MOUSEMOVE����󻭵���Щ
	CCreateLine* l_pEraseLine1 = new CCreateLine(m_middle,m_End);
	l_pEraseLine1->DrawMove(pDC,dmDrag);
	delete l_pEraseLine1;
	//////////////////////////////////////////////////////////////////////////
	//�Ѳ���������ӵ�����������
	AddUndoList((LONG)m_PolylineLast);				
	g_pView->ReleaseDC(pDC);
	//������ͼԪ//��Ҫ����ͼԪɾ��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n���:");
	else
		RemindContent("\r\nComplete:");		
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	return 0;
}

/*
 *	�����������ӵ���������
 */
void CDrawPolyline::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}