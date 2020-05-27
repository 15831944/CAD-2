// DrawExtendLine.cpp: implementation of the CDrawExtendLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "Draw.h"
#include "math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawExtendLine::CDrawExtendLine()
{
	m_iNumOfClick = 0;
	m_iExtendWhichTop = 0;
	m_dLongExtend = 0.0;
}

CDrawExtendLine::~CDrawExtendLine()
{

}

/*
 *	当前点与起点近时返回1，当前点与末点近时返回2，与两边一样近时返回0
 */
int CDrawExtendLine::ClosePos(CCreateBase* pCreateBase,Position pos)
{
	double l_dDisBegin,l_dDisEnd;
	//直线//////////////////////////////////////////
	if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine *pLine = (CCreateLine*)pCreateBase;
		l_dDisBegin = pos.Distance(pLine->m_Begin);
		l_dDisEnd = pos.Distance(pLine->m_End);
		if (l_dDisBegin == l_dDisEnd) 
			return 1;
		if (l_dDisBegin < l_dDisEnd) 
			return 1;
		else 
			return 2;
	}	
	//多线段//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
	{
		CCreatePolyline *pPolyLine = (CCreatePolyline*)pCreateBase;
		int l_iSize = pPolyLine->m_point.GetSize();
		Position posBegin = pPolyLine->m_point.GetAt(0);
		Position posEnd = pPolyLine->m_point.GetAt(l_iSize-1);
		l_dDisBegin = pos.Distance(posBegin);
		l_dDisEnd = pos.Distance(posEnd);
		if (l_dDisBegin == l_dDisEnd) 
			return 0;
		if (l_dDisBegin < l_dDisEnd) 
			return 1;
		else 
			return 2;
	}	
	//矩形//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
	}
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{//圆
		return 0;
	}	
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{//圆弧
		return 0;		
	}
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{//椭圆
		return 0;		
	}
	return 0;
}

int CDrawExtendLine::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{	
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	//如果是鼠标点击退出或者没有双击图形
	if(nFlags == 1)
	{
		//如果延长长度为零的话退出
		if (m_dLongExtend == 0.0 )
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n长度不可以为:0");		
			else
				RemindContent("\r\nLength is not zero:");		
			return FALSE;			
		}
		CPoint point;
		g_pView->WorldToScreem(l_cPos,point);
		POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//获得头指针，在下面循环上用。
		while(POS != NULL)//进入扫描链表
		{
			m_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);
			// 如果链表里的图元被选中
			if( m_pCreateBase->IsPick(point) != -1)
			{ 
				//当前点与起点近时返回1，当前点与末点近时返回2，与两边一样近时返回0
				m_iExtendWhichTop = ClosePos(m_pCreateBase,l_cPos);
				//执行把线延长
				ExtendLine(m_pCreateBase,m_dLongExtend,m_iExtendWhichTop);
				return TRUE;
			}
		}		
	}
	if (nFlags == 2)	//若是空格或回车就要把输入的数据做为点
	{	
 		m_dLongExtend = atof(cmd);
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n点击延长或按ESC退出。输入延长长度为:");		
		else
			RemindContent("\r\nClick the extension or press ESC to exit. Extended length of input:");		
	}
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");				
	return TRUE;
}

int CDrawExtendLine::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));//改变鼠标形状	
	//对象捕捉
	CDC*	pDC = g_pView->GetDC(); 
	FindPointAll(pDC,pos,pos,NEAR_POINT);
	g_pView->ReleaseDC(pDC); 
	return 0;
}


int CDrawExtendLine::OnRButtonDown(UINT nFlags,const Position &pos)
{
	
	return 0;
}

/*
 *	对线延长
 */
void CDrawExtendLine::ExtendLine(CCreateBase* pCreateBase,double LongExtend,int IsBegin)
{
	//直线//////////////////////////////////////////
	if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		PutLineEx(pCreateBase,LongExtend,IsBegin);	
	//多线段//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
	{
		CCreatePolyline *Polyline = (CCreatePolyline*)pCreateBase;
		//如果不是闭合的话
		if (!PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))
			PutPolylineEx(pCreateBase,LongExtend,IsBegin);
		else
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n闭合多段线不能延长:");		
			else
				RemindContent("\r\nClosure polyline cannot extend:");											
		}
	}
	
	//矩形//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n矩形不能延长:");		
		else
			RemindContent("\r\nRectangle cannot extend:");		
	}
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{//圆
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n圆不能延长:");		
		else
			RemindContent("\r\nRound cannot extend:");		
	}
	
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{//圆弧
//		SuccessExtend();
	}
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{//椭圆
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n椭圆不能延长:");		
		else
			RemindContent("\r\nCEllipse cannot extend:");		
	}		
}

//把直线延长
void CDrawExtendLine::PutLineEx(CCreateBase* pCreateBase,double LongExtend,int IsBegin)
{
	CCreateLine *Line = (CCreateLine*)pCreateBase;
	m_posEnd = Line->m_End;
	m_posBegin = Line->m_Begin;
	if (LongExtend < 0&&fabs(LongExtend) > m_posEnd.Distance(m_posBegin))
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n超出范围，重新点击或按ESC键取消");
		else
			RemindContent("\r\nBeyond the range, click or press the ESC key to cancel：");
		return;		
	}
	//延长成功
	CCreatePolyline *polyLine = (CCreatePolyline*)pCreateBase;
	AddUndoList(polyLine->m_point,polyLine->m_dTuDu);
	Line = (CCreateLine*)pCreateBase;
	m_posEnd = Line->m_End;
	m_posBegin = Line->m_Begin;
	//看是延长哪个端点
	if (IsBegin == 1)//起点
	{
		m_dAgl = GetAngleToXAxis(m_posBegin,m_posEnd);
		Line->m_Begin.x = Line->m_Begin.x - LongExtend*cos(m_dAgl);
		Line->m_Begin.y = Line->m_Begin.y - LongExtend*sin(m_dAgl);
	}
	else if(IsBegin == 2)//末点
	{
		m_dAgl = GetAngleToXAxis(m_posEnd,m_posBegin);
		Line->m_End.x = Line->m_End.x - LongExtend*cos(m_dAgl);
		Line->m_End.y = Line->m_End.y - LongExtend*sin(m_dAgl);
	}
	SuccessExtend();				
}

/*
*	延长成功
*/
void CDrawExtendLine::SuccessExtend()
{
	//延长成功后要把“延长”修改清空
	m_iNumOfClick = 0;
	g_pView->Invalidate();
	//中文还是英文
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n延长成功。可继续点击延长或按ESC退出:");
	else
		RemindContent("\r\nExtend the success. Can continue to click to extend or press ESC to exit:");		
}

//把多段线延长
void CDrawExtendLine::PutPolylineEx(CCreateBase* pCreateBase,double LongExtend,int IsBegin)
{
 	CCreatePolyline *Line = (CCreatePolyline*)pCreateBase;	
	Position l_posBegin;
	int l_iWhichLine;
	//看是延长哪个端点
	if (IsBegin == 1)//起点
	{
		l_posBegin = Line->m_point.GetAt(0);
		if (LongExtend < 0)//缩短
		{
			//输入的长度返回线上的点，返回假说明输入的长度不在范围内
			if (!Line->RtnLongPos(l_posBegin,-LongExtend,m_posBegin,l_iWhichLine)) 
			{
					//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n超出范围，重新点击或按ESC键取消");
				else
					RemindContent("\r\nBeyond the range, click or press the ESC key to cancel：");
				return;
			}	
			else
			{
				AddUndoList(Line->m_point,Line->m_dTuDu);
				Line->m_point.RemoveAt(0,l_iWhichLine);
				Line->m_point.SetAt(0,m_posBegin);
				//延长成功
				SuccessExtend();				
			}
		}
		else//延伸
		{
			AddUndoList(Line->m_point,Line->m_dTuDu);
			m_posEnd = Line->m_point.GetAt(1);
			m_posBegin = Line->m_point.GetAt(0);
			m_dAgl = GetAngleToXAxis(m_posBegin,m_posEnd);
			m_posBegin.x = m_posBegin.x - LongExtend*cos(m_dAgl);
			m_posBegin.y = m_posBegin.y - LongExtend*sin(m_dAgl);
			Line->m_point.SetAt(0,m_posBegin);
			//延长成功
			SuccessExtend();				
		}
	}
	else if(IsBegin == 2)//末点
	{
		l_posBegin = Line->m_point.GetAt(Line->m_point.GetSize()-1);
		if (LongExtend < 0)//缩短
		{
			//输入的长度返回线上的点，返回假说明输入的长度不在范围内
			if (!Line->RtnLongPos(l_posBegin,LongExtend,m_posEnd,l_iWhichLine)) 
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n超出范围，重新点击或按ESC键取消");
				else
					RemindContent("\r\nBeyond the range, click or press the ESC key to cancel：");
				return;
			}	
			else
			{
				AddUndoList(Line->m_point,Line->m_dTuDu);
				Line->m_point.RemoveAt(l_iWhichLine+1,Line->m_point.GetSize()-2-l_iWhichLine);
				Line->m_point.SetAt(Line->m_point.GetSize()-1,m_posEnd);
				//延长成功
				SuccessExtend();				
			}
		}
		else//延伸
		{
			AddUndoList(Line->m_point,Line->m_dTuDu);
			m_posEnd = Line->m_point.GetAt(Line->m_point.GetSize()-1);
			m_posBegin = Line->m_point.GetAt(Line->m_point.GetSize()-2);
			m_dAgl = GetAngleToXAxis(m_posEnd,m_posBegin);
			m_posEnd.x = m_posEnd.x - LongExtend*cos(m_dAgl);
			m_posEnd.y = m_posEnd.y - LongExtend*sin(m_dAgl);
			Line->m_point.SetAt(Line->m_point.GetSize()-1,m_posEnd);
			//延长成功
			SuccessExtend();				
		}
	}
}

/*
 *	向撤消链表里添加操作
 */
void CDrawExtendLine::AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	m_pUndoList = m_UndoRedo.NewUndoList();
	m_pUndoList->OperatieWhat = ID_IO_KNIFE;
	
	m_pCurGraphPos = m_UndoRedo.NewGraphPos();
	m_pCurGraphPos->pCreateBasepos = (LONG)m_pCreateBase;
	//如果是直线的话
	if(m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*) m_pCreateBase;
		m_pUndoList->pOperatieParam = m_UndoRedo.NewOperateParam();
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potBase,pLine->m_Begin);
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potDes,pLine->m_End);		
	}
	else//多段线
	{
		m_pCurGraphPos->aryPoint.SetSize(aryPoint.GetSize());
		m_pCurGraphPos->aryPoint.Copy(aryPoint);
		m_pCurGraphPos->aryTudu.SetSize(aryTudu.GetSize());
		m_pCurGraphPos->aryTudu.Copy(aryTudu);
	}	
	m_pUndoList->pGraphPos = m_pCurGraphPos;
	g_pDoc->m_Undo->AddUndoList(m_pUndoList);
}





