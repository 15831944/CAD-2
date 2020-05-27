// PropertyBar.cpp : implementation file
//

#include "stdafx.h"
#include "CAD_PowerLand.h"
#include "PropertyBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar

CPropertyBar::CPropertyBar()
{
}

CPropertyBar::~CPropertyBar()
{
}


BEGIN_MESSAGE_MAP(CPropertyBar, CToolBar)
	//{{AFX_MSG_MAP(CPropertyBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar message handlers
/////////////////////////////////////////////////////////////////////////////
// CLineStyleCmb

CLineStyleCmb::CLineStyleCmb()
{
}

CLineStyleCmb::~CLineStyleCmb()
{
}


BEGIN_MESSAGE_MAP(CLineStyleCmb, CComboBox)
	//{{AFX_MSG_MAP(CLineStyleCmb)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineStyleCmb message handlers
/////////////////////////////////////////////////////////////////////////////
// CColorCmb

CColorCmb::CColorCmb()
{
}

CColorCmb::~CColorCmb()
{
}


BEGIN_MESSAGE_MAP(CColorCmb, CComboBox)
	//{{AFX_MSG_MAP(CColorCmb)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorCmb message handlers
/////////////////////////////////////////////////////////////////////////////
// CLineWidthCmb

CLineWidthCmb::CLineWidthCmb()
{
}

CLineWidthCmb::~CLineWidthCmb()
{
}


BEGIN_MESSAGE_MAP(CLineWidthCmb, CComboBox)
	//{{AFX_MSG_MAP(CLineWidthCmb)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// CColorCmb message handlers
int CColorCmb::AddItem(COLORREF rgbColor, char *pszName)
{
	CColorData* pData = new CColorData(rgbColor, pszName) ;
	
	int nRet = AddString((LPCSTR)pData) ;
//	DeleteString((LPCSTR)pData);
	if(nRet == LB_ERR)
		delete pData ;
	return nRet ;
}

void CColorCmb::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	CColorData* pData = (CColorData*)(lpDeleteItemStruct->itemData) ;
	ASSERT(pData) ;
	delete pData ;
	
	//	CComboBox::DeleteItem(lpDeleteItemStruct);
}

/*
 *	画组合框函数，重载了
 */
void CColorCmb::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC) ;
	CColorData* pData = (CColorData*)(lpDrawItemStruct->itemData) ;
	ASSERT(pData) ;
	CRect rc(lpDrawItemStruct->rcItem) ;
	
	if(lpDrawItemStruct->itemID == LB_ERR)
		return ;
	if(lpDrawItemStruct->itemAction&(ODA_DRAWENTIRE|ODA_SELECT))
		pData->DrawItem(pDC, rc, lpDrawItemStruct->itemState&ODS_SELECTED) ;
}

void CColorCmb::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = 20 ;//下拉组合框的颜色之间的距离（红色与黑色）
	
}

/*
 *	组合框里的内容改变时的事件
 */
void CColorCmb::OnSelchange() 
{
	int nIndex = GetCurSel() ;
	if(nIndex != LB_ERR) 
	{
		CColorData* pData = (CColorData*)GetItemDataPtr(nIndex) ;
		if(pData != (CColorData*) -1)
		{
			//需要修改：
			g_CurColor = pData->m_rgbColor;
			switch(nIndex)	
			{
			case 0:
				g_strCBM = g_pDoc->m_stcColorToM.m_CBM1;
				g_strSBM = g_pDoc->m_stcColorToM.m_SBM1;
				break;
			case 1:
				g_strCBM = g_pDoc->m_stcColorToM.m_CBM2;
				g_strSBM = g_pDoc->m_stcColorToM.m_SBM2;
				break;
			case 2:
				g_strCBM = g_pDoc->m_stcColorToM.m_CBM3;
				g_strSBM = g_pDoc->m_stcColorToM.m_SBM3;
				break;
			case 3:
				g_strCBM = g_pDoc->m_stcColorToM.m_CBM4;
				g_strSBM = g_pDoc->m_stcColorToM.m_SBM4;
				break;
			case 4:
				g_strCBM = g_pDoc->m_stcColorToM.m_CBM5;
				g_strSBM = g_pDoc->m_stcColorToM.m_SBM5;
				break;
			default:
				break;
			}
		}
	}
	
}

/////////////////////////////////////////////////////////////////////////////
// CLineStyleCmb

//////////////////////////////////////////////////////////////////////
// CColorData Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorData::~CColorData()
{

}

void CColorData::DrawItem(CDC* pDC, const CRect& rc, BOOL bSelected)
{
	CRect rcDraw(rc) ;
	COLORREF	crBackground, crText ;
	if(bSelected)
	{
		crBackground = GetSysColor(COLOR_HIGHLIGHT) ;
		crText = GetSysColor(COLOR_HIGHLIGHTTEXT) ;
	}
	else
	{
		crBackground = GetSysColor(COLOR_WINDOW) ;
		crText = GetSysColor(COLOR_WINDOWTEXT) ;
	}
	pDC->SetBkColor(crBackground) ;
	ExtTextOut(pDC->GetSafeHdc(), 0,0,ETO_OPAQUE, rcDraw, NULL, 0,NULL) ;
	
	CBrush NewBrush ;
	NewBrush.CreateSolidBrush(m_rgbColor) ;
	CBrush* pOldBrush = pDC->SelectObject(&NewBrush) ;
	pDC->Rectangle(rcDraw.left+2,rcDraw.top+2,rcDraw.left+16,rcDraw.top+16) ;
	pDC->SelectObject(pOldBrush) ;
	rcDraw.left+=16+5 ;//选择颜色的组合框，，，颜色与字的距离（红色与刀二的距离）！
	
	pDC->SetBkColor(crBackground) ;
	pDC->SetTextColor(crText) ;
	pDC->DrawText(m_strCname, m_strCname.GetLength(), rcDraw, DT_SINGLELINE|
		DT_VCENTER|DT_LEFT) ;
}
