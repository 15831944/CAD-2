// DrawRound.cpp: implementation of the CDrawRound class.
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

CDrawRound::CDrawRound()
	:m_Center(0,0),m_EndPos(0,0)
{
	m_iNumOfClick = 0;
	m_iDirect = AD_CLOCKWISE;
}

CDrawRound::~CDrawRound()
{

}

int CDrawRound::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//若是空格或回车就要把输入的数据做为点
	{
		if (cmd.Find(",") == -1&&m_iNumOfClick == 0) //第一次输入方向时进入下面
		{
			if(cmd == "s"||cmd == "S")
				m_iDirect = AD_CLOCKWISE;
			else if (cmd == "n"||cmd == "N")
				m_iDirect = AD_COUNTERCLOCKWISE;
			else
				return InvalidContent();//向命令提示框输出“无效的命令”
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n指定圆的圆心:");
			else
				RemindContent("\r\nSpecifies the center of the circle：");			
			
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			return 0;
		}
		else if (cmd.Find(",") == -1&&m_iNumOfClick == 1) //第二次输入半径
		{
			double l_dRadius = atof(cmd);
			l_cPos.y = m_Center.y;
			l_cPos.x = m_Center.x + l_dRadius;
		}
		else	//否则进入点里
		{
			int l_num = cmd.FindOneOf(",");
			if(l_num == -1)//输入的内容不对
				return InvalidContent();//向命令提示框输出“无效的命令”
			l_cPos.x = atof(cmd.Left(l_num));	//豆号前的数字
			l_cPos.y = atof(cmd.Mid(l_num+1));	//豆号后的数字
		}
	}
	
	m_iNumOfClick++;
	g_refresh = 0;
	switch(m_iNumOfClick)
	{
	case 1:
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n指定半径或点击圆周的任意一点:");
		else
			RemindContent("\r\nSpecify the radius or circumference of a circle at any point：");			
		
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_Center = l_cPos;
		break;
	case 2:
		
		CDC *pDC = g_pView->GetDC();
		//擦除MOUSEMOVE的最后画的那些
		CCreateRound* l_pEraseRound = new CCreateRound(m_Center,m_EndPos,m_iDirect);
		CCreateLine* l_pEraseLine = new CCreateLine(m_Center,m_EndPos);
		l_pEraseRound->DrawMove(pDC,dmDrag);
		l_pEraseLine->DrawMove(pDC,dmDrag);
		delete l_pEraseRound;
		delete l_pEraseLine;

		m_EndPos = l_cPos;
		CCreateRound* l_pLine = new CCreateRound(m_Center,l_cPos,m_iDirect);
		g_pDoc->m_CCreateBaseList.AddTail(l_pLine);
		l_pLine->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		//把操作步骤添加到撤消链表里
		AddUndoList((LONG)l_pLine);		
		g_pView->ReleaseDC(pDC);			
		m_iNumOfClick = 0;
		//结束画图元//把要画的图元删除
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;	//清除掉选择的图元
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n完成:");
		else
			RemindContent("\r\nComplete:");		
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		break;
	}
	//把焦点放在命令输入框里
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	return 0;
}

int CDrawRound::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//改变鼠标形状	
	CDC*	pDC = g_pView->GetDC(); 
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	Position	prePos, curPos;
	prePos = m_EndPos; 
	curPos = pos;
	//对象捕捉
	FindPointAll(pDC,m_Center,curPos,g_pView->m_iSwitch);
	if (m_iNumOfClick == 1)
	{
		
		CCreateLine* ptem = new CCreateLine(m_Center,prePos);
		CCreateRound*	pTempLine1 = new CCreateRound(m_Center, prePos,m_iDirect);
		if(g_refresh != 1)
		{
			ptem->DrawMove(pDC,dmDrag);
			pTempLine1->DrawMove(pDC,dmDrag);
		}
		delete pTempLine1;
		delete ptem;
		//设定捕捉到的点
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);
		
		CCreateLine* ptem2 = new CCreateLine(m_Center,curPos);
		CCreateRound*	pTempLine2 = new CCreateRound(m_Center, curPos,m_iDirect);	
		pTempLine2->DrawMove(pDC,dmDrag);
		ptem2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		delete ptem2;
		m_EndPos = curPos; 
	}
	g_pView->ReleaseDC(pDC); 		
	return 0;
}

int CDrawRound::OnRButtonDown(UINT nFlags,const Position &pos)
{

	return 0;
}


/*
 *	把这个操作添加到撤消链表
 */
void CDrawRound::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}