// CreateSpline.cpp: implementation of the CCreateSpline class.
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
IMPLEMENT_SERIAL(CCreateSpline, CCreateBase, 0)
CCreateSpline::CCreateSpline()
{

}

CCreateSpline::~CCreateSpline()
{

}

CCreateSpline::CCreateSpline(CArray<Position,Position&>&  point,
							 CArray<Position,Position&>& aryControlPoints,CArray<int,int&>& aryNumControl)
{
	m_ColorOfLine = g_CurColor;		//创建时的画色就是当前所选色
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_point.Copy(point);
	int l_NumOfm_point = m_point.GetSize();
	double aaa = 0.0;
	for(int i = 0;i<l_NumOfm_point;i++)
		m_dTuDu.Add(aaa);
	m_aryControlPoints.Copy(aryControlPoints);
	m_aryNumControl.Copy(aryNumControl);
	m_iKnifeStyle = 41;//刀补类型 41左刀补 42右刀补
	m_iIsBeginGraph = 2;//0不是原始图形，1是原始图形，2是保存时不关心
}

/*
 *	画样条曲线
 */
void CCreateSpline::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{
	int		n = GetROP2(pDC->GetSafeHdc()); 	
	CPen	pen; 
	if (DrawMode == dmNormal)		
		pen.CreatePen(PS_SOLID,1,m_ColorOfLine);//如果是正常画图模式，线色就要取决于所选色
	else
		SetDrawEnvir(pDC, DrawMode, &pen);	//调用SetDrawEnvir函数设置绘图环境
	CPen* pOldPen = pDC->SelectObject(&pen); // 得到原来的画笔	

	CArray<POINT,POINT&> l_ArtScreemPos;//屏幕的点集
	Position l_posWord;
	CPoint l_potScreem;
	int l_iSizeOfPos = m_point.GetSize();//多段线共有多少个点
	for(int i=0;i<l_iSizeOfPos;i++)
	{
		l_posWord = m_point.GetAt(i);
		g_pView->WorldToScreem(l_posWord,l_potScreem);
		l_ArtScreemPos.Add(l_potScreem);
		if(i == 0)
			m_potBegin = l_potScreem;
	}
	pDC->Polyline(l_ArtScreemPos.GetData(), l_ArtScreemPos.GetSize());	
	//画方框和箭头
	if (FlagShowArrow)	
	{
		//画起始点的方框
		CRect rect(m_potBegin.x-5,m_potBegin.y-5,m_potBegin.x+5,m_potBegin.y+5);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
		
		int l_iSize = m_aryNumControl.GetSize();
		int l_iControlOnCurve = 0;//控制点在曲线点集中的位置
		Position l_posBegin,l_posEnd;
		for(int i=1;i<l_iSize;i++)
		{
			l_iControlOnCurve = m_aryNumControl.GetAt(i);
			l_posBegin = m_point.GetAt(l_iControlOnCurve-2);
			l_posEnd = m_point.GetAt(l_iControlOnCurve-1);
			g_pView->WorldToScreem(l_posEnd,l_potScreem);		
			double angle = GetAngleToXAxis(l_posBegin,l_posEnd);
			DrawArrow(angle,l_potScreem,pDC);//画箭头的函数 
		}
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
			if(this == (CCreateSpline *)l_pDelList)//如果与链表里的元素一样，就要记下位置
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //这样就可以透明化背景了				
				pDC->TextOut(m_potBegin.x,m_potBegin.y,str);//输出到界面 上
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

CCreateBase* CCreateSpline::Copy()
{
	CCreateSpline *pCopy = new CCreateSpline(m_point,m_aryControlPoints,m_aryNumControl);//复制点
	pCopy->m_ColorOfLine = m_ColorOfLine;		//复制线色
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}

//移动
void CCreateSpline::Move(const Position& basePos,const Position& desPos)
{
	int l_iSize = m_point.GetSize();//点集大小
	Position pt;
	for (int i=0;i < l_iSize;i++)
	{
		pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Offset(desPos - basePos));	
	}	
	l_iSize = m_aryControlPoints.GetSize();//点集大小
	for (int j=0;j < l_iSize;j++)
	{
		pt = m_aryControlPoints.ElementAt(j);
		m_aryControlPoints.SetAt(j,pt.Offset(desPos - basePos));	
	}	
	
}

//旋转
void CCreateSpline::Rotate(const Position& basePos, const double angle)
{
	int l_iSize = m_point.GetSize();//返回多段线的点的个数
	Position pt;
	//把每个点旋转
	for (int i=0;i < l_iSize;i++)
	{
		pt = m_point.ElementAt(i);//从点集里提取每一个点
		m_point.SetAt(i,pt.Rotate(basePos,angle));//旋转完再放回点集里	
	}	
	l_iSize = m_aryControlPoints.GetSize();//返回多段线的点的个数
	//把每个点旋转
	for (int j=0;j < l_iSize;j++)
	{
		pt = m_aryControlPoints.ElementAt(j);//从点集里提取每一个点
		m_aryControlPoints.SetAt(j,pt.Rotate(basePos,angle));//旋转完再放回点集里	
	}	
	
}

//镜像
void CCreateSpline::Mirror(const Position& pos1, const Position& pos2)
{
	int l_iSize = m_point.GetSize();
	Position pt;
	double l_dTudu;
	for (int i=0;i < l_iSize;i++)
	{
		pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Mirror(pos1,pos2));	
		l_dTudu = -(m_dTuDu.ElementAt(i));
		m_dTuDu.SetAt(i,l_dTudu);	
	}
	l_iSize = m_aryControlPoints.GetSize();
	for (int j=0;j < l_iSize;j++)
	{
		pt = m_aryControlPoints.ElementAt(j);
		m_aryControlPoints.SetAt(j,pt.Mirror(pos1,pos2));	
	}
}

/*
 *	改变切割方向
 */
void CCreateSpline::ChangeDir()
{
	Position l_ScreemPos;
	int l_iNumControl;
	CArray<Position,Position&> l_ListPoint; 
	CArray<Position,Position&> l_ListControlPoint; 
	CArray<int,int&> l_ListNumControl;//存放凸度的
	int i=0;
	for(i=m_point.GetSize();  i>0;  i--)
	{
		l_ScreemPos = m_point.GetAt(i-1); 
		l_ListPoint.Add(l_ScreemPos);
	}
	m_point.Copy(l_ListPoint);

	for(i=m_aryControlPoints.GetSize();  i>0;  i--)
	{
		l_ScreemPos = m_aryControlPoints.GetAt(i-1); 
		l_ListControlPoint.Add(l_ScreemPos);
	}
	m_aryControlPoints.Copy(l_ListControlPoint);

	int l_iSize = m_aryNumControl.GetAt(m_aryNumControl.GetSize()-1);
	for(i=m_aryNumControl.GetSize();  i>0;  i--)
	{
		if(i==m_aryNumControl.GetSize()||i==1)
			l_iNumControl = l_iSize - m_aryNumControl.GetAt(i-1); 
		else
			l_iNumControl = l_iSize - m_aryNumControl.GetAt(i-1)+1; 		
		l_ListNumControl.Add(l_iNumControl);
	}
	m_aryNumControl.Copy(l_ListNumControl);
	
}

/*
*	当在特性对话框弹出改变图元特性时要重画的的函数
*/
void CCreateSpline::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	Position pos;
	switch(WhichEdit) //哪个EDIT框改了，就变哪个值
	{
			case 2:
				pos = m_aryControlPoints.GetAt(iNumOfTop-1);
				pos.x = e;
				m_aryControlPoints.SetAt(iNumOfTop-1,pos);
				break;
			case 3:
				pos = m_aryControlPoints.GetAt(iNumOfTop-1);
				pos.y = e;
				m_aryControlPoints.SetAt(iNumOfTop-1,pos);
				break;
			default:
				break;		
	}
	CArray<Position,Position&> l_aryEveryPoint;
	int l_iSize = m_aryControlPoints.GetSize(),i=0,j=0;
	Position l_posControl;
	m_aryNumControl.RemoveAll();
	m_aryNumControl.Add(i);			
	//
	for(j=2;j<=l_iSize;j++)
	{
		l_aryEveryPoint.RemoveAll();
		for(i=0;i<j;i++)
		{
			l_posControl = m_aryControlPoints.GetAt(i);
			l_aryEveryPoint.Add(l_posControl);
		}
		//创建一个样条曲线对象
		Spline spline(l_aryEveryPoint, l_aryEveryPoint.GetSize());
		//根据控制点生成一个样条曲线
		spline.Generate();
		//设定曲线点数				得到控制点数
		m_point.SetSize(spline.GetCurveCount());
		//得到曲线的轮廓点集
		int PointCount = 0;
		spline.GetCurve(m_point, PointCount);
		m_aryNumControl.Add(PointCount);			
		//向图形链表里添加	
	}

	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);	
}

/*
 *	通过控制点刷新样条曲线
 */
void CCreateSpline::RefreshSpline()
{
	CArray<Position,Position&> l_aryEveryPoint;
	int l_iSize = m_aryControlPoints.GetSize(),i=0,j=0;
	Position l_posControl;
	m_aryNumControl.RemoveAll();
	m_aryNumControl.Add(i);			
	//
	for(j=2;j<=l_iSize;j++)
	{
		l_aryEveryPoint.RemoveAll();
		for(i=0;i<j;i++)
		{
			l_posControl = m_aryControlPoints.GetAt(i);
			l_aryEveryPoint.Add(l_posControl);
		}
		//创建一个样条曲线对象
		Spline spline(l_aryEveryPoint, l_aryEveryPoint.GetSize());
		//根据控制点生成一个样条曲线
		spline.Generate();
		//设定曲线点数				得到控制点数
		m_point.SetSize(spline.GetCurveCount());
		//得到曲线的轮廓点集
		int PointCount = 0;
		spline.GetCurve(m_point, PointCount);
		m_aryNumControl.Add(PointCount);			
		//向图形链表里添加	
	}
	int l_NumOfm_point = m_point.GetSize();
	double aaa = 0.0;
	for(int k = 0;k<l_NumOfm_point;k++)
		m_dTuDu.Add(aaa);
	
}








