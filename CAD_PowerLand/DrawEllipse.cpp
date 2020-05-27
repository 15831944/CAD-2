// DrawEllipse.cpp: implementation of the CDrawEllipse class.
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

CDrawEllipse::CDrawEllipse()
	:m_Begin(0,0),m_End(0,0),m_Second(0,0)
{
	m_iNumOfClick = 0;
}

CDrawEllipse::~CDrawEllipse()
{

}


int CDrawEllipse::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//若是空格或回车就要把输入的数据做为点
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num == -1)//输入的内容不对
			return InvalidContent();
		l_cPos.x = atof(cmd.Left(l_num));	//豆号前的数字
		l_cPos.y = atof(cmd.Mid(l_num+1));	//豆号后的数字
	}

	g_refresh = 0;
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
	case 1:
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n指定轴的另一个端点:");
		else
			RemindContent("\r\nAnother endpoint of the specified axis：");			
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_Begin = l_cPos;
		break;
	case 2:
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n指定一点到椭圆心的距离为另一条半轴长度:");
		else
			RemindContent("\r\nSpecify a bit to the elliptical heart another half shaft length：");			
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_Second = l_cPos;
		break;
	case 3:
		CDC *pDC = g_pView->GetDC();

		// 擦除上一条橡皮线
		CCreateEllipse*	l_pEllipse = new CCreateEllipse(m_Begin,m_Second, m_End);	
		l_pEllipse->DrawMove(pDC,dmDrag);
		delete l_pEllipse;

		m_End = l_cPos;
		CCreateEllipse* l_pLine = new CCreateEllipse(m_Begin,m_Second,m_End);
		g_pDoc->m_CCreateBaseList.AddTail(l_pLine);
		l_pLine->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
		//把操作步骤添加到撤消链表里
		AddUndoList((LONG)l_pLine);		
		g_pView->ReleaseDC(pDC);			
		m_iNumOfClick = 0;
		//结束画图元//把要画的图元删除complete
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

int CDrawEllipse::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//改变鼠标形状	
	// 得到设备环境指针
	CDC*	pDC = g_pView->GetDC(); 
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	if (m_iNumOfClick == 2)
	{
		Position	prePos, curPos;
		// 获得鼠标所在的前一个位置
		prePos = m_End; 
		curPos = pos;
		
		// 创建临时对象擦除上一条橡皮线
		CCreateEllipse*	pTempLine1 = new CCreateEllipse(m_Begin,m_Second, prePos);
		if(g_refresh != 1)
			// 当视窗没有被刷新时，重画原来的橡皮线使其被擦除
			pTempLine1->DrawMove(pDC,dmDrag);
		delete pTempLine1;
		// 创建临时对象，根据当前位置绘制一条橡皮线
		CCreateEllipse*	pTempLine2 = new CCreateEllipse(m_Begin,m_Second, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		// 将当前位置设置为直线终点，以备下一次鼠标移动时用
		m_End = curPos; 
	}
	//对象捕捉
	FindPointAll(pDC,m_Second,pos,g_pView->m_iSwitch);
	// 释放设备环境指针
	g_pView->ReleaseDC(pDC); 		
	return 0;
}

int CDrawEllipse::OnRButtonDown(UINT nFlags,const Position &pos)
{

	return 0;
}


/*
 *	把这个操作添加到撤消链表
 */
void CDrawEllipse::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}