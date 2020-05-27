// CreateEllipse.cpp: implementation of the CCreateEllipse class.
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
	IMPLEMENT_SERIAL(CCreateEllipse, CCreateBase, 0)
CCreateEllipse::CCreateEllipse()
{
		m_iFlagArcOrLine = -1;
		
}

CCreateEllipse::CCreateEllipse(const Position& begin,const Position& second,const Position& end)
{
	m_iFlagArcOrLine = -1;
	m_ColorOfLine = g_CurColor;		//创建时的画色就是当前所选色
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_Begin = begin;
	m_End = end;
	m_Second = second;

	m_Center.x = (m_Begin.x+m_Second.x)/2;
	m_Center.y = (m_Begin.y+m_Second.y)/2;	//椭圆的中心点坐标
	m_a = m_Begin.Distance(m_Center);	//长轴长
	m_b = m_End.Distance(m_Center);		//短轴长
	double c = sqrt(fabs((m_a*m_a)-(m_b*m_b)));		//焦点长度
	if (m_a>=m_b)
	{
		m_Focus1.x = c;
		m_Focus1.y = 0;
		m_Focus2.x = -c;
		m_Focus2.y = 0;
	}
	else
	{
		m_Focus1.x = 0;
		m_Focus1.y = c;
		m_Focus2.x = 0;
		m_Focus2.y = -c;
	}
	m_k = GetAngleToXAxis(m_Begin,m_Second);	//长轴的斜率（椭圆旋转角度）
	//第一个焦点
	m_Focus1 = m_Focus1.Offset(m_Center);
	m_Focus1 = m_Focus1.Rotate(m_Center,m_k);
	//第二个焦点
	m_Focus2 = m_Focus2.Offset(m_Center);
	m_Focus2 = m_Focus2.Rotate(m_Center,m_k);
	//产生椭圆圆周上的点
/*	Position pos(0,0);
	double angle200 = MATH_PI/100;
	for(double i=0;i<MATH_2PI;i=i+angle200)
	{
		GetEllipsesPoint(pos,i);	//返回椭圆圆周上的点（中心点在圆心上）
		pos = pos.Offset(m_Center);
		pos = pos.Rotate(m_Center,m_k);	
		m_point.Add(pos);
	}*/
/*
	Position pos(0,0);
	double angle3_6 = MATH_2PI/100;
	double l_dAngleBig = angle3_6*m_a/(m_a+m_b);//最大角度
	double l_dAnglelit = angle3_6*m_b/(m_a+m_b);//最小角度
	double l_dAngleAdd = (l_dAngleBig - l_dAnglelit)/49;
	double l_dAngleEll = 0.0;
	double l_dAngleAll = 0.0;
	for(int i=1;i<201;i++)
	{
		GetEllipsesPoint(pos,l_dAngleAll);	//返回椭圆圆周上的点（中心点在圆心上）
		pos = pos.Offset(m_Center);
		pos = pos.Rotate(m_Center,m_k);	
		m_point.Add(pos);	

		if(i<=50)
			l_dAngleEll = l_dAnglelit + (l_dAngleAdd*(i-1));
		else if (i>50&&i<=100)
			l_dAngleEll = l_dAngleBig - (l_dAngleAdd*(i-51));
		else if(i>100&&i<=150)
			l_dAngleEll = l_dAnglelit + (l_dAngleAdd*(i-101));
		else
			l_dAngleEll = l_dAngleBig - (l_dAngleAdd*(i-151));
		
		l_dAngleAll = l_dAngleAll+l_dAngleEll;

	}
*/
	Position pos(0,0);
	double angle3_6 = MATH_2PI/100;
	double l_dAngleBig = angle3_6*m_a*m_a*m_a*m_a/(m_a*m_a*m_a*m_a+m_b*m_b*m_b*m_b);//最大角度
	double l_dAnglelit = angle3_6*m_b*m_b*m_b*m_b/(m_a*m_a*m_a*m_a+m_b*m_b*m_b*m_b);//最小角度
	double l_dAngleAdd = (l_dAngleBig - l_dAnglelit)/49;
	double l_dAngleEll = 0.0;
	double l_dAngleAll = 0.0;
	for(int i=1;i<202;i++)
	{
		GetEllipsesPoint(pos,l_dAngleAll);	//返回椭圆圆周上的点（中心点在圆心上）
		pos = pos.Offset(m_Center);
		pos = pos.Rotate(m_Center,m_k);	
		m_point.Add(pos);	
		
		if(i<=50)
			l_dAngleEll = l_dAnglelit + (l_dAngleAdd*(i-1));
		else if (i>50&&i<=100)
			l_dAngleEll = l_dAngleBig - (l_dAngleAdd*(i-51));
		else if(i>100&&i<=150)
			l_dAngleEll = l_dAnglelit + (l_dAngleAdd*(i-101));
		else if(i>150)
			l_dAngleEll = l_dAngleBig - (l_dAngleAdd*(i-151));
		
		l_dAngleAll = l_dAngleAll+l_dAngleEll;
		
	}
	//把凸度的值负进去
	int l_NumOfm_point = m_point.GetSize();
	double aaa = 0.0;
	for(int k = 0;k<l_NumOfm_point;k++)
		m_dTuDu.Add(aaa);
	m_iKnifeStyle = 41;//刀补类型 41左刀补 42右刀补
	m_iIsBeginGraph = 2;//0不是原始图形，1是原始图形，2是保存时不关心
}

CCreateEllipse::~CCreateEllipse()
{
	m_point.RemoveAll();
}


void CCreateEllipse::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{	
	CPoint pt_begin;// 屏幕坐标点
	Position pt;
	double angle;//两点与X轴的角度
	
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
	
	for(int i=0;i<m_point.GetSize();i++)
	{
		pt=m_point.GetAt(i); 
		// 将世界坐标转化为屏幕坐标
		g_pView->WorldToScreem(pt,pt_begin); 
		// 得到原来的绘图模式
		// 根据屏幕坐标绘制图元
		if (i!=0)
		{
			pDC->LineTo(pt_begin); 
			if (FlagShowArrow&&i==200)//是否显示箭头
			{
				angle = GetAngleToXAxis(m_PrePos,pt);
				DrawArrow(angle,pt_begin,pDC);//画箭头的函数 
			}
		}
		else
		{
			if (FlagShowArrow)//是否显示方框
			{
				CRect rect(pt_begin.x-5,pt_begin.y-5,pt_begin.x+5,pt_begin.y+5);
				pDC->SelectStockObject(NULL_BRUSH);
				pDC->Rectangle(rect);
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
					if(this == (CCreateEllipse *)l_pDelList)//如果与链表里的元素一样，就要记下位置
					{
						CString str;
						str.Format("%d",i-l_iNumOfMCode);
						int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //这样就可以透明化背景了				
						pDC->TextOut(pt_begin.x,pt_begin.y,str);//输出到界面 上
						pDC->SetBkMode(l_iOldMode);
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			pDC->MoveTo(pt_begin);
		}
		m_PrePos = pt;
	}
	// 恢复原来的画笔 
	pDC->SelectObject(pOldPen); 
	// 恢复原来的绘图模式
	pDC->SetROP2(n);
	pen.DeleteObject();//释放pen的Create的;
}

int CCreateEllipse::IsPick(CPoint point)
{
	Position Pos1,Pos2;// 世界坐标点
	CPoint pt_pos1,pt_pos2;// 屏幕坐标点
	for(int i=0;i<m_point.GetSize()-1;i++)
	{
		Pos1=m_point.GetAt(i); 
		Pos2=m_point.GetAt(i+1); 
		// 世界坐标转化为屏幕坐标
		g_pView->WorldToScreem(Pos1,pt_pos1); 
		g_pView->WorldToScreem(Pos2,pt_pos2); 
		//把线段的斜率也要算进去，下面是算斜率的。
		double angle = GetAngleToXAxis(Pos1,Pos2);
		double d_Sin = PRECISION * sin(angle);
		double d_Cos = PRECISION * cos(angle);
		
		CRgn	pr;
		POINT	pt[4];//这个区域由四个点组成，
		pt[0].x = pt_pos1.x - (int)d_Sin;
		pt[0].y = pt_pos1.y - (int)d_Cos;//起始点上的一点
		
		pt[1].x = pt_pos1.x + (int)d_Sin;
		pt[1].y = pt_pos1.y + (int)d_Cos;//起始点下的一点
		
		pt[2].x = pt_pos2.x + (int)d_Sin;
		pt[2].y = pt_pos2.y + (int)d_Cos;//终点下的一点
		
		pt[3].x = pt_pos2.x - (int)d_Sin;
		pt[3].y = pt_pos2.y - (int)d_Cos;//终点上的一点
		
		pr.CreatePolygonRgn(pt,4,ALTERNATE);
		if(pr.PtInRegion(point)) 
		{
			pr.DeleteObject();
			return i;
			break;
		}
		pr.DeleteObject();
	}
	return -1;
/*		焦点的方法
	Position pos;
	g_pView->ScreemToWorld(point,pos);
	double l_dLong = m_Focus1.Distance(pos)+m_Focus2.Distance(pos) ;//点到两焦点的距离和;
	double l_dIsPick;	//拾取的精度
	if(m_a>m_b)
		l_dIsPick = fabs(l_dLong - 2*m_a)/g_pView->ReturnProportion();	
	else
		l_dIsPick = fabs(l_dLong - 2*m_b)/g_pView->ReturnProportion();
	if (l_dIsPick<PRECISION)
		return TRUE;
	return FALSE;*/

}

CCreateBase* CCreateEllipse::Copy()
{
	CCreateEllipse *pCopy = new CCreateEllipse(m_Begin,m_Second,m_End);//复制点
	pCopy->m_ColorOfLine = m_ColorOfLine;		//复制线色
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	pCopy->m_a = m_a;
	pCopy->m_b = m_b;
	pCopy->m_Center = m_Center;
	pCopy->m_k = m_k;
	pCopy->m_Focus1 = m_Focus1;
	pCopy->m_Focus2 = m_Focus2;
	return pCopy;
}

//移动
void CCreateEllipse::Move(const Position& basePos,const Position& desPos)
{
	for (int i=0;i < m_point.GetSize();i++)
	{
		Position pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Offset(desPos - basePos));	
		Position pt1 = m_point.ElementAt(i);
	}
	m_Begin = m_Begin.Offset(desPos - basePos);
	m_End = m_End.Offset(desPos - basePos);
	m_Second = m_Second.Offset(desPos - basePos);
	m_Center = m_Center.Offset(desPos - basePos);
	m_Focus1 = m_Focus1.Offset(desPos - basePos);
	m_Focus2 = m_Focus2.Offset(desPos - basePos);
}
//旋转
void CCreateEllipse::Rotate(const Position& basePos, const double angle)
{
	for (int i=0;i < m_point.GetSize();i++)
	{
		Position pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Rotate(basePos,angle));	
		Position pt1 = m_point.ElementAt(i);
	}
	m_Begin = m_Begin.Rotate(basePos, angle) ;
	m_End =m_End.Rotate(basePos, angle) ;
	m_Second =m_Second.Rotate(basePos, angle) ;
	m_Center = m_Center.Rotate(basePos,angle);
	m_Focus1 =m_Focus1.Rotate(basePos, angle) ;
	m_Focus2 =m_Focus2.Rotate(basePos, angle) ;
}
//镜像
void CCreateEllipse::Mirror(const Position& pos1, const Position& pos2)
{
	for (int i=0;i < m_point.GetSize();i++)
	{
		Position pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Mirror(pos1,pos2));	
		Position pt1 = m_point.ElementAt(i);
	}
	m_Begin = m_Begin.Mirror(pos1, pos2) ;
	m_End =m_End.Mirror(pos1, pos2) ;
	m_Second =m_Second.Mirror(pos1, pos2) ;
	m_Center = m_Center.Mirror(pos1,pos2);
	m_Focus1 =m_Focus1.Mirror(pos1, pos2) ;
	m_Focus2 = m_Focus2.Mirror(pos1,pos2);
}

//根据角度求象限
int CCreateEllipse::GetArcQuadrant(double &fArc)
{
	UINT nIndex;
	if(fArc<0)
		fArc += MATH_2PI;
	if(fArc>MATH_2PI)
		fArc -= MATH_2PI;
	if(fArc<=(MATH_PI/2))			nIndex=0;
	else if(fArc<=(MATH_PI))			nIndex=1;
	else if(fArc<=(MATH_PI*1.5f))	nIndex=2;
	else							nIndex=3;
	return nIndex;
}

//获取椭圆弧上对应角度为fArc的一个点的坐标
void CCreateEllipse::GetEllipsesPoint(Position &output, double fArc)
{
	double fTan, fFx, fFy;
	UINT nQuadrant;
	
	if(fArc<0)
		fArc += MATH_2PI;
	if(fArc>MATH_2PI)
		fArc -= MATH_2PI;
	
   	nQuadrant = GetArcQuadrant(fArc);
	switch(nQuadrant)
	{
	case 0:
		fArc = fArc;
		fFx = 1;	fFy = 1;
		break;
	case 1:
		fArc = MATH_PI-fArc;
		fFx = -1;	fFy = 1;
		break;
	case 2:
		fArc = fArc-MATH_PI;
		fFx = -1;	fFy = -1;
		break;
	case 3:
		fArc = MATH_2PI-fArc;
		fFx = 1;	fFy = -1;
		break;
	default:
		break;
	}
	
	fTan = tan(fArc);
	
	output.x = (m_a*m_b)/(sqrt((m_b*m_b)+(m_a*m_a)*(fTan*fTan)));
	output.y = output.x*fTan;
	output.x *= fFx;
	output.y *= fFy;
}


/*
*	属性对话框
*/
void CCreateEllipse::PropertyView()
{
	//显示属性对话框
	(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 
	
	//把隐藏的STATIC和Edit显示出来
	ShowControl(3,TRUE);
	
	//设定要显示的颜色	
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(RtnColorInt()-1);
	
	//设置STATIC要显示“起点和终点”
	//设置Edit要显示起点和终点的位置
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"顶点");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"顶点X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"顶点Y");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Vertex");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Vertex X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"Vertex Y");
	}
	CString str;
	Position pos = m_point.GetAt(0);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,"1");
	str.Format("%.4f",pos.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
	str.Format("%.4f",pos.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	//让顶点数从1开始
	((CMainFrame*)AfxGetMainWnd())->m_iNumOfTop = 1;
	////////////////////////遍历链表里的看自己排在第几位，刀顺时用///////////////////////////////////
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	for (int i=0;l_ListPos != NULL;i++)//遍历链表，i可表示链表里的排序号
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//如果是M代码
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(this == (CCreateEllipse *)l_pDelList)//如果与链表里的元素一样，就要记下位置
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
void CCreateEllipse::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	Position pos;
	switch(WhichEdit) //哪个EDIT框改了，就变哪个值
	{
	case 2:
		pos = m_point.GetAt(iNumOfTop-1);
		pos.x = e;
		m_point.SetAt(iNumOfTop-1,pos);
		break;
	case 3:
		pos = m_point.GetAt(iNumOfTop-1);
		pos.y = e;
		m_point.SetAt(iNumOfTop-1,pos);
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
BOOL CCreateEllipse::IsSelect(CRgn *selectrect,BOOL bComplex)
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
		if (EllipseOnRect(l_posRgn1,l_posRgn2))
			return TRUE;
	}
	BOX2D pBox;		//包围盒结构
	CPoint pos1,pos2;	//包围盒的两个屏幕顶点（左上，右下）
	//以下是得到包围盒的两个点，世界坐标（左下，右上）
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
BOOL CCreateEllipse::GetBox(BOX2D *pBox,BOOL IsOutside /* = FALSE */)
{
	Position CurPos,PrePosMin,PrePosMax;//当前点和过去点
	PrePosMin=m_point.GetAt(0); 
	PrePosMax=m_point.GetAt(0); 
	for(int i=0;i<m_point.GetSize()-1;i++)
	{
		CurPos=m_point.GetAt(i+1);
		//取所有点中最小的和最大的
		PrePosMin.x = min( PrePosMin.x, CurPos.x );
		PrePosMin.y = min( PrePosMin.y, CurPos.y );
		PrePosMax.x = max( PrePosMax.x, CurPos.x );
		PrePosMax.y = max( PrePosMax.y, CurPos.y );
	}
	//把包围盒外括
	double curRadius = g_pView->ReturnProportion()*PRECISION;
	//如果没有外括
	if (IsOutside == FALSE)
	curRadius = 0.0;
	pBox->min[0] = PrePosMin.x - curRadius;
	pBox->min[1] = PrePosMin.y - curRadius;
	pBox->max[0] = PrePosMax.x + curRadius;
	pBox->max[1] = PrePosMax.y + curRadius;
	return TRUE;
}

//XY缩放功能
BOOL CCreateEllipse::GraphZoomXY(double dValueX, double dValueY)
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
	Position pos1(0,0);
	int l_iSizeOfPoint = m_point.GetSize();
	for(int i=0;i<l_iSizeOfPoint;i++)
	{
		pos1 = m_point.GetAt(i);
		d1 = (pos1.x - pBox.min[0])/dXRadio;
		if (d1>1)//当添的值为零时
			d1 = 1;
		d2 = d1 * dValueX;
		pos1.x += d2;
		
		d1 = (pos1.y - pBox.min[1])/dYRadio;
		if (d1>1)
			d1 = 1;
		d2 = d1 * dValueY;
		pos1.y += d2;
		
		m_point.SetAt(i, pos1);
	}	
	return TRUE;
}


/*
 *	对于图形上的一点输入长度返回一个点
 */
BOOL CCreateEllipse::RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine /* = 0 */)
{
	//返回点在哪条线上
	CPoint pot;
	g_pView->WorldToScreem(ClickPos,pot);
	int l_iNumClick = this->IsPick(pot);
	//起点			终点			圆心
	Position l_posBegin,l_posEnd,l_posCenter;
	//凸度
	double l_dTudu,l_dAngle,l_dFuLongPos = -dLongPos;
	int l_iSize = m_point.GetSize();
	for(int i = l_iNumClick;i<l_iSize-1&&i>=0;)
	{
		l_dTudu = m_dTuDu.GetAt(i);
		l_posBegin = m_point.GetAt(i);		
		l_posEnd = m_point.GetAt(i+1);
		//直线
		if (l_dTudu == 0.0)
		{
			if (dLongPos>0)//输入的大于0说明是距末点的距离
			{
				if(ClickPos.Distance(l_posEnd)<dLongPos)//如果输入的比本段直线长就要减去本段线
				{
					dLongPos = dLongPos - ClickPos.Distance(l_posEnd);
					ClickPos = l_posEnd;
					i++;
				}
				else//否则求点
				{
					//直线起点与终点的角度 
					l_dAngle = GetAngleToXAxis(l_posBegin,l_posEnd);
					posTarget.x = ClickPos.x + dLongPos*cos(l_dAngle);
					posTarget.y = ClickPos.y + dLongPos*sin(l_dAngle);
					return TRUE;
				}
			}
			else//输入的小于0说明是距起点的距离    (dLongPos<0)
			{
					
				if(ClickPos.Distance(l_posBegin)<l_dFuLongPos)//如果输入的比本段直线长就要减去本段线
				{
					l_dFuLongPos = l_dFuLongPos - ClickPos.Distance(l_posBegin);
					ClickPos = l_posBegin;
					i--;
				}
				else//否则求点
				{
					//直线起点与终点的角度 
					l_dAngle = GetAngleToXAxis(l_posBegin,l_posEnd);
					posTarget.x = ClickPos.x - l_dFuLongPos*cos(l_dAngle);
					posTarget.y = ClickPos.y - l_dFuLongPos*sin(l_dAngle);	
					return TRUE;
				}
			}
		}
		else//圆弧 
		{
			double l_dLongArc;//弧长
			if (dLongPos>0)//输入的大于0说明是距末点的距离
			{
				l_dLongArc = RtnLongArc(ClickPos,l_posBegin,l_posEnd,l_dTudu);//弧长
				if(l_dLongArc < dLongPos)//如果输入的比本段弧线长就要减去本弧线
				{
					dLongPos = dLongPos - l_dLongArc;
					ClickPos = l_posEnd;
					i++;
				}
				else//否则求点
				{
					posTarget = RtnLongArcPos(l_posBegin,l_posEnd,l_dTudu,ClickPos,dLongPos);
					return TRUE;
				}
			}
			else//输入的小于0说明是距起点的距离    (dLongPos<0)
			{
				l_dLongArc = RtnLongArc(ClickPos,l_posEnd,l_posBegin,-l_dTudu);//弧长				
				if(l_dLongArc < l_dFuLongPos)//如果输入的比本段直线长就要减去本段线
				{
					l_dFuLongPos = l_dFuLongPos - l_dLongArc;
					ClickPos = l_posBegin;
					i--;
				}
				else//否则求点
				{
					posTarget = RtnLongArcPos(l_posEnd,l_posBegin,-l_dTudu,ClickPos,l_dFuLongPos);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/*
 *	输入起点终点凸度返回弧长
 */
double CCreateEllipse::RtnLongArc(Position posClick,Position posBegin,Position PosEnd,double dTudu)
{
	//求圆心
	Position l_posCenter;
	g_pDoc->polylineToArc(posBegin,PosEnd,dTudu,l_posCenter);
	//求半径
	double l_dRadius = posBegin.Distance(l_posCenter);
	//求周长
	double l_dPerimeter = 2*MATH_PI*l_dRadius;
	//求夹角
	double l_dAngleInclude,l_dLongArc,l_dAngleStart,l_dAngleEnd;
	//起始角，终止角
	l_dAngleStart = GetAngleToXAxis(l_posCenter,posBegin);
	l_dAngleEnd = GetAngleToXAxis(l_posCenter,PosEnd);
	l_dLongArc = GetAngleToXAxis(l_posCenter,posClick);
	if (dTudu<0.0)//顺时针
	{
		if (l_dLongArc<l_dAngleEnd)//过零度线时，就是X轴正方向
			l_dAngleInclude = l_dLongArc +MATH_2PI - l_dAngleEnd;		
		else
			l_dAngleInclude = l_dLongArc - l_dAngleEnd;
	}
	else//逆时针
	{
		if (l_dLongArc>l_dAngleEnd)//过零度线时，就是X轴正方向
			l_dAngleInclude = l_dAngleEnd +MATH_2PI - l_dLongArc;		
		else
			l_dAngleInclude = l_dAngleEnd - l_dLongArc;		
	}	
	return (l_dAngleInclude/MATH_2PI)*l_dPerimeter;
}

/*
 *	输入起点，终点，凸度，点击点，弧长，返回弧上点
 *	输入参数时要注意，是对于圆弧的起点终点和方向
 */
Position CCreateEllipse::RtnLongArcPos(Position posBegin,Position PosEnd,double dTudu,Position posClick,double dLongPos)
{
	//求圆心
	Position l_posCenter,posTarget(0,0);
	g_pDoc->polylineToArc(posBegin,PosEnd,dTudu,l_posCenter);
	//半径
	double l_dRadius = l_posCenter.Distance(posBegin);
	//圆心到鼠标点击的角度
	double l_dAngle = GetAngleToXAxis(l_posCenter,posClick);
	double l_dLongArc,l_dAngle2,l_dAngleStart,l_dAngleEnd;
	//起始角，终止角
	l_dAngleStart = GetAngleToXAxis(l_posCenter,posBegin);
	l_dAngleEnd = GetAngleToXAxis(l_posCenter,PosEnd);
	//圆弧的周长
	double l_dPerimeter = 2*MATH_PI*l_dRadius;
	//如果输入的长度大于圆弧的长度就不满足
	if (dLongPos > 0)//判断离末点的距离
	{
		if (dTudu < 0.0)//顺时针 
		{	//末点和点击点在零度线的两边
			if (l_dAngleStart<l_dAngleEnd&&l_dAngle<l_dAngleEnd)//末点过零度线并且点击没过零度线
			{
				l_dLongArc = ((l_dAngle+MATH_2PI-l_dAngleEnd)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle+MATH_2PI-((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
			else//末点与点击点在零度线一边
			{
				l_dLongArc = ((l_dAngle-l_dAngleEnd)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle-((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
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
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
			else//末点与点击点在零度线一边
			{
				l_dLongArc = ((l_dAngleEnd-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = ((dLongPos/l_dPerimeter)*MATH_2PI)+l_dAngle;//长度对就的角度
					//求弧长对应有角度
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
		}
	}
	else if (dLongPos < 0 )
	{
		dLongPos = fabs(dLongPos);			
		if (dTudu < 0.0)//顺时针 
		{	//起点和点击点在零度线的两边
			if (l_dAngleStart<l_dAngleEnd&&l_dAngle>l_dAngleStart)//起点过零度线并且点击没过零度线
			{
				l_dLongArc = ((l_dAngleStart+MATH_2PI-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle-MATH_2PI+((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
			else//起点与点击点在零度线一边
			{
				l_dLongArc = ((l_dAngleStart-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle+((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
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
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
			else//起点与点击点在零度线一边
			{
				l_dLongArc = ((l_dAngle-l_dAngleStart)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//如果输入的长度满足弧长
				{
					l_dAngle2 = l_dAngle-((dLongPos/l_dPerimeter)*MATH_2PI);//长度对就的角度
					//求弧长对应有角度
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
		}
	}
	return posTarget;
}

/*
 *	改变切割方向
 */
void CCreateEllipse::ChangeDir()
{
	Position l_ScreemPos;
	CArray<Position,Position&> l_ListPoint; 
	for(int i=m_point.GetSize();  i>0;  i--)
	{
		l_ScreemPos = m_point.GetAt(i-1); 
		l_ListPoint.Add(l_ScreemPos);			
	}
	m_point.Copy(l_ListPoint);	
}

/*
 *	看椭圆是否与矩形有交点并且交点是否在直线上
 */
BOOL CCreateEllipse::EllipseOnRect(Position pos1,Position pos3)
{
	Position pos2(pos1.x,pos3.y),pos4(pos3.x,pos1.y),l_posInter;
	Position l_posBegin,l_posEnd;
	int l_iEllipseOfSize = m_point.GetSize();
	for(int i=0;i<l_iEllipseOfSize-1;i++)
	{	
		l_posBegin = m_point.GetAt(i);
		l_posEnd = m_point.GetAt(i+1);
		if (GetIntersectPoint(l_posBegin,l_posEnd,pos1,pos2,l_posInter)&&PosOnLine(pos1,pos2,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
			return TRUE;
		else if (GetIntersectPoint(l_posBegin,l_posEnd,pos2,pos3,l_posInter)&&PosOnLine(pos2,pos3,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
			return TRUE;
		else if (GetIntersectPoint(l_posBegin,l_posEnd,pos3,pos4,l_posInter)&&PosOnLine(pos3,pos4,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
			return TRUE;
		else if (GetIntersectPoint(l_posBegin,l_posEnd,pos4,pos1,l_posInter)&&PosOnLine(pos4,pos1,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
			return TRUE;
	}
	return FALSE;
}

/*
*	按比例缩放
*/
void CCreateEllipse::GraphZoomScale(Position posBase,double dScale)
{
	//点集的大小 
	int l_iSize = m_point.GetSize();
	//每个点
	Position posCur;
	//基点与点的,角度,	 长度
	double l_dAnglePos,l_dLongPos;
	for(int i=0;i<l_iSize;i++)
	{
		posCur = m_point.GetAt(i);
		l_dAnglePos = GetAngleToXAxis(posBase,posCur);
		l_dLongPos = posBase.Distance(posCur);
		//形成的新点
		posCur.x = posBase.x + cos(l_dAnglePos)*l_dLongPos*dScale; 
		posCur.y = posBase.y + sin(l_dAnglePos)*l_dLongPos*dScale; 
		m_point.SetAt(i,posCur);		
	}
}

/*
*	实线变虚线
*/
PGRAPHPOS CCreateEllipse::SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist)
{
	int l_iSize = m_point.GetSize()-1;
	double l_dTudu = 0.0,l_dRadius,l_dAngleStart,l_dAngleEnd,l_dRemainingEmty =0.0;
	Position l_posBegin,l_posEnd,l_posCenter;//起点终点
	PGRAPHPOS headGraphPos = NULL;//总虚线段的集
	for(int i=0;i<l_iSize;i++)
	{
		l_dTudu = m_dTuDu.GetAt(i);
		l_posBegin = m_point.GetAt(i);
		l_posEnd = m_point.GetAt(i+1);
		PGRAPHPOS GraphPos;//虚线段的集
		if(l_dTudu == 0.0)//直线
		{
			GraphPos = TurnLineDash(dLongSolid,dLongEmpty,l_posBegin,l_posEnd);			
		}
		else//圆弧
		{
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);
			l_dAngleStart = GetAngleToXAxis(l_posCenter,l_posBegin)/MATH_PI*180;
			l_dAngleEnd = GetAngleToXAxis(l_posCenter,l_posEnd)/MATH_PI*180;
			l_dRadius = l_posCenter.Distance(l_posBegin);
			GraphPos = TurnArcDash(dLongSolid,dLongEmpty,l_posCenter,l_dAngleStart,l_dAngleEnd,l_dRadius,RtnArcDir(l_dTudu));			
		}
		//添加图形
		if(GraphPos != NULL)//如果没有图形生成就不存
		{
			if (headGraphPos == NULL)
				headGraphPos = GraphPos;
			else
				g_pDoc->AddTail(headGraphPos,GraphPos);
		}
	}
	return headGraphPos;
}
