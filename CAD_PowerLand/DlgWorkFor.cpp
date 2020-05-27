// DlgWorkFor.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgWorkFor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgWorkFor dialog


CDlgWorkFor::CDlgWorkFor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWorkFor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgWorkFor)
	m_dMachineSizeX = 0.0;
	m_dMachineSizeY = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgWorkFor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgWorkFor)
	DDX_Text(pDX, IDC_EDIT_MACHINE_SIZEX, m_dMachineSizeX);
	DDX_Text(pDX, IDC_EDIT_MACHINE_SIZEY, m_dMachineSizeY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgWorkFor, CDialog)
	//{{AFX_MSG_MAP(CDlgWorkFor)
	ON_BN_CLICKED(IDC_BTN_SAVE_WORK, OnBtnSaveWork)
	ON_BN_CLICKED(IDC_BTN_CANCLE_WORK, OnBtnCancleWork)
	ON_BN_CLICKED(IDC_BTN_CHIOCE_FILEPATHG, OnBtnChioceFilepathg)
	ON_BN_CLICKED(IDC_BTN_CHIOCE_FILEPATHDXF, OnBtnChioceFilepathdxf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgWorkFor message handlers

/*
 *	���水ť
 */
void CDlgWorkFor::OnBtnSaveWork() 
{
	if(!UpdateData(TRUE)) //�����ĸ�ʽ ����
		return;
	// TODO: Add your control notification handler code here
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "WorkFor.ini";
	
	CString l_strDataName;			//�����õ��м�����һ�������ݣ�һ���Ǽ���
	//�浥ѡ�� 
	CButton *l_rdoBtn;
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_GLASSCUT);
	BOOL l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("WorkFor","IsGlassCut",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsGlassCut = l_bIsPicked;
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_GLASSGRIND);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("WorkFor","IsGlassGrind",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsGlassGrind = l_bIsPicked;
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_CLOTHCUT);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("WorkFor","IsClothCut",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsClothCut = l_bIsPicked;
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_LANGUAGE_CH);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("WorkFor","IsChinese",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsChinese = l_bIsPicked;	
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_ROTATESYMMETRY_SAVEOLD);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("WorkFor","RotateSymmetrySaveOld",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.RotateSymmetrySaveOld = l_bIsPicked;

	
	//���ļ�����·��
	GetDlgItemText(IDC_EDIT_FILE_SEND_PATHG,l_strDataName);
	WritePrivateProfileString("WorkFor","strFilePathG",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.strFilePathG = l_strDataName;
	GetDlgItemText(IDC_EDIT_FILE_SEND_PATHDXF,l_strDataName);
	WritePrivateProfileString("WorkFor","strFilePathDxf",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.strFilePathDxf = l_strDataName;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_FILE_SEND_NAME_DEF);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("WorkFor","bSaveDefault",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.bSaveDefault = l_bIsPicked;
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_FILE_SEND_STYLEG);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("WorkFor","bSendStyleG",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.bSendStyleG = l_bIsPicked;
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_FILE_SEND_STYLEDXF);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("WorkFor","bSendStyleDxf",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.bSendStyleDxf = l_bIsPicked;
	
	//��̨�ߴ�
	GetDlgItemText(IDC_EDIT_MACHINE_SIZEX,l_strDataName);
	WritePrivateProfileString("WorkFor","MachineSizeX",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.MachineX = m_dMachineSizeX;
	
	GetDlgItemText(IDC_EDIT_MACHINE_SIZEY,l_strDataName);
	WritePrivateProfileString("WorkFor","MachineSizeY",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.MachineY = m_dMachineSizeY;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_SHOW_MACHINE_SIZE);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("WorkFor","IsShowSize",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsShowSize = l_bIsPicked;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_SHOW_COORDINATE);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("WorkFor","IsShowCoordinate",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsShowCoordinate = l_bIsPicked;

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_SHOW_RULER_LINE);
	l_bIsPicked = l_rdoBtn->GetCheck();
	l_strDataName.Format("%d",l_bIsPicked);
	WritePrivateProfileString("WorkFor","IsShowRulerLine",l_strDataName,l_strFlieName);
	g_pDoc->m_stcWorkFor.IsShowRulerLine = l_bIsPicked;
	
	CDlgWorkFor::OnOK();
}

/*
 *	ȡ����ť
 */
void CDlgWorkFor::OnBtnCancleWork() 
{
	// TODO: Add your control notification handler code here

	CDlgWorkFor::OnCancel();
}

BOOL CDlgWorkFor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		SetWindowText("���ղ����Ի���");
	else
		SetWindowText("The process parameters");		
	OnInitParam();//��ȡ����

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
 *	��ȡ����
 */
BOOL CDlgWorkFor::OnInitParam()
{
	m_dMachineSizeX = g_pDoc->m_stcWorkFor.MachineX;
	m_dMachineSizeY = g_pDoc->m_stcWorkFor.MachineY;
	UpdateData(FALSE);
	
	CButton *l_rdoBtn;
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_GLASSCUT);
	l_rdoBtn->SetCheck(g_pDoc->m_stcWorkFor.IsGlassCut);
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_GLASSGRIND);
	l_rdoBtn->SetCheck(g_pDoc->m_stcWorkFor.IsGlassGrind);
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_CLOTHCUT);
	l_rdoBtn->SetCheck(g_pDoc->m_stcWorkFor.IsClothCut);

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_FILE_SEND_NAME_DEF);
	l_rdoBtn->SetCheck(g_pDoc->m_stcWorkFor.bSaveDefault);
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_FILE_SEND_STYLEG);
	l_rdoBtn->SetCheck(g_pDoc->m_stcWorkFor.bSendStyleG);
	l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_FILE_SEND_STYLEDXF);
	l_rdoBtn->SetCheck(g_pDoc->m_stcWorkFor.bSendStyleDxf);
	
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_SHOW_MACHINE_SIZE);
	l_rdoBtn->SetCheck(g_pDoc->m_stcWorkFor.IsShowSize);
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_SHOW_COORDINATE);
	l_rdoBtn->SetCheck(g_pDoc->m_stcWorkFor.IsShowCoordinate);
	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_SHOW_RULER_LINE);
	l_rdoBtn->SetCheck(g_pDoc->m_stcWorkFor.IsShowRulerLine);

	l_rdoBtn = (CButton*)GetDlgItem(IDC_CHECK_ROTATESYMMETRY_SAVEOLD);
	l_rdoBtn->SetCheck(g_pDoc->m_stcWorkFor.RotateSymmetrySaveOld);
	
	if (g_pDoc->m_stcWorkFor.IsChinese == TRUE)
	{
		l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_LANGUAGE_CH);
		l_rdoBtn->SetCheck(1);
		l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_LANGUAGE_EN);
		l_rdoBtn->SetCheck(0);
	}
	else
	{
		l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_LANGUAGE_CH);
		l_rdoBtn->SetCheck(0);
		l_rdoBtn = (CButton*)GetDlgItem(IDC_RADIO_LANGUAGE_EN);
		l_rdoBtn->SetCheck(1);		
	}

	SetDlgItemText(IDC_EDIT_FILE_SEND_PATHG,g_pDoc->m_stcWorkFor.strFilePathG);
	SetDlgItemText(IDC_EDIT_FILE_SEND_PATHDXF,g_pDoc->m_stcWorkFor.strFilePathDxf);
	

	return TRUE;
}

void CDlgWorkFor::OnBtnChioceFilepathg() 
{
	// TODO: Add your control notification handler code here
	CString strFilePath = _T("");//���������ļ�·��
	BROWSEINFO bi;
	char Buffer[512];
	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = "ѡ��·��";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;
	//��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)//ѡ��·��(��������ȷ����ť)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//ȡ���ļ���·����Buffer��
		strFilePath = Buffer;//��·��������һ��CString������
		SetDlgItemText(IDC_EDIT_FILE_SEND_PATHG,strFilePath);
	}
	else//ȡ��ѡ��
		return;
}

void CDlgWorkFor::OnBtnChioceFilepathdxf() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	CString strFilePath = _T("");//���������ļ�·��
	BROWSEINFO bi;
	char Buffer[512];
	//��ʼ����ڲ���bi��ʼ
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;//�˲�����ΪNULL������ʾ�Ի���
	bi.lpszTitle = "ѡ��·��";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.iImage = 0;
	//��ʼ����ڲ���bi����
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//������ʾѡ��Ի���
	if(pIDList)//ѡ��·��(��������ȷ����ť)
	{
		SHGetPathFromIDList(pIDList, Buffer);
		//ȡ���ļ���·����Buffer��
		strFilePath = Buffer;//��·��������һ��CString������
		SetDlgItemText(IDC_EDIT_FILE_SEND_PATHDXF,strFilePath);
	}
	else//ȡ��ѡ��
		return;
}
