// TransToGraphList.cpp: implementation of the TransToGraphList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "TransToGraphList.h"
#include <cfloat>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TransToGraphList::TransToGraphList()
{
	m_pHeadList = NULL;
	m_pCurrList = NULL;
	m_pCurrGraph = NULL;
	m_pCurrData = NULL;
	memset(&m_ptSet, 0, sizeof(DOUBLEPOINT));
	m_dCurX = m_dCurY = 0.0;
	m_bFlagAddList  = FALSE;
}

TransToGraphList::~TransToGraphList()
{
	//���m_pHeadList
	ClearGraphList();
}

/*
 *	ĥ��������ĺ���
 */
PGRAPHLIST TransToGraphList::OutGraphList(PUNDOLIST list)
{
	m_pUndoList = list;
	//1.���m_pHeadList
	ClearGraphList();
	//2.ת������
	Transfer();
	return m_pHeadList;
}

/*
 *	ĥ������ĺ���
 */
void TransToGraphList::OutMillSide()
{
	CUndoRedo l_UndoRedo;
	m_pUndoList = l_UndoRedo.NewUndoList();
	m_pUndoList->OperatieWhat = ID_GRIND_GLASS;
	//1.���m_pHeadList
	ClearGraphList();
	//�䵶��ת��
	TransMillside();
	//ɾ��ԭͼ
	DeleteOriginal(m_pUndoList);
	//���û��ѡ��ͼ�Σ������� ���ͷ�
	if (m_pUndoList->pGraphPos->pCreateBasepos == NULL)
	{
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû����Ҫ������ͼ��!");
		else
			RemindContent("\r\nThere is no need to knife repairing graphics!");		
		delete m_pUndoList->pGraphPos;
		m_pUndoList->pGraphPos = NULL;
		delete m_pUndoList;
		m_pUndoList = NULL;
	}
	else
	{
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n�������");
		else
			RemindContent("\r\nKnife repairing completion");		
	}
}

/*
 *	ɾ��ѡ�е�ͼ�� 
 */
void TransToGraphList::DeleteOriginal(PUNDOLIST pUndoList)
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
		g_pDoc->m_CCreateBaselayer1.AddTail(pSelEntity);
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
void TransToGraphList::ClearGraphList()
{
	PGRAPHLIST pGraphListTemp = m_pHeadList;
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

			delete pGraphDel;
			pGraphDel = NULL;
		}
		
		pGraphListTemp = pGraphListTemp->Next;
		delete pGraphListDel;
		pGraphListDel = NULL;
	}
	m_pHeadList = NULL;
}

//���GraphList
void TransToGraphList::AddGraphList() 
{
	// TODO: Add your dispatch handler code here
	PGRAPHLIST	pGraphList;
	pGraphList = new GRAPHLIST;
	memset(pGraphList, 0, sizeof(GRAPHLIST));
	pGraphList->chKerfComp = 41;
	pGraphList->dToolSize = 0;
	pGraphList->bClose = FALSE;
	if(m_pCurrList)
	{
		pGraphList->Prev = m_pCurrList;
		m_pCurrList->Next = pGraphList;
	}
	else
	{
		m_pHeadList = m_pCurrList = pGraphList;
	}
	m_pCurrList = pGraphList;
}

//���Action
void TransToGraphList::AddAction() 
{
	if (m_pCurrList)
	{
		PACTION	pAction;
		pAction = new ACTION;
		memset(pAction, 0, sizeof(ACTION));
		m_pCurrList->pAction = pAction;
	}
}

//���CADData
void TransToGraphList::AddCADData()
{
	PCADDATA	pCadBlock = NULL;
	//////////////////////////////////////////////////////////////////////////	
	pCadBlock = new CADDATA;
	memset(pCadBlock, 0, sizeof(CADDATA));
	
	memcpy(&(pCadBlock->PointPara), &m_ptSet, sizeof(DOUBLEPOINT));
	
	if (!m_pCurrList->pCadData)
		m_pCurrList->pCadData = pCadBlock;
	else
	{
		m_pCurrData->pNext = pCadBlock;
		pCadBlock->pPrev = m_pCurrData;
	}
	m_pCurrData = pCadBlock;
}

//���Graph
void TransToGraphList::AddGraph()
{
	PGRAPH	pGraph;
	pGraph = new GRAPH;
	memset(pGraph, 0, sizeof(GRAPH));
	
	if (!m_pCurrList->pGraph)
	{
		m_pCurrList->pGraph = pGraph;
		pGraph->nIndex = 0;
	}
	else
	{
		pGraph->Prev = m_pCurrGraph;
		m_pCurrGraph->Next = pGraph;
		pGraph->nIndex = pGraph->Prev->nIndex + 1;
	}
	m_pCurrGraph = pGraph;
}

//���ArcInfo
void TransToGraphList::AddArcInfo()
{
	PARCINFO	pTempArcInfo;
	if(m_pCurrGraph->pArcInfo==NULL)
	{
		pTempArcInfo = new ARCINFO;
		m_pCurrGraph->pArcInfo = pTempArcInfo;
	}
	else
		pTempArcInfo = m_pCurrGraph->pArcInfo;
}

//ת��ͼ����Ϣ��m_pHeadList��
void TransToGraphList::Transfer()
{	
	POSITION l_POSList = NULL;
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	for( int i = 0 ; i < l_iSize; i++ )
	{
		CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CollectOfPicked[i];
		//��������ѡ��ʲô����G����
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransLine(line);	//ֱ��
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();	
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransPolyline(Polyline);	//���߶�
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransRectangle(Rectangle);//����
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{
			CCreateRound *Round = (CCreateRound*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransRound(Round);//Բ
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransArc(Arc);	//Բ��
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{
			CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransEllipse(Ellipse);//��Բ
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
	}	
}

/*
 *	ת����ϳ�����ͼ��
 */
void TransToGraphList::TransMillside()
{
	POSITION l_POSList = NULL;
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	for( int i = 0 ; i < l_iSize; i++ )
	{
		CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CollectOfPicked[i];
		//��������ѡ��ʲô����G����
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransLine(line);	//ֱ��
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();	
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
			||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransPolyline(Polyline);	//���߶�
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransRectangle(Rectangle);//����
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{
			CCreateRound *Round = (CCreateRound*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransRound(Round);//Բ
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransArc(Arc);	//Բ��
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{
			CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransEllipse(Ellipse);//��Բ
			//ת��β��
			TransTail();
			//�ӵ�������
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//���m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
	}	
}

/*
 *	ת��β��
 */
BOOL TransToGraphList::TransTail()
{
	//��ӻص���0,0����G00
	AddGraphList();
	AddAction();
	m_pCurrList->pAction->nType = 1;
	m_pCurrList->pAction->nCode = MCodeValue(m_strCBM_What);

/*
	AddGraphList();
	m_ptSet.x = m_dCurX;
	m_ptSet.y = m_dCurY;
	AddCADData();

	m_ptSet.x = 0;
	m_ptSet.y = 0;
	AddCADData();

	if ( m_pCurrData->pPrev )
	{
		AddGraph();
		//���ֱ����Ϣ
		m_pCurrGraph->chLine = 0;
		m_pCurrGraph->pPosBegin = m_pCurrData->pPrev;
		m_pCurrGraph->pPosEnd = m_pCurrData;
	}*/


	return TRUE;
}

/*
 *	ת��ֱ��
 */
BOOL TransToGraphList::TransLine(CCreateLine* pLine)
{
	double x1,y1;	//��������
	CString l_strSBM_What;	//M����

	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(pLine->m_strSBM);	
	
	//�������
	x1 = pLine->m_Begin.x;
	y1 = pLine->m_Begin.y;

	//				�����һ�����һ�㲻���							��ɫ��һ��   ��Ҫ��һ��G00
	if((fabs(m_dCurX-x1)>DOUBLE_EQUAL&&fabs(m_dCurY-y1)>DOUBLE_EQUAL)||(m_iColorLast != pLine->m_ColorOfLine))
	{
		DOUBLEPOINT ptEnd;
		ptEnd.x = x1;
		ptEnd.y = y1;
		AddG00(ptEnd, l_strSBM_What);

		AddGraphList();
	}
	

	//�յ������
	DOUBLEPOINT ptEnd;
	ptEnd.x = pLine->m_End.x;
	ptEnd.y = pLine->m_End.y;
	AddG01(ptEnd);
	m_dCurX = ptEnd.x;
	m_dCurY = ptEnd.y;
	
	//�Ѷ�Ӧ��ɫת���ַ���M����������
	m_strCBM_What = AnalysisAryM(pLine->m_strCBM);	
	//��������ɫ��������
	m_iColorLast = pLine->m_ColorOfLine;
	return TRUE;
}

/*
 *	ת����Բ
 */
BOOL TransToGraphList::TransRound(CCreateRound* pRound)
{
	double x1,y1;	//���յ������
	CString l_strSBM_What;	//M����
	//��������
	x1 = pRound->m_EndPos.x;
	y1 = pRound->m_EndPos.y;
	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(pRound->m_strSBM);	
	
	DOUBLEPOINT ptEnd;
	//				�����һ�����һ�㲻���							��ɫ��һ��   ��Ҫ��һ��G00
	if((fabs(m_dCurX-x1)>DOUBLE_EQUAL&&fabs(m_dCurY-y1)>DOUBLE_EQUAL)||(m_iColorLast != pRound->m_ColorOfLine))
	{
		ptEnd.x = x1;
		ptEnd.y = y1;
		AddG00(ptEnd, l_strSBM_What);		
		AddGraphList();
	}	
	m_dCurX = ptEnd.x = x1;
	m_dCurY = ptEnd.y = y1;
	AddG0X(pRound, ptEnd);
	//�Ѷ�Ӧ��ɫת���ַ���M
	m_strCBM_What = AnalysisAryM(pRound->m_strCBM);	
	return TRUE;
}

/*
 *	ת�������
 */
BOOL TransToGraphList::TransPolyline(CCreatePolyline* Polyline)
{
	//�������Բ��
	Position pos,posNext,l_posCenter;	
	//���յ������,͹��
	double x1,y1,x2,y2,l_dTuDu;	
	CString l_content,l_strSBM_What;	//д���ļ��ַ���������

	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(Polyline->m_strSBM);	
	//�㼯�Ķ���
	int l_iSizeOfPoint = Polyline->m_point.GetSize();
	//���ɴ���
	for(int i=0;i<l_iSizeOfPoint-1;i++)
	{
		pos = Polyline->m_point.GetAt(i);
		posNext = Polyline->m_point.GetAt(i+1);
		l_dTuDu = Polyline->m_dTuDu.GetAt(i);
		//�������
		x1 = pos.x;
		y1 = pos.y;
		x2 = posNext.x;
		y2 = posNext.y;
		////����ߵĵ�һ���Ƿ�����һ����ͬ,��ɫ�Ƿ���ͬ 
		if((i==0)&&(fabs(m_dCurX-x1)>DOUBLE_EQUAL||fabs(m_dCurY-y1)>DOUBLE_EQUAL||m_iColorLast != Polyline->m_ColorOfLine))
		{
			DOUBLEPOINT ptEnd;
			ptEnd.x = x1;
			ptEnd.y = y1;
			
			AddG00(ptEnd, l_strSBM_What);
			AddGraphList();
		}
		DOUBLEPOINT ptEnd;
		ptEnd.x = x2;
		ptEnd.y = y2;
		if(l_dTuDu == 0.0)//	ֱ��
			AddG01(ptEnd);
		else//Բ��
		{
			g_pDoc->polylineToArc(pos,posNext,l_dTuDu,l_posCenter);	
			CCreateRoundArc arc;
			arc.m_Begin = pos;
			arc.m_End = posNext;
			arc.m_dRadius = l_posCenter.Distance(pos);
			arc.m_iDirect = RtnArcDir(l_dTuDu);
			arc.m_Center = l_posCenter;

			AddG0X(&arc);
		}
	}
	//�Ѷ�Ӧ��ɫת���ַ���M
	m_strCBM_What = AnalysisAryM(Polyline->m_strCBM);	
	//��������ɫ��������
	m_iColorLast = Polyline->m_ColorOfLine;
	return TRUE;
}

/*
 *	ת����Բ
 */
BOOL TransToGraphList::TransEllipse(CCreateEllipse* Ellipse)
{
	Position pos;	//���
	double x1,y1;	//���յ������
	CString l_content,l_strSBM_What;	//д���ļ��ַ���������
	
	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(Ellipse->m_strSBM);	
	//���ɴ���
	for(int i=0  ;   i < Ellipse->m_point.GetSize()  ;i++)
	{
		pos = Ellipse->m_point.GetAt(i);
		x1 = pos.x;
		y1 = pos.y;		

		DOUBLEPOINT ptEnd;
		ptEnd.x = x1;
		ptEnd.y = y1;

		if (i!=0)//��ʱΪ��һ��
		{
			AddG01(ptEnd);
		}
		else
		{	

			AddG00(ptEnd, l_strSBM_What);
			AddGraphList();
			m_dCurX = x1;
			m_dCurY = y1;
		}
	}
	//�Ѷ�Ӧ��ɫת���ַ���M
	m_strCBM_What = AnalysisAryM(Ellipse->m_strCBM);	
	return TRUE;
}

/*
 *	ת��Բ��
 */
BOOL TransToGraphList::TransArc(CCreateRoundArc* pArc)
{	
	double x1,y1;	//���յ������
	CString l_strSBM_What;	//д���ļ��ַ���������
	//��������
	x1 = pArc->m_Begin.x;
	y1 = pArc->m_Begin.y;
	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(pArc->m_strSBM);	
	//				�����һ�����һ�㲻���							��ɫ��һ��   ��Ҫ��һ��G00
	if((fabs(m_dCurX-x1)>DOUBLE_EQUAL&&fabs(m_dCurY-y1)>DOUBLE_EQUAL)||(m_iColorLast != pArc->m_ColorOfLine))
	{
		DOUBLEPOINT ptEnd;
		ptEnd.x = pArc->m_Begin.x;
		ptEnd.y = pArc->m_Begin.y;
		AddG00(ptEnd, l_strSBM_What);
		AddGraphList();
	}	
	
	//���Բ����Ϣ
	AddG0X(pArc);

	//�Ѷ�Ӧ��ɫת���ַ���M
	m_strCBM_What = AnalysisAryM(pArc->m_strCBM);	
	//��������ɫ��������
	m_iColorLast = pArc->m_ColorOfLine;
	return TRUE;
}

/*
 *	ת������
 */
BOOL TransToGraphList::TransRectangle(CCreateRectangle* pRectangle)
{
	//�������Բ��
	Position pos,posNext,l_posCenter;	
	//���յ������,͹��
	double x1,y1,x2,y2,l_dTuDu;	
	CString l_content,l_strSBM_What;	//д���ļ��ַ���������
	
	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(pRectangle->m_strSBM);	
	//�㼯�Ķ���
	int l_iSizeOfPoint = pRectangle->m_point.GetSize();
	//���ɴ���
	for(int i=0;i<l_iSizeOfPoint-1;i++)
	{
		pos = pRectangle->m_point.GetAt(i);
		posNext = pRectangle->m_point.GetAt(i+1);
		l_dTuDu = pRectangle->m_dTuDu.GetAt(i);
		//�������
		x1 = pos.x;
		y1 = pos.y;
		x2 = posNext.x;
		y2 = posNext.y;
		////����ߵĵ�һ���Ƿ�����һ����ͬ,��ɫ�Ƿ���ͬ 
		if((i==0)&&(fabs(m_dCurX-x1)>DOUBLE_EQUAL||fabs(m_dCurY-y1)>DOUBLE_EQUAL||m_iColorLast != pRectangle->m_ColorOfLine))
		{
			DOUBLEPOINT ptEnd;
			ptEnd.x = x1;
			ptEnd.y = y1;
			
			AddG00(ptEnd, l_strSBM_What);
			AddGraphList();
		}
		DOUBLEPOINT ptEnd;
		ptEnd.x = x2;
		ptEnd.y = y2;
		if(l_dTuDu == 0.0)//	ֱ��
			AddG01(ptEnd);
		else//Բ��
		{
			g_pDoc->polylineToArc(pos,posNext,l_dTuDu,l_posCenter);	
			CCreateRoundArc arc;
			arc.m_Begin = pos;
			arc.m_End = posNext;
			arc.m_dRadius = l_posCenter.Distance(pos);
			arc.m_iDirect = RtnArcDir(l_dTuDu);
			arc.m_Center = l_posCenter;
			
			AddG0X(&arc);
		}
	}
	//�Ѷ�Ӧ��ɫת���ַ���M
	m_strCBM_What = AnalysisAryM(pRectangle->m_strCBM);	
	//��������ɫ��������
	m_iColorLast = pRectangle->m_ColorOfLine;
	return TRUE;
}

/*
 *	������ɫת���ַ�̧��M���ٵĺ���
 */
CString TransToGraphList::CintToCBMstr(int RtnColor)
{
	CString l_strM;		//������ɫת���ַ�
	//�ַ����ⲿ��Ӧ�趨��
	switch(RtnColor)	
	{
	case 1:
		l_strM = g_pDoc->m_stcColorToM.m_CBM1;
		break;
	case 2:
		l_strM = g_pDoc->m_stcColorToM.m_CBM2;
		break;
	case 3:
		l_strM = g_pDoc->m_stcColorToM.m_CBM3;
		break;
	case 4:
		l_strM = g_pDoc->m_stcColorToM.m_CBM4;
		break;
	case 5:
		l_strM = g_pDoc->m_stcColorToM.m_CBM5;
		break;
	default:
		break;
	}
	return l_strM;
}

/*
 *	������ɫת���ַ��µ�M���ٵĺ���
 */
CString TransToGraphList::CintToSBMstr(int RtnColor)
{
	CString l_strM;		//������ɫת���ַ�
	//�ַ����ⲿ��Ӧ�趨��
	switch(RtnColor)	
	{
	case 1:
		l_strM = g_pDoc->m_stcColorToM.m_SBM1;
		break;
	case 2:
		l_strM = g_pDoc->m_stcColorToM.m_SBM2;
		break;
	case 3:
		l_strM = g_pDoc->m_stcColorToM.m_SBM3;
		break;
	case 4:
		l_strM = g_pDoc->m_stcColorToM.m_SBM4;
		break;
	case 5:
		l_strM = g_pDoc->m_stcColorToM.m_SBM5;
		break;
	default:
		break;
	}
	return l_strM;
}

//��ȡM�����ֵ
int TransToGraphList::MCodeValue(CString str)
{
	str.TrimLeft(_T(" M"));
	str.TrimRight(_T(" "));
	return atoi(str);
}

//���ܣ����G00
//������DOUBLEPOINT ptEnd��G00��ĩ�㣨������m_dCurX,m_dCurY��
//		CString l_strSBM_What��G00֮���M����
BOOL TransToGraphList::AddG00(DOUBLEPOINT ptEnd, CString l_strSBM_What)
{
	//���MCode
	if (m_strCBM_What != "")
	{
		AddGraphList();
		AddAction();
		m_pCurrList->pAction->nType = 1;
		m_pCurrList->pAction->nCode = MCodeValue(m_strCBM_What);
	}
	AddGraphList();
	//���G00
	m_ptSet.x = m_dCurX;
	m_ptSet.y = m_dCurY;
	AddCADData();

	m_ptSet.x = ptEnd.x;
	m_ptSet.y = ptEnd.y;
	AddCADData();

	m_dCurX = ptEnd.x;
	m_dCurY = ptEnd.y;

	if ( m_pCurrData->pPrev )
	{
		AddGraph();
		//���ֱ����Ϣ
		m_pCurrGraph->chLine = 0;
		m_pCurrGraph->pPosBegin = m_pCurrData->pPrev;
		m_pCurrGraph->pPosEnd = m_pCurrData;
	}
	//���MCode
	if (l_strSBM_What != "")
	{
		AddGraphList();
		AddAction();
		m_pCurrList->pAction->nType = 1;
		m_pCurrList->pAction->nCode = MCodeValue(l_strSBM_What);
	}
	
	return TRUE;
}

//���ܣ����G01
//������DOUBLEPOINT ptEnd��G01��ĩ�㣨������m_dCurX,m_dCurY��
BOOL TransToGraphList::AddG01(DOUBLEPOINT ptEnd)
{
	if (m_pCurrList->pCadData == NULL)
	{
		m_ptSet.x = m_dCurX;
		m_ptSet.y = m_dCurY;
		AddCADData();
	}

	m_ptSet.x = ptEnd.x;
	m_ptSet.y = ptEnd.y;
	AddCADData();
	m_dCurX = m_ptSet.x;
	m_dCurY = m_ptSet.y;
	if ( m_pCurrData->pPrev )
	{
		AddGraph();
		//���ֱ����Ϣ
		m_pCurrGraph->chLine = 1;
		m_pCurrGraph->pPosBegin = m_pCurrData->pPrev;
		m_pCurrGraph->pPosEnd = m_pCurrData;
	}
	return TRUE;
}

//���ܣ����G02/03
//������CCreateRoundArc *pArc��Բ����Ϣ
BOOL TransToGraphList::AddG0X(CCreateRoundArc *pArc)
{
	DOUBLEPOINT ptBeg, ptMid, ptEnd;
	ptBeg.x = pArc->m_Begin.x;
	ptBeg.y = pArc->m_Begin.y;

	if (m_pCurrList->pGraph != NULL)	//��ͼ���뵱ǰԲ����ӣ���ʱԲ����������һ��ͼ��ĩ��
	{
		AddGraph();
		m_pCurrGraph->pPosBegin = m_pCurrGraph->Prev->pPosEnd;
	}
	else								//û��ͼ���뵱ǰԲ����ӣ��������������Բ�����
	{
		AddGraph();
		m_ptSet.x = ptBeg.x;
		m_ptSet.y = ptBeg.y;
		AddCADData();
		m_pCurrList->pGraph->pPosBegin =  m_pCurrData;
	}
	
	//���Բ���е�
	ptMid.x = ptMid.y = 0.0;
	m_ptSet.x = m_ptSet.y = 0.0;
	AddCADData();
		
	//���Բ���յ�
	ptEnd.x = pArc->m_End.x;
	ptEnd.y = pArc->m_End.y;
	m_dCurX = m_ptSet.x = pArc->m_End.x;
	m_dCurY = m_ptSet.y = pArc->m_End.y;
	AddCADData();

	//����Բ�����յ�
	m_pCurrGraph->pPosMid = m_pCurrData->pPrev;
	m_pCurrGraph->pPosEnd = m_pCurrData;
	
	if (pArc->m_iDirect == AD_CLOCKWISE)//�����˳ʱ��
		m_pCurrGraph->chLine = 2;
	else//������ʱ��
		m_pCurrGraph->chLine = 3;
	
	DOUBLEPOINT ptCenter, ptIJK;
	//Բ��
	ptCenter.x = pArc->m_Center.x;
	ptCenter.y = pArc->m_Center.y;
	ptCenter.z = 0;
	//Բ�������������
	ptIJK.x = pArc->m_Center.x - pArc->m_Begin.x;
	ptIJK.y = pArc->m_Center.y - pArc->m_Begin.y;
	ptIJK.z = 0;

	double dBeginPointAngle, dEndPointAngle;
	//��ʼ�Ƕ�
	dBeginPointAngle = PointToPointAngle(ptCenter, ptBeg);
	//��ֹ�Ƕ�
	dEndPointAngle = PointToPointAngle(ptCenter, ptEnd);
	
	//���Բ����Ϣ
	AddArcInfo();
	m_pCurrGraph->pArcInfo->PosCenter = ptCenter;
	m_pCurrGraph->pArcInfo->PosIJK = ptIJK;
	m_pCurrGraph->pArcInfo->dBeginAngle = dBeginPointAngle;
	m_pCurrGraph->pArcInfo->dEndAngle = dEndPointAngle;
	m_pCurrGraph->pArcInfo->dAngleSweep = CalculateTraverseAngle(m_pCurrGraph->chLine, dBeginPointAngle, dEndPointAngle);

	return TRUE;
}

//���ܣ����G02/03
//������CCreateRound *pRound����Բ��Ϣ
//		DOUBLEPOINT ptBegAndEnd����Բ�������յ�
//		pRound->m_EndPos��Ϊ�û�����ĵ㣬����Բ�����յ�ΪԲ��x��������Բ�Ľ���
BOOL TransToGraphList::AddG0X(CCreateRound *pRound, DOUBLEPOINT ptBegAndEnd)
{	
	DOUBLEPOINT ptBeg, ptMid, ptEnd;
	ptBeg.x = ptBegAndEnd.x;
	ptBeg.y = ptBegAndEnd.y;
	
	if (m_pCurrList->pGraph != NULL)	//��ͼ���뵱ǰԲ����ӣ���ʱԲ����������һ��ͼ��ĩ��
	{
		AddGraph();
		m_pCurrGraph->pPosBegin = m_pCurrGraph->Prev->pPosEnd;
	}
	else								//û��ͼ���뵱ǰԲ����ӣ��������������Բ�����
	{
		AddGraph();
		m_ptSet.x = ptBeg.x;
		m_ptSet.y = ptBeg.y;
		AddCADData();
		m_pCurrList->pGraph->pPosBegin =  m_pCurrData;
	}
	
	//����е�
	ptMid.x = ptMid.y = 0.0;
	m_ptSet.x = m_ptSet.y = 0.0;
	AddCADData();

	//����յ�
	ptEnd.x = ptBegAndEnd.x;
	ptEnd.y = ptBegAndEnd.y;
	m_dCurX = m_ptSet.x = ptBegAndEnd.x;
	m_dCurY = m_ptSet.y = ptBegAndEnd.y;
	AddCADData();
	
	//������Բ���յ�
	m_pCurrGraph->pPosMid = m_pCurrData->pPrev;
	m_pCurrGraph->pPosEnd = m_pCurrData;
	
	if (pRound->m_iDirect == AD_CLOCKWISE)//�����˳ʱ��
		m_pCurrGraph->chLine = 2;
	else//������ʱ��
		m_pCurrGraph->chLine = 3;
	
	DOUBLEPOINT ptCenter, ptIJK;
	//Բ��
	ptCenter.x = pRound->m_Center.x;
	ptCenter.y = pRound->m_Center.y;
	ptCenter.z = 0.0;
	//Բ�������������
	ptIJK.x = pRound->m_Center.x - ptBegAndEnd.x;
	ptIJK.y = pRound->m_Center.y - ptBegAndEnd.y;
	ptIJK.z = 0.0;
	
	double dBeginPointAngle, dEndPointAngle;
	//��ʼ�Ƕ�
	dBeginPointAngle = PointToPointAngle(ptCenter, ptBeg);
	//��ֹ�Ƕ�
	dEndPointAngle = PointToPointAngle(ptCenter, ptEnd);
	
	//���Բ����Ϣ
	AddArcInfo();
	m_pCurrGraph->pArcInfo->PosCenter = ptCenter;
	m_pCurrGraph->pArcInfo->PosIJK = ptIJK;
	m_pCurrGraph->pArcInfo->dBeginAngle = dBeginPointAngle;
	m_pCurrGraph->pArcInfo->dEndAngle = dEndPointAngle;
	m_pCurrGraph->pArcInfo->dAngleSweep = CalculateTraverseAngle(m_pCurrGraph->chLine, dBeginPointAngle, dEndPointAngle);
	
	return TRUE;
}

//����ֱ�߽Ƕ�
double TransToGraphList::PointToPointAngle(DOUBLEPOINT ptBeg, DOUBLEPOINT ptEnd)
{
#ifndef MATH_PI
#define MATH_PI 3.1415926535897
#endif
	double angle;
	double dx = ptEnd.x - ptBeg.x;
	double dy = ptEnd.y - ptBeg.y;
	
	if (fabs(dx-0.0) < DBL_MIN)		
	{
		if (dy > 0.0)	
			angle = 90;
		else
			angle = 270;
	}		
	else if (fabs(dy-0.0) < DBL_MIN)
	{
		if (dx > 0.0)
			angle = 0;
		else 
			angle = 180;
	}
	else
	{
		if (fabs(dx-0.0)<DBL_MIN && fabs(dy-0.0)<DBL_MIN)
			angle = 0.0;
		else 
			angle = (180/MATH_PI) * atan2(dy, dx);
		
		if (angle < 0)		angle += 360;      //���ڸ��������360�����Եõ�0-360�ڵĽ��
		else if (angle > 360)      angle -= 360;	  //�Դ���360�Ľ����360�����Եõ�0-360�ڵĽ��
	}
	
	return angle;
}

//����ɨ���ĽǶ�
double TransToGraphList::CalculateTraverseAngle(char chDirection, double dBeginAngle, double dEndAngle)
{
	double fTraverseAngle;
	
	fTraverseAngle = dEndAngle - dBeginAngle;   
	if (chDirection == 3)/* ��ʱ�� */
	{
		if (dEndAngle < dBeginAngle)
			fTraverseAngle += 360; /* Wrap around the clock */
	}
	else
	{  
		if (dEndAngle > dBeginAngle)
			fTraverseAngle -= 360; /* Wrap around the clock */ 
	}   
	
	if (fabs(fTraverseAngle) < .000001)
		fTraverseAngle = 0;
	
	if (!fTraverseAngle)
	{
		if (chDirection == 3)
			fTraverseAngle = 360;
		else
			fTraverseAngle = -360;
	}
	
	return fTraverseAngle; 
}

void TransToGraphList::WriteGraphList(PGRAPHLIST pList)
{
	CString strRes;
	CString strLine;
	PGRAPHLIST pTempGraphList = pList;
	while( pTempGraphList != NULL )
	{
		if ( pTempGraphList->pAction != NULL )
		{
			//���Ѿ�ת��Ϊ������ϵ��ͼ��������Ѱ�Ҿ���ptStart����ĵ�
			//��pAction�����޸�
		}
		else if ( pTempGraphList->pGraph != NULL )
		{
			if ( pTempGraphList->pGraph->chLine == 0  )		//����ֱ��
			{
				strLine.Format("bg:%f %f en:%f %f\r\n", pTempGraphList->pGraph->pPosBegin->PointPara.x, pTempGraphList->pGraph->pPosBegin->PointPara.y, \
					pTempGraphList->pGraph->pPosEnd->PointPara.x, pTempGraphList->pGraph->pPosEnd->PointPara.y);
				strRes += strLine;
				strLine.Format("--------------------------------------------\r\n");
				strRes += strLine;
			}
			else 
			{
				for ( PGRAPH pTempGraph=pTempGraphList->pGraph; pTempGraph; pTempGraph = pTempGraph->Next )
				{
					if (pTempGraph->chLine == 1)
					{
						double x = pTempGraph->pPosBegin->PointPara.x;
						double y = pTempGraph->pPosBegin->PointPara.y;
						strLine.Format("bg:%f %f en:%f %f\r\n", pTempGraph->pPosBegin->PointPara.x, pTempGraph->pPosBegin->PointPara.y, \
							pTempGraph->pPosEnd->PointPara.x, pTempGraph->pPosEnd->PointPara.y);
						strRes += strLine;
					}
					else if (pTempGraph->chLine == 2 || pTempGraph->chLine == 3)
					{
						strLine.Format("bg:%f %f en:%f %f ", pTempGraph->pPosBegin->PointPara.x, pTempGraph->pPosBegin->PointPara.y, \
							pTempGraph->pPosEnd->PointPara.x, pTempGraph->pPosEnd->PointPara.y);
						strRes += strLine;
						strLine.Format("center:%f %f IJK:%f %f beginAngle: %f endAngle: %f sweepAngle: %f\r\n", pTempGraph->pArcInfo->PosCenter.x, pTempGraph->pArcInfo->PosCenter.y, \
							pTempGraph->pArcInfo->PosIJK.x, pTempGraph->pArcInfo->PosIJK.y, pTempGraph->pArcInfo->dBeginAngle, 
							pTempGraph->pArcInfo->dEndAngle, pTempGraph->pArcInfo->dAngleSweep);
						strRes += strLine;
					}	
				}
			}
		}
		
		pTempGraphList = pTempGraphList->Next;
	}
	strLine.Format("====================================================================\r\n");
	strRes += strLine;
	CFile file("ptData.txt", CFile::modeCreate|CFile::modeWrite);
	file.Write(strRes.GetBuffer(strRes.GetLength()), strRes.GetLength());
	file.Close();
}

/*
 *	GRAPHLISTת��Createbase
 */
void TransToGraphList::ListToCreateForMillSide(PGRAPHLIST pHeadList,POSITION POS)
{
	PGRAPHLIST pGraphListTemp = pHeadList;
	CArray<Position,Position&> l_aryPoint;//�㼯
	CArray<double,double&> l_aryTuDu;//���͹�ȵļ���
	Position l_posBegin,l_posEnd,l_posCenter,l_posTemp;//��ʱ�õ�������
	double l_dTuDu;//��ʱ�õ�͹��
	COLORREF l_RGBGraphColor;	//�ߵ���ɫ
	int l_iMcode,l_iKnifeStyle;
	//�ж����ҵ�������ÿ��ͼ����
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iKnifeStyle = 41;
	else
		l_iKnifeStyle = 42;

	while(pGraphListTemp != NULL)
	{
		if (pGraphListTemp->pAction != NULL) //�����M����
		{
			if (m_bFlagAddList == FALSE)//G00�����M����
				m_iColorLast = pGraphListTemp->pAction->nCode;
			else//����ͼ���M����
			{
				//�����ɵĵ�·����ɫ
				l_iMcode = pGraphListTemp->pAction->nCode;				
				l_RGBGraphColor  = RtnColorOfKnife(0,l_iMcode);
				//Ȼ��㼯�����ҵĽṹ
				l_dTuDu = 0.0;
				l_aryPoint.Add(l_posEnd);
				l_aryTuDu.Add(l_dTuDu);
				if(l_aryPoint.GetSize() == 2&&l_aryTuDu.GetAt(0) != 0.0)//˵��ֻ��һ��Բ��
				{
					//͹��
					l_dTuDu = l_aryTuDu.GetAt(0);
					l_posBegin = l_aryPoint.GetAt(0);
					l_posEnd = l_aryPoint.GetAt(1);
					g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTuDu,l_posCenter);
					//��Բ��
					CCreateRoundArc *l_pTempArc = new CCreateRoundArc(l_posBegin,l_posCenter,l_posEnd,RtnArcDir(l_dTuDu));
					l_pTempArc->m_iKnifeStyle = l_iKnifeStyle;	
					//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
					m_CollectOfG00.Add(l_pTempArc);
					//������������� 
					g_pDoc->m_CCreateBaseList.InsertBefore(POS,l_pTempArc);//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
					AddUndoList((LONG)l_pTempArc,POS);
				}
				else //˵���Ƕ����
				{
					if(l_aryPoint.GetSize() > 1)
					{
						CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
						pPolyline->m_iKnifeStyle = l_iKnifeStyle;
						//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
						m_CollectOfG00.Add(pPolyline);
						//������������� 
						g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
						AddUndoList((LONG)pPolyline,POS);
					}
				}
				//��һ��M ֮���ͼԪ������ɫ���ӵ�������
				for( int i = 0 ; i < m_CollectOfG00.GetSize() ; i++ )
				{
					CCreateBase*	pSelEntity = (CCreateBase*) m_CollectOfG00[i] ;
					pSelEntity->m_ColorOfLine = l_RGBGraphColor;
				}
				m_CollectOfG00.RemoveAll();
				l_aryPoint.RemoveAll();//����ɾ����
				l_aryTuDu.RemoveAll();	
			}
		}
		//�������M���룬��G����
		else if(pGraphListTemp->pGraph != NULL)	
		{
			m_bFlagAddList = TRUE;
			if (pGraphListTemp->pGraph->chLine == 0) //�����G00
			{
				PosToPOS(l_posEnd,pGraphListTemp->pGraph->pPosEnd->PointPara);
				m_bFlagAddList = FALSE;
			}
			else//������G01��G02��G03,��ϸ�ɿ�COpenGCode�Ǹ���Ľṹ
			{		
				m_bFlagAddList = TRUE;			
				for(PGRAPH pGraphTemp=pGraphListTemp->pGraph; pGraphTemp; pGraphTemp=pGraphTemp->Next)
				{
					if (pGraphTemp->chLine == 1) 
					{
						l_aryPoint.Add(l_posEnd);
						l_dTuDu = 0.0;
						l_aryTuDu.Add(l_dTuDu);
						PosToPOS(l_posEnd,pGraphTemp->pPosEnd->PointPara);
					}
					else if(pGraphTemp->chLine == 2)
					{
						PosToPOS(l_posBegin,pGraphTemp->pPosBegin->PointPara);
						PosToPOS(l_posCenter,pGraphTemp->pArcInfo->PosCenter);
						PosToPOS(l_posTemp,pGraphTemp->pPosEnd->PointPara);
						if(PosEqual(l_posEnd,l_posTemp))//�����Բ�Ļ�
						{
							if(l_aryPoint.GetSize()>1)//���û�ж���ߣ��Ͳ����������
							{
								l_aryPoint.Add(l_posEnd);
								l_aryTuDu.Add(l_dTuDu);
								CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
								pPolyline->m_iKnifeStyle = l_iKnifeStyle;
								//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
								m_CollectOfG00.Add(pPolyline);
								g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
								AddUndoList((LONG)pPolyline,POS);
							}
							l_aryPoint.RemoveAll();
							l_aryTuDu.RemoveAll();	
							CCreateRound *pRound  = new CCreateRound(l_posCenter,l_posBegin,AD_CLOCKWISE);
							pRound->m_iKnifeStyle = l_iKnifeStyle;
							//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
							m_CollectOfG00.Add(pRound);
							//������������� 
							g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound);
							AddUndoList((LONG)pRound,POS);
						}
						else//������Բ��
						{
							l_aryPoint.Add(l_posEnd);
							l_posEnd = l_posTemp;
							l_dTuDu = g_pDoc->RtnTudu(l_posBegin,l_posEnd,l_posCenter,AD_CLOCKWISE);
							l_aryTuDu.Add(l_dTuDu);
						}
					}
					else if (pGraphTemp->chLine == 3) 
					{
						PosToPOS(l_posBegin,pGraphTemp->pPosBegin->PointPara);
						PosToPOS(l_posCenter,pGraphTemp->pArcInfo->PosCenter);
						PosToPOS(l_posTemp,pGraphTemp->pPosEnd->PointPara);
						if(PosEqual(l_posEnd,l_posTemp))//�����Բ�Ļ�
						{
							if(l_aryPoint.GetSize()>1)//���û�ж���ߣ��Ͳ����������
							{
								l_aryPoint.Add(l_posEnd);
								l_aryTuDu.Add(l_dTuDu);
								CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
								pPolyline->m_iKnifeStyle = l_iKnifeStyle;
								//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
								m_CollectOfG00.Add(pPolyline);
								//������������� 
								g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
								AddUndoList((LONG)pPolyline,POS);
							}
							l_aryPoint.RemoveAll();
							l_aryTuDu.RemoveAll();
							CCreateRound *pRound  = new CCreateRound(l_posCenter,l_posBegin,AD_COUNTERCLOCKWISE);
							pRound->m_iKnifeStyle = l_iKnifeStyle;
							//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
							m_CollectOfG00.Add(pRound);
							//������������� 
							g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound);
							AddUndoList((LONG)pRound,POS);
						}
						else//������Բ��
						{
							l_aryPoint.Add(l_posEnd);
							l_posEnd = l_posTemp;
							l_dTuDu = g_pDoc->RtnTudu(l_posBegin,l_posEnd,l_posCenter,AD_COUNTERCLOCKWISE);
							l_aryTuDu.Add(l_dTuDu);
						}
					}	
				}
			}
		}
		pGraphListTemp = pGraphListTemp->Next;
	}	
}

/*
 *	��DATASTRUCT�Ľṹת��CreateBase�ṹ
 */
BOOL TransToGraphList::GraphListTurnCreateStc(PGRAPHLIST pHeadList,POSITION POS,int StyleKnife)
{
	PGRAPHLIST pGraphListTemp = pHeadList;
	CArray<Position,Position&> l_aryPoint;//�㼯
	CArray<double,double&> l_aryTuDu;//���͹�ȵļ���
	Position l_posBegin,l_posEnd,l_posCenter,l_posTemp;//��ʱ�õ�������
	double l_dTuDu;//��ʱ�õ�͹��
	COLORREF l_RGBGraphColor;	//�ߵ���ɫ
	int l_iMcode,l_iKnifeStyle,l_iIsBeginGraph = 2;
	//�ж����ҵ�������ÿ��ͼ����
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iKnifeStyle = 41;
	else
		l_iKnifeStyle = 42;
	//�ж��ǲ���ԭʼ��ͼ��
	if(StyleKnife == 0)
		l_iIsBeginGraph = 1;
	if(StyleKnife == 1)
		l_iIsBeginGraph = 0;

	while(pGraphListTemp != NULL)
	{
		if (pGraphListTemp->pAction != NULL) //�����M����
		{
			if (m_bFlagAddList == FALSE)//G00�����M����
				m_iColorLast = pGraphListTemp->pAction->nCode;
			else//����ͼ���M����
			{
				//�����ɵĵ�·����ɫ
				l_iMcode = pGraphListTemp->pAction->nCode;				
				l_RGBGraphColor  = RtnColorOfKnife(StyleKnife,l_iMcode);
				//Ȼ��㼯�����ҵĽṹ
				l_dTuDu = 0.0;
				l_aryPoint.Add(l_posEnd);
				l_aryTuDu.Add(l_dTuDu);
				if(l_aryPoint.GetSize() == 2&&l_aryTuDu.GetAt(0) != 0.0)//˵��ֻ��һ��Բ��
				{
					//͹��
					l_dTuDu = l_aryTuDu.GetAt(0);
					l_posBegin = l_aryPoint.GetAt(0);
					l_posEnd = l_aryPoint.GetAt(1);
					g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTuDu,l_posCenter);
					//��Բ��
					CCreateRoundArc *l_pTempArc = new CCreateRoundArc(l_posBegin,l_posCenter,l_posEnd,RtnArcDir(l_dTuDu));
					l_pTempArc->m_iKnifeStyle = l_iKnifeStyle;	
					l_pTempArc->m_iIsBeginGraph = l_iIsBeginGraph;
					//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
					m_CollectOfG00.Add(l_pTempArc);
					//������������� 
					g_pDoc->m_CCreateBaseList.InsertBefore(POS,l_pTempArc);//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
					AddUndoList((LONG)l_pTempArc,POS);
				}
				else //˵���Ƕ����
				{
					if(l_aryPoint.GetSize() > 1)
					{
						CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
						pPolyline->m_iKnifeStyle = l_iKnifeStyle;
						pPolyline->m_iIsBeginGraph = l_iIsBeginGraph;
						//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
						m_CollectOfG00.Add(pPolyline);
						//������������� 
						g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
						AddUndoList((LONG)pPolyline,POS);
					}
				}
				//��һ��M ֮���ͼԪ������ɫ���ӵ�������
				for( int i = 0 ; i < m_CollectOfG00.GetSize() ; i++ )
				{
					CCreateBase*	pSelEntity = (CCreateBase*) m_CollectOfG00[i] ;
					pSelEntity->m_ColorOfLine = l_RGBGraphColor;
				}
				m_CollectOfG00.RemoveAll();
				l_aryPoint.RemoveAll();//����ɾ����
				l_aryTuDu.RemoveAll();	
			}
		}
		//�������M���룬��G����
		else if(pGraphListTemp->pGraph != NULL)	
		{
			m_bFlagAddList = TRUE;
			if (pGraphListTemp->pGraph->chLine == 0) //�����G00
			{
				PosToPOS(l_posEnd,pGraphListTemp->pGraph->pPosEnd->PointPara);
				m_bFlagAddList = FALSE;
			}
			else//������G01��G02��G03,��ϸ�ɿ�COpenGCode�Ǹ���Ľṹ
			{		
				m_bFlagAddList = TRUE;			
				for(PGRAPH pGraphTemp=pGraphListTemp->pGraph; pGraphTemp; pGraphTemp=pGraphTemp->Next)
				{
					if (pGraphTemp->chLine == 1) 
					{
						l_aryPoint.Add(l_posEnd);
						l_dTuDu = 0.0;
						l_aryTuDu.Add(l_dTuDu);
						PosToPOS(l_posEnd,pGraphTemp->pPosEnd->PointPara);
					}
					else if(pGraphTemp->chLine == 2)
					{
						PosToPOS(l_posBegin,pGraphTemp->pPosBegin->PointPara);
						PosToPOS(l_posCenter,pGraphTemp->pArcInfo->PosCenter);
						PosToPOS(l_posTemp,pGraphTemp->pPosEnd->PointPara);
						if(PosEqual(l_posEnd,l_posTemp))//�����Բ�Ļ�
						{
							if(l_aryPoint.GetSize()>1)//���û�ж���ߣ��Ͳ����������
							{
								l_aryPoint.Add(l_posEnd);
								l_aryTuDu.Add(l_dTuDu);
								CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
								pPolyline->m_iKnifeStyle = l_iKnifeStyle;
								pPolyline->m_iIsBeginGraph = l_iIsBeginGraph;
								//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
								m_CollectOfG00.Add(pPolyline);
								g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
								AddUndoList((LONG)pPolyline,POS);
							}
							l_aryPoint.RemoveAll();
							l_aryTuDu.RemoveAll();	
							CCreateRound *pRound  = new CCreateRound(l_posCenter,l_posBegin,AD_CLOCKWISE);
							pRound->m_iKnifeStyle = l_iKnifeStyle;
							pRound->m_iIsBeginGraph = l_iIsBeginGraph;
							//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
							m_CollectOfG00.Add(pRound);
							//������������� 
							g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound);
							AddUndoList((LONG)pRound,POS);
						}
						else//������Բ��
						{
							l_aryPoint.Add(l_posEnd);
							l_posEnd = l_posTemp;
							l_dTuDu = g_pDoc->RtnTudu(l_posBegin,l_posEnd,l_posCenter,AD_CLOCKWISE);
							l_aryTuDu.Add(l_dTuDu);
						}
					}
					else if (pGraphTemp->chLine == 3) 
					{
						PosToPOS(l_posBegin,pGraphTemp->pPosBegin->PointPara);
						PosToPOS(l_posCenter,pGraphTemp->pArcInfo->PosCenter);
						PosToPOS(l_posTemp,pGraphTemp->pPosEnd->PointPara);
						if(PosEqual(l_posEnd,l_posTemp))//�����Բ�Ļ�
						{
							if(l_aryPoint.GetSize()>1)//���û�ж���ߣ��Ͳ����������
							{
								l_aryPoint.Add(l_posEnd);
								l_aryTuDu.Add(l_dTuDu);
								CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
								pPolyline->m_iKnifeStyle = l_iKnifeStyle;
								pPolyline->m_iIsBeginGraph = l_iIsBeginGraph;
								//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
								m_CollectOfG00.Add(pPolyline);
								//������������� 
								g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
								AddUndoList((LONG)pPolyline,POS);
							}
							l_aryPoint.RemoveAll();
							l_aryTuDu.RemoveAll();
							CCreateRound *pRound  = new CCreateRound(l_posCenter,l_posBegin,AD_COUNTERCLOCKWISE);
							pRound->m_iKnifeStyle = l_iKnifeStyle;
							pRound->m_iIsBeginGraph = l_iIsBeginGraph;
							//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
							m_CollectOfG00.Add(pRound);
							//������������� 
							g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound);
							AddUndoList((LONG)pRound,POS);
						}
						else//������Բ��
						{
							l_aryPoint.Add(l_posEnd);
							l_posEnd = l_posTemp;
							l_dTuDu = g_pDoc->RtnTudu(l_posBegin,l_posEnd,l_posCenter,AD_COUNTERCLOCKWISE);
							l_aryTuDu.Add(l_dTuDu);
						}
					}	
				}
			}
		}
		pGraphListTemp = pGraphListTemp->Next;
	}	
	return TRUE;
}

/*
 *	���ش֣������׹����ɫ
 */
COLORREF TransToGraphList::RtnColorOfKnife(int StyleKnife,int l_iMcode)
{
	COLORREF l_RGBGraphColor;	//�ߵ���ɫ
	if(StyleKnife == 0)
	{
		if (m_iColorLast == MCodeValue(g_pDoc->m_stcColorToM.m_SBM1)&&l_iMcode == MCodeValue(g_pDoc->m_stcColorToM.m_CBM1))	
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color1;	//��һɫ
		else if (m_iColorLast == MCodeValue(g_pDoc->m_stcColorToM.m_SBM2)&&l_iMcode == MCodeValue(g_pDoc->m_stcColorToM.m_CBM2))
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color2;	//����ɫ
		else if (m_iColorLast == MCodeValue(g_pDoc->m_stcColorToM.m_SBM3)&&l_iMcode == MCodeValue(g_pDoc->m_stcColorToM.m_CBM3))
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color3;	//����ɫ
		else if (m_iColorLast == MCodeValue(g_pDoc->m_stcColorToM.m_SBM4)&&l_iMcode == MCodeValue(g_pDoc->m_stcColorToM.m_CBM4))
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color4;	//����ɫ
		else if (m_iColorLast == MCodeValue(g_pDoc->m_stcColorToM.m_SBM5)&&l_iMcode == MCodeValue(g_pDoc->m_stcColorToM.m_CBM5))
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color5;	//����ɫ
		else//�����趨��M�����Ӧ����ɫ����õ���ɫ���ɣ�����^*^
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color5;
	}
	else
	{
		if(StyleKnife == 1)
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color1;	//��һɫ
		if(StyleKnife == 2)
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color2;	//����ɫ
		if(StyleKnife == 3)
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color3;	//����ɫ
		}	 
	return l_RGBGraphColor;
}

/*
 *	�ӵ������� 
 */
PGRAPHLIST TransToGraphList::AddKnifeStyleForMillSide(long lData)
{
	int l_iDaoBu = 0;	//�������� 41�󵶲� �� 42�ҵ���
	if(g_pDoc->m_stcClothCut.m_bMillSideLorR == 1)
		l_iDaoBu = 41;
	else
		l_iDaoBu = 42;
	//�ӵ���
	PGRAPHLIST  RtnKnife = (PGRAPHLIST)lData;
	while(RtnKnife != NULL)
	{
		RtnKnife->chKerfComp = l_iDaoBu;	
		RtnKnife = RtnKnife->Next;
	}
	return RtnKnife;
}

/*
*	�ӵ������� 
*/
PGRAPHLIST TransToGraphList::AddKnifeStyle(long lData)
{
	int l_iDaoBu = 0;	//�������� 41�󵶲� �� 42�ҵ���
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iDaoBu = 41;
	else
		l_iDaoBu = 42;
	//�ӵ���
	PGRAPHLIST  RtnKnife = (PGRAPHLIST)lData;
	while(RtnKnife != NULL)
	{
		RtnKnife->chKerfComp = l_iDaoBu;	
		RtnKnife = RtnKnife->Next;
	}
	return RtnKnife;
}

/*
 *	�ӵ����ߴ��ϳ��ͼ��
 */
void TransToGraphList::AddKnifeLongForMillSide(POSITION POS)
{
	PGRAPHLIST CreateList = NULL;	
	CKnifeData  Dllknife;
	//�Ӵ�ĥ
	double l_iTempSize = g_pDoc->m_stcClothCut.m_dMillSideDis;
	Dllknife.Knife_SetGraphData((long)m_pHeadList,l_iTempSize); 
	CreateList = (PGRAPHLIST)Dllknife.Knife_GetGraphFinishData();
	ListToCreateForMillSide(CreateList,POS);
}


/*
 *	�ӵ����ߴ�
 */
void TransToGraphList::AddKnifeLong(POSITION POS)
{
	PGRAPHLIST CreateList = NULL;	
	CKnifeData  Dllknife;
	//�Ӵ�ĥ
	double l_iTempSize = g_pDoc->m_stcGlassGrind.PerfectSize * g_pDoc->m_stcGlassGrind.PerfectCount+
		g_pDoc->m_stcGlassGrind.PolishCount*g_pDoc->m_stcGlassGrind.PolishSize+
		g_pDoc->m_stcGlassGrind.KnifeRadius/2;
	//��ĥ���ٵ�
	int l_iNumOfKnife = g_pDoc->m_stcGlassGrind.PerfectCount+g_pDoc->m_stcGlassGrind.PolishCount+g_pDoc->m_stcGlassGrind.RouphCount;
	for(int k = g_pDoc->m_stcGlassGrind.RouphCount-1;k>=0;k--)
	{
		Dllknife.Knife_SetGraphData((long)m_pHeadList,g_pDoc->m_stcGlassGrind.RouphSize*k+l_iTempSize); 
		CreateList = (PGRAPHLIST)Dllknife.Knife_GetGraphFinishData();
		if(l_iNumOfKnife == g_pDoc->m_stcGlassGrind.RouphCount-k)//��������һ���Ļ�����Ҫ�����һ����Ϊԭͼ��ɫ
			GraphListTurnCreateStc(CreateList,POS,0);
		else
			GraphListTurnCreateStc(CreateList,POS,1);
	}
	//��ϸĥ
	l_iTempSize = g_pDoc->m_stcGlassGrind.PolishCount*g_pDoc->m_stcGlassGrind.PolishSize+g_pDoc->m_stcGlassGrind.KnifeRadius/2;
	for(int j = g_pDoc->m_stcGlassGrind.PerfectCount-1;j>=0;j--)
	{
		Dllknife.Knife_SetGraphData((long)m_pHeadList,g_pDoc->m_stcGlassGrind.PerfectSize*j+l_iTempSize); 
		CreateList = (PGRAPHLIST)Dllknife.Knife_GetGraphFinishData();
		//��������һ���Ļ�����Ҫ�����һ����Ϊԭͼ��ɫ
		if(l_iNumOfKnife == g_pDoc->m_stcGlassGrind.PerfectCount-j+g_pDoc->m_stcGlassGrind.RouphCount)
			GraphListTurnCreateStc(CreateList,POS,0);
		else
			GraphListTurnCreateStc(CreateList,POS,2);
	}
	//���׹�
	l_iTempSize = g_pDoc->m_stcGlassGrind.KnifeRadius/2;
	for(int i = g_pDoc->m_stcGlassGrind.PolishCount-1;i>=0;i--)
	{
		Dllknife.Knife_SetGraphData((long)m_pHeadList,g_pDoc->m_stcGlassGrind.PolishSize*i+l_iTempSize); 
		CreateList = (PGRAPHLIST)Dllknife.Knife_GetGraphFinishData();
		//��������һ���Ļ�����Ҫ�����һ����Ϊԭͼ��ɫ
		if(l_iNumOfKnife == g_pDoc->m_stcGlassGrind.PolishCount-i+g_pDoc->m_stcGlassGrind.RouphCount+g_pDoc->m_stcGlassGrind.PerfectCount)
			GraphListTurnCreateStc(CreateList,POS,0);
		else
			GraphListTurnCreateStc(CreateList,POS,3);
	}
}

/*
 *	�������������
 */
void TransToGraphList::AddUndoList(LONG CreateBase,POSITION pos)
{
	LONG pCreateBaseSpare = (LONG)g_pDoc->m_CCreateBaseList.GetAt(pos);
	if (m_pUndoList->pGraphPosSpare == NULL)
	{
		m_pCurGraphPos = m_pUndoList->pGraphPosSpare = m_UndoRedo.NewGraphPos();
		m_pCurGraphPos->pCreateBasepos = CreateBase;
		m_pCurGraphPos->pCreateBaseposSpare1 = pCreateBaseSpare;

	}
	else
	{
		m_pGraphPos = m_UndoRedo.NewGraphPos();						
		m_pGraphPos->pCreateBasepos = CreateBase;
		m_pGraphPos->pCreateBaseposSpare1 = pCreateBaseSpare;
		m_pCurGraphPos->pNext = m_pGraphPos;
		m_pGraphPos->pPrev = m_pCurGraphPos;
		m_pCurGraphPos = m_pGraphPos;
	}	
}

/*
 *	��M���뼯�������ַ���
 */
CString TransToGraphList::AnalysisAryM(CArray<class CString,class CString &>& aryM)
{
	int l_iSizeAryM = aryM.GetSize();
	CString l_strMCode = "";
	for(int i=0;i<l_iSizeAryM;i++)
	{
		if(i != 0)
			l_strMCode = l_strMCode + "\r\n";
		l_strMCode = l_strMCode + aryM.GetAt(i);
	}
	return l_strMCode;
}





