// DlgParam1.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgParamGlassGrind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgParamGlassGrind dialog


CDlgParamGlassGrind::CDlgParamGlassGrind(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParamGlassGrind::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgParamGlassGrind)
	m_EditPerfectCount = 0;
	m_EditPolishCount = 0;
	m_EditRoughCount = 0;
	m_EditPerfectSize = 0.0;
	m_EditPolishSize = 0.0;
	m_EditKnifeRadius = 0.0;
	m_EditRoughSize = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgParamGlassGrind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgParamGlassGrind)
	DDX_Control(pDX, IDC_CMB_DIR_CLOCK5, m_cmbDirClock5);
	DDX_Control(pDX, IDC_CMB_DIR_CLOCK4, m_cmbDirClock4);
	DDX_Control(pDX, IDC_CMB_DIR_CLOCK3, m_cmbDirClock3);
	DDX_Control(pDX, IDC_CMB_DIR_CLOCK2, m_cmbDirClock2);
	DDX_Control(pDX, IDC_CMB_DIR_CLOCK1, m_cmbDirClock1);
	DDX_Control(pDX, IDC_CMB_DAOBU, m_cmbDaoBu);
	DDX_Text(pDX, IDC_EDIT_PERFECT_COUNT, m_EditPerfectCount);
	DDV_MinMaxInt(pDX, m_EditPerfectCount, 0, 9999);
	DDX_Text(pDX, IDC_EDIT_POLISH_COUNT, m_EditPolishCount);
	DDV_MinMaxInt(pDX, m_EditPolishCount, 0, 9999);
	DDX_Text(pDX, IDC_EDIT_ROUGH_COUNT, m_EditRoughCount);
	DDV_MinMaxInt(pDX, m_EditRoughCount, 0, 9999);
	DDX_Text(pDX, IDC_EDIT_PERFECT_SIZE, m_EditPerfectSize);
	DDV_MinMaxDouble(pDX, m_EditPerfectSize, 0., 9999.);
	DDX_Text(pDX, IDC_EDIT_POLISH_SIZE, m_EditPolishSize);
	DDV_MinMaxDouble(pDX, m_EditPolishSize, 0., 9999.);
	DDX_Text(pDX, IDC_EDIT_KNIFE_RADIUS, m_EditKnifeRadius);
	DDV_MinMaxDouble(pDX, m_EditKnifeRadius, 0., 9999.);
	DDX_Text(pDX, IDC_EDIT_ROUGH_SIZE, m_EditRoughSize);
	DDV_MinMaxDouble(pDX, m_EditRoughSize, 0., 9999.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgParamGlassGrind, CDialog)
	//{{AFX_MSG_MAP(CDlgParamGlassGrind)
	ON_BN_CLICKED(IDC_BTN_PARAM1_OK, OnBtnParam1Ok)
	ON_BN_CLICKED(IDC_BTN_PARAM1_CANCEL, OnBtnParam1Cancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParamGlassGrind message handlers

void CDlgParamGlassGrind::OnBtnParam1Ok() 
{
	// TODO: Add your control notification handler code here

	//参数保存
	if(!OnSaveParam())
		return;
	CDlgParamGlassGrind::OnOK();
}

void CDlgParamGlassGrind::OnBtnParam1Cancel() 
{
	// TODO: Add your control notification handler code here
	CDlgParamGlassGrind::OnCancel();
}

BOOL CDlgParamGlassGrind::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		SetWindowText("磨玻璃参数");	
	else
		SetWindowText("Glass grinding parameters");		
	OnInitParam();//读取参数
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
 *	参数保存
 */
BOOL CDlgParamGlassGrind::OnSaveParam()
{
	if(!UpdateData(TRUE)) //如果添的格式 不对
		return FALSE;
	//把参数给到g_pDoc的成员变量中去
	g_pDoc->m_stcGlassGrind.RouphCount = m_EditRoughCount;
	g_pDoc->m_stcGlassGrind.RouphSize = m_EditRoughSize;
	g_pDoc->m_stcGlassGrind.PerfectCount = m_EditPerfectCount;
	g_pDoc->m_stcGlassGrind.PerfectSize = m_EditPerfectSize;
	g_pDoc->m_stcGlassGrind.PolishCount = m_EditPolishCount;
	g_pDoc->m_stcGlassGrind.PolishSize = m_EditPolishSize;
//	g_pDoc->m_stcGlassGrind.KnifeRadius = m_EditKnifeRadius;
////////////////把参数保存到INI文件中//////////////////////////////
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "GlassGrind.ini";
	
	CString l_strDataName;			//保存用的中间数据一个是数据，一个是键名
	GetDlgItemText(IDC_EDIT_ROUGH_COUNT,l_strDataName);
	WritePrivateProfileString("Grind","RoughCount",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_ROUGH_SIZE,l_strDataName);
	WritePrivateProfileString("Grind","RoughSize",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_PERFECT_COUNT,l_strDataName);
	WritePrivateProfileString("Grind","PerfectCount",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_PERFECT_SIZE,l_strDataName);
	WritePrivateProfileString("Grind","PerfectSize",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_POLISH_COUNT,l_strDataName);
	WritePrivateProfileString("Grind","PolishCount",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_POLISH_SIZE,l_strDataName);
	WritePrivateProfileString("Grind","PolishSize",l_strDataName,l_strFlieName);

//	GetDlgItemText(IDC_EDIT_KNIFE_RADIUS,l_strDataName);
//	WritePrivateProfileString("Grind","KnifeRadius",l_strDataName,l_strFlieName);
	
	int l_iRanking = m_cmbDaoBu.GetCurSel();
	l_strDataName.Format("%d",l_iRanking);
	WritePrivateProfileString("Grind","Daobu",l_strDataName,l_strFlieName);
	g_pDoc->m_stcGlassGrind.DaoBuStyle = l_iRanking;

	l_iRanking = m_cmbDirClock1.GetCurSel();
	l_strDataName.Format("%d",l_iRanking);
	WritePrivateProfileString("Grind","iDirClock1",l_strDataName,l_strFlieName);
	g_pDoc->m_stcGlassGrind.iDirClock1 = l_iRanking;

	l_iRanking = m_cmbDirClock2.GetCurSel();
	l_strDataName.Format("%d",l_iRanking);
	WritePrivateProfileString("Grind","iDirClock2",l_strDataName,l_strFlieName);
	g_pDoc->m_stcGlassGrind.iDirClock2 = l_iRanking;
	
	l_iRanking = m_cmbDirClock3.GetCurSel();
	l_strDataName.Format("%d",l_iRanking);
	WritePrivateProfileString("Grind","iDirClock3",l_strDataName,l_strFlieName);
	g_pDoc->m_stcGlassGrind.iDirClock3 = l_iRanking;
	
	l_iRanking = m_cmbDirClock4.GetCurSel();
	l_strDataName.Format("%d",l_iRanking);
	WritePrivateProfileString("Grind","iDirClock4",l_strDataName,l_strFlieName);
	g_pDoc->m_stcGlassGrind.iDirClock4 = l_iRanking;
	
	l_iRanking = m_cmbDirClock5.GetCurSel();
	l_strDataName.Format("%d",l_iRanking);
	WritePrivateProfileString("Grind","iDirClock5",l_strDataName,l_strFlieName);
	g_pDoc->m_stcGlassGrind.iDirClock5 = l_iRanking;
	
	return TRUE;
}

/*
 *	读取保存
 */
BOOL CDlgParamGlassGrind::OnInitParam()
{
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "GlassGrind.ini";
	char l_chDataName[30]={0};
	GetPrivateProfileString("Grind","RoughCount","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_ROUGH_COUNT,l_chDataName);

	GetPrivateProfileString("Grind","RoughSize","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_ROUGH_SIZE,l_chDataName);

	GetPrivateProfileString("Grind","PerfectCount","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_PERFECT_COUNT,l_chDataName);
	
	GetPrivateProfileString("Grind","PerfectSize","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_PERFECT_SIZE,l_chDataName);

	GetPrivateProfileString("Grind","PolishCount","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_POLISH_COUNT,l_chDataName);
	
	GetPrivateProfileString("Grind","PolishSize","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_POLISH_SIZE,l_chDataName);

	GetPrivateProfileString("Grind","KnifeRadius","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_KNIFE_RADIUS,l_chDataName);	

	GetPrivateProfileString("Grind","Daobu","-1",l_chDataName,20,l_strFlieName);
	int l_iRanking = atoi(l_chDataName);
	m_cmbDaoBu.SetCurSel(l_iRanking);	

	m_cmbDirClock1.SetCurSel(g_pDoc->m_stcGlassGrind.iDirClock1);
	m_cmbDirClock2.SetCurSel(g_pDoc->m_stcGlassGrind.iDirClock2);
	m_cmbDirClock3.SetCurSel(g_pDoc->m_stcGlassGrind.iDirClock3);
	m_cmbDirClock4.SetCurSel(g_pDoc->m_stcGlassGrind.iDirClock4);
	m_cmbDirClock5.SetCurSel(g_pDoc->m_stcGlassGrind.iDirClock5);
	
	SetDlgItemText(IDC_STATIC_KNIFE1,g_pDoc->m_stcColorToM.m_ColorMean1);
	SetDlgItemText(IDC_STATIC_KNIFE2,g_pDoc->m_stcColorToM.m_ColorMean2);
	SetDlgItemText(IDC_STATIC_KNIFE3,g_pDoc->m_stcColorToM.m_ColorMean3);
	SetDlgItemText(IDC_STATIC_KNIFE4,g_pDoc->m_stcColorToM.m_ColorMean4);
	SetDlgItemText(IDC_STATIC_KNIFE5,g_pDoc->m_stcColorToM.m_ColorMean5);
	return TRUE;
}
