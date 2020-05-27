// ch.h : main header file for the CH DLL
//

#if !defined(AFX_CH_H__2003E800_46C3_482A_8741_E452A10F62EC__INCLUDED_)
#define AFX_CH_H__2003E800_46C3_482A_8741_E452A10F62EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChApp
// See ch.cpp for the implementation of this class
//

class CChApp : public CWinApp
{
public:
	CChApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CH_H__2003E800_46C3_482A_8741_E452A10F62EC__INCLUDED_)
