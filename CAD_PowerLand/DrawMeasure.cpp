// DrawMeasure.cpp: implementation of the CDrawMeasure class.
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

CDrawMeasure::CDrawMeasure()
{
	
}

CDrawMeasure::~CDrawMeasure()
{
	
}


int CDrawMeasure::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
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
			RemindContent("\r\n����ֱ�ߵ���һ��,�������Ҽ����:");
		else
			RemindContent("\r\nInput point, or the right mouse button to finish:");					
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		break;
	case 2:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n����ֱ�ߵ���һ��,�������Ҽ����:");
		else
			RemindContent("\r\nInput point, or the right mouse button to finish:");					
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		//����MOUSEMOVE����󻭵���Щ
		CCreateBase* l_pEraseLine1 = new CCreateLine(m_middle,m_End);
		l_pEraseLine1->DrawMove(pDC,dmDrag);
		delete l_pEraseLine1;
		//����������µ�
		m_End = l_cPos;	
		CCreateBase* l_pLine1 = new CCreateLine(m_middle,m_End);
		g_pDoc->m_pMeasureCreateBase = l_pLine1; 		
		l_pLine1->DrawMove(pDC,dmLayer);
		//��ֱ���룬ˮƽ���룬ֱ�߳���
		double disVer,disHor,disLong;
		disVer = fabs(m_middle.y - m_End.y);
		disHor = fabs(m_middle.x - m_End.x);
		disLong = m_middle.Distance(m_End);
		CString strVer,strHor,strLong;
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
		{
			strVer.Format("��ֱ%.3f",disVer);
			strHor.Format("ˮƽ%.3f",disHor);
			strLong.Format("ֱ�߳�%.3f",disLong);	
		}
		else
		{
			strVer.Format("vert%.3f",disVer);
			strHor.Format("Hori%.3f",disHor);
			strLong.Format("Long%.3f",disLong);	
		}
		
		pDC->SetTextColor(RGB(64,128,128));
		pDC->TextOut(20,20,strVer);
		pDC->TextOut(20,40,strHor);
		pDC->TextOut(20,60,strLong);
		//������ͼԪ//��Ҫ����ͼԪɾ��
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;	//�����ѡ���ͼԪ	
		m_middle = l_cPos;		
		break;			
	}
	g_pView->ReleaseDC(pDC);
	return 0;

}

int CDrawMeasure::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	// �õ��豸����ָ��
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	prePos = m_End; 
	curPos = pos;
	//����׽
	FindPointAll(pDC,m_middle,curPos,g_pView->m_iSwitch);	

	if (m_iNumOfClick >= 1)
	{
		
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

		//��ֱ���룬ˮƽ���룬ֱ�߳���
		double disVer,disHor,disLong;
		disVer = fabs(m_middle.y - curPos.y);
		disHor = fabs(m_middle.x - curPos.x);
		disLong = m_middle.Distance(curPos);
		CString strVer,strHor,strLong;
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
		{
			strVer.Format("��ֱ%.3f",disVer);
			strHor.Format("ˮƽ%.3f",disHor);
			strLong.Format("ֱ�߳�%.3f",disLong);	
		}
		else
		{
			strVer.Format("vert%.3f",disVer);
			strHor.Format("Hori%.3f",disHor);
			strLong.Format("Long%.3f",disLong);	
		}
		pDC->SetTextColor(RGB(64,128,128));
		pDC->TextOut(20,20,strVer);
		pDC->TextOut(20,40,strHor);
		pDC->TextOut(20,60,strLong);
		delete pTempLine2;
		// �ͷ��豸����ָ��
		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_End = curPos; 
	}
	return 0;
}

int CDrawMeasure::OnRButtonDown(UINT nFlags,const Position &pos)
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
	return 0;
}

