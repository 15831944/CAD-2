// CAD_PowerLand.h : main header file for the CAD_POWERLAND application
//

#if !defined(AFX_CAD_POWERLAND_H__C067D13D_20CD_49FB_8C54_E5C65B9A7E59__INCLUDED_)
#define AFX_CAD_POWERLAND_H__C067D13D_20CD_49FB_8C54_E5C65B9A7E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "..\\Language_ch\\resource.h"       // main symbols
#include "MainFrm.h"
#include "CAD_PowerLandDoc.h"
#include "CAD_PowerLandView.h"

#define INI_FILE_COLORM           "C:\\Program Files\\CAD_PowerLand\\"
#define COLOR_GRAY		RGB(200,200,200)

//////////////////////////////////////////////////////////////////////////
extern class CCAD_PowerLandView* g_pView;
extern class CCAD_PowerLandDoc* g_pDoc;
extern int g_refresh;
extern int g_iNumUndo;
extern COLORREF				g_CurColor ;
extern CString g_strCBM,g_strSBM;

/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandApp:
// See CAD_PowerLand.cpp for the implementation of this class
//

class CCAD_PowerLandApp : public CWinApp
{
public:
	CCAD_PowerLandApp();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCAD_PowerLandApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCAD_PowerLandApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
private:

		
	BOOL m_bChinese;
	HINSTANCE m_hLangDLL;
		DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAD_POWERLAND_H__C067D13D_20CD_49FB_8C54_E5C65B9A7E59__INCLUDED_)
