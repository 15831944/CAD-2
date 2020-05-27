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
 *	�õ����յ�����
 */
BOOL CGlassGrind::GetFinishList()
{
	//������Ķ���
	CUndoRedo l_UndoRedo;
	//������������ӵ�һ�����	
	PUNDOLIST l_pUndoList;
	l_pUndoList = l_UndoRedo.NewUndoList();
	l_pUndoList->OperatieWhat = ID_GRIND_GLASS;

	//�ṹת��
	TransToGraphList trans;
	PGRAPHLIST pList = trans.OutGraphList(l_pUndoList);
	//��ѡ�е�ɾ��
	DeleteOriginal(l_pUndoList);
	//���û����ӽ����������������ͷŵ�
	if (l_pUndoList->pGraphPos == NULL)
	{
		delete l_pUndoList;
		l_pUndoList = NULL;
	}

	g_pView->Invalidate();
	return TRUE;
}


/*
 *	ɾ��ѡ�е�ͼ�� 
 */
void CGlassGrind::DeleteOriginal(PUNDOLIST pUndoList)
{	
	//������Ķ���,�����ǳ���
	CUndoRedo l_UndoRedo;
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;
	/////////////////////////////////
	CCreateBase* pSelEntity = NULL,*pCreateBase=NULL;
	POSITION pos = NULL ,pos1=NULL;
	//�������в��ұ���ӵ�ѡ����ͼԪ
	//���������������Զɾ��
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
		//�����������ɾ������
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
	//���ѡ��
	g_pDoc->m_CollectOfPicked.RemoveAll();
	//�����ĵ������»���
	g_pView->Invalidate() ;
}


//���m_pHeadList
void CGlassGrind::ClearGraphList(PGRAPHLIST GraphList)
{
	PGRAPHLIST pGraphListTemp = GraphList;
	while (pGraphListTemp != NULL)
	{
		PGRAPHLIST pGraphListDel = pGraphListTemp;
		
		//�ͷ�pAction
		if (pGraphListDel->pAction != NULL)
		{
			delete pGraphListDel->pAction;
			pGraphListDel->pAction = NULL;
		}
		
		
		//�ͷ�pCadData
		while(pGraphListDel->pCadData != NULL)
		{
			PCADDATA pCadDataDel = pGraphListDel->pCadData;
			pGraphListDel->pCadData = pGraphListDel->pCadData->pNext;
			delete pCadDataDel;
			pCadDataDel = NULL;
		}
		
		//�ͷ�pGraph
		while(pGraphListDel->pGraph != NULL)
		{
			PGRAPH pGraphDel = pGraphListDel->pGraph;
			pGraphListDel->pGraph = pGraphListDel->pGraph->Next;
			
			//�ͷ�Բ����Ϣ
			if (pGraphDel->pArcInfo != NULL)
			{
				delete pGraphDel->pArcInfo;
				pGraphDel->pArcInfo = NULL;
			}
			//�ͷ�ֱ�ߵ���Ϣ
			if (pGraphDel->pPosBegin != NULL)
			{
				delete pGraphDel->pPosBegin;
				pGraphDel->pPosBegin = NULL;
			}
			//�ͷ�ֱ�ߵ���Ϣ
			if (pGraphDel->pPosEnd != NULL)
			{
				delete pGraphDel->pPosEnd;
				pGraphDel->pPosEnd = NULL;
			}
			//�ͷ�ֱ�ߵ���Ϣ
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
 *	����PGRAPHLIST
 */
PGRAPHLIST CGlassGrind::CopyGraphList(PGRAPHLIST pGraphList)
{
	ASSERT(pGraphList != NULL);
	//����Datastruct����
	PGRAPHLIST pGraphListTemp = pGraphList;
	PGRAPHLIST pCurGraphList = NULL;
	PGRAPHLIST pHeadGraphList = NULL;
	PGRAPH pCurGraph = NULL;
	PCADDATA pCurPtData = NULL;
	
	while( pGraphListTemp != NULL )
	{
		PGRAPHLIST  pGraphListInsert = new GRAPHLIST;
		memset(pGraphListInsert, 0, sizeof(GRAPHLIST));
		//�ȸ��Ƴ�pCadData��pAcition��pGraph��������Ϣ
		pGraphListInsert->chKerfComp = pGraphListTemp->chKerfComp;
		pGraphListInsert->nSort = pGraphListTemp->nSort;	//�ṹ��Ŵ���ʲô��
		//���²����pGraphListInsert��pCurGraphList������
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

		//����M������Ϣ
		if ( pGraphListTemp->pAction != NULL )
		{
			PACTION pActionTemp = new ACTION;
			memset(pActionTemp, 0, sizeof(ACTION));
			pActionTemp->nType = pGraphListTemp->pAction->nType;
			pActionTemp->nCode = pGraphListTemp->pAction->nCode;
			
			pCurGraphList->pAction = pActionTemp;
		}
		//����ͼ����Ϣ
		else if ( pGraphListTemp->pGraph != NULL )
		{		
			for(PGRAPH pGraphTemp = pGraphListTemp->pGraph; pGraphTemp; pGraphTemp = pGraphTemp->Next ) 
			{
				if (pGraphTemp->chLine == 0)//ֱ��
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
				else if((pGraphTemp->chLine == 2) || (pGraphTemp->chLine == 3)) // Բ��
				{
					//��ǰͼ��������pCurGraphList����ӽ����Ϣ
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
					
					//��ǰͼ��������pCurGraphList�����Բ����Ϣ
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

