// ResizableDlgBar.cpp: implementation of the CResizableDlgBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "ResizableDlgBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResizableDlgBar::CResizableDlgBar()
{

}

CResizableDlgBar::~CResizableDlgBar()
{

}


BOOL CResizableDlgBar::Create( CWnd* pParentWnd, UINT nIDTemplate,
							  UINT nStyle, UINT nID, BOOL bChange)
{
	if(!CDialogBar::Create(pParentWnd,nIDTemplate,nStyle,nID))
		return FALSE;
	
	m_bChangeDockedSize = bChange;
	m_sizeFloating = m_sizeDocked = m_sizeDefault;
	return TRUE;
}
BOOL CResizableDlgBar::Create( CWnd* pParentWnd,
							  LPCTSTR lpszTemplateName, UINT nStyle,
							  UINT nID, BOOL bChange)
{
	if (!CDialogBar::Create( pParentWnd, lpszTemplateName,
		nStyle, nID))
		return FALSE;
	
	m_bChangeDockedSize = bChange;
	m_sizeFloating = m_sizeDocked = m_sizeDefault;
	return TRUE;
}

//////////////////////////////////////////////////////////////////// 
// Overloaded functions

CSize CResizableDlgBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	// Return default if it is being docked or floated
	if ((dwMode & LM_VERTDOCK) || (dwMode & LM_HORZDOCK))
	{
		if (dwMode & LM_STRETCH) // if not docked stretch to fit
			return CSize((dwMode & LM_HORZ) ? 32767 : m_sizeDocked.cx,
			(dwMode & LM_HORZ) ? m_sizeDocked.cy : 32767);
		else
			return m_sizeDocked;
	}
	if (dwMode & LM_MRUWIDTH)
		return m_sizeFloating;
	// In all other cases, accept the dynamic length
	if (dwMode & LM_LENGTHY)
		return CSize(m_sizeFloating.cx, (m_bChangeDockedSize) ?
		m_sizeFloating.cy = m_sizeDocked.cy = nLength :
	m_sizeFloating.cy = nLength);
	else
		return CSize((m_bChangeDockedSize) ?
		m_sizeFloating.cx = m_sizeDocked.cx = nLength :
	m_sizeFloating.cx = nLength, m_sizeFloating.cy);
}

BEGIN_MESSAGE_MAP(CResizableDlgBar, CDialogBar)
//{{AFX_MSG_MAP(CResizableDlgBar)
// NOTE - the ClassWizard will add and remove mapping macros
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////// 
// CResizableDlgBar message handlers
/////////////////////////////////////////////////////////////////////

