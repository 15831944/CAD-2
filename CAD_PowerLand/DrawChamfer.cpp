// DrawChamfer.cpp: implementation of the CDrawChamfer class.
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

CDrawChamfer::CDrawChamfer()
{
	m_iNumOfClick = 0;
	//倒角半径
	m_dRadius = 0;
	m_bConvex = TRUE;//默认是凸圆
}

CDrawChamfer::~CDrawChamfer()
{

}

int CDrawChamfer::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{	
	Position l_cPos(pos);
	m_inFlags = nFlags;
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);	
	//输入的半径
	if (nFlags == 2)
	{
		if (cmd == "o"||cmd == "O")//凸角
		{
			m_bConvex = TRUE;
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n点击两个线，倒其夹角:");	
			else
				RemindContent("\r\nClick on the two lines, chamfer its Angle：");	
			return 0;
		}
		else if (cmd == "i"||cmd == "I")//凹角
		{
			m_bConvex = FALSE;
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n点击两个线，倒其夹角:");	
			else
				RemindContent("\r\nClick on the two lines, chamfer its Angle：");	
			return 0;
		}
		else
		{
			m_dRadius = atof(cmd);
			if(m_dRadius == 0.0)
				return InvalidContent();
		}
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	}
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
	if(l_bPicked == TRUE||m_iNumOfClick == 2)
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
		}
		else if (m_iNumOfClick == 2)//鼠标点击的第二点
		{
			m_createbaseSecond = l_pCreateBase;
			//如果点击的是两个图形
			if (m_createbaseFirst != m_createbaseSecond)
			{
				//清空选择集
				g_pDoc->RemoveAllCollected();
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n点到的是两个图形，无效！重新选择第一条线：");			
				else
					RemindContent("\r\nPoint to the two graphics, invalid! To choose the first line：");	
				m_iNumOfClick = 0;	
				return FALSE;
			}
			else
			{
				//中文还是英文
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n输入倒角半径:");
				else
					RemindContent("\r\nEnter the chamfering radius:");
			}
			m_iPick2 = l_iPick;//点击到图形上的哪个线断
		}
		else if(m_iNumOfClick ==3)
		{
			m_posClickThree = l_cPos;
			if(ChamferPolyline())
			{
				//把改完的带倒角的图形添加到总链表里
				CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(m_AryPoint,m_aryTudu);
				l_pNewPolyline1->m_ColorOfLine = m_createbaseFirst->m_ColorOfLine;//把颜色也定义进去
				l_pNewPolyline1->m_strCBM.Copy(m_createbaseFirst->m_strCBM);	
				l_pNewPolyline1->m_strSBM.Copy(m_createbaseFirst->m_strSBM);	
				
				POSITION posOld = g_pDoc->m_CCreateBaseList.Find(m_createbaseFirst);
				POSITION posNew = g_pDoc->m_CCreateBaseList.InsertBefore(posOld,l_pNewPolyline1);	
				g_pDoc->RemoveAllCollected();
				g_pDoc->m_CCreateBaseList.RemoveAt(posOld) ;
				//向链表里添加操作
				CUndoRedo l_UndoRedo;//撤消类的对象 
				PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
				pUndoList->OperatieWhat = ID_CHAMFER;
				PGRAPHPOS headGraphPos;
				headGraphPos = l_UndoRedo.NewGraphPos();
				headGraphPos->pCreateBasepos = (LONG)m_createbaseFirst;
				headGraphPos->pListPos = posNew;
				pUndoList->pGraphPos = headGraphPos;
				g_pDoc->m_Undo->AddUndoList(pUndoList);
				m_createbaseFirst =NULL;
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n倒角成功。继续点击两条线倒角或重新选择倒角凹凸角：");			
				else
					RemindContent("\r\nChamfering success. Continue to chamfer or press the ESC key to exit");			
				m_iNumOfClick = 0;	
			}			
		}
	}
	else
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n没有选中，重新选择",2);
		else
			RemindContent("\r\nNot selected, choose again：");	
		return FALSE;
	}
	g_pView->Invalidate();
	g_pView->ReleaseDC(pDC);
	//把焦点放在命令输入框里
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	return TRUE;
}

int CDrawChamfer::OnMouseMove(UINT nFlags,const Position &pos)
{
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	curPos = pos;
	//对象捕捉
	FindPointAll(pDC,m_middle,curPos,g_pView->m_iSwitch);	
	if (m_iNumOfClick == 2)
	{	

		// 获得鼠标所在的前一个位置
		prePos = m_End; 
		// 创建临时对象擦除上一条橡皮线
		m_posClickThree = prePos;
		if(ChamferPolyline())
		{
			CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(m_AryPoint,m_aryTudu);
		
			if(g_refresh != 1)	//这是为了第一次没有擦除时所做的
				// 当视窗没有被刷新时，重画原来的橡皮线使其被擦除
				l_pNewPolyline1->DrawMove(pDC,dmDrag);
			delete l_pNewPolyline1;
		}
		// 创建临时对象，根据当前位置绘制一条橡皮线
		m_posClickThree = curPos;
		if(ChamferPolyline())
		{
			CCreatePolyline* l_pNewPolyline2 = new CCreatePolyline(m_AryPoint,m_aryTudu);
			l_pNewPolyline2->DrawMove(pDC,dmDrag);
			delete l_pNewPolyline2;
		}
		// 释放设备环境指针		
		// 将当前位置设置为直线终点，以备下一次鼠标移动时用
		m_End = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	
	
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));//改变鼠标形状		
	return 0;
}

int CDrawChamfer::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//结束画图元//把要画的图元删除
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	return 0;
}
/*
 *	倒多线段
 */
BOOL CDrawChamfer::ChamferPolyline()
{
	CCreatePolyline* pRect = (CCreatePolyline*)m_createbaseFirst;
	//建立一个新的点集把矩形变成多段线
	CArray<Position,Position&> l_point1,l_point2;
	CArray<double,double&> l_tudu1,l_tudu2;
	//多段线上的三个点							起点       终点		圆心
	Position l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin,l_posEnd,l_posCenter;
	//  ，凸度
	double  l_dTudu = 0.0;
	//倒角是顺时针还是逆时针
	int l_iClock,l_iSize = pRect->m_point.GetSize();
	//如果先点第一个再点第二个
	if (m_iPick1+1 == m_iPick2)
	{
		//先把前几个点加进来
		for(int i=0;i<m_iPick2;i++)
		{
			l_point1.Add(pRect->m_point.GetAt(i));
			l_dTudu = pRect->m_dTuDu.GetAt(i);
			l_tudu1.Add(l_dTudu);
		}
		l_posPoly1 = pRect->m_point.GetAt(i-1);
		l_posPoly2 = pRect->m_point.GetAt(i);
		l_posPoly3 = pRect->m_point.GetAt(i+1);
		//判断第三点在向量的左右
		double l_dRightOrLeft = (l_posPoly2.x - l_posPoly1.x) * (l_posPoly3.y - l_posPoly1.y) - 
					(l_posPoly3.x - l_posPoly1.x) * (l_posPoly2.y - l_posPoly1.y);
		if(l_dRightOrLeft > 0)//点在向量的左边
		{
			l_iClock = AD_COUNTERCLOCKWISE;
		}
		else if(l_dRightOrLeft == 0)//点在向量上
		{
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n错误！两条线平行",2);
			else
				RemindContent("\r\nError! Two lines parallel：");			
			
			m_iNumOfClick = 0;
			return FALSE;
		}	
		else//点在向量的右边
		{
			l_iClock = AD_CLOCKWISE;			
		}
		//返回起点，终点，圆
		GetArcPoint(l_posPoly1,l_posPoly2,l_posPoly3,l_iClock,l_posBegin,l_posEnd,l_posCenter);
		//返回凸度
		l_dTudu = g_pDoc->RtnTudu(l_posBegin,l_posEnd,l_posCenter,l_iClock);
		if(m_bConvex == FALSE)//如果是凹角的话
			l_dTudu = -l_dTudu;
		l_point1.Add(l_posBegin);
		l_tudu1.Add(l_dTudu);
		l_dTudu = 0.0;
		l_point1.Add(l_posEnd);
		l_tudu1.Add(l_dTudu);
		
		for(i=m_iPick2+1;i<l_iSize;i++)
		{
			l_point1.Add(pRect->m_point.GetAt(i));
			l_dTudu = pRect->m_dTuDu.GetAt(i);
			l_tudu1.Add(l_dTudu);
		}
	}
	//先第后二条线，再第一条线
	else if (m_iPick2+1 == m_iPick1)
	{
		double l_dTudu = 0.0;
		for(int i=0;i<m_iPick1;i++)
		{
			l_point1.Add(pRect->m_point.GetAt(i));
			l_dTudu = pRect->m_dTuDu.GetAt(i);
			l_tudu1.Add(l_dTudu);
		}
		l_posPoly1 = pRect->m_point.GetAt(i-1);
		l_posPoly2 = pRect->m_point.GetAt(i);
		l_posPoly3 = pRect->m_point.GetAt(i+1);
		//判断第三点在向量的左右
		double l_dRightOrLeft = (l_posPoly2.x - l_posPoly1.x) * (l_posPoly3.y - l_posPoly1.y) - 
			(l_posPoly3.x - l_posPoly1.x) * (l_posPoly2.y - l_posPoly1.y);
		if(l_dRightOrLeft > 0)//点在向量的左边
		{
			l_iClock = AD_COUNTERCLOCKWISE;
		}
		else if(l_dRightOrLeft == 0)//点在向量上
		{
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n错误！两条线平行",2);
			else
				RemindContent("\r\nError! Two lines parallel：");			
			m_iNumOfClick = 0;
			return FALSE;
		}	
		else//点在向量的右边
		{
			l_iClock = AD_CLOCKWISE;			
		}
		//返回起点，终点，圆
		GetArcPoint(l_posPoly1,l_posPoly2,l_posPoly3,l_iClock,l_posBegin,l_posEnd,l_posCenter);
		//返回凸度
		l_dTudu = g_pDoc->RtnTudu(l_posBegin,l_posEnd,l_posCenter,l_iClock);
		if(m_bConvex == FALSE)//如果是凹角的话
			l_dTudu = -l_dTudu;
		l_point1.Add(l_posBegin);
		l_tudu1.Add(l_dTudu);
		l_dTudu = 0.0;
		l_point1.Add(l_posEnd);
		l_tudu1.Add(l_dTudu);
		
		for(i=m_iPick1+1;i<l_iSize;i++)
		{
			l_point1.Add(pRect->m_point.GetAt(i));
			l_dTudu = pRect->m_dTuDu.GetAt(i);
			l_tudu1.Add(l_dTudu);
		}

	}
	else if ((m_iPick2 == l_iSize-2&&m_iPick1 == 0)||(m_iPick1 == l_iSize-2&&m_iPick2 == 0))//点击的是首尾相接的线
	{
		if(pRect->m_point.GetAt(0) != pRect->m_point.GetAt(l_iSize-1))
		{
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n错误！两条线不相接",2);
			else
				RemindContent("\r\nError! Polyline no close：",2);			
			m_iNumOfClick = 0;
			return FALSE;
			
		}
		l_posPoly1 = pRect->m_point.GetAt(l_iSize-2);
		l_posPoly2 = pRect->m_point.GetAt(0);
		l_posPoly3 = pRect->m_point.GetAt(1);
		//判断第三点在向量的左右
		double l_dRightOrLeft = (l_posPoly2.x - l_posPoly1.x) * (l_posPoly3.y - l_posPoly1.y) - 
			(l_posPoly3.x - l_posPoly1.x) * (l_posPoly2.y - l_posPoly1.y);
		if(l_dRightOrLeft > 0)//点在向量的左边
		{
			l_iClock = AD_COUNTERCLOCKWISE;
		}
		else if(l_dRightOrLeft == 0)//点在向量上
		{
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n错误！两条线平行",2);
			else
				RemindContent("\r\nError! Two lines parallel：",2);			
			m_iNumOfClick = 0;
			return FALSE;
		}	
		else//点在向量的右边
		{
			l_iClock = AD_CLOCKWISE;			
		}
		//返回起点，终点，圆
		GetArcPoint(l_posPoly1,l_posPoly2,l_posPoly3,l_iClock,l_posBegin,l_posEnd,l_posCenter);
		//返回凸度
		l_dTudu = g_pDoc->RtnTudu(l_posBegin,l_posEnd,l_posCenter,l_iClock);
		if(m_bConvex == FALSE)//如果是凹角的话
			l_dTudu = -l_dTudu;
		l_point1.Add(l_posBegin);
		l_tudu1.Add(l_dTudu);
		l_dTudu =0.0;
		l_point1.Add(l_posEnd);
		l_tudu1.Add(l_dTudu);	
		for(int i=1;i<l_iSize-1;i++)
		{
			l_point1.Add(pRect->m_point.GetAt(i));
			l_dTudu = pRect->m_dTuDu.GetAt(i);
			l_tudu1.Add(l_dTudu);
		}
		l_dTudu =0.0;
		l_point1.Add(l_posBegin);
		l_tudu1.Add(l_dTudu);
	}
	else//点击的不是两个相邻的线
	{
		m_iNumOfClick = 0;		
		if (g_pDoc->m_stcWorkFor.IsChinese)
		{	
			RemindContent("\r\n无效，没有点到两个连续的线上",2);
			RemindContent("\r\n重新点击第一条线",1);
		}
		else
		{
			RemindContent("\r\nInvalid, there is no point to two consecutive line",2);
			RemindContent("\r\nClick on the first line again",1);
		}
		//清空选择集
		g_pDoc->RemoveAllCollected();
		return FALSE;
	}
	m_aryTudu.Copy(l_tudu1);
	m_AryPoint.Copy(l_point1);
	return TRUE;
}

/*
 *	得到圆心，起点终点
 */
void CDrawChamfer::GetArcPoint(Position pos1,Position pos2,Position pos3,int clock,Position& begin,Position& end,Position& center)
{
	//求圆心:圆心是两线的交点
	double l_dAngle1,l_dAngle2;
	//过圆心两条线的四个点
	Position l_pos1,l_pos2,l_pos3,l_pos4;
	//哪条直线与X轴平行
	int l_iNumParallel = 0;
	if (clock == AD_CLOCKWISE)//如果是顺时针的圆
	{
		l_dAngle1 = GetAngleToXAxis(pos1,pos2)-MATH_PI/2;
		l_dAngle2 = GetAngleToXAxis(pos2,pos3)-MATH_PI/2;		
	}
	else//否则是逆时针的圆
	{
		l_dAngle1 = GetAngleToXAxis(pos1,pos2)+MATH_PI/2;
		l_dAngle2 = GetAngleToXAxis(pos2,pos3)+MATH_PI/2;
	}
	if(m_inFlags == 2)//输入的半径 
	{
		//两条过圆心的线的四个点求出来了
		l_pos1.x = pos1.x + m_dRadius*cos(l_dAngle1);
		l_pos1.y = pos1.y + m_dRadius*sin(l_dAngle1);
		l_pos2.x = pos2.x + m_dRadius*cos(l_dAngle1);
		l_pos2.y = pos2.y + m_dRadius*sin(l_dAngle1);
		l_pos3.x = pos2.x + m_dRadius*cos(l_dAngle2);
		l_pos3.y = pos2.y + m_dRadius*sin(l_dAngle2);
		l_pos4.x = pos3.x + m_dRadius*cos(l_dAngle2);
		l_pos4.y = pos3.y + m_dRadius*sin(l_dAngle2);		
	}
	else//不输入半径
	{
		double l_dLongPosThree = pos2.Distance(m_posClickThree);//交点与点击点的距离
		double l_dAnglePos1 = GetAngleToXAxis(pos2,pos1);		
		double l_dAnglePos2 = GetAngleToXAxis(pos2,pos3);
		l_pos1.x = pos2.x + l_dLongPosThree*cos(l_dAnglePos1);
		l_pos1.y = pos2.y + l_dLongPosThree*sin(l_dAnglePos1);
		l_pos2.x = l_pos1.x + l_dLongPosThree*cos(l_dAngle1);
		l_pos2.y = l_pos1.y + l_dLongPosThree*sin(l_dAngle1);
		l_pos3.x = pos2.x + l_dLongPosThree*cos(l_dAnglePos2);
		l_pos3.y = pos2.y + l_dLongPosThree*sin(l_dAnglePos2);
		l_pos4.x = l_pos3.x + l_dLongPosThree*cos(l_dAngle2);
		l_pos4.y = l_pos3.y + l_dLongPosThree*sin(l_dAngle2);		
	}
	//两条线的斜率和截距
	double k1,k2,b1,b2;
	if (fabs(l_pos1.x - l_pos2.x ) < DISTANCE_ZERO)//如果第一条线与Y轴平行
	{	
		l_iNumParallel = 1;
		center.x = l_pos1.x;
	}
	else
	{
		//求斜率//求截距
		k1 = (l_pos1.y - l_pos2.y)/(l_pos1.x - l_pos2.x);
		b1 = l_pos1.y - k1*l_pos1.x;
	}
	if (fabs(l_pos3.x - l_pos4.x ) < DISTANCE_ZERO)//如果第二条线与Y轴平行
	{	
		l_iNumParallel = 2;
		center.x = l_pos3.x;
	}
	else
	{
		k2 = (l_pos3.y - l_pos4.y)/(l_pos3.x - l_pos4.x);
		b2 = l_pos3.y - k2*l_pos3.x;
	}
	
	//求圆心点、如果没有线与Y轴平行
	if (l_iNumParallel == 0)
		center.x = (b2-b1)/(k1-k2);		
	//如果第一条线与X轴平行
	if (l_iNumParallel == 1)
		center.y = k2*center.x+b2;
	else
		center.y = k1*center.x+b1;
	//求起点和终点
	begin = center.DistancePosFoot(pos1,pos2);
	end = center.DistancePosFoot(pos2,pos3);

	
}