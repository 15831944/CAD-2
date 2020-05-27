// CAD_PowerLandView.cpp : implementation of the CCAD_PowerLandView class
//

#include "stdafx.h"
#include "CAD_PowerLand.h"

#include "CAD_PowerLandDoc.h"
#include "CAD_PowerLandView.h"
#include "Draw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandView

IMPLEMENT_DYNCREATE(CCAD_PowerLandView, CView)

BEGIN_MESSAGE_MAP(CCAD_PowerLandView, CView)
	//{{AFX_MSG_MAP(CCAD_PowerLandView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_COPYDATA()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	//画图元
	ON_COMMAND_RANGE(ID_LINE,ID_ELLIPSE,OnButtonDraw)
	//修改图元
	ON_COMMAND_RANGE(ID_DELETE,ID_MATRIX,OnButtonFix)
	ON_UPDATE_COMMAND_UI_RANGE(ID_DELETE,ID_MATRIX,OnUpdateButtonFix)
	//参数设置
	ON_COMMAND_RANGE(ID_PARAM_IN_OUT_GRAPH,ID_PROPERTY,OnButtonParam)
	//窗口显示
	ON_COMMAND_RANGE(ID_VIEW_COLOR,ID_VIEW_COMMAND,OnButtonShowBar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_COLOR,ID_VIEW_COMMAND,OnUpdateButtonShowBar)
	//开关按键
	ON_COMMAND_RANGE(ID_SHOW_ARROW,ID_SHOW_NUM,OnButtonSwitch)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SHOW_ARROW,ID_SHOW_NUM,OnUpdateButtonSwitch)
	//翻转菜单
	ON_COMMAND_RANGE(ID_ROTATE_180,ID_ROTATE_ANGLE,OnButtonRotateSymmetry)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandView construction/destruction

CCAD_PowerLandView::CCAD_PowerLandView()
{
	// TODO: add construction code here
	g_pView = this;	
	m_dOriginX = m_dOriginY = 0;
	m_dProportion = 1;
	m_pDrawBase = NULL;
	m_iFlagWhichBtnDown = MEIYOU;
	m_bFlagMBtnMove = FALSE;	//鼠标中键按下
	m_dRulerLineX = 1000;//刻度尺初始的值
	m_dRulerLineY = 800;
	m_bBreakThreadOne = TRUE;
	m_iLastFuntion = 0;

	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "View.ini";
	char l_chDataName[80]={0};
	GetPrivateProfileString("View","bShowNum","-1",l_chDataName,20,l_strFlieName);//显示数字按钮
	m_stcSwitch.bShowNum = atoi(l_chDataName);
	GetPrivateProfileString("View","bShowArrow","-1",l_chDataName,20,l_strFlieName);//显示箭头按钮
	m_stcSwitch.bShowArrow = atoi(l_chDataName);
	GetPrivateProfileString("View","Catch","-1",l_chDataName,20,l_strFlieName);
	m_iSwitch = atoi(l_chDataName);
}

CCAD_PowerLandView::~CCAD_PowerLandView()
{
	//操作到一半时就关软件
	if (m_pDrawBase)
		delete m_pDrawBase;


	//保存参数
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "View.ini";
	
	CString l_strDataName;			//保存用的中间数据一个是数据，一个是键名
	l_strDataName.Format("%d",m_stcSwitch.bShowNum);
	WritePrivateProfileString("View","bShowNum",l_strDataName,l_strFlieName);
	l_strDataName.Format("%d",m_stcSwitch.bShowArrow);
	WritePrivateProfileString("View","bShowArrow",l_strDataName,l_strFlieName);
	
	l_strDataName.Format("%d",m_iSwitch);
	WritePrivateProfileString("View","Catch",l_strDataName,l_strFlieName);
}

BOOL CCAD_PowerLandView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

//以下是使背变黑色的
/*
	if(CView::PreCreateWindow(cs) == FALSE)
	{
		return FALSE;
	}
	
	WNDCLASS WndClass = {0};
	HINSTANCE hInstance = AfxGetInstanceHandle();
	LPCSTR szClassName = "MYViewClass";
	
	if (!GetClassInfo(hInstance,szClassName, &WndClass))
	{
		GetClassInfo(hInstance, cs.lpszClass, &WndClass);
		WndClass.style &= ~(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS);
		WndClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		WndClass.hbrBackground = CreateSolidBrush(RGB(0,0,0));//主要是这句
		WndClass.hIcon = NULL;
		WndClass.lpszClassName = szClassName;
		
		if (!AfxRegisterClass(&WndClass))
		{
			AfxThrowResourceException();
		}
	}
	
	cs.lpszClass = szClassName;
	
	return TRUE;
*/

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandView drawing

void CCAD_PowerLandView::OnDraw(CDC* pDC)
{
	CCAD_PowerLandDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	DrawCoord(pDC);	//画坐标系函数
/*
	//把背景刷成黑色，不用这个，在PreCreateWindow(CREATESTRUCT& cs)里会好点
	CRect rect; 
	this->GetClientRect(rect); 
	pDC->FillSolidRect(&rect,RGB(0,0,0));
*/

	//////////////////////////////////////////////////////////////////////////
	g_refresh = 0;		//
	g_pDoc->Draw(pDC);		//刷新时把存起来的图元指针都画一下。

	
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandView printing

BOOL CCAD_PowerLandView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCAD_PowerLandView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCAD_PowerLandView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandView diagnostics

#ifdef _DEBUG
void CCAD_PowerLandView::AssertValid() const
{
	CView::AssertValid();
}

void CCAD_PowerLandView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCAD_PowerLandDoc* CCAD_PowerLandView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCAD_PowerLandDoc)));
	return (CCAD_PowerLandDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCAD_PowerLandView message handlers



void CCAD_PowerLandView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CCAD_PowerLandDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	Position pos;
	ScreemToWorld(point,pos);

	if (m_pDrawBase != NULL)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));	//改变鼠标形状	
		m_pDrawBase->OnLButtonDown(nFlags,pos,"");	//1为鼠标点击，2为键盘点击
	}
	else
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_SELECT));//改变鼠标形状
		g_pDoc->OnLButtonDown(nFlags,point);
	}


	CView::OnLButtonDown(nFlags, point);
}

void CCAD_PowerLandView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//改变鼠标形状
	if (m_pDrawBase !=NULL)
		SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));		
	else
		SetCursor(AfxGetApp()->LoadCursor(IDC_SELECT));
	Position pos;

	ScreemToWorld(point,pos);
	
//	g_pDoc->OnLButtonDblClk(point);
	

	CView::OnLButtonDblClk(nFlags, point);
}

void CCAD_PowerLandView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	Position l_pos2(0,0);	
	ScreemToWorld(point,l_pos2);
	((CMainFrame*)AfxGetMainWnd())->m_potMouseCurPos = point;
	CStatusBar * pStatus=(CStatusBar *)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	Position pos;
	ScreemToWorld(point,pos);
	if(pStatus)//显示当前坐标
	{
		CString str;
		str.Format("x=%.4f,y=%.4f",pos.x,pos.y);
		pStatus->SetPaneText(1,str);
	}
	if (m_pDrawBase !=NULL)
		m_pDrawBase->OnMouseMove(nFlags,pos);			
	else	
	{	//鼠标形状
		if (m_bFlagMBtnMove == TRUE) //鼠标中键按下时移动视图
		{
			SetCursor(AfxGetApp()->LoadCursor(IDC_MBTN_MOVE));
			m_dOriginX = m_cOrigin.x-l_pos2.x+m_dOriginX;
			m_dOriginY = m_cOrigin.y-l_pos2.y+m_dOriginY;
			Invalidate() ;			
		}
		else
			SetCursor(AfxGetApp()->LoadCursor(IDC_SELECT));//改变鼠标形状
	}	
//	SetFocus();	//当鼠标移动到客户区时，焦点也要在客户区；
	CView::OnMouseMove(nFlags, point);
}

/*
 *	鼠标中键按下的消息,坐标平移时用
 */
void CCAD_PowerLandView::OnMButtonDown(UINT nFlags,CPoint point)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_MBTN_MOVE));
	ScreemToWorld(point,m_cOrigin);
	CView::OnLButtonDown(nFlags, point);
	m_bFlagMBtnMove = TRUE;
}

/*
 *	鼠标中键双击的消息//全屏显示图形
 */
void CCAD_PowerLandView::OnMButtonDblClk(UINT nFlags,CPoint point)
{
	g_pDoc->OnFullScreemShow();	
	m_bFlagMBtnMove = -1;
}

/*
 *	鼠标中键台起的消息,坐标平移时用
 */
void CCAD_PowerLandView::OnMButtonUp(UINT nFlags,CPoint point)
{
	if (m_bFlagMBtnMove == -1)
		return;
	Position l_pos2(0,0);	
	ScreemToWorld(point,l_pos2);

	m_dOriginX = m_cOrigin.x-l_pos2.x+m_dOriginX;
	m_dOriginY = m_cOrigin.y-l_pos2.y+m_dOriginY;
	Invalidate() ;
	m_bFlagMBtnMove = FALSE;
	CView::OnLButtonDown(nFlags, point);
}

/*
 *	世界坐标转屏幕坐标
 */
void CCAD_PowerLandView::WorldToScreem(const Position& pos,CPoint& ScreemPt)
{
	double l_dNum1,l_dNum2;
	CRect rect;
	GetClientRect(&rect);
	int l_iCenterOfWindowX = ( rect.left + rect.right ) / 2;
	int l_iCenterOfWindowY = ( rect.top + rect.bottom ) / 2;
	l_dNum1 = ((pos.x - m_dOriginX)/m_dProportion + l_iCenterOfWindowX);
	l_dNum2 = (( - pos.y + m_dOriginY)/m_dProportion+l_iCenterOfWindowY);
	if(l_dNum1>=0)
		ScreemPt.x = (int)l_dNum1+0.5;
	else
		ScreemPt.x = (int)l_dNum1-0.5;

	if(l_dNum2>=0)
		ScreemPt.y = (int)l_dNum2+0.5;
	else
		ScreemPt.y = (int)l_dNum2-0.5;
}

/*
 *	屏幕坐标转世界坐标
 */
void CCAD_PowerLandView::ScreemToWorld(const CPoint& ScreemPt,Position& pos)
{
	CRect rect;
	GetClientRect(&rect);
	int l_iCenterOfWindowX = ( rect.left + rect.right ) / 2;
	int l_iCenterOfWindowY = ( rect.top + rect.bottom ) / 2;
	pos.x = (ScreemPt.x-l_iCenterOfWindowX)*m_dProportion + m_dOriginX;
	pos.y = (l_iCenterOfWindowY - ScreemPt.y)*m_dProportion + m_dOriginY;
}


/*
 *	点击软件上的画线按钮的函数
 */
void CCAD_PowerLandView::OnButtonDraw(int BtnID)
{	
	POSITION pos = g_pDoc->m_CCreateBaselayer1.GetHeadPosition();
	CDC* pDC = GetDC();	
	while (pos != NULL)
	{
		CCreateBase* l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaselayer1.GetNext(pos);
		l_pCreateBase->DrawMove(pDC,dmInvalid);
	}
	g_pDoc->m_CCreateBaselayer1.RemoveAll();
	if (g_pDoc->m_pMeasureCreateBase != NULL) 
	{
		g_pDoc->m_pMeasureCreateBase->DrawMove(pDC,dmLayer);
		delete 	g_pDoc->m_pMeasureCreateBase;
		g_pDoc->m_pMeasureCreateBase = NULL;
	}
	ReleaseDC(pDC);
	ClearSelectLine();//清除框选留下的方框
	delete m_pDrawBase;
 	m_pDrawBase = NULL;
	g_pDoc->RemoveAllCollected();
	m_iLastFuntion = BtnID;//把功能的ID号记住
	switch(BtnID)
	{
	case ID_LINE:
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n输入直线的第一点,ESC键取消:");
		else
			RemindContent("\r\nnput line of the first point, the ESC key to cancel:");
		m_pDrawBase = new CDrawLine();
		break;
	case ID_POLYLINE:
		m_pDrawBase = new CDrawPolyline();
		break;
	case ID_RECTANGLE:
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n指定一个角点[或圆角(F)],ESC键取消:");									 
		else
		RemindContent("\r\nSpecify a corner [or rounded corners (F)], the ESC key to cancel:");									 
		m_pDrawBase = new CDrawRectangle();
		break;
	case ID_ROUNDARC:
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n指定圆弧的圆心[或方向顺S/逆N--默认顺],ESC取消:");
		else
			RemindContent("\r\nSpecifies the center of the circle arc,[or direction specified circular arc of the circle (S) inverse (N) - the default shun], ESC to cancel:");
		m_pDrawBase = new CDrawRoundArc();
		break;
	case ID_ROUND:
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n指定圆的圆心[或方向顺S/逆N--默认顺],ESC取消:");
		else
			RemindContent("\r\nSpecifies the center of the circle,[or direction specified circular arc of the circle (S) inverse (N) - the default shun], ESC to cancel:");
		m_pDrawBase = new CDrawRound();
		break;
	case ID_SPLINE:
		m_pDrawBase = new CDrawSpline();
//		m_iFlagWhichBtnDown = SPLINEDOWN;
		break;
	case ID_ELLIPSE:
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n指定椭圆的轴端点,或按ESC键取消:");
		else
			RemindContent("\r\nSpecify the axis of the ellipse endpoint, or press the ESC key to cancel:");
		m_pDrawBase = new CDrawEllipse();		
		break;
	case ID_POINT:
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n指定点:");
		else
			RemindContent("\r\nInput point:");
		m_pDrawBase = new CDrawPoint();		
		break;
	case ID_VERTICAL_LINE://画垂线
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点击要垂直的线段:");
		else
			RemindContent("\r\nClick on to the vertical line:");
		m_pDrawBase = new CDrawVerticalLine();
		break;
		

	default:
		break;
	}
	//把焦点放在命令输入框里
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	
}

/*
 *	翻转菜单里的按键
 */
void CCAD_PowerLandView::OnButtonRotateSymmetry(int BtnID)
{
	POSITION pos = g_pDoc->m_CCreateBaselayer1.GetHeadPosition();
	CDC* pDC = GetDC();	
	while (pos != NULL)
	{
		CCreateBase* l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaselayer1.GetNext(pos);
		l_pCreateBase->DrawMove(pDC,dmLayer);
	}
	g_pDoc->m_CCreateBaselayer1.RemoveAll();
	if (g_pDoc->m_pMeasureCreateBase != NULL) 
	{
		g_pDoc->m_pMeasureCreateBase->DrawMove(pDC,dmLayer);
		delete 	g_pDoc->m_pMeasureCreateBase;
		g_pDoc->m_pMeasureCreateBase = NULL;
	}
	ReleaseDC(pDC);
	
	ClearSelectLine();//清除框选留下的方框
	delete m_pDrawBase;
	m_pDrawBase = NULL;
	switch(BtnID)
	{	
	case ID_ROTATE_180:
		g_pDoc->OnRotateCenter(MATH_PI);
		break;
	case ID_ROTATE_90:
		g_pDoc->OnRotateCenter(MATH_PI/2);		
		break;
	case ID_SYMMETRY_XAXIS:
		g_pDoc->OnSymmetry(ID_SYMMETRY_XAXIS);
		break;
	case ID_SYMMETRY_YAXIS:
		g_pDoc->OnSymmetry(ID_SYMMETRY_YAXIS);
		break;
	case ID_SYMMETRY_LANDR:
		g_pDoc->OnSymmetry(ID_SYMMETRY_LANDR);
		break;
	case ID_SYMMETRY_TANDB:
		g_pDoc->OnSymmetry(ID_SYMMETRY_TANDB);
		break;
	case ID_ROTATE_ANGLE:
		{
			CDlgParallel dlg;
			dlg.m_iCase = ROTATE_ANGLE;
			dlg.DoModal();
		}
		break;
	case ID_ACCORDING_TO_LINE_HORI://按该线水平
		if(m_dAngleRightBtn == 0.0)
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n右键没有点击到线",SEND_RICHEDIT);
			else
				RemindContent("\r\nRight click to line no",SEND_RICHEDIT);		
		}
		else
		{
			AngleTurnHoriMode(m_dAngleRightBtn);
			g_pDoc->OnRotateCenter(m_dAngleRightBtn);
		}
		break;
	case ID_ACCORDING_TO_LINE_VERT://按该线垂直
		if(m_dAngleRightBtn == 0.0)
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n右键没有点击到线",SEND_RICHEDIT);
			else
				RemindContent("\r\nRight click to line no",SEND_RICHEDIT);		
		}
		else
		{
			AngleTurnVertMode(m_dAngleRightBtn);
			g_pDoc->OnRotateCenter(m_dAngleRightBtn);		
		}
		break;
		
	default:
		break;
	}
}

/*
 *	点击软件上的修改按钮的函数
 */
void CCAD_PowerLandView::OnButtonFix(int BtnID)
{	
	POSITION pos = g_pDoc->m_CCreateBaselayer1.GetHeadPosition();
	CDC* pDC = GetDC();	
	while (pos != NULL)
	{
		CCreateBase* l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaselayer1.GetNext(pos);
		l_pCreateBase->DrawMove(pDC,dmLayer);
	}
	g_pDoc->m_CCreateBaselayer1.RemoveAll();
	if (g_pDoc->m_pMeasureCreateBase != NULL) 
	{
		g_pDoc->m_pMeasureCreateBase->DrawMove(pDC,dmLayer);
		delete 	g_pDoc->m_pMeasureCreateBase;
		g_pDoc->m_pMeasureCreateBase = NULL;
	}
	ReleaseDC(pDC);
	
	ClearSelectLine();//清除框选留下的方框
	delete m_pDrawBase;
	m_pDrawBase = NULL;
	m_iLastFuntion = BtnID;//把功能的ID号记住
	switch(BtnID)
	{	
	case ID_DELETE:
		DeleteList();
		break;
	case ID_MOVE:
		if(RemindNoSelect() == FALSE)//如果是0，就不修改操作
			break;
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n请输入基点[或位移D],或按ESC键取消:");
		else
			RemindContent("\r\nPlease enter the basis points, or press the ESC key to cancel:");
		m_pDrawBase = new CModifyMove();
		//把焦点放在命令输入框里
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;
	case ID_COPY:
		if(RemindNoSelect() == FALSE)//如果是0，就不修改操作
			break;
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n请输入基点,或按ESC键取消:");
		else
			RemindContent("\r\nPlease enter the basis points, or press the ESC key to cancel:");
		//把焦点放在命令输入框里
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		m_pDrawBase = new CModifyCopy();
		break;
	case ID_MIRROR:
		if(RemindNoSelect() == FALSE)//如果是0，就不修改操作
			break;
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n指定镜像线的第一点,或选择是否删除源对象?[Y是，N否--默认N]:");
		else
			RemindContent("\r\nSpecify the mirror line first, or choose to delete the source object? [Y, N -Default N] :");
		m_pDrawBase = new CModifyMirror();
		//把焦点放在命令输入框里
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;
	case ID_ROTATE:
		if(RemindNoSelect() == FALSE)//如果是0，就不修改操作
			break;
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n请输入基点,或按ESC键取消,或选择是否删除源对象?[Y是，N否--默认N]:");
		else
			RemindContent("\r\nPlease enter the basis points, or press the ESC key to cancel, or choose whether to delete the source object? [Y, N -Default N] :");
		m_pDrawBase = new CModifyRotate();
		//把焦点放在命令输入框里
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;
	case ID_BREAK_TWO://一个图形打断成两个图形
		m_pDrawBase = new CDrawBreakTwo();
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点击图形打断,或右键输入距此点距离,或按ESC键取消:");
		else
			RemindContent("\r\nClick graphic interrupt, or right-click the input distance from this point, or press the ESC key to cancel:");
		break;
	case ID_OVER_CUT://过切
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n对所有图形过切:");
		else
			RemindContent("\r\nFor all graph overcut:");		
		g_pDoc->OverCut();
		break;		
	case ID_RE_ARRANG://重新排序
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n重新按点击顺序排序,或按ESC键取消:");
		else
			RemindContent("\r\nClick order again, or press the ESC key to cancel:");
		g_pDoc->RemoveAllCollected();
		m_stcSwitch.bShowNum = TRUE;
		Invalidate();
		m_pDrawBase = new CDrawArrange();
		break;	
	case ID_CHAMFER://倒角
		g_pDoc->RemoveAllCollected();
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点击两个线倒其夹角[或选择倒角类型，凸角(O)/凹角(I)，默认为凸角]:");	
		else
			RemindContent("\r\n[Choose chamfer type,O convex Angle/I concave Angle, the default for convex Angle]");	
		m_pDrawBase = new CDrawChamfer();
		//把焦点放在命令输入框里
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;	
	case ID_GRIND_GLASS://磨边
		g_pDoc->OnGraphForGrind();
		break;	
	case ID_SELECT_BEGIN_POS://选择起点
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点击直线选择起点:");
		else
			RemindContent("\r\nClick the line selection starting point:");		
		m_pDrawBase = new CDrawSetBeginPos();
		break;
	case ID_IO_KNIFE://进退刀
		g_pDoc->OnGraphKinfe();
		break;
	case ID_SCALE_XY://缩放
		g_pDoc->OnGraphScale();
		break;
	case ID_TOGETHER://合并
		g_pDoc->OnGraphTogether();
		break;
	case ID_SAVE_TO_FILE://文件发送
		g_pDoc->OnFileSend();
		break;
	case ID_EXTEND_LINE://给线延长
		m_pDrawBase = new CDrawExtendLine;
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n请输入延长长度,然后点击要延长端点:");
		else
			RemindContent("\r\nPlease enter the extension length, and then click to extend the endpoint:");
		//把焦点放在命令输入框里
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;
	case ID_MATRIX://打开矩阵对话框
		g_pDoc->OnOpenDlgMatrix();
		break;	
	case ID_MILL_GROOVES://加轮廓图
		g_pDoc->OnTurnMill();
		break;
	case  ID_UNDO://撤消
		g_pDoc->RemoveAllCollected();
		g_pDoc->m_Undo->OnUndo();
		break;
	case  ID_REDO://恢复
		g_pDoc->RemoveAllCollected();
		g_pDoc->m_Undo->OnRedo();
		break;
	case  ID_MILL_SIDE://模板机刀补
		g_pDoc->OnMillSide();
		break;
	case  ID_TO_LESS_POINT://把多点的多段线根据精度变成少点的多段线
		g_pDoc->OnToLessPoint();
		break;
	case  ID_ADD_RECT://加边框
		g_pDoc->OnAddRect();
		break;
	case  ID_MEASURE_LONG://测量长度
		m_pDrawBase = new CDrawMeasure();
		break;
	case  ID_PARALLEL://平行线
//		g_pDoc->OnParallel();	
		m_pDrawBase = new CDrawParallel();
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点击要平行的图形:");
		else
			RemindContent("\r\nClick on the graphic to parallel:");
		
		break;
	case  ID_ADD_POINT://相对加点
		m_pDrawBase = new CDrawAddPoint();
		break;
	case  ID_DELETE_MORE_LINE://删重刀
		g_pDoc->OnDeleteMoreLine();
		break;	

	case  ID_VIEW_MOVE://移动视图窗口
		m_pDrawBase = new CDrawMoveView();
//		m_bBreakThreadOne = TRUE;
//		m_ThreadOne = CreateThread(NULL,1,ThreadFunOne,(void*)this,CREATE_SUSPENDED,NULL);	
//		ResumeThread(m_ThreadOne);
		
		break;	
	case  ID_VIEW_FULL://全屏显示图形
		g_pDoc->OnFullScreemShow();
	//	m_bBreakThreadOne = FALSE;
		
		break;	
	case  ID_MOVE_ORIGIN://把图形移动到原点
		g_pDoc->OnMoveOrigin();
		break;	
	case  ID_SCALE://把图形按比例缩放
		if(RemindNoSelect() == FALSE)//如果是0，就不修改操作
			break;
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n请输入基点:");
		else
			RemindContent("\r\nPlease enter the basis points:");
		m_pDrawBase = new CDrawZoomScale();		
		//把焦点放在命令输入框里
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;
	case  ID_SOLID_TO_DASH://把线变成虚线
		g_pDoc->OnSolidToDash();
		break;	
	case  ID_TRIM://修剪
		g_pDoc->RemoveAllCollected();
		m_pDrawBase = new CDrawTrim();
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点击选中想修剪的线:");
		else
			RemindContent("\r\nClick on the selected want to trim line:");		
		break;	
	case  ID_MOVE_POINT://移动点
		g_pDoc->RemoveAllCollected();
		m_pDrawBase = new CDrawMovePoint();
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点击要移动的点:");
		else
			RemindContent("\r\nClick to want to point to move:");		
		break;	
		
	default:
		break;
	}

}

/*
 *	负责一些参数的按钮的点击
 */
void CCAD_PowerLandView::OnButtonParam(int BtnID)
{
	switch(BtnID)
	{
	case ID_PROPERTY://画图的参数
		g_pDoc->OnProperty();
		break;
	case ID_PARAM_JISAN:
		g_pDoc->OnOpenDlgParamGlassGrind();
		break;
	case ID_PARAM_IN_OUT_GRAPH:
		g_pDoc->OnOpenDlgParamInoutKnife();
		break;
	case ID_PARAM_OVER_CUT:
		g_pDoc->OnOpenDlgParamOverCut();
		break;
	case ID_WORK_FOR:
		g_pDoc->OnOpenDlgWorkFor();
		break;
	default:
		break;
	}
}

/*
 *	显示工具栏
 */
void CCAD_PowerLandView::OnButtonShowBar(int BtnID)
{
	((CMainFrame*)AfxGetMainWnd())->m_wndDrawToolBar.IsWindowVisible();
	switch(BtnID)
	{	
	case ID_VIEW_COLOR:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndPropBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
				(&(((CMainFrame*)AfxGetMainWnd())->m_wndPropBar), FALSE, TRUE); 	
		else	
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
				(&(((CMainFrame*)AfxGetMainWnd())->m_wndPropBar), TRUE, TRUE); 	
		m_iFlagWhichBtnDown = COLOR;	//标志哪个按下，下面按扭刷新用
		break;
	case ID_VIEW_DRAW:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndDrawToolBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndDrawToolBar), FALSE, TRUE); 	
		else	
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
				(&(((CMainFrame*)AfxGetMainWnd())->m_wndDrawToolBar), TRUE, TRUE); 	
		m_iFlagWhichBtnDown = DRAW;	//标志哪个按下，下面按扭刷新用
		break;
	case ID_VIEW_MODIFY:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndModifyToolBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndModifyToolBar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndModifyToolBar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = MODIFY;	//标志哪个按下，下面按扭刷新用
		break;
	case ID_VIEW_PROPERTY:
		if(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), FALSE, TRUE); 	
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 	
		m_iFlagWhichBtnDown = PROPERTY;	//标志哪个按下，下面按扭刷新用
		break;
	case ID_VIEW_COMMAND:
		if(((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = COMMAND;	//标志哪个按下，下面按扭刷新用
		break;
	case ID_VIEW_PARAM:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndParamTollbar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndParamTollbar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndParamTollbar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = COMMAND;	//标志哪个按下，下面按扭刷新用
		break;
	case ID_VIEW_VIEW:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndViewBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndViewBar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndViewBar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = VIEW;	//标志哪个按下，下面按扭刷新用
		break;
	case ID_VIEW_TEMPLATE:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndTemplateToolBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndTemplateToolBar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndTemplateToolBar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = VIEW;	//标志哪个按下，下面按扭刷新用
		break;
	case ID_VIEW_GRIND_GLASS:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndGrindGlassToolBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndGrindGlassToolBar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndGrindGlassToolBar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = VIEW;	//标志哪个按下，下面按扭刷新用
		break;
	default:
		break;
	}	
}

/*
 *	点击软件上的几个开关按键
 */
void CCAD_PowerLandView::OnButtonSwitch(int BtnID)
{	
	ClearSelectLine();//清除框选留下的方框
	switch(BtnID)
	{	
	case ID_SHOW_NUM://显示切割顺序号
		if (m_stcSwitch.bShowNum == FALSE) 
			m_stcSwitch.bShowNum = TRUE;
		else
			m_stcSwitch.bShowNum = FALSE;
		this->Invalidate(TRUE);
		break;
	case ID_SHOW_ARROW://显示切割方向箭头
		if (m_stcSwitch.bShowArrow == FALSE) 
			m_stcSwitch.bShowArrow = TRUE;
		else
			m_stcSwitch.bShowArrow = FALSE;
		this->Invalidate(TRUE);
		break;
	case ID_POINT_END:
		if ((m_iSwitch&END_POINT) == END_POINT) 
			m_iSwitch = m_iSwitch^END_POINT;
		else
			m_iSwitch = m_iSwitch|END_POINT;
		break;
	case ID_POINT_NEAR:
		if ((m_iSwitch&NEAR_POINT) == NEAR_POINT) 
			m_iSwitch = m_iSwitch^NEAR_POINT;
		else
			m_iSwitch = m_iSwitch|NEAR_POINT;
		break;
	case ID_ORTHOGONAL:
		if ((m_iSwitch&ORTHOGONAL) == ORTHOGONAL) 
			m_iSwitch = m_iSwitch^ORTHOGONAL;
		else
			m_iSwitch = m_iSwitch|ORTHOGONAL;
		break;
	case ID_POINT_FOOT:
		if ((m_iSwitch&FOOT_POINT) == FOOT_POINT) 
			m_iSwitch = m_iSwitch^FOOT_POINT;
		else
			m_iSwitch = m_iSwitch|FOOT_POINT;
		break;	
	case ID_POINT_MID:
		if ((m_iSwitch&MID_POINT) == MID_POINT) 
			m_iSwitch = m_iSwitch^MID_POINT;
		else
			m_iSwitch = m_iSwitch|MID_POINT;
		break;	

	}
}

/*
 *	软件按钮状态函数
 */
void CCAD_PowerLandView::OnUpdateButtonSwitch(CCmdUI* pCmdUI)
{
	int l_iFlag = 0;
	switch(pCmdUI->m_nID)
	{
	case ID_SHOW_ARROW:
		if(m_stcSwitch.bShowArrow == TRUE)
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_SHOW_NUM:
		if(m_stcSwitch.bShowNum == TRUE)
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_POINT_END:
		if((m_iSwitch&END_POINT) == END_POINT)
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_POINT_NEAR:
		if((m_iSwitch&NEAR_POINT) == NEAR_POINT)
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_ORTHOGONAL:
		if((m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_POINT_FOOT:
		if((m_iSwitch&FOOT_POINT) == FOOT_POINT)
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_POINT_MID:
		if((m_iSwitch&MID_POINT) == MID_POINT)
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	default:
		break;
	}
	pCmdUI->SetCheck(l_iFlag);//设定按下还是抬起
}


/*
 *	软件按钮状态函数
 */
void CCAD_PowerLandView::OnUpdateButtonShowBar(CCmdUI* pCmdUI)
{	
	int l_iFlag = 0;
	switch(pCmdUI->m_nID)
	{
	case ID_VIEW_COLOR:
		if ((((CMainFrame*)AfxGetMainWnd()))->m_wndPropBar.IsWindowVisible())
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_VIEW_DRAW:
		if ((((CMainFrame*)AfxGetMainWnd()))->m_wndDrawToolBar.IsWindowVisible())
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_VIEW_MODIFY:
		if ((((CMainFrame*)AfxGetMainWnd()))->m_wndModifyToolBar.IsWindowVisible())
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_VIEW_PROPERTY:
		if ((((CMainFrame*)AfxGetMainWnd()))->m_DlgPropertyBar.IsWindowVisible())
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_VIEW_COMMAND:
		if ((((CMainFrame*)AfxGetMainWnd()))->m_DlgCommandBar.IsWindowVisible())
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_VIEW_PARAM:
		if ((((CMainFrame*)AfxGetMainWnd()))->m_wndParamTollbar.IsWindowVisible())
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_VIEW_VIEW:
	   if ((((CMainFrame*)AfxGetMainWnd()))->m_wndViewBar.IsWindowVisible())
		   l_iFlag = 1;
	   else
		   l_iFlag = 0;
	   break;
	case ID_VIEW_TEMPLATE:
		if ((((CMainFrame*)AfxGetMainWnd()))->m_wndTemplateToolBar.IsWindowVisible())
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	case ID_VIEW_GRIND_GLASS:
		if ((((CMainFrame*)AfxGetMainWnd()))->m_wndGrindGlassToolBar.IsWindowVisible())
			l_iFlag = 1;
		else
			l_iFlag = 0;
		break;
	default:
		break;
	}
	pCmdUI->SetCheck(l_iFlag);//设定按扭按下还是抬起，如果工具栏是否隐藏。
}

/*
 *	删除图元函数，上面的点击“删除”按钮要实现的功能
 */
void CCAD_PowerLandView::DeleteList()
{	
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize() ;
	//如果是选择集是空的就返回
	if (l_iSize == 0)
		return;
	//撤消类的对象,以下是撤消
	CUndoRedo l_UndoRedo;
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_DELETE;
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;
	/////////////////////////////////
	CCreateBase* pSelEntity = NULL ;
	POSITION pos = NULL ,pos1=NULL;
	for( int i = 0 ; i < l_iSize; i++ )
	{
		pSelEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i] ;
		pos1 = pos = g_pDoc->m_CCreateBaseList.Find(pSelEntity);
		g_pDoc->m_CCreateBaseList.GetNext(pos1);
		g_pDoc->m_CCreateBaseList.RemoveAt(pos);
		//向撤消链表里加删除操作
		if (i == 0)
		{
			CurGraphPos->pCreateBasepos = (long)pSelEntity;
			CurGraphPos->pListPos = pos1;
		}
		else
		{
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (long)pSelEntity;
			GraphPos->pListPos = pos1;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		}
		pSelEntity = NULL ;
	}
	if (pUndoList->pGraphPos->pCreateBasepos != NULL)
		g_pDoc->m_Undo->AddUndoList(pUndoList);	
	//清空选择集
	g_pDoc->m_CollectOfPicked.RemoveAll();
	//更新文档，重新绘制
	Invalidate() ;
}

/*
 *	软件按钮状态函数
 */
void CCAD_PowerLandView::OnUpdateButtonFix(CCmdUI* pCmdUI)
{	

}

void CCAD_PowerLandView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
/**/
	Position pos;
	ScreemToWorld(point,pos);
	if (m_pDrawBase != NULL)
		m_pDrawBase->OnRButtonDown(nFlags,pos);

	CView::OnRButtonDown(nFlags, point);
}

void CCAD_PowerLandView::GetAndSendCommand()
{
	CString l_strGetCommand;
	CString posx,posy;
	Position pos;
	g_pView->ScreemToWorld((((CMainFrame*)AfxGetMainWnd())->m_potMouseCurPos),pos);
	//先获得文本内容
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItemText(IDC_EDIT_COMMAND,l_strGetCommand);
	if (l_strGetCommand == "")
	{
		if(	m_iLastFuntion != 0||m_iLastFuntion!=ID_UNDO||m_iLastFuntion!=ID_REDO)//当按下空格或回车时执行上一次的功能
		{
			OnButtonFix(m_iLastFuntion);
			if(m_pDrawBase == NULL)
				OnButtonDraw(m_iLastFuntion);
			return;
		}
		return;
	}
	if(m_pDrawBase)
		m_pDrawBase->OnLButtonDown(2,pos,l_strGetCommand);//1为鼠标点击，2为键盘点击
}
/*
 *	画坐标系，标尺，桌面尺寸函数
 */
void CCAD_PowerLandView::DrawCoord(CDC* pDC)
{
	CPoint l_ScreemOrigin;
	//把世界坐标原点转成屏幕坐标点
	Position l_WorldOrigin(0,0);
	WorldToScreem(l_WorldOrigin,l_ScreemOrigin);
	//是否画坐标系
	if (g_pDoc->m_stcWorkFor.IsShowCoordinate == 1)
	{
		//画圆点方框
		pDC->Rectangle(l_ScreemOrigin.x-5,l_ScreemOrigin.y-5,l_ScreemOrigin.x+5,l_ScreemOrigin.y+5);
		//画X坐标
		pDC->MoveTo(l_ScreemOrigin.x-4,l_ScreemOrigin.y );
		pDC->LineTo(l_ScreemOrigin.x+100,l_ScreemOrigin.y);
		pDC->MoveTo(l_ScreemOrigin.x+92,l_ScreemOrigin.y-8);
		pDC->LineTo(l_ScreemOrigin.x+100,l_ScreemOrigin.y);
		pDC->LineTo(l_ScreemOrigin.x+92,l_ScreemOrigin.y+8);
		pDC->LineTo(l_ScreemOrigin.x+92,l_ScreemOrigin.y-8);
		//画X字
		pDC->MoveTo(l_ScreemOrigin.x+115,l_ScreemOrigin.y-7);
		pDC->LineTo(l_ScreemOrigin.x+129,l_ScreemOrigin.y+7);

		pDC->MoveTo(l_ScreemOrigin.x+129,l_ScreemOrigin.y-7);
		pDC->LineTo(l_ScreemOrigin.x+115,l_ScreemOrigin.y+7);
		//画Y坐标
		pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y+4);
		pDC->LineTo(l_ScreemOrigin.x,l_ScreemOrigin.y-100);
		pDC->MoveTo(l_ScreemOrigin.x-8,l_ScreemOrigin.y-92);
		pDC->LineTo(l_ScreemOrigin.x,l_ScreemOrigin.y-100);
		pDC->LineTo(l_ScreemOrigin.x+8,l_ScreemOrigin.y-92);
		pDC->LineTo(l_ScreemOrigin.x-8,l_ScreemOrigin.y-92);
		//画Y字
		pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y-115);
		pDC->LineTo(l_ScreemOrigin.x,l_ScreemOrigin.y-125);
		pDC->LineTo(l_ScreemOrigin.x+5,l_ScreemOrigin.y-135);

		pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y-125);
		pDC->LineTo(l_ScreemOrigin.x-5,l_ScreemOrigin.y-135);
	}
	//画机台,是否显示机台
	if(g_pDoc->m_stcWorkFor.IsShowSize == 1)
	{
		//
		Position l_posMachineSize(g_pDoc->m_stcWorkFor.MachineX,g_pDoc->m_stcWorkFor.MachineY);
		CPoint l_MachineSize(0,0);
		WorldToScreem(l_posMachineSize,l_MachineSize); 
		CPen pen; 
		pen.CreatePen(PS_DASHDOT,2,233429);
		CPen* pOldPen = pDC->SelectObject(&pen); 
		pDC->MoveTo(l_ScreemOrigin.x,l_MachineSize.y);
		pDC->LineTo(l_MachineSize.x+5,l_MachineSize.y);
		pDC->MoveTo(l_MachineSize.x,l_ScreemOrigin.y);
		pDC->LineTo(l_MachineSize.x,l_MachineSize.y-5);
		// 恢复原来的画笔 
		pDC->SelectObject(pOldPen); 
		pen.DeleteObject();//释放pen的Create的;
	}
	//是否显示标尺
	if (g_pDoc->m_stcWorkFor.IsShowRulerLine == 1)
	{	
		pDC->SetTextAlign(TA_CENTER);//文字居中		
		CPoint l_pot(0,0);
		Position l_pos(0,0);
		CString l_strNum;		
		//画X轴
		pDC->MoveTo(l_ScreemOrigin.x-1000,l_ScreemOrigin.y);
		pDC->LineTo(l_ScreemOrigin.x+1000,l_ScreemOrigin.y);
		//画刻度加字
		for(int i = 0;i<200;i++)
		{
			//正方向
			if (i%10==0 && i!=0)//整10个画一个长刻度
			{
				pDC->MoveTo(l_ScreemOrigin.x+i*5,l_ScreemOrigin.y);
				pDC->LineTo(l_ScreemOrigin.x+i*5,l_ScreemOrigin.y+10);	
				l_pot.x = l_ScreemOrigin.x+i*5;
				l_pot.y = l_ScreemOrigin.y;
				ScreemToWorld(l_pot,l_pos);
				l_strNum.Format("%.f",l_pos.x);
				pDC->TextOut(l_ScreemOrigin.x+i*5,l_ScreemOrigin.y+10,l_strNum);				
			}
			else//画小刻度
			{
				pDC->MoveTo(l_ScreemOrigin.x+i*5,l_ScreemOrigin.y);
				pDC->LineTo(l_ScreemOrigin.x+i*5,l_ScreemOrigin.y+5);
			}
			//负方向
			if (i%10==0 && i!=0)//整10个画一个长刻度
			{
				pDC->MoveTo(l_ScreemOrigin.x-i*5,l_ScreemOrigin.y);
				pDC->LineTo(l_ScreemOrigin.x-i*5,l_ScreemOrigin.y+10);	
				l_pot.x = l_ScreemOrigin.x-i*5;
				l_pot.y = l_ScreemOrigin.y;
				ScreemToWorld(l_pot,l_pos);
				l_strNum.Format("%.f",l_pos.x);
				pDC->TextOut(l_ScreemOrigin.x-i*5,l_ScreemOrigin.y+10,l_strNum);				
			}
			else//画小刻度
			{
				pDC->MoveTo(l_ScreemOrigin.x-i*5,l_ScreemOrigin.y);
				pDC->LineTo(l_ScreemOrigin.x-i*5,l_ScreemOrigin.y+5);
			}
		}
		//画Y轴
		pDC->SetTextAlign(TA_RIGHT);//文字居左			
		pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y-800);
		pDC->LineTo(l_ScreemOrigin.x,l_ScreemOrigin.y+800);
		//画刻度加字
		for(int j = 0;j<160;j++)
		{
			//正方向
			if (j%10==0 && j!=0)//整10个画一个长刻度
			{
				pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y+j*5);
				pDC->LineTo(l_ScreemOrigin.x-10,l_ScreemOrigin.y+j*5);	
				l_pot.x = l_ScreemOrigin.x;
				l_pot.y = l_ScreemOrigin.y+j*5;
				ScreemToWorld(l_pot,l_pos);
				l_strNum.Format("%.f",l_pos.y);
				pDC->TextOut(l_ScreemOrigin.x-15,l_ScreemOrigin.y+j*5-8,l_strNum);				
			}
			else//画小刻度
			{
				pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y+j*5);
				pDC->LineTo(l_ScreemOrigin.x-5,l_ScreemOrigin.y+j*5);
			}
			//负方向
			if (j%10==0 && j!=0)//整10个画一个长刻度
			{
				pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y-j*5);
				pDC->LineTo(l_ScreemOrigin.x-10,l_ScreemOrigin.y-j*5);	
				l_pot.x = l_ScreemOrigin.x;
				l_pot.y = l_ScreemOrigin.y-j*5;
				ScreemToWorld(l_pot,l_pos);
				l_strNum.Format("%.f",l_pos.y);
				pDC->TextOut(l_ScreemOrigin.x-15,l_ScreemOrigin.y-j*5-8,l_strNum);				
			}
			else//画小刻度
			{
				pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y-j*5);
				pDC->LineTo(l_ScreemOrigin.x-5,l_ScreemOrigin.y-j*5);
			}
		}		
	}
}


//返回缩放比例系数
double CCAD_PowerLandView::ReturnProportion()
{
	return m_dProportion;
}

void CCAD_PowerLandView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pDrawBase != NULL)
	{
		Position pos(0,0);
		ScreemToWorld(point,pos);
		m_pDrawBase->OnLButtonUp(nFlags,pos);
	}
	CView::OnLButtonUp(nFlags, point);
}

BOOL CCAD_PowerLandView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
// 	BOOL A = CView::OnSetCursor(pWnd, nHitTest, message);	
	return TRUE;		//不让鼠标闪
}

void CCAD_PowerLandView::ClearSelectLine()
{
	if(m_iFlagWhichBtnDown == DRAW_SELECT)
	{
		CDC*	pDC = GetDC(); 	
		// 创擦除上一条橡皮线
		CCreateRectangle*	pTempLine2 = new CCreateRectangle(m_PosBegin,m_PosLast);	
		pTempLine2->DrawMove(pDC,dmDrag,FALSE,FALSE);
		delete pTempLine2;
		// 释放设备环境指针
		ReleaseDC(pDC); 
		m_iFlagWhichBtnDown = MEIYOU;
		delete m_pDrawBase;
		m_pDrawBase = NULL;
	}
}

/*
 *	提示没有单击选中图形
 */
BOOL CCAD_PowerLandView::RemindNoSelect()
{
	//获得选择集大小，下面用，如果是0，就不修改操作
	int l_iCount = g_pDoc->m_CollectOfPicked.GetSize();
	if(l_iCount == 0)//如果是0，就不修改操作
	{
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n没有选中，重新选择");
		else
			RemindContent("\r\nNot selected, choose again：");			
		return FALSE;
	}		
	return TRUE;
}





BOOL CCAD_PowerLandView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
//	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CCAD_PowerLandView::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if (pCopyDataStruct->cbData > 0)
    {
        char recvData[256] = {0} ;
        strncpy(recvData, (char *)pCopyDataStruct->lpData, pCopyDataStruct->cbData);
    }
	return CView::OnCopyData(pWnd, pCopyDataStruct);
}

void CCAD_PowerLandView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
/*
	//如果选择集为空，不弹框
	if(g_pDoc->m_CollectOfPicked.GetSize() == 0||m_pDrawBase != NULL)
		return;
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	for(int i=0;i<l_iSize;i++)
	{
		CCreateBase* pCreateBase = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(i);
		if(pCreateBase->IsPick(point) != -1)
			AfxMessageBox("adf");
	}


	CPoint p;  
	GetCursorPos(&p);//鼠标点的屏幕坐标  
	CMenu m_Menu;  
	m_Menu.LoadMenu(IDR_MENU_CILENT_RBTNDOW);//编辑好的菜单资源  
	CMenu *m_SubMenu = m_Menu.GetSubMenu(0); 
	m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);  */
}

DWORD CCAD_PowerLandView::ThreadFunOne(LPVOID lpParam)
{
	CDC* pDC = g_pView->GetDC();
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase = NULL;
	while (POS)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		
	}

	g_pView->ReleaseDC(pDC);
	return 0;
}








void CCAD_PowerLandView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/**/
	//如果选择集为空，不弹框
	if(g_pDoc->m_CollectOfPicked.GetSize() == 0||m_pDrawBase != NULL)
		return;
	m_dAngleRightBtn = 0.0;
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	for(int i=0;i<l_iSize;i++)
	{
		CCreateBase* pCreateBase = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(i);
		int l_iPick = pCreateBase->IsPick(point);
		if(l_iPick != -1)
		{
			if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				CCreateLine *pLine = (CCreateLine*)pCreateBase;
				m_dAngleRightBtn = GetAngleToXAxis(pLine->m_Begin,pLine->m_End);
			}
			else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{
				CCreatePolyline *pPolyline = (CCreatePolyline*)pCreateBase;
				m_dAngleRightBtn = GetAngleToXAxis(pPolyline->m_point.GetAt(l_iPick),pPolyline->m_point.GetAt(l_iPick+1));
			}
			else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{
				CCreateRectangle *pPolyline = (CCreateRectangle*)pCreateBase;
				m_dAngleRightBtn = GetAngleToXAxis(pPolyline->m_point.GetAt(l_iPick),pPolyline->m_point.GetAt(l_iPick+1));
			}
			else if(pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{
				CCreateEllipse *pPolyline = (CCreateEllipse*)pCreateBase;
				m_dAngleRightBtn = GetAngleToXAxis(pPolyline->m_point.GetAt(l_iPick),pPolyline->m_point.GetAt(l_iPick+1));
			}
			break;
		}
	}
	
	
	CPoint p;  
	GetCursorPos(&p);//鼠标点的屏幕坐标  
	CMenu m_Menu;  
	m_Menu.LoadMenu(IDR_MENU_CILENT_RBTNDOW);//编辑好的菜单资源  
	CMenu *m_SubMenu = m_Menu.GetSubMenu(0); 
	m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);  
	

	CView::OnRButtonUp(nFlags, point);
}

/*
 *	角度变成按线水平应该的角度
 */
void CCAD_PowerLandView::AngleTurnHoriMode(double& dAngleHori)
{
	if(dAngleHori>0.0&&dAngleHori<=MATH_PI/2)
		dAngleHori = -dAngleHori;
	else if(dAngleHori>=MATH_PI/2&&dAngleHori<=MATH_PI*3/2)
		dAngleHori = MATH_PI-dAngleHori;
	else if(dAngleHori>=MATH_PI*3/2&&dAngleHori<=MATH_2PI)
		dAngleHori = MATH_2PI-dAngleHori;
}

/*
 *	角度变成按线垂直应该的角度
 */
void CCAD_PowerLandView::AngleTurnVertMode(double& dAngleHori)
{
	if(dAngleHori>0.0&&dAngleHori<=MATH_PI/2)
		dAngleHori = MATH_PI/2-dAngleHori;
	else if(dAngleHori>=MATH_PI/2&&dAngleHori<=MATH_PI)
		dAngleHori = MATH_PI/2-dAngleHori;
	else if(dAngleHori>=MATH_PI&&dAngleHori<=MATH_2PI)
		dAngleHori = MATH_PI*3/2-dAngleHori;
}



