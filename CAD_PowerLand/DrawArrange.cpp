// Arrange.cpp: implementation of the CArrange class.
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

CDrawArrange::CDrawArrange()
{
	m_PosOfArrange = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	g_pDoc->m_CCreateBaseList.GetNext(m_PosOfArrange);
	while (m_PosOfArrange != NULL)//遍历链表，i可表示链表里的排序号
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(m_PosOfArrange);
		if (!l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))//看前面有多少个M代码
			break;
	}
	if (m_PosOfArrange == NULL)
	{
		//取消排序的功能
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;
	}
	else
		g_pDoc->m_CCreateBaseList.GetPrev(m_PosOfArrange);	
}

CDrawArrange::~CDrawArrange()
{

}

int CDrawArrange::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	CPoint l_ptScreen;
	BOOL l_bFlagClick = FALSE; //标志是否点中
	POSITION l_posPre;	//放前一图元的
	g_pView->WorldToScreem(pos,l_ptScreen);	//转成屏幕坐标
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//获得头指针，在下面循环上用。
	//查看哪个图形被单选中
	while(position != NULL)//进入扫描链表
	{
		l_posPre = position;
		CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
		// 如果链表里的图元被单选选中
		if( l_pCreateBase->IsPick(l_ptScreen) != -1)
		{
			//当点到最后一点时要退出了
			if(m_PosOfArrange == g_pDoc->m_CCreateBaseList.GetTailPosition())
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n排序完成！");
				else
					RemindContent("\r\nArrange to complete！");
				//取消排序的功能
				delete g_pView->m_pDrawBase;
				g_pView->m_pDrawBase = NULL;
				return 0;
			}
			if (l_posPre == m_PosOfArrange)//如果点中的是插入点就要向后排一下
				g_pDoc->m_CCreateBaseList.GetNext(m_PosOfArrange);
			//向后插入
			position = l_posPre;
			g_pDoc->m_CCreateBaseList.GetPrev(position);
			g_pDoc->m_CCreateBaseList.RemoveAt(l_posPre);
			AddUndoList(position,(LONG)l_pCreateBase);
			g_pDoc->m_CCreateBaseList.InsertBefore(m_PosOfArrange,l_pCreateBase);
			g_pView->Invalidate();
			l_bFlagClick = TRUE;//标志点中
			break;
		}
	}
	if (l_bFlagClick == TRUE)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点中：");
		else
			RemindContent("\r\nClicked：");	
	}		
	else
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n没有点中：");
		else
			RemindContent("\r\nNo clicked：");	
	}
	return 0;
}

/*
 *	添加撤消链表
 */
void CDrawArrange::AddUndoList(POSITION pos,LONG pCreateBase)
{
	//向撤消链表里添加操作
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_RE_ARRANG;

	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	long pGraph = (long)g_pDoc->m_CCreateBaseList.GetAt(pos);
	pUndoList->pGraphPos->pCreateBaseposSpare1 = pGraph;
	pGraph = (long)g_pDoc->m_CCreateBaseList.GetAt(m_PosOfArrange);
	pUndoList->pGraphPos->pCreateBaseposSpare2 = pGraph;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}

int CDrawArrange::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	return 0;	
}

int CDrawArrange::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n排序完成！");
	else
		RemindContent("\r\nArrange to complete！");
	//取消排序的功能
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	return 0;
}

