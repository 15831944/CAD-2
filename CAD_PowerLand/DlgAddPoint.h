#if !defined(AFX_DLGADDPOINT_H__07C669A5_62FA_4DDD_B5DE_7D6F67F30818__INCLUDED_)
#define AFX_DLGADDPOINT_H__07C669A5_62FA_4DDD_B5DE_7D6F67F30818__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddPoint.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddPoint dialog

class CDlgAddPoint : public CDialog
{
// Construction
public:
	CDlgAddPoint(CWnd* pParent = NULL);   // standard constructor

	Position m_posBegin,m_posEnd;
// Dialog Data
	//{{AFX_DATA(CDlgAddPoint)
	enum { IDD = IDD_DLG_ADD_POINT };
	double	m_dEditLongHori;
	double	m_dEditLongVert;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddPoint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddPoint)
	afx_msg void OnAddPointOk();
	afx_msg void OnAddPointCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

private:
	//点是否在给定的点集里
	BOOL PosOnAryPoint(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,Position posClick);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDPOINT_H__07C669A5_62FA_4DDD_B5DE_7D6F67F30818__INCLUDED_)
