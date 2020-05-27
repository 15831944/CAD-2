#if !defined(AFX_DLGMATRIX_H__FEDBAADE_1B8B_47EB_A5AC_6B88121CA196__INCLUDED_)
#define AFX_DLGMATRIX_H__FEDBAADE_1B8B_47EB_A5AC_6B88121CA196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMatrix.h : header file
//
#include "UndoRedo.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMatrix dialog

class CDlgMatrix : public CDialog//阵列功能类
{
// Construction
public:
	CDlgMatrix(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMatrix)
	enum { IDD = IDD_DLG_MATRIX };
	int		m_EditHorNum;
	int		m_EditVerNum;
	double	m_dEditHorOfst;
	double	m_dEditVerOfst;
	double	m_dEditCreateLong;
	double	m_dEditCreateWide;
	BOOL	m_checkRotateMove;
	double	m_dEditYouHuaXMove;
	double	m_dEditYouHuaYMove;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMatrix)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMatrix)
	afx_msg void OnBtnMatrixOk();
	afx_msg void OnBtnMatrixNo();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEditHorizontal();
	afx_msg void OnKillfocusEditVeritcal();
	afx_msg void OnKillfocusEditHorizontalOffset();
	afx_msg void OnKillfocusEditVeritcalOffset();
	afx_msg void OnBtnGetLongWide();
	//}}AFX_MSG

private:
	//画矩阵的图元
	void DrawMatrix(CDC* pDC);



	//撤消类的对象
	CUndoRedo m_UndoRedo;
	//向撤消链表里添加的一个结点	
	PUNDOLIST m_pUndoList;
	//存图形的指针
	PGRAPHPOS m_pCurGraphPos,m_pheadGraphPos,m_pGraphPos;
	double m_dLong,m_dWide;


	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMATRIX_H__FEDBAADE_1B8B_47EB_A5AC_6B88121CA196__INCLUDED_)
