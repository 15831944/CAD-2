// DrawSelect.cpp: implementation of the CDrawSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "Draw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawSelect::CDrawSelect()
: m_Begin(0,0), m_End(0,0)
{
	m_iNumOfClick = 0; // 初始化操作步为 0
}

CDrawSelect::~CDrawSelect()
{
	
}

int CDrawSelect::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	g_refresh = 0;
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
	case 1:
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		//点击其它按扭时，清除时用
		g_pView->m_PosBegin = m_Begin = l_cPos;
		break;
	case 2:		
		CDC *pDC = g_pView->GetDC();
		//擦除MOUSEMOVE的最后画的那些
		if (m_Begin.x <= m_End.x)
			m_draw2 = dmDragSelect1;
		else
			m_draw2 = dmDragSelect2;
		CCreateRectangle* l_EraseRect = new CCreateRectangle(m_Begin,m_End);
		l_EraseRect->DrawMove(pDC,m_draw2,FALSE,FALSE);
		delete l_EraseRect;
		
		//画上最后留下的
		m_End = l_cPos;
		//建框选的矩形区域
		CPoint pos1,pos2;
		g_pView->WorldToScreem(m_Begin,pos1);
		g_pView->WorldToScreem(m_End,pos2);		
		CRgn l_cSelectRect;
		l_cSelectRect.CreateRectRgn(pos1.x,pos1.y,pos2.x,pos2.y);
		//框选时是不是碰到就算选中
		BOOL l_bComplex = (pos1.x >= pos2.x)?TRUE:FALSE;
		//查看哪个图形被选到了框选里
		POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//获得头指针，在下面循环上用。
		while(position != NULL)//进入扫描链表
		{
			CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
			// 如果链表里的图元被框选中
			if( l_pCreateBase->IsSelect(&l_cSelectRect,l_bComplex))
			{
				int l_iNumCollect = g_pDoc->IsSelected(l_pCreateBase);
				if( l_iNumCollect == -1 )// 判断图元对象是否已经在选择集中,没在选择集中
				{
					l_pCreateBase->DrawMove(pDC,dmSelect);
					g_pDoc->m_CollectOfPicked.Add(l_pCreateBase);  // 将图元放入选择集中
				}
				else//在选择集中
				{
					if (nFlags == 5)//按SHIFT，反选从选择集中移除
					{
						g_pDoc->m_CollectOfPicked.RemoveAt(l_iNumCollect);
						l_pCreateBase->DrawMove(pDC,dmNormal);
					}
				}
			}
		}	
		for(int i = 0;  i<g_pDoc->m_CollectOfPicked.GetSize();i++)
		{
			CCreateBase* l_pCreateBaseList = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
			l_pCreateBaseList->DrawMove(pDC,dmSelect,FALSE,FALSE);
		}
		l_cSelectRect.DeleteObject();
		g_pView->ReleaseDC(pDC);			
		m_iNumOfClick = 0;
		//结束画图元//把要画的图元删除
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;
		g_pView->m_iFlagWhichBtnDown = MEIYOU;
		break;		
	}
	//框选集里只有一个元素
	if(g_pDoc->m_CollectOfPicked.GetSize() == 1)
	{
		CCreateBase* pPropertCreate = (CCreateBase*)g_pDoc->m_CollectOfPicked.GetAt(0);
		pPropertCreate->PropertyView();
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC10,"加工顺序:");					
	}	
	else 
	{
		CCreateBase l_show;
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,"");				
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(5);					
		l_show.ShowControl(0,FALSE);
	}
	return 0;
}

int CDrawSelect::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_NONE));
	//点击其它按扭时，清除时用
	g_pView->m_PosLast = pos;
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	if (m_iNumOfClick == 1)
	{
		Position	prePos, curPos;
		// 获得鼠标所在的前一个位置
		prePos = m_End; 
		curPos = pos;
		// 得到设备环境指针
		CDC*	pDC = g_pView->GetDC(); 
		//框选方式
		if (m_Begin.x <= curPos.x)
			m_draw2 = dmDragSelect1;
		else
			m_draw2 = dmDragSelect2;
		// 创建临时对象擦除上一条橡皮线
		CCreateRectangle*	pTempLine1 = new CCreateRectangle(m_Begin,prePos);
		if(g_refresh != 1)// 当视窗没有被刷新时，重画原来的橡皮线使其被擦除			
			pTempLine1->DrawMove(pDC,m_draw1,FALSE,FALSE);
		delete pTempLine1;
		// 创建临时对象，根据当前位置绘制一条橡皮线
		CCreateRectangle*	pTempLine2 = new CCreateRectangle(m_Begin,curPos);	
		pTempLine2->DrawMove(pDC,m_draw2,FALSE,FALSE);
		delete pTempLine2;

		// 释放设备环境指针
		g_pView->ReleaseDC(pDC); 		
		// 将当前位置设置为直线终点，以备下一次鼠标移动时用
		m_End = curPos; 
		m_draw1 = m_draw2;
	}
	return 0;	
}

int CDrawSelect::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//结束画图元//把要画的图元删除
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	return 0;
}