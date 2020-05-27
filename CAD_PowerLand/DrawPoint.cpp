// DrawPoint.cpp: implementation of the CDrawPoint class.
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

CDrawPoint::CDrawPoint()
{

}

CDrawPoint::~CDrawPoint()
{

}

int CDrawPoint::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	CDC *pDC = g_pView->GetDC();
	CCreatePoint* l_pLine1;


	if (nFlags == 2)
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num != -1)
		{
			l_cPos.x = atof(cmd.Left(l_num));	//豆号前的数字
			l_cPos.y = atof(cmd.Mid(l_num+1));	//豆号后的数字
		}
		else
		{
			double l_dLongLine = atof(cmd);
			int l_iInvali;
			m_pCreateBase->RtnLongPos(m_posRight,l_dLongLine,l_cPos,l_iInvali);
		}
	}
	l_pLine1 = new CCreatePoint(l_cPos);
	g_pDoc->m_CCreateBaseList.AddTail(l_pLine1);		
	l_pLine1->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
	g_pView->ReleaseDC(pDC); 
	//把操作步骤添加到撤消链表里
	AddUndoList((LONG)l_pLine1);
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	//把焦点放在命令输入框里
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	
	return 0;
}

int CDrawPoint::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//改变鼠标形状	
	CDC*	pDC = g_pView->GetDC(); 
	//对象捕捉
	FindPointAll(pDC,pos,pos,g_pView->m_iSwitch);	
	g_pView->ReleaseDC(pDC); 
	return 0;
}



int CDrawPoint::OnRButtonDown(UINT nFlags,const Position &pos)
{
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	
	//屏幕鼠标的点
	CPoint point;
	g_pView->WorldToScreem(l_cPos,point);
	POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//获得头指针，在下面循环上用。
	while(POS != NULL)//进入扫描链表
	{
		m_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);
		// 如果链表里的图元被选中
		if( m_pCreateBase->IsPick(point) != -1)
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n输入距离此点的长度或按ESC键取消");
			else
				RemindContent("\r\nEnter the length of the distance from this point or press the ESC key to cancel：");			
			
			m_posRight = l_cPos;
			//把焦点放在命令输入框里
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
			break;
		}
	}
	return 0;
}

void CDrawPoint::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}