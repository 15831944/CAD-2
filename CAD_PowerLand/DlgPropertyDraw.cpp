// DlgPropertyDraw.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgPropertyDraw.h"
#include "DrawBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPropertyDraw dialog


CDlgPropertyDraw::CDlgPropertyDraw(CWnd* pParent /*=NULL*/)
//	: CDialog(CDlgPropertyDraw::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPropertyDraw)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPropertyDraw::DoDataExchange(CDataExchange* pDX)
{
//	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPropertyDraw)
	DDX_Control(pDX, IDC_COMBO_COLOR, m_ComboColor);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CDlgPropertyDraw, CDialogBar)
	//{{AFX_MSG_MAP(CDlgPropertyDraw)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOR, OnSelchangeComboColor)//组合框里的内容改变时的事件
	ON_MESSAGE(WM_INITDIALOG,OnInitDialog)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDlgPropertyDraw message handlers



BOOL CDlgPropertyDraw::OnInitDialog() 
{
//	CDialogBar::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!UpdateData(FALSE))
		TRACE("初始化属性对话框有问题！");
	m_ComboColor.InsertString(0,g_pDoc->m_stcColorToM.m_ColorMean1);
	m_ComboColor.InsertString(1,g_pDoc->m_stcColorToM.m_ColorMean2);
	m_ComboColor.InsertString(2,g_pDoc->m_stcColorToM.m_ColorMean3);
	m_ComboColor.InsertString(3,g_pDoc->m_stcColorToM.m_ColorMean4);
	m_ComboColor.InsertString(4,g_pDoc->m_stcColorToM.m_ColorMean5);
	m_ComboColor.InsertString(5,"");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
 *	组合框里的内容改变时的事件
 */
void CDlgPropertyDraw::OnSelchangeComboColor() 
{
	// TODO: Add your control notification handler code here
	int l_iCurColor = m_ComboColor.GetCurSel();//返回当前的颜色
	CString l_strCBM,l_strSBM;
	COLORREF l_cCurColor;
	if (l_iCurColor == 5)
		return;
	if(l_iCurColor == 0)
	{
		l_cCurColor = g_pDoc->m_stcColorToM.m_color1;
		l_strCBM = g_pDoc->m_stcColorToM.m_CBM1;
		l_strSBM = g_pDoc->m_stcColorToM.m_SBM1;
	}
	if(l_iCurColor == 1)
	{
		l_cCurColor = g_pDoc->m_stcColorToM.m_color2;
		l_strCBM = g_pDoc->m_stcColorToM.m_CBM2;
		l_strSBM = g_pDoc->m_stcColorToM.m_SBM2;
	}
	if(l_iCurColor == 2)
	{
		l_cCurColor = g_pDoc->m_stcColorToM.m_color3;
		l_strCBM = g_pDoc->m_stcColorToM.m_CBM3;
		l_strSBM = g_pDoc->m_stcColorToM.m_SBM3;
	}
	if(l_iCurColor == 3) 
	{
		l_cCurColor = g_pDoc->m_stcColorToM.m_color4;
		l_strCBM = g_pDoc->m_stcColorToM.m_CBM4;
		l_strSBM = g_pDoc->m_stcColorToM.m_SBM4;
	}
	if(l_iCurColor == 4)
	{
		l_cCurColor = g_pDoc->m_stcColorToM.m_color5;
		l_strCBM = g_pDoc->m_stcColorToM.m_CBM5;
		l_strSBM = g_pDoc->m_stcColorToM.m_SBM5;
	}
	//选择集的大小 
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	if (l_iSize == 0)
		return;
	//撤消
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = IDC_COMBO_COLOR;	
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;
	
	CDC* pDC = g_pView->GetDC();
	for( int i = 0 ; i < l_iSize; i++ )
	{
		CCreateBase*	pSelEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i] ;
		//把要改变的图形放到撤消集里
		if (i == 0)
		{
			CurGraphPos->pCreateBasepos = (long)pSelEntity;
			CurGraphPos->iData1 = pSelEntity->m_ColorOfLine;
		}
		else
		{
			GraphPos = new GRAPHPOS;
			memset(GraphPos,0,sizeof(GRAPHPOS));						
			GraphPos->pCreateBasepos = (long)pSelEntity;
			GraphPos->iData1 = pSelEntity->m_ColorOfLine;			
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		}		
		//变色
		//如果颜色能对应上，就替换M代码;如果对应不上，就添加M代码
		if(!g_pDoc->ReplaceMCode(pSelEntity->m_ColorOfLine,pSelEntity->m_strSBM,l_strSBM,pSelEntity->m_strCBM,l_strCBM))
		{
			pSelEntity->m_strSBM.Add(l_strSBM);
			pSelEntity->m_strCBM.Add(l_strCBM);
		}
		pSelEntity->m_ColorOfLine = l_cCurColor;
		pSelEntity->DrawMove(pDC,dmSelect,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
	}
	pUndoList->pOperatieParam = l_UndoRedo.NewOperateParam();
	pUndoList->pOperatieParam->direction = l_cCurColor;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
	
	g_pView->ReleaseDC(pDC);
}

