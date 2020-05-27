#if !defined(AFX_DLGPROPERTYDRAW_H__FE19EBC0_0791_4126_A5C3_5A54592F8855__INCLUDED_)
#define AFX_DLGPROPERTYDRAW_H__FE19EBC0_0791_4126_A5C3_5A54592F8855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPropertyDraw.h : header file
//
#include "ResizableDlgBar.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgPropertyDraw dialog
 
class CDlgPropertyDraw : public CResizableDlgBar
{
// Construction
public:
	
	CDlgPropertyDraw(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CDlgPropertyDraw)
	enum { IDD = IDD_DLG_PROPERTY_DRAW };
	CComboBox	m_ComboColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPropertyDraw)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPropertyDraw)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboColor();//组合框里的内容改变时的事件
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPERTYDRAW_H__FE19EBC0_0791_4126_A5C3_5A54592F8855__INCLUDED_)
