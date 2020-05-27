#if !defined(AFX_DLGCOMMANDVIEW_H__D193AD65_BE84_46D1_AE99_FCEED6E186B1__INCLUDED_)
#define AFX_DLGCOMMANDVIEW_H__D193AD65_BE84_46D1_AE99_FCEED6E186B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCommandView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCommandView dialog

class CDlgCommandView : public CResizableDlgBar
{
// Construction
public:
	CDlgCommandView(CWnd* pParent = NULL);   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDlgCommandView)
	enum { IDD = IDD_DLG_COMMAND_VIEW };
	CRichEditCtrl	m_RichEdit_Cmd;
	CEdit	m_ctlRemind;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCommandView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCommandView)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeRichedit4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOMMANDVIEW_H__D193AD65_BE84_46D1_AE99_FCEED6E186B1__INCLUDED_)
