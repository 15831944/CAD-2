// DrawMeasure.cpp: implementation of the CDrawMeasure class.
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

CDrawMeasure::CDrawMeasure()
{
	
}

CDrawMeasure::~CDrawMeasure()
{
	
}


int CDrawMeasure::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
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
			RemindContent("\r\n输入直线的下一点,或点鼠标右键完成:");
		else
			RemindContent("\r\nInput point, or the right mouse button to finish:");					
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		break;
	case 2:
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n输入直线的下一点,或点鼠标右键完成:");
		else
			RemindContent("\r\nInput point, or the right mouse button to finish:");					
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		//擦除MOUSEMOVE的最后画的那些
		CCreateBase* l_pEraseLine1 = new CCreateLine(m_middle,m_End);
		l_pEraseLine1->DrawMove(pDC,dmDrag);
		delete l_pEraseLine1;
		//画上最后留下的
		m_End = l_cPos;	
		CCreateBase* l_pLine1 = new CCreateLine(m_middle,m_End);
		g_pDoc->m_pMeasureCreateBase = l_pLine1; 		
		l_pLine1->DrawMove(pDC,dmLayer);
		//垂直距离，水平距离，直线长度
		double disVer,disHor,disLong;
		disVer = fabs(m_middle.y - m_End.y);
		disHor = fabs(m_middle.x - m_End.x);
		disLong = m_middle.Distance(m_End);
		CString strVer,strHor,strLong;
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
		{
			strVer.Format("垂直%.3f",disVer);
			strHor.Format("水平%.3f",disHor);
			strLong.Format("直线长%.3f",disLong);	
		}
		else
		{
			strVer.Format("vert%.3f",disVer);
			strHor.Format("Hori%.3f",disHor);
			strLong.Format("Long%.3f",disLong);	
		}
		
		pDC->SetTextColor(RGB(64,128,128));
		pDC->TextOut(20,20,strVer);
		pDC->TextOut(20,40,strHor);
		pDC->TextOut(20,60,strLong);
		//结束画图元//把要画的图元删除
		delete g_pView->m_pDrawBase;
		g_pView->m_pDrawBase = NULL;	//清除掉选择的图元	
		m_middle = l_cPos;		
		break;			
	}
	g_pView->ReleaseDC(pDC);
	return 0;

}

int CDrawMeasure::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//改变鼠标形状	
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	// 得到设备环境指针
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	prePos = m_End; 
	curPos = pos;
	//对象捕捉
	FindPointAll(pDC,m_middle,curPos,g_pView->m_iSwitch);	

	if (m_iNumOfClick >= 1)
	{
		
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

		//垂直距离，水平距离，直线长度
		double disVer,disHor,disLong;
		disVer = fabs(m_middle.y - curPos.y);
		disHor = fabs(m_middle.x - curPos.x);
		disLong = m_middle.Distance(curPos);
		CString strVer,strHor,strLong;
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
		{
			strVer.Format("垂直%.3f",disVer);
			strHor.Format("水平%.3f",disHor);
			strLong.Format("直线长%.3f",disLong);	
		}
		else
		{
			strVer.Format("vert%.3f",disVer);
			strHor.Format("Hori%.3f",disHor);
			strLong.Format("Long%.3f",disLong);	
		}
		pDC->SetTextColor(RGB(64,128,128));
		pDC->TextOut(20,20,strVer);
		pDC->TextOut(20,40,strHor);
		pDC->TextOut(20,60,strLong);
		delete pTempLine2;
		// 释放设备环境指针
		// 将当前位置设置为直线终点，以备下一次鼠标移动时用
		m_End = curPos; 
	}
	return 0;
}

int CDrawMeasure::OnRButtonDown(UINT nFlags,const Position &pos)
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
	return 0;
}

