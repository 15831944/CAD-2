// DlgAddPoint.cpp : implementation file
//

#include "stdafx.h"
#include "cad_powerland.h"
#include "DlgAddPoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddPoint dialog


CDlgAddPoint::CDlgAddPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddPoint::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddPoint)
	m_dEditLongHori = 0.0;
	m_dEditLongVert = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgAddPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddPoint)
	DDX_Text(pDX, IDC_EDIT_LONG_HORI, m_dEditLongHori);
	DDX_Text(pDX, IDC_EDIT_LONG_VERT, m_dEditLongVert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddPoint, CDialog)
	//{{AFX_MSG_MAP(CDlgAddPoint)
	ON_BN_CLICKED(ID_ADD_POINT_OK, OnAddPointOk)
	ON_BN_CLICKED(ID_ADD_POINT_CANCEL, OnAddPointCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddPoint message handlers

void CDlgAddPoint::OnAddPointOk() 
{
	// TODO: Add your control notification handler code here
	if (!UpdateData())
		return;
	CDC* pDC = g_pView->GetDC();
	Position pos;
	pos.x = m_posBegin.x + m_dEditLongHori;
	pos.y = m_posBegin.y + m_dEditLongVert;
	//判断点是否在线上
	POSITION POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase = NULL;
	BOOL l_bIsOnLine = FALSE;//点是否在线上
	while (POS)
	{
		l_pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(POS);
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine* pLine = (CCreateLine*)l_pCreateBase;
			if(PosOnLine(pLine->m_Begin,pLine->m_End,pos))//如果点在线上
			{
				l_bIsOnLine = TRUE;
				break;
			}
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
			||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreatePolyline* pLine = (CCreatePolyline*)l_pCreateBase;
			if(PosOnAryPoint(pLine->m_point,pLine->m_dTuDu,pos))//如果点在线上
			{
				l_bIsOnLine = TRUE;
				break;
			}
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle* pLine = (CCreateRectangle*)l_pCreateBase;
			if(PosOnAryPoint(pLine->m_point,pLine->m_dTuDu,pos))//如果点在线上
			{
				l_bIsOnLine = TRUE;
				break;
			}
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{
			CCreateRound* pLine = (CCreateRound*)l_pCreateBase;
			if(pos.Distance(pLine->m_Center) == pLine->m_dRadius)//如果点在线上
			{
				l_bIsOnLine = TRUE;
				break;
			}
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
			CCreateRoundArc* pArc = (CCreateRoundArc*)l_pCreateBase;
			if(PosOnArc(pos,pArc->m_Begin,pArc->m_End,pArc->m_Center,pArc->m_dRadius,pArc->m_iDirect))//如果点在线上
			{
				l_bIsOnLine = TRUE;
				break;
			}
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{
			CCreateRectangle* pLine = (CCreateRectangle*)l_pCreateBase;
			if(PosOnAryPoint(pLine->m_point,pLine->m_dTuDu,pos))//如果点在线上
			{
				l_bIsOnLine = TRUE;
				break;
			}
		}
		
	}
	//如果点在线上
	if(l_bIsOnLine == TRUE)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点在线上，无效的点:");
		else
			RemindContent("\r\nPoint on line, invalid points:");
		CDialog::OnOK();	
		return;		
	}
	CCreatePoint* pPoint = new CCreatePoint(pos);
	g_pDoc->m_CCreateBaseList.AddTail(pPoint);
	pPoint->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
	g_pView->ReleaseDC(pDC);
//	把这个操作添加到撤消链表
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = (long)pPoint;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
	CDialog::OnOK();	
}

void CDlgAddPoint::OnAddPointCancel() 
{
	// TODO: Add your control notification handler code here

	CDialog::OnCancel();
}

BOOL CDlgAddPoint::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_dEditLongHori = m_posEnd.x - m_posBegin.x;	
	m_dEditLongVert = m_posEnd.y - m_posBegin.y;
	
	CString str;
	str.Format("%.4f",m_dEditLongHori);
	SetDlgItemText(IDC_EDIT_LONG_HORI,str);
	str.Format("%.4f",m_dEditLongVert);
	SetDlgItemText(IDC_EDIT_LONG_VERT,str);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
 *	点是否在给定的点集里
 */
BOOL CDlgAddPoint::PosOnAryPoint(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,Position posClick)
{
	int l_iSize = aryPoint.GetSize()-1;
	Position l_posBegin,l_posEnd,l_posCenter;
	double l_dTudu,l_dRadius;
	for(int i=0;i<l_iSize;i++)
	{
		l_posBegin = aryPoint.GetAt(i);
		l_posEnd = aryPoint.GetAt(i+1);
		l_dTudu = aryTudu.GetAt(i);
		if (l_dTudu == 0.0 )//说明是直线，看点是否在直线上
		{
 			if(PosOnLine(l_posBegin,l_posEnd,posClick,0.01))
				return TRUE;
		}
		else//说明是圆弧，看点是否在上面
		{
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);
			l_dRadius = l_posCenter.Distance(l_posBegin);		
			if(PosOnArc(posClick,l_posBegin,l_posEnd,l_posCenter,l_dRadius,RtnArcDir(l_dTudu)))
				return TRUE;			
		}
	}
	return FALSE;
}



