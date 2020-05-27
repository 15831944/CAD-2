// CreateRound.cpp: implementation of the CCreateRound class.
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
IMPLEMENT_SERIAL(CCreateRound, CCreateBase, 0)
CCreateRound::CCreateRound()
{

}

CCreateRound::CCreateRound(const Position& center,const Position& endpos,int Direct)
{
	m_iDirect = Direct;
	m_ColorOfLine = g_CurColor;		//创建时的画色就是当前所选色
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_Center = center;
	m_EndPos = endpos;
	m_dRadius = m_Center.Distance(m_EndPos);
	m_iKnifeStyle = 41;//刀补类型 41左刀补 42右刀补
	m_iIsBeginGraph = 2;//0不是原始图形，1是原始图形，2是保存时不关心
	
}

CCreateRound::~CCreateRound()
{

}

/*
void CCreateRound::Draw(CDC *pDC)
{
	double l_dRadius = m_Center.Distance(m_EndPos);
	// 得到圆的外接矩形框
	Position ltpos(m_Center.x - l_dRadius, m_Center.y + l_dRadius ) ;
	Position rbpos(m_Center.x + l_dRadius, m_Center.y - l_dRadius ) ;
	// 屏幕坐标的外接矩形框的两个角点和圆心点
	CPoint slt, srb, scenp;
	// 将世界坐标转化为屏幕坐标
	g_pView->WorldToScreem(ltpos,slt) ;
	g_pView->WorldToScreem(rbpos,srb) ;
	pDC->SelectStockObject(NULL_BRUSH) ;
	Graphics myGraphics(pDC->m_hDC);
	Pen myPen(Color(0, 0 ,0),1); 
	myGraphics.DrawEllipse(&myPen,slt.x, slt.y, srb.x-slt.x, srb.y-slt.y);
}*/


void CCreateRound::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{
	Position ltpos(m_Center.x - m_dRadius, m_Center.y + m_dRadius ) ;
	Position rbpos(m_Center.x + m_dRadius, m_Center.y - m_dRadius ) ;
	// 屏幕坐标的外接矩形框的两个角点和圆心点
	CPoint slt, srb,pt_begin;//起点坐标（屏幕坐标系）
	// 将世界坐标转化为屏幕坐标
	g_pView->WorldToScreem(m_EndPos,pt_begin) ;
	g_pView->WorldToScreem(ltpos,slt) ;
	g_pView->WorldToScreem(rbpos,srb) ;
	
	int		n = GetROP2(pDC->GetSafeHdc()); 

	CPen	pen; 
	if (DrawMode == dmNormal)		
		pen.CreatePen(PS_SOLID,1,m_ColorOfLine);//如果是正常画图模式，线色就要取决于所选色
	else
		SetDrawEnvir(pDC, DrawMode, &pen);	//调用SetDrawEnvir函数设置绘图环境
	CPen* pOldPen = pDC->SelectObject(&pen); // 得到原来的画笔
	// 根据屏幕坐标绘制图元	
	pDC->SelectStockObject(NULL_BRUSH) ;	//把圆中间透明
	pDC->Ellipse(slt.x, slt.y, srb.x, srb.y) ;
	if (FlagShowArrow)	//画方框和箭头
	{
		//画起始点的方框
		CRect rect(pt_begin.x-5,pt_begin.y-5,pt_begin.x+5,pt_begin.y+5);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect); 
		double angle = atan((m_Center.x-m_EndPos.x) /(m_EndPos.y -m_Center.y ));
		if (m_iDirect == AD_COUNTERCLOCKWISE) //如果是逆时针时
			angle = angle+MATH_PI;
		if (m_Center.y > m_EndPos.y)	//如果是顺时针，的三四象线里。
			angle = angle + MATH_PI;
		if (m_iDirect == AD_CLOCKWISE)
			DrawArrow(angle,pt_begin,pDC);//画箭头的函数 
		else
			DrawArrow(angle,pt_begin,pDC);//画箭头的函数
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
			if(this == (CCreateRound *)l_pDelList)//如果与链表里的元素一样，就要记下位置
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //这样就可以透明化背景了				
				pDC->TextOut(pt_begin.x+1,pt_begin.y+1,str);//输出到界面 上
				pDC->SetBkMode(l_iOldMode);				
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

int CCreateRound::IsPick(CPoint point)
{
	double  d_lRadius = m_Center.Distance(m_EndPos);//圆的半径
	Position pos(0,0);
	g_pView->ScreemToWorld(point,pos);
	double d_lDistance = m_Center.Distance(pos);
	double l_dProportion = g_pView->ReturnProportion();
	if ((fabs(d_lRadius-d_lDistance)/l_dProportion)<=PRECISION)
		return TRUE;
	return -1;
}

CCreateBase* CCreateRound::Copy()
{
	CCreateRound *pCopy = new CCreateRound(m_Center,m_EndPos,m_iDirect);//复制点
	pCopy->m_ColorOfLine = m_ColorOfLine;		//复制线色
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}

//移动
void CCreateRound::Move(const Position& basePos,const Position& desPos)
{
	Position pos(basePos.x,basePos.y);
	m_Center = m_Center.Offset(desPos - pos);
	m_EndPos = m_EndPos.Offset(desPos - pos);
}
//旋转
void CCreateRound::Rotate(const Position& basePos, const double angle)
{
	m_Center = m_Center.Rotate(basePos,angle);
	m_EndPos = m_EndPos.Rotate(basePos,angle);
}
//镜像
void CCreateRound::Mirror(const Position& pos1, const Position& pos2)
{
	m_Center = m_Center.Mirror(pos1,pos2);
	m_EndPos = m_EndPos.Mirror(pos1,pos2);
}

/*
*	属性对话框
*/
void CCreateRound::PropertyView()
{
	//显示属性对话框
	(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 
	
	//把隐藏的STATIC和Edit显示出来
	ShowControl(3,FALSE);
	//设定要显示的颜色
	
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(RtnColorInt()-1);
	//设置STATIC要显示“起点和终点”
	//设置Edit要显示起点和终点的位置
	CString str;
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"圆心X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"圆心Y");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"半径");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Center X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Center Y");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"Radius");
	}
	
	str.Format("%.4f",m_Center.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
	str.Format("%.4f",m_Center.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
	str.Format("%.4f",m_dRadius);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	////////////////////////遍历链表里的看自己排在第几位，刀顺时用///////////////////////////////////
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	for (int i=0;l_ListPos != NULL;i++)//遍历链表，i可表示链表里的排序号
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//如果是M代码
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(this == (CCreateRound *)l_pDelList)//如果与链表里的元素一样，就要记下位置
		{
			str.Format("%d",i);
			((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,str);
		}
	}
	//////////////////////////////////////////////////////////////////////////	
}

/*
*	当在特性对话框弹出改变图元特性时要重画的的函数
*/
void CCreateRound::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	Position l_posDes;
	switch(WhichEdit) //哪个EDIT框改了，就变哪个值
	{
	case 1:
		l_posDes.x = e;
		l_posDes.y = m_Center.y;
		Move(m_Center,l_posDes);
		break;
	case 2:
		l_posDes.x = m_Center.x;
		l_posDes.y = e;
		Move(m_Center,l_posDes);
		break;
	case 3:
		m_dRadius = e;
		m_EndPos.x = m_Center.x - m_dRadius;
		m_EndPos.y = m_Center.y;
		break;
	default:
		break;
	}
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);	
}

/*
 *	框选时是否选中
 */
BOOL CCreateRound::IsSelect(CRgn *selectrect,BOOL bComplex)
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
		if (RoundOnRect(l_posRgn1,l_posRgn2))
			return TRUE;
	}
	BOX2D pBox;		//包围盒结构
	CPoint pos1,pos2;	//包围盒的两个屏幕顶点（左上，右下）
	//得到包围盒的两个点，世界坐标（左下，右上）
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
BOOL CCreateRound::GetBox(BOX2D* pBox,BOOL IsOutside /* = FALSE */)
{
	double curRadius =  g_pView->ReturnProportion();
	//如果没有外括
	if (IsOutside == FALSE)
		curRadius = 0.0;
	pBox->min[0] = m_Center.x-m_dRadius - curRadius;
	pBox->min[1] = m_Center.y-m_dRadius - curRadius;
	pBox->max[0] = m_Center.x+m_dRadius + curRadius;
	pBox->max[1] = m_Center.y+m_dRadius + curRadius;
	return TRUE;
}

//XY缩放功能
BOOL CCreateRound::GraphZoomXY(double dValueX, double dValueY)
{
	return FALSE;
	
}

/*
 *	对于图形上的一点输入长度返回一个点
 */
BOOL CCreateRound::RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine /* = 0 */)
{
	//圆心到鼠标点击的角度
	double l_dAngle = GetAngleToXAxis(m_Center,ClickPos);
	double l_dLongArc;
	//圆弧的周长
	double l_dPerimeter = 2*MATH_PI*m_dRadius;
	//如果输入大于周长退出
	if (l_dPerimeter < dLongPos)
		return FALSE;
	else
	{//求输入的长度对应的角度
		l_dLongArc = ((dLongPos/l_dPerimeter)*MATH_2PI);
	}
	//如果输入的长度大于圆弧的长度就不满足
	if (dLongPos > 0)//判断离末点的距离
	{
		if (m_iDirect == AD_CLOCKWISE)//顺时针 
		{//末点和点击点在零度线的两边
			posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle-l_dLongArc);
			posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle-l_dLongArc);	
			return TRUE;
		}
		else//逆时针
		{
			posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle+l_dLongArc);
			posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle+l_dLongArc);				
			return TRUE;
		}
	}
	else//判断离起点的距离
	{
		dLongPos = fabs(dLongPos);			
		if (m_iDirect == AD_CLOCKWISE)//顺时针 
		{//末点和点击点在零度线的两边
			posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle+l_dLongArc);
			posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle+l_dLongArc);				
			return TRUE;
		}
		else//逆时针
		{
			posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle-l_dLongArc);
			posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle-l_dLongArc);				
			return TRUE;
		}
	}
	return FALSE;
}


/*
 *	改变切割方向
 */
void CCreateRound::ChangeDir()
{
	//改变圆的切割方向
	if(m_iDirect == AD_CLOCKWISE)
		m_iDirect = AD_COUNTERCLOCKWISE;
	else
		m_iDirect = AD_CLOCKWISE;
	
}

/*
 *	圆弧是否在矩形上
 */
BOOL CCreateRound::RoundOnRect(Position pos1,Position pos3)
{
	Position posTemp1 = pos1,posTemp2,pos2(pos1.x,pos3.y),pos4(pos3.x,pos1.y),l_posInter;
	posTemp2 = pos2;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//是否在圆上
	{
		if(PosOnLine(pos1,pos2,posTemp1)||PosOnLine(pos1,pos2,posTemp2))//是否在弧上
			return TRUE;
	}
	posTemp1 = pos2;
	posTemp2 = pos3;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))    //是否在圆上
	{
		if(PosOnLine(pos3,pos2,posTemp1)||PosOnLine(pos3,pos2,posTemp2))//是否在弧上
			return TRUE;
	}
	posTemp1 = pos3;
	posTemp2 = pos4;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//是否在圆上
	{
		if(PosOnLine(pos3,pos4,posTemp1)||PosOnLine(pos3,pos4,posTemp2))//是否在弧上
			return TRUE;
	}
	posTemp1 = pos4;
	posTemp2 = pos1;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//是否在圆上
	{
		if(PosOnLine(pos1,pos4,posTemp1)||PosOnLine(pos1,pos4,posTemp2))//是否在弧上
			return TRUE;
	}
	return FALSE;
}

/*
*	按比例缩放
*/
void CCreateRound::GraphZoomScale(Position posBase,double dScale)
{
	//基点与起末点的角度
	double l_dAngleCT = GetAngleToXAxis(posBase,m_Center);
	double l_dAngleEN = GetAngleToXAxis(posBase,m_EndPos);
	//基点到起末点的长度
	double l_dLongCT = posBase.Distance(m_Center);
	m_Center.x = posBase.x + cos(l_dAngleCT)*l_dLongCT*dScale; 
	m_Center.y = posBase.y + sin(l_dAngleCT)*l_dLongCT*dScale; 
	//基点到起末点的长度
	double l_dLongEN = posBase.Distance(m_EndPos);
	m_EndPos.x = posBase.x + cos(l_dAngleEN)*l_dLongEN*dScale; 
	m_EndPos.y = posBase.y + sin(l_dAngleEN)*l_dLongEN*dScale; 
	//半径变化
	m_dRadius *= dScale;
}

/*
 *	实线变虚线
 */
PGRAPHPOS CCreateRound::SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist)
{
	double l_dAngle = GetAngleToXAxis(m_Center,m_EndPos);
	PGRAPHPOS headGraphPos =  TurnArcDash(dLongSolid,dLongEmpty,m_Center,l_dAngle,l_dAngle,m_dRadius,m_iDirect);	
	return headGraphPos;
}





