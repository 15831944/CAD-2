// DlgSendFileName.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgSendFileName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSendFileName dialog


CDlgSendFileName::CDlgSendFileName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSendFileName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSendFileName)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSendFileName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSendFileName)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSendFileName, CDialog)
	//{{AFX_MSG_MAP(CDlgSendFileName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSendFileName message handlers

void CDlgSendFileName::OnOK() 
{
	// TODO: Add extra validation here
	CString l_strFileName;//文件的名字	
	GetDlgItemText(IDC_EDIT_SEND_NAME,l_strFileName);
	g_pDoc->m_stcWorkFor.FileName = l_strFileName;//文件名
	g_pDoc->m_stcWorkFor.IsSendFile = TRUE;//发送
	CDialog::OnOK();
}

void CDlgSendFileName::OnCancel() 
{
	// TODO: Add extra cleanup here
	g_pDoc->m_stcWorkFor.IsSendFile = FALSE;//发送
	
	CDialog::OnCancel();
}

BOOL CDlgSendFileName::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_EDIT_SEND_NAME,g_pDoc->m_stcWorkFor.FileName);
	GetDlgItem(IDC_EDIT_SEND_NAME)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
