#if !defined(AFX_DLGCOLORTOM_H__492EF22A_5BCD_45BC_8A22_9FC005CB9F9B__INCLUDED_)
#define AFX_DLGCOLORTOM_H__492EF22A_5BCD_45BC_8A22_9FC005CB9F9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgColorToM.h : header file
//
#include "PropertyBar.h"	//颜色 定义对话框的头文件

/////////////////////////////////////////////////////////////////////////////
// CDlgColorToM dialog

class CDlgColorToM : public CDialog
{
// Construction
public:
	CDlgColorToM(CWnd* pParent = NULL);   // standard constructor

	//五个线的颜色 
	COLORREF m_color1;	
	COLORREF m_color2;
	COLORREF m_color3;
	COLORREF m_color4;
	COLORREF m_color5;

	CPropertyBar m_propertyBar;
// Dialog Data
	//{{AFX_DATA(CDlgColorToM)
	enum { IDD = IDD_DLG_COLORTOM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgColorToM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgColorToM)
	afx_msg void OnBtnOk();
	afx_msg void OnBtnCancle();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnColor1();	//改变线色的按扭
	afx_msg void OnBtnColor2();
	afx_msg void OnBtnColor3();
	afx_msg void OnBtnColor4();
	afx_msg void OnBtnColor5();
	afx_msg void OnPaint();
	//}}AFX_MSG

private:
	void ReadDate();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCOLORTOM_H__492EF22A_5BCD_45BC_8A22_9FC005CB9F9B__INCLUDED_)
