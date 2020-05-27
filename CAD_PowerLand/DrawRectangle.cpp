// DrawRectangle.cpp: implementation of the CDrawRectangle class.
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

CDrawRectangle::CDrawRectangle()
	: m_Begin(0,0), m_End(0,0),m_BeginDown(0,0),m_BeginRight(0,0)
{
	m_iNumOfClick = 0; // 初始化操作步为 0
	m_bFlagRoundRect = FALSE;
	m_bSizeRect = FALSE;
	m_dRadius = 0;
	m_dRectWide = 0.0;
	m_dRectLong = 0.0;
}

CDrawRectangle::~CDrawRectangle()
{

}

int CDrawRectangle::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//若是空格或回车就要把输入的数据做为点
	{
		if (cmd.Find(",") == -1&&m_iNumOfClick == 0) //第一次输入看是不是圆角矩形
		{
			if(cmd == "f"||cmd == "F")
			{
				m_bFlagRoundRect = TRUE;
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n指定矩形的圆角半径:");
				else
					RemindContent("\r\nThe radius of the specified rectangle:");
				((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
				return 0;				
			}
			else
			{
				if (m_bFlagRoundRect == FALSE)
					return InvalidContent();//向命令提示框输出“无效的命令”
				m_dRadius = atof(cmd);
				if(m_dRadius == 0.0)
					return InvalidContent();//向命令提示框输出“无效的命令”
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n指定第一个角点:");
				else
					RemindContent("\r\nSpecify the first corner:");
				((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
				return 0;
			}
		}
		else if((m_bFlagRoundRect == FALSE||(m_bFlagRoundRect == TRUE&&m_dRadius != 0.0))&&cmd.Find(",") != -1)
		{
			int l_num = cmd.FindOneOf(",");
			if(l_num == -1)//输入的内容不对
				return InvalidContent();//向命令提示框输出“无效的命令”
			l_cPos.x = atof(cmd.Left(l_num));	//豆号前的数字
			l_cPos.y = atof(cmd.Mid(l_num+1));	//豆号后的数字
		}
		else if(m_iNumOfClick == 1)//当输入的是D尺寸时
		{
			if(cmd == "d"||cmd == "D")
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n指定矩形的长度:");
				else
					RemindContent("\r\nThe long of the specified rectangle:");
				((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
				return 0;				
			}
			else
			{
				if (m_dRectLong == 0.0)
				{
					m_dRectLong = atof(cmd);
					//中文还是英文
					if (g_pDoc->m_stcWorkFor.IsChinese)
						RemindContent("\r\n指定矩形的宽度:");
					else
						RemindContent("\r\nThe wide of the specified rectangle:");
				}
				else
				{
					m_dRectWide = atof(cmd);
					if(m_dRectWide == 0.0)
						return InvalidContent();//向命令提示框输出“无效的命令”
					else
					{
						//中文还是英文
						if (g_pDoc->m_stcWorkFor.IsChinese)
							RemindContent("\r\n点击鼠标选择方向:");
						else
							RemindContent("\r\nClick mouse to sure direction:");						
						m_bSizeRect = TRUE;
					}
				}
				((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");			
				return 0;
			}
			
		}
		else
			return InvalidContent();//向命令提示框输出“无效的命令”		
	}
	g_refresh = 0;
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
	case 1:
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n指定另一个角点[或尺寸D]:");
		else
			RemindContent("\r\nSpecify another corner[or size D]：");			
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_Begin = l_cPos;
		break;
	case 2:

		CDC *pDC = g_pView->GetDC();
		//擦除MOUSEMOVE的最后画的那些
		CCreateRectangle* l_EraseRect = new CCreateRectangle(m_Begin,m_End,m_bFlagRoundRect,m_dRadius);
		l_EraseRect->DrawMove(pDC,dmDrag,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum );
		delete l_EraseRect;

		//如果输入的是长和宽
		if(m_bSizeRect == FALSE)		
			m_End = l_cPos;
		CCreateRectangle* l_pLine = new CCreateRectangle(m_Begin,m_End,m_bFlagRoundRect,m_dRadius);
		g_pDoc->m_CCreateBaseList.AddTail(l_pLine);
		l_pLine->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		//把操作步骤添加到撤消链表里
		AddUndoList((LONG)l_pLine);		
		g_pView->ReleaseDC(pDC);			
		m_iNumOfClick = 0;
		//结束画图元//把要画的图元删除
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;
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

int CDrawRectangle::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//改变鼠标形状	
	// 得到设备环境指针
	CDC*	pDC = g_pView->GetDC(); 
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	//对象捕捉
	if ((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL) 
		g_pView->m_iSwitch = g_pView->m_iSwitch^ORTHOGONAL;
	FindPointAll(pDC,m_Begin,pos,g_pView->m_iSwitch);
	//设定捕捉到的点
	Position l_cPos(pos);
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (m_iNumOfClick == 1)
	{
		if(m_bSizeRect == TRUE)//如果输入的是长和宽
		{
			if (m_Begin.x <= pos.x&& m_Begin.y <= pos.y) 
			{
				l_cPos.x = m_Begin.x + m_dRectLong;
				l_cPos.y = m_Begin.y + m_dRectWide;
			}
			else if (m_Begin.x <= pos.x&& m_Begin.y >= pos.y) 
			{
				l_cPos.x = m_Begin.x + m_dRectLong;
				l_cPos.y = m_Begin.y - m_dRectWide;
			}
			else if (m_Begin.x >= pos.x&& m_Begin.y <= pos.y) 
			{
				l_cPos.x = m_Begin.x - m_dRectLong;
				l_cPos.y = m_Begin.y + m_dRectWide;
			}
			else if (m_Begin.x >= pos.x&& m_Begin.y >= pos.y) 
			{
				l_cPos.x = m_Begin.x - m_dRectLong;
				l_cPos.y = m_Begin.y - m_dRectWide;
			}
		}
		Position	prePos, curPos;
		// 获得鼠标所在的前一个位置
		prePos = m_End; 
		curPos = l_cPos;
		
		// 创建临时对象擦除上一条橡皮线
		CCreateRectangle*	pTempLine1 = new CCreateRectangle(m_Begin,prePos,m_bFlagRoundRect,m_dRadius);
		if(g_refresh != 1)
			// 当视窗没有被刷新时，重画原来的橡皮线使其被擦除
			pTempLine1->DrawMove(pDC,dmDrag,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		delete pTempLine1;
		// 创建临时对象，根据当前位置绘制一条橡皮线
		CCreateRectangle*	pTempLine2 = new CCreateRectangle(m_Begin,curPos,m_bFlagRoundRect,m_dRadius);	
		pTempLine2->DrawMove(pDC,dmDrag,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		delete pTempLine2;
		// 将当前位置设置为直线终点，以备下一次鼠标移动时用
		m_End = curPos; 
	}
	// 释放设备环境指针
	g_pView->ReleaseDC(pDC); 		
	return 0;	
}

int CDrawRectangle::OnRButtonDown(UINT nFlags,const Position &pos)
{

	return 0;
}


/*
 *	把这个操作添加到撤消链表
 */
void CDrawRectangle::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}