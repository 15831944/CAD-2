// DlgCommandView.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgCommandView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCommandView dialog


CDlgCommandView::CDlgCommandView(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CDlgCommandView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCommandView::DoDataExchange(CDataExchange* pDX)
{
//	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCommandView)
	DDX_Control(pDX, IDC_RICHEDIT4, m_RichEdit_Cmd);
	DDX_Control(pDX, IDC_EDIT_REMIND, m_ctlRemind);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgCommandView, CDialogBar)
	//{{AFX_MSG_MAP(CDlgCommandView)
	ON_MESSAGE(WM_INITDIALOG,OnInitDialog)
	ON_EN_CHANGE(IDC_RICHEDIT4, OnChangeRichedit4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCommandView message handlers




BOOL CDlgCommandView::OnInitDialog() 
{
//	CDialogBar::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (!UpdateData(FALSE))
		TRACE("初始化命令对话框有问题！");
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCommandView::OnChangeRichedit4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
