// DlgParamInoutKnife.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgParamInoutKnife.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgParamInoutKnife dialog


CDlgParamInoutKnife::CDlgParamInoutKnife(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParamInoutKnife::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgParamInoutKnife)
	m_EditIArcAngle = 0.0;
	m_EditIArcRadius = 0.0;
	m_EditIArcRadiusPercent = 0.0;
	m_EditILineLong = 0.0;
	m_EditILineLongPercent = 0.0;
	m_EditOArcAngle = 0.0;
	m_EditOArcRadius = 0.0;
	m_EditOArcRadiusPercent = 0.0;
	m_EditOLineLongPercent = 0.0;
	m_EditOLineLong = 0.0;
	m_EditOOverLong = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgParamInoutKnife::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgParamInoutKnife)
	DDX_Control(pDX, IDC_CHECK_IO_KNIFE_ISCENTER, m_IsCenterInput);
	DDX_Text(pDX, IDC_EDIT_IKNIFE_ARC_ANGLE, m_EditIArcAngle);
	DDV_MinMaxDouble(pDX, m_EditIArcAngle, 0., 360.);
	DDX_Text(pDX, IDC_EDIT_IKNIFE_ARC_RADIUS, m_EditIArcRadius);
	DDV_MinMaxDouble(pDX, m_EditIArcRadius, 0., 99999.);
	DDX_Text(pDX, IDC_EDIT_IKNIFE_ARC_RADIUSPERCENT, m_EditIArcRadiusPercent);
	DDV_MinMaxDouble(pDX, m_EditIArcRadiusPercent, 0., 99999.);
	DDX_Text(pDX, IDC_EDIT_IKNIFE_LINE_LONG, m_EditILineLong);
	DDV_MinMaxDouble(pDX, m_EditILineLong, 0., 99999.);
	DDX_Text(pDX, IDC_EDIT_IKNIFE_LINE_LONGPERCENT, m_EditILineLongPercent);
	DDV_MinMaxDouble(pDX, m_EditILineLongPercent, 0., 99999.);
	DDX_Text(pDX, IDC_EDIT_OKNIFE_ARC_ANGLE, m_EditOArcAngle);
	DDV_MinMaxDouble(pDX, m_EditOArcAngle, 0., 360.);
	DDX_Text(pDX, IDC_EDIT_OKNIFE_ARC_RADIUS, m_EditOArcRadius);
	DDV_MinMaxDouble(pDX, m_EditOArcRadius, 0., 99999.);
	DDX_Text(pDX, IDC_EDIT_OKNIFE_ARC_RADIUSPERCENT, m_EditOArcRadiusPercent);
	DDV_MinMaxDouble(pDX, m_EditOArcRadiusPercent, 0., 99999.);
	DDX_Text(pDX, IDC_EDIT_OKNIFE_LINE_LONGPERCENT, m_EditOLineLongPercent);
	DDV_MinMaxDouble(pDX, m_EditOLineLongPercent, 0., 99999.);
	DDX_Text(pDX, IDC_EDIT_OKNIFE_LINE_LONG, m_EditOLineLong);
	DDV_MinMaxDouble(pDX, m_EditOLineLong, 0., 99999.);
	DDX_Text(pDX, IDC_EDIT_OKNIFE_OVERLONG, m_EditOOverLong);
	DDV_MinMaxDouble(pDX, m_EditOOverLong, 0., 99999.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgParamInoutKnife, CDialog)
	//{{AFX_MSG_MAP(CDlgParamInoutKnife)
	ON_BN_CLICKED(IDC_BTN_SAVE_INOUT_KNIFE, OnBtnSaveInoutKnife)
	ON_BN_CLICKED(IDC_BTN_CANCLE_INOUT_KNIFE, OnBtnCancleInoutKnife)
	ON_BN_CLICKED(IDC_BTN_IOKNIFE_COPY, OnBtnIoknifeCopy)
	ON_EN_KILLFOCUS(IDC_EDIT_IKNIFE_LINE_LONGPERCENT, OnKillfocusEditIknifeLineLongpercent)
	ON_EN_KILLFOCUS(IDC_EDIT_IKNIFE_LINE_LONG, OnKillfocusEditIknifeLineLong)
	ON_EN_KILLFOCUS(IDC_EDIT_IKNIFE_ARC_RADIUSPERCENT, OnKillfocusEditIknifeArcRadiuspercent)
	ON_EN_KILLFOCUS(IDC_EDIT_IKNIFE_ARC_RADIUS, OnKillfocusEditIknifeArcRadius)
	ON_EN_KILLFOCUS(IDC_EDIT_OKNIFE_LINE_LONGPERCENT, OnKillfocusEditOknifeLineLongpercent)
	ON_EN_KILLFOCUS(IDC_EDIT_OKNIFE_LINE_LONG, OnKillfocusEditOknifeLineLong)
	ON_EN_KILLFOCUS(IDC_EDIT_OKNIFE_ARC_RADIUSPERCENT, OnKillfocusEditOknifeArcRadiuspercent)
	ON_EN_KILLFOCUS(IDC_EDIT_OKNIFE_ARC_RADIUS, OnKillfocusEditOknifeArcRadius)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParamInoutKnife message handlers

void CDlgParamInoutKnife::OnBtnSaveInoutKnife() 
{
	// TODO: Add your control notification handler code here
	//参数保存
	if(!OnSaveParam())
		return;	
	CDlgParamInoutKnife::OnOK();
}

void CDlgParamInoutKnife::OnBtnCancleInoutKnife() 
{
	// TODO: Add your control notification handler code here
	CDlgParamInoutKnife::OnCancel();
}

BOOL CDlgParamInoutKnife::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		SetWindowText("进退刀参数");	
	else
		SetWindowText("I/0 knife parameters");		
	OnInitParam();//读取参数	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*
*	参数保存
*/
BOOL CDlgParamInoutKnife::OnSaveParam()
{
	if(!UpdateData(TRUE)) //如果添的格式 不对
		return FALSE;
	//把值传给单文档
	g_pDoc->m_stcIOKnife.IArcAngle = m_EditIArcAngle;
	g_pDoc->m_stcIOKnife.IArcRadius = m_EditIArcRadius;
	g_pDoc->m_stcIOKnife.IArcRadiusPercent = m_EditIArcRadiusPercent;
	g_pDoc->m_stcIOKnife.ILineLong = m_EditILineLong;
	g_pDoc->m_stcIOKnife.ILineLongPercent = m_EditILineLongPercent;
	g_pDoc->m_stcIOKnife.OArcAngle = m_EditOArcAngle;
	g_pDoc->m_stcIOKnife.OArcRadius = m_EditOArcRadius;
	g_pDoc->m_stcIOKnife.OArcRadiusPercent = m_EditOArcRadiusPercent;
	g_pDoc->m_stcIOKnife.OLineLongPercent = m_EditOLineLongPercent;
	g_pDoc->m_stcIOKnife.OLineLong = m_EditOLineLong;
	g_pDoc->m_stcIOKnife.OOverLong = m_EditOOverLong;
				
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "GlassGrind.ini";
	
	CString l_strDataName;			//保存用的中间数据一个是数据，一个是键名
	GetDlgItemText(IDC_EDIT_IKNIFE_ARC_ANGLE,l_strDataName);
	WritePrivateProfileString("IOKnife","i_ArcAngle",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_IKNIFE_ARC_RADIUS,l_strDataName);
	WritePrivateProfileString("IOKnife","i_ArcRadius",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_IKNIFE_ARC_RADIUSPERCENT,l_strDataName);
	WritePrivateProfileString("IOKnife","i_ArcRadiusPercent",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_IKNIFE_LINE_LONG,l_strDataName);
	WritePrivateProfileString("IOKnife","i_LineLong",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_IKNIFE_LINE_LONGPERCENT,l_strDataName);
	WritePrivateProfileString("IOKnife","i_LineLongPercent",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_OKNIFE_ARC_ANGLE,l_strDataName);
	WritePrivateProfileString("IOKnife","o_ArcAngle",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_OKNIFE_ARC_RADIUS,l_strDataName);
	WritePrivateProfileString("IOKnife","o_ArcRadius",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_OKNIFE_ARC_RADIUSPERCENT,l_strDataName);
	WritePrivateProfileString("IOKnife","o_ArcRadiusPercent",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_OKNIFE_LINE_LONGPERCENT,l_strDataName);
	WritePrivateProfileString("IOKnife","o_LineLongPercent",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_OKNIFE_LINE_LONG,l_strDataName);
	WritePrivateProfileString("IOKnife","o_LineLong",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_OKNIFE_OVERLONG,l_strDataName);
	WritePrivateProfileString("IOKnife","o_OverLong",l_strDataName,l_strFlieName);
	
 	BOOL l_bIsPicked = m_IsCenterInput.GetCheck();//存是否在中心上进刀的参数
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("IOKnife","IsCenterInput",l_strDataName,l_strFlieName);
	g_pDoc->m_stcIOKnife.IsCenter = l_bIsPicked;
	//存单选框
	CButton *l_rdoBtn;
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_IKNIFE_LINE_VERTICAL);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("IOKnife","RadioILineVertical",l_strDataName,l_strFlieName);
	g_pDoc->m_stcIOKnife.IsIKnifeVertical = l_bIsPicked;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_IKNIFE_LINE_TANGENT);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("IOKnife","RadioILineTangent",l_strDataName,l_strFlieName);

	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_OKNIFE_LINE_VERTICAL);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("IOKnife","RadioOLineVertical",l_strDataName,l_strFlieName);
	g_pDoc->m_stcIOKnife.IsOknifeVertical = l_bIsPicked;
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_OKNIFE_LINE_TANGENT);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("IOKnife","RadioOLineTangent",l_strDataName,l_strFlieName);

	return TRUE;

}

/*
*	读取保存
*/
BOOL CDlgParamInoutKnife::OnInitParam()
{
	//
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "GlassGrind.ini";
	char l_chDataName[30]={0};

	GetPrivateProfileString("IOKnife","i_ArcAngle","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_IKNIFE_ARC_ANGLE,l_chDataName);
	
	GetPrivateProfileString("IOKnife","i_ArcRadius","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_IKNIFE_ARC_RADIUS,l_chDataName);
	
	GetPrivateProfileString("IOKnife","i_ArcRadiusPercent","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_IKNIFE_ARC_RADIUSPERCENT,l_chDataName);
	
	GetPrivateProfileString("IOKnife","i_LineLong","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_IKNIFE_LINE_LONG,l_chDataName);
	
	GetPrivateProfileString("IOKnife","i_LineLongPercent","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_IKNIFE_LINE_LONGPERCENT,l_chDataName);
	
	GetPrivateProfileString("IOKnife","o_ArcAngle","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_OKNIFE_ARC_ANGLE,l_chDataName);
	
	GetPrivateProfileString("IOKnife","o_ArcRadius","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_OKNIFE_ARC_RADIUS,l_chDataName);
	
	GetPrivateProfileString("IOKnife","o_ArcRadiusPercent","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_OKNIFE_ARC_RADIUSPERCENT,l_chDataName);
	
	GetPrivateProfileString("IOKnife","o_LineLongPercent","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_OKNIFE_LINE_LONGPERCENT,l_chDataName);
	
	GetPrivateProfileString("IOKnife","o_LineLong","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_OKNIFE_LINE_LONG,l_chDataName);

	GetPrivateProfileString("IOKnife","o_OverLong","-1",l_chDataName,20,l_strFlieName);
	SetDlgItemText(IDC_EDIT_OKNIFE_OVERLONG,l_chDataName);
	
	GetPrivateProfileString("IOKnife","IsCenterInput","-1",l_chDataName,20,l_strFlieName);
	int l_iIsCenterInput = atoi(l_chDataName);
	m_IsCenterInput.SetCheck(l_iIsCenterInput);

	//设定选框
	CButton *l_rdoBtn;
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_IKNIFE_LINE_VERTICAL);
	GetPrivateProfileString("IOKnife","RadioILineVertical","-1",l_chDataName,20,l_strFlieName);
	l_iIsCenterInput = atoi(l_chDataName);
	l_rdoBtn->SetCheck(l_iIsCenterInput);
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_IKNIFE_LINE_TANGENT);
	GetPrivateProfileString("IOKnife","RadioILineTangent","-1",l_chDataName,20,l_strFlieName);
	l_iIsCenterInput = atoi(l_chDataName);
	l_rdoBtn->SetCheck(l_iIsCenterInput);
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_OKNIFE_LINE_VERTICAL);
	GetPrivateProfileString("IOKnife","RadioOLineVertical","-1",l_chDataName,20,l_strFlieName);
	l_iIsCenterInput = atoi(l_chDataName);
	l_rdoBtn->SetCheck(l_iIsCenterInput);
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_OKNIFE_LINE_TANGENT);
	GetPrivateProfileString("IOKnife","RadioOLineTangent","-1",l_chDataName,20,l_strFlieName);
	l_iIsCenterInput = atoi(l_chDataName);
	l_rdoBtn->SetCheck(l_iIsCenterInput);
	
	return TRUE;
}

/*
 *	映射
 */
void CDlgParamInoutKnife::OnBtnIoknifeCopy() 
{
	// TODO: Add your control notification handler code here
	CString l_strValil;
	GetDlgItemText(IDC_EDIT_IKNIFE_LINE_LONG,l_strValil);
	SetDlgItemText(IDC_EDIT_OKNIFE_LINE_LONG,l_strValil);

	GetDlgItemText(IDC_EDIT_IKNIFE_LINE_LONGPERCENT,l_strValil);
	SetDlgItemText(IDC_EDIT_OKNIFE_LINE_LONGPERCENT,l_strValil);

	GetDlgItemText(IDC_EDIT_IKNIFE_ARC_RADIUS,l_strValil);
	SetDlgItemText(IDC_EDIT_OKNIFE_ARC_RADIUS,l_strValil);

	GetDlgItemText(IDC_EDIT_IKNIFE_ARC_RADIUSPERCENT,l_strValil);
	SetDlgItemText(IDC_EDIT_OKNIFE_ARC_RADIUSPERCENT,l_strValil);

	GetDlgItemText(IDC_EDIT_IKNIFE_ARC_ANGLE,l_strValil);
	SetDlgItemText(IDC_EDIT_OKNIFE_ARC_ANGLE,l_strValil);

	CButton *l_rdoBtn;
	int l_ivalil;
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_IKNIFE_LINE_VERTICAL);
	l_ivalil = l_rdoBtn->GetCheck();
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_OKNIFE_LINE_VERTICAL);
	l_rdoBtn->SetCheck(l_ivalil);

	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_IKNIFE_LINE_TANGENT);
	l_ivalil = l_rdoBtn->GetCheck();
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_OKNIFE_LINE_TANGENT);
	l_rdoBtn->SetCheck(l_ivalil);
}

/*
 *	当进刀编辑框失去焦点时
 */
//////////////////////////////////////////////////////////////////////////
void CDlgParamInoutKnife::OnKillfocusEditIknifeLineLongpercent() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
/*
	double l_dLineLong = m_EditILineLongPercent/100*g_pDoc->m_stcGlassGrind.KnifeRadius;
	CString l_strLineLong;
	l_strLineLong.Format("%.4f",l_dLineLong);
	SetDlgItemText(IDC_EDIT_IKNIFE_LINE_LONG,l_strLineLong);
*/
	
}

void CDlgParamInoutKnife::OnKillfocusEditIknifeLineLong() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
/*
	double l_dLineLong = m_EditILineLong/g_pDoc->m_stcGlassGrind.KnifeRadius*100;
	CString l_strLineLong;
	l_strLineLong.Format("%.4f",l_dLineLong);
	SetDlgItemText(IDC_EDIT_IKNIFE_LINE_LONGPERCENT,l_strLineLong);
*/
}

void CDlgParamInoutKnife::OnKillfocusEditIknifeArcRadiuspercent() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
/*
	double l_dLineLong = m_EditIArcRadiusPercent/100*g_pDoc->m_stcGlassGrind.KnifeRadius;
	CString l_strLineLong;
	l_strLineLong.Format("%.4f",l_dLineLong);
	SetDlgItemText(IDC_EDIT_IKNIFE_ARC_RADIUS,l_strLineLong);
*/
}

void CDlgParamInoutKnife::OnKillfocusEditIknifeArcRadius() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
/*
	double l_dLineLong = m_EditIArcRadius/g_pDoc->m_stcGlassGrind.KnifeRadius*100;
	CString l_strLineLong;
	l_strLineLong.Format("%.4f",l_dLineLong);
	SetDlgItemText(IDC_EDIT_IKNIFE_ARC_RADIUSPERCENT,l_strLineLong);	
*/
}

/*
 *	当退刀编辑框失去焦点时
 */
//////////////////////////////////////////////////////////////////////////
void CDlgParamInoutKnife::OnKillfocusEditOknifeLineLongpercent() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
/*
	double l_dLineLong = m_EditOLineLongPercent/100*g_pDoc->m_stcGlassGrind.KnifeRadius;
	CString l_strLineLong;
	l_strLineLong.Format("%.4f",l_dLineLong);
	SetDlgItemText(IDC_EDIT_OKNIFE_LINE_LONG,l_strLineLong);	
*/
}

void CDlgParamInoutKnife::OnKillfocusEditOknifeLineLong() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
/*
	double l_dLineLong = m_EditOLineLong/g_pDoc->m_stcGlassGrind.KnifeRadius*100;
	CString l_strLineLong;
	l_strLineLong.Format("%.4f",l_dLineLong);
	SetDlgItemText(IDC_EDIT_OKNIFE_LINE_LONGPERCENT,l_strLineLong);	
*/
}

void CDlgParamInoutKnife::OnKillfocusEditOknifeArcRadiuspercent() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
/*
	double l_dLineLong = m_EditOArcRadiusPercent/100*g_pDoc->m_stcGlassGrind.KnifeRadius;
	CString l_strLineLong;
	l_strLineLong.Format("%.4f",l_dLineLong);
	SetDlgItemText(IDC_EDIT_OKNIFE_ARC_RADIUS,l_strLineLong);	
*/
}

void CDlgParamInoutKnife::OnKillfocusEditOknifeArcRadius() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
/*
	double l_dLineLong = m_EditOArcRadius/g_pDoc->m_stcGlassGrind.KnifeRadius*100;
	CString l_strLineLong;
	l_strLineLong.Format("%.4f",l_dLineLong);
	SetDlgItemText(IDC_EDIT_OKNIFE_ARC_RADIUSPERCENT,l_strLineLong);	
*/
}
