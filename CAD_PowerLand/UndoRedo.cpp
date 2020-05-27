// UndoRedo.cpp: implementation of the CUndoRedo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "UndoRedo.h"
#include "Draw.h"	//��ִ�г����ӳ�ֱ�߹���ʱ���õ��ӳ�ֱ������ķ���

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUndoRedo::CUndoRedo()
{
	m_pHeadList = new UNDOLIST;
	memset(m_pHeadList,0,sizeof(UNDOLIST));
	m_pHeadList->OperatieNum = 0;
	m_pCurrList = NULL;
	m_pTailList = NULL;
}

CUndoRedo::~CUndoRedo()
{
	//��ճ�����ռ�õ��ڴ�
	if (m_pCurrList != NULL)
	{
		ClearUndoList(m_pCurrList->pNext,TRUE);
		m_pCurrList->pNext = NULL;
	}				
	ClearUndoList(m_pHeadList,FALSE);
}

/*
 *	�ͷ�LIST
 */
void CUndoRedo::ClearUndoList(PUNDOLIST pUndoList,BOOL IsDelCreate /* = FALSE */)
{
	PUNDOLIST pUndoListTemp = pUndoList;
	while (pUndoListTemp != NULL)
	{
		PUNDOLIST pUndoListDel = pUndoListTemp;
		
		//�ͷ�OperatieParam
		if (pUndoListDel->pOperatieParam != NULL)
		{
			delete pUndoListDel->pOperatieParam;
			pUndoListDel->pOperatieParam = NULL;
		}
		
		
		//�ͷ�pGraphPos��ͼ���ڳ���������ռ�õ��ڴ�
		while(pUndoListDel->pGraphPos != NULL)
		{
			PGRAPHPOS pGraphPosDel = pUndoListDel->pGraphPos;
			pUndoListDel->pGraphPos = pUndoListDel->pGraphPos->pNext;
			//���������Ϊ��ֻ�ͷ���ͼ�����ͼ�� �� �����ͷ����飬
			if (pUndoListDel->OperatieWhat == ID_LINE&&IsDelCreate == TRUE)
			{
				CCreateBase* pCreateBasePos = (CCreateBase*)pGraphPosDel->pCreateBasepos;
				delete pCreateBasePos;
				pCreateBasePos = NULL;
			}
			//ɾ������
			else if (pUndoListDel->OperatieWhat == ID_DELETE&&IsDelCreate == FALSE)
			{
				CCreateBase* pCreateBasePos = (CCreateBase*)pGraphPosDel->pCreateBasepos;
				delete pCreateBasePos;
				pCreateBasePos = NULL;
			}
			//���Ǻͺϲ�����
			else if (pUndoListDel->OperatieWhat == ID_CHAMFER||pUndoListDel->OperatieWhat == ID_TOGETHER)
			{
				CCreateBase* pCreateBasePos = (CCreateBase*)pGraphPosDel->pCreateBasepos;
				delete pCreateBasePos;
				pCreateBasePos = NULL;
			}
			//��ϲ���
			else if (pUndoListDel->OperatieWhat == ID_BREAK_TWO||pUndoListDel->OperatieWhat == ID_MILL_GROOVES)
			{
				if (IsDelCreate == FALSE)
				{
					CCreateBase* pCreateBasePos = (CCreateBase*)pGraphPosDel->pCreateBasepos;
					delete pCreateBasePos;
					pCreateBasePos = NULL;
				}
				else
				{
					CCreateBase* pCreateBasePos1 = (CCreateBase*)pGraphPosDel->pCreateBaseposSpare1;
					delete pCreateBasePos1;
					pCreateBasePos1 = NULL;	
					if (pGraphPosDel->pCreateBaseposSpare2)
					{
						CCreateBase* pCreateBasePos2 = (CCreateBase*)pGraphPosDel->pCreateBaseposSpare2;
						delete pCreateBasePos2;
						pCreateBasePos1 = NULL;	
					}		
				}
			}
			//ĥ�߲���
			else if (pUndoListDel->OperatieWhat == ID_GRIND_GLASS||pUndoListDel->OperatieWhat == ID_SOLID_TO_DASH)
			{
				if (IsDelCreate == FALSE)
				{
					CCreateBase* pCreateBasePos = (CCreateBase*)pGraphPosDel->pCreateBasepos;
					delete pCreateBasePos;
					pCreateBasePos = NULL;
				}
			}
			//���в���
			else if (pUndoListDel->OperatieWhat == ID_OVER_CUT)
			{
				if (IsDelCreate == FALSE)
				{
					CCreateBase* pCreateBasePos1 = (CCreateBase*)pGraphPosDel->pCreateBaseposSpare1;
					if (pCreateBasePos1)
					{
						delete pCreateBasePos1;
						pCreateBasePos1 = NULL;	
					}
				}
				else
				{
					CCreateBase* pCreateBasePos = (CCreateBase*)pGraphPosDel->pCreateBasepos;
					delete pCreateBasePos;
					pCreateBasePos = NULL;			
				}
			}
			//ϳ������
			else if (pUndoListDel->OperatieWhat == ID_IO_KNIFE)
			{
				if (pGraphPosDel->pCreateBaseposSpare1)//˵���Ǳպϵ�ͼ�Σ���Ҫ���ڴ�
				{
					if (IsDelCreate == FALSE)
					{	
						delete	(CCreateBase*)pGraphPosDel->pCreateBasepos;
						pGraphPosDel->pCreateBasepos = NULL;
					}
					else
					{
						delete (CCreateBase*)pGraphPosDel->pCreateBaseposSpare1;
						delete (CCreateBase*)pGraphPosDel->pCreateBaseposSpare2;
						pGraphPosDel->pCreateBaseposSpare1 = NULL;
						pGraphPosDel->pCreateBaseposSpare2 = NULL;
					}
				}
			}
			//ƽ���߲���
			else if (pUndoListDel->OperatieWhat == ID_PARALLEL)
			{
				if (IsDelCreate == TRUE)
				{	
					delete	(CCreateBase*)pGraphPosDel->pCreateBaseposSpare1;
					pGraphPosDel->pCreateBaseposSpare1 = NULL;
				}				
			}
			//��ת����
			else if (pUndoListDel->OperatieWhat == ID_ROTATE_180)
			{
				if (IsDelCreate == TRUE)
				{	
					delete	(CCreateBase*)pGraphPosDel->pCreateBasepos;
					pGraphPosDel->pCreateBasepos = NULL;
					pGraphPosDel->pCreateBaseposSpare2 = NULL;
				}	
				else
				{
					if (pGraphPosDel->pCreateBaseposSpare2)
					{
						delete	(CCreateBase*)pGraphPosDel->pCreateBaseposSpare1;
						pGraphPosDel->pCreateBaseposSpare1 = NULL;
						pGraphPosDel->pCreateBaseposSpare2 = NULL;						
					}
				}
			}
			//��ת����
			else if (pUndoListDel->OperatieWhat == ID_ROTATE||pUndoListDel->OperatieWhat == ID_MIRROR)
			{
				if (IsDelCreate == TRUE)
				{
					POSITION POS  = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)pGraphPosDel->pCreateBasepos);
					delete	(CCreateBase*)pGraphPosDel->pCreateBasepos;					
				}					
			}
			delete pGraphPosDel;
			pGraphPosDel = NULL;
		}

		//�ͷ�pGraphPosSpare��ͼ���ڳ���������ռ�õ��ڴ�
		while(pUndoListDel->pGraphPosSpare != NULL)
		{
			PGRAPHPOS pGraphPosDel = pUndoListDel->pGraphPosSpare;
			pUndoListDel->pGraphPosSpare = pUndoListDel->pGraphPosSpare->pNext;
			//ĥ�߲���
			if (pUndoListDel->OperatieWhat == ID_GRIND_GLASS||pUndoListDel->OperatieWhat == ID_SOLID_TO_DASH)
			{
				if (IsDelCreate == TRUE)
				{
					CCreateBase* pCreateBasePos = (CCreateBase*)pGraphPosDel->pCreateBasepos;
					delete pCreateBasePos;
					pCreateBasePos = NULL;	
				}
			}
			delete pGraphPosDel;
			pGraphPosDel = NULL;
		}		
		pUndoListTemp = pUndoListTemp->pNext;
		delete pUndoListDel;
		pUndoListDel = NULL;
	}
	pUndoList = NULL;
}

/*
 *	���PUNDOLIST
 */
void CUndoRedo::AddUndoList(PUNDOLIST UndoList)
{
	if (m_pCurrList == NULL)//��һ��Ҫ����ͷ�������
	{
		m_pHeadList->pNext = m_pCurrList = UndoList;
		m_pCurrList->pPrev = m_pHeadList;
	}
	else
	{
		//��������������µĲ�����ӽ�����Ҫ����������ɾ�������ͷ��ڴ�
		if (m_pCurrList->pNext != NULL)
		{
			g_iNumUndo = m_pCurrList->OperatieNum;
			ClearUndoList(m_pCurrList->pNext,TRUE);
		}
		
		m_pCurrList->pNext = UndoList;
		UndoList->pPrev = m_pCurrList;
	}
	//��Ӳ�������
	g_iNumUndo++;
	UndoList->OperatieNum = g_iNumUndo;	
	m_pTailList = m_pCurrList = UndoList;
}

/*
 *	ִ�г�������
 */
void CUndoRedo::OnUndo()
{
	if (m_pCurrList == NULL)
		return;
	if(m_pCurrList->OperatieNum == 0)
		return;
	switch(m_pCurrList->OperatieWhat)
	{
	case ID_MOVE:		
			OnMove(TRUE);
		break;
	case ID_MIRROR:
			OnMirror(TRUE);
		break;
	case ID_LINE:
		OnCreateGraph(TRUE);
		break;
	case ID_ROTATE:
		OnRotate(TRUE);
		break;
	case  ID_DELETE:
		OnDeleteGraph(TRUE);
		break;
	case  ID_CHAMFER:
		OnChamferGraph(TRUE);
		break;
	case  ID_RE_ARRANG:
		OnArrangeGraph(TRUE);
		break;
	case  ID_BREAK_TWO:
		OnBreakGraph(TRUE);
		break;
	case  ID_OVER_CUT:
		OnOverCutGraph(TRUE);
		break;
	case  ID_SCALE_XY:
		OnScaleGraph(TRUE);
		break;	
	case  ID_IO_KNIFE:
		OnIOKnife(TRUE);
		break;			
	case  ID_TOGETHER:
		OnTogetherGraph(TRUE);
		break;			
	case  ID_GRIND_GLASS:
		OnGrindGraph(TRUE);
		break;			
	case  ID_MILL_GROOVES:
		OnMillGrooves(TRUE);
		break;		
	case  ID_ROTATE_180:
		OnRotateSymmetry(TRUE);
		break;
	case  IDC_BTN_CUTDIR:
		OnChangeDir(TRUE);
		break;		
	case  IDC_COMBO_COLOR:
		OnChangeColor(TRUE);
		break;		
	case  IDC_BTN_FORWARD:
		OnPropertyChange(TRUE);
		break;		
	case  IDC_BTN_INSERT_BEFORE:
		OnInsertAfterGraph(TRUE);
		break;		
	case  IDC_BTN_INSERT_AFTER:
		OnInsertBeforGraph(TRUE);
		break;		
	case  ID_SCALE:
		OnZoomScale(TRUE);
		break;	
	case  ID_SOLID_TO_DASH:
		OnTurnDash(TRUE);
		break;			
	case  ID_MOVE_POINT:
		OnMovePoint(TRUE);
		break;			
	default:
		break;
	}
	m_pCurrList = m_pCurrList->pPrev;
	g_pView->Invalidate();
}

/*
 *	ִ����������
 */
void CUndoRedo::OnRedo()
{
	if (m_pCurrList == NULL)
		return;
	if (m_pCurrList == m_pTailList)
		return;
	m_pCurrList = m_pCurrList->pNext;		
	switch(m_pCurrList->OperatieWhat)
	{
	case ID_MOVE:		
		OnMove(FALSE);
		break;
	case ID_MIRROR:
		OnMirror(FALSE);
		break;
	case ID_LINE:
		OnCreateGraph(FALSE);
		break;
	case ID_ROTATE:
		OnRotate(FALSE);
		break;
	case  ID_DELETE:
		OnDeleteGraph(FALSE);
		break;
	case  ID_CHAMFER:
		OnChamferGraph(FALSE);
		break;
	case  ID_RE_ARRANG:
		OnArrangeGraph(FALSE);
		break;
	case  ID_BREAK_TWO:
		OnBreakGraph(FALSE);
		break;
	case  ID_OVER_CUT:
		OnOverCutGraph(FALSE);
		break;
	case  ID_SCALE_XY:
		OnScaleGraph(FALSE);
		break;		
	case  ID_IO_KNIFE:
		OnIOKnife(FALSE);
		break;	
	case  ID_TOGETHER:
		OnTogetherGraph(FALSE);
		break;	
	case  ID_GRIND_GLASS:
		OnGrindGraph(FALSE);
		break;	
	case  ID_MILL_GROOVES:
		OnMillGrooves(FALSE);
		break;	
	case  ID_ROTATE_180:
		OnRotateSymmetry(FALSE);
		break;		
	case  IDC_BTN_CUTDIR:
		OnChangeDir(FALSE);
		break;
	case  IDC_COMBO_COLOR:
		OnChangeColor(FALSE);
		break;		
	case  IDC_BTN_FORWARD:
		OnPropertyChange(FALSE);
		break;		
	case  IDC_BTN_INSERT_BEFORE:
		OnInsertAfterGraph(FALSE);
		break;		
	case  IDC_BTN_INSERT_AFTER:
		OnInsertBeforGraph(FALSE);
		break;		
	case  ID_SCALE:
		OnZoomScale(FALSE);
		break;	
	case  ID_SOLID_TO_DASH:
		OnTurnDash(FALSE);
			break;			
	case  ID_MOVE_POINT:
		OnMovePoint(FALSE);
		break;			
		
		
	default:
		break;
	}
	g_pView->Invalidate();
}


/*
 *	�½�һ��UNDOLIST
 */
PUNDOLIST CUndoRedo::NewUndoList()
{
	//�½�һ��UNDOLIST
	PUNDOLIST pUndoList = new UNDOLIST;
	memset(pUndoList,0,sizeof(UNDOLIST));
	return pUndoList;
}



/*
 * �½�һ��POPERATE_PARAM
 */

POPERATE_PARAM CUndoRedo::NewOperateParam()
{
	POPERATE_PARAM pOperateParam = new OPERATE_PARAM;
	memset(pOperateParam,0,sizeof(OPERATE_PARAM));
	return pOperateParam;
}

/*
 * �½�һ��GRAPHPOS
 */
PGRAPHPOS CUndoRedo::NewGraphPos()
{
	PGRAPHPOS pGraphPos = new GRAPHPOS;
	pGraphPos->pCreateBasepos = NULL;
	pGraphPos->pCreateBaseposSpare1 = NULL;
	pGraphPos->pCreateBaseposSpare2 = NULL;
	
	pGraphPos->potData1.x=0.0;	
	pGraphPos->potData2.x=0.0;	
	pGraphPos->potData1.y=0.0;	
	pGraphPos->potData2.y=0.0;	
	pGraphPos->iData1= NULL;		
	pGraphPos->iData2= NULL;
	pGraphPos->dData1= NULL;
	pGraphPos->dData2= NULL;		
	
	pGraphPos->pListPos = NULL;
	pGraphPos->pListPosSpare = NULL;
	pGraphPos->pNext = NULL;
	pGraphPos->pPrev = NULL;
	return pGraphPos;
}

/*
 *	ִ�г����ƶ�����
 */
void CUndoRedo::OnMove(BOOL IsUndo)
{
	Position posDes,posBase;
	if (IsUndo == FALSE)
	{
		g_pDoc->PosBeEqualed(posDes,m_pCurrList->pOperatieParam->potDes);
		g_pDoc->PosBeEqualed(posBase,m_pCurrList->pOperatieParam->potBase);
	}
	else
	{
		g_pDoc->PosBeEqualed(posBase,m_pCurrList->pOperatieParam->potDes);
		g_pDoc->PosBeEqualed(posDes,m_pCurrList->pOperatieParam->potBase);		
	}
	CCreateBase* l_pGraph;
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	while (GraphPosTemp != NULL)
	{
		l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
		l_pGraph->Move(posBase,posDes);
		GraphPosTemp = GraphPosTemp->pNext;
	}
}

/*
 *	ִ�г����������
 */
void CUndoRedo::OnMirror(BOOL IsUndo)
{
	CCreateBase* l_pGraph;
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	POSITION POS;
	if(m_pCurrList->pOperatieParam->direction == FALSE)
	{
		if (IsUndo == TRUE)
		{
			while (GraphPosTemp != NULL)
			{
				l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
				POS = g_pDoc->m_CCreateBaseList.Find(l_pGraph);
				g_pDoc->m_CCreateBaseList.RemoveAt(POS);
				GraphPosTemp = GraphPosTemp->pNext;
			}
		}	
		else
		{
			while (GraphPosTemp != NULL)
			{
				l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
				g_pDoc->m_CCreateBaseList.AddTail(l_pGraph);
				GraphPosTemp = GraphPosTemp->pNext;
			}			
		}
		return;
	}	
	Position posDes,posBase;
	g_pDoc->PosBeEqualed(posDes,m_pCurrList->pOperatieParam->potDes);
	g_pDoc->PosBeEqualed(posBase,m_pCurrList->pOperatieParam->potBase);
	while (GraphPosTemp != NULL)
	{
		l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
		l_pGraph->Mirror(posBase,posDes);
		GraphPosTemp = GraphPosTemp->pNext;
	}
}

/*
 *	ִ�г�����ת����
 */
void CUndoRedo::OnRotate(BOOL IsUndo)
{
	CCreateBase* l_pGraph;
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	POSITION POS;
	if(m_pCurrList->pOperatieParam->direction == FALSE)
	{
		if (IsUndo == TRUE)
		{
			while (GraphPosTemp != NULL)
			{
				l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
				POS = g_pDoc->m_CCreateBaseList.Find(l_pGraph);
				g_pDoc->m_CCreateBaseList.RemoveAt(POS);
				GraphPosTemp = GraphPosTemp->pNext;
			}
		}	
		else
		{
			while (GraphPosTemp != NULL)
			{
				l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
				g_pDoc->m_CCreateBaseList.AddTail(l_pGraph);
				GraphPosTemp = GraphPosTemp->pNext;
			}			
		}
		return;
	}
	Position posBase;
	g_pDoc->PosBeEqualed(posBase,m_pCurrList->pOperatieParam->potBase);
	double l_dAngle;
	if (IsUndo == TRUE)
		l_dAngle = -m_pCurrList->pOperatieParam->angle;
	else
		l_dAngle = m_pCurrList->pOperatieParam->angle;
	
	while (GraphPosTemp != NULL)
	{
		l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
		l_pGraph->Rotate(posBase,l_dAngle);
		GraphPosTemp = GraphPosTemp->pNext;
	}
}

/*
 *	ִ�г�����ͼ����
 */
void CUndoRedo::OnCreateGraph(BOOL IsUndo)
{
	POSITION pos = NULL;
	if (IsUndo == TRUE)
	{
		PGRAPHPOS pGraphPosTemp = m_pCurrList->pGraphPos;
		while (pGraphPosTemp)
		{
			pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(pGraphPosTemp->pCreateBasepos)) ;
			g_pDoc->m_CCreateBaseList.RemoveAt(pos) ;
			pGraphPosTemp = pGraphPosTemp->pNext;
		}
	}
	else
	{
		PGRAPHPOS pGraphPosTemp = m_pCurrList->pGraphPos;
		CCreateBase* l_pGraph;
		while (pGraphPosTemp)
		{
			l_pGraph = (CCreateBase*)(pGraphPosTemp->pCreateBasepos);
			g_pDoc->m_CCreateBaseList.AddTail(l_pGraph);
			pGraphPosTemp = pGraphPosTemp->pNext;
		}
	}
}

/*
 *	ִ�г���ɾ������
 */
void CUndoRedo::OnDeleteGraph(BOOL IsUndo)
{
	POSITION pos = NULL;
	if (IsUndo == FALSE)
	{
		PGRAPHPOS pGraphPosTemp = m_pCurrList->pGraphPos;
		while (pGraphPosTemp)
		{
			pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(pGraphPosTemp->pCreateBasepos)) ;
			g_pDoc->m_CCreateBaseList.RemoveAt(pos) ;
			pGraphPosTemp = pGraphPosTemp->pNext;
		}
	}
	else
	{
		//�ȵõ�ͼ�������βָ��
		PGRAPHPOS pGraphPosTemp1 = m_pCurrList->pGraphPos;
		PGRAPHPOS pGraphPosTemp;
		while (pGraphPosTemp1)
		{
			pGraphPosTemp = pGraphPosTemp1;
			pGraphPosTemp1 = pGraphPosTemp1->pNext;
		}
		//�Ӻ���ָ�
		CCreateBase* l_pGraph;
		while (pGraphPosTemp)
		{
			l_pGraph = (CCreateBase*)(pGraphPosTemp->pCreateBasepos);
			pos = pGraphPosTemp->pListPos;
			if(pos == NULL)
				g_pDoc->m_CCreateBaseList.AddTail(l_pGraph);			
			else
				g_pDoc->m_CCreateBaseList.InsertBefore(pos,l_pGraph);
			pGraphPosTemp = pGraphPosTemp->pPrev;
		}
	}
}

/*
 *	ִ�г������ǲ���
 */
void CUndoRedo::OnChamferGraph(BOOL IsUndo)
{
	POSITION pos = NULL;
	PGRAPHPOS pGraphPosTemp = m_pCurrList->pGraphPos;
	CCreateBase *pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetAt(pGraphPosTemp->pListPos);
	pos = g_pDoc->m_CCreateBaseList.InsertAfter(pGraphPosTemp->pListPos,(CCreateBase*)pGraphPosTemp->pCreateBasepos);
	g_pDoc->m_CCreateBaseList.RemoveAt(pGraphPosTemp->pListPos);
	pGraphPosTemp->pListPos = pos;
	pGraphPosTemp->pCreateBasepos = (LONG)pCreateBase;
}


/*
 *	ִ�г��������������
 */
void CUndoRedo::OnArrangeGraph(BOOL IsUndo)
{

	POSITION pos = NULL;
	PGRAPHPOS pGraphPosTemp = m_pCurrList->pGraphPos;
	pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(pGraphPosTemp->pCreateBasepos)); 	
	g_pDoc->m_CCreateBaseList.RemoveAt(pos);
	if (IsUndo == TRUE)
	{
		pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(pGraphPosTemp->pCreateBaseposSpare1));
		g_pDoc->m_CCreateBaseList.InsertAfter(pos,(CCreateBase*)pGraphPosTemp->pCreateBasepos);
	}
	else
	{
		pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(pGraphPosTemp->pCreateBaseposSpare2));		
		g_pDoc->m_CCreateBaseList.InsertBefore(pos,(CCreateBase*)pGraphPosTemp->pCreateBasepos);
	}
}

/*
 *	ִ�г�����ϲ���
 */
void CUndoRedo::OnBreakGraph(BOOL IsUndo)
{	
	PGRAPHPOS pGraphPosTemp = m_pCurrList->pGraphPos;
	POSITION pos = pGraphPosTemp->pListPos;	
	if (IsUndo == TRUE)
	{
		g_pDoc->m_CCreateBaseList.InsertAfter(pos,(CCreateBase*)pGraphPosTemp->pCreateBasepos);
		g_pDoc->m_CCreateBaseList.GetPrev(pos);
		g_pDoc->m_CCreateBaseList.RemoveAt(pGraphPosTemp->pListPos);
		g_pDoc->m_CCreateBaseList.RemoveAt(pos);
		pGraphPosTemp->pListPos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)pGraphPosTemp->pCreateBasepos);
	}
	else
	{
		g_pDoc->m_CCreateBaseList.InsertBefore(pos,(CCreateBase*)pGraphPosTemp->pCreateBaseposSpare1);
		g_pDoc->m_CCreateBaseList.InsertBefore(pos,(CCreateBase*)pGraphPosTemp->pCreateBaseposSpare2);	
		g_pDoc->m_CCreateBaseList.RemoveAt(pos);
		pGraphPosTemp->pListPos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)pGraphPosTemp->pCreateBaseposSpare2);
	}
}

/*
 *	ִ�г������в���
 */
 void CUndoRedo::OnOverCutGraph(BOOL IsUndo)
{
	 PGRAPHPOS pGraphPosTemp = m_pCurrList->pGraphPos;
	 POSITION pos = (POSITION)m_pCurrList->pOperatieParam->pPosition;	
	 CCreatePolyline *pPolyline = (CCreatePolyline*)m_pCurrList->pGraphPos->pCreateBaseposSpare1;
	 if (IsUndo == TRUE)
	 {
		 g_pDoc->m_CCreateBaseList.InsertAfter(pos,pPolyline);
		 while (pGraphPosTemp)
		 {
			pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)pGraphPosTemp->pCreateBasepos);
			g_pDoc->m_CCreateBaseList.RemoveAt(pos);
			pGraphPosTemp = pGraphPosTemp->pNext;
		 }
		m_pCurrList->pOperatieParam->pPosition = (LONG)g_pDoc->m_CCreateBaseList.Find(pPolyline);
	 }
	 else
	 {	 
		 m_pCurrList->pOperatieParam->pPosition = (LONG)g_pDoc->m_CCreateBaseList.Find((CCreateBase*)pGraphPosTemp->pCreateBasepos);
		 while (pGraphPosTemp)
		 {
			 g_pDoc->m_CCreateBaseList.InsertBefore(pos,(CCreateBase*)pGraphPosTemp->pCreateBasepos);
			 pGraphPosTemp = pGraphPosTemp->pNext;
		 }	
		 g_pDoc->m_CCreateBaseList.RemoveAt(pos);
	 }
}

/*
 *	ִ�г������Ų���
 */
void CUndoRedo::OnScaleGraph(BOOL IsUndo)
{
	double dValueX, dValueY;
	if (IsUndo == TRUE)
	{
		dValueX = -m_pCurrList->pOperatieParam->potBase.x;
		dValueY = -m_pCurrList->pOperatieParam->potBase.y;
	}
	else
	{
		dValueX = m_pCurrList->pOperatieParam->potBase.x;
		dValueY = m_pCurrList->pOperatieParam->potBase.y;
	}	
	CCreateBase* l_pGraph;
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	while (GraphPosTemp != NULL)
	{
		l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
		l_pGraph->GraphZoomXY(dValueX,dValueY);
		GraphPosTemp = GraphPosTemp->pNext;
	}
}

/*
 *	ִ�г�����ϳ������
 */
void CUndoRedo::OnMillGrooves(BOOL IsUndo)
{
 	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	while (GraphPosTemp != NULL)
	{
		if (IsUndo == TRUE) 
		{
			POSITION pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(GraphPosTemp->pCreateBaseposSpare1)) ;
			g_pDoc->m_CCreateBaseList.InsertBefore(pos,(CCreateBase*)(GraphPosTemp->pCreateBasepos));
			if (GraphPosTemp->pCreateBaseposSpare2)
			{
				POSITION pos2 = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(GraphPosTemp->pCreateBaseposSpare2));	
				g_pDoc->m_CCreateBaseList.RemoveAt(pos2);
			}
			g_pDoc->m_CCreateBaseList.RemoveAt(pos);
		}
		else
		{
			POSITION pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(GraphPosTemp->pCreateBasepos)) ;
			g_pDoc->m_CCreateBaseList.InsertBefore(pos,(CCreateBase*)(GraphPosTemp->pCreateBaseposSpare1));
			if (GraphPosTemp->pCreateBaseposSpare2)
				g_pDoc->m_CCreateBaseList.InsertBefore(pos,(CCreateBase*)(GraphPosTemp->pCreateBaseposSpare2));
			g_pDoc->m_CCreateBaseList.RemoveAt(pos);
		}
		GraphPosTemp = GraphPosTemp->pNext;
	}
}

/*
 *	ִ�г�����ϳ����ѡ������
 */
void CUndoRedo::OnIOKnife(BOOL IsUndo)
{
	CArray<Position,Position&> l_aryPoint;
	CArray<double,double&> l_aryTudu;
	CCreateBase* l_pGraph;
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	POPERATE_PARAM OperateParam = m_pCurrList->pOperatieParam;
	while (GraphPosTemp != NULL)
	{
		if (GraphPosTemp->pCreateBaseposSpare1 == NULL)
		{
			l_pGraph = (CCreateBase*)GraphPosTemp->pCreateBasepos;
			if (l_pGraph->IsKindOf(RUNTIME_CLASS(CCreatePolyline))) 
			{
				CCreatePolyline* l_pPolyline = (CCreatePolyline*)GraphPosTemp->pCreateBasepos;
				//�ѵ�ǰ�ĵ㼯�ó���
				l_aryPoint.Copy(l_pPolyline->m_point);
				l_aryTudu.Copy(l_pPolyline->m_dTuDu);
				//��������������ǰ�ĵ㼯�Ż�ȥ
				l_pPolyline->m_point.Copy(GraphPosTemp->aryPoint);
				l_pPolyline->m_dTuDu.Copy(GraphPosTemp->aryTudu);
				//�ٰ�֮ǰ�ĵ㼯������
				GraphPosTemp->aryPoint.Copy(l_aryPoint);
				GraphPosTemp->aryTudu.Copy(l_aryTudu);
			}
			else if (l_pGraph->IsKindOf(RUNTIME_CLASS(CCreateEllipse))) 
			{
				CCreateEllipse* l_pEllipse = (CCreateEllipse*)GraphPosTemp->pCreateBasepos;
				l_aryPoint.Copy(l_pEllipse->m_point);
				l_aryTudu.Copy(l_pEllipse->m_dTuDu);
				l_pEllipse->m_point.Copy(GraphPosTemp->aryPoint);
				l_pEllipse->m_dTuDu.Copy(GraphPosTemp->aryTudu);
				GraphPosTemp->aryPoint.Copy(l_aryPoint);
				GraphPosTemp->aryTudu.Copy(l_aryTudu);
			}
			else if (l_pGraph->IsKindOf(RUNTIME_CLASS(CCreateRectangle))) 
			{
				CCreateRectangle* l_pRect = (CCreateRectangle*)GraphPosTemp->pCreateBasepos;
				l_aryPoint.Copy(l_pRect->m_point);
				l_aryTudu.Copy(l_pRect->m_dTuDu);
				l_pRect->m_point.Copy(GraphPosTemp->aryPoint);
				l_pRect->m_dTuDu.Copy(GraphPosTemp->aryTudu);
				GraphPosTemp->aryPoint.Copy(l_aryPoint);
				GraphPosTemp->aryTudu.Copy(l_aryTudu);
			}
			else if (l_pGraph->IsKindOf(RUNTIME_CLASS(CCreateRound))) 
			{
				CCreateRound* l_pRound = (CCreateRound*)GraphPosTemp->pCreateBasepos;
				Position posEnd;
				g_pDoc->PosBeEqualed(posEnd,OperateParam->potBase);
				g_pDoc->PosBeEqualed(OperateParam->potBase,l_pRound->m_EndPos);		
				l_pRound->m_EndPos = posEnd;		
			}
			else if(l_pGraph->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				CCreateLine* pLine = (CCreateLine*) l_pGraph;
				Position posBegin,posEnd;
				posBegin = pLine->m_Begin;
				posEnd = pLine->m_End;
				g_pDoc->PosBeEqualed(pLine->m_Begin,OperateParam->potBase);
				g_pDoc->PosBeEqualed(pLine->m_End,OperateParam->potDes);						
				g_pDoc->PosBeEqualed(OperateParam->potBase,posBegin);
				g_pDoc->PosBeEqualed(OperateParam->potDes,posEnd);						
			}
			else if(l_pGraph->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
			{
				CCreateRoundArc* pArc = (CCreateRoundArc*) l_pGraph;
				Position posBegin = pArc->m_Begin,posEnd = pArc->m_End;
				double l_dAngelStart = pArc->m_dAngleStart,l_dAngleEnd = pArc->m_dAngleEnd;

				g_pDoc->PosBeEqualed(pArc->m_Begin,OperateParam->potBase);
				g_pDoc->PosBeEqualed(pArc->m_End,OperateParam->potDes);	
				pArc->m_dAngleStart = 	OperateParam->angle;
				pArc->m_dAngleEnd = OperateParam->distance;
				
				g_pDoc->PosBeEqualed(OperateParam->potBase,posBegin);
				g_pDoc->PosBeEqualed(OperateParam->potDes,posEnd);
				OperateParam->angle = l_dAngelStart;
				OperateParam->distance = l_dAngleEnd;
			}
		}
		else
		{
			if (IsUndo == TRUE) 
			{
				POSITION pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(GraphPosTemp->pCreateBaseposSpare1)) ;
				g_pDoc->m_CCreateBaseList.InsertBefore(pos,(CCreateBase*)(GraphPosTemp->pCreateBasepos));
				if(GraphPosTemp->pCreateBaseposSpare2)
				{
					POSITION pos2 = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(GraphPosTemp->pCreateBaseposSpare2)) ;	
					g_pDoc->m_CCreateBaseList.RemoveAt(pos2);
				}
				g_pDoc->m_CCreateBaseList.RemoveAt(pos);
			}
			else
			{
				POSITION pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(GraphPosTemp->pCreateBasepos)) ;
				g_pDoc->m_CCreateBaseList.InsertBefore(pos,(CCreateBase*)(GraphPosTemp->pCreateBaseposSpare1));
				if(GraphPosTemp->pCreateBaseposSpare2)
					g_pDoc->m_CCreateBaseList.InsertBefore(pos,(CCreateBase*)(GraphPosTemp->pCreateBaseposSpare2));
				g_pDoc->m_CCreateBaseList.RemoveAt(pos);
			}
		}
		GraphPosTemp = GraphPosTemp->pNext;
	}
}

/*
 *	ִ�г����ϲ�����
 */
void CUndoRedo::OnTogetherGraph(BOOL IsUndo)
{
	PGRAPHPOS pGraphPosTemp = m_pCurrList->pGraphPos;	
	//		���Ƶ�ǰ����
	CObList m_cpyBaseList;
	CCreateBase* l_pCreateBase;
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	while(position != NULL)
	{
		l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
		m_cpyBaseList.AddTail(l_pCreateBase);
	}
	g_pDoc->m_CCreateBaseList.RemoveAll();
	//	��������
	while (pGraphPosTemp)
	{
		g_pDoc->m_CCreateBaseList.AddTail((CCreateBase *)pGraphPosTemp->pCreateBasepos);
		pGraphPosTemp = pGraphPosTemp->pNext;
	}
	//��ͼ�����ָ�붼��ɿ�
//	pGraphPosTemp = m_pCurrList->pGraphPos;
	while(m_pCurrList->pGraphPos != NULL)
	{
		PGRAPHPOS pGraphPosDel = m_pCurrList->pGraphPos;
		m_pCurrList->pGraphPos = m_pCurrList->pGraphPos->pNext;
		pGraphPosDel->pCreateBasepos = NULL;
		delete pGraphPosDel;
		pGraphPosDel = NULL;
	}

	//�ѵ�ǰ��ͼ�δ浽����������
	pGraphPosTemp = m_pCurrList->pGraphPos = NewGraphPos();	
	PGRAPHPOS GraphPos;
	position = m_cpyBaseList.GetHeadPosition();
	while(position != NULL)
	{
		l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(position);	
		//�������������
		if (pGraphPosTemp->pCreateBasepos == NULL)
			pGraphPosTemp->pCreateBasepos = (LONG)l_pCreateBase;
		else
		{
			GraphPos = NewGraphPos();						
			GraphPos->pCreateBasepos = (LONG)l_pCreateBase;
			pGraphPosTemp->pNext = GraphPos;
			GraphPos->pPrev = pGraphPosTemp;
			pGraphPosTemp = GraphPos;
		} 
	}
	m_cpyBaseList.RemoveAll();
}

/*
 *	ִ�г���ĥ��������
 */
void CUndoRedo::OnGrindGraph(BOOL IsUndo)
{
	POSITION pos = NULL ,pos1=NULL;
	PGRAPHPOS pGraphPosTemp,pGraphPosTemp1;
	
	if (IsUndo == FALSE)
	{
		pGraphPosTemp1 = m_pCurrList->pGraphPosSpare;
		while (pGraphPosTemp1)
		{
			pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(pGraphPosTemp1->pCreateBaseposSpare1)) ;
			g_pDoc->m_CCreateBaseList.InsertBefore(pos,(CCreateBase*)(pGraphPosTemp1->pCreateBasepos));			
			pGraphPosTemp1 = pGraphPosTemp1->pNext;
		}
		pGraphPosTemp = m_pCurrList->pGraphPos;
		while (pGraphPosTemp)
		{
			pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(pGraphPosTemp->pCreateBasepos)) ;
			g_pDoc->m_CCreateBaseList.RemoveAt(pos) ;
			pGraphPosTemp = pGraphPosTemp->pNext;
		}
	}
	else
	{
		//�ȵõ�ͼ�������βָ��
		pGraphPosTemp = m_pCurrList->pGraphPos;
		CCreateBase* l_pGraph;
		//��ԭͼ�ָ�
		while (pGraphPosTemp)
		{
			l_pGraph = (CCreateBase*)(pGraphPosTemp->pCreateBasepos);
			pos = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)(pGraphPosTemp->pCreateBaseposSpare1)) ;
			g_pDoc->m_CCreateBaseList.InsertAfter(pos,l_pGraph);
			pGraphPosTemp = pGraphPosTemp->pNext;
		}
		//���¼ӵ�ĥ����ͼ��ɾ��
		pGraphPosTemp = m_pCurrList->pGraphPosSpare;
		while (pGraphPosTemp)
		{
			l_pGraph = (CCreateBase*)(pGraphPosTemp->pCreateBasepos);
			pos = g_pDoc->m_CCreateBaseList.Find(l_pGraph);
			g_pDoc->m_CCreateBaseList.RemoveAt(pos);
			pGraphPosTemp = pGraphPosTemp->pNext;
		}
	}
}

/*
 *	ִ�г�����ת����
 */
void CUndoRedo::OnRotateSymmetry(BOOL IsUndo)
{
	CCreateBase* l_pGraph;
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	POSITION POS;
	if (IsUndo == TRUE)
	{
		while (GraphPosTemp != NULL)
		{
			l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
			POS = g_pDoc->m_CCreateBaseList.Find(l_pGraph);
			if (GraphPosTemp->pCreateBaseposSpare2)
				g_pDoc->m_CCreateBaseList.InsertBefore(POS,(CCreateBase*)(GraphPosTemp->pCreateBaseposSpare1));
			g_pDoc->m_CCreateBaseList.RemoveAt(POS);
			GraphPosTemp = GraphPosTemp->pNext;
		}
	}
	else
	{
		while (GraphPosTemp != NULL)
		{
			l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBaseposSpare1);
			POS = g_pDoc->m_CCreateBaseList.Find(l_pGraph);
			g_pDoc->m_CCreateBaseList.InsertAfter(POS,(CCreateBase*)(GraphPosTemp->pCreateBasepos));
			if (GraphPosTemp->pCreateBaseposSpare2)
				g_pDoc->m_CCreateBaseList.RemoveAt(POS);
			GraphPosTemp = GraphPosTemp->pNext;
		}
	}
}

/*
 *	ִ�г����ı��и�����
 */
void CUndoRedo::OnChangeDir(BOOL IsUndo)
{
	CCreateBase* l_pGraph;
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	while (GraphPosTemp != NULL)
	{
		l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
		l_pGraph->ChangeDir();
		GraphPosTemp = GraphPosTemp->pNext;
	}
}

/*
 *	ִ�г�������ɫ����
 */
void CUndoRedo::OnChangeColor(BOOL IsUndo)
{
	CCreateBase* l_pGraph;
	CString l_strSBM,l_strCBM;
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	COLORREF l_color = 0;
	if (IsUndo == TRUE)
	{
		while (GraphPosTemp != NULL)
		{
			l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
			l_color = GraphPosTemp->iData1;
			if(g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_color))//����������ɫ�ж�Ӧ��M����
				g_pDoc->ReplaceMCode(l_pGraph->m_ColorOfLine,l_pGraph->m_strSBM,l_strSBM,l_pGraph->m_strCBM,l_strCBM);
			else
			{
				g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_pGraph->m_ColorOfLine);
				int l_iSizeSB = l_pGraph->m_strSBM.GetSize();
				for(int i=0;i<l_iSizeSB;i++)
				{
					if(l_strSBM == l_pGraph->m_strSBM.GetAt(i))
						l_pGraph->m_strSBM.RemoveAt(i);
				}
				int l_iSizeCB = l_pGraph->m_strCBM.GetSize();
				for(int j=0;j<l_iSizeCB;j++)
				{
					if(l_strCBM == l_pGraph->m_strCBM.GetAt(j))
						l_pGraph->m_strCBM.RemoveAt(j);
				}
				
			}
			l_pGraph->m_ColorOfLine = l_color;
			GraphPosTemp = GraphPosTemp->pNext;
		}
	}
	else
	{
		l_color = m_pCurrList->pOperatieParam->direction;
		while (GraphPosTemp != NULL)
		{
			l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
			g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_color);
			if(!g_pDoc->ReplaceMCode(l_pGraph->m_ColorOfLine,l_pGraph->m_strSBM,l_strSBM,l_pGraph->m_strCBM,l_strCBM))
			{				
				l_pGraph->m_strSBM.Add(l_strSBM);
				l_pGraph->m_strCBM.Add(l_strCBM);
			}
			l_pGraph->m_ColorOfLine = l_color;
			GraphPosTemp = GraphPosTemp->pNext;
		}		
	}
}

/*
 *	ִ�г������ԶԻ���ı༭�����;TRUEΪ������FALSEΪ����
 */
void CUndoRedo::OnPropertyChange(BOOL IsUndo)
{
	int l_iWhichEdit = m_pCurrList->pOperatieParam->direction;	
	CCreateBase* l_pGraph;
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
	if(IsUndo == TRUE)		
		l_pGraph->ChangeProperty(m_pCurrList->pOperatieParam->angle,l_iWhichEdit,m_pCurrList->pOperatieParam->pPosition);
	else
		l_pGraph->ChangeProperty(m_pCurrList->pOperatieParam->distance,l_iWhichEdit,m_pCurrList->pOperatieParam->pPosition);
}

/*
 *	ִ�г����ı䵶�����;TRUEΪ������FALSEΪ����
 */
void CUndoRedo::OnInsertAfterGraph(BOOL IsUndo)
{
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	CCreateBase* l_pGraph1 = (CCreateBase*)(GraphPosTemp->pCreateBaseposSpare1);
	CCreateBase* l_pGraph2 = (CCreateBase*)(GraphPosTemp->pCreateBaseposSpare2);
	POSITION POS2  = g_pDoc->m_CCreateBaseList.Find(l_pGraph2);
	POSITION POS1  = g_pDoc->m_CCreateBaseList.Find(l_pGraph1);
	g_pDoc->m_CCreateBaseList.RemoveAt(POS1);
	if(IsUndo == TRUE)
		g_pDoc->m_CCreateBaseList.InsertBefore(POS2,l_pGraph1);
	else
		g_pDoc->m_CCreateBaseList.InsertAfter(POS2,l_pGraph1);
}

/*
 *	ִ�г����ı䵶�����;TRUEΪ������FALSEΪ����
 */
void CUndoRedo::OnInsertBeforGraph(BOOL IsUndo)
{
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	CCreateBase* l_pGraph1 = (CCreateBase*)(GraphPosTemp->pCreateBaseposSpare1);
	CCreateBase* l_pGraph2 = (CCreateBase*)(GraphPosTemp->pCreateBaseposSpare2);
	POSITION POS2  = g_pDoc->m_CCreateBaseList.Find(l_pGraph2);
	POSITION POS1  = g_pDoc->m_CCreateBaseList.Find(l_pGraph1);
	g_pDoc->m_CCreateBaseList.RemoveAt(POS1);
	if(IsUndo == TRUE)
		g_pDoc->m_CCreateBaseList.InsertAfter(POS2,l_pGraph1);
	else
		g_pDoc->m_CCreateBaseList.InsertBefore(POS2,l_pGraph1);
}

/*
 *	ִ�г������Ų���
 */
void CUndoRedo::OnZoomScale(BOOL IsUndo)
{
	Position l_posBase;
	double l_dScale=0.0;
	if (IsUndo == TRUE)
	{
		g_pDoc->PosBeEqualed(l_posBase,m_pCurrList->pOperatieParam->potBase);
		l_dScale = 1/m_pCurrList->pOperatieParam->angle;
	}
	else
	{
		g_pDoc->PosBeEqualed(l_posBase,m_pCurrList->pOperatieParam->potBase);
		l_dScale = m_pCurrList->pOperatieParam->angle;
	}	
	CCreateBase* l_pGraph;
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	while (GraphPosTemp != NULL)
	{
		l_pGraph = (CCreateBase*)(GraphPosTemp->pCreateBasepos);
		l_pGraph->GraphZoomScale(l_posBase,l_dScale);
		GraphPosTemp = GraphPosTemp->pNext;
	}
	
}

/*
 *	ִ�г��������߲���;TRUEΪ������FALSEΪ����
 */
void CUndoRedo::OnTurnDash(BOOL IsUndo)
{
	CCreateBase* pCreateBase = NULL;
	PGRAPHPOS pGraphPosPre = m_pCurrList->pGraphPos;
	PGRAPHPOS pGraphPosCur = m_pCurrList->pGraphPosSpare;
	POSITION POS1;
	if (IsUndo == TRUE)
	{
		POS1 = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)pGraphPosCur->pCreateBasepos);
		pCreateBase = (CCreateBase*)pGraphPosPre->pCreateBasepos;
		g_pDoc->m_CCreateBaseList.InsertBefore(POS1,pCreateBase);
		while (pGraphPosCur)
		{
			POS1 = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)pGraphPosCur->pCreateBasepos);
			g_pDoc->m_CCreateBaseList.RemoveAt(POS1);
			pGraphPosCur = pGraphPosCur->pNext;
		}
	}	
	else
	{
		POS1 = g_pDoc->m_CCreateBaseList.Find((CCreateBase*)pGraphPosPre->pCreateBasepos);
		while (pGraphPosCur)
		{
			pCreateBase = (CCreateBase*)pGraphPosCur->pCreateBasepos;
			g_pDoc->m_CCreateBaseList.InsertBefore(POS1,pCreateBase);
			pGraphPosCur = pGraphPosCur->pNext;
		}		
		g_pDoc->m_CCreateBaseList.RemoveAt(POS1);
	}
}

/*
 *	ִ�е��ƶ�����
 */
void CUndoRedo::OnMovePoint(BOOL IsUndo)
{
	CArray<Position,Position&> l_aryPoint;
	CArray<double,double&> l_aryTudu;
	CCreateBase* l_pGraph;
	PGRAPHPOS GraphPosTemp = m_pCurrList->pGraphPos;
	POPERATE_PARAM OperateParam = m_pCurrList->pOperatieParam;
	while (GraphPosTemp != NULL)
	{
		l_pGraph = (CCreateBase*)GraphPosTemp->pCreateBasepos;
		if (l_pGraph->IsKindOf(RUNTIME_CLASS(CCreatePolyline))) 
		{
			CCreatePolyline* l_pPolyline = (CCreatePolyline*)GraphPosTemp->pCreateBasepos;
			//�ѵ�ǰ�ĵ㼯�ó���
			l_aryPoint.Copy(l_pPolyline->m_point);
			l_aryTudu.Copy(l_pPolyline->m_dTuDu);
			//��������������ǰ�ĵ㼯�Ż�ȥ
			l_pPolyline->m_point.Copy(GraphPosTemp->aryPoint);
			l_pPolyline->m_dTuDu.Copy(GraphPosTemp->aryTudu);
			//�ٰ�֮ǰ�ĵ㼯������
			GraphPosTemp->aryPoint.Copy(l_aryPoint);
			GraphPosTemp->aryTudu.Copy(l_aryTudu);
		}
		else if (l_pGraph->IsKindOf(RUNTIME_CLASS(CCreateEllipse))) 
		{
			CCreateEllipse* l_pEllipse = (CCreateEllipse*)GraphPosTemp->pCreateBasepos;
			l_aryPoint.Copy(l_pEllipse->m_point);
			l_aryTudu.Copy(l_pEllipse->m_dTuDu);
			l_pEllipse->m_point.Copy(GraphPosTemp->aryPoint);
			l_pEllipse->m_dTuDu.Copy(GraphPosTemp->aryTudu);
			GraphPosTemp->aryPoint.Copy(l_aryPoint);
			GraphPosTemp->aryTudu.Copy(l_aryTudu);
		}
		else if (l_pGraph->IsKindOf(RUNTIME_CLASS(CCreateSpline))) 
		{
			CCreateSpline* l_pSpline = (CCreateSpline*)GraphPosTemp->pCreateBasepos;
			l_aryPoint.Copy(l_pSpline->m_aryControlPoints);
			l_pSpline->m_aryControlPoints.Copy(GraphPosTemp->aryPoint);
			GraphPosTemp->aryPoint.Copy(l_aryPoint);
			l_pSpline->RefreshSpline();
		}
		else if (l_pGraph->IsKindOf(RUNTIME_CLASS(CCreateRectangle))) 
		{
			CCreateRectangle* l_pRect = (CCreateRectangle*)GraphPosTemp->pCreateBasepos;
			l_aryPoint.Copy(l_pRect->m_point);
			l_aryTudu.Copy(l_pRect->m_dTuDu);
			l_pRect->m_point.Copy(GraphPosTemp->aryPoint);
			l_pRect->m_dTuDu.Copy(GraphPosTemp->aryTudu);
			GraphPosTemp->aryPoint.Copy(l_aryPoint);
			GraphPosTemp->aryTudu.Copy(l_aryTudu);
		}
		else if (l_pGraph->IsKindOf(RUNTIME_CLASS(CCreateRound))) 
		{
		}
		else if(l_pGraph->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine* pLine = (CCreateLine*) l_pGraph;
			Position posBegin,posEnd;
			posBegin = pLine->m_Begin;
			posEnd = pLine->m_End;
			g_pDoc->PosBeEqualed(pLine->m_Begin,GraphPosTemp->potData1);
			g_pDoc->PosBeEqualed(pLine->m_End,GraphPosTemp->potData2);						
			g_pDoc->PosBeEqualed(GraphPosTemp->potData1,posBegin);
			g_pDoc->PosBeEqualed(GraphPosTemp->potData2,posEnd);						
		}
		else if(l_pGraph->IsKindOf(RUNTIME_CLASS(CCreatePoint)))
		{
			CCreatePoint* pPoint = (CCreatePoint*) l_pGraph;
			Position posBegin;
			posBegin = pPoint->m_Begin;
			g_pDoc->PosBeEqualed(pPoint->m_Begin,GraphPosTemp->potData1);
			g_pDoc->PosBeEqualed(GraphPosTemp->potData1,posBegin);
		}
		else if(l_pGraph->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
		}
		GraphPosTemp = GraphPosTemp->pNext;		
	}
}