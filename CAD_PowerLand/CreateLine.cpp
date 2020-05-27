// CreateLine.cpp: implementation of the CCreateLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "CreateBase.h"
#include "CAD_PowerLand.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCreateLine, CCreateBase, 0)
CCreateLine::CCreateLine()
{

}

CCreateLine::CCreateLine(const Position& begin,const Position& end)
{
	m_ColorOfLine = g_CurColor;		//创建时的画色就是当前所选色
	m_Begin = begin;
	m_End = end;
	m_iKnifeStyle = 41;//刀补类型 41左刀补 42右刀补
	m_iIsBeginGraph = 2;//0不是原始图形，1是原始图形，2是保存时不关心
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
}

CCreateLine::~CCreateLine()
{

}

/*//曾经用DGI+画的函数
void CCreateLine::Draw(CDC *pDC)
{	//把世界坐标转成屏幕坐标
	CPoint  l_ScreemPtO,l_ScreemPtT;
	g_pView->WorldToScreem(m_Begin,l_ScreemPtO);
	g_pView->WorldToScreem(m_End,l_ScreemPtT);	
	//建立GDI+的对象
	Graphics myGraphics(pDC->m_hDC);
	myGraphics.TranslateTransform(m_dMoveX,m_dMoveY);
	Pen myPen(Color(0, 0 ,0),1); 
	myGraphics.DrawLine(&myPen,l_ScreemPtO.x,l_ScreemPtO.y,l_ScreemPtT.x,l_ScreemPtT.y); 
	myGraphics.TranslateTransform(0,0);
	GraphicsPath aaa;
	aaa.AddEllipse(100,100,200,300);
}*/


void CCreateLine::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{
	CPoint pt_begin, pt_end; // 屏幕坐标的起点和终点
	// 将世界坐标转化为屏幕坐标
	g_pView->WorldToScreem(m_Begin,pt_begin); 
	g_pView->WorldToScreem(m_End,pt_end) ;
	
	// 得到原来的绘图模式
	int		n = GetROP2(pDC->GetSafeHdc()); 
	// 创建画笔的原则：
	// 如果在正常的绘图模式下，使用成员变量创建画笔
	// 如果是其它的模式,使用全局函数"SetDrawEnvir"创建画笔
	CPen	pen; 
	if (DrawMode == dmNormal)		
		pen.CreatePen(PS_SOLID,1,m_ColorOfLine);//如果是正常画图模式，线色就要取决于所选色
	else
		SetDrawEnvir(pDC, DrawMode, &pen);	//调用SetDrawEnvir函数设置绘图环境
	CPen* pOldPen = pDC->SelectObject(&pen); // 得到原来的画笔
	// 根据屏幕坐标绘制图元

	pDC->MoveTo(pt_begin); 
	pDC->LineTo(pt_end);
	if (FlagShowArrow)	//画方框和箭头
	{
		//画起始点的方框
		CRect rect(pt_begin.x-5,pt_begin.y-5,pt_begin.x+5,pt_begin.y+5);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
		
		double angle = GetAngleToXAxis(m_Begin,m_End);
		DrawArrow(angle,pt_end,pDC);//画箭头的函数 
	}
///////////////////////遍历链表里的看自己排在第几位，刀顺时用然后输出到界面 上////////
	if (FlagShowNum)
	{
		POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
		int l_iNumOfMCode = 0;
		for (int i=0;l_ListPos != NULL;i++)//遍历链表，i可表示链表里的排序号
		{
			CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
			if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))//看前面有多少个M代码
				l_iNumOfMCode++;
			if(this == (CCreateLine *)l_pDelList)//如果与链表里的元素一样，就要记下位置
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //这样就可以透明化背景了				
				pDC->TextOut(pt_begin.x,pt_begin.y,str);//输出到界面 上
				pDC->SetBkMode(l_iOldMode);
				break;
			}
		}
	}
//////////////////////////////////////////////////////////////////////////		
	// 恢复原来的画笔 
	pDC->SelectObject(pOldPen); 
	// 恢复原来的绘图模式
	pDC->SetROP2(n);
	pen.DeleteObject();//释放pen的Create的;
}

int CCreateLine::IsPick(CPoint point)
{
	CPoint pt_begin, pt_end; // 屏幕坐标的起点和终点
	Position pos;
	g_pView->ScreemToWorld(point,pos);	
	//获得缩放比例
	double l_dProportion = g_pView->ReturnProportion();	
	//如果距离起点或末点的距离满足就算拾取到，相当起末点包围盒外扩
	if (pos.Distance(m_Begin)/l_dProportion<=PRECISION||pos.Distance(m_End)/l_dProportion<=PRECISION)
		return TRUE;
	// 将世界坐标转化为屏幕坐标
	g_pView->WorldToScreem(m_Begin,pt_begin); 
	g_pView->WorldToScreem(m_End,pt_end) ;
	//把线段的斜率也要算进去，下面是算斜率的。
	double angle = GetAngleToXAxis(m_Begin,m_End);
	double d_Sin = PRECISION * sin(angle);
	double d_Cos = PRECISION * cos(angle);

	CRgn	pr;
	POINT	pt[4];//这个区域由四个点组成，
	pt[0].x = pt_begin.x - (int)d_Sin;
	pt[0].y = pt_begin.y - (int)d_Cos;//起始点上的一点

	pt[1].x = pt_begin.x + (int)d_Sin;
	pt[1].y = pt_begin.y + (int)d_Cos;//起始点下的一点
	
	pt[2].x = pt_end.x + (int)d_Sin;
	pt[2].y = pt_end.y + (int)d_Cos;//终点下的一点
	
	pt[3].x = pt_end.x - (int)d_Sin;
	pt[3].y = pt_end.y - (int)d_Cos;//终点上的一点
	
	pr.CreatePolygonRgn(pt,4,ALTERNATE);
	if(pr.PtInRegion(point)) 
	{
		pr.DeleteObject();
		return TRUE;
	}
	else

	{
		pr.DeleteObject();
		return -1;
	}
}

//移动
void CCreateLine::Move(const Position& basePos,const Position& desPos)
{
	m_Begin = m_Begin.Offset(desPos - basePos);
	m_End = m_End.Offset(desPos - basePos);
}
//旋转
void CCreateLine::Rotate(const Position& basePos, const double angle)
{
	m_Begin = m_Begin.Rotate(basePos, angle) ;
	m_End =m_End.Rotate(basePos, angle) ;
}
//镜像
void CCreateLine::Mirror(const Position& pos1, const Position& pos2)
{
	m_Begin = m_Begin.Mirror(pos1, pos2) ;
	m_End =m_End.Mirror(pos1, pos2) ;
}

/*
 *	属性对话框
 */
void CCreateLine::PropertyView()
{
	//显示属性对话框
	(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 

	//把隐藏的STATIC和Edit显示出来
	ShowControl(4,FALSE);
	//设定要显示的颜色
	
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(RtnColorInt()-1);
	//设置STATIC要显示“起点和终点”
	//设置Edit要显示起点和终点的位置
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"起点X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"起点Y");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Start X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Start Y");
	}
	CString str;
	str.Format("%.4f",m_Begin.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
	str.Format("%.4f",m_Begin.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
	if (g_pDoc->m_stcWorkFor.IsChinese)
	{
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"终点X");
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"终点Y");
	}
	else
	{
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"End X");
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"End Y");
	}
	str.Format("%.4f",m_End.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	str.Format("%.4f",m_End.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT4,str);
	////////////////////////遍历链表里的看自己排在第几位，刀顺时用///////////////////////////////////
	POSITION pos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	for (int i=0;pos != NULL;i++)//遍历链表，i可表示链表里的排序号
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(pos);
		//如果是M代码
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(this == (CCreateLine *)l_pDelList)//如果与链表里的元素一样，就要记下位置
		{
			str.Format("%d",i);
			((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,str);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	
}

CCreateBase* CCreateLine::Copy()
{
	CCreateLine *pCopy = new CCreateLine(m_Begin,m_End);	//复制点
	pCopy->m_ColorOfLine = m_ColorOfLine;		//复制线色
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}


/*
*	当在特性对话框弹出改变图元特性时要重画的的函数
*/
void CCreateLine::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	switch(WhichEdit) //哪个EDIT框改了，就变哪个值
	{
	case 1:
		m_Begin.x = e;
		break;
	case 2:
		m_Begin.y = e;
		break;
	case 3:
		m_End.x = e;
		break;
	case 4:
		m_End.y = e;
		break;
	default:
		break;
	}
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);	
}

void CCreateLine::DrawMove(CDC* pDC,int DrawMode,BOOL Poly)
{
	CPoint pt_begin, pt_end; // 屏幕坐标的起点和终点
	// 将世界坐标转化为屏幕坐标
	g_pView->WorldToScreem(m_Begin,pt_begin); 
	g_pView->WorldToScreem(m_End,pt_end) ;
	// 得到原来的绘图模式
	int		n = GetROP2(pDC->GetSafeHdc()); 
	// 创建画笔的原则：
	// 如果在正常的绘图模式下，使用成员变量创建画笔
	// 如果是其它的模式,使用全局函数"SetDrawEnvir"创建画笔
	CPen	pen; 
	if (DrawMode == dmNormal)		
		pen.CreatePen(PS_SOLID,1,m_ColorOfLine);//如果是正常画图模式，线色就要取决于所选色
	else
		SetDrawEnvir(pDC, DrawMode, &pen);	//调用SetDrawEnvir函数设置绘图环境
	CPen* pOldPen = pDC->SelectObject(&pen); // 得到原来的画笔
	// 根据屏幕坐标绘制图元
	pDC->MoveTo(pt_begin); 
	pDC->LineTo(pt_end);
	// 恢复原来的画笔 
	pDC->SelectObject(pOldPen); 
	// 恢复原来的绘图模式
	pDC->SetROP2(n);
	pen.DeleteObject();//释放pen的Create的;
}

/*
 *	框选时是否选中
 */
BOOL CCreateLine::IsSelect(CRgn *selectrect,BOOL bComplex)
{
	//如果框选时鼠标是从右向左选的话
	if (bComplex == TRUE)
	{
		//把框选区域用矩形表示
		CRect l_rectRgn;
		selectrect->GetRgnBox(l_rectRgn);
		//把框选的屏幕坐标转成世界坐标
		CPoint l_potRgn1(l_rectRgn.left,l_rectRgn.top),l_potRgn2(l_rectRgn.right,l_rectRgn.bottom);
		Position l_posRgn1,l_posRgn2;
		g_pView->ScreemToWorld(l_potRgn1,l_posRgn1);
		g_pView->ScreemToWorld(l_potRgn2,l_posRgn2);
		//看直线是否与矩形相交
		if (LineOnRect(l_posRgn1,l_posRgn2))
			return TRUE;
	}
	BOX2D pBox;		//包围盒结构
	CPoint pos1,pos2;	//包围盒的两个屏幕顶点（左上，右下）
	//得到包围盒的两个点，世界坐标（左下，右上）	//把包围盒外括
	GetBox(&pBox,TRUE);
	Position l_cPosLT(pBox.min[0],pBox.max[1]),l_cPosRB(pBox.max[0],pBox.min[1]);	
	//转成屏幕坐标
	g_pView->WorldToScreem(l_cPosLT,pos1);
	g_pView->WorldToScreem(l_cPosRB,pos2);
	//直线的包围盒的方形区域
	CRgn RgnLine;
	RgnLine.CreateRectRgn(pos1.x,pos1.y,pos2.x,pos2.y);
	//交集部分的区域
	CRgn RgnRect;
	RgnRect.CreateRectRgn(0,0,50,50);
	//取两个区域的交集
	RgnRect.CombineRgn(selectrect,&RgnLine,RGN_AND);
	//看两个交集是否相等
	BOOL l_bIsSelect= RgnLine.EqualRgn(&RgnRect);
	return l_bIsSelect;
}

/*
 *	得到包围盒
 */
BOOL CCreateLine::GetBox(BOX2D* pBox,BOOL IsOutside /* = FALSE */)
{
	double curRadius = g_pView->ReturnProportion()*PRECISION;
	//如果没有外括
	if (IsOutside == FALSE)
		curRadius = 0.0;
	pBox->min[0] = min( m_Begin.x, m_End.x ) - curRadius;
	pBox->min[1] = min( m_Begin.y, m_End.y ) - curRadius;
	pBox->max[0] = max( m_Begin.x, m_End.x ) + curRadius;
	pBox->max[1] = max( m_Begin.y, m_End.y ) + curRadius;
	return TRUE;
}

//XY缩放功能
BOOL CCreateLine::GraphZoomXY(double dValueX, double dValueY)
{
	double dXRadio = 1;
	double dYRadio = 1;
	BOX2D pBox;		//包围盒结构
	//得到包围盒的两个点，世界坐标（左下，右上）	//把包围盒外括
	GetBox(&pBox);
	if (fabs(dValueX)>DISTANCE_ZERO)
	{
		dXRadio = pBox.max[0]-pBox.min[0];
	}
	if (fabs(dValueY)>DISTANCE_ZERO)
	{
		dYRadio = pBox.max[1]-pBox.min[1];
	}
	double d1=0;//当前点相对于对小点的距离和最大距离的比值
	double d2=0;//增加的距离
	d1 = (m_Begin.x - pBox.min[0])/dXRadio;
	if (d1>1)//当添的值为零时
		d1 = 1;
	d2 = d1 * dValueX;
	m_Begin.x += d2;

	d1 = (m_Begin.y - pBox.min[1])/dYRadio;
	if (d1>1)
		d1 = 1;
	d2 = d1 * dValueY;
	m_Begin.y += d2;

	d1 = (m_End.x - pBox.min[0])/dXRadio;
	if (d1>1)
		d1 = 1;
	d2 = d1 * dValueX;
	m_End.x += d2;
	
	d1 = (m_End.y - pBox.min[1])/dYRadio;
	if (d1>1)
		d1 = 1;
	d2 = d1 * dValueY;
	m_End.y += d2;
	return TRUE;
}

/*
 *	对于图形上的一点输入长度返回一个点
 */
BOOL CCreateLine::RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine /* = 0 */)
{
	//如果输入的长度大于直线的长度就不满足
	double l_dAngle;
	if (dLongPos > 0 && dLongPos < ClickPos.Distance(m_End))
	{
		l_dAngle = GetAngleToXAxis(m_Begin,m_End);
		posTarget.x = ClickPos.x + dLongPos*cos(l_dAngle);
		posTarget.y = ClickPos.y + dLongPos*sin(l_dAngle);
		return TRUE;
	}
	else if (dLongPos < 0 &&  fabs(dLongPos) < ClickPos.Distance(m_Begin))
	{
		dLongPos = fabs(dLongPos);
		l_dAngle = GetAngleToXAxis(m_Begin,m_End);
		posTarget.x = ClickPos.x - dLongPos*cos(l_dAngle);
		posTarget.y = ClickPos.y - dLongPos*sin(l_dAngle);
		return TRUE;
	}
	return FALSE;
}

/*
 *	改变切割方向
 */
void CCreateLine::ChangeDir()
{
	//把起点和终点换个位置
	Position pos = m_Begin;
	m_Begin = m_End;
	m_End = pos;	
}

/*
 *	看直线是否与矩形有交点并且交点是否在直线上
 */
BOOL CCreateLine::LineOnRect(Position pos1,Position pos3)
{
	Position pos2(pos1.x,pos3.y),pos4(pos3.x,pos1.y),l_posInter;
	if (GetIntersectPoint(m_Begin,m_End,pos1,pos2,l_posInter)&&PosOnLine(pos1,pos2,l_posInter)&&PosOnLine(m_Begin,m_End,l_posInter))
		return TRUE;
	else if (GetIntersectPoint(m_Begin,m_End,pos2,pos3,l_posInter)&&PosOnLine(pos2,pos3,l_posInter)&&PosOnLine(m_Begin,m_End,l_posInter))
		return TRUE;
	else if (GetIntersectPoint(m_Begin,m_End,pos3,pos4,l_posInter)&&PosOnLine(pos3,pos4,l_posInter)&&PosOnLine(m_Begin,m_End,l_posInter))
		return TRUE;
	else if (GetIntersectPoint(m_Begin,m_End,pos4,pos1,l_posInter)&&PosOnLine(pos4,pos1,l_posInter)&&PosOnLine(m_Begin,m_End,l_posInter))
		return TRUE;
	return FALSE;
}

/*
 *	按比例缩放
 */
void CCreateLine::GraphZoomScale(Position posBase,double dScale)
{
	//基点与起末点的角度
	double l_dAngleBG = GetAngleToXAxis(posBase,m_Begin);
	double l_dAngleEN = GetAngleToXAxis(posBase,m_End);
	//基点到起末点的长度
	double l_dLongBG = posBase.Distance(m_Begin);
	m_Begin.x = posBase.x + cos(l_dAngleBG)*l_dLongBG*dScale; 
	m_Begin.y = posBase.y + sin(l_dAngleBG)*l_dLongBG*dScale; 
	//基点到起末点的长度
	double l_dLongEN = posBase.Distance(m_End);
	m_End.x = posBase.x + cos(l_dAngleEN)*l_dLongEN*dScale; 
	m_End.y = posBase.y + sin(l_dAngleEN)*l_dLongEN*dScale; 
}

/*
 *	实线变虚线
 */
PGRAPHPOS CCreateLine::SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist)
{
	double l_dRemainingEmty =0.0;
	PGRAPHPOS headGraphPos =  TurnLineDash(dLongSolid,dLongEmpty,m_Begin,m_End);
	return headGraphPos;
}








