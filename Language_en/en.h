// en.h : main header file for the EN DLL
//

#if !defined(AFX_EN_H__C054968A_7F59_49A6_AB92_867979BA2AC8__INCLUDED_)
#define AFX_EN_H__C054968A_7F59_49A6_AB92_867979BA2AC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEnApp
// See en.cpp for the implementation of this class
//

class CEnApp : public CWinApp
{
public:
	CEnApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CEnApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EN_H__C054968A_7F59_49A6_AB92_867979BA2AC8__INCLUDED_)
