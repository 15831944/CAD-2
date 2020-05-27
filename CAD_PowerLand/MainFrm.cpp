// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CAD_PowerLand.h"


#include "MainFrm.h"
#include "Splash.h"
#include "HarddogInfo.h"//加密狗头文件


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
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocusEdit1)//EDIT框失去焦点时的事件
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

//按钮
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
//	//检查加密狗是否存在，若不存在，退出
	BOOL bHarddogStatus = m_Harddog.HarddogCheck();
	if(bHarddogStatus)
		m_bSaveFile = TRUE;
/////////////////////////////////////////////////////////////////////////
	//	读控制器号
	CDMCWin g_Controller;
//	CDMCWinRegistry DMCWinRegistry;//注册类
	//	GALILREGISTRY galilregistry;//注册信息的专用数据结构体
//	USHORT nNum=1;//返回控制器注册号
	// PCI的控制器不需要判断，直接打开
	// 	if (DMCWinRegistry.GetGalilRegistryInfo(nNum, &galilregistry) == DMCERROR_CONTROLLER)
	// 	{
	// 		//附注册信息
	// 		memset(&galilregistry,0,sizeof(galilregistry));
	// 		CHAR    Model[16]="DMC-1800";	
	// 		memcpy(galilregistry.szModel,Model,16);
	// 		galilregistry.ulTimeout = 5000;
	// 		galilregistry.usCommPort = 5000;
	// 		galilregistry.fControllerType = 4;//4:网口  1：串口
	// 		CHAR    PNPHardwareKey[64] = "192.168.1.10";
	// 		memcpy(galilregistry.szPNPHardwareKey,PNPHardwareKey,64);	
	// 		DMCWinRegistry.AddGalilRegistry(&galilregistry,&nNum);
	// 	}
	
	
	if(g_Controller.Open(1) == 0)//打开控制器
		m_bSaveFile = TRUE;




//	if(!g_Controller.IsConnected())
//	{
//		Sleep(10);
//		MessageBox("打开控制器错误！");
//		return -1;
//	}
//	if(!g_Controller.IsConnected())
//	{
//		CommunicationErrorMessage();
//		g_Controller.Close();
//		DMCWinRegistry.DeleteGalilRegistry(nNum);//删除1注册信息
//	}
//	else		
/*	CString strVersion;
		char chResponese[80];
		g_Controller.Command("MG_BN", chResponese, sizeof(chResponese));
		if(atoi(chResponese)!=60505)
		{
			MessageBox("控制器版本不对，请与供应商联系！","提示",MB_OK);
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
//创建图形颜色工具栏
	if(!CreatePropertyBar())
		return -1;      // fail to create
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndPropBar.SetWindowText(_T("图形颜色工具栏")) ;
	else
		m_wndPropBar.SetWindowText(_T("Color graphics toolbar")) ;
	m_wndPropBar.EnableDocking(CBRS_ALIGN_ANY) ;
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndPropBar,AFX_IDW_DOCKBAR_TOP); 	
//	DockControlBarLeftOf(&m_wndPropBar, &m_wndToolBar);//让线色工具在工具栏右边
//////////////////////////////////////////////////////////////////////////


//建命令行对话框栏的函数
	if (!m_DlgCommandBar.Create(this, IDD_DLG_COMMAND_VIEW, WS_CHILD |CBRS_SIZE_DYNAMIC|
		WS_VISIBLE | CBRS_BOTTOM, IDD_DLG_COMMAND_VIEW))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // failfd to create
	}
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_DlgCommandBar.SetWindowText(_T("命令工具栏")) ;
	else
		m_DlgCommandBar.SetWindowText(_T("Command toolbar")) ;
    m_DlgCommandBar.EnableDocking(CBRS_ALIGN_BOTTOM);
    DockControlBar(&m_DlgCommandBar,AFX_IDW_DOCKBAR_BOTTOM); 


//建立图形属性对话框
	if (!m_DlgPropertyBar.Create(this, IDD_DLG_PROPERTY_DRAW, WS_CHILD | CBRS_SIZE_DYNAMIC|
		WS_VISIBLE | CBRS_LEFT, IDD_DLG_PROPERTY_DRAW))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_DlgPropertyBar.SetWindowText(_T("图形属性对话框")) ;
	else
		m_DlgPropertyBar.SetWindowText(_T("Graphic properties dialog")) ;
	m_DlgPropertyBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);	//只许靠在右边和左边
 	EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
    DockControlBar(&m_DlgPropertyBar); 
	//隐藏属性框
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
	//创建画图工具栏
	CreateDrawToolBar();
	DockControlBar(&m_wndDrawToolBar,AFX_IDW_DOCKBAR_LEFT);
	//创建修改工具栏
	CreateModifyToolBar();
	DockControlBar(&m_wndModifyToolBar,AFX_IDW_DOCKBAR_RIGHT);
	//创建视图的按钮工具栏
	CreateViewToolBar();
	DockControlBar(&m_wndViewBar,AFX_IDW_DOCKBAR_TOP);	
	DockControlBarLeftOf(&m_wndViewBar, &m_wndToolBar);//让线色工具在工具栏右边	
	//创建参数的按钮工具栏
	CreateParamToolBar();
	DockControlBar(&m_wndParamTollbar,AFX_IDW_DOCKBAR_TOP);	
	DockControlBarLeftOf(&m_wndParamTollbar, &m_wndViewBar);//让线色工具在工具栏右边	
	//创建模板工具栏
	CreateTemplateToolBar();
	DockControlBar(&m_wndTemplateToolBar,AFX_IDW_DOCKBAR_TOP);	
	DockControlBarLeftOf(&m_wndTemplateToolBar, &m_wndParamTollbar);//让线色工具在工具栏右边
	//创磨玻璃工具栏
	CreateGrindGlassToolBar();
	DockControlBar(&m_wndGrindGlassToolBar,AFX_IDW_DOCKBAR_RIGHT);	
	DockControlBarLeftOf(&m_wndGrindGlassToolBar, &m_wndTemplateToolBar);//让线色工具在工具栏右边
	//隐藏属性框
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
 *	创建画图的工具栏
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
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndDrawToolBar.SetWindowText(_T("画图工具栏")) ;
	else
		m_wndDrawToolBar.SetWindowText(_T("Drawing Toolbar")) ;
	m_wndDrawToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
	return TRUE;
}

/*
 *	创建修改图的工具栏
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
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndModifyToolBar.SetWindowText(_T("修改工具栏")) ;
	else
		m_wndModifyToolBar.SetWindowText(_T("Modify toolbar ")) ;
	m_wndModifyToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);

	return TRUE;
}

/*
 *	创建模板机工具栏
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
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndTemplateToolBar.SetWindowText(_T("模板工具栏")) ;
	else
		m_wndTemplateToolBar.SetWindowText(_T("Templete Tools")) ;
	m_wndTemplateToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);

	return TRUE;
}

/*
 *	创建磨玻璃工具栏
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
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndGrindGlassToolBar.SetWindowText(_T("磨玻璃工具栏")) ;
	else
		m_wndGrindGlassToolBar.SetWindowText(_T("Ground glass toolbar")) ;
	m_wndGrindGlassToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);

	return TRUE;
}

/*
 *	创建参数按钮的工具栏r
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
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndParamTollbar.SetWindowText(_T("参数工具栏")) ;
	else
		m_wndParamTollbar.SetWindowText(_T("Parameters toolbar")) ;
	m_wndParamTollbar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
	return TRUE;
}

/*
 *	创建参数按钮的工具栏CreateViewToolBar
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
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		m_wndViewBar.SetWindowText(_T("视图工具栏")) ;
	else
		m_wndViewBar.SetWindowText(_T("View toolbar")) ;
	m_wndViewBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
	return TRUE;
}


/*
 *	创建线颜色的函数	
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
	////设置颜色组合框的                                  宽度					
	m_wndPropBar.SetButtonInfo(8,IDW_COLOR, TBBS_SEPARATOR,150) ;

	/*绘制颜色组合框*/
	CRect rc ;
	//得到线型组合框的矩形框
	m_wndPropBar.GetItemRect(8, &rc) ;//设定组合框下拉长度
	const int nDropHeight = 130 ;		//线色组合框下拉长度
	rc.bottom = rc.top + nDropHeight ;//
	//创建颜色组合框
	if(!m_wndPropBar.m_wndColorCmb.Create(
		CBS_DROPDOWNLIST|WS_VISIBLE|WS_TABSTOP|
		WS_VSCROLL|CBS_OWNERDRAWVARIABLE,
		rc, &m_wndPropBar,IDW_COLOR))
	{
		TRACE0("Failed to create combo--box!\n") ;
		return FALSE ;
	}
	/////////////////////////读文件值（关于颜色的）/////////////////////////////////////
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
	/////////////////////////读文件值（关于刀名的）/////////////////////////////////////
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
	//在颜色组合框添加选项
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
	//设置当前缺省的选项---黑色
	m_wndPropBar.m_wndColorCmb.SetCurSel(0) ;

	return TRUE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ((pMsg->wParam == VK_RETURN&&pMsg->message == WM_KEYDOWN)||(pMsg->wParam == VK_SPACE&&pMsg->message == WM_KEYDOWN))
	{
		g_pView->GetAndSendCommand();
		return TRUE;		//使空格和回车里这里无效
	}
	if(pMsg->wParam == VK_ESCAPE&&pMsg->message == WM_KEYDOWN)//如果是ESC键按下的话
	{
		//清空选择集
		g_pDoc->RemoveAllCollected();
		//清空选择的图元，要不画的时候还有一个线蓝线;
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;
		Invalidate();
	}
	if(pMsg->wParam == VK_DELETE&&pMsg->message == WM_KEYDOWN)//如果是ESC键按下的话
	{
		//清空选择集
		g_pView->DeleteList();
		Invalidate();
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

/*
 *	分配工具栏的左右位置
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
 *	1当属性对话框的Edit控件失去焦点时的响应函数;
 */
void CMainFrame::OnKillfocusEdit1() 
{
	g_pView->ClearSelectLine();//清除框选留下的方框	
	//框选集里只有一个元素
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"操作无效");
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
		//先转成CCreatePolyline类型，下面调成员时用
		CCreatePolyline *Polyline = (CCreatePolyline*)pPropertCreate;
		//如果输入的数不在范围内
		if (l_dEdit > Polyline->m_point.GetSize()||l_dEdit<1)
		{
			str.Format("%d",m_iNumOfTop);
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
			return;
		}
		//显示对应顶点的坐标
		Position pos = Polyline->m_point.GetAt((int)l_dEdit-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
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
		
		//如果输入的数不在范围内
		if (l_dEdit > pSpline->m_aryControlPoints.GetSize()||l_dEdit<1)
		{	
			str.Format("%d",m_iNumOfTop);
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
			return;
		}
		//显示对应顶点的坐标
		Position pos = pSpline->m_aryControlPoints.GetAt((int)l_dEdit-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
		m_iNumOfTop = (int)l_dEdit;
	}
	
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		//先转成CCreateEllipse类型，下面调成员时用
		CCreateEllipse *Ellipse = (CCreateEllipse*)pPropertCreate;
		
		//如果输入的数不在范围内
		if (l_dEdit > Ellipse->m_point.GetSize()||l_dEdit<1)
		{	
			str.Format("%d",m_iNumOfTop);
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
			return;
		}
		//显示对应顶点的坐标
		Position pos = Ellipse->m_point.GetAt((int)l_dEdit-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
		m_iNumOfTop = (int)l_dEdit;
	}
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		//先转成CCreateEllipse类型，下面调成员时用
		CCreateRectangle *Ellipse = (CCreateRectangle*)pPropertCreate;
		
		//如果输入的数不在范围内
		if (l_dEdit > Ellipse->m_point.GetSize()||l_dEdit<1)
		{	
			str.Format("%d",m_iNumOfTop);
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
			return;
		}
		//显示对应顶点的坐标
		Position pos = Ellipse->m_point.GetAt((int)l_dEdit-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
		m_iNumOfTop = (int)l_dEdit;
	}
	else
	{
		//如果没有改变内容就不变
		if(fabs(l_dEdit - m_stcProperEdit.dEdit1) < DISTANCE_ZERO)
			return;
		pPropertCreate->ChangeProperty(l_dEdit,1);	
		pPropertCreate->AddUndoList(m_stcProperEdit.dEdit1,l_dEdit,1);
	}
	g_pView->ReleaseDC(pDC);
}

/*
 *	2当属性对话框的Edit控件失去焦点时的响应函数;
 */
void CMainFrame::OnKillfocusEdit2() 
{	
	// TODO: Add your control notification handler code here
	g_pView->ClearSelectLine();//清除框选留下的方框
	//框选集里只有一个元素
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"操作无效");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT2,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//如果没有改变内容就不变
	if(fabs(l_dEdit - m_stcProperEdit.dEdit2) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,2,m_iNumOfTop);	
	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit2,l_dEdit,2,m_iNumOfTop);
	
}

/*
 *	3当属性对话框的Edit控件失去焦点时的响应函数;
 */
void CMainFrame::OnKillfocusEdit3() 
{
	g_pView->ClearSelectLine();//清除框选留下的方框
	//框选集里只有一个元素
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//中文还是英文
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"操作无效");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT3,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//如果没有改变内容就不变
	if(fabs(l_dEdit - m_stcProperEdit.dEdit3) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,3,m_iNumOfTop);	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit3,l_dEdit,3,m_iNumOfTop);
}

/*
 *	4当属性对话框的Edit控件失去焦点时的响应函数;
 */
void CMainFrame::OnKillfocusEdit4() 
{
	g_pView->ClearSelectLine();//清除框选留下的方框
	//框选集里只有一个元素
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"操作无效");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT4,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//如果没有改变内容就不变
	if(fabs(l_dEdit - m_stcProperEdit.dEdit4) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,4);	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit4,l_dEdit,4);
}

/*
 *	5当属性对话框的Edit控件失去焦点时的响应函数;
 */
void CMainFrame::OnKillfocusEdit5() 
{
	g_pView->ClearSelectLine();//清除框选留下的方框
	//框选集里只有一个元素
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC5,"操作无效");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC5,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT5,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//如果没有改变内容就不变
	if(fabs(l_dEdit - m_stcProperEdit.dEdit5) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,5);	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit5,l_dEdit,5);
}

/*
 *	6当属性对话框的Edit控件失去焦点时的响应函数;
 */
void CMainFrame::OnKillfocusEdit6() 
{
	g_pView->ClearSelectLine();//清除框选留下的方框
	//框选集里只有一个元素
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC6,"操作无效");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC6,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT6,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//如果没有改变内容就不变
	if(fabs(l_dEdit - m_stcProperEdit.dEdit6) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,6);	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit6,l_dEdit,6);
}

/*
 *	7当属性对话框的Edit控件失去焦点时的响应函数;
 */
void CMainFrame::OnKillfocusEdit7() 
{
	g_pView->ClearSelectLine();//清除框选留下的方框
	//框选集里只有一个元素
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC7,"操作无效");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC7,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CString l_strEdit1;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT7,l_strEdit1);
	double l_dEdit = atof(l_strEdit1);
	//如果没有改变内容就不变
	if(fabs(l_dEdit - m_stcProperEdit.dEdit7) < DISTANCE_ZERO)
		return;
	pPropertCreate->ChangeProperty(l_dEdit,7);	
	pPropertCreate->AddUndoList(m_stcProperEdit.dEdit7,l_dEdit,7);
}

/*
 *	8当属性对话框的Edit控件失去焦点时的响应函数;
 */
void CMainFrame::OnKillfocusEdit8() 
{
	g_pView->ClearSelectLine();//清除框选留下的方框
	//框选集里只有一个元素
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC8,"操作无效");
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
 *	向前翻点的按扭
 */
void CMainFrame::OnBtnForward() 
{
	g_pView->ClearSelectLine();//清除框选留下的方框
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"操作无效");
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
		//先转成CCreatePolyline类型，下面调成员时用
		CCreatePolyline *Polyline = (CCreatePolyline*)pPropertCreate;
		//让顶点数加1并显示
		m_iNumOfTop--;
		//如果是闭合的
		if (PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))			
			l_iIsClose = 1;
		if (m_iNumOfTop < 1)
			m_iNumOfTop = Polyline->m_point.GetSize()-l_iIsClose;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//显示对应顶点的坐标
		Position pos = Polyline->m_point.GetAt(m_iNumOfTop-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
		m_PerXPoint = pos;	

		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreateSpline *pSpline = (CCreateSpline*)pPropertCreate;
		//让顶点数加1并显示
		m_iNumOfTop--;
		//如果是闭合的
		if (PosEqual(pSpline->m_aryControlPoints.GetAt(0), pSpline->m_aryControlPoints.GetAt(pSpline->m_aryControlPoints.GetSize()-1)))			
			l_iIsClose = 1;		
		if (m_iNumOfTop < 1)
			m_iNumOfTop = pSpline->m_aryControlPoints.GetSize()-l_iIsClose;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//显示对应顶点的坐标
		Position pos = pSpline->m_aryControlPoints.GetAt(m_iNumOfTop-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
		m_PerXPoint = pos;	
		
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		//先转成CCreateEllipse类型，下面调成员时用
		CCreateEllipse *Ellipse = (CCreateEllipse*)pPropertCreate;
		//让顶点数加1并显示
		m_iNumOfTop--;
		if (m_iNumOfTop < 1)
			m_iNumOfTop = Ellipse->m_point.GetSize()-1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//显示对应顶点的坐标
		Position pos = Ellipse->m_point.GetAt(m_iNumOfTop-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
		m_PerXPoint = pos;	
		
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	//确定选中的是哪个类的图，如果是矩形的话
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{	
		//先转成CCreateEllipse类型，下面调成员时用
		CCreateRectangle *Rectangle = (CCreateRectangle*)pPropertCreate;
		
		//让顶点数加1并显示
		m_iNumOfTop--;
		if (m_iNumOfTop < 1)
			m_iNumOfTop=Rectangle->m_point.GetSize()-1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//显示对应顶点的坐标
		Position pos = Rectangle->m_point.GetAt(m_iNumOfTop-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	g_pView->ReleaseDC(pDC);
}

/*
 *	向后翻点的按扭
 */
void CMainFrame::OnBtnBehind() 
{
	g_pView->ClearSelectLine();//清除框选留下的方框
	//框选集里只有一个元素
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"操作无效");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
	// TODO: Add your control notification handler code here
	CDC* pDC = g_pView->GetDC();
	CString str;
	int l_iIsClose = 0;
	//确定选中的是哪个类的图，如果是多段线的话
	if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))	
	{
		//先转成CCreatePolyline类型，下面调成员时用
		CCreatePolyline *Polyline = (CCreatePolyline*)pPropertCreate;
		//让顶点数加1并显示
		m_iNumOfTop++;
		//如果是闭合图形，首点就要等于末点
		if (PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))			
			l_iIsClose = 1;
		if (m_iNumOfTop > Polyline->m_point.GetSize()-l_iIsClose)
			m_iNumOfTop=1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//显示对应顶点的坐标
		Position pos = Polyline->m_point.GetAt(m_iNumOfTop-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
		m_PerXPoint = pos;		
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{	
		CCreateSpline *pSpline = (CCreateSpline*)pPropertCreate;
		//让顶点数加1并显示
		m_iNumOfTop++;
		//如果是闭合图形，首点就要等于末点
		if (PosEqual(pSpline->m_aryControlPoints.GetAt(0), pSpline->m_aryControlPoints.GetAt(pSpline->m_aryControlPoints.GetSize()-1)))			
			l_iIsClose = 1;
		if (m_iNumOfTop > pSpline->m_aryControlPoints.GetSize()-l_iIsClose)
			m_iNumOfTop=1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//显示对应顶点的坐标
		Position pos = pSpline->m_aryControlPoints.GetAt(m_iNumOfTop-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	//确定选中的是哪个类的图，如果是椭圆的话
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{	
		//先转成CCreateEllipse类型，下面调成员时用
		CCreateEllipse *Ellipse = (CCreateEllipse*)pPropertCreate;

		//让顶点数加1并显示
		m_iNumOfTop++;
		if (m_iNumOfTop > Ellipse->m_point.GetSize()-1)
			m_iNumOfTop=1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//显示对应顶点的坐标
		Position pos = Ellipse->m_point.GetAt(m_iNumOfTop-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	//确定选中的是哪个类的图，如果是矩形的话
	else if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{	
		//先转成CCreateEllipse类型，下面调成员时用
		CCreateRectangle *Rectangle = (CCreateRectangle*)pPropertCreate;
		
		//让顶点数加1并显示
		m_iNumOfTop++;
		if (m_iNumOfTop > Rectangle->m_point.GetSize()-1)
			m_iNumOfTop=1;
		str.Format("%d",m_iNumOfTop);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
		//显示对应顶点的坐标
		Position pos = Rectangle->m_point.GetAt(m_iNumOfTop-1);
		//画多线段上的顶点的地方的X,并把前一点的X画没
		DrawPointX(pDC,pos);
		//前一点的X保存走来
		m_PerXPoint = pos;
		str.Format("%.4f",pos.x);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
		str.Format("%.4f",pos.y);
		m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	}
	g_pView->ReleaseDC(pDC);
}

/*
*	链表向前插入一个
*/
void CMainFrame::OnBtnInsertBefore() 
{
	// TODO: Add your control notification handler code here
	///////////////////////遍历链表里的看自己排在第几位，刀顺时用然后输出到界面 上////////
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"操作无效");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"Invalid");
		return;
	}
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);	
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	POSITION l_ListPrePos;
	for (int i=0;l_ListPos != NULL;i++)//遍历链表，i可表示链表里的排序号
	{
		l_ListPrePos = l_ListPos;
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//如果是M代码
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(pPropertCreate == l_pDelList)//如果与链表里的元素一样，就要记下位置
		{
			if(i == 1)
				break;	//不让属性里的切割顺序变成0
			CString str;
			str.Format("%d",i-1);	//i是元素在链表中的顺序号
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,str);//把当前的序号显示上
			l_ListPos = l_ListPrePos;
			g_pDoc->m_CCreateBaseList.GetPrev(l_ListPos);
			g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPrePos);//把该结点移除，
			g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pDelList);//然后擦入向后移一下

			//向撤消链表里添加操作
			CUndoRedo l_UndoRedo;//撤消类的对象 
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
*	链表向后插入一个
*/
void CMainFrame::OnBtnInsertAfter() 
{
	// TODO: Add your control notification handler code here
	///////////////////////遍历链表里的看自己排在第几位，刀顺时用然后输出到界面 上////////
	if(g_pDoc->m_CollectOfPicked.GetSize() != 1)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"操作无效");
		else
			m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"Invalid");
		return;
	}
	//M代码的个数
	int l_iNumOfMCode = 0;
	CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);	
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	while (l_ListPos)
	{
		pPropertCreate = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//如果是M代码
		if (pPropertCreate->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			l_iNumOfMCode++;
	}
	pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);	
	l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();	
	POSITION l_ListPrePos;
	int l_iCountOfList = g_pDoc->m_CCreateBaseList.GetCount();	//获得链表里共有多少个元素
	for (int i=0;l_ListPos != NULL;i++)//遍历链表，i可表示链表里的排序号
	{
		l_ListPrePos = l_ListPos;
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//如果是M代码
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(pPropertCreate == l_pDelList)//如果与链表里的元素一样，就要记下位置
		{
			if(i == l_iCountOfList-l_iNumOfMCode-1)
				break;	//不让属性里的切割顺序超过最大的元素个数
			CString str;
			str.Format("%d",i+1);	//i+1是元素在链表中的顺序号
			m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,str);//把当前的序号显示上
			g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPrePos);//把该结点移除，
			g_pDoc->m_CCreateBaseList.InsertAfter(l_ListPos,l_pDelList);//然后擦入向后移一下

			//向撤消链表里添加操作
			CUndoRedo l_UndoRedo;//撤消类的对象 
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
 *	在点上画个X,多线段时标明是哪个点。
 */
void CMainFrame::DrawPointX(CDC* pDC,Position pos)
{
	CPen	pen; 		
	pen.CreatePen(PS_SOLID,1,RGB(100,100,100));//
	CPen* pOldPen = pDC->SelectObject(&pen); // 得到原来的画笔
	//把之前的X擦除
	CPoint SCreemPos;
	g_pView->WorldToScreem(m_PerXPoint,SCreemPos);
	CRect rect(SCreemPos.x-15,SCreemPos.y-15,SCreemPos.x+15,SCreemPos.y+15);
	g_pView->InvalidateRect(rect);
	//画现在的X
	g_pView->WorldToScreem(pos,SCreemPos);
	pDC->MoveTo(SCreemPos.x-10,SCreemPos.y-10);
	pDC->LineTo(SCreemPos.x+10,SCreemPos.y+10);
	pDC->MoveTo(SCreemPos.x-10,SCreemPos.y+10);
	pDC->LineTo(SCreemPos.x+10,SCreemPos.y-10);

	// 恢复原来的画笔 
	pDC->SelectObject(pOldPen); 
	pen.DeleteObject();//释放pen的Create的;
}

/*
 *  编辑控件1改变时的消息，当EDIT1里顶点时用	
 */
void CMainFrame::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogBar::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

//改变切割方向
void CMainFrame::OnBtnCutdir() 
{
	// TODO: Add your control notification handler code here
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize() ;
	if (l_iSize == 0)
		return;
	CCreateBase* pSelEntity = NULL ;
	CUndoRedo l_UndoRedo;//撤消类的对象 
	
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

	Position l_posMousePos;//鼠标的位置
	pt = m_potMouseCurPos;
	if (zDelta > 0)              //zDelta >0滑轮向前滑，放大
	{
		//改变比例系数
		if(g_pView->m_dProportion>1e-5)
		{
			//移动坐标原点
			g_pView->ScreemToWorld(pt,l_posMousePos);
			double l_dPropor = 1.0/6.0;//为：1-1/1.2
			g_pView->m_dProportion = g_pView->m_dProportion /1.2; //转换的比例变小		
			g_pView->m_dOriginX = (l_posMousePos.x-g_pView->m_dOriginX)*l_dPropor+g_pView->m_dOriginX;
			g_pView->m_dOriginY = (l_posMousePos.y-g_pView->m_dOriginY)*l_dPropor+g_pView->m_dOriginY;	
			g_pView->WorldToScreem(l_posMousePos,pt);
		}
	}
	if (zDelta < 0)               //zDelta < 0，滑轮向后滑，缩小
	{
        
		if(g_pView->m_dProportion<18)
		{
			///移动坐标原点
			g_pView->ScreemToWorld(pt,l_posMousePos);
			g_pView->m_dOriginX = (g_pView->m_dOriginX-l_posMousePos.x)*0.2+g_pView->m_dOriginX;
			g_pView->m_dOriginY = (g_pView->m_dOriginY-l_posMousePos.y)*0.2+g_pView->m_dOriginY;//	
			g_pView->m_dProportion = g_pView->m_dProportion *1.2;  //改变比例系数
			
		}
		//		g_pDoc->SetModifiedFlag(TRUE);
	}
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);

	return CFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
}

/*
 *	编辑框获得焦点时
 */
void CMainFrame::OnSetfocusEdit1() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT1,l_strEdit);
	m_stcProperEdit.dEdit1 = atof(l_strEdit);
}
/*
 *	编辑框获得焦点时
 */
void CMainFrame::OnSetfocusEdit2() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT2,l_strEdit);
	m_stcProperEdit.dEdit2 = atof(l_strEdit);
	
}/*
 *	编辑框获得焦点时
 */
void CMainFrame::OnSetfocusEdit3() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT3,l_strEdit);
	m_stcProperEdit.dEdit3 = atof(l_strEdit);
	
}/*
 *	编辑框获得焦点时
 */
void CMainFrame::OnSetfocusEdit4() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT4,l_strEdit);
	m_stcProperEdit.dEdit4 = atof(l_strEdit);
	
}/*
 *	编辑框获得焦点时
 */
void CMainFrame::OnSetfocusEdit5() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT5,l_strEdit);
	m_stcProperEdit.dEdit5 = atof(l_strEdit);
	
}/*
 *	编辑框获得焦点时
 */
void CMainFrame::OnSetfocusEdit6() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT6,l_strEdit);
	m_stcProperEdit.dEdit6 = atof(l_strEdit);
	
}/*
 *	编辑框获得焦点时
 */
void CMainFrame::OnSetfocusEdit7() 
{
	// TODO: Add your control notification handler code here
	CString l_strEdit;
	m_DlgPropertyBar.GetDlgItemText(IDC_EDIT7,l_strEdit);
	m_stcProperEdit.dEdit7 = atof(l_strEdit);
	
}/*
 *	编辑框获得焦点时
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
	GetCursorPos(&p);//鼠标点的屏幕坐标  
	CMenu m_Menu;  
	m_Menu.LoadMenu(IDR_MENU_VIEW);//编辑好的菜单资源  
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
		l_cGCode.ReadGCode(str);//打开文件函数		
    }
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default		
	if(g_pDoc->m_Undo->m_pCurrList != NULL)
	{
		int l_iIsSave = AfxMessageBox("是否保存！",MB_YESNOCANCEL ,MB_ICONQUESTION );
		if(l_iIsSave == IDCANCEL)//取消
			return;
		else if(l_iIsSave == IDYES)//是
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
