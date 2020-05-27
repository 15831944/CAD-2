// DlgAddRect.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgAddRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRect dialog


CDlgAddRect::CDlgAddRect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddRect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddRect)
	m_dEditLeft = 0.0;
	m_dEditBottom = 0.0;
	m_dEditRight = 0.0;
	m_dEditTop = 0.0;
	m_dEditRadius = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgAddRect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddRect)
	DDX_Text(pDX, IDC_EDIT_ADDRECT_LEFT, m_dEditLeft);
	DDV_MinMaxDouble(pDX, m_dEditLeft, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_ADDRECT_BOTTOM, m_dEditBottom);
	DDV_MinMaxDouble(pDX, m_dEditBottom, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_ADDRECT_RIGHT, m_dEditRight);
	DDV_MinMaxDouble(pDX, m_dEditRight, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_ADDRECT_TOP, m_dEditTop);
	DDV_MinMaxDouble(pDX, m_dEditTop, 0., 100.);
	DDX_Text(pDX, IDC_EDIT_ADDRECT_RADIUS, m_dEditRadius);
	DDV_MinMaxDouble(pDX, m_dEditRadius, 0., 100000.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddRect, CDialog)
	//{{AFX_MSG_MAP(CDlgAddRect)
	ON_BN_CLICKED(ID_ADDRECT_OK, OnAddrectOk)
	ON_BN_CLICKED(ID_ADDRECT_CANCEL, OnAddrectCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddRect message handlers

void CDlgAddRect::OnAddrectOk() 
{
	// TODO: Add your control notification handler code here

	if(!UpdateData(TRUE)) //�����ĸ�ʽ ����
		return;
	g_pDoc->m_stcClothCut.m_dAddRectBottom = m_dEditBottom;
	g_pDoc->m_stcClothCut.m_dAddRectLeft = m_dEditLeft;
	g_pDoc->m_stcClothCut.m_dAddRectRight = m_dEditRight;
	g_pDoc->m_stcClothCut.m_dAddRectTop = m_dEditTop;
	g_pDoc->m_stcClothCut.dAddRectRadius = m_dEditRadius;
	
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "ParamClothCut.ini";
	
	CString l_strDataName;			//�����õ��м�����һ�������ݣ�һ���Ǽ���
	GetDlgItemText(IDC_EDIT_ADDRECT_BOTTOM,l_strDataName);
	WritePrivateProfileString("OverCut","AddRectBottom",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_ADDRECT_LEFT,l_strDataName);
	WritePrivateProfileString("OverCut","AddRectLeft",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_ADDRECT_RIGHT,l_strDataName);
	WritePrivateProfileString("OverCut","AddRectRight",l_strDataName,l_strFlieName);
	
	GetDlgItemText(IDC_EDIT_ADDRECT_TOP,l_strDataName);
	WritePrivateProfileString("OverCut","AddRectTop",l_strDataName,l_strFlieName);

	GetDlgItemText(IDC_EDIT_ADDRECT_RADIUS,l_strDataName);
	WritePrivateProfileString("OverCut","AddRectRadius",l_strDataName,l_strFlieName);
	//�浥ѡ��
	CButton *l_rdoBtn;
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_LT);
	BOOL l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("OverCut","CheckChamferLT",l_strDataName,l_strFlieName);
	g_pDoc->m_stcClothCut.bChamferLT = l_bIsPicked;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_LB);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("OverCut","CheckChamferLB",l_strDataName,l_strFlieName);
	g_pDoc->m_stcClothCut.bChamferLB = l_bIsPicked;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_RT);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("OverCut","CheckChamferRT",l_strDataName,l_strFlieName);
	g_pDoc->m_stcClothCut.bChamferRT = l_bIsPicked;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_RB);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("OverCut","CheckChamferRB",l_strDataName,l_strFlieName);
	g_pDoc->m_stcClothCut.bChamferRB = l_bIsPicked;
	//���ѡ�񼯰�Χ�в���Ӿ���
	OnGetBoxAddRect();
	g_pDoc->RemoveAllCollected();


	CDialog::OnOK();
}

void CDlgAddRect::OnAddrectCancel() 
{
	// TODO: Add your control notification handler code here
	

	CDialog::OnCancel();
}

BOOL CDlgAddRect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_dEditBottom = g_pDoc->m_stcClothCut.m_dAddRectBottom;
	m_dEditLeft = g_pDoc->m_stcClothCut.m_dAddRectLeft;
	m_dEditRight = g_pDoc->m_stcClothCut.m_dAddRectRight;
	m_dEditTop = g_pDoc->m_stcClothCut.m_dAddRectTop;
	m_dEditRadius = g_pDoc->m_stcClothCut.dAddRectRadius;
	UpdateData(FALSE);

	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "ParamClothCut.ini";
	
	CString l_strDataName;			//�����õ��м�����һ�������ݣ�һ���Ǽ���
	//�趨ѡ��
	CButton *l_rdoBtn;
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_LT);
	l_rdoBtn->SetCheck(g_pDoc->m_stcClothCut.bChamferLT);
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_LB);
	l_rdoBtn->SetCheck(g_pDoc->m_stcClothCut.bChamferLB);
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_RT);
	l_rdoBtn->SetCheck(g_pDoc->m_stcClothCut.bChamferRT);
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_RB);
	l_rdoBtn->SetCheck(g_pDoc->m_stcClothCut.bChamferRB);
	
	

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*
 *	���ѡ�񼯰�Χ�в���Ӿ���
 */
void CDlgAddRect::OnGetBoxAddRect() 
{
	// TODO: Add your control notification handler code here
	if(g_pDoc->m_CollectOfPicked.GetSize() == 0)  
	{
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû��ѡ�У�����ѡ��",2);
		else
			RemindContent("\r\nNot selected, choose again��");			
		return;//����ǿռ�����Ҫ����
	}
	UpdateData();
	BOX2D pBox;
	Position l_posMin,l_posMax;	//ѡ�񼯵İ�Χ�е�������
	CArray<Position,Position&> m_point;
	CArray<double,double&> m_dTuDu;//���͹�ȵ�
	m_point.RemoveAll();
	m_dTuDu.RemoveAll();

	Position pos,posb(0,10),posc(10,10),pose(10,0);
	double aaa = g_pDoc->RtnTudu(posb,pose,posc,AD_COUNTERCLOCKWISE);
	double zoer = 0.0;
	/////////////////���ѡ�񼯵İ�Χ��//////////////////////////////
	g_pDoc->GetCollectBox(&pBox);
	l_posMin.x = pBox.min[0] - m_dEditLeft;
	l_posMin.y = pBox.min[1] - m_dEditBottom;
	l_posMax.x = pBox.max[0] + m_dEditRight;
	l_posMax.y = pBox.max[1] + m_dEditTop;
	//�����ǿ��ĸ�����Ҫ����
	CButton *l_rdoBtn;
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_LT);
	BOOL l_bIsPicked = l_rdoBtn->GetCheck();
	if (l_bIsPicked == TRUE)//���Ͻ�
	{
		pos.x = l_posMin.x + m_dEditRadius;
		pos.y = l_posMax.y;
		m_point.Add(pos);
		m_dTuDu.Add(aaa);	
		pos.x = l_posMin.x;
		pos.y = l_posMax.y - m_dEditRadius;
		m_point.Add(pos);
		m_dTuDu.Add(zoer);	
	}
	else
	{
		pos.x = l_posMin.x;
		pos.y = l_posMax.y;
		m_point.Add(pos);
		m_dTuDu.Add(zoer);
	}
	//���½�
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_LB);
	l_bIsPicked = l_rdoBtn->GetCheck();
	if (l_bIsPicked == TRUE)
	{
		pos.x = l_posMin.x;
		pos.y = l_posMin.y + m_dEditRadius;
		m_point.Add(pos);
		m_dTuDu.Add(aaa);	
		pos.x = l_posMin.x + m_dEditRadius;
		pos.y = l_posMin.y;
		m_point.Add(pos);
		m_dTuDu.Add(zoer);
	}
	else
	{
		pos.x = l_posMin.x;
		pos.y = l_posMin.y;
		m_point.Add(pos);
		m_dTuDu.Add(zoer);
	}
	//���½�
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_RB);
	l_bIsPicked = l_rdoBtn->GetCheck();
	if (l_bIsPicked == TRUE)
	{
		pos.x = l_posMax.x - m_dEditRadius;
		pos.y = l_posMin.y;
		m_point.Add(pos);
		m_dTuDu.Add(aaa);	
		pos.x = l_posMax.x;
		pos.y = l_posMin.y + m_dEditRadius;
		m_point.Add(pos);
		m_dTuDu.Add(zoer);		
	}
	else
	{
		pos.x = l_posMax.x;
		pos.y = l_posMin.y;
		m_point.Add(pos);
		m_dTuDu.Add(zoer);		
	}

	//���Ͻ�
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_RT);
	l_bIsPicked = l_rdoBtn->GetCheck();
	if (l_bIsPicked == TRUE)
	{
		pos.x = l_posMax.x;
		pos.y = l_posMax.y - m_dEditRadius;
		m_point.Add(pos);
		m_dTuDu.Add(aaa);
		pos.x = l_posMax.x - m_dEditRadius;
		pos.y = l_posMax.y;
		m_point.Add(pos);
		m_dTuDu.Add(zoer);		
	}
	else
	{
		pos.x = l_posMax.x;
		pos.y = l_posMax.y;
		m_point.Add(pos);
		m_dTuDu.Add(zoer);		
	}
	//���Ͻ�
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_CHAMFER_LT);
	l_bIsPicked = l_rdoBtn->GetCheck();
	if (l_bIsPicked == TRUE)
	{
		pos.x = l_posMin.x + m_dEditRadius;
		pos.y = l_posMax.y;
		m_point.Add(pos);
		m_dTuDu.Add(zoer);	
	}
	else
	{
		pos.x = l_posMin.x;
		pos.y = l_posMax.y;
		m_point.Add(pos);
		m_dTuDu.Add(zoer);
	}
	

	CCreatePolyline *pRect = new CCreatePolyline(m_point,m_dTuDu);
	pRect->m_ColorOfLine = g_CurColor;		//����ʱ�Ļ�ɫ���ǵ�ǰ��ѡɫ
	CDC *pdc = g_pView->GetDC();
	pRect->DrawMove(pdc,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
	g_pView->ReleaseDC(pdc);
	g_pDoc->m_CCreateBaseList.AddTail(pRect);

	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = (long)pRect;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}
