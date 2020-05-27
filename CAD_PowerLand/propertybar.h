#if !defined(AFX_PROPERTYBAR_H__9B00C247_E157_4141_9C11_2B07FB9E2E7B__INCLUDED_)
#define AFX_PROPERTYBAR_H__9B00C247_E157_4141_9C11_2B07FB9E2E7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLineStyleCmb : public CComboBox
{
// Construction
public:
	CLineStyleCmb();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineStyleCmb)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLineStyleCmb();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLineStyleCmb)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CColorCmb window

class CColorCmb : public CComboBox
{
// Construction
public:
	CColorCmb();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorCmb)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	int AddItem(COLORREF rgbColor, char* pszName);	//向组合框里添加线色
	virtual ~CColorCmb();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorCmb)
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CLineWidthCmb window

class CLineWidthCmb : public CComboBox
{
// Construction
public:
	CLineWidthCmb();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineWidthCmb)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLineWidthCmb();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLineWidthCmb)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

class CColorData  
{
public:
	virtual ~CColorData();

public:

	COLORREF	m_rgbColor ;
	CString		m_strCname ;


	CColorData(COLORREF rgbcolor, char* pColorName)
		:m_rgbColor(rgbcolor), m_strCname(pColorName)
	{};
	void DrawItem(CDC* pDC, const CRect& rc, BOOL bSelected) ;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


// PropertyBar.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CPropertyBar window

class CPropertyBar : public CToolBar
{
	// Construction
public:
	CPropertyBar();
	
	CColorCmb			m_wndColorCmb ;
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertyBar)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CPropertyBar();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CPropertyBar)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CLineStyleCmb window

#endif // !defined(AFX_PROPERTYBAR_H__9B00C247_E157_4141_9C11_2B07FB9E2E7B__INCLUDED_)


