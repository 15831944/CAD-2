// CreateRoundArc.cpp: implementation of the CCreateRoundArc class.
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
IMPLEMENT_SERIAL(CCreateRoundArc, CCreateBase, 0)
CCreateRoundArc::CCreateRoundArc()
{

}

CCreateRoundArc::CCreateRoundArc(const Position Begin,const Position Center,const Position End,int Direct)
{
	m_ColorOfLine = g_CurColor;		//创建时的画色就是当前所选色
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_Begin = Begin;	//起点
	m_Center = Center;	//中点
	m_iDirect = Direct;	//方向
	m_dRadius = m_Center.Distance(m_Begin) ;//半径
	
	double  angle2 = GetAngleToXAxis(m_Center, End);//中心与末点的角度
	
	m_End.x = m_dRadius * cos(angle2) + m_Center.x ;
	m_End.y = m_dRadius * sin(angle2) + m_Center.y ;
	m_dAngleEnd = GetAngle(m_Center, m_End);
	m_dAngleStart = GetAngle(m_Center, m_Begin);
	m_iKnifeStyle = 41;//刀补类型 41左刀补 42右刀补
	m_iIsBeginGraph = 2;//0不是原始图形，1是原始图形，2是保存时不关心
	
}

CCreateRoundArc::~CCreateRoundArc()
{

}

/*
void CCreateRoundArc::Draw(CDC *pDC)
{
	double l_dRadius = m_Center.Distance(m_End);
	// 得到圆弧的外接矩形框
	Position	offset(-l_dRadius,l_dRadius);
	Position	ltpos = m_Center + offset;
	Position	rbpos = m_Center - offset;
	// 屏幕坐标的圆心点、起始点和终止点
	CPoint sltp, srbp, ssp, sep, center ;
	// 将世界坐标转化为屏幕坐标
	g_pView->WorldToScreem(m_End, sep ) ;
//	g_pView->WorldToScreem(m_Begin, ssp) ;
	g_pView->WorldToScreem(ltpos,sltp) ;
	g_pView->WorldToScreem(rbpos,srbp) ;

	g_pView->WorldToScreem(m_Center,center ) ;


	Graphics myGraphics(pDC->m_hDC);
	Pen myPen(Color(0, 0 ,0),1); 

	double  angle1 = GetAngleToXAxis(m_Center, m_Begin);
	double  angle2 = GetAngleToXAxis(m_Center, m_End);
	double l_StartAngle = 360-angle1/PI*180;
	double l_SweepAngle1 = 360-angle2/PI*180;
	double l_SweepAngle;
	if (l_StartAngle<l_SweepAngle1)
		l_SweepAngle = l_SweepAngle1 - l_StartAngle;
	if (l_StartAngle>l_SweepAngle1)
		l_SweepAngle = 360 - l_StartAngle + l_SweepAngle1;
		
	myGraphics.DrawArc(&myPen,sltp.x,sltp.y,srbp.x-sltp.x,srbp.y-sltp.y ,l_StartAngle,l_SweepAngle);	
}*/


void CCreateRoundArc::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{		
	// 屏幕坐标的圆心点、起始点和终止点
	CPoint l_ptCenter, l_ptBegin, l_ptEnd ,l_ptDrawBegin;
	// 将世界坐标转化为屏幕坐标
	g_pView->WorldToScreem(m_End, l_ptEnd ) ;
	g_pView->WorldToScreem(m_Begin, l_ptBegin) ;
	g_pView->WorldToScreem(m_Center,l_ptCenter) ;
	//		 偏差角
	double l_dAngleOffset,l_dAngleStart = m_dAngleStart;
	l_ptDrawBegin = l_ptBegin;
	if(!GetAngleStartAndOffset(l_dAngleStart,l_dAngleOffset, l_ptDrawBegin))
		return;
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
	//画圆弧
	pDC->MoveTo(l_ptDrawBegin);
	DrawArc(pDC,l_ptCenter,m_dRadius/g_pView->m_dProportion,l_dAngleStart,l_dAngleOffset);
	//画方框和箭头
	if (FlagShowArrow)	
	{
		//画起始点的方框
		CRect rect(l_ptBegin.x-5,l_ptBegin.y-5,l_ptBegin.x+5,l_ptBegin.y+5);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
		double angle = atan((m_Center.x-m_End.x) /(m_End.y -m_Center.y ));
		if (m_iDirect == AD_COUNTERCLOCKWISE) //如果是逆时针时
			angle = angle+MATH_PI;
		if (m_Center.y > m_End.y)	//如果是顺时针，的三四象线里。
			angle = angle + MATH_PI;
		DrawArrow(angle,l_ptEnd,pDC);//画箭头的函数 
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
			if(this == (CCreateRoundArc *)l_pDelList)//如果与链表里的元素一样，就要记下位置
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //这样就可以透明化背景了				
				pDC->TextOut(l_ptBegin.x,l_ptBegin.y,str);//输出到界面 上
				pDC->SetBkMode(l_iOldMode);				
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////	
	// 恢复原来的画笔 
	pDC->SelectObject(pOldPen) ;
	// 恢复原来的绘图模式
	pDC->SetROP2(n);
	pen.DeleteObject();
}

int CCreateRoundArc::IsPick(CPoint point)
{
	Position pos(0,0);
	//获得缩放比例
	double l_dProportion = g_pView->ReturnProportion();	
	g_pView->ScreemToWorld(point,pos);
	//如果距离起点或末点的距离满足就算拾取到，相当起末点包围盒外扩
	if (pos.Distance(m_Begin)/l_dProportion<=PRECISION||pos.Distance(m_End)/l_dProportion<=PRECISION)
		return TRUE;
	//计算圆弧半径
	double radius = m_Center.Distance(m_Begin) ;
	//计算拾取点和圆弧中心连线与x轴的夹角
	double distance =  m_Center.Distance(pos); 
	//拾取点到圆心的距离
	if(!(  (fabs( radius - distance)/l_dProportion)<=PRECISION)  )
		return  -1;

	//计算圆弧的起始角
	double angle = GetAngle(m_Center, pos);
	double angle1 = GetAngle(m_Center, m_Begin) ;
	//计算圆弧的终止角
	double angle2 = GetAngle(m_Center, m_End) ;

	//拾取点和圆弧中心连线与x轴的夹角应该存在于
	//起始角和终止角范围之外，并且拾取点到圆心的距离
	//和圆弧半径之差的绝对值小于给定值时，返回TRUE
	if (m_iDirect == AD_CLOCKWISE)//如果是顺时针
	{
		if (angle1<angle2)//过零度线时，就是X轴正方向
		{
			if (angle<angle1||angle>angle2)
				return TRUE;
			else
				return -1;
		}
		if (angle<angle1&&angle>angle2)
			return TRUE;
		return -1;
	}
	else//逆时针
	{
		if (angle1>angle2)//过零度线时，就是X轴正方向
		{
			if (angle<angle2||angle>angle1)
				return TRUE;
			else
				return -1;
		}
		if (angle<angle2&&angle>angle1)
			return TRUE;
		return -1;		
	}
}

CCreateBase* CCreateRoundArc::Copy()
{
	
	CCreateRoundArc *pCopy = new CCreateRoundArc(m_Begin,m_Center,m_End,m_iDirect);//复制点
	pCopy->m_ColorOfLine = m_ColorOfLine;		//复制线色
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}


//移动
void CCreateRoundArc::Move(const Position& basePos,const Position& desPos)
{
	m_Begin = m_Begin.Offset(desPos - basePos);
	m_End = m_End.Offset(desPos - basePos);
	m_Center = m_Center.Offset(desPos - basePos);
	
}
//旋转
void CCreateRoundArc::Rotate(const Position& basePos, const double angle)
{
	m_Begin = m_Begin.Rotate(basePos, angle) ;
	m_End =m_End.Rotate(basePos, angle) ;
	m_Center =m_Center.Rotate(basePos, angle) ;
	m_dAngleStart = GetAngle(m_Center, m_Begin);
	m_dAngleEnd = GetAngle(m_Center, m_End);
}
//镜像
void CCreateRoundArc::Mirror(const Position& pos1, const Position& pos2)
{
	m_Begin = m_Begin.Mirror(pos1, pos2) ;
	m_End =m_End.Mirror(pos1, pos2) ;
	m_Center =m_Center.Mirror(pos1, pos2) ;
	m_dAngleStart = GetAngle(m_Center, m_Begin);
	m_dAngleEnd = GetAngle(m_Center, m_End);
	if(m_iDirect == AD_CLOCKWISE)
		m_iDirect = AD_COUNTERCLOCKWISE;
	else
		m_iDirect = AD_CLOCKWISE;
}

/*
*	属性对话框
*/
void CCreateRoundArc::PropertyView()
{
	//显示属性对话框
	(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 
	
	//把隐藏的STATIC和Edit显示出来
	ShowControl(5,FALSE);
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
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"起始角度");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC5,"端点角度");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Center X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Center Y");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"Radius");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"Angle start");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC5,"Angle End");
	}
	
	str.Format("%.4f",m_Center.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
	str.Format("%.4f",m_Center.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
	str.Format("%.4f",m_dRadius);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	str.Format("%.4f",m_dAngleStart);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT4,str);
	str.Format("%.4f",m_dAngleEnd);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT5,str);
	////////////////////////遍历链表里的看自己排在第几位，刀顺时用///////////////////////////////////
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	for (int i=0;l_ListPos != NULL;i++)//遍历链表，i可表示链表里的排序号
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//如果是M代码
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(this == (CCreateRoundArc *)l_pDelList)//如果与链表里的元素一样，就要记下位置
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
void CCreateRoundArc::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
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
		m_Begin.x = m_dRadius*cos(m_dAngleStart/360*MATH_2PI)+m_Center.x;
		m_Begin.y = m_dRadius*sin(m_dAngleStart/360*MATH_2PI)+m_Center.y;
		m_End.x = m_dRadius*cos(m_dAngleEnd/360*MATH_2PI)+m_Center.x;
		m_End.y = m_dRadius*sin(m_dAngleEnd/360*MATH_2PI)+m_Center.y;
		break;
	case 4:
		m_dAngleStart = e;
		m_Begin.x = m_dRadius*cos(m_dAngleStart/360*MATH_2PI)+m_Center.x;
		m_Begin.y = m_dRadius*sin(m_dAngleStart/360*MATH_2PI)+m_Center.y;
		break;
	case 5:
		m_dAngleEnd = e;
		m_End.x = m_dRadius*cos(m_dAngleEnd/360*MATH_2PI)+m_Center.x;
		m_End.y = m_dRadius*sin(m_dAngleEnd/360*MATH_2PI)+m_Center.y;
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
BOOL CCreateRoundArc::IsSelect(CRgn *selectrect,BOOL bComplex)
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
		if (ArcOnRect(l_posRgn1,l_posRgn2))
			return TRUE;
	}
	
	BOX2D pBox;		//包围盒结构
	CPoint pos1,pos2;	//包围盒的两个屏幕顶点（左上，右下）
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
	BOOL IsSelec= RgnLine.EqualRgn(&RgnRect);
	return IsSelec;
}

/*
 *	得到包围盒
 */
BOOL CCreateRoundArc::GetBox(BOX2D* pBox,BOOL IsOutside /* = FALSE */)
{
	double MinX, MinY, MaxX, MaxY ,tempX, tempY;
	int pb = 0 ;
	
	double radius = m_Center.Distance(m_Begin) ;
	double StartRa = GetAngleToXAxis(m_Center, m_Begin) ;
	double EndRa = GetAngleToXAxis(m_Center, m_End) ;
	MinX = min( m_Begin.x, m_End.x );
	MinY = min( m_Begin.y, m_End.y );
	MaxX = max( m_Begin.x, m_End.x );
	MaxY = max( m_Begin.y, m_End.y );
	
	if (m_iDirect == AD_COUNTERCLOCKWISE)	//如果是逆时针
	{
		if(StartRa < EndRa) pb = 1 ;
		else
		{
			MinX = min(MinX, m_Center.x+radius) ;
			MinY = min(MinY, m_Center.y) ;
			MaxX = max(MaxX, m_Center.x+radius) ;
			MaxY = max(MaxY, m_Center.y) ;
			tempX = MaxX ;
			tempY = MaxY ;
			pb = 2 ;
		}
		for(int i=0 ;i<4 ;i++)
		{
			if(pb==1 && StartRa<MATH_PI*i/2. && EndRa>MATH_PI*i/2.||
				pb==2 &&!(StartRa>MATH_PI*i/2. && EndRa<MATH_PI*i/2.))			
			{
				if(i==1)
				{
					tempX = m_Center.x  ; tempY = m_Center.y+radius ;
				}
				if(i==2)
				{
					tempX = m_Center.x - radius ; tempY = m_Center.y ;
				}
				if(i==3)
				{
					tempX = m_Center.x ; tempY = m_Center.y - radius ;
				}
				MinX = min(MinX, tempX) ;
				MinY = min(MinY, tempY) ;
				MaxX = max(MaxX, tempX) ;
				MaxY = max(MaxY, tempY) ;
			}
		}
	}
	else//顺时针
	{
		if(StartRa > EndRa) pb = 1 ;
		else
		{
			MinX = min(MinX, m_Center.x+radius) ;
			MinY = min(MinY, m_Center.y) ;
			MaxX = max(MaxX, m_Center.x+radius) ;
			MaxY = max(MaxY, m_Center.y) ;
			tempX = MaxX ;
			tempY = MaxY ;
			pb = 2 ;
		}
		for(int i=0 ;i<4 ;i++)
		{
			if(pb==1 && StartRa>MATH_PI*i/2. && EndRa<MATH_PI*i/2.||
				pb==2 &&!(StartRa<MATH_PI*i/2. && EndRa>MATH_PI*i/2.))			
			{
				if(i==1)
				{
					tempX = m_Center.x  ; tempY = m_Center.y+radius ;
				}
				if(i==2)
				{
					tempX = m_Center.x - radius ; tempY = m_Center.y ;
				}
				if(i==3)
				{
					tempX = m_Center.x ; tempY = m_Center.y - radius ;
				}
				MinX = min(MinX, tempX) ;
				MinY = min(MinY, tempY) ;
				MaxX = max(MaxX, tempX) ;
				MaxY = max(MaxY, tempY) ;
			}
		}
	}
	//	//得到包围盒的两个点，世界坐标（左下，右上）
	//把包围盒外括
	double curRadius = g_pView->ReturnProportion()*PRECISION;
	//如果没有外括
	if (IsOutside == FALSE)
		curRadius = 0.0;
	pBox->min[0] = MinX - curRadius;
	pBox->min[1] = MinY - curRadius;
	pBox->max[0] = MaxX + curRadius;
	pBox->max[1] = MaxY + curRadius;
	return TRUE;
}

//XY缩放功能
BOOL CCreateRoundArc::GraphZoomXY(double dValueX, double dValueY)
{
	return FALSE;
	
}

/*
 *	对于图形上的一点输入长度返回一个点
 */
BOOL CCreateRoundArc::RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine /* = 0 */)
{
	//圆心到鼠标点击的角度
	double l_dAngle = GetAngleToXAxis(m_Center,ClickPos);
	double l_dLongArc,l_dAngle2,l_dAngleStart = m_dAngleStart/180*MATH_PI,l_dAngleEnd = m_dAngleEnd/180*MATH_PI;
	//圆弧的周长
	double l_dPerimeter = 2*MATH_PI*m_dRadius;
	//如果输入的长度大于圆弧的长度就不满足
	if (dLongPos > 0)//判断离末点的距离
	{
		if (m_iDirect == AD_CLOCKWISE)//顺时针 
		{	//末点和点击点在零度线的两边
			if (l_dAngleStart<l_dAngleEnd&&l_dAngle<l_dAngleEnd)//末点过零度线并且点击没过零度线
			{
				l_dLongArc = ((l_dAngle+MATH_2PI-l_dAngleEnd)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle+MATH_2PI-((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
			else//末点与点击点在零度线一边
			{
				l_dLongArc = ((l_dAngle-l_dAngleEnd)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle-((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
		}
		else//逆时针
		{	//末点和点击点在零度线的两边
			if (l_dAngleStart>l_dAngleEnd&&l_dAngle>l_dAngleEnd)//末点过零度线并且点击没过零度线
			{
				l_dLongArc = ((l_dAngleEnd+MATH_2PI-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle-MATH_2PI+((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
			else//末点与点击点在零度线一边
			{
				l_dLongArc = ((l_dAngleEnd-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = ((dLongPos/l_dPerimeter)*MATH_2PI)+l_dAngle;//长度对就的角度
					//求弧长对应有角度
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
		}
	}
	else if (dLongPos < 0 )
	{
		dLongPos = fabs(dLongPos);			
		if (m_iDirect == AD_CLOCKWISE)//顺时针 
		{	//起点和点击点在零度线的两边
			if (l_dAngleStart<l_dAngleEnd&&l_dAngle>l_dAngleStart)//起点过零度线并且点击没过零度线
			{
				l_dLongArc = ((l_dAngleStart+MATH_2PI-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle-MATH_2PI+((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
			else//起点与点击点在零度线一边
			{
				l_dLongArc = ((l_dAngleStart-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle+((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
		}
		else//逆时针
		{	//起点和点击点在零度线的两边
			if (l_dAngleStart>l_dAngleEnd&&l_dAngle<l_dAngleStart)//起点过零度线并且点击没过零度线
			{
				l_dLongArc = ((l_dAngle+MATH_2PI-l_dAngleStart)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle+MATH_2PI-((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
			else//起点与点击点在零度线一边
			{
				l_dLongArc = ((l_dAngle-l_dAngleStart)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle-((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
		}
	}
	return FALSE;
}


/*
 *	改变切割方向
 */
void CCreateRoundArc::ChangeDir()
{
	//把起点和终点坐标位置换一下！
	Position pos = m_Begin;
	m_Begin = m_End;
	m_End = pos;
	//改变圆的切割方向
	if(m_iDirect == AD_CLOCKWISE)
		m_iDirect = AD_COUNTERCLOCKWISE;
	else
		m_iDirect = AD_CLOCKWISE;
	m_dAngleStart = GetAngle(m_Center, m_Begin);
	m_dAngleEnd = GetAngle(m_Center, m_End);
	
}

/*
 *	圆弧是否在矩形上
 */
BOOL CCreateRoundArc::ArcOnRect(Position pos1,Position pos3)
{
	Position posTemp1,posTemp2,pos2(pos1.x,pos3.y),pos4(pos3.x,pos1.y),l_posInter;
	//以下是四条线是否在圆弧上
	posTemp1 = pos1;
	posTemp2 = pos2;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//是否在圆上
	{
		if((PosOnArc(posTemp1)&&PosOnLine(pos1,pos2,posTemp1))||(PosOnArc(posTemp2)&&PosOnLine(pos1,pos2,posTemp2)))//是否在弧上
			return TRUE;
	}
	posTemp1 = pos2;
	posTemp2 = pos3;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))    //是否在圆上
	{
		if((PosOnArc(posTemp1)&&PosOnLine(pos2,pos3,posTemp1))||(PosOnArc(posTemp2)&&PosOnLine(pos2,pos3,posTemp2)))//是否在弧上
			return TRUE;
	}
	posTemp1 = pos3;
	posTemp2 = pos4;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//是否在圆上
	{
		if((PosOnArc(posTemp1)&&PosOnLine(pos3,pos4,posTemp1))||(PosOnArc(posTemp2)&&PosOnLine(pos3,pos4,posTemp2)))//是否在弧上
			return TRUE;
	}
	posTemp1 = pos4;
	posTemp2 = pos1;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//是否在圆上
	{
		if((PosOnArc(posTemp1)&&PosOnLine(pos1,pos4,posTemp1))||(PosOnArc(posTemp2)&&PosOnLine(pos1,pos4,posTemp2)))//是否在弧上
			return TRUE;
	}
	return FALSE;
}

/*
 *	点是否在圆弧上
 */
BOOL CCreateRoundArc::PosOnArc(Position pos)
{	
	//计算圆弧的起始角
	double angle = GetAngle(m_Center, pos);
	double angle1 = GetAngle(m_Center, m_Begin) ;
	//计算圆弧的终止角
	double angle2 = GetAngle(m_Center, m_End) ;
	
	if (m_iDirect == AD_CLOCKWISE)//如果是顺时针
	{
		if (angle1<angle2)//过零度线时，就是X轴正方向
		{
			if (angle<angle1||angle>angle2)
				return TRUE;
			else
				return FALSE;
		}
		if (angle<angle1&&angle>angle2)
			return TRUE;
		return FALSE;
	}
	else//逆时针
	{
		if (angle1>angle2)//过零度线时，就是X轴正方向
		{
			if (angle<angle2||angle>angle1)
				return TRUE;
			else
				return FALSE;
		}
		if (angle<angle2&&angle>angle1)
			return TRUE;
		return FALSE;		
	}
	return FALSE;
}

/*
 *	返回在屏幕里显示的起始角和转过角度
 */
BOOL CCreateRoundArc::GetAngleStartAndOffset(double& dAngleStart,double& dAngleOffset,CPoint& potBegin)
{
//判断图形是否在全部在客户区
	//把屏幕区域用矩形表示
	CRect l_rectRgn;
	g_pView->GetClientRect(l_rectRgn);
	CRgn selectrect;
	CPoint l_potRgn1(0,0),l_potRgn2(l_rectRgn.right,l_rectRgn.bottom);
	selectrect.CreateRectRgn(l_potRgn1.x,l_potRgn1.y,l_potRgn2.x,l_potRgn2.y);
	//把客户区的屏幕坐标转成世界坐标
	Position l_posRgn1,l_posRgn3;
	g_pView->ScreemToWorld(l_potRgn1,l_posRgn1);
	g_pView->ScreemToWorld(l_potRgn2,l_posRgn3);
	//包围盒结构
	BOX2D pBox;		
	CPoint pos1,pos2;	//包围盒的两个屏幕顶点（左上，右下）
	GetBox(&pBox);
	Position l_cPosLT(pBox.min[0],pBox.max[1]),l_cPosRB(pBox.max[0],pBox.min[1]);	
	//转图形屏幕坐标
	g_pView->WorldToScreem(l_cPosLT,pos1);
	g_pView->WorldToScreem(l_cPosRB,pos2);
	//直线的包围盒的方形区域
	CRgn RgnLine;
	RgnLine.CreateRectRgn(pos1.x,pos1.y,pos2.x,pos2.y);
	//交集部分的区域
	CRgn RgnRect;
	RgnRect.CreateRectRgn(0,0,50,50);
	//取两个区域的交集
	RgnRect.CombineRgn(&selectrect,&RgnLine,RGN_AND);
	//看两个交集是否相等
	if(RgnLine.EqualRgn(&RgnRect))
		return CaleStartAndOffset(dAngleStart,dAngleOffset);
//部分在客户区的情况	
	//与边框的交点
	CArray<Position,Position&> l_aryArcPoint;//点集	
	Position posTemp1,posTemp2,l_posRgn2(l_posRgn1.x,l_posRgn3.y),l_posRgn4(l_posRgn3.x,l_posRgn1.y),l_posInter;
	//以下是四条线是否在圆弧上
	posTemp1 = l_posRgn1;
	posTemp2 = l_posRgn2;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//是否在圆上
	{
		//如果两个交点相等
		if (posTemp1 == posTemp2)
			l_aryArcPoint.Add(posTemp1);
		else
		{
			if(PosOnArc(posTemp1)&&PosOnLine(l_posRgn1,l_posRgn2,posTemp1))
				l_aryArcPoint.Add(posTemp1);
			if (PosOnArc(posTemp2)&&PosOnLine(l_posRgn1,l_posRgn2,posTemp2))
				l_aryArcPoint.Add(posTemp2);
		}
	}
	posTemp1 = l_posRgn2;
	posTemp2 = l_posRgn3;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))    //是否在圆上
	{
		//如果两个交点相等
		if (posTemp1 == posTemp2)
			l_aryArcPoint.Add(posTemp1);
		else
		{
			if(PosOnArc(posTemp1)&&PosOnLine(l_posRgn2,l_posRgn3,posTemp1))
				l_aryArcPoint.Add(posTemp1);
			if (PosOnArc(posTemp2)&&PosOnLine(l_posRgn2,l_posRgn3,posTemp2))
				l_aryArcPoint.Add(posTemp2);
		}
	}
	posTemp1 = l_posRgn3;
	posTemp2 = l_posRgn4;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//是否在圆上
	{
		//如果两个交点相等
		if (posTemp1 == posTemp2)
			l_aryArcPoint.Add(posTemp1);
		else
		{
			if(PosOnArc(posTemp1)&&PosOnLine(l_posRgn3,l_posRgn4,posTemp1))
				l_aryArcPoint.Add(posTemp1);
			if (PosOnArc(posTemp2)&&PosOnLine(l_posRgn3,l_posRgn4,posTemp2))
				l_aryArcPoint.Add(posTemp2);
		}
	}
	posTemp1 = l_posRgn4;
	posTemp2 = l_posRgn1;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//是否在圆上
	{
		//如果两个交点相等
		if (posTemp1 == posTemp2)
			l_aryArcPoint.Add(posTemp1);
		else
		{
			if(PosOnArc(posTemp1)&&PosOnLine(l_posRgn1,l_posRgn4,posTemp1))
				l_aryArcPoint.Add(posTemp1);
			if (PosOnArc(posTemp2)&&PosOnLine(l_posRgn1,l_posRgn4,posTemp2))
				l_aryArcPoint.Add(posTemp2);
		}
	}
	//交点个数
	int l_iNumArcPoint = l_aryArcPoint.GetSize();
	//夹角
	double l_dAngle = 0.0;
	if (l_iNumArcPoint >2)
		return CaleStartAndOffset(dAngleStart,dAngleOffset);
	else if(l_iNumArcPoint == 1)//圆弧与客户区只有一个交点
	{
		Position l_posInterArc = l_aryArcPoint.GetAt(0);
		if(PosBelongBox(l_posRgn1,l_posRgn3,m_Begin))//起点在客户区
		{
			l_dAngle = GetAngle(m_Center,l_posInterArc);
			if(m_iDirect == AD_CLOCKWISE)		//顺时针
			{
				if(l_dAngle<m_dAngleStart)//没过0度角时
					dAngleOffset = l_dAngle - m_dAngleStart;
				else//过零度角时
					dAngleOffset = l_dAngle - 360 -m_dAngleStart;
			}
			else//逆时针
			{
				if(l_dAngle>m_dAngleStart)//没过0度角时
					dAngleOffset = l_dAngle - m_dAngleStart;
				else//过零度角时
					dAngleOffset = 360 -m_dAngleStart + l_dAngle;
			}
			dAngleStart = m_dAngleStart;
			return TRUE;
		}
		if(PosBelongBox(l_posRgn1,l_posRgn3,m_End))//终点在客户区
		{
			l_dAngle = GetAngle(m_Center,l_posInterArc);
			g_pView->WorldToScreem(l_posInterArc,potBegin); 
			dAngleStart = l_dAngle;
			if(m_iDirect == AD_CLOCKWISE)		//顺时针
			{
				if(m_dAngleEnd<l_dAngle)//没过0度角时
					dAngleOffset = m_dAngleEnd - l_dAngle;
				else//过零度角时
					dAngleOffset = m_dAngleEnd - 360 -l_dAngle;
			}
			else//逆时针
			{
				if(m_dAngleEnd>l_dAngle)//没过0度角时
					dAngleOffset = m_dAngleEnd - l_dAngle;
				else//过零度角时
					dAngleOffset = 360 -l_dAngle + m_dAngleEnd;
			}
			return TRUE;
		}	
	}
	else if(l_iNumArcPoint == 2)
	{	//终点和起点在客户区
		if(PosBelongBox(l_posRgn1,l_posRgn3,m_End)&&PosBelongBox(l_posRgn1,l_posRgn3,m_Begin))
			return CaleStartAndOffset(dAngleStart,dAngleOffset);
		else
		{
			Position l_posInterArc1 = l_aryArcPoint.GetAt(0);			
			Position l_posInterArc2 = l_aryArcPoint.GetAt(1);
			//两个交点与圆心角度，与起始点的偏置角
			double angle1,angle2,l_dAngleOffset1,l_dAngleOffset2;
			angle1 = GetAngle(m_Center,l_posInterArc1);
			angle2 = GetAngle(m_Center,l_posInterArc2);
			if(m_iDirect == AD_CLOCKWISE)		//顺时针
			{
				if(angle1<m_dAngleStart)//没过0度角时
					l_dAngleOffset1 = angle1 - m_dAngleStart;
				else//过零度角时
					l_dAngleOffset1 = angle1 - 360 -m_dAngleStart;
				if(angle2<m_dAngleStart)//没过0度角时
					l_dAngleOffset2 = angle2 - m_dAngleStart;
				else//过零度角时
					l_dAngleOffset2 = angle2 - 360 -m_dAngleStart;
				if(l_dAngleOffset1 < l_dAngleOffset2)
				{
					dAngleStart = angle2;
					g_pView->WorldToScreem(l_posInterArc2,potBegin); 
					dAngleOffset = l_dAngleOffset1 - l_dAngleOffset2;
				}				
				else
				{
					dAngleStart = angle1;
					g_pView->WorldToScreem(l_posInterArc1,potBegin); 
					dAngleOffset = l_dAngleOffset2 - l_dAngleOffset1;					
				}
				
			}
			else//逆时针
			{
				if(angle1>m_dAngleStart)//没过0度角时
					l_dAngleOffset1 = angle1 - m_dAngleStart;
				else//过零度角时
					l_dAngleOffset1 = 360 -m_dAngleStart + angle1;
				if(angle2>m_dAngleStart)//没过0度角时
					l_dAngleOffset2 = angle2 - m_dAngleStart;
				else//过零度角时
					l_dAngleOffset2 = 360 -m_dAngleStart + angle2;
				if(l_dAngleOffset1 < l_dAngleOffset2)
				{
					dAngleStart = angle1;
					g_pView->WorldToScreem(l_posInterArc1,potBegin); 
					dAngleOffset = l_dAngleOffset2 - l_dAngleOffset1;
				}				
				else
				{
					dAngleStart = angle2;
					g_pView->WorldToScreem(l_posInterArc2,potBegin); 
					dAngleOffset = l_dAngleOffset1 - l_dAngleOffset2;					
				}
			}
			return TRUE;
		}
	}
	return FALSE;	
}

/*
 *	计算起始角和转过角度
 */
BOOL CCreateRoundArc::CaleStartAndOffset(double& dAngleStart,double& dAngleOffset)
{
	if(m_iDirect == AD_CLOCKWISE)		//顺时针
	{
	 if(m_dAngleEnd<m_dAngleStart)//没过0度角时
		 dAngleOffset = m_dAngleEnd - m_dAngleStart;
	 else//过零度角时
		 dAngleOffset = m_dAngleEnd - 360 -m_dAngleStart;
	}
	else//逆时针
	{
	 if(m_dAngleEnd>m_dAngleStart)//没过0度角时
		 dAngleOffset = m_dAngleEnd - m_dAngleStart;
	 else//过零度角时
		 dAngleOffset = 360 -m_dAngleStart + m_dAngleEnd;
	}
	dAngleStart = m_dAngleStart;
	return TRUE;
}

/*
*	按比例缩放
*/
void CCreateRoundArc::GraphZoomScale(Position posBase,double dScale)
{
	//基点与起末点的角度
	double l_dAngleCT = GetAngleToXAxis(posBase,m_Center);
	double l_dAngleBG = GetAngleToXAxis(posBase,m_Begin);
	double l_dAngleEN = GetAngleToXAxis(posBase,m_End);
	//基点到起末点的长度
	double l_dLongCT = posBase.Distance(m_Center);
	m_Center.x = posBase.x + cos(l_dAngleCT)*l_dLongCT*dScale; 
	m_Center.y = posBase.y + sin(l_dAngleCT)*l_dLongCT*dScale; 
	//基点到起末点的长度
	double l_dLongEN = posBase.Distance(m_End);
	m_End.x = posBase.x + cos(l_dAngleEN)*l_dLongEN*dScale; 
	m_End.y = posBase.y + sin(l_dAngleEN)*l_dLongEN*dScale; 
	//基点到起末点的长度
	double l_dLongBG = posBase.Distance(m_Begin);
	m_Begin.x = posBase.x + cos(l_dAngleBG)*l_dLongBG*dScale; 
	m_Begin.y = posBase.y + sin(l_dAngleBG)*l_dLongBG*dScale; 
	//半径变化
	m_dRadius *= dScale;
}

/*
 *	实线变虚线
 */
PGRAPHPOS CCreateRoundArc::SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist)
{
	PGRAPHPOS headGraphPos =  TurnArcDash(dLongSolid,dLongEmpty,m_Center,m_dAngleStart,m_dAngleEnd,m_dRadius,m_iDirect);
	return headGraphPos;
}











