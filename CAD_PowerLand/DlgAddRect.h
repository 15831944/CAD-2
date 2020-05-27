#if !defined(AFX_DLGADDRECT_H__CBE56F90_03ED_4EB4_9F5C_7A3056BAC056__INCLUDED_)
#define AFX_DLGADDRECT_H__CBE56F90_03ED_4EB4_9F5C_7A3056BAC056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddRect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRect dialog

class CDlgAddRect : public CDialog
{
// Construction
public:
	CDlgAddRect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAddRect)
	enum { IDD = IDD_DLG_ADD_RECT };
	double	m_dEditLeft;
	double	m_dEditBottom;
	double	m_dEditRight;
	double	m_dEditTop;
	double	m_dEditRadius;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddRect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddRect)
	afx_msg void OnAddrectOk();
	afx_msg void OnAddrectCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	//获得选择集包围盒并外加矩形
	void OnGetBoxAddRect();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDRECT_H__CBE56F90_03ED_4EB4_9F5C_7A3056BAC056__INCLUDED_)
