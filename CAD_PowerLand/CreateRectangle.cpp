// CreateRectangle.cpp: implementation of the CCreateRectangle class.
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
IMPLEMENT_SERIAL(CCreateRectangle, CCreateBase, 0)
CCreateRectangle::CCreateRectangle()
{
	
}

//给修改的那些函数使用，Copy()函数使用。
CCreateRectangle::CCreateRectangle(CArray<Position,Position&>&  point,CArray<double,double&>& TuDu)
{
	m_ColorOfLine = g_CurColor;		//创建时的画色就是当前所选色
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_point.Copy(point);
	m_dTuDu.Copy(TuDu);
	m_iKnifeStyle = 41;//刀补类型 41左刀补 42右刀补
	m_iIsBeginGraph = 2;//0不是原始图形，1是原始图形，2是保存时不关心
	m_iFlagArcOrLine = -1;
	
}

//给画图，创建图元时使用。
CCreateRectangle::CCreateRectangle(const Position& begin,const Position& end,BOOL FlagRoundRect,double Radius)
{
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	if (FlagRoundRect == FALSE||Radius < 0.001)//不是圆角矩形
	{
		m_ColorOfLine = g_CurColor;		//创建时的画色就是当前所选色
		Position pos1,pos2,pos3,pos4;
		pos1 = begin;
		pos3 = end;
		if((begin.x<=end.x&&begin.y>=end.y)||(begin.x > end.x&&begin.y<=end.y))//从左上角到右下角
		{
			pos2.x = begin.x;
			pos2.y = end.y;
			pos4.x = end.x;
			pos4.y = begin.y;
		}			
		else if((begin.x <= end.x&&begin.y<=end.y)||(begin.x >= end.x&&begin.y>=end.y))//从左下角到右上角
		{
			pos2.x = end.x;
			pos2.y = begin.y;
			pos4.x = begin.x;
			pos4.y = end.y;			
		}
		m_point.Add(pos1);
		m_point.Add(pos2);
		m_point.Add(pos3);
		m_point.Add(pos4);
		m_point.Add(pos1);
		//把凸度的值负进去
		int l_NumOfm_point = m_point.GetSize();
		double aaa = 0.0;
		for(int i = 0;i<l_NumOfm_point;i++)
			m_dTuDu.Add(aaa);
	}
	else//圆角矩形
	{
		m_ColorOfLine = g_CurColor;		//创建时的画色就是当前所选色
		Position pos,posb(0,10),posc(10,10),pose(10,0);
		double aaa = g_pDoc->RtnTudu(posb,pose,posc,AD_COUNTERCLOCKWISE);
		double zoer = 0.0;
		if(begin.x<end.x&&begin.y>end.y)//从左上角到右下角
		{
			pos.x = begin.x;
			pos.y = begin.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x;
			pos.y = end.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x + Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x - Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x;
			pos.y = end.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x;
			pos.y = begin.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x - Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x + Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x;
			pos.y = begin.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
		}
		else if(begin.x < end.x&&begin.y<end.y)//从左下角到右上角
		{
			pos.x = begin.x + Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x - Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x;
			pos.y = begin.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x;
			pos.y = end.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x - Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x + Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x;
			pos.y = end.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x;
			pos.y = begin.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x + Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
		}
		else if(begin.x > end.x&&begin.y<end.y)//从右下角到左上角
		{
			pos.x = begin.x;
			pos.y = begin.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x;
			pos.y = end.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x - Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x + Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x;
			pos.y = end.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x;
			pos.y = begin.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x + Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x - Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x;
			pos.y = begin.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
		}
		else if(begin.x > end.x&&begin.y>end.y)//从左下角到右上角
		{
			pos.x = begin.x - Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x + Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x;
			pos.y = begin.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x;
			pos.y = end.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x + Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x - Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x;
			pos.y = end.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x;
			pos.y = begin.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x - Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
		}
	}
	m_iKnifeStyle = 41;//刀补类型 41左刀补 42右刀补
	m_iIsBeginGraph = 2;//0不是原始图形，1是原始图形，2是保存时不关心	
	m_iFlagArcOrLine = -1;
	
}

CCreateRectangle::~CCreateRectangle()
{
	m_point.RemoveAll();
	m_dTuDu.RemoveAll();
}

/*
void CCreateRectangle::Draw(CDC *pDC)
{
	CPoint  l_ScreemPtO,l_ScreemPtT;
	g_pView->WorldToScreem(m_Begin,l_ScreemPtO);
	g_pView->WorldToScreem(m_End,l_ScreemPtT);
	Graphics myGraphics(pDC->m_hDC);
	Pen myPen(Color(0, 0 ,0),1); 
	
	if (l_ScreemPtT.x > l_ScreemPtO.x  &&  l_ScreemPtT.y < l_ScreemPtO.y)//向一象限拉
		myGraphics.DrawRectangle(&myPen,l_ScreemPtO.x,l_ScreemPtT.y,
		l_ScreemPtT.x-l_ScreemPtO.x,l_ScreemPtO.y-l_ScreemPtT.y); 

	if (l_ScreemPtT.x > l_ScreemPtO.x  &&  l_ScreemPtT.y > l_ScreemPtO.y)//向四象限拉
		myGraphics.DrawRectangle(&myPen,l_ScreemPtO.x,l_ScreemPtO.y,
		l_ScreemPtT.x-l_ScreemPtO.x,l_ScreemPtT.y-l_ScreemPtO.y); 

	if (l_ScreemPtT.x < l_ScreemPtO.x  &&  l_ScreemPtT.y > l_ScreemPtO.y)//向三象限拉
		myGraphics.DrawRectangle(&myPen,l_ScreemPtT.x,l_ScreemPtO.y,
		l_ScreemPtO.x-l_ScreemPtT.x,l_ScreemPtT.y-l_ScreemPtO.y); 
	
	if (l_ScreemPtT.x < l_ScreemPtO.x  &&  l_ScreemPtT.y < l_ScreemPtO.y)//向二象限拉
		myGraphics.DrawRectangle(&myPen,l_ScreemPtT.x,l_ScreemPtT.y,
		l_ScreemPtO.x-l_ScreemPtT.x,l_ScreemPtO.y-l_ScreemPtT.y); 
	


}*/


void CCreateRectangle::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{	//当前点     前一点
	Position l_posCurPos,l_posNextPos;
	double l_dTuDu;
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
	int l_iSizeOfPos = m_point.GetSize();//多段线共有多少个点
	int l_iStyleFlag = LINE;	//图形类型。1 圆弧，2 直线。
	for(int i=0;i<l_iSizeOfPos-1;i++)
	{
		//获得当前点和下一点
		l_posCurPos = m_point.GetAt(i); 
		l_posNextPos = m_point.GetAt(i+1);
		//获得凸度
		l_dTuDu = m_dTuDu.GetAt(i);						
		// 得到原来的绘图模式
		// 根据屏幕坐标绘制图元
		DrawLineOrArc(pDC,i,FlagShowArrow,FlagShowNum,l_posCurPos,l_posNextPos,l_dTuDu);
	}
	//////////////////////////////////////////////////////////////////////////	
	// 恢复原来的画笔 
	pDC->SelectObject(pOldPen); 
	// 恢复原来的绘图模式
	pDC->SetROP2(n); 
	pen.DeleteObject();//释放pen的Create的;
}

/*
 *	画或圆弧
 */
void CCreateRectangle::DrawLineOrArc(CDC *pDC,int i,BOOL FlagShowArrow,BOOL FlagShowNum,Position l_posCurPos,Position l_posNextPos,double TuDu)
{
	// 屏幕坐标点
	CPoint pt_begin,pt_End;
	// 将世界坐标转化为屏幕坐标
	g_pView->WorldToScreem(l_posCurPos,pt_begin); 
	g_pView->WorldToScreem(l_posNextPos,pt_End); 
	//画圆或直线
	if(i==0)
		pDC->MoveTo(pt_begin);
	DrawLineAndArc(pDC,FlagShowArrow,l_posCurPos,l_posNextPos,TuDu);
	if (FlagShowArrow&&i==0)//是否显示起始点的方框
	{
		CRect rect(pt_begin.x-5,pt_begin.y-5,pt_begin.x+5,pt_begin.y+5);
 		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
	}
	///////////////////////遍历链表里的看自己排在第几位，刀顺时用然后输出到界面 上////////
	if (FlagShowNum&&i==0)//画数字
	{
		POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
		int l_iNumOfMCode = 0;
		for (int i=0;l_ListPos != NULL;i++)//遍历链表，i可表示链表里的排序号
		{
			CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
			if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
				l_iNumOfMCode++;
			if(this == (CCreateRectangle *)l_pDelList)//如果与链表里的元素一样，就要记下位置
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //这样就可以透明化背景了				
				pDC->TextOut(pt_begin.x+1,pt_begin.y+1,str);//输出到界面 上
				pDC->SetBkMode(l_iOldMode);				
			}
		}
	}
}

/*
 *	画或圆弧
 */
void CCreateRectangle::DrawLineAndArc(CDC *pDC,BOOL FlagShowArrow,Position l_posCurPos,Position l_posNextPos,double TuDu)
{
	//两点与X轴的角度		圆弧半径  起始角    偏差角
	double angle,    l_fRadius,l_fAngleBG,l_fAngleOffset;
	// 屏幕坐标点
	CPoint pt_begin,pt_End,pt_Center;
	// 将世界坐标转化为屏幕坐标
	g_pView->WorldToScreem(l_posCurPos,pt_begin); 
	g_pView->WorldToScreem(l_posNextPos,pt_End); 
	Position l_posCenter(0,0);
	if(TuDu == 0.0)	//直线
	{
		if(m_iFlagArcOrLine == 2)	//不加这个单击图形时显示不好
			pDC->MoveTo(pt_begin);
		pDC->LineTo(pt_End); 
		//是否显示箭头
		if (FlagShowArrow)
			angle = GetAngleToXAxis(l_posCurPos,l_posNextPos);
		m_iFlagArcOrLine = 1;
	}
	else if(TuDu < 0.0)//顺弧
	{
		//求圆心点
		g_pDoc->polylineToArc(l_posCurPos,l_posNextPos,TuDu,l_posCenter);
		// 将世界坐标转化为屏幕坐标
		g_pView->WorldToScreem(l_posCenter,pt_Center);	
		//求半径，起始角度，终止角度
		l_fRadius = (float)(l_posCenter.Distance(l_posCurPos)/g_pView->ReturnProportion());
		l_fAngleBG = (float)GetAngle(l_posCenter,l_posCurPos);
		l_fAngleOffset = (float)GetAngle(l_posCenter,l_posNextPos);
		//求偏差的角度
		if(l_fAngleOffset<l_fAngleBG)//没过0度角时
			l_fAngleOffset = l_fAngleOffset - l_fAngleBG;
		else//过零度角时
			l_fAngleOffset = l_fAngleOffset - 360 -l_fAngleBG;
		//画弧
		if(m_iFlagArcOrLine == 2)	//不加这个单击图形时显示不好
			pDC->MoveTo(pt_begin);
		pDC->AngleArc(pt_Center.x,pt_Center.y,(int)l_fRadius,l_fAngleBG,l_fAngleOffset) ;
		//画箭头的角度		//是否显示箭头
		if (FlagShowArrow)
			angle = atan((l_posCenter.x-l_posNextPos.x) /(l_posNextPos.y -l_posCenter.y ));
		m_iFlagArcOrLine = 2;
	}
	else if(TuDu>0.0)//逆弧
	{
		//求圆心点
		g_pDoc->polylineToArc(l_posCurPos,l_posNextPos,TuDu,l_posCenter);
		// 将世界坐标转化为屏幕坐标
		g_pView->WorldToScreem(l_posCenter,pt_Center);	
		//求半径，起始角度，终止角度
		l_fRadius = (float)(l_posCenter.Distance(l_posCurPos)/g_pView->ReturnProportion());
		l_fAngleBG = (float)GetAngle(l_posCenter,l_posCurPos);
		l_fAngleOffset = (float)GetAngle(l_posCenter,l_posNextPos);
		//求偏差的角度
		if(l_fAngleOffset>l_fAngleBG)//没过0度角时
			l_fAngleOffset = l_fAngleOffset - l_fAngleBG;
		else//过零度角时
			l_fAngleOffset = 360 -l_fAngleBG + l_fAngleOffset;
		//画弧
		if(m_iFlagArcOrLine == 2)	//不加这个单击图形时显示不好
			pDC->MoveTo(pt_begin);
		pDC->AngleArc(pt_Center.x,pt_Center.y,(int)l_fRadius,l_fAngleBG,l_fAngleOffset) ;
		//画箭头的角度		//是否显示箭头
		if (FlagShowArrow)
			angle = atan((l_posCenter.x-l_posNextPos.x) /(l_posNextPos.y -l_posCenter.y ));
		m_iFlagArcOrLine = 2;
	}
	//画箭头的角度		//是否显示箭头
	if (FlagShowArrow)
	{
		if (TuDu>0.0) //如果是逆时针时
			angle = angle+MATH_PI;
		if (l_posCenter.y > l_posNextPos.y&&TuDu!=0.0)	//如果是顺时针，的三四象线里。
			angle = angle + MATH_PI;
		DrawArrow(angle,pt_End,pDC);//画箭头的函数 
	}
}
/*
 *	拾取函数
 */
int CCreateRectangle::IsPick(CPoint point)
{
	Position Pos1,Pos2,pos;// 世界坐标点
	double l_dTuDu;//凸度
	g_pView->ScreemToWorld(point,pos);
	//获得缩放比例
	double l_dProportion = g_pView->ReturnProportion();	
	//点集的大小 
	int l_iSizeOfPos = m_point.GetSize()-1;
	for(int i=0;i<l_iSizeOfPos;i++)
	{
		Pos1=m_point.GetAt(i); 
		Pos2=m_point.GetAt(i+1); 
		//如果距离起点或末点的距离满足就算拾取到，相当起末点包围盒外扩
		if (pos.Distance(Pos1)/l_dProportion<=PRECISION||pos.Distance(Pos2)/l_dProportion<=PRECISION)
			return i;
		l_dTuDu = m_dTuDu.GetAt(i);
		if(l_dTuDu == 0.0)//直线
		{
			if(IsPickLine(Pos1,Pos2,point))//如果选中的话
				return i;
		}
		else//圆弧
		{
			if(IsPickArc( Pos1, Pos2, point, l_dTuDu))
				return i;
		}
	}
	return -1;
	
}

/*
 *	直线的拾取方法
 */
BOOL CCreateRectangle::IsPickLine(Position Pos1,Position Pos2,CPoint point)
{
	CRgn	pr;//直线周围的区域
	CPoint pt_pos1,pt_pos2;// 屏幕坐标点
	// 世界坐标转化为屏幕坐标
	g_pView->WorldToScreem(Pos1,pt_pos1); 
	g_pView->WorldToScreem(Pos2,pt_pos2); 
	//把线段的斜率也要算进去，下面是算斜率的。
	double angle = GetAngleToXAxis(Pos1,Pos2);
	double d_Sin = PRECISION * sin(angle);
	double d_Cos = PRECISION * cos(angle);
	
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
		return TRUE;
	}
	pr.DeleteObject();	
	return FALSE;
}

/*
 *	圆弧的拾取方法
 */
BOOL CCreateRectangle::IsPickArc(Position Pos1,Position Pos2,CPoint point,double TuDu)
{
	Position pos(0,0),l_posCenter;
	g_pDoc->polylineToArc(Pos1,Pos2,TuDu,l_posCenter);
	g_pView->ScreemToWorld(point,pos);
	//计算圆弧半径
	double radius = l_posCenter.Distance(Pos1) ;
	//拾取点到圆心的距离
	double distance =  l_posCenter.Distance(pos); 
	//转换的比例
	double l_dProportion = g_pView->ReturnProportion();	
	if(!(  (fabs( radius - distance)/l_dProportion)<=PRECISION)  )
		return  FALSE;
	
	//计算圆弧的起始角
	double angle = GetAngle(l_posCenter, pos);
	double angle1 = GetAngle(l_posCenter, Pos1) ;
	//计算圆弧的终止角
	double angle2 = GetAngle(l_posCenter, Pos2) ;
	
	//拾取点和圆弧中心连线与x轴的夹角应该存在于
	//起始角和终止角范围之外，并且拾取点到圆心的距离
	//和圆弧半径之差的绝对值小于给定值时，返回TRUE
	if (TuDu < 0.0)//如果是顺时针
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
CCreateBase* CCreateRectangle::Copy()
{
	CCreatePolyline *pCopy = new CCreatePolyline(m_point,m_dTuDu);//复制点
	pCopy->m_ColorOfLine = m_ColorOfLine;		//复制线色
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}

//移动
void CCreateRectangle::Move(const Position& basePos,const Position& desPos)
{
	int l_iSize = m_point.GetSize();//点集大小
	Position pt,pt1;
	for (int i=0;i < l_iSize;i++)
	{
		pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Offset(desPos - basePos));	
		pt1 = m_point.ElementAt(i);
	}
}
//旋转
void CCreateRectangle::Rotate(const Position& basePos, const double angle)
{
	for (int i=0;i < m_point.GetSize();i++)
	{
		Position pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Rotate(basePos,angle));	
		Position pt1 = m_point.ElementAt(i);
	}
}
//镜像
void CCreateRectangle::Mirror(const Position& pos1, const Position& pos2)
{
	for (int i=0;i < m_point.GetSize();i++)
	{
		Position pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Mirror(pos1,pos2));	
		Position pt1 = m_point.ElementAt(i);
	}
}

/*
*	属性对话框
*/
void CCreateRectangle::PropertyView()
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
		if(this == (CCreateRectangle *)l_pDelList)//如果与链表里的元素一样，就要记下位置
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
void CCreateRectangle::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	Position pos;
	switch(WhichEdit) //哪个EDIT框改了，就变哪个值
	{
	case 2:
		pos = m_point.GetAt(iNumOfTop-1);
		pos.x = e;
		m_point.SetAt(iNumOfTop-1,pos);
		if(iNumOfTop == 1)
		{
			int l_size = m_point.GetSize();
			pos = m_point.GetAt(l_size-1);
			pos.x = e;
			m_point.SetAt(l_size-1,pos);
		}
		
		break;
	case 3:
		pos = m_point.GetAt(iNumOfTop-1);
		pos.y = e;
		m_point.SetAt(iNumOfTop-1,pos);
		if(iNumOfTop == 1)
		{
			int l_size = m_point.GetSize();
			pos = m_point.GetAt(l_size-1);
			pos.y = e;
			m_point.SetAt(l_size-1,pos);
		}
		
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
BOOL CCreateRectangle::IsSelect(CRgn *selectrect,BOOL bComplex)
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
		if (RectangleOnRect(l_posRgn1,l_posRgn2))
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
BOOL CCreateRectangle::GetBox(BOX2D* pBox,BOOL IsOutside /* = FALSE */)
{
	Position PrePos,CurPos,PrePosMin,PrePosMax,l_posCenter(0,0);//当前点和过去点
	double l_dTudu = 0.0;//凸度
	int l_iDirect;//圆弧方向
	BOX2D l_boxArc;
	PrePosMin=m_point.GetAt(0); 
	PrePosMax=m_point.GetAt(0); 
	//得到包围盒
	for(int i=0;i<m_point.GetSize()-1;i++)
	{
		PrePos = m_point.GetAt(i);
		CurPos=m_point.GetAt(i+1);
		l_dTudu = m_dTuDu.GetAt(i);
		if(l_dTudu==0.0)//如果是直线
		{	//取两点中最小的和最大的
			PrePosMin.x = min( PrePosMin.x, CurPos.x );
			PrePosMin.y = min( PrePosMin.y, CurPos.y );
			PrePosMax.x = max( PrePosMax.x, CurPos.x );
			PrePosMax.y = max( PrePosMax.y, CurPos.y );
		}
		else//圆弧
		{
			g_pDoc->polylineToArc(PrePos,CurPos,l_dTudu,l_posCenter);//求圆弧圆心
			//新创一个圆弧来求多段线中的包围盒
			if(l_dTudu<0.0)l_iDirect = AD_CLOCKWISE;
			else l_iDirect = AD_COUNTERCLOCKWISE;
			CCreateRoundArc *pArc = new CCreateRoundArc(PrePos,l_posCenter,CurPos,l_iDirect);
			pArc->GetBox(&l_boxArc);
			delete pArc;
			//取两点中最小的和最大的
			PrePosMin.x = min( PrePosMin.x, l_boxArc.min[0] );
			PrePosMin.y = min( PrePosMin.y, l_boxArc.min[1] );
			PrePosMax.x = max( PrePosMax.x, l_boxArc.max[0] );
			PrePosMax.y = max( PrePosMax.y, l_boxArc.max[1] );
		}
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
BOOL CCreateRectangle::GraphZoomXY(double dValueX, double dValueY)
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
BOOL CCreateRectangle::RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine /* = 0 */)
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
double CCreateRectangle::RtnLongArc(Position posClick,Position posBegin,Position PosEnd,double dTudu)
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
Position CCreateRectangle::RtnLongArcPos(Position posBegin,Position PosEnd,double dTudu,Position posClick,double dLongPos)
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
void CCreateRectangle::ChangeDir()
{
	Position l_ScreemPos;
	double l_dTude;
	CArray<Position,Position&> l_ListPoint; 
	CArray<double,double&> l_ListTuDu;//存放凸度的
	for(int i=m_point.GetSize();  i>1;  i--)
	{
		l_ScreemPos = m_point.GetAt(i-1); 
		l_ListPoint.Add(l_ScreemPos);
		l_dTude = -m_dTuDu.GetAt(i-2); 
		l_ListTuDu.Add(l_dTude);
	}
	l_ScreemPos = m_point.GetAt(0); 
	l_ListPoint.Add(l_ScreemPos);
	l_dTude = 0.0;
	l_ListTuDu.Add(l_dTude);
	m_point.Copy(l_ListPoint);
	m_dTuDu.Copy(l_ListTuDu);	
}


/*
 *	看多段线是否与矩形有交点并且交点是否在直线上
 */
BOOL CCreateRectangle::RectangleOnRect(Position pos1,Position pos3)
{
	Position pos2(pos1.x,pos3.y),pos4(pos3.x,pos1.y),l_posInter;
	Position l_posBegin,l_posEnd,l_posCenter;
	Position posTemp1,posTemp2;
	//点集的大小 
	int l_iSize = m_point.GetSize()-1,l_iDir;
	//凸度
	double l_dTudu,l_dRadius;
	for(int i=0;i<l_iSize;i++)
	{	
		l_dTudu = m_dTuDu.GetAt(i);
		l_posBegin = m_point.GetAt(i);
		l_posEnd = m_point.GetAt(i+1);
		if (l_dTudu == 0.0)//说明是直线
		{		//先求两条直线的交点										再看交点是否都在线段上
			if (GetIntersectPoint(l_posBegin,l_posEnd,pos1,pos2,l_posInter)&&PosOnLine(pos1,pos2,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
				return TRUE;
			else if (GetIntersectPoint(l_posBegin,l_posEnd,pos2,pos3,l_posInter)&&PosOnLine(pos2,pos3,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
				return TRUE;
			else if (GetIntersectPoint(l_posBegin,l_posEnd,pos3,pos4,l_posInter)&&PosOnLine(pos3,pos4,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
				return TRUE;
			else if (GetIntersectPoint(l_posBegin,l_posEnd,pos4,pos1,l_posInter)&&PosOnLine(pos4,pos1,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
				return TRUE;
		}
		else//说明是圆弧
		{
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);
			l_dRadius = l_posCenter.Distance(l_posBegin);
			l_iDir = RtnArcDir(l_dTudu);
			posTemp1 = pos1;
			posTemp2 = pos2;
			//先求与相同的圆的两个交点	
			if (GetIntersectLineRound(posTemp1,posTemp2,l_posCenter,l_dRadius))//是否在圆上
			{//								再看交点是否都在线段上
				if((PosOnRectangleArc(posTemp1,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos1,pos2,posTemp1))||
					(PosOnRectangleArc(posTemp2,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos1,pos2,posTemp2)))//是否在弧上
					return TRUE;
			}
			posTemp1 = pos2;
			posTemp2 = pos3;
			if (GetIntersectLineRound(posTemp1,posTemp2,l_posCenter,l_dRadius))    //是否在圆上
			{
				if((PosOnRectangleArc(posTemp1,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos3,pos2,posTemp1))||
					(PosOnRectangleArc(posTemp2,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos3,pos2,posTemp2)))//是否在弧上
					return TRUE;
			}
			posTemp1 = pos3;
			posTemp2 = pos4;
			if (GetIntersectLineRound(posTemp1,posTemp2,l_posCenter,l_dRadius))//是否在圆上
			{
				if((PosOnRectangleArc(posTemp1,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos3,pos4,posTemp1))||
					(PosOnRectangleArc(posTemp2,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos3,pos4,posTemp2)))//是否在弧上
					return TRUE;
			}
			posTemp1 = pos4;
			posTemp2 = pos1;
			if (GetIntersectLineRound(posTemp1,posTemp2,l_posCenter,l_dRadius))//是否在圆上
			{
				if((PosOnRectangleArc(posTemp1,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos1,pos4,posTemp1))||
					(PosOnRectangleArc(posTemp2,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos1,pos4,posTemp2)))//是否在弧上
					return TRUE;
			}
		}
	}
	return FALSE;
}

/*
 *	点是否在圆弧上
*/
BOOL CCreateRectangle::PosOnRectangleArc(Position pos,Position posBegin,Position posEnd,Position posCenter,int iDir)
{	
	//计算圆弧的起始角
	double angle = GetAngle(posCenter, pos);
	double angle1 = GetAngle(posCenter, posBegin) ;
	//计算圆弧的终止角
	double angle2 = GetAngle(posCenter, posEnd) ;
	
	if (iDir == AD_CLOCKWISE)//如果是顺时针
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
*	按比例缩放
*/
void CCreateRectangle::GraphZoomScale(Position posBase,double dScale)
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
PGRAPHPOS CCreateRectangle::SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist)
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
			if(m_aryDash.GetSize() > 0)
			{
				m_aryDash.Add(l_posBegin);
				CCreatePolyline* pLine = new CCreatePolyline(m_aryDash);
				POSITION POS = g_pDoc->m_CCreateBaseList.Find(this);
				g_pDoc->m_CCreateBaseList.InsertBefore(POS,pLine);
				m_aryDash.RemoveAll();							
				PGRAPHPOS GraphPosPara = NULL;
				CUndoRedo l_UndoRedo;
				GraphPosPara =  l_UndoRedo.NewGraphPos();
				GraphPosPara->pCreateBasepos = (long)pLine;
				//添加图形
				if (headGraphPos == NULL)
					headGraphPos = GraphPosPara;
				else
					g_pDoc->AddTail(headGraphPos,GraphPosPara);
			}			
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











