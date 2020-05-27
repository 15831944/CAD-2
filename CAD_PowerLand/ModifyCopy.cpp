// ModifyCopy.cpp: implementation of the CModifyCopy class.
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

CModifyCopy::CModifyCopy()
{
	m_iNumOfClick =0 ;
}

CModifyCopy::~CModifyCopy()
{

}

int CModifyCopy::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//设定捕捉到的点
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if (nFlags == 2)	//若是空格或回车就要把输入的数据做为点
	{
		int l_num = cmd.FindOneOf(",");
		if(l_num == -1)//输入的内容不对
			return InvalidContent();//向命令提示框输出“无效的命令”
		l_cPos.x = atof(cmd.Left(l_num));	//豆号前的数字
		l_cPos.y = atof(cmd.Mid(l_num+1));	//豆号后的数字
	}
	m_iNumOfClick ++ ;
	g_refresh = 0;
	switch(m_iNumOfClick)
	{
	case 1:
		// 第一次单击鼠标左键时得到基点位置，并初步设定目标位置
		RemindContent("\r\n指定第二个点,或按ESC键取消:");
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
		m_basePos = m_desPos = l_cPos;
		break;
	case 2:
		{
			//中文还是英文
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n完成:");
			else
				RemindContent("\r\nComplete:");		
			((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
			CDC*	pDC = g_pView->GetDC(); // 获得视类的设备环境指针
			//删除移动路径的橡皮线
			CCreateLine*	pTempLine2 = new CCreateLine(m_basePos, m_desPos);	
			pTempLine2->DrawMove(pDC,dmDrag);
			delete pTempLine2;
			m_desPos = l_cPos;

			CUndoRedo l_UndoRedo;//撤消类的对象 
			PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
			pUndoList->OperatieWhat = ID_LINE;
			
			PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
			headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
			pUndoList->pGraphPos = headGraphPos;
			
			
			CCreateBase	*pEntity,*pCopy;
			int i, n;
			for(n = g_pDoc->m_CollectOfPicked.GetSize(), i = 0; i < n; i++)
			{
				pEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i];
				pCopy = pEntity->Copy();	//创建一个新的要复制的图形 
				pCopy->Move(m_basePos, m_desPos);
				pCopy->DrawMove(pDC,dmNormal); // 在目标位置上绘制图元
				g_pDoc->m_CCreateBaseList.AddTail(pCopy);//复制后的图元放到链表里
				pEntity->DrawMove(pDC,dmNormal);	//把原来的图形重画一下，从选中变为不选中的样子
				if (i == 0)
					CurGraphPos->pCreateBasepos = (long)pCopy;
				else
				{
					GraphPos = l_UndoRedo.NewGraphPos();						
					GraphPos->pCreateBasepos = (long)pCopy;
					CurGraphPos->pNext = GraphPos;
					GraphPos->pPrev = CurGraphPos;
					CurGraphPos = GraphPos;
				}
			}
			//把操作步骤添加到撤消链表里
			g_pDoc->m_Undo->AddUndoList(pUndoList);
//			g_pDoc->m_CollectOfPicked.RemoveAll(); //  清空选择集
			g_pView->ReleaseDC(pDC); // 释放视类的设备环境指针
//			m_iNumOfClick = 0; 
			//结束画图元//把要画的图元删除
//			delete g_pView->m_pDrawBase;
//			g_pView->m_pDrawBase = NULL;	//设为空，当下次点击鼠标时，会跳到是否会选中。
			g_pView->Invalidate(TRUE);
			m_iNumOfClick = 1;
			break;
		}
	default:
		break;
	}	
	return 0;
}

int CModifyCopy::OnMouseMove(UINT nFlags,const Position &pos)
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
					pCopy1->Move(m_basePos, prePos); // 将拷贝移动到上一个位置
					pCopy1->DrawMove(pDC,dmDrag); // 对在上一个位置对拷贝进行重画
					delete pCopy1; // 删除临时拷贝
				}
				// 在当前位置上绘制图元
				CCreateBase*	pCopy2 = pEntity->Copy();// 得到图元的拷贝
				pCopy2->Move(m_basePos, curPos); // 将拷贝移动到上一个位置
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

int CModifyCopy::OnRButtonDown(UINT nFlags,const Position &pos)
{	
	//结束画图元//把要画的图元删除
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	g_pDoc->RemoveAllCollected();
	g_pView->Invalidate();
	return 0;
}










