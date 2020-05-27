// CreatePoint.cpp: implementation of the CCreatePoint class.
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
IMPLEMENT_SERIAL(CCreatePoint, CCreateBase, 0)
CCreatePoint::CCreatePoint()
{

}

CCreatePoint::CCreatePoint(Position pot)
{
	m_Begin = pot;
	m_ColorOfLine = g_CurColor;		//创建时的画色就是当前所选色
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_iKnifeStyle = 41;//刀补类型 41左刀补 42右刀补
	m_iIsBeginGraph = 2;//0不是原始图形，1是原始图形，2是保存时不关心
}

CCreatePoint::~CCreatePoint()
{

}

/*
 *	画点
 */
void CCreatePoint::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow /* = FALSE */,BOOL FlagShowNum /* = FALSE */)
{
	CPoint pt_begin; // 屏幕坐标
	// 将世界坐标转化为屏幕坐标
	g_pView->WorldToScreem(m_Begin,pt_begin); 

//////////////////////////////////////////////////////////////////////////
	// 得到原来的绘图模式
	int		n = GetROP2(pDC->GetSafeHdc()); 
	// 创建画笔的原则：
	// 如果在正常的绘图模式下，使用成员变量创建画笔
	// 如果是其它的模式,使用全局函数"SetDrawEnvir"创建画笔
	CPen	pen; 
	int			lineStyle, lineWidth;
	COLORREF	color;
	switch(DrawMode) 
	{
	case dmSelect: // 选中状态
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_SOLID;
			lineWidth = 1;
			color = m_ColorOfLine;//RGB(0,0,0);
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
	case dmPrompt: // 提示状态
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_DOT;
			lineWidth = 1;
			color = m_ColorOfLine;
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
	pen.CreatePen(lineStyle,lineWidth,color) ;

	CPen* pOldPen = pDC->SelectObject(&pen); // 得到原来的画笔
//////////////////////////////////////////////////////////////////////////
	if (DrawMode == dmNormal)
	{
		pDC->SetPixel(pt_begin,m_ColorOfLine);	// 根据屏幕坐标绘制图元
	}
	else	
	{		
		CRect rect(pt_begin.x-5,pt_begin.y-5,pt_begin.x+5,pt_begin.y+5);		
		pDC->SelectStockObject(GRAY_BRUSH);	
		pDC->Rectangle(rect);
		FlagShowArrow = TRUE;
	}
	if (FlagShowArrow)	//画方框和箭头
	{
		//画起始点的方框
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
			if(this == (CCreatePoint *)l_pDelList)//如果与链表里的元素一样，就要记下位置
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				//pDC->SetBkMode(TRANSPARENT);  //这样就可以透明化背景了
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

/*
 *	单击是否选中
 */
int CCreatePoint::IsPick(CPoint point)
{
	Position l_posPick;
	g_pView->ScreemToWorld(point,l_posPick);
	if(l_posPick.Distance(m_Begin)/g_pView->ReturnProportion()<=PRECISION)
		return TRUE;
	return -1;
}


CCreateBase* CCreatePoint::Copy()
{
	CCreatePoint *pCopy = new CCreatePoint(m_Begin);	//复制点
	pCopy->m_ColorOfLine = m_ColorOfLine;		//复制线色
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}


void CCreatePoint::Move(const Position& basePos,const Position& desPos)
{
	m_Begin = m_Begin.Offset(desPos - basePos);
}

void CCreatePoint::Mirror(const Position& pos1, const Position& pos2)
{
	m_Begin = m_Begin.Mirror(pos1, pos2) ;
}

void CCreatePoint::Rotate(const Position& basePos, const double angle)
{
	m_Begin = m_Begin.Rotate(basePos, angle) ;
}





BOOL CCreatePoint::IsSelect(CRgn *selectrect,BOOL bComplex)
{
	CPoint pos1;	
	//转成屏幕坐标
	g_pView->WorldToScreem(m_Begin,pos1);
	//看点是否属于区域
	BOOL l_bIsSelect= selectrect->PtInRegion(pos1);
	return l_bIsSelect;
}

void CCreatePoint::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	switch(WhichEdit) //哪个EDIT框改了，就变哪个值
	{
	case 1:
		m_Begin.x = e;
		break;
	case 2:
		m_Begin.y = e;
		break;
	default:
		break;
	}
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);	
}

void CCreatePoint::PropertyView()
{
	//显示属性对话框
	(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 
	
	//把隐藏的STATIC和Edit显示出来
	ShowControl(2,FALSE);
	//设定要显示的颜色
	
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(RtnColorInt()-1);
	//设置STATIC要显示“起点和终点”
	//设置Edit要显示起点和终点的位置
	if (g_pDoc->m_stcWorkFor.IsChinese)
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"点X坐标");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"点Y坐标");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Y");
	}
	CString str;
	str.Format("%.4f",m_Begin.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
	str.Format("%.4f",m_Begin.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
	////////////////////////遍历链表里的看自己排在第几位，刀顺时用///////////////////////////////////
	POSITION pos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	for (int i=0;pos != NULL;i++)//遍历链表，i可表示链表里的排序号
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(pos);
		//如果是M代码
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(this == (CCreatePoint *)l_pDelList)//如果与链表里的元素一样，就要记下位置
		{
			str.Format("%d",i);
			((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,str);
		}
	}
}

//得到包围盒
BOOL CCreatePoint::GetBox(BOX2D *pBox,BOOL IsOutside /* = FALSE */)
{
	double curRadius = g_pView->ReturnProportion();
	//如果没有外括
	if (IsOutside == FALSE)
		curRadius = 0.0;
	pBox->min[0] = m_Begin.x - curRadius;
	pBox->min[1] = m_Begin.y - curRadius;
	pBox->max[0] = m_Begin.x + curRadius;
	pBox->max[1] = m_Begin.y + curRadius;
	return TRUE;
	return FALSE;
}











