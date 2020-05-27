#if !defined(AFX_DLGPARAM1_H__433324D3_B4B4_428B_98BA_CDF950170B38__INCLUDED_)
#define AFX_DLGPARAM1_H__433324D3_B4B4_428B_98BA_CDF950170B38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgParam1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgParamGlassGrind dialog

class CDlgParamGlassGrind : public CDialog
{
// Construction
public:
	CDlgParamGlassGrind(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgParamGlassGrind)
	enum { IDD = IDD_DLG_PARAM_GLASS_GRIND };
	CComboBox	m_cmbDirClock5;
	CComboBox	m_cmbDirClock4;
	CComboBox	m_cmbDirClock3;
	CComboBox	m_cmbDirClock2;
	CComboBox	m_cmbDirClock1;
	CComboBox	m_cmbDaoBu;
	int		m_EditPerfectCount;
	int		m_EditPolishCount;
	int		m_EditRoughCount;
	double	m_EditPerfectSize;
	double	m_EditPolishSize;
	double	m_EditKnifeRadius;
	double	m_EditRoughSize;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgParamGlassGrind)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgParamGlassGrind)
	afx_msg void OnBtnParam1Ok();
	afx_msg void OnBtnParam1Cancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
private:
	BOOL OnSaveParam();//保存参数
	BOOL OnInitParam();//读取参数 

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPARAM1_H__433324D3_B4B4_428B_98BA_CDF950170B38__INCLUDED_)
