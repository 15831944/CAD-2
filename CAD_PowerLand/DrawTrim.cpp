// DrawTrim.cpp: implementation of the CDrawTrim class.
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

CDrawTrim::CDrawTrim()
{

}

CDrawTrim::~CDrawTrim()
{

}


int CDrawTrim::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	CDC* pDC = g_pView->GetDC();
	CPoint l_pot;
	g_pView->WorldToScreem(l_cPos,l_pot);
	//选中图形的哪个线
	int l_iPick=0;
	//是否选中的标志
	BOOL	l_bPicked = FALSE;
	CCreateBase*	l_pCreateBase = NULL;
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//获得头指针，在下面循环上用。
	while(position != NULL)//进入扫描链表
	{
		l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
		// 如果链表里的图元被选中
		l_iPick = l_pCreateBase->IsPick(l_pot);
		if( l_iPick != -1 )
		{ 
			l_bPicked = TRUE;
			break;// 当某一个图元被拾取到后，即退出循环
		}
	}	
	//如果选中
	if(l_bPicked == TRUE)
	{
		l_pCreateBase->DrawMove(pDC,dmSelect);
		m_iNumOfClick++;
		if (m_iNumOfClick == 1)//鼠标点击的第一点
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n已选中第一条线，再选第二条");	
			else
				RemindContent("\r\nSelect the first line, and then choose the second：");	
			
			m_createbaseFirst = l_pCreateBase;
			g_pDoc->m_CollectOfPicked.Add(m_createbaseFirst);  // 将图元放入选择集中
			m_iPick1 = l_iPick;//点击到图形上的哪个线断
			m_posFirst = l_cPos;
		}
		if (m_iNumOfClick == 2)//鼠标点击的第二点
		{
			m_createbaseSecond = l_pCreateBase;
			//如果点击的是两个图形
			if (m_createbaseFirst == m_createbaseSecond)
			{
				//清空选择集
				g_pDoc->RemoveAllCollected();
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n点到的是一个图形，无效！重新选择第一条线：");			
				else
					RemindContent("\r\nPoint to the one graphics, invalid! To choose the first line：");	
				m_iNumOfClick = 1;	
				return FALSE;
			}
			m_posSecond =  l_cPos;
			m_iPick2 = l_iPick;//点击到图形上的哪个线断
			//把线修剪
			DoTrimGraph();
			//清空选择集
			g_pDoc->RemoveAllCollected();
			m_iNumOfClick = 0;			
		}
	}
	else
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n没有选中，重新选择",SEND_RICHEDIT);
		else
			RemindContent("\r\nNot selected, choose again：",SEND_RICHEDIT);	
		return FALSE;
	}
	g_pView->Invalidate();
	g_pView->ReleaseDC(pDC);
	return 0;	
}

int CDrawTrim::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	curPos = pos;
	//对象捕捉
	FindPointAll(pDC,m_posFirst,curPos,g_pView->m_iSwitch);	
	if (m_iNumOfClick == 1)
	{	
		// 获得鼠标所在的前一个位置
		prePos = m_posSecond; 
		// 得到设备环境指针
		
		// 创建临时对象擦除上一条橡皮线
		CCreateBase*	pTempLine1 = new CCreateLine(m_posFirst, prePos);
		if(g_refresh != 1)	//这是为了第一次没有擦除时所做的
			// 当视窗没有被刷新时，重画原来的橡皮线使其被擦除
			pTempLine1->DrawMove(pDC,dmDrag);
		delete pTempLine1;
		//设定捕捉到的点
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);	
		// 创建临时对象，根据当前位置绘制一条橡皮线
		CCreateBase*	pTempLine2 = new CCreateLine(m_posFirst, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		// 释放设备环境指针		
		// 将当前位置设置为直线终点，以备下一次鼠标移动时用
		m_posSecond = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	return 0;
}



int CDrawTrim::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//结束画图元//把要画的图元删除
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;	//清除掉选择的图元
	return 0;
}

/*
 *	把线修剪
 */
void CDrawTrim::DoTrimGraph()
{
	if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{	
		AddUndoList();
		TrimLine();
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
	{
		AddUndoList();
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_createbaseFirst;	
		TrimPolyline(pPolyline->m_point,pPolyline->m_dTuDu);
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		AddUndoList();
		CCreateRectangle* pPolyline = (CCreateRectangle*)m_createbaseFirst;	
		TrimPolyline(pPolyline->m_point,pPolyline->m_dTuDu);
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{
		AddUndoList();
		TrimAarc();
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{
		AddUndoList();
		TrimRound();
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		AddUndoList();
		CCreateEllipse* pPolyline = (CCreateEllipse*)m_createbaseFirst;	
		TrimPolyline(pPolyline->m_point,pPolyline->m_dTuDu);
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
	}
	
}

/*
 *	修剪直线
 */
void CDrawTrim::TrimLine()
{
	CCreateLine* pLine = (CCreateLine*)m_createbaseFirst;
	int l_iNoUse = 0;
	//交点
	Position posInter;
	if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateLine)))//直线
	{
		CCreateLine* pLine2 = (CCreateLine*)m_createbaseSecond;
		//求交点
		GetIntersectPoint(pLine->m_Begin,pLine->m_End,pLine2->m_Begin,pLine2->m_End,posInter);
		//如果交点不在切割线上，不进行操作
		if(!PosOnLine(pLine2->m_Begin,pLine2->m_End,posInter))
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n没有相交点",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		//点是否在直线上
		if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
			pLine->m_End = posInter;
		else
			pLine->m_Begin = posInter;
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateSpline)))//多段线
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosLineAndPolyline(l_iNoUse,posInter,pLine->m_Begin,pLine->m_End,pPolyline->m_point,pPolyline->m_dTuDu))
		{
			//点是否在直线上
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
		}	
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosLineAndPolyline(l_iNoUse,posInter,pLine->m_Begin,pLine->m_End,pPolyline->m_point,pPolyline->m_dTuDu))
		{
			//点是否在直线上
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
		}	
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{
		CCreateRoundArc* pArc = (CCreateRoundArc*)m_createbaseSecond;
		//与圆弧的交点个数
		int l_iNumOfInter =0;
		//求与圆的两个交点
		Position l_posLine1 = pLine->m_Begin,l_posLine2 = pLine->m_End;
		if(GetIntersectLineRound(l_posLine1,l_posLine2,pArc->m_Center,pArc->m_dRadius))//如果与圆有交点
		{
			//如果交点在弧上
			if(PosOnArc(l_posLine1,pArc->m_Begin,pArc->m_End,pArc->m_Center,pArc->m_dRadius,pArc->m_iDirect))
			{
				l_iNumOfInter++;
				posInter = l_posLine1;
			}
			if(PosOnArc(l_posLine2,pArc->m_Begin,pArc->m_End,pArc->m_Center,pArc->m_dRadius,pArc->m_iDirect))
			{
				l_iNumOfInter++;
				posInter = l_posLine2;
			}
		}
		//如果交点不在切割线上，不进行操作
		if(l_iNumOfInter == 0)
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n没有相交点",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		else if(l_iNumOfInter == 1)//有一个交点
		{
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
		}
		else if(l_iNumOfInter == 2)//有二个交点
		{
			//看应该取哪个交点
			if(l_posLine1.Distance(m_posSecond) >= l_posLine2.Distance(m_posSecond))
				posInter = l_posLine2;
			else
				posInter = l_posLine1;
			//修剪
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
			
		}
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{
		CCreateRound* pRound = (CCreateRound*)m_createbaseSecond;
		//与圆弧的交点个数
		int l_iNumOfInter =0;
		//求与圆的两个交点
		Position l_posLine1 = pLine->m_Begin,l_posLine2 = pLine->m_End;
		if(GetIntersectLineRound(l_posLine1,l_posLine2,pRound->m_Center,pRound->m_dRadius))//如果与圆有交点
		{
			//看应该取哪个交点
			if(l_posLine1.Distance(m_posSecond) >= l_posLine2.Distance(m_posSecond))
				posInter = l_posLine2;
			else
				posInter = l_posLine1;
			//修剪
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
			
		}
		else//如果交点不在切割线上，不进行操作
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n没有相交点",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosLineAndPolyline(l_iNoUse,posInter,pLine->m_Begin,pLine->m_End,pPolyline->m_point,pPolyline->m_dTuDu))
		{
			//点是否在直线上
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
		}	
	}
}

/*
 *	修剪圆弧
 */
void CDrawTrim::TrimAarc()
{	
	CCreateRoundArc* pArc = (CCreateRoundArc*)m_createbaseFirst;	
	//交点 
	Position posInter1,posInter2;
	//交点在多段线的哪段线上,交点个数
	int l_iNumOfInter = 0;
	if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*)m_createbaseSecond;
		//求与圆的两个交点
		posInter1 = pLine->m_Begin;posInter2 = pLine->m_End;
		//如果与圆有交点
		l_iNumOfInter = GetIntersectLineRound(posInter1,posInter2,pArc->m_Center,pArc->m_dRadius);		
		if(l_iNumOfInter == 0)
		{
			RemindNoInter();//提示没有交点		
			return;			
		}
		//选最近的交点//有二个交点 //两个点都在线上，并且第一点的距离大于第二点时选择第二点
		else if(l_iNumOfInter == 2)
		{
			int l_iLastInter = 0;
			if(PosOnLine(pLine->m_Begin,pLine->m_End,posInter1))
				l_iLastInter++;
			if(PosOnLine(pLine->m_Begin,pLine->m_End,posInter2))
				l_iLastInter++;
			if(l_iLastInter == 0)	
			{
				RemindNoInter();//提示没有交点		
				return;							
			}
			if(l_iLastInter == 1&&PosOnLine(pLine->m_Begin,pLine->m_End,posInter2))
				posInter1 = posInter2;	
			if(l_iLastInter == 2&&m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
				posInter1 = posInter2;	
		}
		GenerateArc((LONG)pArc,posInter1);
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosArcAndPolyline(l_iNumOfInter,posInter1,pArc->m_Begin,pArc->m_End,pArc->m_Center
								,pArc->m_dRadius,pArc->m_iDirect,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateArc((LONG)pArc,posInter1);
		else
			RemindNoInter();//提示没有交点		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosArcAndPolyline(l_iNumOfInter,posInter1,pArc->m_Begin,pArc->m_End,pArc->m_Center
			,pArc->m_dRadius,pArc->m_iDirect,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateArc((LONG)pArc,posInter1);
		else
			RemindNoInter();//提示没有交点		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{
		CCreateRoundArc* pArcSecond = (CCreateRoundArc*)m_createbaseSecond;
		//如果与圆有交点
		l_iNumOfInter = GetIntersectArcRound(pArc->m_Center,pArc->m_dRadius,pArcSecond->m_Center,
			pArcSecond->m_End,pArcSecond->m_Center,pArcSecond->m_dRadius,pArcSecond->m_iDirect,posInter1,posInter2);		
		if(l_iNumOfInter == 0||l_iNumOfInter == -1)
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n没有相交点",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		//选最近的交点//有二个交点 //两个点都在线上，并且第一点的距离大于第二点时选择第二点
		else if(m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
			posInter1 = posInter2;
		GenerateArc((LONG)pArc,posInter1);		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{
		CCreateRound* pRound = (CCreateRound*)m_createbaseSecond;
		//如果与圆有交点
		l_iNumOfInter = GetIntersectTwoRound(pRound->m_Center,pRound->m_dRadius,pArc->m_Center,
			pArc->m_dRadius,posInter1,posInter2);		
		if(l_iNumOfInter == 0||l_iNumOfInter == -1)
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n没有相交点",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		//选最近的交点//有二个交点 //两个点都在线上，并且第一点的距离大于第二点时选择第二点
		else if(l_iNumOfInter == 2&&m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
			posInter1 = posInter2;
		GenerateArc((LONG)pArc,posInter1);
		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosArcAndPolyline(l_iNumOfInter,posInter1,pArc->m_Begin,pArc->m_End,pArc->m_Center
			,pArc->m_dRadius,pArc->m_iDirect,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateArc((LONG)pArc,posInter1);
		else
			RemindNoInter();//提示没有交点		
	}	
}

/*
 *	修剪圆
 */

void CDrawTrim::TrimRound()
{
	CCreateRound* pRound = (CCreateRound*)m_createbaseFirst;	
	//交点 
	Position posInter1,posInter2;
	//交点在多段线的哪段线上,交点个数
	int l_iNumOfInter = 0;
	if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*)m_createbaseSecond;
		//求与圆的两个交点
		posInter1 = pLine->m_Begin;posInter2 = pLine->m_End;
		//如果与圆有交点
		l_iNumOfInter = GetIntersectLineRound(posInter1,posInter2,pRound->m_Center,pRound->m_dRadius);		
		if(l_iNumOfInter == 0)
		{
			RemindNoInter();//提示没有交点		
			return;			
		}
		//选最近的交点//有二个交点 //两个点都在线上，并且第一点的距离大于第二点时选择第二点
		else if(l_iNumOfInter == 2)
		{
			int l_iLastInter = 0;
			if(PosOnLine(pLine->m_Begin,pLine->m_End,posInter1))
				l_iLastInter++;
			if(PosOnLine(pLine->m_Begin,pLine->m_End,posInter2))
				l_iLastInter++;
			if(l_iLastInter == 0)	
			{
				RemindNoInter();//提示没有交点		
				return;							
			}
			if(l_iLastInter == 1&&PosOnLine(pLine->m_Begin,pLine->m_End,posInter2))
				posInter1 = posInter2;	
			if(l_iLastInter == 2&&m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
				posInter1 = posInter2;	
		}
		GenerateRound((LONG)m_createbaseFirst,posInter1);
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosRoundAndPolyline(l_iNumOfInter,posInter1,pRound->m_Center,pRound->m_dRadius,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateRound((LONG)m_createbaseFirst,posInter1);
		else
			RemindNoInter();//提示没有交点		
		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosRoundAndPolyline(l_iNumOfInter,posInter1,pRound->m_Center,pRound->m_dRadius,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateRound((LONG)m_createbaseFirst,posInter1);
		else
			RemindNoInter();//提示没有交点		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{
		CCreateRoundArc* pArc = (CCreateRoundArc*)m_createbaseSecond;
		//如果与圆有交点
		l_iNumOfInter = GetIntersectArcRound(pRound->m_Center,pRound->m_dRadius,pArc->m_Center,
			pArc->m_End,pArc->m_Center,pArc->m_dRadius,pArc->m_iDirect,posInter1,posInter2);		
		if(l_iNumOfInter == 0||l_iNumOfInter == -1)
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n没有相交点",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		//选最近的交点//有二个交点 //两个点都在线上，并且第一点的距离大于第二点时选择第二点
		else if(l_iNumOfInter == 2&&m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
			posInter1 = posInter2;
		GenerateRound((LONG)m_createbaseFirst,posInter1);		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{
		CCreateRound* pRoundSecond = (CCreateRound*)m_createbaseSecond;	
		//如果与圆有交点
		l_iNumOfInter = GetIntersectTwoRound(pRound->m_Center,pRound->m_dRadius,pRoundSecond->m_Center,
			pRoundSecond->m_dRadius,posInter1,posInter2);		
		if(l_iNumOfInter == 0||l_iNumOfInter == -1)
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n没有相交点",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		//选最近的交点//有二个交点 //两个点都在线上，并且第一点的距离大于第二点时选择第二点
		else if(l_iNumOfInter == 2&&m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
			posInter1 = posInter2;
		GenerateRound((LONG)m_createbaseFirst,posInter1);		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosRoundAndPolyline(l_iNumOfInter,posInter1,pRound->m_Center,pRound->m_dRadius,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateRound((LONG)m_createbaseFirst,posInter1);
		else
			RemindNoInter();//提示没有交点		
	}	
}

/*
 *	修剪多段线
 */
void CDrawTrim::TrimPolyline(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	//交点 
	Position posInter;
	//交点在多段线的哪段线上
	int l_iNumPoint = 0;
	if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosLineAndPolyline(l_iNumPoint,posInter,pLine->m_Begin,pLine->m_End,aryPoint,aryTudu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//重新生成多段线
		else
			RemindNoInter();//提示没有交点		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosPolylineAndPolyline(l_iNumPoint,posInter,aryPoint,aryTudu,pPolyline->m_point,pPolyline->m_dTuDu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//重新生成多段线
		else
			RemindNoInter();//提示没有交点		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosPolylineAndPolyline(l_iNumPoint,posInter,aryPoint,aryTudu,pPolyline->m_point,pPolyline->m_dTuDu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//重新生成多段线
		else
			RemindNoInter();//提示没有交点		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{
		CCreateRoundArc* pArc = (CCreateRoundArc*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosArcAndPolyline(l_iNumPoint,posInter,pArc->m_Begin,pArc->m_End,pArc->m_Center,
			pArc->m_dRadius,pArc->m_iDirect,aryPoint,aryTudu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//重新生成多段线
		else
			RemindNoInter();//提示没有交点		
		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{
		CCreateRound* pRound = (CCreateRound*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosRoundAndPolyline(l_iNumPoint,posInter,pRound->m_Center,pRound->m_dRadius,aryPoint,aryTudu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//重新生成多段线
		else
			RemindNoInter();//提示没有交点			
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*)m_createbaseSecond;
		//如果与多段线有交点
		if(GetInterPosPolylineAndPolyline(l_iNumPoint,posInter,aryPoint,aryTudu,pPolyline->m_point,pPolyline->m_dTuDu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//重新生成多段线
		else
			RemindNoInter();//提示没有交点		
	}
}

/*
 *	返回圆弧与多段线的交点修剪直线
 */
BOOL CDrawTrim::GetInterPosArcAndPolyline(int& iNumPoint,Position& posInterClose,Position posBegin,
										  Position posEnd,Position posCenter,double dRadius,int iDir,
								CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	Position l_posPolyline1,l_posPolyline2,l_posInter1,l_posInter2,l_posCenter;//多段线上的两个点
	//	多段线节点的凸度	点击的第二点与交点的最短距离		第二点与交点的距离
	double l_dTudu,l_dMinDisInter = 999999999999,				l_dDis;
	//点集的大小 
	int l_iSize = aryPoint.GetSize()-1,l_iNumInterPos;
	//先点的是否是圆弧
	BOOL l_bFirstArc = FALSE;
	if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		l_bFirstArc = TRUE;
	for(int i=0;i<l_iSize;i++)
	{
		l_posPolyline1 = aryPoint.GetAt(i);
		l_posPolyline2 = aryPoint.GetAt(i+1);
		l_dTudu = aryTudu.GetAt(i);
		//如果这段多段线是直线
		if(l_dTudu == 0.0)
		{
			//以下是求交点			
			if(l_bFirstArc==TRUE)//如果先点的是圆弧
			{
				//得到圆与直线的交点
				l_posInter1 = l_posPolyline1;l_posInter2 = l_posPolyline2;
				l_iNumInterPos = GetIntersectLineRound(l_posInter1,l_posInter2,posCenter,dRadius);
				if(l_iNumInterPos == 1)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter&&PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1))
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
				else if(l_iNumInterPos == 2)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter&&PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1))
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}			
					l_dDis = l_posInter2.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter&&PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter2))
					{
						posInterClose = l_posInter2;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
			}
			else//如果先点的是多段线
			{
				l_posInter1 = l_posPolyline1;l_posInter2 = l_posPolyline2;
				l_iNumInterPos = GetIntersectLineArc(l_posInter1,l_posInter2,posBegin,posEnd,posCenter,dRadius,iDir);
				if(l_iNumInterPos == 1)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
						&&PosOnArc(l_posInter1,posBegin,posEnd,posCenter,dRadius,iDir))
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
				else if(l_iNumInterPos == 2)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
						&&PosOnArc(l_posInter1,posBegin,posEnd,posCenter,dRadius,iDir))
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}			
					l_dDis = l_posInter2.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter2)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
						&&PosOnArc(l_posInter2,posBegin,posEnd,posCenter,dRadius,iDir))
					{
						posInterClose = l_posInter2;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}				
			}
		}
		else//如果这段多段线是圆弧		
		{
			g_pDoc->polylineToArc(l_posPolyline1,l_posPolyline2,l_dTudu,l_posCenter);
			double l_dRadius = l_posCenter.Distance(l_posPolyline1);
			int l_iDir = RtnArcDir(l_dTudu);
			//以下是求交点			
			if(l_bFirstArc==TRUE)//如果先点的是圆弧
			{
				//得到圆与直线的交点
				l_iNumInterPos = GetIntersectArcRound(posCenter,dRadius,l_posPolyline1,l_posPolyline2,l_posCenter,
					l_dRadius,l_iDir,l_posInter1,l_posInter2);
				if(l_iNumInterPos == 1)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter)
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
				else if(l_iNumInterPos == 2)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter)
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}			
					l_dDis = l_posInter2.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter)
					{
						posInterClose = l_posInter2;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
			}
			else//如果先点的是多段线
			{
				//与弧有交点个数			
				l_iNumInterPos = GetIntersectTwoArc(l_posPolyline1,l_posPolyline2,l_posCenter,l_dRadius,l_iDir,
					posBegin,posEnd,posCenter,dRadius,iDir,l_posInter1,l_posInter2);
				if((i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))//如果是起点或末点要求与圆的交点
					//得到圆与弧的交点
					l_iNumInterPos = GetIntersectArcRound(l_posCenter,l_dRadius,posBegin,posEnd,
												posCenter,dRadius,iDir,l_posInter1,l_posInter2);				
				if(l_iNumInterPos == 1)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter)
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
				else if(l_iNumInterPos == 2)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
						&&PosOnArc(l_posInter1,posBegin,posEnd,posCenter,dRadius,iDir))
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}			
					l_dDis = l_posInter2.Distance(m_posSecond);//交点与点击点的距离
					if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter2)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
						&&PosOnArc(l_posInter2,posBegin,posEnd,posCenter,dRadius,iDir))
					{
						posInterClose = l_posInter2;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}				
			}			
		}
	}	
	if(l_dMinDisInter < 999999999990)//说明有交点
		return TRUE;
	return FALSE;
}

/*
 *	返回圆与多段线的交点修剪直线
 */
 BOOL CDrawTrim::GetInterPosRoundAndPolyline(int& iNumPoint,Position& posInterClose,Position posCenter,
	 double dRadius,CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	Position l_posPolyline1,l_posPolyline2,l_posInter1,l_posInter2,l_posCenter;//多段线上的两个点
	//	多段线节点的凸度	点击的第二点与交点的最短距离		第二点与交点的距离
	double l_dTudu,l_dMinDisInter = 999999999999,				l_dDis;
	//点集的大小 
	int l_iSize = aryPoint.GetSize()-1,l_iNumInterPos;
	//先点的是否是圆弧
	BOOL l_bFirstArc = FALSE;
	if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		l_bFirstArc = TRUE;
	for(int i=0;i<l_iSize;i++)
	{
		l_posPolyline1 = aryPoint.GetAt(i);
		l_posPolyline2 = aryPoint.GetAt(i+1);
		l_dTudu = aryTudu.GetAt(i);
		//如果这段多段线是直线
		if(l_dTudu == 0.0)
		{
			//以下是求交点			
			if(l_bFirstArc==TRUE)//如果先点的是圆弧
			//得到圆与直线的交点
			l_posInter1 = l_posPolyline1;l_posInter2 = l_posPolyline2;
			l_iNumInterPos = GetIntersectLineRound(l_posInter1,l_posInter2,posCenter,dRadius);
			if(l_iNumInterPos == 1)
			{
				l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
				//当距离大于最后距离 并且 交点在线上。当先点的是多段线并且是首末线时可以不用在线上
				if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1)||
					(l_bFirstArc==FALSE&&((i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1)))))
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}								
			}
			else if(l_iNumInterPos == 2)
			{
				l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
				if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1)||
					(l_bFirstArc==FALSE&&((i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1)))))
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}			
				l_dDis = l_posInter2.Distance(m_posSecond);//交点与点击点的距离
				if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter2)||
					(l_bFirstArc==FALSE&&((i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1)))))
				{
					posInterClose = l_posInter2;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}								
			}
		}
		else//如果这段多段线是圆弧		
		{
			g_pDoc->polylineToArc(l_posPolyline1,l_posPolyline2,l_dTudu,l_posCenter);
			double l_dRadius = l_posCenter.Distance(l_posPolyline1);
			int l_iDir = RtnArcDir(l_dTudu);
			//得到圆与弧的交点
			l_iNumInterPos = GetIntersectArcRound(posCenter,dRadius,l_posPolyline1,l_posPolyline2,l_posCenter,
				l_dRadius,l_iDir,l_posInter1,l_posInter2);
			//如果先点的是多段线并且是首末点	
			if(l_bFirstArc==FALSE&&((i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1)))
				l_iNumInterPos = GetIntersectTwoRound(posCenter,dRadius,l_posCenter,l_dRadius,l_posInter1,l_posInter2);
			if(l_iNumInterPos == 1)
			{
				l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}								
			}
			else if(l_iNumInterPos == 2)
			{
				l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}			
				l_dDis = l_posInter2.Distance(m_posSecond);//交点与点击点的距离
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter2;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}								
			}
		}
	}	
	if(l_dMinDisInter < 999999999990)//说明有交点
		return TRUE;
	return FALSE;
}

/*
 *	返回直线与多段线的交点修剪直线
 */
BOOL CDrawTrim::GetInterPosLineAndPolyline(int& iNumPoint,Position& posInterClose,Position posBegin,Position posEnd,
		CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	Position l_posPolyline1,l_posPolyline2,l_posInter;//多段线上的两个点
	double l_dTudu,l_dMinDisInter = 999999999999;
	//点集的大小 
	int l_iSize = aryPoint.GetSize()-1;
	//先点的是否是直线
	BOOL l_bFirstLine = FALSE;
	if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		l_bFirstLine = TRUE;
	for(int i=0;i<l_iSize;i++)
	{
		l_posPolyline1 = aryPoint.GetAt(i);
		l_posPolyline2 = aryPoint.GetAt(i+1);
		l_dTudu = aryTudu.GetAt(i);
		//如果是直线
		if(l_dTudu == 0.0)
		{
			//求交点
			if(GetIntersectPoint(posBegin,posEnd,l_posPolyline1,l_posPolyline2,l_posInter))
			{
				double l_dDis = l_posInter.Distance(m_posSecond);//交点与点击点的距离
				//如果交点在切割线上并且比上个交点距离短,先点击的是直线
				if(l_bFirstLine==TRUE&&PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter)&&l_dDis < l_dMinDisInter)
				{
					posInterClose = l_posInter;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}	//若先点的多段线					若先点的多段线的第一条或最后一条可以不用在多段线上
				else if(l_bFirstLine==FALSE&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
					&&PosOnLine(posBegin,posEnd,l_posInter)&&l_dDis < l_dMinDisInter)
				{
					posInterClose = l_posInter;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;					
				}
			}
		}
		else
		{
			Position l_posInter1 = posBegin,l_posInter2 = posEnd,l_posCenter;
			g_pDoc->polylineToArc(l_posPolyline1,l_posPolyline2,l_dTudu,l_posCenter);
			//与弧有交点个数
			int l_iNumInterPos = GetIntersectLineArc(l_posInter1,l_posInter2,l_posPolyline1,
				l_posPolyline2,l_posCenter,l_posCenter.Distance(l_posPolyline1),RtnArcDir(l_dTudu));
			if((i==0||i==l_iSize-1)&& l_bFirstLine==FALSE)//如果是第一条线或最后一条线要延长，就得看与圆
			{
				l_posInter1 = posBegin;l_posInter2 = posEnd;
				l_iNumInterPos = GetIntersectLineRound(l_posInter1,l_posInter2,l_posCenter,l_posCenter.Distance(l_posPolyline1));
			}
			if(l_iNumInterPos == 1)//如果有一个交点
			{
				double l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}			
			}				
			if(l_iNumInterPos == 2)//如果有两个交点
			{
				double l_dDis = l_posInter1.Distance(m_posSecond);//交点与点击点的距离
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}			
				l_dDis = l_posInter2.Distance(m_posSecond);//交点与点击点的距离
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter2;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}			
			}
		}
	}	
	if(l_dMinDisInter < 999999999990)//说明有交点
		return TRUE;
	return FALSE;
}

/*
 *	返回多段线与多段线的交点修剪多段线
 */
BOOL CDrawTrim::GetInterPosPolylineAndPolyline(int& iNumPoint,Position& posInterClose,CArray<Position,Position&> &aryPoint1, CArray<double,double&> &aryTudu1,CArray<Position,Position&> &aryPoint2,CArray<double,double&> &aryTudu2)
{
	Position l_posPolyline11,l_posPolyline12,l_posPolyline21,l_posPolyline22,l_posCenter,l_posInter;//多段线上的两个点
	double l_dTudu1,l_dTudu2,l_dRadius1,l_dRadius2,l_dMinDisInter = 999999999999;
	//点集的大小 
	int l_iSize1 = aryPoint1.GetSize()-1;
	int l_iSize2 = aryPoint2.GetSize()-1;
	//交点与点击点的距离
	double l_dDis;
	for(int i=0;i<l_iSize1;i++)//第一次点击的线
	{
		l_posPolyline11 = aryPoint1.GetAt(i);
		l_posPolyline12 = aryPoint1.GetAt(i+1);
		l_dTudu1 = aryTudu1.GetAt(i);
		//第一次点击的线如果是直线
		if(l_dTudu1 == 0.0)
		{
			for(int j=0;j<l_iSize2;j++)//第二次点击的线
			{
				l_posPolyline21 = aryPoint2.GetAt(j);
				l_posPolyline22 = aryPoint2.GetAt(j+1);
				//求交点
				l_dTudu2 = aryTudu2.GetAt(j);
				if(l_dTudu2 == 0.0)//直线与直线交点
				{
					if(GetIntersectPoint(l_posPolyline21,l_posPolyline22,l_posPolyline11,l_posPolyline12,l_posInter))
					{
						l_dDis = l_posInter.Distance(m_posSecond);//交点与点击点的距离
						//如果交点在切割线上并且比上个交点距离短					头或尾时有可能是延长交点没必要在自身上
						if((PosOnLine(l_posPolyline11,l_posPolyline12,l_posInter)||i==0||i==l_iSize1-1)&&
							PosOnLine(l_posPolyline21,l_posPolyline22,l_posInter)&&l_dDis < l_dMinDisInter)
						{
							posInterClose = l_posInter;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}			
					}
				}
				else//直线与圆弧交点
				{
					//与圆弧的两个交点
					Position posArcInter1 = l_posPolyline11,posArcInter2 = l_posPolyline12;
					g_pDoc->polylineToArc(l_posPolyline21,l_posPolyline22,l_dTudu2,l_posCenter);//求圆心
					l_dRadius1 = l_posCenter.Distance(l_posPolyline21);
					//交点个数
					int l_iNumInterPos = GetIntersectLineArc(posArcInter1,posArcInter2,l_posPolyline21,
						l_posPolyline22,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu2));
					if(l_iNumInterPos == 1)//如果只有一个交点
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//交点与点击点的距离
						//如果交点在切割线上并且比上个交点距离短					头或尾时有可能是延长交点没必要在自身上
						if((PosOnLine(l_posPolyline11,l_posPolyline12,posArcInter1)||i==0||i==l_iSize1-1)&&
							PosOnArc(posArcInter1,l_posPolyline21,l_posPolyline22,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu2))
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
					}
					else if(l_iNumInterPos == 2)//如果有二个交点
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//交点与点击点的距离
						//如果交点在切割线上并且比上个交点距离短					头或尾时有可能是延长交点没必要在自身上
						if((PosOnLine(l_posPolyline11,l_posPolyline12,posArcInter1)||i==0||i==l_iSize1-1)&&
							PosOnArc(posArcInter1,l_posPolyline21,l_posPolyline22,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu2))
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
						l_dDis = posArcInter2.Distance(m_posSecond);//交点与点击点的距离
						//如果交点在切割线上并且比上个交点距离短					头或尾时有可能是延长交点没必要在自身上
						if((PosOnLine(l_posPolyline11,l_posPolyline12,posArcInter2)||i==0||i==l_iSize1-1)&&
							PosOnArc(posArcInter2,l_posPolyline21,l_posPolyline22,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu2))
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter2;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}											
					}
				}
			}			
		}
		else//第一次点击的线如果是圆弧		
		{
			for(int j=0;j<l_iSize2;j++)//第二次点击的线
			{
				l_posPolyline21 = aryPoint2.GetAt(j);
				l_posPolyline22 = aryPoint2.GetAt(j+1);
				//求交点
				l_dTudu2 = aryTudu2.GetAt(j);
				if(l_dTudu2 == 0.0)//圆弧与直线交点
				{
					//与圆弧的两个交点
					Position posArcInter1 = l_posPolyline21,posArcInter2 = l_posPolyline22;
					g_pDoc->polylineToArc(l_posPolyline11,l_posPolyline12,l_dTudu1,l_posCenter);//求圆心
					l_dRadius1 = l_posCenter.Distance(l_posPolyline11);
					//交点个数
					int l_iNumInterPos = GetIntersectLineArc(posArcInter1,posArcInter2,l_posPolyline11,
						l_posPolyline12,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu1));
					if(i==0||i==l_iSize1-1 )//如果是第一条线或最后一条线要延长，就得看与圆
					{
						posArcInter1 = l_posPolyline21;posArcInter2 = l_posPolyline22;
						l_iNumInterPos = GetIntersectLineRound(posArcInter1,posArcInter2,l_posCenter,l_dRadius1);
					}
					if(l_iNumInterPos == 1)//如果只有一个交点
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//交点与点击点的距离
						//如果交点在切割线上并且比上个交点距离短					头或尾时有可能是延长交点没必要在自身上
						if(PosOnLine(l_posPolyline21,l_posPolyline22,posArcInter1)&&
							(PosOnArc(posArcInter1,l_posPolyline11,l_posPolyline12,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
					}
					else if(l_iNumInterPos == 2)//如果有二个交点
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//交点与点击点的距离
						//如果交点在切割线上并且比上个交点距离短					头或尾时有可能是延长交点没必要在自身上
						if(PosOnLine(l_posPolyline21,l_posPolyline22,posArcInter1)&&
							(PosOnArc(posArcInter1,l_posPolyline11,l_posPolyline12,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
						l_dDis = posArcInter2.Distance(m_posSecond);//交点与点击点的距离
						//如果交点在切割线上并且比上个交点距离短					头或尾时有可能是延长交点没必要在自身上
						if(PosOnLine(l_posPolyline21,l_posPolyline22,posArcInter2)&&
							(PosOnArc(posArcInter2,l_posPolyline11,l_posPolyline12,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter2;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}											
					}
				}
				else//圆弧与圆弧相交
				{
					//圆弧与圆弧的两个交点
					Position posArcInter1,posArcInter2,l_posCenter1,l_posCenter2;
					g_pDoc->polylineToArc(l_posPolyline11,l_posPolyline12,l_dTudu1,l_posCenter1);//求圆心
					l_dRadius1 = l_posCenter1.Distance(l_posPolyline11);
					g_pDoc->polylineToArc(l_posPolyline21,l_posPolyline22,l_dTudu2,l_posCenter2);//求圆心
					l_dRadius2 = l_posCenter2.Distance(l_posPolyline21);
					
					//交点个数
					int l_iNumInterPos = GetIntersectTwoArc(l_posPolyline11,l_posPolyline12,l_posCenter1,
						l_dRadius1,RtnArcDir(l_dTudu1),l_posPolyline21,l_posPolyline22,l_posCenter2,
						l_dRadius2,RtnArcDir(l_dTudu2),posArcInter1,posArcInter2);
					if(i==0||i==l_iSize1-1 )//如果是第一条线或最后一条线要延长，就得看与圆
					{
						l_iNumInterPos = GetIntersectArcRound(l_posCenter1,l_dRadius1,l_posPolyline21,
							l_posPolyline22,l_posCenter2,l_dRadius2,RtnArcDir(l_dTudu2),posArcInter1,posArcInter2);
					}
					if(l_iNumInterPos == 1)//如果只有一个交点
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//交点与点击点的距离
						//如果交点在切割线上并且比上个交点距离短					头或尾时有可能是延长交点没必要在自身上
						if(PosOnArc(posArcInter1,l_posPolyline21,l_posPolyline22,l_posCenter2,l_dRadius2,RtnArcDir(l_dTudu2))&&
							(PosOnArc(posArcInter1,l_posPolyline11,l_posPolyline12,l_posCenter1,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
					}
					else if(l_iNumInterPos == 2)//如果有二个交点
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//交点与点击点的距离
						//如果交点在切割线上并且比上个交点距离短					头或尾时有可能是延长交点没必要在自身上
						if(PosOnArc(posArcInter1,l_posPolyline21,l_posPolyline22,l_posCenter2,l_dRadius2,RtnArcDir(l_dTudu2))&&
							(PosOnArc(posArcInter1,l_posPolyline11,l_posPolyline12,l_posCenter1,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
						l_dDis = posArcInter2.Distance(m_posSecond);//交点与点击点的距离
						//如果交点在切割线上并且比上个交点距离短					头或尾时有可能是延长交点没必要在自身上
						if(PosOnArc(posArcInter2,l_posPolyline21,l_posPolyline22,l_posCenter2,l_dRadius2,RtnArcDir(l_dTudu2))&&
							(PosOnArc(posArcInter2,l_posPolyline11,l_posPolyline12,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter2;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}											
					}			
				}
			}								
		}
	}	
	if(l_dMinDisInter < 999999999990)//说明有交点
		return TRUE;
	return FALSE;
}

/*
 *	向撤消链表里添加操作
 */
void CDrawTrim::AddUndoList()
{
	m_pUndoList = m_UndoRedo.NewUndoList();
	m_pUndoList->OperatieWhat = ID_IO_KNIFE;
	
	m_pCurGraphPos = m_UndoRedo.NewGraphPos();
	m_pCurGraphPos->pCreateBasepos = (LONG)m_createbaseFirst;
	//如果是直线的话
	if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*) m_createbaseFirst;
		m_pUndoList->pOperatieParam = m_UndoRedo.NewOperateParam();
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potBase,pLine->m_Begin);
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potDes,pLine->m_End);		
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))//多段线
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*) m_createbaseFirst;
		m_pCurGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		m_pCurGraphPos->aryPoint.Copy(pPolyline->m_point);
		m_pCurGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		m_pCurGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))//矩形
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*) m_createbaseFirst;
		m_pCurGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		m_pCurGraphPos->aryPoint.Copy(pPolyline->m_point);
		m_pCurGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		m_pCurGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))//椭圆
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*) m_createbaseFirst;
		m_pCurGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		m_pCurGraphPos->aryPoint.Copy(pPolyline->m_point);
		m_pCurGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		m_pCurGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))//圆弧
	{
		CCreateRoundArc* pArc = (CCreateRoundArc*) m_createbaseFirst;
		m_pUndoList->pOperatieParam = m_UndoRedo.NewOperateParam();
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potBase,pArc->m_Begin);
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potDes,pArc->m_End);	
		m_pUndoList->pOperatieParam->angle = pArc->m_dAngleStart;
		m_pUndoList->pOperatieParam->distance = pArc->m_dAngleEnd;
	}
	m_pUndoList->pGraphPos = m_pCurGraphPos;
	g_pDoc->m_Undo->AddUndoList(m_pUndoList);
}

/*
 *	通过圆生成新圆弧
 */
BOOL CDrawTrim::GenerateRound(LONG pRound,Position posInter)
{
	CCreateRound* pRoundTurn = (CCreateRound*)pRound;
	CCreateRoundArc* pNewArc = NULL;
	//如起点和断点之间
	if(PosOnArc(m_posFirst,pRoundTurn->m_EndPos,posInter,pRoundTurn->m_Center,pRoundTurn->m_dRadius,pRoundTurn->m_iDirect))
		pNewArc = new CCreateRoundArc(pRoundTurn->m_EndPos,pRoundTurn->m_Center,posInter,pRoundTurn->m_iDirect);
	else
		pNewArc = new CCreateRoundArc(posInter,pRoundTurn->m_Center,pRoundTurn->m_EndPos,pRoundTurn->m_iDirect);
	POSITION POS = g_pDoc->m_CCreateBaseList.Find(m_createbaseFirst);
	g_pDoc->m_CCreateBaseList.InsertBefore(POS,pNewArc);
	m_pCurGraphPos->pCreateBaseposSpare1 = (LONG)pNewArc;	
	g_pDoc->m_CCreateBaseList.RemoveAt(POS);		
	
	return TRUE;
}

/*
 *	生成新圆弧
 */
BOOL CDrawTrim::GenerateArc(LONG pArc,Position posInter)
{
	CCreateRoundArc* pArcTurn = (CCreateRoundArc*)pArc;
	//如果在圆弧上
	if(PosOnArc(posInter,pArcTurn->m_Begin,pArcTurn->m_End,pArcTurn->m_Center,pArcTurn->m_dRadius,pArcTurn->m_iDirect))
	{
		if(PosOnArc(m_posFirst,pArcTurn->m_Begin,posInter,pArcTurn->m_Center,pArcTurn->m_dRadius,pArcTurn->m_iDirect))
			pArcTurn->m_End = posInter;
		else
			pArcTurn->m_Begin = posInter;		
	}	
	else//不在圆弧上
	{
		if(m_posFirst.Distance(pArcTurn->m_Begin) >= m_posFirst.Distance(pArcTurn->m_End))
			pArcTurn->m_End = posInter;
		else
			pArcTurn->m_Begin = posInter;
	}
	pArcTurn->m_dAngleEnd = GetAngle(pArcTurn->m_Center,pArcTurn->m_End);
	pArcTurn->m_dAngleStart = GetAngle(pArcTurn->m_Center,pArcTurn->m_Begin);
	return TRUE;
}

/*
 *	生成新的多段线
 */
BOOL CDrawTrim::GeneratePolyline(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,
								 Position posInter,int l_iNumPoint)
{
	//交点 
	Position l_posBegin,l_posEnd,l_posCenter;
	double l_dTudu = 0.0;
	l_posBegin = aryPoint.GetAt(l_iNumPoint);
	l_posEnd = aryPoint.GetAt(l_iNumPoint + 1);
	l_dTudu = aryTudu.GetAt(l_iNumPoint);
	//点击点是否在开始点与交点之间
	BOOL l_bClickOnBegin = FALSE;
	double l_dNewTudu = 0.0;
	//当交点与点击点在一条线上时，根据交点在线的哪边来删除或保留线
	if(l_dTudu == 0.0)
		l_bClickOnBegin = PosOnLine(l_posBegin,posInter,m_posFirst);
	else
	{
		g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//求圆心
		//求新的圆弧的凸度
		l_dNewTudu = g_pDoc->RtnTudu(l_posBegin,posInter,l_posCenter,RtnArcDir(l_dTudu));
		l_bClickOnBegin = PosOnArc(m_posFirst,l_posBegin,posInter,l_posCenter,l_posCenter.Distance(l_posBegin),RtnArcDir(l_dTudu));
		if(l_iNumPoint == 0&&!PosOnArc(posInter,l_posBegin,l_posEnd,l_posCenter,l_posCenter.Distance(l_posBegin),RtnArcDir(l_dTudu)))
		{
			l_dNewTudu = g_pDoc->RtnTudu(posInter,l_posEnd,l_posCenter,RtnArcDir(l_dTudu));					
			l_bClickOnBegin = FALSE;
		}
	}
	//重新生成多段线
	if(l_iNumPoint>m_iPick1||(l_iNumPoint==m_iPick1&&l_bClickOnBegin))
	{
		((CCreatePolyline*)m_createbaseFirst)->m_point.RemoveAt(l_iNumPoint+1,aryPoint.GetSize()-l_iNumPoint-1);
		((CCreatePolyline*)m_createbaseFirst)->m_dTuDu.RemoveAt(l_iNumPoint+1,aryTudu.GetSize()-l_iNumPoint-1);
		((CCreatePolyline*)m_createbaseFirst)->m_point.Add(posInter);
		((CCreatePolyline*)m_createbaseFirst)->m_dTuDu.SetAt(aryTudu.GetSize()-1,l_dNewTudu);
	}
	else
	{
		((CCreatePolyline*)m_createbaseFirst)->m_point.RemoveAt(0,l_iNumPoint+1);
		((CCreatePolyline*)m_createbaseFirst)->m_dTuDu.RemoveAt(0,l_iNumPoint+1);
		((CCreatePolyline*)m_createbaseFirst)->m_point.InsertAt(0,posInter);
		if(l_dNewTudu!=0.0)
			l_dNewTudu = g_pDoc->RtnTudu(posInter,l_posEnd,l_posCenter,RtnArcDir(l_dTudu));
		((CCreatePolyline*)m_createbaseFirst)->m_dTuDu.InsertAt(0,l_dNewTudu);
	}				
	return TRUE;
}

/*
 *	提示没有交点
 */
void CDrawTrim::RemindNoInter()
{
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n没有交点:");
	else
		RemindContent("\r\nNo intersection:");					
	
}



