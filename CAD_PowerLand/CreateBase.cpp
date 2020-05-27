// CreateBase.cpp: implementation of the CCreateBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "CreateBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCreateBase, CObject, 0)
CCreateBase::CCreateBase()
{
	m_dRemainingDash = 0.0;
}

CCreateBase::~CCreateBase()
{

}

void CCreateBase::DrawRectForPick(CDC* pDC,CPoint Point)
{
	CPoint p1(Point.x+4,Point.y+4);
//	CPoint p2(Point.x-4,Point.y+4);
	CPoint p3(Point.x-4,Point.y-4);
//	CPoint p4(Point.x+4,Point.y-4);
	pDC->Rectangle(p3.x,p3.y,p1.x,p1.y);
	pDC->SelectStockObject(HOLLOW_BRUSH) ;	//把中间添黑
}

// 根据绘图模式设置绘图环境：设置设备环境的模式并创建画笔
void CCreateBase::SetDrawEnvir(CDC*	pDC, int drawMode, CPen* pPen)
{
	int			lineStyle, lineWidth;
	COLORREF	color;
	switch(drawMode) 
	{
	case dmSelect: // 选中状态
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_DOT;
			lineWidth = 1;
			color = m_ColorOfLine;//RGB(0,0,0);
			break;
		}
	case dmPrompt: // 提示状态
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_DASH;
			lineWidth = 1;
			color = m_ColorOfLine;
			break;
		}
	case dmDrag: // 拖动状态
		{
			pDC->SetROP2(R2_XORPEN);
			lineStyle = PS_SOLID;
			lineWidth = 1;
			color = RGB(190,190,0);
			break;
		}
	case dmInvalid: // 擦除状态
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_SOLID;
			lineWidth = 1;
			color = RGB(255,255,255);; // 用背景色画
			break;
		}
	case dmLayer: // 图层链表里的画图
		{
			pDC->SetROP2(R2_XORPEN);
			lineStyle = PS_SOLID;//PS_DASHDOT;
			lineWidth = 1;
			color = RGB(55,55,55); // 用背景色画
			break;
		}
	case dmDragSelect1: // 框选1
		{
			pDC->SetROP2(R2_XORPEN);
			lineStyle = PS_SOLID;
			lineWidth = 1;
			color = RGB(255,55,55);
			break;
		}
	case dmDragSelect2: // 框选2
		{
			pDC->SetROP2(R2_XORPEN);
			lineStyle = PS_DOT;
			lineWidth = 1;
			color = RGB(0,0,0);
			break;
		}
	case dmNormal:   
	default:
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_SOLID;
			lineWidth = 1;
			color = m_ColorOfLine;
			break;
		}		
	}
	pPen->CreatePen(lineStyle,lineWidth,color) ;
}

/*
 *	返回线色号如，黑色1，红色2.。。
 */
int CCreateBase::RtnColorInt()
{
	//如果是磨玻璃就要把最后一刀的G代码换成最后的抬下刀
	if(m_iIsBeginGraph == 1&&g_pDoc->m_stcGlassGrind.RouphCount != 0 && g_pDoc->m_stcWorkFor.IsGlassGrind == 1)
	{
		if(g_pDoc->m_stcGlassGrind.PerfectCount == 0 )
			return 1;
		if(g_pDoc->m_stcGlassGrind.PolishCount == 0)
			return 2;
		else 
			return 3;
	}		
	//一个图形两边的M代码
	CString l_strOneM,l_strTwoM;
	//两个M代码集的大小 
	int l_iSizeOne = m_strSBM.GetSize();
	int l_iSizeTwo = m_strCBM.GetSize();
	for(int i=0;i<l_iSizeOne;i++)
	{
		l_strOneM = m_strSBM.GetAt(i);
		for(int j=0;j<l_iSizeTwo;j++)
		{
			l_strTwoM = m_strCBM.GetAt(j);
			if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM1&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM1)	
				return 1;
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM2&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM2)
				return 2;
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM3&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM3)
				return 3;
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM4&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM4)
				return 4;
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM5&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM5)
				return 5;
		}
	}
	return 6;
}

/*
 *	看下刀库里是否与字符串相等
 */
BOOL CCreateBase::MstrIsEqualMary(CString strM,CArray<CString,CString&>& aryM)
{
	int l_iSizeofarym = aryM.GetSize();
	CString l_strMCode="";
	//看字符串组里面包含M代码吗？
	for(int i=0;i<l_iSizeofarym;i++)
	{
		l_strMCode = aryM.GetAt(i);
		if(l_strMCode == strM)
			return TRUE;
	}
	return FALSE;
}



/*
 *	让属性对话框中的控件显示个数
 *	IDC_EDIT1 1018
 *	IDC_STATIC1 1026
 */					   //显示几个EDIT,是否显示按扭
void CCreateBase::ShowControl(int show,BOOL btn)
{
	int i = 0;
	for(i = 0;i<show;i++)//显示Edit和STATIC
	{
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(i+1018))->ShowWindow(SW_SHOW);
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(i+1026))->ShowWindow(SW_SHOW);
	}
	for( ;i<8 ;i++)//隐藏Edit和STATIC
	{
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(i+1018))->ShowWindow(SW_HIDE);
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(i+1026))->ShowWindow(SW_HIDE);
	}
	if (btn == TRUE)	//TRUE时要显示按扭
	{
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(IDC_BTN_FORWARD))->ShowWindow(SW_SHOW);
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(IDC_BTN_BEHIND))->ShowWindow(SW_SHOW);
	}
	else
	{
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(IDC_BTN_FORWARD))->ShowWindow(SW_HIDE);
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(IDC_BTN_BEHIND))->ShowWindow(SW_HIDE);
	}
}

/*
 *	画箭头函数
 */
void CCreateBase::DrawArrow(double angle,CPoint PosEnd,CDC* pDC)
{
	double d_Sin1 = 10 * sin(angle+MATH_PI/6);
	double d_Cos1 = 10 * cos(angle+MATH_PI/6);	
	double d_Sin2 = 10 * sin(angle-MATH_PI/6);
	double d_Cos2 = 10 * cos(angle-MATH_PI/6);
	CPoint pt[3];
	pt[0] = PosEnd;
	pt[1].x = PosEnd.x - (int)d_Cos1;
	pt[1].y = PosEnd.y + (int)d_Sin1;
	pt[2].x = PosEnd.x - (int)d_Cos2;
	pt[2].y = PosEnd.y + (int)d_Sin2;
	
	pDC->MoveTo(PosEnd);
	pDC->LineTo(pt[1]);
	
	pDC->MoveTo(pt[2]);
	pDC->LineTo(PosEnd);
}

void CCreateBase::DrawMove(CDC *pDC,int DrawMode,BOOL FlagShowArrow /* = FALSE */,BOOL FlagShowNum /* = FALSE */)
{
	int aaa= 0;
}

/*
 *	用直线画圆弧函数
 */
void CCreateBase::DrawArc(CDC* pDC,CPoint potCenter,double dRadius,double dAngleStart,double dAngleOffset)
{
	//周长
	double l_dPerimeter =dRadius * MATH_PI * 2;
	double l_dAngleOfArc = dAngleOffset/fabs(l_dPerimeter/3*dAngleOffset/360);//圆弧数
	CPoint l_potArc;//圆弧的每个结点
	double l_dAngleStart = dAngleStart + l_dAngleOfArc;
	for(double i = l_dAngleStart;fabs(i-dAngleOffset-dAngleStart)>=fabs(l_dAngleOfArc);i += l_dAngleOfArc)
	{
		l_potArc.x = potCenter.x + dRadius*cos(i/360*MATH_2PI);
		l_potArc.y = potCenter.y - dRadius*sin(i/360*MATH_2PI);
		pDC->LineTo(l_potArc);
	}
	i = dAngleOffset+dAngleStart;
	l_potArc.x = potCenter.x + dRadius*cos(i/360*MATH_2PI);
	l_potArc.y = potCenter.y - dRadius*sin(i/360*MATH_2PI);
	pDC->LineTo(l_potArc);
}

/*
 *	属性对话框里的编辑框改变时的撤消
 */
void CCreateBase::AddUndoList(double dEditNew,double dEditOld,int iWhichEdit,int iNumOfTop/* =0 */)
{
	//向撤消链表里添加操作
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = IDC_BTN_FORWARD;
	
	pUndoList->pOperatieParam = l_UndoRedo.NewOperateParam();
	pUndoList->pOperatieParam->angle = dEditNew;
	pUndoList->pOperatieParam->direction = iWhichEdit;
	pUndoList->pOperatieParam->distance = dEditOld;
	pUndoList->pOperatieParam->pPosition = iNumOfTop;

	pUndoList->pGraphPos  = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = (LONG)this;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
	return;
}

/*
 *	把圆弧变成虚线
 */
PGRAPHPOS CCreateBase::TurnArcDash(double dLongSolid,double dLongEmpty,Position posCenter,double dAngleStart,
							  double dAngeleEnd,double dRadius,int iDirect)
{
	Position l_posBegin,l_posEnd;
	double l_dPerimeter = MATH_PI*dRadius*2;//周长
	//存虚线的链表
	CUndoRedo l_UndoRedo;
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = GraphPos =  l_UndoRedo.NewGraphPos();	
	//线的角度
	double l_dAngleSolid = MATH_2PI/(MATH_PI*dRadius*2/dLongSolid);
	double l_dAngleEmpty = MATH_2PI/(MATH_PI*dRadius*2/dLongEmpty);;//线的角度
	POSITION POS = g_pDoc->m_CCreateBaseList.Find(this);
	if(iDirect == AD_CLOCKWISE)//顺时针
	{
		if(dAngeleEnd >= dAngleStart)//当过零度线时
			dAngeleEnd = dAngeleEnd -360;
		for(double l_dDash=dAngleStart/180*MATH_PI;l_dDash>dAngeleEnd/180*MATH_PI;l_dDash=l_dDash-l_dAngleSolid-l_dAngleEmpty)
		{
			l_posBegin.x = posCenter.x + cos(l_dDash)*dRadius;
			l_posBegin.y = posCenter.y + sin(l_dDash)*dRadius;
			if((l_dDash-l_dAngleSolid)< (dAngeleEnd/180*MATH_PI))//到最后一个线段时
				l_dAngleSolid = l_dDash - (dAngeleEnd/180*MATH_PI);
			l_posEnd.x = posCenter.x + cos(l_dDash-l_dAngleSolid)*dRadius;
			l_posEnd.y = posCenter.y + sin(l_dDash-l_dAngleSolid)*dRadius;
			CCreateRoundArc* pArc= new CCreateRoundArc(l_posBegin,posCenter,l_posEnd,iDirect);
			g_pDoc->m_CCreateBaseList.InsertBefore(POS,pArc);
			//向撤消链表里存图形
			if(CurGraphPos->pCreateBasepos == NULL)
			{
				CurGraphPos->pCreateBasepos = (long)pArc;
			}
			else
			{
				GraphPos = l_UndoRedo.NewGraphPos();
				GraphPos->pCreateBasepos = (long)pArc;
				CurGraphPos->pNext = GraphPos;
				GraphPos->pPrev = CurGraphPos;
				CurGraphPos = GraphPos;
			}
		}
	}
	else//逆时针 
	{
		if(dAngeleEnd <= dAngleStart)//当过零度线时
			dAngeleEnd = dAngeleEnd +360;
		for(double l_dDash=dAngleStart/180*MATH_PI;l_dDash<dAngeleEnd/180*MATH_PI;l_dDash=l_dDash+l_dAngleSolid+l_dAngleEmpty)
		{
			l_posBegin.x = posCenter.x + cos(l_dDash)*dRadius;
			l_posBegin.y = posCenter.y + sin(l_dDash)*dRadius;
			if((l_dDash+l_dAngleSolid)> (dAngeleEnd/180*MATH_PI))//到最后一个线段时
				l_dAngleSolid = (dAngeleEnd/180*MATH_PI)-l_dDash;
			l_posEnd.x = posCenter.x + cos(l_dDash+l_dAngleSolid)*dRadius;
			l_posEnd.y = posCenter.y + sin(l_dDash+l_dAngleSolid)*dRadius;
			CCreateRoundArc* pArc= new CCreateRoundArc(l_posBegin,posCenter,l_posEnd,iDirect);
			g_pDoc->m_CCreateBaseList.InsertBefore(POS,pArc);
			//向撤消链表里存图形
			if(CurGraphPos->pCreateBasepos == NULL)
			{
				CurGraphPos->pCreateBasepos = (long)pArc;
			}
			else
			{
				GraphPos = l_UndoRedo.NewGraphPos();
				GraphPos->pCreateBasepos = (long)pArc;
				CurGraphPos->pNext = GraphPos;
				GraphPos->pPrev = CurGraphPos;
				CurGraphPos = GraphPos;
			}
		}		
	}
	return headGraphPos;	
}


/*
 *	把直线变成虚线
 */
PGRAPHPOS CCreateBase::TurnLineDash(double dLongSolid,double dLongEmpty,Position posBegin,Position PosEnd)
{
	Position l_posBegin,l_posEnd;
	double l_dLongLine = posBegin.Distance(PosEnd);//线的长度
	double l_dAngleLine = GetAngleToXAxis(posBegin,PosEnd);//线的角度
	//存虚线的链表
	CUndoRedo l_UndoRedo;
	PGRAPHPOS CurGraphPos,GraphPos,headGraphPos = NULL;
	headGraphPos = CurGraphPos = GraphPos =  l_UndoRedo.NewGraphPos();
	//把直线变成虚线
	POSITION POS = g_pDoc->m_CCreateBaseList.Find(this);
	for(double l_dDash=0.0;l_dDash<l_dLongLine;l_dDash=l_dDash+dLongSolid+dLongEmpty)
	{
		if(m_dRemainingDash < 0&&l_dDash==0.0)//说明空白的长度要在这个线段的起点后面//求起点
		{
			if(-m_dRemainingDash > l_dLongLine)//如果新线段长度还不够空白长
			{
				l_dDash = -m_dRemainingDash;
				m_dRemainingDash = 0.0;
				break;
			}
			l_posBegin.x = posBegin.x + cos(l_dAngleLine)*(-m_dRemainingDash);
			l_posBegin.y = posBegin.y + sin(l_dAngleLine)*(-m_dRemainingDash);		
		}
		else
		{
			l_posBegin.x = posBegin.x + cos(l_dAngleLine)*l_dDash;
			l_posBegin.y = posBegin.y + sin(l_dAngleLine)*l_dDash;
		}
		if((l_dDash+dLongSolid - m_dRemainingDash) > l_dLongLine)//如果剩下的长度不够一个实线长
		{
			m_dRemainingDash = l_dLongLine - l_dDash + m_dRemainingDash;
			m_aryDash.Add(l_posBegin);
		}
		else
		{
			if(l_dDash == 0.0)
				l_dDash -= m_dRemainingDash;
			//求末点
			l_posEnd.x = posBegin.x + cos(l_dAngleLine)*(l_dDash+dLongSolid);
			l_posEnd.y = posBegin.y + sin(l_dAngleLine)*(l_dDash+dLongSolid);
			m_aryDash.Add(l_posBegin);
			m_aryDash.Add(l_posEnd);
			CCreatePolyline* pLine = new CCreatePolyline(m_aryDash);
			g_pDoc->m_CCreateBaseList.InsertBefore(POS,pLine);
			m_aryDash.RemoveAll();							
			//向撤消链表里存图形
			if(CurGraphPos->pCreateBasepos == NULL)
			{
				CurGraphPos->pCreateBasepos = (long)pLine;
			}
			else
			{
				GraphPos = l_UndoRedo.NewGraphPos();
				GraphPos->pCreateBasepos = (long)pLine;
				CurGraphPos->pNext = GraphPos;
				GraphPos->pPrev = CurGraphPos;
				CurGraphPos = GraphPos;
			}
			m_dRemainingDash = 0.0;			
		}
	}
	if(m_dRemainingDash == 0.0)//说明线段剩余的长度有可能没有空白长度长
	{
		m_dRemainingDash = l_dLongLine - l_dDash;
	}
	if(headGraphPos->pCreateBasepos == NULL)//如果线段太短没有生成图形
	{
		delete headGraphPos;
		headGraphPos = NULL;
	}
	return headGraphPos;
}









