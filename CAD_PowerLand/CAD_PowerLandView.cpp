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
	//��ͼԪ
	ON_COMMAND_RANGE(ID_LINE,ID_ELLIPSE,OnButtonDraw)
	//�޸�ͼԪ
	ON_COMMAND_RANGE(ID_DELETE,ID_MATRIX,OnButtonFix)
	ON_UPDATE_COMMAND_UI_RANGE(ID_DELETE,ID_MATRIX,OnUpdateButtonFix)
	//��������
	ON_COMMAND_RANGE(ID_PARAM_IN_OUT_GRAPH,ID_PROPERTY,OnButtonParam)
	//������ʾ
	ON_COMMAND_RANGE(ID_VIEW_COLOR,ID_VIEW_COMMAND,OnButtonShowBar)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_COLOR,ID_VIEW_COMMAND,OnUpdateButtonShowBar)
	//���ذ���
	ON_COMMAND_RANGE(ID_SHOW_ARROW,ID_SHOW_NUM,OnButtonSwitch)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SHOW_ARROW,ID_SHOW_NUM,OnUpdateButtonSwitch)
	//��ת�˵�
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
	m_bFlagMBtnMove = FALSE;	//����м�����
	m_dRulerLineX = 1000;//�̶ȳ߳�ʼ��ֵ
	m_dRulerLineY = 800;
	m_bBreakThreadOne = TRUE;
	m_iLastFuntion = 0;

	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "View.ini";
	char l_chDataName[80]={0};
	GetPrivateProfileString("View","bShowNum","-1",l_chDataName,20,l_strFlieName);//��ʾ���ְ�ť
	m_stcSwitch.bShowNum = atoi(l_chDataName);
	GetPrivateProfileString("View","bShowArrow","-1",l_chDataName,20,l_strFlieName);//��ʾ��ͷ��ť
	m_stcSwitch.bShowArrow = atoi(l_chDataName);
	GetPrivateProfileString("View","Catch","-1",l_chDataName,20,l_strFlieName);
	m_iSwitch = atoi(l_chDataName);
}

CCAD_PowerLandView::~CCAD_PowerLandView()
{
	//������һ��ʱ�͹����
	if (m_pDrawBase)
		delete m_pDrawBase;


	//�������
	CString l_strFlieName(INI_FILE_COLORM);
	l_strFlieName = l_strFlieName + "View.ini";
	
	CString l_strDataName;			//�����õ��м�����һ�������ݣ�һ���Ǽ���
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

//������ʹ�����ɫ��
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
		WndClass.hbrBackground = CreateSolidBrush(RGB(0,0,0));//��Ҫ�����
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
	
	DrawCoord(pDC);	//������ϵ����
/*
	//�ѱ���ˢ�ɺ�ɫ�������������PreCreateWindow(CREATESTRUCT& cs)���õ�
	CRect rect; 
	this->GetClientRect(rect); 
	pDC->FillSolidRect(&rect,RGB(0,0,0));
*/

	//////////////////////////////////////////////////////////////////////////
	g_refresh = 0;		//
	g_pDoc->Draw(pDC);		//ˢ��ʱ�Ѵ�������ͼԪָ�붼��һ�¡�

	
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
		SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));	//�ı������״	
		m_pDrawBase->OnLButtonDown(nFlags,pos,"");	//1Ϊ�������2Ϊ���̵��
	}
	else
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_SELECT));//�ı������״
		g_pDoc->OnLButtonDown(nFlags,point);
	}


	CView::OnLButtonDown(nFlags, point);
}

void CCAD_PowerLandView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//�ı������״
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
	if(pStatus)//��ʾ��ǰ����
	{
		CString str;
		str.Format("x=%.4f,y=%.4f",pos.x,pos.y);
		pStatus->SetPaneText(1,str);
	}
	if (m_pDrawBase !=NULL)
		m_pDrawBase->OnMouseMove(nFlags,pos);			
	else	
	{	//�����״
		if (m_bFlagMBtnMove == TRUE) //����м�����ʱ�ƶ���ͼ
		{
			SetCursor(AfxGetApp()->LoadCursor(IDC_MBTN_MOVE));
			m_dOriginX = m_cOrigin.x-l_pos2.x+m_dOriginX;
			m_dOriginY = m_cOrigin.y-l_pos2.y+m_dOriginY;
			Invalidate() ;			
		}
		else
			SetCursor(AfxGetApp()->LoadCursor(IDC_SELECT));//�ı������״
	}	
//	SetFocus();	//������ƶ����ͻ���ʱ������ҲҪ�ڿͻ�����
	CView::OnMouseMove(nFlags, point);
}

/*
 *	����м����µ���Ϣ,����ƽ��ʱ��
 */
void CCAD_PowerLandView::OnMButtonDown(UINT nFlags,CPoint point)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_MBTN_MOVE));
	ScreemToWorld(point,m_cOrigin);
	CView::OnLButtonDown(nFlags, point);
	m_bFlagMBtnMove = TRUE;
}

/*
 *	����м�˫������Ϣ//ȫ����ʾͼ��
 */
void CCAD_PowerLandView::OnMButtonDblClk(UINT nFlags,CPoint point)
{
	g_pDoc->OnFullScreemShow();	
	m_bFlagMBtnMove = -1;
}

/*
 *	����м�̨�����Ϣ,����ƽ��ʱ��
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
 *	��������ת��Ļ����
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
 *	��Ļ����ת��������
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
 *	�������ϵĻ��߰�ť�ĺ���
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
	ClearSelectLine();//�����ѡ���µķ���
	delete m_pDrawBase;
 	m_pDrawBase = NULL;
	g_pDoc->RemoveAllCollected();
	m_iLastFuntion = BtnID;//�ѹ��ܵ�ID�ż�ס
	switch(BtnID)
	{
	case ID_LINE:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n����ֱ�ߵĵ�һ��,ESC��ȡ��:");
		else
			RemindContent("\r\nnput line of the first point, the ESC key to cancel:");
		m_pDrawBase = new CDrawLine();
		break;
	case ID_POLYLINE:
		m_pDrawBase = new CDrawPolyline();
		break;
	case ID_RECTANGLE:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\nָ��һ���ǵ�[��Բ��(F)],ESC��ȡ��:");									 
		else
		RemindContent("\r\nSpecify a corner [or rounded corners (F)], the ESC key to cancel:");									 
		m_pDrawBase = new CDrawRectangle();
		break;
	case ID_ROUNDARC:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nָ��Բ����Բ��[����˳S/��N--Ĭ��˳],ESCȡ��:");
		else
			RemindContent("\r\nSpecifies the center of the circle arc,[or direction specified circular arc of the circle (S) inverse (N) - the default shun], ESC to cancel:");
		m_pDrawBase = new CDrawRoundArc();
		break;
	case ID_ROUND:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nָ��Բ��Բ��[����˳S/��N--Ĭ��˳],ESCȡ��:");
		else
			RemindContent("\r\nSpecifies the center of the circle,[or direction specified circular arc of the circle (S) inverse (N) - the default shun], ESC to cancel:");
		m_pDrawBase = new CDrawRound();
		break;
	case ID_SPLINE:
		m_pDrawBase = new CDrawSpline();
//		m_iFlagWhichBtnDown = SPLINEDOWN;
		break;
	case ID_ELLIPSE:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nָ����Բ����˵�,��ESC��ȡ��:");
		else
			RemindContent("\r\nSpecify the axis of the ellipse endpoint, or press the ESC key to cancel:");
		m_pDrawBase = new CDrawEllipse();		
		break;
	case ID_POINT:
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nָ����:");
		else
			RemindContent("\r\nInput point:");
		m_pDrawBase = new CDrawPoint();		
		break;
	case ID_VERTICAL_LINE://������
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���Ҫ��ֱ���߶�:");
		else
			RemindContent("\r\nClick on to the vertical line:");
		m_pDrawBase = new CDrawVerticalLine();
		break;
		

	default:
		break;
	}
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	
}

/*
 *	��ת�˵���İ���
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
	
	ClearSelectLine();//�����ѡ���µķ���
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
	case ID_ACCORDING_TO_LINE_HORI://������ˮƽ
		if(m_dAngleRightBtn == 0.0)
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n�Ҽ�û�е������",SEND_RICHEDIT);
			else
				RemindContent("\r\nRight click to line no",SEND_RICHEDIT);		
		}
		else
		{
			AngleTurnHoriMode(m_dAngleRightBtn);
			g_pDoc->OnRotateCenter(m_dAngleRightBtn);
		}
		break;
	case ID_ACCORDING_TO_LINE_VERT://�����ߴ�ֱ
		if(m_dAngleRightBtn == 0.0)
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n�Ҽ�û�е������",SEND_RICHEDIT);
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
 *	�������ϵ��޸İ�ť�ĺ���
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
	
	ClearSelectLine();//�����ѡ���µķ���
	delete m_pDrawBase;
	m_pDrawBase = NULL;
	m_iLastFuntion = BtnID;//�ѹ��ܵ�ID�ż�ס
	switch(BtnID)
	{	
	case ID_DELETE:
		DeleteList();
		break;
	case ID_MOVE:
		if(RemindNoSelect() == FALSE)//�����0���Ͳ��޸Ĳ���
			break;
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���������[��λ��D],��ESC��ȡ��:");
		else
			RemindContent("\r\nPlease enter the basis points, or press the ESC key to cancel:");
		m_pDrawBase = new CModifyMove();
		//�ѽ�����������������
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;
	case ID_COPY:
		if(RemindNoSelect() == FALSE)//�����0���Ͳ��޸Ĳ���
			break;
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���������,��ESC��ȡ��:");
		else
			RemindContent("\r\nPlease enter the basis points, or press the ESC key to cancel:");
		//�ѽ�����������������
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		m_pDrawBase = new CModifyCopy();
		break;
	case ID_MIRROR:
		if(RemindNoSelect() == FALSE)//�����0���Ͳ��޸Ĳ���
			break;
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nָ�������ߵĵ�һ��,��ѡ���Ƿ�ɾ��Դ����?[Y�ǣ�N��--Ĭ��N]:");
		else
			RemindContent("\r\nSpecify the mirror line first, or choose to delete the source object? [Y, N -Default N] :");
		m_pDrawBase = new CModifyMirror();
		//�ѽ�����������������
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;
	case ID_ROTATE:
		if(RemindNoSelect() == FALSE)//�����0���Ͳ��޸Ĳ���
			break;
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���������,��ESC��ȡ��,��ѡ���Ƿ�ɾ��Դ����?[Y�ǣ�N��--Ĭ��N]:");
		else
			RemindContent("\r\nPlease enter the basis points, or press the ESC key to cancel, or choose whether to delete the source object? [Y, N -Default N] :");
		m_pDrawBase = new CModifyRotate();
		//�ѽ�����������������
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;
	case ID_BREAK_TWO://һ��ͼ�δ�ϳ�����ͼ��
		m_pDrawBase = new CDrawBreakTwo();
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���ͼ�δ��,���Ҽ������˵����,��ESC��ȡ��:");
		else
			RemindContent("\r\nClick graphic interrupt, or right-click the input distance from this point, or press the ESC key to cancel:");
		break;
	case ID_OVER_CUT://����
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n������ͼ�ι���:");
		else
			RemindContent("\r\nFor all graph overcut:");		
		g_pDoc->OverCut();
		break;		
	case ID_RE_ARRANG://��������
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���°����˳������,��ESC��ȡ��:");
		else
			RemindContent("\r\nClick order again, or press the ESC key to cancel:");
		g_pDoc->RemoveAllCollected();
		m_stcSwitch.bShowNum = TRUE;
		Invalidate();
		m_pDrawBase = new CDrawArrange();
		break;	
	case ID_CHAMFER://����
		g_pDoc->RemoveAllCollected();
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n��������ߵ���н�[��ѡ�񵹽����ͣ�͹��(O)/����(I)��Ĭ��Ϊ͹��]:");	
		else
			RemindContent("\r\n[Choose chamfer type,O convex Angle/I concave Angle, the default for convex Angle]");	
		m_pDrawBase = new CDrawChamfer();
		//�ѽ�����������������
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;	
	case ID_GRIND_GLASS://ĥ��
		g_pDoc->OnGraphForGrind();
		break;	
	case ID_SELECT_BEGIN_POS://ѡ�����
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���ֱ��ѡ�����:");
		else
			RemindContent("\r\nClick the line selection starting point:");		
		m_pDrawBase = new CDrawSetBeginPos();
		break;
	case ID_IO_KNIFE://���˵�
		g_pDoc->OnGraphKinfe();
		break;
	case ID_SCALE_XY://����
		g_pDoc->OnGraphScale();
		break;
	case ID_TOGETHER://�ϲ�
		g_pDoc->OnGraphTogether();
		break;
	case ID_SAVE_TO_FILE://�ļ�����
		g_pDoc->OnFileSend();
		break;
	case ID_EXTEND_LINE://�����ӳ�
		m_pDrawBase = new CDrawExtendLine;
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n�������ӳ�����,Ȼ����Ҫ�ӳ��˵�:");
		else
			RemindContent("\r\nPlease enter the extension length, and then click to extend the endpoint:");
		//�ѽ�����������������
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;
	case ID_MATRIX://�򿪾���Ի���
		g_pDoc->OnOpenDlgMatrix();
		break;	
	case ID_MILL_GROOVES://������ͼ
		g_pDoc->OnTurnMill();
		break;
	case  ID_UNDO://����
		g_pDoc->RemoveAllCollected();
		g_pDoc->m_Undo->OnUndo();
		break;
	case  ID_REDO://�ָ�
		g_pDoc->RemoveAllCollected();
		g_pDoc->m_Undo->OnRedo();
		break;
	case  ID_MILL_SIDE://ģ�������
		g_pDoc->OnMillSide();
		break;
	case  ID_TO_LESS_POINT://�Ѷ��Ķ���߸��ݾ��ȱ���ٵ�Ķ����
		g_pDoc->OnToLessPoint();
		break;
	case  ID_ADD_RECT://�ӱ߿�
		g_pDoc->OnAddRect();
		break;
	case  ID_MEASURE_LONG://��������
		m_pDrawBase = new CDrawMeasure();
		break;
	case  ID_PARALLEL://ƽ����
//		g_pDoc->OnParallel();	
		m_pDrawBase = new CDrawParallel();
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���Ҫƽ�е�ͼ��:");
		else
			RemindContent("\r\nClick on the graphic to parallel:");
		
		break;
	case  ID_ADD_POINT://��Լӵ�
		m_pDrawBase = new CDrawAddPoint();
		break;
	case  ID_DELETE_MORE_LINE://ɾ�ص�
		g_pDoc->OnDeleteMoreLine();
		break;	

	case  ID_VIEW_MOVE://�ƶ���ͼ����
		m_pDrawBase = new CDrawMoveView();
//		m_bBreakThreadOne = TRUE;
//		m_ThreadOne = CreateThread(NULL,1,ThreadFunOne,(void*)this,CREATE_SUSPENDED,NULL);	
//		ResumeThread(m_ThreadOne);
		
		break;	
	case  ID_VIEW_FULL://ȫ����ʾͼ��
		g_pDoc->OnFullScreemShow();
	//	m_bBreakThreadOne = FALSE;
		
		break;	
	case  ID_MOVE_ORIGIN://��ͼ���ƶ���ԭ��
		g_pDoc->OnMoveOrigin();
		break;	
	case  ID_SCALE://��ͼ�ΰ���������
		if(RemindNoSelect() == FALSE)//�����0���Ͳ��޸Ĳ���
			break;
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���������:");
		else
			RemindContent("\r\nPlease enter the basis points:");
		m_pDrawBase = new CDrawZoomScale();		
		//�ѽ�����������������
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
		break;
	case  ID_SOLID_TO_DASH://���߱������
		g_pDoc->OnSolidToDash();
		break;	
	case  ID_TRIM://�޼�
		g_pDoc->RemoveAllCollected();
		m_pDrawBase = new CDrawTrim();
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���ѡ�����޼�����:");
		else
			RemindContent("\r\nClick on the selected want to trim line:");		
		break;	
	case  ID_MOVE_POINT://�ƶ���
		g_pDoc->RemoveAllCollected();
		m_pDrawBase = new CDrawMovePoint();
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���Ҫ�ƶ��ĵ�:");
		else
			RemindContent("\r\nClick to want to point to move:");		
		break;	
		
	default:
		break;
	}

}

/*
 *	����һЩ�����İ�ť�ĵ��
 */
void CCAD_PowerLandView::OnButtonParam(int BtnID)
{
	switch(BtnID)
	{
	case ID_PROPERTY://��ͼ�Ĳ���
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
 *	��ʾ������
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
		m_iFlagWhichBtnDown = COLOR;	//��־�ĸ����£����水Ťˢ����
		break;
	case ID_VIEW_DRAW:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndDrawToolBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndDrawToolBar), FALSE, TRUE); 	
		else	
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
				(&(((CMainFrame*)AfxGetMainWnd())->m_wndDrawToolBar), TRUE, TRUE); 	
		m_iFlagWhichBtnDown = DRAW;	//��־�ĸ����£����水Ťˢ����
		break;
	case ID_VIEW_MODIFY:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndModifyToolBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndModifyToolBar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndModifyToolBar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = MODIFY;	//��־�ĸ����£����水Ťˢ����
		break;
	case ID_VIEW_PROPERTY:
		if(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), FALSE, TRUE); 	
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 	
		m_iFlagWhichBtnDown = PROPERTY;	//��־�ĸ����£����水Ťˢ����
		break;
	case ID_VIEW_COMMAND:
		if(((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = COMMAND;	//��־�ĸ����£����水Ťˢ����
		break;
	case ID_VIEW_PARAM:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndParamTollbar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndParamTollbar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndParamTollbar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = COMMAND;	//��־�ĸ����£����水Ťˢ����
		break;
	case ID_VIEW_VIEW:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndViewBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndViewBar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndViewBar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = VIEW;	//��־�ĸ����£����水Ťˢ����
		break;
	case ID_VIEW_TEMPLATE:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndTemplateToolBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndTemplateToolBar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndTemplateToolBar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = VIEW;	//��־�ĸ����£����水Ťˢ����
		break;
	case ID_VIEW_GRIND_GLASS:
		if(((CMainFrame*)AfxGetMainWnd())->m_wndGrindGlassToolBar.IsWindowVisible())
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndGrindGlassToolBar), FALSE, TRUE); 
		else
			(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar	\
			(&(((CMainFrame*)AfxGetMainWnd())->m_wndGrindGlassToolBar), TRUE, TRUE); 
		m_iFlagWhichBtnDown = VIEW;	//��־�ĸ����£����水Ťˢ����
		break;
	default:
		break;
	}	
}

/*
 *	�������ϵļ������ذ���
 */
void CCAD_PowerLandView::OnButtonSwitch(int BtnID)
{	
	ClearSelectLine();//�����ѡ���µķ���
	switch(BtnID)
	{	
	case ID_SHOW_NUM://��ʾ�и�˳���
		if (m_stcSwitch.bShowNum == FALSE) 
			m_stcSwitch.bShowNum = TRUE;
		else
			m_stcSwitch.bShowNum = FALSE;
		this->Invalidate(TRUE);
		break;
	case ID_SHOW_ARROW://��ʾ�и���ͷ
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
 *	�����ť״̬����
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
	pCmdUI->SetCheck(l_iFlag);//�趨���»���̧��
}


/*
 *	�����ť״̬����
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
	pCmdUI->SetCheck(l_iFlag);//�趨��Ť���»���̧������������Ƿ����ء�
}

/*
 *	ɾ��ͼԪ����������ĵ����ɾ������ťҪʵ�ֵĹ���
 */
void CCAD_PowerLandView::DeleteList()
{	
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize() ;
	//�����ѡ���ǿյľͷ���
	if (l_iSize == 0)
		return;
	//������Ķ���,�����ǳ���
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
		//�����������ɾ������
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
	//���ѡ��
	g_pDoc->m_CollectOfPicked.RemoveAll();
	//�����ĵ������»���
	Invalidate() ;
}

/*
 *	�����ť״̬����
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
	//�Ȼ���ı�����
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItemText(IDC_EDIT_COMMAND,l_strGetCommand);
	if (l_strGetCommand == "")
	{
		if(	m_iLastFuntion != 0||m_iLastFuntion!=ID_UNDO||m_iLastFuntion!=ID_REDO)//�����¿ո��س�ʱִ����һ�εĹ���
		{
			OnButtonFix(m_iLastFuntion);
			if(m_pDrawBase == NULL)
				OnButtonDraw(m_iLastFuntion);
			return;
		}
		return;
	}
	if(m_pDrawBase)
		m_pDrawBase->OnLButtonDown(2,pos,l_strGetCommand);//1Ϊ�������2Ϊ���̵��
}
/*
 *	������ϵ����ߣ�����ߴ纯��
 */
void CCAD_PowerLandView::DrawCoord(CDC* pDC)
{
	CPoint l_ScreemOrigin;
	//����������ԭ��ת����Ļ�����
	Position l_WorldOrigin(0,0);
	WorldToScreem(l_WorldOrigin,l_ScreemOrigin);
	//�Ƿ�����ϵ
	if (g_pDoc->m_stcWorkFor.IsShowCoordinate == 1)
	{
		//��Բ�㷽��
		pDC->Rectangle(l_ScreemOrigin.x-5,l_ScreemOrigin.y-5,l_ScreemOrigin.x+5,l_ScreemOrigin.y+5);
		//��X����
		pDC->MoveTo(l_ScreemOrigin.x-4,l_ScreemOrigin.y );
		pDC->LineTo(l_ScreemOrigin.x+100,l_ScreemOrigin.y);
		pDC->MoveTo(l_ScreemOrigin.x+92,l_ScreemOrigin.y-8);
		pDC->LineTo(l_ScreemOrigin.x+100,l_ScreemOrigin.y);
		pDC->LineTo(l_ScreemOrigin.x+92,l_ScreemOrigin.y+8);
		pDC->LineTo(l_ScreemOrigin.x+92,l_ScreemOrigin.y-8);
		//��X��
		pDC->MoveTo(l_ScreemOrigin.x+115,l_ScreemOrigin.y-7);
		pDC->LineTo(l_ScreemOrigin.x+129,l_ScreemOrigin.y+7);

		pDC->MoveTo(l_ScreemOrigin.x+129,l_ScreemOrigin.y-7);
		pDC->LineTo(l_ScreemOrigin.x+115,l_ScreemOrigin.y+7);
		//��Y����
		pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y+4);
		pDC->LineTo(l_ScreemOrigin.x,l_ScreemOrigin.y-100);
		pDC->MoveTo(l_ScreemOrigin.x-8,l_ScreemOrigin.y-92);
		pDC->LineTo(l_ScreemOrigin.x,l_ScreemOrigin.y-100);
		pDC->LineTo(l_ScreemOrigin.x+8,l_ScreemOrigin.y-92);
		pDC->LineTo(l_ScreemOrigin.x-8,l_ScreemOrigin.y-92);
		//��Y��
		pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y-115);
		pDC->LineTo(l_ScreemOrigin.x,l_ScreemOrigin.y-125);
		pDC->LineTo(l_ScreemOrigin.x+5,l_ScreemOrigin.y-135);

		pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y-125);
		pDC->LineTo(l_ScreemOrigin.x-5,l_ScreemOrigin.y-135);
	}
	//����̨,�Ƿ���ʾ��̨
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
		// �ָ�ԭ���Ļ��� 
		pDC->SelectObject(pOldPen); 
		pen.DeleteObject();//�ͷ�pen��Create��;
	}
	//�Ƿ���ʾ���
	if (g_pDoc->m_stcWorkFor.IsShowRulerLine == 1)
	{	
		pDC->SetTextAlign(TA_CENTER);//���־���		
		CPoint l_pot(0,0);
		Position l_pos(0,0);
		CString l_strNum;		
		//��X��
		pDC->MoveTo(l_ScreemOrigin.x-1000,l_ScreemOrigin.y);
		pDC->LineTo(l_ScreemOrigin.x+1000,l_ScreemOrigin.y);
		//���̶ȼ���
		for(int i = 0;i<200;i++)
		{
			//������
			if (i%10==0 && i!=0)//��10����һ�����̶�
			{
				pDC->MoveTo(l_ScreemOrigin.x+i*5,l_ScreemOrigin.y);
				pDC->LineTo(l_ScreemOrigin.x+i*5,l_ScreemOrigin.y+10);	
				l_pot.x = l_ScreemOrigin.x+i*5;
				l_pot.y = l_ScreemOrigin.y;
				ScreemToWorld(l_pot,l_pos);
				l_strNum.Format("%.f",l_pos.x);
				pDC->TextOut(l_ScreemOrigin.x+i*5,l_ScreemOrigin.y+10,l_strNum);				
			}
			else//��С�̶�
			{
				pDC->MoveTo(l_ScreemOrigin.x+i*5,l_ScreemOrigin.y);
				pDC->LineTo(l_ScreemOrigin.x+i*5,l_ScreemOrigin.y+5);
			}
			//������
			if (i%10==0 && i!=0)//��10����һ�����̶�
			{
				pDC->MoveTo(l_ScreemOrigin.x-i*5,l_ScreemOrigin.y);
				pDC->LineTo(l_ScreemOrigin.x-i*5,l_ScreemOrigin.y+10);	
				l_pot.x = l_ScreemOrigin.x-i*5;
				l_pot.y = l_ScreemOrigin.y;
				ScreemToWorld(l_pot,l_pos);
				l_strNum.Format("%.f",l_pos.x);
				pDC->TextOut(l_ScreemOrigin.x-i*5,l_ScreemOrigin.y+10,l_strNum);				
			}
			else//��С�̶�
			{
				pDC->MoveTo(l_ScreemOrigin.x-i*5,l_ScreemOrigin.y);
				pDC->LineTo(l_ScreemOrigin.x-i*5,l_ScreemOrigin.y+5);
			}
		}
		//��Y��
		pDC->SetTextAlign(TA_RIGHT);//���־���			
		pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y-800);
		pDC->LineTo(l_ScreemOrigin.x,l_ScreemOrigin.y+800);
		//���̶ȼ���
		for(int j = 0;j<160;j++)
		{
			//������
			if (j%10==0 && j!=0)//��10����һ�����̶�
			{
				pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y+j*5);
				pDC->LineTo(l_ScreemOrigin.x-10,l_ScreemOrigin.y+j*5);	
				l_pot.x = l_ScreemOrigin.x;
				l_pot.y = l_ScreemOrigin.y+j*5;
				ScreemToWorld(l_pot,l_pos);
				l_strNum.Format("%.f",l_pos.y);
				pDC->TextOut(l_ScreemOrigin.x-15,l_ScreemOrigin.y+j*5-8,l_strNum);				
			}
			else//��С�̶�
			{
				pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y+j*5);
				pDC->LineTo(l_ScreemOrigin.x-5,l_ScreemOrigin.y+j*5);
			}
			//������
			if (j%10==0 && j!=0)//��10����һ�����̶�
			{
				pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y-j*5);
				pDC->LineTo(l_ScreemOrigin.x-10,l_ScreemOrigin.y-j*5);	
				l_pot.x = l_ScreemOrigin.x;
				l_pot.y = l_ScreemOrigin.y-j*5;
				ScreemToWorld(l_pot,l_pos);
				l_strNum.Format("%.f",l_pos.y);
				pDC->TextOut(l_ScreemOrigin.x-15,l_ScreemOrigin.y-j*5-8,l_strNum);				
			}
			else//��С�̶�
			{
				pDC->MoveTo(l_ScreemOrigin.x,l_ScreemOrigin.y-j*5);
				pDC->LineTo(l_ScreemOrigin.x-5,l_ScreemOrigin.y-j*5);
			}
		}		
	}
}


//�������ű���ϵ��
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
	return TRUE;		//���������
}

void CCAD_PowerLandView::ClearSelectLine()
{
	if(m_iFlagWhichBtnDown == DRAW_SELECT)
	{
		CDC*	pDC = GetDC(); 	
		// ��������һ����Ƥ��
		CCreateRectangle*	pTempLine2 = new CCreateRectangle(m_PosBegin,m_PosLast);	
		pTempLine2->DrawMove(pDC,dmDrag,FALSE,FALSE);
		delete pTempLine2;
		// �ͷ��豸����ָ��
		ReleaseDC(pDC); 
		m_iFlagWhichBtnDown = MEIYOU;
		delete m_pDrawBase;
		m_pDrawBase = NULL;
	}
}

/*
 *	��ʾû�е���ѡ��ͼ��
 */
BOOL CCAD_PowerLandView::RemindNoSelect()
{
	//���ѡ�񼯴�С�������ã������0���Ͳ��޸Ĳ���
	int l_iCount = g_pDoc->m_CollectOfPicked.GetSize();
	if(l_iCount == 0)//�����0���Ͳ��޸Ĳ���
	{
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû��ѡ�У�����ѡ��");
		else
			RemindContent("\r\nNot selected, choose again��");			
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
	//���ѡ��Ϊ�գ�������
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
	GetCursorPos(&p);//�������Ļ����  
	CMenu m_Menu;  
	m_Menu.LoadMenu(IDR_MENU_CILENT_RBTNDOW);//�༭�õĲ˵���Դ  
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
	//���ѡ��Ϊ�գ�������
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
	GetCursorPos(&p);//�������Ļ����  
	CMenu m_Menu;  
	m_Menu.LoadMenu(IDR_MENU_CILENT_RBTNDOW);//�༭�õĲ˵���Դ  
	CMenu *m_SubMenu = m_Menu.GetSubMenu(0); 
	m_SubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, p.x, p.y, this);  
	

	CView::OnRButtonUp(nFlags, point);
}

/*
 *	�Ƕȱ�ɰ���ˮƽӦ�õĽǶ�
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
 *	�Ƕȱ�ɰ��ߴ�ֱӦ�õĽǶ�
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



