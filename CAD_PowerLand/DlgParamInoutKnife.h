#if !defined(AFX_DLGPARAMINOUTKNIFE_H__E33D3115_0D07_4CE8_A697_14CC00F32B72__INCLUDED_)
#define AFX_DLGPARAMINOUTKNIFE_H__E33D3115_0D07_4CE8_A697_14CC00F32B72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgParamInoutKnife.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgParamInoutKnife dialog

class CDlgParamInoutKnife : public CDialog
{
// Construction
public:
	CDlgParamInoutKnife(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgParamInoutKnife)
	enum { IDD = IDD_DLG_PARAM_INOUT_KNIFE };
	CButton	m_IsCenterInput;
	double	m_EditIArcAngle;
	double	m_EditIArcRadius;
	double	m_EditIArcRadiusPercent;
	double	m_EditILineLong;
	double	m_EditILineLongPercent;
	double	m_EditOArcAngle;
	double	m_EditOArcRadius;
	double	m_EditOArcRadiusPercent;
	double	m_EditOLineLongPercent;
	double	m_EditOLineLong;
	double	m_EditOOverLong;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgParamInoutKnife)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgParamInoutKnife)
	afx_msg void OnBtnSaveInoutKnife();
	afx_msg void OnBtnCancleInoutKnife();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnIoknifeCopy();
	afx_msg void OnKillfocusEditIknifeLineLongpercent();
	afx_msg void OnKillfocusEditIknifeLineLong();
	afx_msg void OnKillfocusEditIknifeArcRadiuspercent();
	afx_msg void OnKillfocusEditIknifeArcRadius();
	afx_msg void OnKillfocusEditOknifeLineLongpercent();
	afx_msg void OnKillfocusEditOknifeLineLong();
	afx_msg void OnKillfocusEditOknifeArcRadiuspercent();
	afx_msg void OnKillfocusEditOknifeArcRadius();
	//}}AFX_MSG

private:
	BOOL OnSaveParam();//保存参数
	BOOL OnInitParam();//读取参数 


	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPARAMINOUTKNIFE_H__E33D3115_0D07_4CE8_A697_14CC00F32B72__INCLUDED_)
