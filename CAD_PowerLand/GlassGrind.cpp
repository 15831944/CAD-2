// GlassGrind.cpp: implementation of the CGlassGrind class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "GlassGrind.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlassGrind::CGlassGrind()
{
	
}

CGlassGrind::~CGlassGrind()
{

}

/*
 *	得到最终的链表
 */
BOOL CGlassGrind::GetFinishList()
{
	//撤消类的对象
	CUndoRedo l_UndoRedo;
	//向撤消链表里添加的一个结点	
	PUNDOLIST l_pUndoList;
	l_pUndoList = l_UndoRedo.NewUndoList();
	l_pUndoList->OperatieWhat = ID_GRIND_GLASS;

	//结构转换
	TransToGraphList trans;
	PGRAPHLIST pList = trans.OutGraphList(l_pUndoList);
	//把选中的删除
	DeleteOriginal(l_pUndoList);
	//如果没有添加进链表把上面申请的释放掉
	if (l_pUndoList->pGraphPos == NULL)
	{
		delete l_pUndoList;
		l_pUndoList = NULL;
	}

	g_pView->Invalidate();
	return TRUE;
}


/*
 *	删除选中的图形 
 */
void CGlassGrind::DeleteOriginal(PUNDOLIST pUndoList)
{	
	//撤消类的对象,以下是撤消
	CUndoRedo l_UndoRedo;
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;
	/////////////////////////////////
	CCreateBase* pSelEntity = NULL,*pCreateBase=NULL;
	POSITION pos = NULL ,pos1=NULL;
	//在链表中查找被添加到选择集中图元
	//并将其从链表中永远删除
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize() ;
	for( int i = 0 ; i < l_iSize; i++ )
	{
		pSelEntity = (CCreateBase*)g_pDoc->m_CollectOfPicked[i] ;
		pos1 = pos = g_pDoc->m_CCreateBaseList.Find(pSelEntity);
		g_pDoc->m_CCreateBaseList.GetPrev(pos1);
		if (pos1 == NULL)
			pCreateBase = NULL;
		else
			pCreateBase = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetAt(pos1);
		g_pDoc->m_CCreateBaseList.RemoveAt(pos);
		//向撤消链表里加删除操作
		if (i == 0)
		{
			CurGraphPos->pCreateBasepos = (LONG)pSelEntity;
			CurGraphPos->pCreateBaseposSpare1 = (LONG)pCreateBase;
		}
		else
		{
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (LONG)pSelEntity;
			GraphPos->pCreateBaseposSpare1 = (LONG)pCreateBase;			
			//			GraphPos->pListPos = pos1;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		}
		pSelEntity = NULL ;
	}
	if (pUndoList->pGraphPos->pCreateBasepos != NULL)
		g_pDoc->m_Undo->AddUndoList(pUndoList);	
	//清空选择集
	g_pDoc->m_CollectOfPicked.RemoveAll();
	//更新文档，重新绘制
	g_pView->Invalidate() ;
}


//清空m_pHeadList
void CGlassGrind::ClearGraphList(PGRAPHLIST GraphList)
{
	PGRAPHLIST pGraphListTemp = GraphList;
	while (pGraphListTemp != NULL)
	{
		PGRAPHLIST pGraphListDel = pGraphListTemp;
		
		//释放pAction
		if (pGraphListDel->pAction != NULL)
		{
			delete pGraphListDel->pAction;
			pGraphListDel->pAction = NULL;
		}
		
		
		//释放pCadData
		while(pGraphListDel->pCadData != NULL)
		{
			PCADDATA pCadDataDel = pGraphListDel->pCadData;
			pGraphListDel->pCadData = pGraphListDel->pCadData->pNext;
			delete pCadDataDel;
			pCadDataDel = NULL;
		}
		
		//释放pGraph
		while(pGraphListDel->pGraph != NULL)
		{
			PGRAPH pGraphDel = pGraphListDel->pGraph;
			pGraphListDel->pGraph = pGraphListDel->pGraph->Next;
			
			//释放圆弧信息
			if (pGraphDel->pArcInfo != NULL)
			{
				delete pGraphDel->pArcInfo;
				pGraphDel->pArcInfo = NULL;
			}
			//释放直线点信息
			if (pGraphDel->pPosBegin != NULL)
			{
				delete pGraphDel->pPosBegin;
				pGraphDel->pPosBegin = NULL;
			}
			//释放直线点信息
			if (pGraphDel->pPosEnd != NULL)
			{
				delete pGraphDel->pPosEnd;
				pGraphDel->pPosEnd = NULL;
			}
			//释放直线点信息
			if (pGraphDel->pPosMid != NULL)
			{
				delete pGraphDel->pPosMid;
				pGraphDel->pPosMid = NULL;
			}
			delete pGraphDel;
			pGraphDel = NULL;
		}
		
		pGraphListTemp = pGraphListTemp->Next;
		delete pGraphListDel;
		pGraphListDel = NULL;
	}
	GraphList = NULL;
}

/*
 *	复制PGRAPHLIST
 */
PGRAPHLIST CGlassGrind::CopyGraphList(PGRAPHLIST pGraphList)
{
	ASSERT(pGraphList != NULL);
	//解析Datastruct链表
	PGRAPHLIST pGraphListTemp = pGraphList;
	PGRAPHLIST pCurGraphList = NULL;
	PGRAPHLIST pHeadGraphList = NULL;
	PGRAPH pCurGraph = NULL;
	PCADDATA pCurPtData = NULL;
	
	while( pGraphListTemp != NULL )
	{
		PGRAPHLIST  pGraphListInsert = new GRAPHLIST;
		memset(pGraphListInsert, 0, sizeof(GRAPHLIST));
		//先复制除pCadData、pAcition、pGraph的有用信息
		pGraphListInsert->chKerfComp = pGraphListTemp->chKerfComp;
		pGraphListInsert->nSort = pGraphListTemp->nSort;	//结构序号代表什么？
		//将新插入的pGraphListInsert和pCurGraphList连起来
		if(pCurGraphList)
		{
			pCurGraphList->Next = pGraphListInsert;
			pGraphListInsert->Prev = pCurGraphList;
		}
		else
		{
			if (pGraphListTemp == pGraphList)
			{
				pHeadGraphList = pGraphListInsert;
			}
			pCurGraphList = pGraphListInsert;
		}
		pCurGraphList = pGraphListInsert;

		//复制M代码信息
		if ( pGraphListTemp->pAction != NULL )
		{
			PACTION pActionTemp = new ACTION;
			memset(pActionTemp, 0, sizeof(ACTION));
			pActionTemp->nType = pGraphListTemp->pAction->nType;
			pActionTemp->nCode = pGraphListTemp->pAction->nCode;
			
			pCurGraphList->pAction = pActionTemp;
		}
		//复制图形信息
		else if ( pGraphListTemp->pGraph != NULL )
		{		
			for(PGRAPH pGraphTemp = pGraphListTemp->pGraph; pGraphTemp; pGraphTemp = pGraphTemp->Next ) 
			{
				if (pGraphTemp->chLine == 0)//直线
				{
					PGRAPH pGraphInsert = new GRAPH;
					memset(pGraphInsert, 0, sizeof(GRAPH));
					pGraphInsert->nIndex = pGraphTemp->nIndex;
					pGraphInsert->chLine = pGraphTemp->chLine;
					if (!pCurGraphList->pGraph)
					{
						PCADDATA ptBegData = new CADDATA;
						memset(ptBegData, 0, sizeof(CADDATA));
						ptBegData->PointPara = pGraphTemp->pPosBegin->PointPara;
						pGraphInsert->pPosBegin = ptBegData;
						if (!pCurGraphList->pCadData)
						{
							pCurGraphList->pCadData = ptBegData;
						}
						else
						{
							pCurPtData->pNext = ptBegData;
							ptBegData->pPrev = pCurPtData;
						}
						pCurPtData = ptBegData;
						pCurGraph = pCurGraphList->pGraph = pGraphInsert;
					}
					else
					{
						pGraphInsert->pPosBegin = pCurGraph->pPosEnd;
					}
	
					PCADDATA ptEndData = new CADDATA;
					memset(ptEndData, 0, sizeof(CADDATA));
					ptEndData->PointPara = pGraphTemp->pPosEnd->PointPara;
					pGraphInsert->pPosEnd = ptEndData;
					
					pCurPtData->pNext = ptEndData;
					ptEndData->pPrev = pCurPtData;
					pCurPtData = ptEndData;
					pCurGraph = pGraphInsert;	
				}
				else if (pGraphTemp->chLine == 1)
				{
					PGRAPH pGraphInsert = new GRAPH;
					memset(pGraphInsert, 0, sizeof(GRAPH));
					pGraphInsert->nIndex = pGraphTemp->nIndex;
					pGraphInsert->chLine = pGraphTemp->chLine;
					if (!pCurGraphList->pGraph)
					{
						PCADDATA ptBegData = new CADDATA;
						memset(ptBegData, 0, sizeof(CADDATA));
						ptBegData->PointPara = pGraphTemp->pPosBegin->PointPara;
						pGraphInsert->pPosBegin = ptBegData;
						if (!pCurGraphList->pCadData)
						{
							pCurGraphList->pCadData = ptBegData;
						}
						else
						{
							pCurPtData->pNext = ptBegData;
							ptBegData->pPrev = pCurPtData;
						}
						pCurPtData = ptBegData;
						pCurGraph = pCurGraphList->pGraph = pGraphInsert;
					}
					else
					{
						pGraphInsert->pPosBegin = pCurGraph->pPosEnd;
					}
					
					PCADDATA ptEndData = new CADDATA;
					memset(ptEndData, 0, sizeof(CADDATA));
					ptEndData->PointPara = pGraphTemp->pPosEnd->PointPara;
					pGraphInsert->pPosEnd = ptEndData;
					
					pCurPtData->pNext = ptEndData;
					ptEndData->pPrev = pCurPtData;
					pCurPtData = ptEndData;
	
					pCurGraph->Next = pGraphInsert;
					pGraphInsert->Prev = pCurGraph;
					pCurGraph = pGraphInsert;	
				}
				else if((pGraphTemp->chLine == 2) || (pGraphTemp->chLine == 3)) // 圆弧
				{
					//向当前图形链表结点pCurGraphList中添加结点信息
					PGRAPH pGraphInsert = new GRAPH;
					memset(pGraphInsert, 0, sizeof(GRAPH));
					pGraphInsert->chLine = pGraphTemp->chLine;
					pGraphInsert->nIndex = pGraphTemp->nIndex;
					if (pCurGraphList->pGraph == NULL)
					{
						pCurGraphList->pGraph = pGraphInsert;
						PCADDATA ptBegData = new CADDATA;
						memset(ptBegData, 0, sizeof(CADDATA));
						ptBegData->PointPara = pGraphTemp->pPosBegin->PointPara;
						pGraphInsert->pPosBegin = ptBegData;
						pCurPtData = pCurGraphList->pCadData = ptBegData;
					}
					else
					{
						pCurGraph->Next = pGraphInsert;
						pGraphInsert->Prev = pCurGraph;
						pGraphInsert->pPosBegin = pCurGraph->pPosEnd;
						pCurPtData = pCurGraph->pPosEnd;
					}
					pCurGraph = pGraphInsert;
					
					PCADDATA ptMidData = new CADDATA;
					memset(ptMidData, 0, sizeof(CADDATA));
					ptMidData->PointPara = pGraphTemp->pPosMid->PointPara;
					pCurGraph->pPosMid = ptMidData;
					
					pCurPtData->pNext = ptMidData;
					ptMidData->pPrev = pCurPtData;
					pCurPtData = ptMidData;
					
					PCADDATA ptEndData = new CADDATA;
					memset(ptEndData, 0, sizeof(CADDATA));
					ptEndData->PointPara = pGraphTemp->pPosEnd->PointPara;
					pCurGraph->pPosEnd = ptEndData;
					
					pCurPtData->pNext = ptEndData;
					ptEndData->pPrev = pCurPtData;
					pCurPtData = ptEndData;
					
					//向当前图形链表结点pCurGraphList中添加圆弧信息
					PARCINFO	pArcInfoInsert = new ARCINFO;
					
					pArcInfoInsert->PosCenter = pGraphTemp->pArcInfo->PosCenter;
					pArcInfoInsert->dBeginAngle = pGraphTemp->pArcInfo->dBeginAngle;
					pArcInfoInsert->dEndAngle = pGraphTemp->pArcInfo->dEndAngle;
					
					pArcInfoInsert->dAngleSweep = pGraphTemp->pArcInfo->dAngleSweep;
					pArcInfoInsert->PosIJK.x = pArcInfoInsert->PosCenter.x - pCurGraph->pPosBegin->PointPara.x;
					pArcInfoInsert->PosIJK.y = pArcInfoInsert->PosCenter.y - pCurGraph->pPosBegin->PointPara.y;
					pArcInfoInsert->PosIJK.z = pArcInfoInsert->PosCenter.z - pCurGraph->pPosBegin->PointPara.z;
					
					pCurGraph->pArcInfo = pArcInfoInsert;
				}
			}
		}
	
		pGraphListTemp = pGraphListTemp->Next;
	}
	return pHeadGraphList;
}

