// DrawRoundArc.cpp: implementation of the CDrawRoundArc class.
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

CDrawRoundArc::CDrawRoundArc()
	:m_Begin(0,0),m_End(0,0),m_Center(0,0)
{
	m_iNumOfClick = 0;
	m_iDirect = AD_CLOCKWISE;
}

CDrawRoundArc::~CDrawRoundArc()
{

}

int CDrawRoundArc::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	CDC*	pDC = g_pView->GetDC(); 
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
				RemindContent("\r\n指定圆弧的圆心:");
			else
				RemindContent("\r\nSpecifies the center of the circle arc：");			
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			return 0;
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
			RemindContent("\r\n指定圆弧的起点:");
		else
			RemindContent("\r\nSpecify the starting point of the arc：");			
		
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_Center = l_cPos;			
			break;
	case 2:
		{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n指定圆弧的终点:");
		else
			RemindContent("\r\nSpecify the end of the arc：");			
		
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		// 擦除在拖动状态时显示的橡皮线
		CCreateLine*	pTempLine2 = new CCreateLine(m_Center, m_Begin);
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		
		m_Begin = l_cPos;
		break;
		}
	case 3:
		{
		// 擦除在拖动状态时显示的橡皮线
		CCreateLine*	pTempLine2 = new CCreateLine(m_Center, m_End);
		CCreateRoundArc*	pTempArc = new CCreateRoundArc(m_Begin,m_Center, m_End,m_iDirect);
		pTempLine2->DrawMove(pDC,dmDrag);
		pTempArc->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		delete pTempArc;
		// 终点为鼠标单击位置
		m_End = l_cPos ;
		if(fabs(l_cPos.y - m_Begin.y)<DISTANCE_ZERO)//如果起点和末点一样就不画
		{
			g_pView->ReleaseDC(pDC); 		
			return 0;
		}		
		CCreateRoundArc*	pNewArc = new CCreateRoundArc(m_Begin,m_Center, m_End,m_iDirect);			
		// 将圆弧指针添加到图元链表
		g_pDoc->m_CCreateBaseList.AddTail(pNewArc);
		pNewArc->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		//把操作步骤添加到撤消链表里
		AddUndoList((LONG)pNewArc);		
		// 释放设备环境指针
		// 将操作步重置为 0
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
	}
	//把焦点放在命令输入框里
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	g_pView->ReleaseDC(pDC); 
	return 0;
}

int CDrawRoundArc::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//改变鼠标形状	
	CDC*	pDC = g_pView->GetDC(); 
	//对象捕捉
	FindPointAll(pDC,m_Center,pos,g_pView->m_iSwitch);
	//设定捕捉到的点
	Position l_cPos(pos);
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	switch(m_iNumOfClick)		
	{
	case 1:
		{
			Position	prePos, curPos;
			prePos = m_Begin; 		
			curPos = l_cPos;
			//擦除
			CCreateLine* pTempLine1= new CCreateLine(m_Center,prePos);
			if(g_refresh != 1)
				pTempLine1->DrawMove(pDC,dmDrag);
			delete pTempLine1;
			//画图形
			CCreateLine*  pTempLine2 = new CCreateLine(m_Center,curPos);
			pTempLine2->DrawMove(pDC,dmDrag);
			delete pTempLine2;			
			m_Begin = curPos; 
			break;
		}
		
	case 2:
		{
		Position	prePos, curPos;
		prePos = m_End; 		
		curPos = l_cPos;
		//擦除
		CCreateLine* pTempLine1= new CCreateLine(m_Center,prePos);
		CCreateRoundArc* ptemArc1 = new CCreateRoundArc(m_Begin,m_Center,prePos,m_iDirect);
		if(g_refresh != 1)
		{
			ptemArc1->DrawMove(pDC,dmDrag);
			pTempLine1->DrawMove(pDC,dmDrag);
		}
		delete pTempLine1;
		delete ptemArc1;
		//画图形
		CCreateLine*  pTempLine2 = new CCreateLine(m_Center,curPos);
		CCreateRoundArc*	ptemArc2 = new CCreateRoundArc(m_Begin,m_Center,curPos,m_iDirect);	
		pTempLine2->DrawMove(pDC,dmDrag);
		ptemArc2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		delete ptemArc2;

		m_End = curPos; 
		break;
		}
	}
	g_pView->ReleaseDC(pDC); 		
	return 0;
}

int CDrawRoundArc::OnRButtonDown(UINT nFlags,const Position &pos)
{

	return 0;
}


/*
 *	把这个操作添加到撤消链表
 */
void CDrawRoundArc::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}