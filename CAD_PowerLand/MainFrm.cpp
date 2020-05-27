// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CAD_PowerLand.h"


#include "MainFrm.h"
#include "Splash.h"
#include "HarddogInfo.h"//���ܹ�ͷ�ļ�


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocusEdit1)//EDIT��ʧȥ����ʱ���¼�
	ON_EN_KILLFOCUS(IDC_EDIT2, OnKillfocusEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT3, OnKillfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT4, OnKillfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT5, OnKillfocusEdit5)
	ON_EN_KILLFOCUS(IDC_EDIT6, OnKillfocusEdit6)
	ON_EN_KILLFOCUS(IDC_EDIT7, OnKillfocusEdit7)
	ON_EN_KILLFOCUS(IDC_EDIT8, OnKillfocusEdit8)
	ON_EN_SETFOCUS(IDC_EDIT1, OnSetfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT2, OnSetfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, OnSetfocusEdit3)
	ON_EN_SETFOCUS(IDC_EDIT4, OnSetfocusEdit4)
	ON_EN_SETFOCUS(IDC_EDIT5, OnSetfocusEdit5)
	ON_EN_SETFOCUS(IDC_EDIT6, OnSetfocusEdit6)
	ON_EN_SETFOCUS(IDC_EDIT7, OnSetfocusEdit7)
	ON_EN_SETFOCUS(IDC_EDIT8, OnSetfocusEdit8)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_BTN_FORWARD, OnBtnForward)
	ON_BN_CLICKED(IDC_BTN_BEHIND, OnBtnBehind)
	ON_BN_CLICKED(IDC_BTN_INSERT_BEFORE, OnBtnInsertBefore)
	ON_BN_CLICKED(IDC_BTN_INSERT_AFTER, OnBtnInsertAfter)
	ON_BN_CLICKED(IDC_BTN_CUTDIR, OnBtnCutdir)
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_WM_COPYDATA()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
		ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

//��ť
static UINT PropertyButtons[] = 
{
	ID_SHOW_NUM,	//0
	ID_SHOW_ARROW,	//1
	ID_POINT_END,	//2
	ID_POINT_NEAR,	//3
	ID_POINT_FOOT,	//4
	ID_POINT_MID,	//5
	ID_ORTHOGONAL,	//6
//	ID_INTERSECT,	//7
	ID_SEPARATOR,	//8
	ID_SEPARATOR,	//9
	ID_SEPARATOR,	//9
	ID_SEPARATOR,	//9
};
/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
:m_PerXPoint(0,0)
{
	// TODO: add member initialization code here
	m_iNumOfTop = 0;
	m_potMouseCurPos.x = 0;
	m_potMouseCurPos.y = 0;
	m_bSaveFile = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
//	//�����ܹ��Ƿ���ڣ��������ڣ��˳�
	BOOL bHarddogStatus = m_Harddog.HarddogCheck();
	if(bHarddogStatus)
		m_bSaveFile = TRUE;
/////////////////////////////////////////////////////////////////////////
	//	����������
	CDMCWin g_Controller;
//	CDMCWinRegistry DMCWinRegistry;//ע����
	//	GALILREGISTRY galilregistry;//ע����Ϣ��ר�����ݽṹ��
//	USHORT nNum=1;//���ؿ�����ע���
	// PCI�Ŀ���������Ҫ�жϣ�ֱ�Ӵ�
	// 	if (DMCWinRegistry.GetGalilRegistryInfo(nNum, &galilregistry) == DMCERROR_CONTROLLER)
	// 	{
	// 		//��ע����Ϣ
	// 		memset(&galilregistry,0,sizeof(galilregistry));
	// 		CHAR    Model[16]="DMC-1800";	
	// 		memcpy(galilregistry.szModel,Model,16);
	// 		galilregistry.ulTimeout = 5000;
	// 		galilregistry.usCommPort = 5000;
	// 		galilregistry.fControllerType = 4;//4:����  1������
	// 		CHAR    PNPHardwareKey[64] = "192.168.1.10";
	// 		memcpy(galilregistry.szPNPHardwareKey,PNPHardwareKey,64);	
	// 		DMCWinRegistry.AddGalilRegistry(&galilregistry,&nNum);
	// 	}
	
	
	if(g_Controller.Open(1) == 0)//�򿪿�����
		m_bSaveFile = TRUE;




//	if(!g_Controller.IsConnected())
//	{
//		Sleep(10);
//		MessageBox("�򿪿���������");
//		return -1;
//	}
//	if(!g_Controller.IsConnected())
//	{
//		CommunicationErrorMessage();
//		g_Controller.Close();
//		DMCWinRegistry.DeleteGalilRegistry(nNum);//ɾ��1ע����Ϣ
//	}
//	else		
/*	CString strVersion;
		char chResponese[80];
		g_Controller.Command("MG_BN", chResponese, sizeof(chResponese));
		if(atoi(chResponese)!=60505)
		{
			MessageBox("�������汾���ԣ����빩Ӧ����ϵ��","��ʾ",MB_OK);
			return -1;
		}
*/
//////////////////////////////////////////////////////////////////////////
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}


	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
		
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
//����ͼ����ɫ������
	if(!CreatePropertyBar())
		return -1;      // fail to create
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndPropBar.SetWindowText(_T("ͼ����ɫ������")) ;
	else
		m_wndPropBar.SetWindowText(_T("Color graphics toolbar")) ;
	m_wndPropBar.EnableDocking(CBRS_ALIGN_ANY) ;
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndPropBar,AFX_IDW_DOCKBAR_TOP); 	
//	DockControlBarLeftOf(&m_wndPropBar, &m_wndToolBar);//����ɫ�����ڹ������ұ�
//////////////////////////////////////////////////////////////////////////


//�������жԻ������ĺ���
	if (!m_DlgCommandBar.Create(this, IDD_DLG_COMMAND_VIEW, WS_CHILD |CBRS_SIZE_DYNAMIC|
		WS_VISIBLE | CBRS_BOTTOM, IDD_DLG_COMMAND_VIEW))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // failfd to create
	}
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_DlgCommandBar.SetWindowText(_T("�������")) ;
	else
		m_DlgCommandBar.SetWindowText(_T("Command toolbar")) ;
    m_DlgCommandBar.EnableDocking(CBRS_ALIGN_BOTTOM);
    DockControlBar(&m_DlgCommandBar,AFX_IDW_DOCKBAR_BOTTOM); 


//����ͼ�����ԶԻ���
	if (!m_DlgPropertyBar.Create(this, IDD_DLG_PROPERTY_DRAW, WS_CHILD | CBRS_SIZE_DYNAMIC|
		WS_VISIBLE | CBRS_LEFT, IDD_DLG_PROPERTY_DRAW))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_DlgPropertyBar.SetWindowText(_T("ͼ�����ԶԻ���")) ;
	else
		m_DlgPropertyBar.SetWindowText(_T("Graphic properties dialog")) ;
	m_DlgPropertyBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);	//ֻ�����ұߺ����
 	EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
    DockControlBar(&m_DlgPropertyBar); 
	//�������Կ�
	ShowControlBar(&m_DlgPropertyBar, TRUE, TRUE); 


/*
	if (!m_wndDialogBar.Create(this, IDD_DIALOG1, CBRS_TOP | CBRS_SIZE_DYNAMIC, IDD_DIALOG1))
	{
		TRACE0("Failed to create dialogbar/n");
		return -1;
	}
	m_wndDialogBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndDialogBar);
*/

//////////////////////////////////////////////////////////////////////////
	//������ͼ������
	CreateDrawToolBar();
	DockControlBar(&m_wndDrawToolBar,AFX_IDW_DOCKBAR_LEFT);
	//�����޸Ĺ�����
	CreateModifyToolBar();
	DockControlBar(&m_wndModifyToolBar,AFX_IDW_DOCKBAR_RIGHT);
	//������ͼ�İ�ť������
	CreateViewToolBar();
	DockControlBar(&m_wndViewBar,AFX_IDW_DOCKBAR_TOP);	
	DockControlBarLeftOf(&m_wndViewBar, &m_wndToolBar);//����ɫ�����ڹ������ұ�	
	//���������İ�ť������
	CreateParamToolBar();
	DockControlBar(&m_wndParamTollbar,AFX_IDW_DOCKBAR_TOP);	
	DockControlBarLeftOf(&m_wndParamTollbar, &m_wndViewBar);//����ɫ�����ڹ������ұ�	
	//����ģ�幤����
	CreateTemplateToolBar();
	DockControlBar(&m_wndTemplateToolBar,AFX_IDW_DOCKBAR_TOP);	
	DockControlBarLeftOf(&m_wndTemplateToolBar, &m_wndParamTollbar);//����ɫ�����ڹ������ұ�
	//��ĥ����������
	CreateGrindGlassToolBar();
	DockControlBar(&m_wndGrindGlassToolBar,AFX_IDW_DOCKBAR_RIGHT);	
	DockControlBarLeftOf(&m_wndGrindGlassToolBar, &m_wndTemplateToolBar);//����ɫ�����ڹ������ұ�
	//�������Կ�
//	ShowControlBar(&m_wndParamTollbar, TRUE, TRUE);

	// CG: The following line was added by the Splash Screen component.
//	CSplashWnd abc;
//	abc.ShowSplashScreen(this);
	CSplashWnd::ShowSplashScreen(this);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

/*
 *	������ͼ�Ĺ�����
 */
BOOL CMainFrame::CreateDrawToolBar()
{
		if (!m_wndDrawToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	    !m_wndDrawToolBar.LoadToolBar(IDR_DRAWTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndDrawToolBar.SetWindowText(_T("��ͼ������")) ;
	else
		m_wndDrawToolBar.SetWindowText(_T("Drawing Toolbar")) ;
	m_wndDrawToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
	return TRUE;
}

/*
 *	�����޸�ͼ�Ĺ�����
 */
BOOL CMainFrame::CreateModifyToolBar()
{
		if (!m_wndModifyToolBar.CreateEx(this, TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	    !m_wndModifyToolBar.LoadToolBar(IDR_MODIFYTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndModifyToolBar.SetWindowText(_T("�޸Ĺ�����")) ;
	else
		m_wndModifyToolBar.SetWindowText(_T("Modify toolbar ")) ;
	m_wndModifyToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);

	return TRUE;
}

/*
 *	����ģ���������
 */
BOOL CMainFrame::CreateTemplateToolBar()
{
	if (!m_wndTemplateToolBar.CreateEx(this, TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndTemplateToolBar.LoadToolBar(IDR_TEMPLATE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndTemplateToolBar.SetWindowText(_T("ģ�幤����")) ;
	else
		m_wndTemplateToolBar.SetWindowText(_T("Templete Tools")) ;
	m_wndTemplateToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);

	return TRUE;
}

/*
 *	����ĥ����������
 */
BOOL CMainFrame::CreateGrindGlassToolBar()
{
	if (!m_wndGrindGlassToolBar.CreateEx(this, TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndGrindGlassToolBar.LoadToolBar(IDR_GRIND_GLASS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndGrindGlassToolBar.SetWindowText(_T("ĥ����������")) ;
	else
		m_wndGrindGlassToolBar.SetWindowText(_T("Ground glass toolbar")) ;
	m_wndGrindGlassToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);

	return TRUE;
}

/*
 *	����������ť�Ĺ�����r
 */
BOOL CMainFrame::CreateParamToolBar()
{
	if (!m_wndParamTollbar.CreateEx(this, TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	    !m_wndParamTollbar.LoadToolBar(IDR_PARAMETER))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndParamTollbar.SetWindowText(_T("����������")) ;
	else
		m_wndParamTollbar.SetWindowText(_T("Parameters toolbar")) ;
	m_wndParamTollbar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
	return TRUE;
}

/*
 *	����������ť�Ĺ�����CreateViewToolBar
 */
BOOL CMainFrame::CreateViewToolBar()
{
	if (!m_wndViewBar.CreateEx(this, TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	    !m_wndViewBar.LoadToolBar(IDR_VIEW_BAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}		
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndViewBar.SetWindowText(_T("��ͼ������")) ;
	else
		m_wndViewBar.SetWindowText(_T("View toolbar")) ;
	m_wndViewBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
	return TRUE;
}


/*
 *	��������ɫ�ĺ���	
 */
BOOL CMainFrame::CreatePropertyBar()
{
	if(!m_wndPropBar.Create(this, WS_CHILD|WS_VISIBLE|CBRS_LEFT|
		CBRS_SIZE_DYNAMIC|CBRS_TOOLTIPS|CBRS_FLYBY, IDR_PROPERTYBAR)||
		!m_wndPropBar.LoadBitmap(IDR_PROPERTYBAR)
		||!m_wndPropBar.SetButtons(PropertyButtons,sizeof(PropertyButtons)/sizeof(UINT)))	
	{
		TRACE0("Failed to create propertybar\n") ;
		return FALSE ;
	}
	////������ɫ��Ͽ��                                  ���					
	m_wndPropBar.SetButtonInfo(8,IDW_COLOR, TBBS_SEPARATOR,150) ;

	/*������ɫ��Ͽ�*/
	CRect rc ;
	//�õ�������Ͽ�ľ��ο�
	m_wndPropBar.GetItemRect(8, &rc) ;//�趨��Ͽ���������
	const int nDropHeight = 130 ;		//��ɫ��Ͽ���������
	rc.bottom = rc.top + nDropHeight ;//
	//������ɫ��Ͽ�
	if(!m_wndPropBar.m_wndColorCmb.Create(
		CBS_DROPDOWNLIST|WS_VISIBLE|WS_TABSTOP|
		WS_VSCROLL|CBS_OWNERDRAWVARIABLE,
		rc, &m_wndPropBar,IDW_COLOR))
	{
		TRACE0("Failed to create combo--box!\n") ;
		return FALSE ;
	}
	/////////////////////////���ļ�ֵ��������ɫ�ģ�/////////////////////////////////////
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "ColorM.ini";
	char l_chDataName[20]={0};
	GetPrivateProfileString("WorkParam","Color1","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color1 = atoi(l_chDataName);
	GetPrivateProfileString("WorkParam","Color2","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color2 = atoi(l_chDataName);
	GetPrivateProfileString("WorkParam","Color3","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color3 = atoi(l_chDataName);
	GetPrivateProfileString("WorkParam","Color4","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color4 = atoi(l_chDataName);
	GetPrivateProfileString("WorkParam","Color5","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_color5 = atoi(l_chDataName);
	/////////////////////////���ļ�ֵ�����ڵ����ģ�/////////////////////////////////////
	GetPrivateProfileString("WorkParam","ColorMean1","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_ColorMean1 = l_chDataName;
	GetPrivateProfileString("WorkParam","ColorMean2","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_ColorMean2 = l_chDataName;
	GetPrivateProfileString("WorkParam","ColorMean3","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_ColorMean3 = l_chDataName;
	GetPrivateProfileString("WorkParam","ColorMean4","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_ColorMean4 = l_chDataName;
	GetPrivateProfileString("WorkParam","ColorMean5","-1",l_chDataName,20,l_strFlieName);
	g_pDoc->m_stcColorToM.m_ColorMean5 = l_chDataName;
	//////////////////////////////////////////////////////////////////////////	
	//����ɫ��Ͽ����ѡ��
	char l_chColorName[20]={0};
	strcpy(l_chColorName,g_pDoc->m_stcColorToM.m_ColorMean1);
	m_wndPropBar.m_wndColorCmb.AddItem(g_pDoc->m_stcColorToM.m_color1,l_chColorName) ;
	strcpy(l_chColorName,g_pDoc->m_stcColorToM.m_ColorMean2);
	m_wndPropBar.m_wndColorCmb.AddItem(g_pDoc->m_stcColorToM.m_color2,l_chColorName) ;
	strcpy(l_chColorName,g_pDoc->m_stcColorToM.m_ColorMean3);
	m_wndPropBar.m_wndColorCmb.AddItem(g_pDoc->m_stcColorToM.m_color3,l_chColorName) ;
	strcpy(l_chColorName,g_pDoc->m_stcColorToM.m_ColorMean4);
	m_wndPropBar.m_wndColorCmb.AddItem(g_pDoc->m_stcColorToM.m_color4,l_chColorName) ;
	strcpy(l_chColorName,g_pDoc->m_stcColorToM.m_ColorMean5);
	m_wndPropBar.m_wndColorCmb.AddItem(g_pDoc->m_stcColorToM.m_color5,l_chColorName) ;
	//���õ�ǰȱʡ��ѡ��---��ɫ
	m_wndPropBar.m_wndColorCmb.SetCurSel(0) ;

	return TRUE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((pMsg->wParam == VK_RETURN&&pMsg->message == WM_KEYDOWN)||(pMsg->wParam == VK_SPACE&&pMsg->message == WM_KEYDOWN))
	{
		g_pView->GetAndSendCommand();
		return TRUE;		//ʹ�ո�ͻس���������Ч
	}
	if(pMsg->wParam == VK_ESCAPE&&pMsg->message == WM_KEYDOWN)//�����ESC�����µĻ�
	{
		//���ѡ��
		g_pDoc->RemoveAllCollected();
		//���ѡ���ͼԪ��Ҫ������ʱ����һ��������;
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;
		Invalidate();
	}
	if(pMsg->wParam == VK_DELETE&&pMsg->message == WM_KEYDOWN)//�����ESC�����µĻ�
	{
		//���ѡ��
		g_pView->DeleteList();
		Invalidate();
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

/*
 *	���乤����������λ��
 */
void CMainFrame::DockControlBarLeftOf(CControlBar* Bar,CControlBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;
	
	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
	n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;
	
	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

/*
 *	1�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
 */
void CMainFrame::OnKillfocusEdit1() 
{
	g_pView->ClearSelectLine();//�����ѡ���µķ���	
	//��ѡ����ֻ��һ��Ԫ��
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CDC* pDC = g_pView->GetDC();
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT1,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	
	CString str;	
	if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
	{
		//��ת��CCreatePolyline���ͣ��������Աʱ��
		CCreatePolyline *Polyline = (CCreatePolyline*)pPropertCreate;
		//�������������ڷ�Χ��
		if (l_dEdit > Polyline->m_point.GetSize()||l_dEdit<1)
		{
			str.Format("%d",m_iNumOfTop);
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
			return;
		}
		//��ʾ��Ӧ���������
		Position pos = Polyline->m_point.GetAt((int)l_dEdit-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
		m_iNumOfTop = (int)l_dEdit;
	}
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreateSpline *pSpline = (CCreateSpline*)pPropertCreate;
		
		//�������������ڷ�Χ��
		if (l_dEdit > pSpline->m_aryControlPoints.GetSize()||l_dEdit<1)
		{	
			str.Format("%d",m_iNumOfTop);
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
			return;
		}
		//��ʾ��Ӧ���������
		Position pos = pSpline->m_aryControlPoints.GetAt((int)l_dEdit-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
		m_iNumOfTop = (int)l_dEdit;
	}
	
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		//��ת��CCreateEllipse���ͣ��������Աʱ��
		CCreateEllipse *Ellipse = (CCreateEllipse*)pPropertCreate;
		
		//�������������ڷ�Χ��
		if (l_dEdit > Ellipse->m_point.GetSize()||l_dEdit<1)
		{	
			str.Format("%d",m_iNumOfTop);
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
			return;
		}
		//��ʾ��Ӧ���������
		Position pos = Ellipse->m_point.GetAt((int)l_dEdit-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
		m_iNumOfTop = (int)l_dEdit;
	}
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		//��ת��CCreateEllipse���ͣ��������Աʱ��
		CCreateRectangle *Ellipse = (CCreateRectangle*)pPropertCreate;
		
		//�������������ڷ�Χ��
		if (l_dEdit > Ellipse->m_point.GetSize()||l_dEdit<1)
		{	
			str.Format("%d",m_iNumOfTop);
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
			return;
		}
		//��ʾ��Ӧ���������
		Position pos = Ellipse->m_point.GetAt((int)l_dEdit-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
		m_iNumOfTop = (int)l_dEdit;
	}
	else
	{
		//���û�иı����ݾͲ���
		if(fabs(l_dEdit - m_stcProperEdit.dEdit1) < DISTANCE_ZERO)
			return;
		pPropertCreate->ChangeProperty(l_dEdit,1);	
		pPropertCreate->AddUndoList(m_stcProperEdit.dEdit1,l_dEdit,1);
	}
	g_pView->ReleaseDC(pDC);
}

/*
 *	2�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
 */
void CMainFrame::OnKillfocusEdit2() 
{	
	// TODO: Add your control notification handler code here
	g_pView->ClearSelectLine();//�����ѡ���µķ���
	//��ѡ����ֻ��һ��Ԫ��
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT2,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//���û�иı����ݾͲ���
	if(fabs(l_dEdit - m_stcProperEdit.dEdit2) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,2,m_iNumOfTop);	
	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit2,l_dEdit,2,m_iNumOfTop);
	
}

/*
 *	3�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
 */
void CMainFrame::OnKillfocusEdit3() 
{
	g_pView->ClearSelectLine();//�����ѡ���µķ���
	//��ѡ����ֻ��һ��Ԫ��
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//���Ļ���Ӣ��
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT3,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//���û�иı����ݾͲ���
	if(fabs(l_dEdit - m_stcProperEdit.dEdit3) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,3,m_iNumOfTop);	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit3,l_dEdit,3,m_iNumOfTop);
}

/*
 *	4�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
 */
void CMainFrame::OnKillfocusEdit4() 
{
	g_pView->ClearSelectLine();//�����ѡ���µķ���
	//��ѡ����ֻ��һ��Ԫ��
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT4,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//���û�иı����ݾͲ���
	if(fabs(l_dEdit - m_stcProperEdit.dEdit4) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,4);	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit4,l_dEdit,4);
}

/*
 *	5�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
 */
void CMainFrame::OnKillfocusEdit5() 
{
	g_pView->ClearSelectLine();//�����ѡ���µķ���
	//��ѡ����ֻ��һ��Ԫ��
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC5,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC5,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT5,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//���û�иı����ݾͲ���
	if(fabs(l_dEdit - m_stcProperEdit.dEdit5) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,5);	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit5,l_dEdit,5);
}

/*
 *	6�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
 */
void CMainFrame::OnKillfocusEdit6() 
{
	g_pView->ClearSelectLine();//�����ѡ���µķ���
	//��ѡ����ֻ��һ��Ԫ��
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC6,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC6,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT6,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//���û�иı����ݾͲ���
	if(fabs(l_dEdit - m_stcProperEdit.dEdit6) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,6);	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit6,l_dEdit,6);
}

/*
 *	7�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
 */
void CMainFrame::OnKillfocusEdit7() 
{
	g_pView->ClearSelectLine();//�����ѡ���µķ���
	//��ѡ����ֻ��һ��Ԫ��
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC7,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC7,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT7,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//���û�иı����ݾͲ���
	if(fabs(l_dEdit - m_stcProperEdit.dEdit7) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,7);	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit7,l_dEdit,7);
}

/*
 *	8�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
 */
void CMainFrame::OnKillfocusEdit8() 
{
	g_pView->ClearSelectLine();//�����ѡ���µķ���
	//��ѡ����ֻ��һ��Ԫ��
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC8,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC8,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT8,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	pPropertCreate->ChangeProperty(l_dEdit,8);	
}

/*
 *	��ǰ����İ�Ť
 */
void CMainFrame::OnBtnForward() 
{
	g_pView->ClearSelectLine();//�����ѡ���µķ���
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CDC* pDC = g_pView->GetDC();
	CString str;	
	int l_iIsClose = 0;
	if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
	{
		//��ת��CCreatePolyline���ͣ��������Աʱ��
		CCreatePolyline *Polyline = (CCreatePolyline*)pPropertCreate;
		//�ö�������1����ʾ
		m_iNumOfTop--;
		//����Ǳպϵ�
		if (PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))			
			l_iIsClose = 1;
		if (m_iNumOfTop < 1)
			m_iNumOfTop = Polyline->m_point.GetSize()-l_iIsClose;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//��ʾ��Ӧ���������
		Position pos = Polyline->m_point.GetAt(m_iNumOfTop-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;	

		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreateSpline *pSpline = (CCreateSpline*)pPropertCreate;
		//�ö�������1����ʾ
		m_iNumOfTop--;
		//����Ǳպϵ�
		if (PosEqual(pSpline->m_aryControlPoints.GetAt(0), pSpline->m_aryControlPoints.GetAt(pSpline->m_aryControlPoints.GetSize()-1)))			
			l_iIsClose = 1;		
		if (m_iNumOfTop < 1)
			m_iNumOfTop = pSpline->m_aryControlPoints.GetSize()-l_iIsClose;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//��ʾ��Ӧ���������
		Position pos = pSpline->m_aryControlPoints.GetAt(m_iNumOfTop-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;	
		
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		//��ת��CCreateEllipse���ͣ��������Աʱ��
		CCreateEllipse *Ellipse = (CCreateEllipse*)pPropertCreate;
		//�ö�������1����ʾ
		m_iNumOfTop--;
		if (m_iNumOfTop < 1)
			m_iNumOfTop = Ellipse->m_point.GetSize()-1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//��ʾ��Ӧ���������
		Position pos = Ellipse->m_point.GetAt(m_iNumOfTop-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;	
		
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	//ȷ��ѡ�е����ĸ����ͼ������Ǿ��εĻ�
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{	
		//��ת��CCreateEllipse���ͣ��������Աʱ��
		CCreateRectangle *Rectangle = (CCreateRectangle*)pPropertCreate;
		
		//�ö�������1����ʾ
		m_iNumOfTop--;
		if (m_iNumOfTop < 1)
			m_iNumOfTop=Rectangle->m_point.GetSize()-1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//��ʾ��Ӧ���������
		Position pos = Rectangle->m_point.GetAt(m_iNumOfTop-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	g_pView->ReleaseDC(pDC);
}

/*
 *	��󷭵�İ�Ť
 */
void CMainFrame::OnBtnBehind() 
{
	g_pView->ClearSelectLine();//�����ѡ���µķ���
	//��ѡ����ֻ��һ��Ԫ��
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CDC* pDC = g_pView->GetDC();
	CString str;
	int l_iIsClose = 0;
	//ȷ��ѡ�е����ĸ����ͼ������Ƕ���ߵĻ�
	if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))	
	{
		//��ת��CCreatePolyline���ͣ��������Աʱ��
		CCreatePolyline *Polyline = (CCreatePolyline*)pPropertCreate;
		//�ö�������1����ʾ
		m_iNumOfTop++;
		//����Ǳպ�ͼ�Σ��׵��Ҫ����ĩ��
		if (PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))			
			l_iIsClose = 1;
		if (m_iNumOfTop > Polyline->m_point.GetSize()-l_iIsClose)
			m_iNumOfTop=1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//��ʾ��Ӧ���������
		Position pos = Polyline->m_point.GetAt(m_iNumOfTop-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;		
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{	
		CCreateSpline *pSpline = (CCreateSpline*)pPropertCreate;
		//�ö�������1����ʾ
		m_iNumOfTop++;
		//����Ǳպ�ͼ�Σ��׵��Ҫ����ĩ��
		if (PosEqual(pSpline->m_aryControlPoints.GetAt(0), pSpline->m_aryControlPoints.GetAt(pSpline->m_aryControlPoints.GetSize()-1)))			
			l_iIsClose = 1;
		if (m_iNumOfTop > pSpline->m_aryControlPoints.GetSize()-l_iIsClose)
			m_iNumOfTop=1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//��ʾ��Ӧ���������
		Position pos = pSpline->m_aryControlPoints.GetAt(m_iNumOfTop-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	//ȷ��ѡ�е����ĸ����ͼ���������Բ�Ļ�
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{	
		//��ת��CCreateEllipse���ͣ��������Աʱ��
		CCreateEllipse *Ellipse = (CCreateEllipse*)pPropertCreate;

		//�ö�������1����ʾ
		m_iNumOfTop++;
		if (m_iNumOfTop > Ellipse->m_point.GetSize()-1)
			m_iNumOfTop=1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//��ʾ��Ӧ���������
		Position pos = Ellipse->m_point.GetAt(m_iNumOfTop-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	//ȷ��ѡ�е����ĸ����ͼ������Ǿ��εĻ�
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{	
		//��ת��CCreateEllipse���ͣ��������Աʱ��
		CCreateRectangle *Rectangle = (CCreateRectangle*)pPropertCreate;
		
		//�ö�������1����ʾ
		m_iNumOfTop++;
		if (m_iNumOfTop > Rectangle->m_point.GetSize()-1)
			m_iNumOfTop=1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//��ʾ��Ӧ���������
		Position pos = Rectangle->m_point.GetAt(m_iNumOfTop-1);
		//�����߶��ϵĶ���ĵط���X,����ǰһ���X��û
		DrawPointX(pDC,pos);
		//ǰһ���X��������
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	g_pView->ReleaseDC(pDC);
}

/*
*	������ǰ����һ��
*/
void CMainFrame::OnBtnInsertBefore() 
{
	// TODO: Add your control notification handler code here
	///////////////////////����������Ŀ��Լ����ڵڼ�λ����˳ʱ��Ȼ����������� ��////////
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);	
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	POSITION l_ListPrePos;
	for (int i=0;l_ListPos != NULL;i++)//��������i�ɱ�ʾ������������
	{
		l_ListPrePos = l_ListPos;
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//�����M����
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(pPropertCreate == l_pDelList)//������������Ԫ��һ������Ҫ����λ��
		{
			if(i == 1)
				break;	//������������и�˳����0
			CString str;
			str.Format("%d",i-1);	//i��Ԫ���������е�˳���
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,str);//�ѵ�ǰ�������ʾ��
			l_ListPos = l_ListPrePos;
			g_pDoc->m_CCreateBaseList.GetPrev(l_ListPos);
			g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPrePos);//�Ѹý���Ƴ���
			g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pDelList);//Ȼ����������һ��

			//������������Ӳ���
			CUndoRedo l_UndoRedo;//������Ķ��� 
			PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
			pUndoList->OperatieWhat = IDC_BTN_INSERT_AFTER;
			
			pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
			pUndoList->pGraphPos->pCreateBaseposSpare1 = (long)l_pDelList;
			l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetAt(l_ListPos);
			pUndoList->pGraphPos->pCreateBaseposSpare2 = (long)l_pDelList;
			g_pDoc->m_Undo->AddUndoList(pUndoList);

			g_pView->Invalidate();
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////		
}

/*
*	����������һ��
*/
void CMainFrame::OnBtnInsertAfter() 
{
	// TODO: Add your control notification handler code here
	///////////////////////����������Ŀ��Լ����ڵڼ�λ����˳ʱ��Ȼ����������� ��////////
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"������Ч");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"Invalid");
		return;
	}
	//M����ĸ���
	int l_iNumOfMCode = 0;
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);	
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	while (l_ListPos)
	{
		pPropertCreate = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//�����M����
		if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			l_iNumOfMCode++;
	}
	pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);	
	l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();	
	POSITION l_ListPrePos;
	int l_iCountOfList = g_pDoc->m_CCreateBaseList.GetCount();	//��������ﹲ�ж��ٸ�Ԫ��
	for (int i=0;l_ListPos != NULL;i++)//��������i�ɱ�ʾ������������
	{
		l_ListPrePos = l_ListPos;
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//�����M����
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(pPropertCreate == l_pDelList)//������������Ԫ��һ������Ҫ����λ��
		{
			if(i == l_iCountOfList-l_iNumOfMCode-1)
				break;	//������������и�˳�򳬹�����Ԫ�ظ���
			CString str;
			str.Format("%d",i+1);	//i+1��Ԫ���������е�˳���
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,str);//�ѵ�ǰ�������ʾ��
			g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPrePos);//�Ѹý���Ƴ���
			g_pDoc->m_CCreateBaseList.InsertAfter(l_ListPos,l_pDelList);//Ȼ����������һ��

			//������������Ӳ���
			CUndoRedo l_UndoRedo;//������Ķ��� 
			PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
			pUndoList->OperatieWhat = IDC_BTN_INSERT_BEFORE;
			
			pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
			pUndoList->pGraphPos->pCreateBaseposSpare1 = (long)l_pDelList;
			l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetAt(l_ListPos);
			pUndoList->pGraphPos->pCreateBaseposSpare2 = (long)l_pDelList;
			g_pDoc->m_Undo->AddUndoList(pUndoList);
			g_pView->Invalidate();
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////		
}

/*
 *	�ڵ��ϻ���X,���߶�ʱ�������ĸ��㡣
 */
void CMainFrame::DrawPointX(CDC* pDC,Position pos)
{
	CPen	pen; 		
	pen.CreatePen(PS_SOLID,1,RGB(100,100,100));//
	CPen* pOldPen = pDC->SelectObject(&pen); // �õ�ԭ���Ļ���
	//��֮ǰ��X����
	CPoint SCreemPos;
	g_pView->WorldToScreem(m_PerXPoint,SCreemPos);
	CRect rect(SCreemPos.x-15,SCreemPos.y-15,SCreemPos.x+15,SCreemPos.y+15);
	g_pView->InvalidateRect(rect);
	//�����ڵ�X
	g_pView->WorldToScreem(pos,SCreemPos);
	pDC->MoveTo(SCreemPos.x-10,SCreemPos.y-10);
	pDC->LineTo(SCreemPos.x+10,SCreemPos.y+10);
	pDC->MoveTo(SCreemPos.x-10,SCreemPos.y+10);
	pDC->LineTo(SCreemPos.x+10,SCreemPos.y-10);

	// �ָ�ԭ���Ļ��� 
	pDC->SelectObject(pOldPen); 
	pen.DeleteObject();//�ͷ�pen��Create��;
}

/*
 *  �༭�ؼ�1�ı�ʱ����Ϣ����EDIT1�ﶥ��ʱ��	
 */
void CMainFrame::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

//�ı��и��
void CMainFrame::OnBtnCutdir() 
{
	// TODO: Add your control notification handler code here
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize() ;
	if (l_iSize == 0)
		return;
	CCreateBase* pSelEntity = NULL ;
	CUndoRedo l_UndoRedo;//������Ķ��� 
	
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = IDC_BTN_CUTDIR;	
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;
				
	for( int i = 0 ; i < l_iSize; i++ )
	{
		pSelEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i] ;

		if (i == 0)
			CurGraphPos->pCreateBasepos = (long)pSelEntity;
		else
		{
			GraphPos = new GRAPHPOS;
			memset(GraphPos,0,sizeof(GRAPHPOS));						
			GraphPos->pCreateBasepos = (long)pSelEntity;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		}							
		
		pSelEntity->ChangeDir();
	}
				
	g_pDoc->m_Undo->AddUndoList(pUndoList);
	g_pView->Invalidate();
}

BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	Position l_posMousePos;//����λ��
	pt = m_potMouseCurPos;
	if (zDelta > 0)              //zDelta >0������ǰ�����Ŵ�
	{
		//�ı����ϵ��
		if(g_pView->m_dProportion>1e-5)
		{
			//�ƶ�����ԭ��
			g_pView->ScreemToWorld(pt,l_posMousePos);
			double l_dPropor = 1.0/6.0;//Ϊ��1-1/1.2
			g_pView->m_dProportion = g_pView->m_dProportion /1.2; //ת���ı�����С		
			g_pView->m_dOriginX = (l_posMousePos.x-g_pView->m_dOriginX)*l_dPropor+g_pView->m_dOriginX;
			g_pView->m_dOriginY = (l_posMousePos.y-g_pView->m_dOriginY)*l_dPropor+g_pView->m_dOriginY;	
			g_pView->WorldToScreem(l_posMousePos,pt);
		}
	}
	if (zDelta < 0)               //zDelta < 0��������󻬣���С
	{
        
		if(g_pView->m_dProportion<18)
		{
			///�ƶ�����ԭ��
			g_pView->ScreemToWorld(pt,l_posMousePos);
			g_pView->m_dOriginX = (g_pView->m_dOriginX-l_posMousePos.x)*0.2+g_pView->m_dOriginX;
			g_pView->m_dOriginY = (g_pView->m_dOriginY-l_posMousePos.y)*0.2+g_pView->m_dOriginY;//	
			g_pView->m_dProportion = g_pView->m_dProportion *1.2;  //�ı����ϵ��
			
		}
		//		g_pDoc->SetModifiedFlag(TRUE);
	}
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);

	return CFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
}

/*
 *	�༭���ý���ʱ
 */
void CMainFrame::OnSetfocusEdit1() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT1,l_strEdit);
	m_stcProperEdit.dEdit1 = atof(l_strEdit);
}
/*
 *	�༭���ý���ʱ
 */
void CMainFrame::OnSetfocusEdit2() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT2,l_strEdit);
	m_stcProperEdit.dEdit2 = atof(l_strEdit);
	
}/*
 *	�༭���ý���ʱ
 */
void CMainFrame::OnSetfocusEdit3() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT3,l_strEdit);
	m_stcProperEdit.dEdit3 = atof(l_strEdit);
	
}/*
 *	�༭���ý���ʱ
 */
void CMainFrame::OnSetfocusEdit4() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT4,l_strEdit);
	m_stcProperEdit.dEdit4 = atof(l_strEdit);
	
}/*
 *	�༭���ý���ʱ
 */
void CMainFrame::OnSetfocusEdit5() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT5,l_strEdit);
	m_stcProperEdit.dEdit5 = atof(l_strEdit);
	
}/*
 *	�༭���ý���ʱ
 */
void CMainFrame::OnSetfocusEdit6() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT6,l_strEdit);
	m_stcProperEdit.dEdit6 = atof(l_strEdit);
	
}/*
 *	�༭���ý���ʱ
 */
void CMainFrame::OnSetfocusEdit7() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT7,l_strEdit);
	m_stcProperEdit.dEdit7 = atof(l_strEdit);
	
}/*
 *	�༭���ý���ʱ
 */
void CMainFrame::OnSetfocusEdit8() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT8,l_strEdit);
	m_stcProperEdit.dEdit8 = atof(l_strEdit);
	
}

void CMainFrame::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CPoint p;  
	GetCursorPos(&p);//�������Ļ����  
	CMenu m_Menu;  
	m_Menu.LoadMenu(IDR_MENU_VIEW);//�༭�õĲ˵���Դ  
	CMenu *m_SubMenu = m_Menu.GetSubMenu(0); 
	m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);  
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if (pCopyDataStruct->cbData > 0)
    {
		char str[80];
		strcpy(str,(char*)pCopyDataStruct->lpData);
		COpenGCode l_cGCode;
		l_cGCode.ReadGCode(str);//���ļ�����		
    }
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default		
	if(g_pDoc->m_Undo->m_pCurrList != NULL)
	{
		int l_iIsSave = AfxMessageBox("�Ƿ񱣴棡",MB_YESNOCANCEL ,MB_ICONQUESTION );
		if(l_iIsSave == IDCANCEL)//ȡ��
			return;
		else if(l_iIsSave == IDYES)//��
		{
			if(g_pDoc->OnCloseSaveFile())
				CFrameWnd::OnClose();
			else 
				return;
		}
		else if(l_iIsSave == IDNO)
			CFrameWnd::OnClose();
		return;
	}
	CFrameWnd::OnClose();		
}
