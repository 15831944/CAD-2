#if !defined(AFX_DLGSENDFILENAME_H__E55AEB64_6F69_4E95_9F42_EE3CC3E006E6__INCLUDED_)
#define AFX_DLGSENDFILENAME_H__E55AEB64_6F69_4E95_9F42_EE3CC3E006E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSendFileName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSendFileName dialog

class CDlgSendFileName : public CDialog
{
// Construction
public:
	CDlgSendFileName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSendFileName)
	enum { IDD = IDD_DLG_SENDFILE_NAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSendFileName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation



protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSendFileName)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSENDFILENAME_H__E55AEB64_6F69_4E95_9F42_EE3CC3E006E6__INCLUDED_)
