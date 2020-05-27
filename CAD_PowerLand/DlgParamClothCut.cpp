// DlgParamClothCut.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgParamClothCut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgParamClothCut dialog


CDlgParamClothCut::CDlgParamClothCut(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParamClothCut::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgParamClothCut)
	m_dEditOverCutAngle = 0.0;
	m_dEditOverCutLong = 0.0;
	m_dEditScaleX = 0.0;
	m_dEditScaleY = 0.0;
	m_dEditChamfer = 0.0;
	m_dEditSimplify = 0.0;
	m_dEditLessPointRadius = 0.0;
	m_dEditMoveOriginX = 0.0;
	m_dEditMoveOriginY = 0.0;
	m_dEditDashEmpty = 0.0;
	m_dEditDashSolid = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgParamClothCut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgParamClothCut)
	DDX_Control(pDX, IDC_COMBO_CHAMFER_END_STYLE, m_comboChamferEndStyle);
	DDX_Text(pDX, IDC_EDIT_OVERCUT_ANGLE, m_dEditOverCutAngle);
	DDV_MinMaxDouble(pDX, m_dEditOverCutAngle, 0., 180.);
	DDX_Text(pDX, IDC_EDIT_OVERCUT_LONG, m_dEditOverCutLong);
	DDV_MinMaxDouble(pDX, m_dEditOverCutLong, 0., 9999.);
	DDX_Text(pDX, IDC_EDIT_SCALE_X, m_dEditScaleX);
	DDX_Text(pDX, IDC_EDIT_SCALE_Y, m_dEditScaleY);
	DDX_Text(pDX, IDC_EDIT_CHAMFER, m_dEditChamfer);
	DDV_MinMaxDouble(pDX, m_dEditChamfer, 0., 50.);
	DDX_Text(pDX, IDC_EDIT_SIMPLIFY, m_dEditSimplify);
	DDV_MinMaxDouble(pDX, m_dEditSimplify, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_LESS_POINT_RADIUS, m_dEditLessPointRadius);
	DDX_Text(pDX, IDC_EDIT_MOVE_ORIGIN_X, m_dEditMoveOriginX);
	DDX_Text(pDX, IDC_EDIT_MOVE_ORIGIN_Y, m_dEditMoveOriginY);
	DDX_Text(pDX, IDC_EDIT_DASH_EMPTY, m_dEditDashEmpty);
	DDX_Text(pDX, IDC_EDIT_DASH_SOLID, m_dEditDashSolid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgParamClothCut, CDialog)
	//{{AFX_MSG_MAP(CDlgParamClothCut)
	ON_CBN_SELCHANGE(IDC_COMBO_CHAMFER_END_STYLE, OnSelchangeComboChamferEndStyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParamClothCut message handlers


BOOL CDlgParamClothCut::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
	{
		m_comboChamferEndStyle.InsertString(0,"圆头");
		m_comboChamferEndStyle.InsertString(1,"方头");		
	}
	else
	{
		m_comboChamferEndStyle.InsertString(0,"round");
		m_comboChamferEndStyle.InsertString(1,"rect");		
	}
	if (g_pDoc->m_stcClothCut.iChamferEndStyle == 0)
		m_comboChamferEndStyle.SetCurSel(0);
	else
		m_comboChamferEndStyle.SetCurSel(1);
	
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		SetWindowText("修改图形参数");	
	else
		SetWindowText("template parameter ");		
	OnInitParam();//读取参数
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgParamClothCut::OnOK() 
{
	// TODO: Add extra validation here
	//参数保存
	if(!OnSaveParam())
		return;	
	CDialog::OnOK();
}

void CDlgParamClothCut::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

/*
*	参数保存
*/
BOOL CDlgParamClothCut::OnSaveParam()
{
	if(!UpdateData(TRUE)) //如果添的格式 不对
		return FALSE;
	g_pDoc->m_stcClothCut.m_dOverCutAngle = m_dEditOverCutAngle;
	g_pDoc->m_stcClothCut.m_dOverCutLong = m_dEditOverCutLong;
	g_pDoc->m_stcClothCut.m_dScaleX = m_dEditScaleX;
	g_pDoc->m_stcClothCut.m_dScaleY = m_dEditScaleY;
	g_pDoc->m_stcClothCut.m_dChamfer = m_dEditChamfer;
	g_pDoc->m_stcClothCut.dSimplify = m_dEditSimplify;
	g_pDoc->m_stcClothCut.dLessPointRadius = m_dEditLessPointRadius;
	g_pDoc->m_stcClothCut.dMoveOriginX = m_dEditMoveOriginX;
	g_pDoc->m_stcClothCut.dMoveOriginY = m_dEditMoveOriginY;
	g_pDoc->m_stcClothCut.dEditDashEmpty = m_dEditDashEmpty;
	g_pDoc->m_stcClothCut.dEditDashSolid = m_dEditDashSolid;
	
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "ParamClothCut.ini";
	
	CString l_strDataName;			//保存用的中间数据一个是数据，一个是键名
	GetDlgItemText(IDC_EDIT_OVERCUT_ANGLE,l_strDataName);
	WritePrivateProfileString("OverCut","Angle",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_OVERCUT_LONG,l_strDataName);
	WritePrivateProfileString("OverCut","Long",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_SCALE_X,l_strDataName);
	WritePrivateProfileString("OverCut","ScaleX",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_SCALE_Y,l_strDataName);
	WritePrivateProfileString("OverCut","ScaleY",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_CHAMFER,l_strDataName);
	WritePrivateProfileString("OverCut","Chamfer",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_SIMPLIFY,l_strDataName);
	WritePrivateProfileString("OverCut","Simplify",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_LESS_POINT_RADIUS,l_strDataName);
	WritePrivateProfileString("OverCut","LessPointRadius",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_MOVE_ORIGIN_X,l_strDataName);
	WritePrivateProfileString("OverCut","dMoveOriginX",l_strDataName,l_strFlieName);	
	GetDlgItemText(IDC_EDIT_MOVE_ORIGIN_Y,l_strDataName);
	WritePrivateProfileString("OverCut","dMoveOriginY",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_DASH_SOLID,l_strDataName);
	WritePrivateProfileString("OverCut","dEditDashSolid",l_strDataName,l_strFlieName);	
	GetDlgItemText(IDC_EDIT_DASH_EMPTY,l_strDataName);
	WritePrivateProfileString("OverCut","dEditDashEmpty",l_strDataName,l_strFlieName);
	
	//存单选框 
	CButton *l_rdoBtn;
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_NEED_OVERCUT1);
	BOOL l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("OverCut","NeedOverCut1",l_strDataName,l_strFlieName);
	g_pDoc->m_stcClothCut.m_bOverCutKnife1 = l_bIsPicked;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_NEED_OVERCUT2);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("OverCut","NeedOverCut2",l_strDataName,l_strFlieName);
	g_pDoc->m_stcClothCut.m_bOverCutKnife2 = l_bIsPicked;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_NEED_OVERCUT3);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("OverCut","NeedOverCut3",l_strDataName,l_strFlieName);
	g_pDoc->m_stcClothCut.m_bOverCutKnife3 = l_bIsPicked;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_NEED_OVERCUT4);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("OverCut","NeedOverCut4",l_strDataName,l_strFlieName);
	g_pDoc->m_stcClothCut.m_bOverCutKnife4 = l_bIsPicked;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_NEED_OVERCUT5);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("OverCut","NeedOverCut5",l_strDataName,l_strFlieName);
	g_pDoc->m_stcClothCut.m_bOverCutKnife5 = l_bIsPicked;

	int l_iChamferEndStyle = m_comboChamferEndStyle.GetCurSel();//返回当前的颜色
	l_strDataName.Format("%d",l_iChamferEndStyle);
	WritePrivateProfileString("OverCut","ChamferEndStyle",l_strDataName,l_strFlieName);
	g_pDoc->m_stcClothCut.iChamferEndStyle = l_iChamferEndStyle;
	
	return TRUE;
}

/*
*	读取保存
*/
BOOL CDlgParamClothCut::OnInitParam()
{
	//读取M值
	CString l_str_DataName;
	l_str_DataName.Format("%.4f",g_pDoc->m_stcClothCut.m_dOverCutAngle);
	SetDlgItemText(IDC_EDIT_OVERCUT_ANGLE,l_str_DataName);
	l_str_DataName.Format("%.4f",g_pDoc->m_stcClothCut.m_dOverCutLong);
	SetDlgItemText(IDC_EDIT_OVERCUT_LONG,l_str_DataName);

	l_str_DataName.Format("%.4f",g_pDoc->m_stcClothCut.m_dScaleX);
	SetDlgItemText(IDC_EDIT_SCALE_X,l_str_DataName);
	l_str_DataName.Format("%.4f",g_pDoc->m_stcClothCut.m_dScaleY);
	SetDlgItemText(IDC_EDIT_SCALE_Y,l_str_DataName);
		
	l_str_DataName.Format("%.4f",g_pDoc->m_stcClothCut.m_dChamfer);
	SetDlgItemText(IDC_EDIT_CHAMFER,l_str_DataName);

	l_str_DataName.Format("%.4f",g_pDoc->m_stcClothCut.dSimplify);
	SetDlgItemText(IDC_EDIT_SIMPLIFY,l_str_DataName);

	l_str_DataName.Format("%.4f",g_pDoc->m_stcClothCut.dLessPointRadius);
	SetDlgItemText(IDC_EDIT_LESS_POINT_RADIUS,l_str_DataName);

	l_str_DataName.Format("%.4f",g_pDoc->m_stcClothCut.dMoveOriginX);
	SetDlgItemText(IDC_EDIT_MOVE_ORIGIN_X,l_str_DataName);
	l_str_DataName.Format("%.4f",g_pDoc->m_stcClothCut.dMoveOriginY);
	SetDlgItemText(IDC_EDIT_MOVE_ORIGIN_Y,l_str_DataName);
	
	l_str_DataName.Format("%.4f",g_pDoc->m_stcClothCut.dEditDashEmpty);
	SetDlgItemText(IDC_EDIT_DASH_EMPTY,l_str_DataName);
	l_str_DataName.Format("%.4f",g_pDoc->m_stcClothCut.dEditDashSolid);
	SetDlgItemText(IDC_EDIT_DASH_SOLID,l_str_DataName);

	//设定"需过切刀的类型"的名
	SetDlgItemText(IDC_CHECK_NEED_OVERCUT1,g_pDoc->m_stcColorToM.m_ColorMean1);
	SetDlgItemText(IDC_CHECK_NEED_OVERCUT2,g_pDoc->m_stcColorToM.m_ColorMean2);
	SetDlgItemText(IDC_CHECK_NEED_OVERCUT3,g_pDoc->m_stcColorToM.m_ColorMean3);
	SetDlgItemText(IDC_CHECK_NEED_OVERCUT4,g_pDoc->m_stcColorToM.m_ColorMean4);
	SetDlgItemText(IDC_CHECK_NEED_OVERCUT5,g_pDoc->m_stcColorToM.m_ColorMean5);
	//设定"需过切刀的类型"的单选框的状态	
	CButton *l_rdoBtn;
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_NEED_OVERCUT1);
	l_rdoBtn->SetCheck(g_pDoc->m_stcClothCut.m_bOverCutKnife1);
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_NEED_OVERCUT2);
	l_rdoBtn->SetCheck(g_pDoc->m_stcClothCut.m_bOverCutKnife2);
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_NEED_OVERCUT3);
	l_rdoBtn->SetCheck(g_pDoc->m_stcClothCut.m_bOverCutKnife3);
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_NEED_OVERCUT4);
	l_rdoBtn->SetCheck(g_pDoc->m_stcClothCut.m_bOverCutKnife4);
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_NEED_OVERCUT5);
	l_rdoBtn->SetCheck(g_pDoc->m_stcClothCut.m_bOverCutKnife5);
	return TRUE;
}


void CDlgParamClothCut::OnSelchangeComboChamferEndStyle() 
{
	// TODO: Add your control notification handler code here
	int l_iCurColor = m_comboChamferEndStyle.GetCurSel();//返回当前的颜色
	
}
