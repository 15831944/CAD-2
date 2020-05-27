// ModifyRotate.cpp: implementation of the CModifyRotate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "Modify.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModifyRotate::CModifyRotate()
{
	m_iNumOfClick = 0;
	m_bFlagIsDeletePer = FALSE;	
	m_CopyList.RemoveAll();
}

CModifyRotate::~CModifyRotate()
{

}


int CModifyRotate::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	BOOL l_FlagUseAngleInput = FALSE;	//标志角度用输入的;
	double l_dAngle;//旋转的角度
	if (nFlags == 2)	//若是空格或回车就要把输入的数据做为点
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num == -1&&(cmd =="Y"||cmd=="y")&&m_iNumOfClick == 0)
		{
			m_bFlagIsDeletePer = TRUE;
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n已经选择为删除原来图形:");
			else
				RemindContent("\r\nHave chosen to delete the original graphics",2);
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			return 1;			
		}
		else if (l_num == -1&&(cmd =="N"||cmd=="n")&&m_iNumOfClick == 0)
		{
			m_bFlagIsDeletePer = FALSE;
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n已经选择为不删除原来图形",2);
			else
				RemindContent("\r\nHas chosen to not to delete the original graphics",2);
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			return 1;
		}
		if (m_iNumOfClick == 1) //第一次输入方向时进入下面
		{
			l_dAngle = atof(cmd)/180*MATH_PI;
			l_FlagUseAngleInput = TRUE;
		}
		else if(m_iNumOfClick == 0)	//输入为点时
		{
			if(l_num == -1)//输入的内容不对
				return InvalidContent();//向命令提示框输出“无效的命令”
			l_cPos.x = atof(cmd.Left(l_num));	//豆号前的数字
			l_cPos.y = atof(cmd.Mid(l_num+1));	//豆号后的数字
		}
	}
	m_iNumOfClick ++ ;
	g_refresh = 0;
	switch(m_iNumOfClick)
	{
	case 1:
		RemindContent("\r\n指定旋转角度,或按ESC键取消:");
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		// 第一次单击鼠标左键时得到基点位置，并初步设定目标位置
		m_basePos = m_desPos = l_cPos;
		break;
	case 2:
		{
			CDC*	pDC = g_pView->GetDC(); // 获得视类的设备环境指针
			int i, n;
/*
			//删除移动路径的橡皮线
			CCreateLine*	pTempLine2 = new CCreateLine(m_basePos, m_desPos);	
			pTempLine2->DrawMove(pDC,dmDrag);
			delete pTempLine2;
			for(n = g_pDoc->m_CollectOfPicked.GetSize(), i = 0; i < n; i++)
			{
				CCreateBase*	pEntity1 = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
				// 清除上一个位置上绘制的图元
				CCreateBase*	pCopy1 = pEntity1->Copy(); // 得到图元的拷贝
				double angle = ::GetAngleToXAxis(m_basePos, m_desPos);
				pCopy1->Rotate(m_basePos, angle); // 将拷贝移动到上一个位置
				pCopy1->DrawMove(pDC,dmDrag,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum); // 对在上一个位置对拷贝进行重画
				delete pCopy1; // 删除临时拷贝
			}			
*/
	////////////////////////////////////////////////////////////////////////
			m_desPos = l_cPos;
			int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
			for(n = l_iSize, i = 0; i < n; i++)
			{
				CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];

				if(m_bFlagIsDeletePer == FALSE)	//保留原来的图形
				{
					CCreateBase* pCopy = pEntity->Copy();
					if (l_FlagUseAngleInput == FALSE)//如果输入角度了，不是两个点形成的角度
						l_dAngle = GetAngleToXAxis(m_basePos, m_desPos);
					pCopy->Rotate(m_basePos, l_dAngle);
					pCopy->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);
					g_pDoc->m_CCreateBaseList.AddTail(pCopy);
					m_CopyList.AddTail(pCopy);
				}
				else//不保留原来的图形
				{
					pEntity->DrawMove(pDC,dmInvalid,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum);//清除原来的图形
					if (l_FlagUseAngleInput == FALSE)//如果输入角度了，不是两个点形成的角度
						l_dAngle = GetAngleToXAxis(m_basePos, m_desPos);
					pEntity->Rotate(m_basePos, l_dAngle);
				}
				pEntity->DrawMove(pDC,dmNormal,g_pView->m_stcSwitch.bShowArrow,g_pView->m_stcSwitch.bShowNum); // 在目标位置上绘制图元
			}
			//把操作步骤添加到撤消链表里
			AddUndoList();

			g_pDoc->m_CollectOfPicked.RemoveAll(); //  清空选择集
			g_pView->ReleaseDC(pDC); // 释放视类的设备环境指针
			m_iNumOfClick = 0; 
			//结束画图元//把要画的图元删除
			delete g_pView->m_pDrawBase;
			g_pView->m_pDrawBase = NULL;	//设为空，当下次点击鼠标时，会跳到是否会选中。
			g_pView->Invalidate();
			break;
		}
	default:
		break;
	}
	//把焦点放在命令输入框里
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	return 0;
}

int CModifyRotate::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//改变鼠标形状	
	CDC*	pDC = g_pView->GetDC(); 
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	switch(m_iNumOfClick)
	{
	case 0:
		//对象捕捉
		FindPointAll(pDC,pos,pos,g_pView->m_iSwitch);
		break;
	case 1:
		{
			Position	prePos, curPos;
			prePos = m_desPos; // 获得上一个目标位置
			curPos = pos; // 得到当前位置
			//对象捕捉
			FindPointAll(pDC,m_basePos,curPos,g_pView->m_iSwitch);
			
			// 如果在操作过程中窗口没有被刷新，则要清除上一次绘制的橡皮线
			CCreateLine*	pTempLine1 = new CCreateLine(m_basePos, prePos);
			if(g_refresh != 1)	//这是为了第一次没有擦除时所做的
				// 当视窗没有被刷新时，重画原来的橡皮线使其被擦除
				pTempLine1->DrawMove(pDC,dmDrag);
			delete pTempLine1;
			//设定捕捉到的点
			if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
				SetLButtonDownPot(curPos,m_iNumOfClick);
			// 创建临时对象，根据当前位置绘制一条橡皮线
			CCreateLine*	pTempLine2 = new CCreateLine(m_basePos, curPos);	
			pTempLine2->DrawMove(pDC,dmDrag);
			delete pTempLine2;
			// 根据当前位置给出选中图元的实时位置
					
			int i, n;
			for(n = g_pDoc->m_CollectOfPicked.GetSize(), i = 0; i < n; i++)
			{
				CCreateBase*	pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
				
				//		pEntity->DrawMove(pDC,dmSelect);
				// 如果在操作过程中窗口没有被刷新，则要清除上一个位置上绘制的图元
				if(g_refresh != 1)	//这是为了第一次没有擦除时所做的
					// 当视窗没有被刷新时，重画原来的橡皮线使其被擦除
				{
					// 清除上一个位置上绘制的图元
					CCreateBase*	pCopy1 = pEntity->Copy(); // 得到图元的拷贝
					double angle = ::GetAngleToXAxis(m_basePos, prePos);
					pCopy1->Rotate(m_basePos, angle); // 将拷贝移动到上一个位置
					pCopy1->DrawMove(pDC,dmDrag); // 对在上一个位置对拷贝进行重画
					delete pCopy1; // 删除临时拷贝
				}
				// 在当前位置上绘制图元// 
				CCreateBase*	pCopy2 = pEntity->Copy();// 得到图元的拷贝
				double angle = ::GetAngleToXAxis(m_basePos, curPos);
				pCopy2->Rotate(m_basePos, angle); // 将拷贝移动到上一个位置
				pCopy2->DrawMove(pDC,dmDrag);	// 对当前位置绘制拷贝
				delete pCopy2; // 删除临时拷贝 
			}
			m_desPos = curPos; // 将目标设置为当前位置
		}
	default:
		break;
	}		
	g_pView->ReleaseDC(pDC);
	return 0;
}

int CModifyRotate::OnRButtonDown(UINT nFlags,const Position &pos)
{
	
	return 0;
}

/*
 *	添加撤消链表
 */
void CModifyRotate::AddUndoList()
{
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_ROTATE;

	pUndoList->pOperatieParam = l_UndoRedo.NewOperateParam();
	g_pDoc->PosBeEqualed(pUndoList->pOperatieParam->potBase,m_basePos);
	double l_dAngle = GetAngleToXAxis(m_basePos, m_desPos);
	pUndoList->pOperatieParam->angle = l_dAngle;
	pUndoList->pOperatieParam->direction = m_bFlagIsDeletePer;

	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;
	int n,i;
	CCreateBase*	pEntity;
	//当选择保留原图时
	if(m_bFlagIsDeletePer == FALSE)
	{
		POSITION POS = m_CopyList.GetHeadPosition();
		pEntity = (CCreateBase*)m_CopyList.GetNext(POS);		
		CurGraphPos->pCreateBasepos = (long)pEntity;
		while (POS)
		{
			pEntity = (CCreateBase*)m_CopyList.GetNext(POS);		
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (long)pEntity;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;									
		}
		g_pDoc->m_Undo->AddUndoList(pUndoList);
		return;
	}
	for(n = g_pDoc->m_CollectOfPicked.GetSize(), i = 0; i < n; i++)
	{		
		pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];		
		if (i == 0)
			CurGraphPos->pCreateBasepos = (long)pEntity;
		else
		{
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (long)pEntity;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		}							
	}
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}