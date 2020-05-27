// DrawVerticalLine.cpp: implementation of the CDrawVerticalLine class.
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

CDrawVerticalLine::CDrawVerticalLine()
	:m_Begin(0,0),m_End(0,0)
{
	m_aryAngleVertline.RemoveAll();
	m_dLongLine = 0.0;
}

CDrawVerticalLine::~CDrawVerticalLine()
{

}

int CDrawVerticalLine::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{		
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	//如果是第一次输入，并且没有选中被垂直线
	if (m_aryAngleVertline.GetSize() == 0 &&m_iNumOfClick == 0)
	{
		if(!GetAngleVert(l_cPos))//如果点到线上
			return FALSE;
	}		
	if (nFlags == 2&&m_iNumOfClick == 1)	//若是空格或回车就要把输入的数据做为点
	{
		m_dLongLine= atof(cmd);
		if(m_dLongLine < DOUBLE_EQUAL)
			return InvalidContent();//向命令提示框输出“无效的命令”
	}
	CDC *pDC = g_pView->GetDC();
	g_refresh = 0;
	//鼠标点击的次数
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
		case 1:
			m_Begin  = l_cPos;
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n输入直线的长度:");
			else
				RemindContent("\r\nEnter the length of the straight line:");					
			break;
		case 2:
			//擦除MOUSEMOVE的最后画的那些
			CCreateBase* l_pEraseLine1 = new CCreateLine(m_Begin,m_End);
			l_pEraseLine1->DrawMove(pDC,dmDrag);
			delete l_pEraseLine1;	

			//垂线的角度
			if(m_dLongLine != 0.0)
			{
				double l_dAngleVertLine = GetAngleToXAxis(m_Begin,m_End);
				m_End.x = m_Begin.x + m_dLongLine*cos(l_dAngleVertLine);
				m_End.y = m_Begin.y + m_dLongLine*sin(l_dAngleVertLine);			
			}
			//画上最后留下的
			CCreateBase* l_pLine1 = new CCreateLine(m_Begin,m_End);
			g_pDoc->m_CCreateBaseList.AddTail(l_pLine1);		
			l_pLine1->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
			m_iNumOfClick = 0;
			m_aryAngleVertline.RemoveAll();
			//把操作步骤添加到撤消链表里
			AddUndoList((LONG)l_pLine1);
			break;		
	}
	//把焦点放在命令输入框里
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");	
	g_pView->ReleaseDC(pDC);
	return 0;
}

int CDrawVerticalLine::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//改变鼠标形状	
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	curPos = pos;
	//对象捕捉
	FindPointAll(pDC,m_Begin,curPos,g_pView->m_iSwitch);	
	if (m_iNumOfClick >= 1)
	{	
		//求应该垂线的点
		double l_dAngleMouse = GetAngleToXAxis(m_Begin,pos);
		//鼠标与起点的角度与角度集里的最小夹角
		double l_dAngleMin = fabs(m_aryAngleVertline.GetAt(0) - l_dAngleMouse);
		//最后应该画的角度
		double l_dAngleDraw = m_aryAngleVertline.GetAt(0);
		//求最后的应该画的角度
		for(int i=1;i<m_aryAngleVertline.GetSize();i++)
		{
			//夹角
			double l_dAngle = fabs(m_aryAngleVertline.GetAt(i) - l_dAngleMouse);
			if(l_dAngle >=MATH_PI)//如果夹角大于180度
				l_dAngle = MATH_2PI - l_dAngle;
			if(l_dAngleMin > l_dAngle)
			{
				l_dAngleMin = l_dAngle;
				l_dAngleDraw = m_aryAngleVertline.GetAt(i);
			}
		}
		//应该画的线的坐标
		double l_dLongDrawLine = cos(fabs(l_dAngleDraw - l_dAngleMouse))*m_Begin.Distance(pos);
		curPos.x = m_Begin.x + l_dLongDrawLine*cos(l_dAngleDraw);
		curPos.y = m_Begin.y + l_dLongDrawLine*sin(l_dAngleDraw);
		// 获得鼠标所在的前一个位置
		prePos = m_End; 
		// 创建临时对象擦除上一条橡皮线
		CCreateBase*	pTempLine1 = new CCreateLine(m_Begin, prePos);
		if(g_refresh != 1)	//这是为了第一次没有擦除时所做的
			// 当视窗没有被刷新时，重画原来的橡皮线使其被擦除
			pTempLine1->DrawMove(pDC,dmDrag);
		delete pTempLine1;
		//设定捕捉到的点
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);	
		// 创建临时对象，根据当前位置绘制一条橡皮线
		CCreateBase*	pTempLine2 = new CCreateLine(m_Begin, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		// 释放设备环境指针		
		// 将当前位置设置为直线终点，以备下一次鼠标移动时用
		m_End = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	return 0;
}

int CDrawVerticalLine::OnRButtonDown(UINT nFlags,const Position &pos)
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
void CDrawVerticalLine::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}

/*
 *	求点击到的线的垂线的角度
 */
BOOL CDrawVerticalLine::GetAngleVert(Position pos)
{
	//屏幕鼠标的点
	CPoint point;
	g_pView->WorldToScreem(pos,point);
	POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//获得头指针，在下面循环上用。
	CCreateBase* l_pCreateBase = NULL;
	//查看是否选中链表
	while(POS != NULL)//进入扫描链表
	{
		l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);
		// 如果链表里的图元被选中
		int l_iPick = l_pCreateBase->IsPick(point);
		if( l_iPick != -1)
		{
			if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				CCreateLine* pLine = (CCreateLine*)l_pCreateBase;
				//				求垂直该线的角度，并且加到集合里
				AddAngleToAry(GetAngleToXAxis(pLine->m_Begin,pLine->m_End));
			}
			else if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
			{
				CCreateRoundArc* pArc = (CCreateRoundArc*)l_pCreateBase;
				//				求垂直该线的角度，并且加到集合里
				AddAngleToAry(GetAngleToXAxis(pArc->m_Center,pos)+MATH_PI/2);
			}
			else if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{
				CCreateRound* pRound = (CCreateRound*)l_pCreateBase;
				//				求垂直该线的角度，并且加到集合里
				AddAngleToAry(GetAngleToXAxis(pRound->m_Center,pos)+MATH_PI/2);
			}
			else if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{
				CCreatePolyline* pPolyline = (CCreatePolyline*)l_pCreateBase;
				GetPolylineAngleVert(l_iPick,pos,pPolyline->m_point,pPolyline->m_dTuDu);
			}
			else if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{
				CCreateRectangle* pRect = (CCreateRectangle*)l_pCreateBase;
				GetPolylineAngleVert(l_iPick,pos,pRect->m_point,pRect->m_dTuDu);
			}
			else if(l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{
				CCreateEllipse* pEllipse = (CCreateEllipse*)l_pCreateBase;
				GetPolylineAngleVert(l_iPick,pos,pEllipse->m_point,pEllipse->m_dTuDu);
			}
			
		}
	}
	if (m_aryAngleVertline.GetSize() == 0)	//如果没有点到图形
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n没有点击到图形上:",2);
		else
			RemindContent("\r\nNot click on the graphic:",2);						
		return FALSE;
	}			
	return TRUE;
}

/*
 *	向角度集合里加角度
 */
void CDrawVerticalLine::AddAngleToAry(double dAngle)
{
	double l_dAngleVertline;//线的垂角
	//如果线角度在第四象限
	if(dAngle+MATH_PI/2 >= MATH_2PI)
	{
		l_dAngleVertline = dAngle - MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);
		l_dAngleVertline = dAngle-MATH_PI- MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);
	}
	else if(dAngle-MATH_PI/2 <= 0)//如果线角度在第一象限
	{
		l_dAngleVertline = dAngle + MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);
		l_dAngleVertline = dAngle+MATH_PI+ MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);
		
	}
	else
	{
		l_dAngleVertline = dAngle + MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);
		l_dAngleVertline = dAngle - MATH_PI/2;
		m_aryAngleVertline.Add(l_dAngleVertline);		
	}

}

/*
 *	当点到多段线上或矩形或椭圆上时垂直的角度
 */
void CDrawVerticalLine::GetPolylineAngleVert(int iPick,Position posClick,CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	//				求垂直该线的角度，并且加到集合里
	Position pos1,pos2,pos3(0,0);
	double dtudu;
	//如果点到多段线的点上
	int l_iSizePoint = aryPoint.GetSize();
	//是否点到端点上了
	BOOL l_bPickPoint = FALSE;
	for(int i=0;i<l_iSizePoint;i++)
	{
		if(posClick == aryPoint.GetAt(i))//如果点到的端点要有四个
		{
			l_bPickPoint = TRUE;
			break;
		}
	}
	if(l_bPickPoint == TRUE)//如果点击到的是点
	{
		if(i!=0)//垂直于点前的线
		{
			pos1 = aryPoint.GetAt(i-1);
			pos2 = aryPoint.GetAt(i);
			dtudu = aryTudu.GetAt(i-1);
			if(dtudu == 0.0)
				AddAngleToAry(GetAngleToXAxis(pos1,pos2));
			else
			{
				Position l_posCenter;//圆心
				g_pDoc->polylineToArc(pos1,pos2,dtudu,l_posCenter);
				AddAngleToAry(GetAngleToXAxis(l_posCenter,posClick)+MATH_PI/2);
			}			
		}
		//垂直于点后的线
		if(i!= l_iSizePoint-1)
		{
			pos1 = aryPoint.GetAt(i);
			pos2 = aryPoint.GetAt(i+1);
			dtudu = aryTudu.GetAt(i);
			if(dtudu == 0.0)
				AddAngleToAry(GetAngleToXAxis(pos1,pos2));
			else
			{
				Position l_posCenter;//圆心
				g_pDoc->polylineToArc(pos1,pos2,dtudu,l_posCenter);
				AddAngleToAry(GetAngleToXAxis(l_posCenter,posClick)+MATH_PI/2);
			}
		}
	}
	else//否则点到线上
	{
		pos1 = aryPoint.GetAt(iPick);
		pos2 = aryPoint.GetAt(iPick+1);
		dtudu = aryTudu.GetAt(iPick);
		if(dtudu == 0.0)
			AddAngleToAry(GetAngleToXAxis(pos1,pos2));
		else
		{
			Position l_posCenter;//圆心
			g_pDoc->polylineToArc(pos1,pos2,dtudu,l_posCenter);
			AddAngleToAry(GetAngleToXAxis(l_posCenter,posClick)+MATH_PI/2);
		}
	}
}





