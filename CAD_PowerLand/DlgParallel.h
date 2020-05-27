#if !defined(AFX_DLGPARALLEL_H__79F5874D_90B8_4F66_B265_B544E9D4D0DF__INCLUDED_)
#define AFX_DLGPARALLEL_H__79F5874D_90B8_4F66_B265_B544E9D4D0DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgParallel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgParallel dialog

#define PARALLEL		1
#define ROTATE_ANGLE	2



class CDlgParallel : public CDialog//平行线（指定角度旋转）对话框
{
// Construction
public:
	CDlgParallel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgParallel)
	enum { IDD = IDD_DLG_PARALLEL };
	double	m_dEditParallelHori;
	double	m_dEditParallelVert;
	//}}AFX_DATA

	int m_iCase;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgParallel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation



protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgParallel)
	afx_msg void OnParallelOk();
	afx_msg void OnParallelCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//向撤消链表里存操作
	void AddUndoList(long pCreatebase1,long pCreatebase2);
	//保存参数
	BOOL SaveParam();
	//执行平行线功能 
	void DoParallel();
	//执行指定角度旋转功能
	void DoRotateAngle();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPARALLEL_H__79F5874D_90B8_4F66_B265_B544E9D4D0DF__INCLUDED_)
