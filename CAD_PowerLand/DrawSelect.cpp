// DrawSelect.cpp: implementation of the CDrawSelect class.
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

CDrawSelect::CDrawSelect()
: m_Begin(0,0), m_End(0,0)
{
	m_iNumOfClick = 0; // ��ʼ��������Ϊ 0
}

CDrawSelect::~CDrawSelect()
{
	
}

int CDrawSelect::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	g_refresh = 0;
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
	case 1:
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		//���������Ťʱ�����ʱ��
		g_pView->m_PosBegin = m_Begin = l_cPos;
		break;
	case 2:		
		CDC *pDC = g_pView->GetDC();
		//����MOUSEMOVE����󻭵���Щ
		if (m_Begin.x <= m_End.x)
			m_draw2 = dmDragSelect1;
		else
			m_draw2 = dmDragSelect2;
		CCreateRectangle* l_EraseRect = new CCreateRectangle(m_Begin,m_End);
		l_EraseRect->DrawMove(pDC,m_draw2,FALSE,FALSE);
		delete l_EraseRect;
		
		//����������µ�
		m_End = l_cPos;
		//����ѡ�ľ�������
		CPoint pos1,pos2;
		g_pView->WorldToScreem(m_Begin,pos1);
		g_pView->WorldToScreem(m_End,pos2);		
		CRgn l_cSelectRect;
		l_cSelectRect.CreateRectRgn(pos1.x,pos1.y,pos2.x,pos2.y);
		//��ѡʱ�ǲ�����������ѡ��
		BOOL l_bComplex = (pos1.x >= pos2.x)?TRUE:FALSE;
		//�鿴�ĸ�ͼ�α�ѡ���˿�ѡ��
		POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//���ͷָ�룬������ѭ�����á�
		while(position != NULL)//����ɨ������
		{
			CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
			// ����������ͼԪ����ѡ��
			if( l_pCreateBase->IsSelect(&l_cSelectRect,l_bComplex))
			{
				int l_iNumCollect = g_pDoc->IsSelected(l_pCreateBase);
				if( l_iNumCollect == -1 )// �ж�ͼԪ�����Ƿ��Ѿ���ѡ����,û��ѡ����
				{
					l_pCreateBase->DrawMove(pDC,dmSelect);
					g_pDoc->m_CollectOfPicked.Add(l_pCreateBase);  // ��ͼԪ����ѡ����
				}
				else//��ѡ����
				{
					if (nFlags == 5)//��SHIFT����ѡ��ѡ�����Ƴ�
					{
						g_pDoc->m_CollectOfPicked.RemoveAt(l_iNumCollect);
						l_pCreateBase->DrawMove(pDC,dmNormal);
					}
				}
			}
		}	
		for(int i = 0;  i<g_pDoc->m_CollectOfPicked.GetSize();i++)
		{
			CCreateBase* l_pCreateBaseList = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
			l_pCreateBaseList->DrawMove(pDC,dmSelect,FALSE,FALSE);
		}
		l_cSelectRect.DeleteObject();
		g_pView->ReleaseDC(pDC);			
		m_iNumOfClick = 0;
		//������ͼԪ//��Ҫ����ͼԪɾ��
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;
		g_pView->m_iFlagWhichBtnDown = MEIYOU;
		break;		
	}
	//��ѡ����ֻ��һ��Ԫ��
	if(g_pDoc->m_CollectOfPicked.GetSize() == 1)
	{
		CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
		pPropertCreate->PropertyView();
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"�ӹ�˳��:");					
	}	
	else 
	{
		CCreateBase l_show;
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,"");				
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(5);					
		l_show.ShowControl(0,FALSE);
	}
	return 0;
}

int CDrawSelect::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_NONE));
	//���������Ťʱ�����ʱ��
	g_pView->m_PosLast = pos;
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	if (m_iNumOfClick == 1)
	{
		Position	prePos, curPos;
		// ���������ڵ�ǰһ��λ��
		prePos = m_End; 
		curPos = pos;
		// �õ��豸����ָ��
		CDC*	pDC = g_pView->GetDC(); 
		//��ѡ��ʽ
		if (m_Begin.x <= curPos.x)
			m_draw2 = dmDragSelect1;
		else
			m_draw2 = dmDragSelect2;
		// ������ʱ���������һ����Ƥ��
		CCreateRectangle*	pTempLine1 = new CCreateRectangle(m_Begin,prePos);
		if(g_refresh != 1)// ���Ӵ�û�б�ˢ��ʱ���ػ�ԭ������Ƥ��ʹ�䱻����			
			pTempLine1->DrawMove(pDC,m_draw1,FALSE,FALSE);
		delete pTempLine1;
		// ������ʱ���󣬸��ݵ�ǰλ�û���һ����Ƥ��
		CCreateRectangle*	pTempLine2 = new CCreateRectangle(m_Begin,curPos);	
		pTempLine2->DrawMove(pDC,m_draw2,FALSE,FALSE);
		delete pTempLine2;

		// �ͷ��豸����ָ��
		g_pView->ReleaseDC(pDC); 		
		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_End = curPos; 
		m_draw1 = m_draw2;
	}
	return 0;	
}

int CDrawSelect::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//������ͼԪ//��Ҫ����ͼԪɾ��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	return 0;
}