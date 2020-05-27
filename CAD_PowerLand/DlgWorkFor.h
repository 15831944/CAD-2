#if !defined(AFX_DLGWORKFOR_H__37C1F2F1_E883_43AC_BFFE_879318DBE139__INCLUDED_)
#define AFX_DLGWORKFOR_H__37C1F2F1_E883_43AC_BFFE_879318DBE139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWorkFor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgWorkFor dialog

class CDlgWorkFor : public CDialog
{
// Construction
public:
	CDlgWorkFor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgWorkFor)
	enum { IDD = IDD_DLG_WORK_FOR };
	double	m_dMachineSizeX;
	double	m_dMachineSizeY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgWorkFor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgWorkFor)
	afx_msg void OnBtnSaveWork();
	afx_msg void OnBtnCancleWork();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnChioceFilepathg();
	afx_msg void OnBtnChioceFilepathdxf();
	//}}AFX_MSG

private:

	BOOL OnInitParam();//¶ÁÈ¡²ÎÊý 
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWORKFOR_H__37C1F2F1_E883_43AC_BFFE_879318DBE139__INCLUDED_)
