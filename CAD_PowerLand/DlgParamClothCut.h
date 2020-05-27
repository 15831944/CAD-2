#if !defined(AFX_DLGPARAMCLOTHCUT_H__5D6BF309_B686_4CBA_A4CD_5C82B6CF02FF__INCLUDED_)
#define AFX_DLGPARAMCLOTHCUT_H__5D6BF309_B686_4CBA_A4CD_5C82B6CF02FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgParamClothCut.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgParamClothCut dialog

class CDlgParamClothCut : public CDialog
{
// Construction
public:
	CDlgParamClothCut(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgParamClothCut)
	enum { IDD = IDD_DLG_PARAM_CLOTH_CUT };
	CComboBox	m_comboChamferEndStyle;
	double	m_dEditOverCutAngle;
	double	m_dEditOverCutLong;
	double	m_dEditScaleX;
	double	m_dEditScaleY;
	double	m_dEditChamfer;
	double	m_dEditSimplify;
	double	m_dEditLessPointRadius;
	double	m_dEditMoveOriginX;
	double	m_dEditMoveOriginY;
	double	m_dEditDashEmpty;
	double	m_dEditDashSolid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgParamClothCut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgParamClothCut)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboChamferEndStyle();
	//}}AFX_MSG
private:
	BOOL OnSaveParam();//保存参数
	BOOL OnInitParam();//读取参数 

	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPARAMCLOTHCUT_H__5D6BF309_B686_4CBA_A4CD_5C82B6CF02FF__INCLUDED_)
