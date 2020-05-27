// ResizableDlgBar.h: interface for the CResizableDlgBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESIZABLEDLGBAR_H__00FD3FBF_D0D4_4B8D_A097_5BE7B15BB995__INCLUDED_)
#define AFX_RESIZABLEDLGBAR_H__00FD3FBF_D0D4_4B8D_A097_5BE7B15BB995__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResizableDlgBar : public CDialogBar  
{
public:
	CResizableDlgBar();
	virtual ~CResizableDlgBar();
	// Construction
public:
	BOOL Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle,
		UINT nID, BOOL = TRUE);
	BOOL Create( CWnd* pParentWnd, LPCTSTR lpszTemplateName,
		UINT nStyle, UINT nID, BOOL = TRUE);
	
	// Attributes
public:
	CSize m_sizeDocked;
	CSize m_sizeFloating;
	BOOL m_bChangeDockedSize;   // Indicates whether to keep
	// a default size for docking
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizableDlgBar)
	//}}AFX_VIRTUAL
	virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );
	
	// Implementation
public:
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CResizableDlgBar)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_RESIZABLEDLGBAR_H__00FD3FBF_D0D4_4B8D_A097_5BE7B15BB995__INCLUDED_)
