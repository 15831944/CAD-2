// DrawZoomScale1.cpp: implementation of the CDrawZoomScale class.
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

CDrawZoomScale::CDrawZoomScale()
{
	m_bBase =FALSE;
}

CDrawZoomScale::~CDrawZoomScale()
{

}

int CDrawZoomScale::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//若是空格或回车就要把输入的数据做为点
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num == -1)//输入的内容是比例因子
		{		
			if(m_bBase == FALSE)
				return InvalidContent();//向命令提示框输出“无效的命令”			
			double l_dLong = atof(cmd);
			if (l_dLong>DISTANCE_ZERO)
				return OnZoomScale(l_dLong);
			else
				return InvalidContent();//向命令提示框输出“无效的命令”
		}
		else
		{
			if(m_bBase == FALSE)//如果没有基点
			{
				l_cPos.x = atof(cmd.Left(l_num));	//豆号前的数字
				l_cPos.y = atof(cmd.Mid(l_num+1));	//豆号后的数字
			}
			else
				return InvalidContent();//向命令提示框输出“无效的命令”			
		}
	}
	m_posBase = l_cPos;
	m_bBase = TRUE;
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n指定比例因子:");
	else
		RemindContent("\r\nSpecifies the scaling factor:");					
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	//把焦点放在命令输入框里
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	
	return 0;
}

int CDrawZoomScale::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//改变鼠标形状	
	CDC* pDC = g_pView->GetDC(); 
	FindPointAll(pDC,pos,pos,g_pView->m_iSwitch);		
	g_pView->ReleaseDC(pDC); 
	return 0;
}



int CDrawZoomScale::OnRButtonDown(UINT nFlags,const Position &pos)
{

	return 0;
}

/*
 *	比例缩放
 */
int CDrawZoomScale::OnZoomScale(double dScale)
{
	CDC* pDC = g_pView->GetDC(); 
	//选择集的大小
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	CCreateBase*	pSelEntity;
	//向撤消链表里添加操作
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_SCALE;
	
	pUndoList->pOperatieParam = l_UndoRedo.NewOperateParam();
	g_pDoc->PosBeEqualed(pUndoList->pOperatieParam->potBase,m_posBase);
	pUndoList->pOperatieParam->angle = dScale;
	
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;
	for( int i = 0 ; i < l_iSize ; i++ )
	{
		pSelEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i] ;
		pSelEntity->GraphZoomScale(m_posBase,dScale);
		if (CurGraphPos->pCreateBasepos == NULL)
			CurGraphPos->pCreateBasepos = (long)pSelEntity;
		else
		{
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (long)pSelEntity;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		} 
		
	}
	//添加撤消链表
	g_pDoc->m_Undo->AddUndoList(pUndoList);
	g_pDoc->RemoveAllCollected();
	//结束画图元//把要画的图元删除
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;	
	g_pView->ReleaseDC(pDC); 
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);
	return 1;
}

