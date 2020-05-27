// DrawAddPoint.cpp: implementation of the CDrawAddPoint class.
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

CDrawAddPoint::CDrawAddPoint()
{

}

CDrawAddPoint::~CDrawAddPoint()
{

}

int CDrawAddPoint::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
		CDC *pDC = g_pView->GetDC();
	g_refresh = 0;
	//������Ĵ���
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
		case 1:
			m_Begin  = l_cPos;
			break;
		case 2:			
			CDlgAddPoint dlg;
			dlg.m_posBegin = m_Begin;
			dlg.m_posEnd = l_cPos;
			dlg.DoModal();
			m_iNumOfClick = 0;	
			break;		
	}
	g_pView->ReleaseDC(pDC);
	return 0;
}

int CDrawAddPoint::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	CDC*	pDC = g_pView->GetDC(); 
	//����׽
	FindPointAll(pDC,m_Begin,pos,g_pView->m_iSwitch);		
	g_pView->ReleaseDC(pDC); 
	return 0;
}



int CDrawAddPoint::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//������ͼԪ//��Ҫ����ͼԪɾ��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;	//�����ѡ���ͼԪ	
	return 0;
}
