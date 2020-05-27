// DlgColorToM.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgColorToM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgColorToM dialog


CDlgColorToM::CDlgColorToM(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgColorToM::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgColorToM)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgColorToM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgColorToM)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgColorToM, CDialog)
	//{{AFX_MSG_MAP(CDlgColorToM)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCLE, OnBtnCancle)
	ON_BN_CLICKED(IDC_BTN_COLOR1, OnBtnColor1)
	ON_BN_CLICKED(IDC_BTN_COLOR2, OnBtnColor2)
	ON_BN_CLICKED(IDC_BTN_COLOR3, OnBtnColor3)
	ON_BN_CLICKED(IDC_BTN_COLOR4, OnBtnColor4)
	ON_BN_CLICKED(IDC_BTN_COLOR5, OnBtnColor5)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgColorToM message handlers
/*
 *	参数保存
 */
void CDlgColorToM::OnBtnOk() 
{
	// TODO: Add your control notification handler code here
	/////////////////保存线色与M值的对应/////////////////////////////////
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "ColorM.ini";

	CString l_strDataName;			//保存用的中间数据一个是数据，一个是键名
	GetDlgItemText(IDC_EDIT_COLORCB1,l_strDataName);
	g_pDoc->m_stcColorToM.m_CBM1 = l_strDataName;
	WritePrivateProfileString("WorkParam","m_CBM1",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_COLORCB2,l_strDataName);
	g_pDoc->m_stcColorToM.m_CBM2 = l_strDataName;
	WritePrivateProfileString("WorkParam","m_CBM2",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_COLORCB3,l_strDataName);
	g_pDoc->m_stcColorToM.m_CBM3 = l_strDataName;
	WritePrivateProfileString("WorkParam","m_CBM3",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_COLORCB4,l_strDataName);
	g_pDoc->m_stcColorToM.m_CBM4 = l_strDataName;
	WritePrivateProfileString("WorkParam","m_CBM4",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_COLORCB5,l_strDataName);
	g_pDoc->m_stcColorToM.m_CBM5 = l_strDataName;
	WritePrivateProfileString("WorkParam","m_CBM5",l_strDataName,l_strFlieName);

	//sb
	GetDlgItemText(IDC_EDIT_COLORSB1,l_strDataName);
	g_pDoc->m_stcColorToM.m_SBM1 = l_strDataName;
	WritePrivateProfileString("WorkParam","m_SBM1",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_COLORSB2,l_strDataName);
	g_pDoc->m_stcColorToM.m_SBM2 = l_strDataName;
	WritePrivateProfileString("WorkParam","m_SBM2",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_COLORSB3,l_strDataName);
	g_pDoc->m_stcColorToM.m_SBM3 = l_strDataName;
	WritePrivateProfileString("WorkParam","m_SBM3",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_COLORSB4,l_strDataName);
	g_pDoc->m_stcColorToM.m_SBM4 = l_strDataName;
	WritePrivateProfileString("WorkParam","m_SBM4",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_COLORSB5,l_strDataName);
	g_pDoc->m_stcColorToM.m_SBM5 = l_strDataName;
	WritePrivateProfileString("WorkParam","m_SBM5",l_strDataName,l_strFlieName);
	////////////////////////把颜色值保存起来//////////////////////////////////
	l_strDataName.Format("%d",m_color1);
	WritePrivateProfileString("WorkParam","Color1",l_strDataName,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color1 = m_color1;

	l_strDataName.Format("%d",m_color2);
	WritePrivateProfileString("WorkParam","Color2",l_strDataName,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color2 = m_color2;
	
	l_strDataName.Format("%d",m_color3);
	WritePrivateProfileString("WorkParam","Color3",l_strDataName,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color3 = m_color3;
	
	l_strDataName.Format("%d",m_color4);
	WritePrivateProfileString("WorkParam","Color4",l_strDataName,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color4 = m_color4;
	
	l_strDataName.Format("%d",m_color5);
	WritePrivateProfileString("WorkParam","Color5",l_strDataName,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color5 = m_color5;
	////////////////////////把刀名保存起来//////////////////////////////////
	GetDlgItemText(IDC_EDIT_COLOR_MEAN1,l_strDataName);
	g_pDoc->m_stcColorToM.m_ColorMean1 = l_strDataName;
	WritePrivateProfileString("WorkParam","ColorMean1",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_COLOR_MEAN2,l_strDataName);
	g_pDoc->m_stcColorToM.m_ColorMean2 = l_strDataName;
	WritePrivateProfileString("WorkParam","ColorMean2",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_COLOR_MEAN3,l_strDataName);
	g_pDoc->m_stcColorToM.m_ColorMean3 = l_strDataName;
	WritePrivateProfileString("WorkParam","ColorMean3",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_COLOR_MEAN4,l_strDataName);
	g_pDoc->m_stcColorToM.m_ColorMean4 = l_strDataName;
	WritePrivateProfileString("WorkParam","ColorMean4",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_COLOR_MEAN5,l_strDataName);
	g_pDoc->m_stcColorToM.m_ColorMean5 = l_strDataName;
	WritePrivateProfileString("WorkParam","ColorMean5",l_strDataName,l_strFlieName);

	CDlgColorToM::OnOK();
}

void CDlgColorToM::OnBtnCancle() 
{
	// TODO: Add your control notification handler code here


	CDlgColorToM::OnCancel();
}

/*
 *	读取参数
 */
void CDlgColorToM::ReadDate()
{
	//读取M值
	SetDlgItemText(IDC_EDIT_COLORCB1,g_pDoc->m_stcColorToM.m_CBM1);
	SetDlgItemText(IDC_EDIT_COLORCB2,g_pDoc->m_stcColorToM.m_CBM2);
	SetDlgItemText(IDC_EDIT_COLORCB3,g_pDoc->m_stcColorToM.m_CBM3);
	SetDlgItemText(IDC_EDIT_COLORCB4,g_pDoc->m_stcColorToM.m_CBM4);
	SetDlgItemText(IDC_EDIT_COLORCB5,g_pDoc->m_stcColorToM.m_CBM5);

	SetDlgItemText(IDC_EDIT_COLORSB1,g_pDoc->m_stcColorToM.m_SBM1);
	SetDlgItemText(IDC_EDIT_COLORSB2,g_pDoc->m_stcColorToM.m_SBM2);
	SetDlgItemText(IDC_EDIT_COLORSB3,g_pDoc->m_stcColorToM.m_SBM3);
	SetDlgItemText(IDC_EDIT_COLORSB4,g_pDoc->m_stcColorToM.m_SBM4);
	SetDlgItemText(IDC_EDIT_COLORSB5,g_pDoc->m_stcColorToM.m_SBM5);

	SetDlgItemText(IDC_EDIT_COLOR_MEAN1,g_pDoc->m_stcColorToM.m_ColorMean1);
	SetDlgItemText(IDC_EDIT_COLOR_MEAN2,g_pDoc->m_stcColorToM.m_ColorMean2);
	SetDlgItemText(IDC_EDIT_COLOR_MEAN3,g_pDoc->m_stcColorToM.m_ColorMean3);
	SetDlgItemText(IDC_EDIT_COLOR_MEAN4,g_pDoc->m_stcColorToM.m_ColorMean4);
	SetDlgItemText(IDC_EDIT_COLOR_MEAN5,g_pDoc->m_stcColorToM.m_ColorMean5);
	//读取线色值 
	m_color1 = g_pDoc->m_stcColorToM.m_color1;
	m_color2 = g_pDoc->m_stcColorToM.m_color2;
	m_color3 = g_pDoc->m_stcColorToM.m_color3;
	m_color4 = g_pDoc->m_stcColorToM.m_color4;
	m_color5 = g_pDoc->m_stcColorToM.m_color5;
}

BOOL CDlgColorToM::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		SetWindowText("M代码功能参数");
	else
		SetWindowText("M code function parameters ");		
	ReadDate();	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
*	改变线色1的按扭
*/
void CDlgColorToM::OnBtnColor1() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_color1 = dlg.GetColor();
		CDC* pDC = GetDC();
		CBrush br,*oldbr;
		br.CreateSolidBrush(m_color1);
		oldbr = pDC->SelectObject(&br);
		pDC->Rectangle(244,47,293,70);
		pDC->SelectObject(oldbr);
		br.DeleteObject();
		ReleaseDC(pDC);
	}
}

/*
*	改变线色2的按扭
*/
void CDlgColorToM::OnBtnColor2() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_color2 = dlg.GetColor();
		CDC* pDC = GetDC();
		CBrush br,*oldbr;
		br.CreateSolidBrush(m_color2);
		oldbr = pDC->SelectObject(&br);
		pDC->Rectangle(244,79,293,102);
		pDC->SelectObject(oldbr);
		br.DeleteObject();
		ReleaseDC(pDC);
	}
}

/*
*	改变线色3的按扭
*/
void CDlgColorToM::OnBtnColor3() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_color3 = dlg.GetColor();
		CDC* pDC = GetDC();
		CBrush br,*oldbr;
		br.CreateSolidBrush(m_color3);
		oldbr = pDC->SelectObject(&br);
		pDC->Rectangle(244,109,293,132);
		pDC->SelectObject(oldbr);
		br.DeleteObject();
		ReleaseDC(pDC);
	}
}

/*
*	改变线色4的按扭
*/
void CDlgColorToM::OnBtnColor4() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_color4 = dlg.GetColor();
		CDC* pDC = GetDC();
		CBrush br,*oldbr;
		br.CreateSolidBrush(m_color4);
		oldbr = pDC->SelectObject(&br);
		pDC->Rectangle(244,141,293,164);
		pDC->SelectObject(oldbr);
		br.DeleteObject();
		ReleaseDC(pDC);
	}
}

/*
*	改变线色5的按扭
*/
void CDlgColorToM::OnBtnColor5() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_color5 = dlg.GetColor();
		CDC* pDC = GetDC();
		CBrush br,*oldbr;
		br.CreateSolidBrush(m_color5);
		oldbr = pDC->SelectObject(&br);
		pDC->Rectangle(244,173,293,196);
		pDC->SelectObject(oldbr);
		br.DeleteObject();
		ReleaseDC(pDC);
	}
}

void CDlgColorToM::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CBrush br,*oldbr;

	br.CreateSolidBrush(m_color1);
	oldbr = dc.SelectObject(&br);
	dc.Rectangle(244,47,293,70);

	br.DeleteObject();
	br.CreateSolidBrush(m_color2);
	dc.SelectObject(&br);
	dc.Rectangle(244,79,293,102);

	br.DeleteObject();
	br.CreateSolidBrush(m_color3);
	dc.SelectObject(&br);
	dc.Rectangle(244,109,293,132);

	br.DeleteObject();
	br.CreateSolidBrush(m_color4);
	dc.SelectObject(&br);
	dc.Rectangle(244,141,293,164);

	br.DeleteObject();
	br.CreateSolidBrush(m_color5);
	dc.SelectObject(&br);
	dc.Rectangle(244,173,293,196);

	dc.SelectObject(oldbr);
	br.DeleteObject();
	
	// Do not call CDialog::OnPaint() for painting messages
}
