// DrawParallel.cpp: implementation of the CDrawParallel class.
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

CDrawParallel::CDrawParallel()
:m_Begin(0,0),m_End(0,0)
{
	m_lCreateBasePick = NULL;
	m_dLongLine = 0.0;

}

CDrawParallel::~CDrawParallel()
{

}

int CDrawParallel::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	//如果是第一次输入，并且没有选中图形
	if (m_lCreateBasePick == NULL &&m_iNumOfClick == 0)
	{
 		if(!g_pDoc->ClickCreateBase(l_cPos,m_lCreateBasePick,m_iPick))
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n没有点到直线上:",SEND_RICHEDIT);
			else
				RemindContent("\r\nThere is no point to the line:",SEND_RICHEDIT);					
			return FALSE;
		}
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
			RemindContent("\r\n输入平行线之间的间距:");
		else
			RemindContent("\r\nSpacing between input parallel lines:");					
		break;
	case 2:
		//擦除MOUSEMOVE的最后画的那些
		CCreateBase* l_pEraseLine1 = new CCreateLine(m_Begin,m_End);
		l_pEraseLine1->DrawMove(pDC,dmDrag);
		delete l_pEraseLine1;	
		//看鼠标在图形的左右		
		double l_dlong = m_dLongLine;
		GetVecter(m_End);
		//垂线的角度
		if(nFlags == 2)
			m_dLongLine = l_dlong;
		AddParallel();//出平行后的图形

		g_pDoc->m_CCreateBaseList.AddTail(m_pDragCreatebase);

		m_iNumOfClick = 0;
		m_dLongLine = 0.0;
		m_lCreateBasePick = NULL;
		g_pView->Invalidate();
		//撤消操作
		CUndoRedo l_UndoRedo;//撤消类的对象 
		PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
		pUndoList->OperatieWhat = ID_LINE;
		
		pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
		pUndoList->pGraphPos->pCreateBasepos = (long)m_pDragCreatebase;
		g_pDoc->m_Undo->AddUndoList(pUndoList);
		
		


		break;		
	}
	//把焦点放在命令输入框里
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");	
	g_pView->ReleaseDC(pDC);
	return 0;
}

int CDrawParallel::OnMouseMove(UINT nFlags,const Position &pos)
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
		// 获得鼠标所在的前一个位置
		prePos = m_End; 
		// 得到设备环境指针
		
		// 创建临时对象擦除上一条橡皮线
		CCreateBase*	pTempLine1 = new CCreateLine(m_Begin, prePos);
		if(g_refresh != 1)	//这是为了第一次没有擦除时所做的
			// 当视窗没有被刷新时，重画原来的橡皮线使其被擦除
		{
			pTempLine1->DrawMove(pDC,dmDrag);
			//看鼠标在图形的左右
			GetVecter(prePos);
			//垂线的角度
			AddParallel();
			m_pDragCreatebase->DrawMove(pDC,dmDrag);
			delete m_pDragCreatebase;
		}
		delete pTempLine1;
		//设定捕捉到的点
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);	
		// 创建临时对象，根据当前位置绘制一条橡皮线
		CCreateBase*	pTempLine2 = new CCreateLine(m_Begin, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		//看鼠标在图形的左右
		GetVecter(curPos);
		//垂线的角度
		AddParallel();
		m_pDragCreatebase->DrawMove(pDC,dmDrag);
		delete m_pDragCreatebase;		

		// 将当前位置设置为直线终点，以备下一次鼠标移动时用
		m_End = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	return 0;
}



int CDrawParallel::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//结束画图元//把要画的图元删除
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;	//清除掉选择的图元
	return 0;
}

/*
 *	把图形变为要铣的图形
 */
void CDrawParallel::AddParallel()
{

	CCreateBase* pCreateBase = (CCreateBase*)m_lCreateBasePick;
	if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		TurnLine();
	//多线段//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline *Polyline = (CCreatePolyline*)pCreateBase;
		//如果不是闭合的话 			
		if (!PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))
			TurnOpenPolyline();
		else
			TurnClosePolyline();
	}	
	//矩形//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		TurnClosePolyline();
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		TurnRound();
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		TurnArc();
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		TurnEllipse();	
}

/*
 *	把不闭合多段线变成铣刀需要的图形
 */
void CDrawParallel::TurnOpenPolyline()
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_lCreateBasePick;
	double l_dLongChamfer = m_dLongLine;
	//多段线的点集的大小
	int l_iSize = Polyline->m_point.GetSize()-1; 
	//多段线外括后的两个向量的四个点和    交点      多段线上的三个点
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin;
	//多段线的两个向量的与X轴的角度和凸度
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dAngle90 = MATH_PI/2;	
	//新的点集与凸度
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	if (!m_bIsLeft)
		l_dAngle90 = -l_dAngle90;
	//对所有点处理
	for(int i = 0;i<l_iSize;i++)
	{
		l_posPoly1 = Polyline->m_point.GetAt(i);
		l_posPoly2 = Polyline->m_point.GetAt(i+1);
		//如果加到最后一点，退出循环，往回加。
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAngle90;
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			if (i ==0)//如果多段线就一条直线
			{
				l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAngle90;
				l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
				l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
				l_posBegin = l_pos1;				
				l_aryPoint1.Add(l_pos1);
				l_aryTudu1.Add(l_dTudu);
			}	
			l_aryPoint1.Add(l_pos2);
			l_aryTudu1.Add(l_dTudu);
			break;
		}
		l_posPoly3 = Polyline->m_point.GetAt(i+2);
		//求外括后的四个点
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAngle90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAngle90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
		if (i ==0)//加第一点
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
		}
		//如果不平行，返回交点
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = Polyline->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(Polyline->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(Polyline->m_strSBM);	
}

/*
 *	把闭合的多线段转化 成铣刀需要的图形
 */
void CDrawParallel::TurnClosePolyline()
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_lCreateBasePick;
	double l_dLongChamfer = m_dLongLine;
	//多段线的点集的大小
	int l_iSize = Polyline->m_point.GetSize()-1,i = 0; 
	//多段线外括后的两个向量的四个点和    交点      多段线上的三个点
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin,l_posEnd;
	//多段线的两个向量的与X轴的角度和凸度
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dAgl90 = MATH_PI/2;	
	//它来决定直线新加线在直线左还是右
	//新的点集与凸度
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	if (m_bIsLeft) 	//左边加一圈
	{
		for(i = 0;i<l_iSize;i++)
		{		
			l_posPoly1 = Polyline->m_point.GetAt(i);
			l_posPoly2 = Polyline->m_point.GetAt(i+1);
			//如果加到最后一点，退出循环，并加上最后一点
			if (i+1 == l_iSize) 
			{
				l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
				l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
				l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
				l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
				l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
				//求第一条直线与第二条直线的交点,如果平行
				if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
				{//让首末点重合
					l_aryPoint1.Add(l_pos);
					l_aryTudu1.Add(l_dTudu);
					l_aryPoint1.SetAt(0,l_pos);
				}
				else
				{
					l_aryPoint1.Add(l_posBegin);
					l_aryTudu1.Add(l_dTudu);
				}
				break;
			}
			l_posPoly3 = Polyline->m_point.GetAt(i+2);
			//求外括后的四个点
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
			l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
			l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
			l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
			if (i ==0)//加第一点
			{
				l_aryPoint1.Add(l_pos1);
				l_aryTudu1.Add(l_dTudu);
				l_posBegin = l_pos1;
				l_posEnd = l_pos2;
			}
			//如果不平行，返回交点
			if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
			{
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
			}	
		}
		m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
		m_pDragCreatebase->m_ColorOfLine = Polyline->m_ColorOfLine;
		m_pDragCreatebase->m_strCBM.Copy(Polyline->m_strCBM);	
		m_pDragCreatebase->m_strSBM.Copy(Polyline->m_strSBM);	
		l_aryPoint1.RemoveAll();
		l_aryTudu1.RemoveAll();
	}
	else	//右边加一圈
	{
	l_dAgl90 = -l_dAgl90;
	//加里面一圈
	for(i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = Polyline->m_point.GetAt(i);
		l_posPoly2 = Polyline->m_point.GetAt(i+1);
		//如果加到最后一点，退出循环，并加上最后一点
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//求第一条直线与第二条直线的交点,如果平行
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//让首末点重合
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
				l_aryPoint1.SetAt(0,l_pos);
			}
			else
			{
				l_aryPoint1.Add(l_posBegin);
				l_aryTudu1.Add(l_dTudu);
			}
			break;
		}
		l_posPoly3 = Polyline->m_point.GetAt(i+2);
		//求外括后的四个点
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
		if (i ==0)//加第一点
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//如果不平行，返回交点
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = Polyline->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(Polyline->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(Polyline->m_strSBM);	
	l_aryPoint1.RemoveAll();
	l_aryTudu1.RemoveAll();	
	}
}

/*
 *	把直线变铣刀图形
 */
void CDrawParallel::TurnLine()
{
	CCreateLine *pLine = (CCreateLine*)m_lCreateBasePick;
	double l_dLongChamfer = m_dLongLine;
	//直线外括后的四个点和                   起点	   末点
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_posPoly1,l_posPoly2;
	//多段线的两个向量的与X轴的角度
	double l_dAngle1,l_dAngle2,l_dTudu = 0.0,l_dTudu180 = TUDU_180;	
	//新的点集与凸度
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	l_posPoly1 = pLine->m_Begin;
	l_posPoly2 = pLine->m_End;
	//求外括后的四个点
	l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
	l_dAngle2 = l_dAngle1-MATH_PI;
	l_pos1.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
	l_pos1.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
	l_pos2.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
	l_pos2.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
	l_pos3.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
	l_pos3.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
	l_pos4.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
	l_pos4.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
	if (m_bIsLeft)
	{
		l_aryPoint1.Add(l_pos1);
		l_aryPoint1.Add(l_pos2);
		l_aryTudu1.Add(l_dTudu);
		l_aryTudu1.Add(l_dTudu180);
	}
	else
	{
		l_aryPoint1.Add(l_pos4);
		l_aryPoint1.Add(l_pos3);	
		l_aryTudu1.Add(l_dTudu);
		l_aryTudu1.Add(l_dTudu180);
	}
	m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = pLine->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(pLine->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(pLine->m_strSBM);	
}

/*
 *	把圆弧变铣刀线
 */
void CDrawParallel::TurnArc()
{
	CCreateRoundArc *pArc = (CCreateRoundArc*)m_lCreateBasePick;
	//铣刀宽度
	double l_dLongChamfer = m_dLongLine,l_dTudu180 = -TUDU_180;
	//直线外括后的四个点和                   起点	   末点
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_posPoly1,l_posPoly2;
	l_posPoly1 = pArc->m_Begin;
	l_posPoly2 = pArc->m_End;
	//新的点集与凸度
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	//圆心与点的角度
	double l_dAglBG = GetAngleToXAxis(pArc->m_Center,pArc->m_Begin);
	//圆心与点的角度
	double l_dAglEN = GetAngleToXAxis(pArc->m_Center,pArc->m_End);
	l_pos1.x = l_posPoly1.x - l_dLongChamfer*RetainValid(cos(l_dAglBG),8);
	l_pos1.y = l_posPoly1.y - l_dLongChamfer*RetainValid(sin(l_dAglBG),8);
	l_pos2.x = l_posPoly2.x - l_dLongChamfer*RetainValid(cos(l_dAglEN),8);
	l_pos2.y = l_posPoly2.y - l_dLongChamfer*RetainValid(sin(l_dAglEN),8);
	l_pos3.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAglEN),8);
	l_pos3.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAglEN),8);
	l_pos4.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAglBG),8);
	l_pos4.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAglBG),8);
	//凸度
	double l_dTudu = g_pDoc->RtnTudu(l_posPoly1,l_posPoly2,pArc->m_Center,pArc->m_iDirect),l_dzore = 0.0;
	double l_dfuTudu = -l_dTudu;
	if ((!m_bIsLeft&&pArc->m_iDirect == AD_CLOCKWISE)||(m_bIsLeft&&pArc->m_iDirect == AD_COUNTERCLOCKWISE))
	{
		l_aryPoint1.Add(l_pos1);	
		l_aryTudu1.Add(l_dTudu);	
		l_aryPoint1.Add(l_pos2);	
		l_aryTudu1.Add(l_dTudu180);	
	}
	else
	{
		l_aryPoint1.Add(l_pos4);	
		l_aryTudu1.Add(l_dTudu);	
		l_aryPoint1.Add(l_pos3);	
		l_aryTudu1.Add(l_dTudu180);		
	}
	
	m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = pArc->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(pArc->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(pArc->m_strSBM);	
}

/*
 *	把圆变铣刀线
 */
void CDrawParallel::TurnRound()
{
	CCreateRound *pRound = (CCreateRound*)m_lCreateBasePick;
	//铣刀宽度
	double l_dLongChamfer = m_dLongLine;
	//圆心与末点的角度
	double l_dAgl = GetAngleToXAxis(pRound->m_Center,pRound->m_EndPos);
	//外圆的末点
	Position posOut(pRound->m_Center.x + (l_dLongChamfer+pRound->m_dRadius)*cos(l_dAgl),
					pRound->m_Center.y + (l_dLongChamfer+pRound->m_dRadius)*sin(l_dAgl));
	//内圆的末点	
	Position posIn(pRound->m_Center.x + (-l_dLongChamfer+pRound->m_dRadius)*cos(l_dAgl),
		pRound->m_Center.y + (-l_dLongChamfer+pRound->m_dRadius)*sin(l_dAgl));
	//变铣刀后的圆
	if (m_bIsLeft)
	{
		if (pRound->m_iDirect == AD_CLOCKWISE)
			m_pDragCreatebase = new CCreateRound(pRound->m_Center,posOut,pRound->m_iDirect);			
		else
			m_pDragCreatebase = new CCreateRound(pRound->m_Center,posIn,pRound->m_iDirect);
	}
	else
	{
		if (pRound->m_iDirect == AD_CLOCKWISE)
			m_pDragCreatebase = new CCreateRound(pRound->m_Center,posIn,pRound->m_iDirect);			
		else
			m_pDragCreatebase = new CCreateRound(pRound->m_Center,posOut,pRound->m_iDirect);
		
	}
}

/*
 *	把椭圆变铣刀线
 */
void CDrawParallel::TurnEllipse()
{
	CCreateEllipse *pEllipse = (CCreateEllipse*)m_lCreateBasePick;
	//铣刀宽度
	double l_dLongChamfer = m_dLongLine;
	//多段线的点集的大小
	int l_iSize = pEllipse->m_point.GetSize()-1,i = 0;; 
	//多段线外括后的两个向量的四个点和    交点      多段线上的三个点
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin,l_posEnd;
	//多段线的两个向量的与X轴的角度和凸度
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dAgl90 = MATH_PI/2;	
	//它来决定直线新加线在直线左还是右
	//新的点集与凸度
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	if (m_bIsLeft) 	//左边加一圈
	{
	for(i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = pEllipse->m_point.GetAt(i);
		l_posPoly2 = pEllipse->m_point.GetAt(i+1);
		//如果加到最后一点，退出循环，并加上最后一点
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//求第一条直线与第二条直线的交点,如果平行
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//让首末点重合
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
				l_aryPoint1.SetAt(0,l_pos);
			}
			else
			{
				l_aryPoint1.Add(l_posBegin);
				l_aryTudu1.Add(l_dTudu);
			}
			break;
		}
		l_posPoly3 = pEllipse->m_point.GetAt(i+2);
		//求外括后的四个点
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
		if (i ==0)//加第一点
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//如果不平行，返回交点
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = pEllipse->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(pEllipse->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(pEllipse->m_strSBM);	
	l_aryPoint1.RemoveAll();
	l_aryTudu1.RemoveAll();
	}
	else	//右边加一圈
	{
	l_dAgl90 = -l_dAgl90;
	for(i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = pEllipse->m_point.GetAt(i);
		l_posPoly2 = pEllipse->m_point.GetAt(i+1);
		//如果加到最后一点，退出循环，并加上最后一点
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//求第一条直线与第二条直线的交点,如果平行
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//让首末点重合
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
				l_aryPoint1.SetAt(0,l_pos);
			}
			else
			{
				l_aryPoint1.Add(l_posBegin);
				l_aryTudu1.Add(l_dTudu);
			}
			break;
		}
		l_posPoly3 = pEllipse->m_point.GetAt(i+2);
		//求外括后的四个点
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
		if (i ==0)//加第一点
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//如果不平行，返回交点
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	m_pDragCreatebase = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = pEllipse->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(pEllipse->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(pEllipse->m_strSBM);	
	l_aryPoint1.RemoveAll();
	l_aryTudu1.RemoveAll();	
	}
}

/*
 *	取得点击的两个点，根据这两个点判断鼠标在图形的左右
 */
void CDrawParallel::GetVecter(Position posClick)
{
	Position pos1,pos2;
	if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateLine))) 
	{
		CCreateLine* pLine = (CCreateLine*)m_lCreateBasePick;
		pos1 = pLine->m_Begin;
		pos2 = pLine->m_End;
		m_dLongLine = posClick.PotDisLine(pos1,pos2);
		if(posClick.AnalysisClockDir(pos1,pos2)==AD_COUNTERCLOCKWISE)//如果大于零在左
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
	}
	else if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_lCreateBasePick;
		pos1 = pPolyline->m_point.GetAt(m_iPick);
		pos2 = pPolyline->m_point.GetAt(m_iPick+1);
		m_dLongLine = posClick.PotDisLine(pos1,pos2);
		if(posClick.AnalysisClockDir(pos1,pos2)==AD_COUNTERCLOCKWISE)//如果大于零在左
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
	}
	else if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateRectangle))) 
	{
		CCreateRectangle* pLine = (CCreateRectangle*)m_lCreateBasePick;
		pos1 = pLine->m_point.GetAt(m_iPick);
		pos2 = pLine->m_point.GetAt(m_iPick+1);
		m_dLongLine = posClick.PotDisLine(pos1,pos2);
		if(posClick.AnalysisClockDir(pos1,pos2)==AD_COUNTERCLOCKWISE)//如果大于零在左
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
	}
	else if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateRoundArc))) 
	{
		CCreateRoundArc* pRound = (CCreateRoundArc*)m_lCreateBasePick;
		m_dLongLine = fabs(posClick.Distance(pRound->m_Center) - pRound->m_dRadius);		
		if((posClick.Distance(pRound->m_Center)>=pRound->m_dRadius&&pRound->m_iDirect == AD_CLOCKWISE)||
			(posClick.Distance(pRound->m_Center)<=pRound->m_dRadius&&pRound->m_iDirect == AD_COUNTERCLOCKWISE))
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
	}
	else if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateRound))) 
	{
		CCreateRound* pRound = (CCreateRound*)m_lCreateBasePick;
		m_dLongLine = fabs(posClick.Distance(pRound->m_Center) - pRound->m_dRadius);		
		if((posClick.Distance(pRound->m_Center)>=pRound->m_dRadius&&pRound->m_iDirect == AD_CLOCKWISE)||
			(posClick.Distance(pRound->m_Center)<=pRound->m_dRadius&&pRound->m_iDirect == AD_COUNTERCLOCKWISE))
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
		
	}
	else if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateEllipse))) 
	{
		CCreateEllipse* pLine = (CCreateEllipse*)m_lCreateBasePick;
		pos1 = pLine->m_point.GetAt(m_iPick);
		pos2 = pLine->m_point.GetAt(m_iPick+1);
		m_dLongLine = posClick.PotDisLine(pos1,pos2);
		if(posClick.AnalysisClockDir(pos1,pos2)==AD_COUNTERCLOCKWISE)//如果大于零在左
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
	}
}













