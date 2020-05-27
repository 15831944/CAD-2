// DrawLine.cpp: implementation of the CDrawLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "Draw.h"

#include "MainFrm.h"
#include "CAD_PowerLandDoc.h"
#include "CAD_PowerLandView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawLine::CDrawLine()
	:m_Begin(0,0),m_End(0,0)
{
	m_iNumOfClick = 0;
}

CDrawLine::~CDrawLine()
{

}


int CDrawLine::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{		
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//若是空格或回车就要把输入的数据做为点
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num == -1)//输入的是长度
		{		
			if (m_iNumOfClick == 1)
			{
				double l_dLong = atof(cmd);
				double l_dAngle = GetAngleToXAxis(m_middle,l_cPos);
				if (l_dLong>DISTANCE_ZERO)
				{
					l_cPos.x = m_middle.x + l_dLong*cos(l_dAngle);
					l_cPos.y = m_middle.y + l_dLong*sin(l_dAngle);					
				}
				else
					return InvalidContent();//向命令提示框输出“无效的命令”
			}
		}
		else
		{
			if(cmd.FindOneOf("@") != -1)//如果找到@说明是相对位置
			{
				l_cPos.x = m_middle.x +  atof(cmd.Mid(1,l_num));	//豆号前的数字
				l_cPos.y = m_middle.y +  atof(cmd.Mid(l_num+1));	//豆号后的数字				
			}
			else
			{
				l_cPos.x = atof(cmd.Left(l_num));	//豆号前的数字
				l_cPos.y = atof(cmd.Mid(l_num+1));	//豆号后的数字
			}
		}
	}
	CDC *pDC = g_pView->GetDC();
	g_refresh = 0;
	//鼠标点击的次数
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
		case 1:
			m_Begin  = m_middle = l_cPos;
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n输入直线的下一点,或长度,或点鼠标右键完成:");
			else
				RemindContent("\r\nInput point, or length, or the right mouse button to finish:");					
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			break;
		case 2:
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n输入直线的下一点,或长度,或点鼠标右键完成:");
			else
				RemindContent("\r\nInput point, or length, or the right mouse button to finish:");					
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");

			//擦除MOUSEMOVE的最后画的那些
			CCreateBase* l_pEraseLine1 = new CCreateLine(m_middle,m_End);
			l_pEraseLine1->DrawMove(pDC,dmDrag);
			delete l_pEraseLine1;			
			m_End = l_cPos;	
			//画上最后留下的
			CCreateBase* l_pLine1 = new CCreateLine(m_middle,m_End);
			g_pDoc->m_CCreateBaseList.AddTail(l_pLine1);		
			l_pLine1->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
			m_iNumOfClick = 1;
			//把操作步骤添加到撤消链表里
			AddUndoList((LONG)l_pLine1);
			m_middle = l_cPos;		
			break;		
	}
	//把焦点放在命令输入框里
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	
	g_pView->ReleaseDC(pDC);
	return 0;
}

int CDrawLine::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//改变鼠标形状	
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	curPos = pos;
	//对象捕捉
	FindPointAll(pDC,m_middle,curPos,g_pView->m_iSwitch);	
	if (m_iNumOfClick >= 1)
	{	
		// 获得鼠标所在的前一个位置
		prePos = m_End; 
		// 得到设备环境指针
		
		// 创建临时对象擦除上一条橡皮线
		CCreateBase*	pTempLine1 = new CCreateLine(m_middle, prePos);
		if(g_refresh != 1)	//这是为了第一次没有擦除时所做的
			// 当视窗没有被刷新时，重画原来的橡皮线使其被擦除
			pTempLine1->DrawMove(pDC,dmDrag);
		delete pTempLine1;
		//设定捕捉到的点
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);	
		// 创建临时对象，根据当前位置绘制一条橡皮线
		CCreateBase*	pTempLine2 = new CCreateLine(m_middle, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		// 释放设备环境指针		
		// 将当前位置设置为直线终点，以备下一次鼠标移动时用
		m_End = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	return 0;
}

int CDrawLine::OnRButtonDown(UINT nFlags,const Position &pos)
{
	CDC *pDC = g_pView->GetDC();
	//擦除MOUSEMOVE的最后画的那些
	CCreateBase* l_pEraseLine1 = new CCreateLine(m_middle,m_End);
	l_pEraseLine1->DrawMove(pDC,dmDrag);
	delete l_pEraseLine1;
	//结束画图元//把要画的图元删除
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	g_pView->ReleaseDC(pDC);
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n完成:");
	else
		RemindContent("\r\nComplete:");		
	
	CPoint pot;
	g_pView->WorldToScreem(pos,pot);
	CRect rect(pot.x-10,pot.y-10,pot.x+10,pot.y+10);
	g_pView->InvalidateRect(rect);
	
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	return 0;
}

/*
 *	把这个操作添加到撤消链表
 */
void CDrawLine::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}