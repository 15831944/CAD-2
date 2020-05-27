// DrawMoveView.cpp: implementation of the CDrawMoveView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Draw.h"
#include "cad_powerland.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawMoveView::CDrawMoveView()
{

}

CDrawMoveView::~CDrawMoveView()
{

}

int CDrawMoveView::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_MBTN_MOVE));
	m_iNumOfClick++;
	m_posBegin = pos;	
	return 0;
}

int CDrawMoveView::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_MBTN_MOVE));
	if (m_iNumOfClick >= 1)
	{
		g_pView->m_dOriginX = m_posBegin.x-pos.x+g_pView->m_dOriginX;
		g_pView->m_dOriginY = m_posBegin.y-pos.y+g_pView->m_dOriginY;
		g_pView->Invalidate() ;
		
	}
	return 0;
}



int CDrawMoveView::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//退出选择起点功能
	m_iNumOfClick = 0;
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	
	return 0;
}

int CDrawMoveView::OnLButtonUp(UINT nFlags,const Position &pos)
{
	g_pView->m_dOriginX = m_posBegin.x-pos.x+g_pView->m_dOriginX;
	g_pView->m_dOriginY = m_posBegin.y-pos.y+g_pView->m_dOriginY;
	m_iNumOfClick = 0;
	g_pView->Invalidate() ;
	return 0;
}
