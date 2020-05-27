// DrawBreakTwo.cpp: implementation of the CDrawBreakTwo class.
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

CDrawBreakTwo::CDrawBreakTwo()
: m_Begin(0,0), m_End(0,0)
{
	m_pCreateBase = NULL;
	m_pCreateBase1 = NULL;
	m_pCreateBase2 = NULL;
	
}

CDrawBreakTwo::~CDrawBreakTwo()
{

}

/*
 *	擦除最后留下的线，并且标注上第一点
 */
void CDrawBreakTwo::InvalLastLine()
{
	CDC *pDC = g_pView->GetDC();	
	CPoint pt;
	g_pView->WorldToScreem(m_Begin,pt);
	pDC->MoveTo(pt.x-5,pt.y-5);
	pDC->LineTo(pt.x+5,pt.y+5);
	pDC->MoveTo(pt.x+5,pt.y-5);
	pDC->LineTo(pt.x-5,pt.y+5);
//	pDC->TextOut(pt.x,pt.y,"第一点");
	
/*
	pDC->MoveTo(pt.x-5,pt.y-5);
	pDC->LineTo(pt.x-5,pt.y+1);
	pDC->LineTo(pt.x-1,pt.y+1);
	pDC->LineTo(pt.x-1,pt.y+5);
	pDC->LineTo(pt.x+5,pt.y+5);
	pDC->LineTo(pt.x+5,pt.y-1);
	pDC->LineTo(pt.x+1,pt.y-1);
	pDC->LineTo(pt.x+1,pt.y-5);
	pDC->LineTo(pt.x-5,pt.y-5);
*/
	g_pView->ReleaseDC(pDC);
	
}

int CDrawBreakTwo::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	int l_iInvali;
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	//输入的长度
	double l_dInputLong=0.0;
	CPoint l_pot;
	g_pView->WorldToScreem(l_cPos,l_pot);
	//点击第二次时
	if (m_iNumOfClick == 1)
	{
		//如果是输入的长度的话
		if(nFlags == 2)//键盘输入
		{
			if (m_pCreateBase2 == NULL)//当键盘输入时没有右键点击
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n没有右键点中图形，重新点击或按ESC键取消");
				else
					RemindContent("\r\nNo right point graphic, click or press the ESC key to cancel：");	
				return FALSE;				
			}
			l_dInputLong = atof(cmd);
			//输入的长度返回线上的点，返回假说明输入的长度不在范围内
			if (!m_pCreateBase->RtnLongPos(m_posRight,l_dInputLong,l_cPos,l_iInvali)) 
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n超出范围，重新点击或按ESC键取消");
				else
					RemindContent("\r\nBeyond the range, click or press the ESC key to cancel：");	
				return FALSE;
			}
			else
			{
				m_pCreateBase = m_pCreateBase1;
				m_iNumOfClick++;	
			}
		}
		else
		{
			if (RtnPickLine(l_pot,2)&&m_pCreateBase2 == m_pCreateBase1)
			{
				m_pCreateBase = m_pCreateBase2;
				m_iNumOfClick++;
			}	
			else
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n没有点中图形，重新点击或按ESC键取消");				
				else
					RemindContent("\r\nThere is no point graphics, click or press the ESC key to cancel：");	
				return FALSE;
			}
		}
	}
	// 如果点中图元
	if(m_iNumOfClick == 0)	
	{	
		//如果是输入的长度的话
		if(nFlags == 2)//键盘输入
		{
			if (m_pCreateBase == NULL)//当键盘输入时没有右键点击
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n没有右键点中图形，重新点击或按ESC键取消");
				else
					RemindContent("\r\nNo right point graphic, click or press the ESC key to cancel：");	
				return FALSE;				
			}
			l_dInputLong = atof(cmd);
			//输入的长度返回线上的点，返回假说明输入的长度不在范围内
			if (!m_pCreateBase->RtnLongPos(m_posRight,l_dInputLong,l_cPos,l_iInvali)) 
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n超出范围，重新点击或按ESC键取消");
				else
					RemindContent("\r\nBeyond the range, click or press the ESC key to cancel：");	
				return FALSE;
			}
			else
			{
				m_pCreateBase = m_pCreateBase1;
				m_iNumOfClick++;	
			}
		}
		else//鼠标输入
		{
			if ( RtnPickLine(l_pot,1))
			{
				m_pCreateBase = m_pCreateBase1;
				m_iNumOfClick++;
			}
			else
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n没有点中图形，重新点击或按ESC键取消");				
				else
					RemindContent("\r\nThere is no point graphics, click or press the ESC key to cancel：");	
				return FALSE;
			}
		}
	}

	switch(m_iNumOfClick)//鼠标点击次数
	{
	case 1:
		m_Begin = l_cPos;
		if (m_pCreateBase != NULL)//如果没有双击选中的图形，就不
		{
			//直线//////////////////////////////////////////
			if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				if(BreakLine(l_cPos))//把直线打成两个的直线
					SuccessBreak();
				else
					m_iNumOfClick = 0;
			}

			//多线段//////////////////////////////////////////
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{
				CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
				//如果不是闭合的话
				if (!PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))
				{
					if(BreakPolyline(l_cPos))//把直线打成两个的直线
						SuccessBreak();
					else
						m_iNumOfClick = 0;
				}
				else
				{
					InvalLastLine();
					//中文还是英文
					if (g_pDoc->m_stcWorkFor.IsChinese)
						RemindContent("\r\n选中第一点，再选第二点");
					else
						RemindContent("\r\nThe first point selected, then choose the second point：");	
				}
			}
			
			//矩形//////////////////////////////////////////
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{
				InvalLastLine();
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n选中第一点，再选第二点");
				else
					RemindContent("\r\nThe first point selected, then choose the second point：");	
			}
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{//圆
				InvalLastLine();
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n选中第一点，再选第二点");
				else
					RemindContent("\r\nThe first point selected, then choose the second point：");	
			}
			
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
			{//圆弧
				if(BreakArc(m_Begin))
					SuccessBreak();
				else
					m_iNumOfClick = 0;				
			}
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{//椭圆
				InvalLastLine();
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n选中第一点，再选第二点");
				else
					RemindContent("\r\nThe first point selected, then choose the second point：");	
			}
		}
		break;
	case 2:			
		//画上最后留下的
		m_End = l_cPos;
		//如果点击的两个点太近返回
		if (m_Begin.Distance(m_End)<2)
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n点击与第一点重合!请重新点击：");
			else
				RemindContent("\r\nClick with the first point! Please click again：");	
			m_iNumOfClick = 1;
			return 0;
		}
		if (m_pCreateBase != NULL)
		{
			if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{//多线段
				if(BreakPolyline(m_Begin,m_End))
				{
					SuccessBreak();
					break;
				}		
			}
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{//矩形
				
				if(BreakRect(m_Begin,m_End))
				{
					SuccessBreak();
					break;
				}
			}
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{//圆
				if(BreakRound(m_Begin,m_End))
				{
					SuccessBreak();
					break;
				}
			}
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{//椭圆
				if(BreakEllipse(m_Begin,m_End))
				{
					SuccessBreak();
					break;
				}
			}
		}
		break;		
	}
	return 0;
}

int CDrawBreakTwo::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC *pDC = g_pView->GetDC();
	Position	prePos(m_End), curPos(pos);
	//对象捕捉
	FindPointAll(pDC,m_posRight,pos,(g_pView->m_iSwitch&NEAR_POINT)|(g_pView->m_iSwitch&END_POINT)|(g_pView->m_iSwitch&MID_POINT));	
	g_pView->ReleaseDC(pDC);
	return 0;	
}


int CDrawBreakTwo::OnRButtonDown(UINT nFlags,const Position &pos)
{
	g_refresh = 0;	
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);

	//是否选中图形
	BOOL l_bIsPick = FALSE;
	//屏幕鼠标的点
	CPoint point;
	g_pView->WorldToScreem(pos,point);
	POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//获得头指针，在下面循环上用。
	while(POS != NULL)//进入扫描链表
	{
		m_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);
		// 如果链表里的图元被选中
		if( m_pCreateBase->IsPick(point) != -1)
		{
			l_bIsPick = TRUE;
			break;
		}
	}
	//如果没有选中退出
	if (l_bIsPick == FALSE)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n右键没有点中图形");						
		else
			RemindContent("\r\nRight without some graphics：");	
		return FALSE;
	}

	//点击第二次时
	if (m_iNumOfClick == 1)
	{
		m_pCreateBase2 = m_pCreateBase;
		if(m_pCreateBase2 == m_pCreateBase1)
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n输入距离此点的长度或按ESC键取消");						
			else
				RemindContent("\r\nEnter the length of the distance from this point or press the ESC key to cancel：");	
		}
		else
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n没有点中图形或点中的不是一个图形!可重新右键点击：");		
			else
				RemindContent("\r\nThere is no point graphics or not a graphic in your site! Can right click again：");	
		}
	}
	// 如果点中图元
	if(m_iNumOfClick == 0)	
	{
		m_pCreateBase1 = m_pCreateBase;
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n输入距离此点的长度或按ESC键取消");						
		else
			RemindContent("\r\nEnter the length of the distance from this point or press the ESC key to cancel：");	
	}
	m_posRight = l_cPos;
	//把焦点放在命令输入框里
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	
	return 0;
}

/*
 *	 把直线打成两个点
 */
BOOL CDrawBreakTwo::BreakLine(Position l_cPos)
{
	CCreateLine *line = (CCreateLine*)m_pCreateBase;
	//以下是怕垂足在直线的延长线上
	if(!PosBelongBox(line->m_Begin,line->m_End,l_cPos))
		return FALSE;
	//返回直线上垂足的点
	Position l_cBreakPos = l_cPos.DistancePosFoot(line->m_Begin,line->m_End);
	//如果点到线的距离太远，不认为点到了
	if(l_cBreakPos.Distance(l_cPos)/g_pView->ReturnProportion()>=PRECISION)
		return FALSE;
	//如果点击点与开始点相近认为是一个点,返回
	if(l_cBreakPos.Distance(line->m_Begin)/g_pView->ReturnProportion()<=PRECISION)
	{		
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点到起点无效!请重新点击：",2);		
		else
			RemindContent("\r\nPoint to the starting point is invalid! Please click again：");	
		return FALSE;
	}
	//如果点击点与结束点相近认为是一个点
	else if(l_cBreakPos.Distance(line->m_End)/g_pView->ReturnProportion()<=PRECISION)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点到末点无效!请重新点击：",2);						
		else
			RemindContent("\r\nAt the end of point to point is invalid! Please click again：");	
		return FALSE;
	}

	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//建立两个新的直线
	CCreateLine* l_pNewLine1 = new CCreateLine(line->m_Begin,l_cBreakPos);
	l_pNewLine1->m_ColorOfLine = line->m_ColorOfLine;//把颜色也定义进去
	l_pNewLine1->m_strCBM.Copy(line->m_strCBM);	
	l_pNewLine1->m_strSBM.Copy(line->m_strSBM);	
	CCreateLine* l_pNewLine2 = new CCreateLine(l_cBreakPos,line->m_End);
	l_pNewLine2->m_ColorOfLine = line->m_ColorOfLine;//把颜色也定义进去
	l_pNewLine2->m_strCBM.Copy(line->m_strCBM);	
	l_pNewLine2->m_strSBM.Copy(line->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewLine1);//然后擦入向后移一下
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewLine2);//然后擦入向后移一下
	//把该结点移除
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewLine2);
	AddUndoList((LONG)l_pNewLine1,(LONG)l_pNewLine2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();

	return TRUE;
}

/*
 *	用一个点打断不闭合多线段
 */
BOOL CDrawBreakTwo::BreakPolyline(Position l_cPos,BOOL FlagMove/*FlagMove == Falsh*/)
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
	//多段线里的点的个数
	int l_iCountOfPoint = Polyline->m_point.GetSize();
	
	Position l_cPolyBgPos,l_cPolyEnPos;
	l_cPolyBgPos = Polyline->m_point.GetAt(0) ;
	l_cPolyEnPos = Polyline->m_point.GetAt( l_iCountOfPoint-1 );
	//如果点在不闭合的多段线的两端，
	if(l_cPos==l_cPolyBgPos||l_cPos==l_cPolyEnPos)
	{		
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点到起点或末点无效!请重新点击：",2);						
		else
			RemindContent("\r\nPoints to the beginning or the end of the invalid! Please click again：");			
		return FALSE;		
	}	

	//以下是怕垂足在直线的延长线上
	Position l_cBegin,l_cEnd,l_cBreakPos;
	BOOL l_bFlagBelongBox = FALSE;
	double l_dTudu = 0.0;
	//建立两个新的点集，把多段线分开
	CArray<Position,Position&> l_point1,l_point2,l_point;
	CArray<double,double&> l_tudu1,l_tudu2,l_tudu;
	l_tudu.Copy(Polyline->m_dTuDu);
	l_point.Copy(Polyline->m_point);
	for(int j=0;j<l_iCountOfPoint-1;j++)
	{
		l_cBegin = Polyline->m_point.GetAt(j);
		l_cEnd = Polyline->m_point.GetAt(j+1);
		l_dTudu = Polyline->m_dTuDu.GetAt(j);
		//如果是直线并且在包围盒里
		if(l_dTudu == 0.0&&PosBelongBox(l_cBegin,l_cEnd,l_cPos))//如果是直线
		{
			if(PutLineThree(l_cBegin,l_cEnd,l_cBreakPos,l_cPos,FALSE))//如果点到直线的距离可以返回这三个点
			{	//返回两个点集和两个凸度集
				PutArrayTwo(j,l_point,l_tudu,l_point1,l_point2,l_tudu1,l_tudu2,l_cBreakPos);
				l_bFlagBelongBox = TRUE;
				break;
			}
		}
		else if(l_dTudu != 0.0&&IsClickArc(l_cBegin,l_cEnd,l_cPos,l_cBreakPos,l_dTudu))//是否在圆弧里，并反回断点
		{
			PutArrayTwo(j,l_point,l_tudu,l_point1,l_point2,l_tudu1,l_tudu2,l_cBreakPos);
			l_bFlagBelongBox = TRUE;
			break;				
		}
	}
	//没有点中，或点到头或尾上了，如果是鼠标移动中，就不向下进行了，下面是把点分配
	if(l_bFlagBelongBox == FALSE||FlagMove==TRUE)
		return FALSE;

	//以下是向链表中元素		
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//建立两个新的直线
	CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(l_point1,l_tudu1);
	l_pNewPolyline1->m_ColorOfLine = Polyline->m_ColorOfLine;//把颜色也定义进去
	l_pNewPolyline1->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline1->m_strSBM.Copy(Polyline->m_strSBM);	
	CCreatePolyline* l_pNewPolyline2 = new CCreatePolyline(l_point2,l_tudu2);		
	l_pNewPolyline2->m_ColorOfLine = Polyline->m_ColorOfLine;//把颜色也定义进去
	l_pNewPolyline2->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline2->m_strSBM.Copy(Polyline->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline1);//然后擦入向后移一下
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline2);//然后擦入向后移一下
	//删除双击选中的，因为它已经变成了两个了			//把该结点移除，
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//向撤消链表里添加操作步骤
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewPolyline2);
	AddUndoList((LONG)l_pNewPolyline1,(LONG)l_pNewPolyline2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();
	
	return TRUE;
}

/*
 *	//把直线通过点击分成三个点
 */	
BOOL CDrawBreakTwo::PutLineThree(Position begin,Position end,Position &breakpos,Position clickpos,BOOL FlagClose)
{
	
	//如果与起点或终点重合返回
	Position l_cPolyBgPos,l_cPolyEnPos;
	if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
		l_cPolyBgPos = Polyline->m_point.GetAt(0) ;
		l_cPolyEnPos = Polyline->m_point.GetAt( Polyline->m_point.GetSize()-1 );
	}
	else if(m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		CCreateRectangle *Polyline = (CCreateRectangle*)m_pCreateBase;
		l_cPolyBgPos = Polyline->m_point.GetAt(0) ;
		l_cPolyEnPos = Polyline->m_point.GetAt( Polyline->m_point.GetSize()-1 );
	}
	else if(m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		CCreateEllipse *Polyline = (CCreateEllipse*)m_pCreateBase;
		l_cPolyBgPos = Polyline->m_point.GetAt(0) ;
		l_cPolyEnPos = Polyline->m_point.GetAt( Polyline->m_point.GetSize()-1 );
	}

	//如果点在不闭合的多段线的两端，
	if((clickpos==l_cPolyBgPos||clickpos==l_cPolyEnPos)&&FlagClose == FALSE)
	{		
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点到起点或末点无效!请重新点击：",2);						
		else
			RemindContent("\r\nPoints to the beginning or the end of the invalid! Please click again：");			
		return FALSE;		
	}
	//返回直线上垂足的点
	breakpos = clickpos.DistancePosFoot(begin,end);
	//如果点到线的距离太远，不认为点到线上了
	if(PosOnLine(begin,end,breakpos))
		return TRUE;
	return FALSE;
}

//根据断点号把点集分成两个
BOOL CDrawBreakTwo::PutArrayTwo(int num,CArray<Position,Position&> &l_point,CArray<double,double&> &tudu,
								CArray<Position,Position&> &l_point1,CArray<Position,Position&> &l_point2,
								CArray<double,double&> &tudu1,CArray<double,double&> &tudu2,Position breakpos)
{
	Position l_posBegin(0,0),l_posEnd(0,0);
	double l_dTuDu = 0.0;
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
	//获得点集的大小
	int l_iSizeOfPoint = l_point.GetSize();
	//把断点前面加入点集一
	for(int i = 0;i<=num;i++)
	{
		if(i == num)//加最后一点时凸度不一样
		{
			l_posBegin = l_point.GetAt(i);
			l_posEnd = l_point.GetAt(i+1);
			l_point1.Add(l_posBegin);
			l_dTuDu = tudu.GetAt(i);
			if(l_dTuDu == 0.0)	//是直线的分点集法
				tudu1.Add(l_dTuDu);
			else//是圆弧的分点集法
			{
				Position l_posCenter;
				g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTuDu,l_posCenter);
				double l_dTuDu1 =  g_pDoc->RtnTudu(l_posBegin,breakpos,l_posCenter,RtnArcDir(l_dTuDu));
				tudu1.Add(l_dTuDu1);
			}
			break;
		}
		l_posBegin = l_point.GetAt(i);
		l_point1.Add(l_posBegin);
		l_dTuDu = tudu.GetAt(i);
		tudu1.Add(l_dTuDu);
	}
	//断点两边的点
	l_posBegin = l_point.GetAt(num);
	l_posEnd = l_point.GetAt(num+1);
	//如果上一点与断点相等，把这点加到下一个点集里
	if(PosEqual(l_posBegin,breakpos))
	{
		l_point2.Add(l_posBegin);
		l_dTuDu = tudu.GetAt(num);
		tudu2.Add(l_dTuDu);
	}
	//如果后一点与断点相等，把后一点加到前一点集里
	else if(PosEqual(l_posEnd,breakpos))
	{
		l_point1.Add(l_posEnd);
		l_dTuDu = tudu.GetAt(num+1);
		tudu1.Add(l_dTuDu);
	}
	else//否则把断点加到两边
	{
		l_posBegin = l_point.GetAt(num);
		l_posEnd = l_point.GetAt(num+1);
		l_dTuDu = tudu.GetAt(num);
		//把断点加入
		l_point1.Add(breakpos);				
		l_point2.Add(breakpos);	
		//把凸度加入
		if(l_dTuDu == 0.0)	//是直线的分点集法
			tudu2.Add(l_dTuDu);
		else//是圆弧的分点集法
		{
			Position l_posCenter;
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTuDu,l_posCenter);
			double l_dTuDu2 =  g_pDoc->RtnTudu(breakpos,l_posEnd,l_posCenter,RtnArcDir(l_dTuDu));
			tudu2.Add(l_dTuDu2);
		}
		l_dTuDu = 0.0;
		tudu1.Add(l_dTuDu);
	}
	//把断点后面加入点集二
	for(int j = num+1;j<l_iSizeOfPoint;j++)
	{
		l_posBegin = l_point.GetAt(j);
		l_point2.Add(l_posBegin);
		l_dTuDu = tudu.GetAt(j);
		tudu2.Add(l_dTuDu);
	}
	return TRUE;
}

/*
 *	打断多线断里的圆弧
 */
BOOL CDrawBreakTwo::IsClickArc(Position begin,Position end,Position clickpos,Position &breakpos,double tudu)
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
	//求中心点
	Position l_posCenter;
	g_pDoc->polylineToArc(begin,end,tudu,l_posCenter);
	//建立一个新圆弧
	CCreateRoundArc *pTempArc = new CCreateRoundArc(begin,l_posCenter,end,RtnArcDir(tudu));
	
	CPoint pt;
	g_pView->WorldToScreem(clickpos,pt);
	if(pTempArc->IsPick(pt) == -1)
	{
		delete pTempArc;
		return FALSE;
	}
	//两个点与圆心的角度
	double l_dRadius = pTempArc->m_dRadius;
	double Angle = GetAngleToXAxis(l_posCenter,clickpos);
	//求在圆弧上的断点
	breakpos.x = l_dRadius*cos(Angle)+pTempArc->m_Center.x;
	breakpos.y = l_dRadius*sin(Angle)+pTempArc->m_Center.y;
/*
	//如果点击点与开始点相近认为是一个点
	if(breakpos.Distance(begin)/g_pView->ReturnProportion()<=PRECISION)
		m_posWillClick = breakpos = begin;
	//如果点击点与结束点相近认为是一个点
	else if(breakpos.Distance(end)/g_pView->ReturnProportion()<=PRECISION)
		m_posWillClick = breakpos = end;
*/
	delete pTempArc;
	return TRUE;
}
/*
 *	用两个点打断闭合多线段
 */
BOOL CDrawBreakTwo::BreakPolyline(Position l_cPos1,Position l_cPos2,BOOL FlagMove /* = FALSE */)
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
	CArray<Position,Position&> l_point;
	CArray<double,double&> tudu;
	l_point.Copy(Polyline->m_point);
	tudu.Copy(Polyline->m_dTuDu);
	//建立两个新的点集，把多段线分开
	CArray<Position,Position&> l_point1,l_point2;
	CArray<double,double&> l_tudu1,l_tudu2;
	//把点的链表根据两点分成两个点集
	if(!BreakClose(l_point,tudu,l_point1,l_point2,l_tudu1,l_tudu2,l_cPos1,l_cPos2 ))
		return FALSE;
	//如果是鼠标移动中，就不向下进行了，下面是把点分配
	if (FlagMove == TRUE)
		return FALSE;
	//以下是向链表中元素
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//建立两个新的直线
	CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(l_point1,l_tudu1);
	l_pNewPolyline1->m_ColorOfLine = Polyline->m_ColorOfLine;//把颜色也定义进去
	l_pNewPolyline1->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline1->m_strSBM.Copy(Polyline->m_strSBM);	
	CCreatePolyline* l_pNewPolyline2 = new CCreatePolyline(l_point2,l_tudu2);		
	l_pNewPolyline2->m_ColorOfLine = Polyline->m_ColorOfLine;//把颜色也定义进去
	l_pNewPolyline2->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline2->m_strSBM.Copy(Polyline->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline1);//然后擦入向后移一下
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline2);//然后擦入向后移一下
	//删除双击选中的，因为它已经变成了两个了//把该结点移除，			
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//向撤消链表里添加操作步骤
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewPolyline2);
	AddUndoList((LONG)l_pNewPolyline1,(LONG)l_pNewPolyline2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();	
	return TRUE;
}

/*
 *	用两个两点打断矩形
 */
BOOL CDrawBreakTwo::BreakRect(Position l_cPos1,Position l_cPos2,BOOL FlagMove /* = FALSE */)
{
	CCreateRectangle *Polyline = (CCreateRectangle*)m_pCreateBase;

	CArray<Position,Position&> l_point;
	CArray<double,double&> tudu;
	l_point.Copy(Polyline->m_point);
	tudu.Copy(Polyline->m_dTuDu);
	//建立两个新的点集，把多段线分开
	CArray<Position,Position&> l_point1,l_point2;
	CArray<double,double&> l_tudu1,l_tudu2;
	//把点的链表根据两点分成两个点集
	if(!BreakClose(l_point,tudu,l_point1,l_point2,l_tudu1,l_tudu2,l_cPos1,l_cPos2 ))
		return FALSE;
	//如果是鼠标移动中，就不向下进行了，下面是把点分配
	if (FlagMove == TRUE)
		return FALSE;
	//以下是向链表中元素
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//建立两个新的直线
	CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(l_point1,l_tudu1);
	l_pNewPolyline1->m_ColorOfLine = Polyline->m_ColorOfLine;//把颜色也定义进去
	l_pNewPolyline1->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline1->m_strSBM.Copy(Polyline->m_strSBM);	
	CCreatePolyline* l_pNewPolyline2 = new CCreatePolyline(l_point2,l_tudu2);		
	l_pNewPolyline2->m_ColorOfLine = Polyline->m_ColorOfLine;//把颜色也定义进去
	l_pNewPolyline2->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline2->m_strSBM.Copy(Polyline->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline1);//然后擦入向后移一下
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline2);//然后擦入向后移一下
	//删除双击选中的，因为它已经变成了两个了//把该结点移除，			
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//向撤消链表里添加操作步骤
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewPolyline2);
	AddUndoList((LONG)l_pNewPolyline1,(LONG)l_pNewPolyline2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();		
	return TRUE;
}

/*
 *	用两个两点打断圆
 */
BOOL CDrawBreakTwo::BreakRound(Position l_cPos1,Position l_cPos2)
{
	CCreateRound *Round = (CCreateRound*)m_pCreateBase;
	//以下两点看是否点到了圆上
	CPoint pt1,pt2;
	g_pView->WorldToScreem(l_cPos1,pt1);
	g_pView->WorldToScreem(l_cPos2,pt2);
	if(Round->IsPick(pt1)==-1&&Round->IsPick(pt2)==-1)
		return FALSE;
	//在圆上的两个切割点和圆心点
	Position PosInRound1,PosInRound2,PosCenter;
	PosCenter = Round->m_Center;
	//两个点与圆心的角度
	double l_dRadius = Round->m_dRadius;
	double Angle1 = GetAngleToXAxis(PosCenter,l_cPos1);
	double Angle2 = GetAngleToXAxis(PosCenter,l_cPos2);
	PosInRound1.x = l_dRadius*cos(Angle1)+Round->m_Center.x;
	PosInRound1.y = l_dRadius*sin(Angle1)+Round->m_Center.y;
	PosInRound2.x = l_dRadius*cos(Angle2)+Round->m_Center.x;
	PosInRound2.y = l_dRadius*sin(Angle2)+Round->m_Center.y;
	//圆的方向
	int l_iDirect = Round->m_iDirect;
	
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//建立两个新的直线
	CCreateRoundArc* l_pNewArc1 = new CCreateRoundArc(PosInRound1,PosCenter,PosInRound2,l_iDirect);
	l_pNewArc1->m_ColorOfLine = Round->m_ColorOfLine;//把颜色也定义进去
	l_pNewArc1->m_strCBM.Copy(Round->m_strCBM);	
	l_pNewArc1->m_strSBM.Copy(Round->m_strSBM);	
	CCreateRoundArc* l_pNewArc2 = new CCreateRoundArc(PosInRound2,PosCenter,PosInRound1,l_iDirect);	
	l_pNewArc2->m_ColorOfLine = Round->m_ColorOfLine;//把颜色也定义进去
	l_pNewArc2->m_strCBM.Copy(Round->m_strCBM);	
	l_pNewArc2->m_strSBM.Copy(Round->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewArc1);//然后擦入向后移一下
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewArc2);//然后擦入向后移一下
	//删除双击选中的，因为它已经变成了两个了
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//向撤消链表里添加操作步骤
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewArc2);
	AddUndoList((LONG)l_pNewArc1,(LONG)l_pNewArc2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();
	return TRUE;
}

/*
 *	用两个两点打断圆弧
 */
BOOL CDrawBreakTwo::BreakArc(Position l_cPos)
{
	CCreateRoundArc *RoundArc = (CCreateRoundArc*)m_pCreateBase;
	//以下看点是否点到了圆上
	CPoint pt;
	g_pView->WorldToScreem(l_cPos,pt);
	if(-1 == RoundArc->IsPick(pt))
		return FALSE;
	if (l_cPos.Distance(RoundArc->m_Begin)/g_pView->ReturnProportion()<=PRECISION||l_cPos.Distance(RoundArc->m_End)/g_pView->ReturnProportion()<=PRECISION)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点到起点或末点无效!请重新点击：",2);						
		else
			RemindContent("\r\nPoints to the beginning or the end of the invalid! Please click again：");			
		return FALSE;
	}
	//在圆上的两个切割点和圆心点
	Position PosInRound,PosCenter;
	PosCenter = RoundArc->m_Center;
	//两个点与圆心的角度
	double l_dRadius = RoundArc->m_dRadius;
	double Angle = GetAngleToXAxis(PosCenter,l_cPos);
	PosInRound.x = l_dRadius*cos(Angle)+RoundArc->m_Center.x;
	PosInRound.y = l_dRadius*sin(Angle)+RoundArc->m_Center.y;
	//圆的方向
	int l_iDirect = RoundArc->m_iDirect;
	
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//建立两个新的直线
	CCreateRoundArc* l_pNewArc1 = new CCreateRoundArc(RoundArc->m_Begin,PosCenter,PosInRound,l_iDirect);
	l_pNewArc1->m_ColorOfLine = RoundArc->m_ColorOfLine;//把颜色也定义进去
	l_pNewArc1->m_strCBM.Copy(RoundArc->m_strCBM);	
	l_pNewArc1->m_strSBM.Copy(RoundArc->m_strSBM);	
	CCreateRoundArc* l_pNewArc2 = new CCreateRoundArc(PosInRound,PosCenter,RoundArc->m_End,l_iDirect);
	l_pNewArc2->m_ColorOfLine = RoundArc->m_ColorOfLine;//把颜色也定义进去
	l_pNewArc2->m_strCBM.Copy(RoundArc->m_strCBM);	
	l_pNewArc2->m_strSBM.Copy(RoundArc->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewArc1);//然后擦入向后移一下
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewArc2);//然后擦入向后移一下
	//删除双击选中的，因为它已经变成了两个了//把该结点移除，			
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//向撤消链表里添加操作步骤
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewArc2);
	AddUndoList((LONG)l_pNewArc1,(LONG)l_pNewArc2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();
	return TRUE;
}


/*
 *	用两个两点打断椭圆
 */
BOOL CDrawBreakTwo::BreakEllipse(Position l_cPos1,Position l_cPos2,BOOL FlagMove /* = FALSE */)
{
	CCreateEllipse *Polyline = (CCreateEllipse*)m_pCreateBase;

	CArray<Position,Position&> l_point;
	CArray<double,double&> tudu;
	l_point.Copy(Polyline->m_point);
	tudu.Copy(Polyline->m_dTuDu);
	//建立两个新的点集，把多段线分开
	CArray<Position,Position&> l_point1,l_point2;
	CArray<double,double&> l_tudu1,l_tudu2;
	//把点的链表根据两点分成两个点集
	if(!BreakClose(l_point,tudu,l_point1,l_point2,l_tudu1,l_tudu2,l_cPos1,l_cPos2 ))
		return FALSE;
	//如果是鼠标移动中，就不向下进行了，下面是把点分配
	if (FlagMove == TRUE)
		return FALSE;
	//以下是向链表中元素
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//建立两个新的直线
	CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(l_point1,l_tudu1);
	l_pNewPolyline1->m_ColorOfLine = Polyline->m_ColorOfLine;//把颜色也定义进去
	l_pNewPolyline1->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline1->m_strSBM.Copy(Polyline->m_strSBM);	
	CCreatePolyline* l_pNewPolyline2 = new CCreatePolyline(l_point2,l_tudu2);		
	l_pNewPolyline2->m_ColorOfLine = Polyline->m_ColorOfLine;//把颜色也定义进去
	l_pNewPolyline2->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline2->m_strSBM.Copy(Polyline->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline1);//然后擦入向后移一下
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline2);//然后擦入向后移一下
	//删除双击选中的，因为它已经变成了两个了//把该结点移除，			
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//向撤消链表里添加操作步骤
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewPolyline2);
	AddUndoList((LONG)l_pNewPolyline1,(LONG)l_pNewPolyline2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();	
	return TRUE;
}

/*
 *	把一个闭合的点集（闭合的多线段）根据两个点分成两个点集
 *  先把闭合的点集用第一个打断点换成以断点为起点的闭合图形，
 *	然后再打断成两个
 */
BOOL CDrawBreakTwo::BreakClose(CArray<Position,Position&> &l_point,CArray<double,double&> &tudu,
				CArray<Position,Position&> &l_point1, CArray<Position,Position&> &l_point2,
				CArray<double,double&> &tudu1,CArray<double,double&> &tudu2,Position l_cPos1,Position l_cPos2 )
 {
	//每个直线的两个端点              和两个断点         弧心
	Position  l_cBegin,l_cEnd,l_cBreakPos1,l_cBreakPos2,l_posCenter,p1,p2;
	double l_dTudu=0.0;
	//两个点分别点到链表中的第几个线段//看断点是否和其它 的点重合 
	int  l_iNumPos1=-1,l_iNumPos2=-1;
	//多段线里的点的个数
	int l_iCountOfPoint = l_point.GetSize();
	//找出在多段线上的两个点
	for(int j=0;j<l_iCountOfPoint-1;j++)
	{
		p1 = l_cBegin = l_point.GetAt(j);
		p2 = l_cEnd = l_point.GetAt(j+1);
		l_dTudu = tudu.GetAt(j);
		//第一点是否在每个直线的包围盒里,点到直线距离是否近，直线的，成立返回断点
		if(l_dTudu == 0.0&&PosBelongBox(l_cBegin,l_cEnd,l_cPos1)&&\
			PutLineThree(l_cBegin,l_cEnd,l_cBreakPos1,l_cPos1,TRUE))
		{
			l_iNumPos1=j;	
			break;
		}
		//圆弧的，成功能返回在线上的断点
		else if(l_dTudu != 0.0&&IsClickArc(l_cBegin,l_cEnd,l_cPos1,l_cBreakPos1,l_dTudu))
		{
			l_iNumPos1=j;
			break;
		}
	}
	//没有拾取到线上
	if (l_iNumPos1<0)
		return FALSE;
//////////////把多段线起点换到断点1////////////////////////////////
	l_dTudu = tudu.GetAt(l_iNumPos1);
	//起点与断点在一个点上时（只有点到起上了时）
	if(PosEqual(l_cBegin,l_cBreakPos1))
	{
		l_point1.Copy(l_point);
		tudu1.Copy(tudu);
/*
		for(int a = l_iNumPos1;;)
		{
			l_cBegin = l_point.GetAt(a);
			l_dTudu = tudu.GetAt(a);
			l_point1.Add(l_cBegin);
			tudu1.Add(l_dTudu);
			a++;
			if(a == l_iNumPos1)
			{
				l_cBegin = l_point.GetAt(a);
				l_dTudu = 0.0;
				l_point1.Add(l_cBegin);
				tudu1.Add(l_dTudu);
				break;
			}
			if (a == l_iCountOfPoint-1)a=0;
		}
*/
	}
	//如果后一点与断点相等，把后一点加到前一点集里
	else if(PosEqual(l_cEnd,l_cBreakPos1))
	{
		for(int a = l_iNumPos1+1;;)
		{
			l_cBegin = l_point.GetAt(a);
			l_dTudu = tudu.GetAt(a);
			l_point1.Add(l_cBegin);
			tudu1.Add(l_dTudu);
			a++;
			if(a == l_iNumPos1+1)
			{
				l_cBegin = l_point.GetAt(a);
				l_dTudu = 0.0;
				l_point1.Add(l_cBegin);
				tudu1.Add(l_dTudu);
				break;
			}
			if (a == l_iCountOfPoint-1)a=0;
		}
	}
	else//否则把断点加到两边
	{
		double l_dTudu1=0.0;
		l_cBegin = l_point.GetAt(l_iNumPos1);
		l_cEnd = l_point.GetAt(l_iNumPos1+1);
		l_dTudu = tudu.GetAt(l_iNumPos1);
		//把断点加入
		l_point1.Add(l_cBreakPos1);				
		//把凸度加入
		if(l_dTudu == 0.0)	//是直线的分点集法
		{
			tudu1.Add(l_dTudu);
		}
		else//是圆弧的分点集法
		{
			g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_posCenter);
			l_dTudu1 =  g_pDoc->RtnTudu(l_cBreakPos1,l_cEnd,l_posCenter,RtnArcDir(l_dTudu));
			tudu1.Add(l_dTudu1);
		}
		//、、、、、////
		for(int a = l_iNumPos1+1;;a++)
		{
			if (a == l_iCountOfPoint-1)a=0;
			//
			if(a == l_iNumPos1)
			{
				l_cBegin = l_point.GetAt(l_iNumPos1);
				l_cEnd = l_point.GetAt(l_iNumPos1+1);
				l_dTudu = tudu.GetAt(l_iNumPos1);
				l_point1.Add(l_cBegin);
				if(l_dTudu == 0.0)	//是直线的分点集法
					tudu1.Add(l_dTudu);
				else//是圆弧的分点集法
				{
					g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_posCenter);
					l_dTudu1 =  g_pDoc->RtnTudu(l_cBegin,l_cBreakPos1,l_posCenter,RtnArcDir(l_dTudu));
					tudu1.Add(l_dTudu1);
				}
				l_dTudu = 0.0;
				l_point1.Add(l_cBreakPos1);
				tudu1.Add(l_dTudu);
				break;
			}	
			l_cBegin = l_point.GetAt(a);
			l_dTudu = tudu.GetAt(a);
			l_point1.Add(l_cBegin);
			tudu1.Add(l_dTudu);			

		}
		//、、、/////
	}
	///////////////分成两个点集/////////////////////////
	l_point.RemoveAll();
	l_point.Copy(l_point1);
	l_point1.RemoveAll();
	CArray<double,double&> l_tudu;
	l_tudu.Copy(tudu1);
	tudu1.RemoveAll();
	int l_iCountOfPoint1 = l_point.GetSize();
	for(int b=0;b<l_iCountOfPoint1-1;b++)
	{
		l_cBegin = l_point.GetAt(b);
		l_cEnd = l_point.GetAt(b+1);
		l_dTudu = l_tudu.GetAt(b);
		//如果是直线并且在包围盒里
		if(l_dTudu == 0.0&&PosBelongBox(l_cBegin,l_cEnd,l_cPos2))//如果是直线
		{
			if(PutLineThree(l_cBegin,l_cEnd,l_cBreakPos2,l_cPos2,FALSE))//如果点到直线的距离可以返回这三个点
			{	//返回两个点集和两个凸度集
				PutArrayTwo(b,l_point,l_tudu,l_point1,l_point2,tudu1,tudu2,l_cBreakPos2);
				return TRUE;
			}
		}
		else if(l_dTudu != 0.0&&IsClickArc(l_cBegin,l_cEnd,l_cPos2,l_cBreakPos2,l_dTudu))//是否在圆弧里，并反回断点
		{
			PutArrayTwo(b,l_point,l_tudu,l_point1,l_point2,tudu1,tudu2,l_cBreakPos2);
			return TRUE;				
		}
	}
	return FALSE;
 }

 /*
  *	打断成功
  */
void CDrawBreakTwo::SuccessBreak()
{
	//打断成功后要把“打断”修改清空
	m_iNumOfClick = 0;
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n打断成功:");					
	else
		RemindContent("\r\nInterrupt the success：");			
	
	
}

/*
 *	向撤消链表里添加操作
 */
void CDrawBreakTwo::AddUndoList(LONG CreateBase1,LONG CreateBase2,LONG CreateBase3,POSITION pos)
{
	//向撤消链表里添加操作
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_BREAK_TWO;

	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBaseposSpare1 = CreateBase1;
	pUndoList->pGraphPos->pCreateBaseposSpare2 = CreateBase2;
	pUndoList->pGraphPos->pCreateBasepos = CreateBase3;
	pUndoList->pGraphPos->pListPos = pos;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}

/*
 *	
 */
BOOL CDrawBreakTwo::RtnPickLine(CPoint point,int NumClick)
{
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//获得头指针，在下面循环上用。
	if (NumClick == 1)//点击第一次时
	{
		while(position != NULL)//进入扫描链表
		{
			m_pCreateBase1 = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
			// 如果链表里的图元被选中
			if( m_pCreateBase1->IsPick(point) != -1)
				return TRUE;
			else
				m_pCreateBase1 = NULL;
		}		
	}
	else//点击第二次时
	{
		while(position != NULL)//进入扫描链表
		{
			m_pCreateBase2 = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
			// 如果链表里的图元被选中
			if( m_pCreateBase2->IsPick(point) != -1)
				return TRUE;
			else
				m_pCreateBase2 = NULL;
		}	
	}

	return FALSE;
}
