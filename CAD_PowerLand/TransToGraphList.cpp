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
	//清空m_pHeadList
	ClearGraphList();
}

/*
 *	磨玻璃输出的函数
 */
PGRAPHLIST TransToGraphList::OutGraphList(PUNDOLIST list)
{
	m_pUndoList = list;
	//1.清空m_pHeadList
	ClearGraphList();
	//2.转换数据
	Transfer();
	return m_pHeadList;
}

/*
 *	磨边输出的函数
 */
void TransToGraphList::OutMillSide()
{
	CUndoRedo l_UndoRedo;
	m_pUndoList = l_UndoRedo.NewUndoList();
	m_pUndoList->OperatieWhat = ID_GRIND_GLASS;
	//1.清空m_pHeadList
	ClearGraphList();
	//变刀补转换
	TransMillside();
	//删掉原图
	DeleteOriginal(m_pUndoList);
	//如果没有选中图形，把申请 的释放
	if (m_pUndoList->pGraphPos->pCreateBasepos == NULL)
	{
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n没有需要刀补的图形!");
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
			RemindContent("\r\n刀补完成");
		else
			RemindContent("\r\nKnife repairing completion");		
	}
}

/*
 *	删除选中的图形 
 */
void TransToGraphList::DeleteOriginal(PUNDOLIST pUndoList)
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
		g_pDoc->m_CCreateBaselayer1.AddTail(pSelEntity);
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
void TransToGraphList::ClearGraphList()
{
	PGRAPHLIST pGraphListTemp = m_pHeadList;
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

			delete pGraphDel;
			pGraphDel = NULL;
		}
		
		pGraphListTemp = pGraphListTemp->Next;
		delete pGraphListDel;
		pGraphListDel = NULL;
	}
	m_pHeadList = NULL;
}

//添加GraphList
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

//添加Action
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

//添加CADData
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

//添加Graph
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

//添加ArcInfo
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

//转换图形信息到m_pHeadList中
void TransToGraphList::Transfer()
{	
	POSITION l_POSList = NULL;
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	for( int i = 0 ; i < l_iSize; i++ )
	{
		CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CollectOfPicked[i];
		//根据类型选择画什么样的G代码
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransLine(line);	//直线
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();	
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransPolyline(Polyline);	//多线段
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransRectangle(Rectangle);//矩形
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{
			CCreateRound *Round = (CCreateRound*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransRound(Round);//圆
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransArc(Arc);	//圆弧
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{
			CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransEllipse(Ellipse);//椭圆
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyle((long)m_pHeadList);
			AddKnifeLong(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
	}	
}

/*
 *	转换成铣刀后的图形
 */
void TransToGraphList::TransMillside()
{
	POSITION l_POSList = NULL;
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	for( int i = 0 ; i < l_iSize; i++ )
	{
		CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CollectOfPicked[i];
		//根据类型选择画什么样的G代码
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransLine(line);	//直线
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();	
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
			||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransPolyline(Polyline);	//多线段
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransRectangle(Rectangle);//矩形
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{
			CCreateRound *Round = (CCreateRound*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransRound(Round);//圆
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransArc(Arc);	//圆弧
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{
			CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;
			l_POSList = g_pDoc->m_CCreateBaseList.Find(l_pCreateBase);			
			TransEllipse(Ellipse);//椭圆
			//转换尾部
			TransTail();
			//加刀补类型
			AddKnifeStyleForMillSide((long)m_pHeadList);
			AddKnifeLongForMillSide(l_POSList);
			//清空m_pHeadList
			m_pCurrList = NULL;
			ClearGraphList();
		}
	}	
}

/*
 *	转换尾部
 */
BOOL TransToGraphList::TransTail()
{
	//添加回到（0,0）的G00
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
		//添加直线信息
		m_pCurrGraph->chLine = 0;
		m_pCurrGraph->pPosBegin = m_pCurrData->pPrev;
		m_pCurrGraph->pPosEnd = m_pCurrData;
	}*/


	return TRUE;
}

/*
 *	转换直线
 */
BOOL TransToGraphList::TransLine(CCreateLine* pLine)
{
	double x1,y1;	//起点的坐标
	CString l_strSBM_What;	//M代码

	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(pLine->m_strSBM);	
	
	//起点坐标
	x1 = pLine->m_Begin.x;
	y1 = pLine->m_Begin.y;

	//				如果上一点和这一点不相等							颜色不一样   就要加一个G00
	if((fabs(m_dCurX-x1)>DOUBLE_EQUAL&&fabs(m_dCurY-y1)>DOUBLE_EQUAL)||(m_iColorLast != pLine->m_ColorOfLine))
	{
		DOUBLEPOINT ptEnd;
		ptEnd.x = x1;
		ptEnd.y = y1;
		AddG00(ptEnd, l_strSBM_What);

		AddGraphList();
	}
	

	//终点的坐标
	DOUBLEPOINT ptEnd;
	ptEnd.x = pLine->m_End.x;
	ptEnd.y = pLine->m_End.y;
	AddG01(ptEnd);
	m_dCurX = ptEnd.x;
	m_dCurY = ptEnd.y;
	
	//把对应颜色转成字符串M，当成最后的
	m_strCBM_What = AnalysisAryM(pLine->m_strCBM);	
	//把最后的颜色保存起来
	m_iColorLast = pLine->m_ColorOfLine;
	return TRUE;
}

/*
 *	转换整圆
 */
BOOL TransToGraphList::TransRound(CCreateRound* pRound)
{
	double x1,y1;	//起终点的坐标
	CString l_strSBM_What;	//M代码
	//起点的坐标
	x1 = pRound->m_EndPos.x;
	y1 = pRound->m_EndPos.y;
	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(pRound->m_strSBM);	
	
	DOUBLEPOINT ptEnd;
	//				如果上一点和这一点不相等							颜色不一样   就要加一个G00
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
	//把对应颜色转成字符串M
	m_strCBM_What = AnalysisAryM(pRound->m_strCBM);	
	return TRUE;
}

/*
 *	转换多段线
 */
BOOL TransToGraphList::TransPolyline(CCreatePolyline* Polyline)
{
	//俩个点和圆心
	Position pos,posNext,l_posCenter;	
	//起终点的坐标,凸度
	double x1,y1,x2,y2,l_dTuDu;	
	CString l_content,l_strSBM_What;	//写入文件字符串的内容

	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(Polyline->m_strSBM);	
	//点集的多少
	int l_iSizeOfPoint = Polyline->m_point.GetSize();
	//生成代码
	for(int i=0;i<l_iSizeOfPoint-1;i++)
	{
		pos = Polyline->m_point.GetAt(i);
		posNext = Polyline->m_point.GetAt(i+1);
		l_dTuDu = Polyline->m_dTuDu.GetAt(i);
		//变成数据
		x1 = pos.x;
		y1 = pos.y;
		x2 = posNext.x;
		y2 = posNext.y;
		////多段线的第一点是否与上一点相同,颜色是否相同 
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
		if(l_dTuDu == 0.0)//	直线
			AddG01(ptEnd);
		else//圆弧
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
	//把对应颜色转成字符串M
	m_strCBM_What = AnalysisAryM(Polyline->m_strCBM);	
	//把最后的颜色保存起来
	m_iColorLast = Polyline->m_ColorOfLine;
	return TRUE;
}

/*
 *	转换椭圆
 */
BOOL TransToGraphList::TransEllipse(CCreateEllipse* Ellipse)
{
	Position pos;	//起点
	double x1,y1;	//起终点的坐标
	CString l_content,l_strSBM_What;	//写入文件字符串的内容
	
	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(Ellipse->m_strSBM);	
	//生成代码
	for(int i=0  ;   i < Ellipse->m_point.GetSize()  ;i++)
	{
		pos = Ellipse->m_point.GetAt(i);
		x1 = pos.x;
		y1 = pos.y;		

		DOUBLEPOINT ptEnd;
		ptEnd.x = x1;
		ptEnd.y = y1;

		if (i!=0)//零时为第一点
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
	//把对应颜色转成字符串M
	m_strCBM_What = AnalysisAryM(Ellipse->m_strCBM);	
	return TRUE;
}

/*
 *	转换圆弧
 */
BOOL TransToGraphList::TransArc(CCreateRoundArc* pArc)
{	
	double x1,y1;	//起终点的坐标
	CString l_strSBM_What;	//写入文件字符串的内容
	//起点的坐标
	x1 = pArc->m_Begin.x;
	y1 = pArc->m_Begin.y;
	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(pArc->m_strSBM);	
	//				如果上一点和这一点不相等							颜色不一样   就要加一个G00
	if((fabs(m_dCurX-x1)>DOUBLE_EQUAL&&fabs(m_dCurY-y1)>DOUBLE_EQUAL)||(m_iColorLast != pArc->m_ColorOfLine))
	{
		DOUBLEPOINT ptEnd;
		ptEnd.x = pArc->m_Begin.x;
		ptEnd.y = pArc->m_Begin.y;
		AddG00(ptEnd, l_strSBM_What);
		AddGraphList();
	}	
	
	//添加圆弧信息
	AddG0X(pArc);

	//把对应颜色转成字符串M
	m_strCBM_What = AnalysisAryM(pArc->m_strCBM);	
	//把最后的颜色保存起来
	m_iColorLast = pArc->m_ColorOfLine;
	return TRUE;
}

/*
 *	转换矩形
 */
BOOL TransToGraphList::TransRectangle(CCreateRectangle* pRectangle)
{
	//俩个点和圆心
	Position pos,posNext,l_posCenter;	
	//起终点的坐标,凸度
	double x1,y1,x2,y2,l_dTuDu;	
	CString l_content,l_strSBM_What;	//写入文件字符串的内容
	
	//把对应颜色转成字符串M
	l_strSBM_What = AnalysisAryM(pRectangle->m_strSBM);	
	//点集的多少
	int l_iSizeOfPoint = pRectangle->m_point.GetSize();
	//生成代码
	for(int i=0;i<l_iSizeOfPoint-1;i++)
	{
		pos = pRectangle->m_point.GetAt(i);
		posNext = pRectangle->m_point.GetAt(i+1);
		l_dTuDu = pRectangle->m_dTuDu.GetAt(i);
		//变成数据
		x1 = pos.x;
		y1 = pos.y;
		x2 = posNext.x;
		y2 = posNext.y;
		////多段线的第一点是否与上一点相同,颜色是否相同 
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
		if(l_dTuDu == 0.0)//	直线
			AddG01(ptEnd);
		else//圆弧
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
	//把对应颜色转成字符串M
	m_strCBM_What = AnalysisAryM(pRectangle->m_strCBM);	
	//把最后的颜色保存起来
	m_iColorLast = pRectangle->m_ColorOfLine;
	return TRUE;
}

/*
 *	根据颜色转成字符抬刀M多少的函数
 */
CString TransToGraphList::CintToCBMstr(int RtnColor)
{
	CString l_strM;		//根据颜色转成字符
	//字符由外部对应设定的
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
 *	根据颜色转成字符下刀M多少的函数
 */
CString TransToGraphList::CintToSBMstr(int RtnColor)
{
	CString l_strM;		//根据颜色转成字符
	//字符由外部对应设定的
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

//获取M代码的值
int TransToGraphList::MCodeValue(CString str)
{
	str.TrimLeft(_T(" M"));
	str.TrimRight(_T(" "));
	return atoi(str);
}

//功能：添加G00
//参数：DOUBLEPOINT ptEnd：G00的末点（起点存在m_dCurX,m_dCurY）
//		CString l_strSBM_What：G00之后的M代码
BOOL TransToGraphList::AddG00(DOUBLEPOINT ptEnd, CString l_strSBM_What)
{
	//添加MCode
	if (m_strCBM_What != "")
	{
		AddGraphList();
		AddAction();
		m_pCurrList->pAction->nType = 1;
		m_pCurrList->pAction->nCode = MCodeValue(m_strCBM_What);
	}
	AddGraphList();
	//添加G00
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
		//添加直线信息
		m_pCurrGraph->chLine = 0;
		m_pCurrGraph->pPosBegin = m_pCurrData->pPrev;
		m_pCurrGraph->pPosEnd = m_pCurrData;
	}
	//添加MCode
	if (l_strSBM_What != "")
	{
		AddGraphList();
		AddAction();
		m_pCurrList->pAction->nType = 1;
		m_pCurrList->pAction->nCode = MCodeValue(l_strSBM_What);
	}
	
	return TRUE;
}

//功能：添加G01
//参数：DOUBLEPOINT ptEnd：G01的末点（起点存在m_dCurX,m_dCurY）
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
		//添加直线信息
		m_pCurrGraph->chLine = 1;
		m_pCurrGraph->pPosBegin = m_pCurrData->pPrev;
		m_pCurrGraph->pPosEnd = m_pCurrData;
	}
	return TRUE;
}

//功能：添加G02/03
//参数：CCreateRoundArc *pArc：圆弧信息
BOOL TransToGraphList::AddG0X(CCreateRoundArc *pArc)
{
	DOUBLEPOINT ptBeg, ptMid, ptEnd;
	ptBeg.x = pArc->m_Begin.x;
	ptBeg.y = pArc->m_Begin.y;

	if (m_pCurrList->pGraph != NULL)	//有图形与当前圆弧相接，此时圆弧起点就是上一个图形末点
	{
		AddGraph();
		m_pCurrGraph->pPosBegin = m_pCurrGraph->Prev->pPosEnd;
	}
	else								//没有图形与当前圆弧相接，则向链表中添加圆弧起点
	{
		AddGraph();
		m_ptSet.x = ptBeg.x;
		m_ptSet.y = ptBeg.y;
		AddCADData();
		m_pCurrList->pGraph->pPosBegin =  m_pCurrData;
	}
	
	//添加圆弧中点
	ptMid.x = ptMid.y = 0.0;
	m_ptSet.x = m_ptSet.y = 0.0;
	AddCADData();
		
	//添加圆弧终点
	ptEnd.x = pArc->m_End.x;
	ptEnd.y = pArc->m_End.y;
	m_dCurX = m_ptSet.x = pArc->m_End.x;
	m_dCurY = m_ptSet.y = pArc->m_End.y;
	AddCADData();

	//设置圆弧起终点
	m_pCurrGraph->pPosMid = m_pCurrData->pPrev;
	m_pCurrGraph->pPosEnd = m_pCurrData;
	
	if (pArc->m_iDirect == AD_CLOCKWISE)//如果是顺时针
		m_pCurrGraph->chLine = 2;
	else//否则逆时针
		m_pCurrGraph->chLine = 3;
	
	DOUBLEPOINT ptCenter, ptIJK;
	//圆心
	ptCenter.x = pArc->m_Center.x;
	ptCenter.y = pArc->m_Center.y;
	ptCenter.z = 0;
	//圆心相对起点的增量
	ptIJK.x = pArc->m_Center.x - pArc->m_Begin.x;
	ptIJK.y = pArc->m_Center.y - pArc->m_Begin.y;
	ptIJK.z = 0;

	double dBeginPointAngle, dEndPointAngle;
	//起始角度
	dBeginPointAngle = PointToPointAngle(ptCenter, ptBeg);
	//终止角度
	dEndPointAngle = PointToPointAngle(ptCenter, ptEnd);
	
	//添加圆弧信息
	AddArcInfo();
	m_pCurrGraph->pArcInfo->PosCenter = ptCenter;
	m_pCurrGraph->pArcInfo->PosIJK = ptIJK;
	m_pCurrGraph->pArcInfo->dBeginAngle = dBeginPointAngle;
	m_pCurrGraph->pArcInfo->dEndAngle = dEndPointAngle;
	m_pCurrGraph->pArcInfo->dAngleSweep = CalculateTraverseAngle(m_pCurrGraph->chLine, dBeginPointAngle, dEndPointAngle);

	return TRUE;
}

//功能：添加G02/03
//参数：CCreateRound *pRound：整圆信息
//		DOUBLEPOINT ptBegAndEnd：整圆的起点和终点
//		pRound->m_EndPos点为用户点击的点，而整圆的起终点为圆心x负半轴与圆的交点
BOOL TransToGraphList::AddG0X(CCreateRound *pRound, DOUBLEPOINT ptBegAndEnd)
{	
	DOUBLEPOINT ptBeg, ptMid, ptEnd;
	ptBeg.x = ptBegAndEnd.x;
	ptBeg.y = ptBegAndEnd.y;
	
	if (m_pCurrList->pGraph != NULL)	//有图形与当前圆弧相接，此时圆弧起点就是上一个图形末点
	{
		AddGraph();
		m_pCurrGraph->pPosBegin = m_pCurrGraph->Prev->pPosEnd;
	}
	else								//没有图形与当前圆弧相接，则向链表中添加圆弧起点
	{
		AddGraph();
		m_ptSet.x = ptBeg.x;
		m_ptSet.y = ptBeg.y;
		AddCADData();
		m_pCurrList->pGraph->pPosBegin =  m_pCurrData;
	}
	
	//添加中点
	ptMid.x = ptMid.y = 0.0;
	m_ptSet.x = m_ptSet.y = 0.0;
	AddCADData();

	//添加终点
	ptEnd.x = ptBegAndEnd.x;
	ptEnd.y = ptBegAndEnd.y;
	m_dCurX = m_ptSet.x = ptBegAndEnd.x;
	m_dCurY = m_ptSet.y = ptBegAndEnd.y;
	AddCADData();
	
	//设置整圆起终点
	m_pCurrGraph->pPosMid = m_pCurrData->pPrev;
	m_pCurrGraph->pPosEnd = m_pCurrData;
	
	if (pRound->m_iDirect == AD_CLOCKWISE)//如果是顺时针
		m_pCurrGraph->chLine = 2;
	else//否则逆时针
		m_pCurrGraph->chLine = 3;
	
	DOUBLEPOINT ptCenter, ptIJK;
	//圆心
	ptCenter.x = pRound->m_Center.x;
	ptCenter.y = pRound->m_Center.y;
	ptCenter.z = 0.0;
	//圆心相对起点的增量
	ptIJK.x = pRound->m_Center.x - ptBegAndEnd.x;
	ptIJK.y = pRound->m_Center.y - ptBegAndEnd.y;
	ptIJK.z = 0.0;
	
	double dBeginPointAngle, dEndPointAngle;
	//起始角度
	dBeginPointAngle = PointToPointAngle(ptCenter, ptBeg);
	//终止角度
	dEndPointAngle = PointToPointAngle(ptCenter, ptEnd);
	
	//添加圆弧信息
	AddArcInfo();
	m_pCurrGraph->pArcInfo->PosCenter = ptCenter;
	m_pCurrGraph->pArcInfo->PosIJK = ptIJK;
	m_pCurrGraph->pArcInfo->dBeginAngle = dBeginPointAngle;
	m_pCurrGraph->pArcInfo->dEndAngle = dEndPointAngle;
	m_pCurrGraph->pArcInfo->dAngleSweep = CalculateTraverseAngle(m_pCurrGraph->chLine, dBeginPointAngle, dEndPointAngle);
	
	return TRUE;
}

//计算直线角度
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
		
		if (angle < 0)		angle += 360;      //对于负数结果加360，可以得到0-360内的结果
		else if (angle > 360)      angle -= 360;	  //对大于360的结果减360，可以得到0-360内的结果
	}
	
	return angle;
}

//计算扫过的角度
double TransToGraphList::CalculateTraverseAngle(char chDirection, double dBeginAngle, double dEndAngle)
{
	double fTraverseAngle;
	
	fTraverseAngle = dEndAngle - dBeginAngle;   
	if (chDirection == 3)/* 逆时针 */
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
			//在已经转换为极坐标系的图形链表中寻找距离ptStart最近的点
			//对pAction无需修改
		}
		else if ( pTempGraphList->pGraph != NULL )
		{
			if ( pTempGraphList->pGraph->chLine == 0  )		//空行直线
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
 *	GRAPHLIST转换Createbase
 */
void TransToGraphList::ListToCreateForMillSide(PGRAPHLIST pHeadList,POSITION POS)
{
	PGRAPHLIST pGraphListTemp = pHeadList;
	CArray<Position,Position&> l_aryPoint;//点集
	CArray<double,double&> l_aryTuDu;//存放凸度的集合
	Position l_posBegin,l_posEnd,l_posCenter,l_posTemp;//临时用的三个点
	double l_dTuDu;//临时用的凸度
	COLORREF l_RGBGraphColor;	//线的颜色
	int l_iMcode,l_iKnifeStyle;
	//判断左右刀补，给每个图形用
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iKnifeStyle = 41;
	else
		l_iKnifeStyle = 42;

	while(pGraphListTemp != NULL)
	{
		if (pGraphListTemp->pAction != NULL) //如果有M代码
		{
			if (m_bFlagAddList == FALSE)//G00下面的M代码
				m_iColorLast = pGraphListTemp->pAction->nCode;
			else//切完图后的M代码
			{
				//求生成的刀路的颜色
				l_iMcode = pGraphListTemp->pAction->nCode;				
				l_RGBGraphColor  = RtnColorOfKnife(0,l_iMcode);
				//然后点集生成我的结构
				l_dTuDu = 0.0;
				l_aryPoint.Add(l_posEnd);
				l_aryTuDu.Add(l_dTuDu);
				if(l_aryPoint.GetSize() == 2&&l_aryTuDu.GetAt(0) != 0.0)//说明只有一个圆弧
				{
					//凸度
					l_dTuDu = l_aryTuDu.GetAt(0);
					l_posBegin = l_aryPoint.GetAt(0);
					l_posEnd = l_aryPoint.GetAt(1);
					g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTuDu,l_posCenter);
					//新圆弧
					CCreateRoundArc *l_pTempArc = new CCreateRoundArc(l_posBegin,l_posCenter,l_posEnd,RtnArcDir(l_dTuDu));
					l_pTempArc->m_iKnifeStyle = l_iKnifeStyle;	
					//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
					m_CollectOfG00.Add(l_pTempArc);
					//向撤消链表里添加 
					g_pDoc->m_CCreateBaseList.InsertBefore(POS,l_pTempArc);//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
					AddUndoList((LONG)l_pTempArc,POS);
				}
				else //说明是多段线
				{
					if(l_aryPoint.GetSize() > 1)
					{
						CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
						pPolyline->m_iKnifeStyle = l_iKnifeStyle;
						//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
						m_CollectOfG00.Add(pPolyline);
						//向撤消链表里添加 
						g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
						AddUndoList((LONG)pPolyline,POS);
					}
				}
				//把一对M 之间的图元负上颜色并加到链表里
				for( int i = 0 ; i < m_CollectOfG00.GetSize() ; i++ )
				{
					CCreateBase*	pSelEntity = (CCreateBase*) m_CollectOfG00[i] ;
					pSelEntity->m_ColorOfLine = l_RGBGraphColor;
				}
				m_CollectOfG00.RemoveAll();
				l_aryPoint.RemoveAll();//并且删除点
				l_aryTuDu.RemoveAll();	
			}
		}
		//如果不是M代码，是G代码
		else if(pGraphListTemp->pGraph != NULL)	
		{
			m_bFlagAddList = TRUE;
			if (pGraphListTemp->pGraph->chLine == 0) //如果是G00
			{
				PosToPOS(l_posEnd,pGraphListTemp->pGraph->pPosEnd->PointPara);
				m_bFlagAddList = FALSE;
			}
			else//否则是G01或G02或G03,详细可看COpenGCode那个类的结构
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
						if(PosEqual(l_posEnd,l_posTemp))//如果是圆的话
						{
							if(l_aryPoint.GetSize()>1)//如果没有多段线，就不向链表里加
							{
								l_aryPoint.Add(l_posEnd);
								l_aryTuDu.Add(l_dTuDu);
								CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
								pPolyline->m_iKnifeStyle = l_iKnifeStyle;
								//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
								m_CollectOfG00.Add(pPolyline);
								g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
								AddUndoList((LONG)pPolyline,POS);
							}
							l_aryPoint.RemoveAll();
							l_aryTuDu.RemoveAll();	
							CCreateRound *pRound  = new CCreateRound(l_posCenter,l_posBegin,AD_CLOCKWISE);
							pRound->m_iKnifeStyle = l_iKnifeStyle;
							//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
							m_CollectOfG00.Add(pRound);
							//向撤消链表里添加 
							g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound);
							AddUndoList((LONG)pRound,POS);
						}
						else//否则是圆弧
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
						if(PosEqual(l_posEnd,l_posTemp))//如果是圆的话
						{
							if(l_aryPoint.GetSize()>1)//如果没有多段线，就不向链表里加
							{
								l_aryPoint.Add(l_posEnd);
								l_aryTuDu.Add(l_dTuDu);
								CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
								pPolyline->m_iKnifeStyle = l_iKnifeStyle;
								//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
								m_CollectOfG00.Add(pPolyline);
								//向撤消链表里添加 
								g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
								AddUndoList((LONG)pPolyline,POS);
							}
							l_aryPoint.RemoveAll();
							l_aryTuDu.RemoveAll();
							CCreateRound *pRound  = new CCreateRound(l_posCenter,l_posBegin,AD_COUNTERCLOCKWISE);
							pRound->m_iKnifeStyle = l_iKnifeStyle;
							//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
							m_CollectOfG00.Add(pRound);
							//向撤消链表里添加 
							g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound);
							AddUndoList((LONG)pRound,POS);
						}
						else//否则是圆弧
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
 *	把DATASTRUCT的结构转成CreateBase结构
 */
BOOL TransToGraphList::GraphListTurnCreateStc(PGRAPHLIST pHeadList,POSITION POS,int StyleKnife)
{
	PGRAPHLIST pGraphListTemp = pHeadList;
	CArray<Position,Position&> l_aryPoint;//点集
	CArray<double,double&> l_aryTuDu;//存放凸度的集合
	Position l_posBegin,l_posEnd,l_posCenter,l_posTemp;//临时用的三个点
	double l_dTuDu;//临时用的凸度
	COLORREF l_RGBGraphColor;	//线的颜色
	int l_iMcode,l_iKnifeStyle,l_iIsBeginGraph = 2;
	//判断左右刀补，给每个图形用
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iKnifeStyle = 41;
	else
		l_iKnifeStyle = 42;
	//判断是不是原始的图形
	if(StyleKnife == 0)
		l_iIsBeginGraph = 1;
	if(StyleKnife == 1)
		l_iIsBeginGraph = 0;

	while(pGraphListTemp != NULL)
	{
		if (pGraphListTemp->pAction != NULL) //如果有M代码
		{
			if (m_bFlagAddList == FALSE)//G00下面的M代码
				m_iColorLast = pGraphListTemp->pAction->nCode;
			else//切完图后的M代码
			{
				//求生成的刀路的颜色
				l_iMcode = pGraphListTemp->pAction->nCode;				
				l_RGBGraphColor  = RtnColorOfKnife(StyleKnife,l_iMcode);
				//然后点集生成我的结构
				l_dTuDu = 0.0;
				l_aryPoint.Add(l_posEnd);
				l_aryTuDu.Add(l_dTuDu);
				if(l_aryPoint.GetSize() == 2&&l_aryTuDu.GetAt(0) != 0.0)//说明只有一个圆弧
				{
					//凸度
					l_dTuDu = l_aryTuDu.GetAt(0);
					l_posBegin = l_aryPoint.GetAt(0);
					l_posEnd = l_aryPoint.GetAt(1);
					g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTuDu,l_posCenter);
					//新圆弧
					CCreateRoundArc *l_pTempArc = new CCreateRoundArc(l_posBegin,l_posCenter,l_posEnd,RtnArcDir(l_dTuDu));
					l_pTempArc->m_iKnifeStyle = l_iKnifeStyle;	
					l_pTempArc->m_iIsBeginGraph = l_iIsBeginGraph;
					//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
					m_CollectOfG00.Add(l_pTempArc);
					//向撤消链表里添加 
					g_pDoc->m_CCreateBaseList.InsertBefore(POS,l_pTempArc);//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
					AddUndoList((LONG)l_pTempArc,POS);
				}
				else //说明是多段线
				{
					if(l_aryPoint.GetSize() > 1)
					{
						CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
						pPolyline->m_iKnifeStyle = l_iKnifeStyle;
						pPolyline->m_iIsBeginGraph = l_iIsBeginGraph;
						//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
						m_CollectOfG00.Add(pPolyline);
						//向撤消链表里添加 
						g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
						AddUndoList((LONG)pPolyline,POS);
					}
				}
				//把一对M 之间的图元负上颜色并加到链表里
				for( int i = 0 ; i < m_CollectOfG00.GetSize() ; i++ )
				{
					CCreateBase*	pSelEntity = (CCreateBase*) m_CollectOfG00[i] ;
					pSelEntity->m_ColorOfLine = l_RGBGraphColor;
				}
				m_CollectOfG00.RemoveAll();
				l_aryPoint.RemoveAll();//并且删除点
				l_aryTuDu.RemoveAll();	
			}
		}
		//如果不是M代码，是G代码
		else if(pGraphListTemp->pGraph != NULL)	
		{
			m_bFlagAddList = TRUE;
			if (pGraphListTemp->pGraph->chLine == 0) //如果是G00
			{
				PosToPOS(l_posEnd,pGraphListTemp->pGraph->pPosEnd->PointPara);
				m_bFlagAddList = FALSE;
			}
			else//否则是G01或G02或G03,详细可看COpenGCode那个类的结构
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
						if(PosEqual(l_posEnd,l_posTemp))//如果是圆的话
						{
							if(l_aryPoint.GetSize()>1)//如果没有多段线，就不向链表里加
							{
								l_aryPoint.Add(l_posEnd);
								l_aryTuDu.Add(l_dTuDu);
								CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
								pPolyline->m_iKnifeStyle = l_iKnifeStyle;
								pPolyline->m_iIsBeginGraph = l_iIsBeginGraph;
								//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
								m_CollectOfG00.Add(pPolyline);
								g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
								AddUndoList((LONG)pPolyline,POS);
							}
							l_aryPoint.RemoveAll();
							l_aryTuDu.RemoveAll();	
							CCreateRound *pRound  = new CCreateRound(l_posCenter,l_posBegin,AD_CLOCKWISE);
							pRound->m_iKnifeStyle = l_iKnifeStyle;
							pRound->m_iIsBeginGraph = l_iIsBeginGraph;
							//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
							m_CollectOfG00.Add(pRound);
							//向撤消链表里添加 
							g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound);
							AddUndoList((LONG)pRound,POS);
						}
						else//否则是圆弧
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
						if(PosEqual(l_posEnd,l_posTemp))//如果是圆的话
						{
							if(l_aryPoint.GetSize()>1)//如果没有多段线，就不向链表里加
							{
								l_aryPoint.Add(l_posEnd);
								l_aryTuDu.Add(l_dTuDu);
								CCreatePolyline *pPolyline  = new CCreatePolyline(l_aryPoint,l_aryTuDu);
								pPolyline->m_iKnifeStyle = l_iKnifeStyle;
								pPolyline->m_iIsBeginGraph = l_iIsBeginGraph;
								//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
								m_CollectOfG00.Add(pPolyline);
								//向撤消链表里添加 
								g_pDoc->m_CCreateBaseList.InsertBefore(POS,pPolyline);
								AddUndoList((LONG)pPolyline,POS);
							}
							l_aryPoint.RemoveAll();
							l_aryTuDu.RemoveAll();
							CCreateRound *pRound  = new CCreateRound(l_posCenter,l_posBegin,AD_COUNTERCLOCKWISE);
							pRound->m_iKnifeStyle = l_iKnifeStyle;
							pRound->m_iIsBeginGraph = l_iIsBeginGraph;
							//把图元加进到中间的链表，等下一个M代码出现时要负上颜色
							m_CollectOfG00.Add(pRound);
							//向撤消链表里添加 
							g_pDoc->m_CCreateBaseList.InsertBefore(POS,pRound);
							AddUndoList((LONG)pRound,POS);
						}
						else//否则是圆弧
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
 *	返回粗，精，抛光的颜色
 */
COLORREF TransToGraphList::RtnColorOfKnife(int StyleKnife,int l_iMcode)
{
	COLORREF l_RGBGraphColor;	//线的颜色
	if(StyleKnife == 0)
	{
		if (m_iColorLast == MCodeValue(g_pDoc->m_stcColorToM.m_SBM1)&&l_iMcode == MCodeValue(g_pDoc->m_stcColorToM.m_CBM1))	
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color1;	//刀一色
		else if (m_iColorLast == MCodeValue(g_pDoc->m_stcColorToM.m_SBM2)&&l_iMcode == MCodeValue(g_pDoc->m_stcColorToM.m_CBM2))
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color2;	//刀二色
		else if (m_iColorLast == MCodeValue(g_pDoc->m_stcColorToM.m_SBM3)&&l_iMcode == MCodeValue(g_pDoc->m_stcColorToM.m_CBM3))
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color3;	//刀三色
		else if (m_iColorLast == MCodeValue(g_pDoc->m_stcColorToM.m_SBM4)&&l_iMcode == MCodeValue(g_pDoc->m_stcColorToM.m_CBM4))
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color4;	//刀四色
		else if (m_iColorLast == MCodeValue(g_pDoc->m_stcColorToM.m_SBM5)&&l_iMcode == MCodeValue(g_pDoc->m_stcColorToM.m_CBM5))
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color5;	//刀五色
		else//不在设定的M代码对应的颜色里就用刀五色画吧！！！^*^
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color5;
	}
	else
	{
		if(StyleKnife == 1)
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color1;	//刀一色
		if(StyleKnife == 2)
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color2;	//刀二色
		if(StyleKnife == 3)
			l_RGBGraphColor = g_pDoc->m_stcColorToM.m_color3;	//刀三色
		}	 
	return l_RGBGraphColor;
}

/*
 *	加刀补类型 
 */
PGRAPHLIST TransToGraphList::AddKnifeStyleForMillSide(long lData)
{
	int l_iDaoBu = 0;	//刀补类型 41左刀补 和 42右刀补
	if(g_pDoc->m_stcClothCut.m_bMillSideLorR == 1)
		l_iDaoBu = 41;
	else
		l_iDaoBu = 42;
	//加刀补
	PGRAPHLIST  RtnKnife = (PGRAPHLIST)lData;
	while(RtnKnife != NULL)
	{
		RtnKnife->chKerfComp = l_iDaoBu;	
		RtnKnife = RtnKnife->Next;
	}
	return RtnKnife;
}

/*
*	加刀补类型 
*/
PGRAPHLIST TransToGraphList::AddKnifeStyle(long lData)
{
	int l_iDaoBu = 0;	//刀补类型 41左刀补 和 42右刀补
	if(g_pDoc->m_stcGlassGrind.DaoBuStyle == 1)
		l_iDaoBu = 41;
	else
		l_iDaoBu = 42;
	//加刀补
	PGRAPHLIST  RtnKnife = (PGRAPHLIST)lData;
	while(RtnKnife != NULL)
	{
		RtnKnife->chKerfComp = l_iDaoBu;	
		RtnKnife = RtnKnife->Next;
	}
	return RtnKnife;
}

/*
 *	加刀补尺寸给铣边图形
 */
void TransToGraphList::AddKnifeLongForMillSide(POSITION POS)
{
	PGRAPHLIST CreateList = NULL;	
	CKnifeData  Dllknife;
	//加粗磨
	double l_iTempSize = g_pDoc->m_stcClothCut.m_dMillSideDis;
	Dllknife.Knife_SetGraphData((long)m_pHeadList,l_iTempSize); 
	CreateList = (PGRAPHLIST)Dllknife.Knife_GetGraphFinishData();
	ListToCreateForMillSide(CreateList,POS);
}


/*
 *	加刀补尺寸
 */
void TransToGraphList::AddKnifeLong(POSITION POS)
{
	PGRAPHLIST CreateList = NULL;	
	CKnifeData  Dllknife;
	//加粗磨
	double l_iTempSize = g_pDoc->m_stcGlassGrind.PerfectSize * g_pDoc->m_stcGlassGrind.PerfectCount+
		g_pDoc->m_stcGlassGrind.PolishCount*g_pDoc->m_stcGlassGrind.PolishSize+
		g_pDoc->m_stcGlassGrind.KnifeRadius/2;
	//共磨多少刀
	int l_iNumOfKnife = g_pDoc->m_stcGlassGrind.PerfectCount+g_pDoc->m_stcGlassGrind.PolishCount+g_pDoc->m_stcGlassGrind.RouphCount;
	for(int k = g_pDoc->m_stcGlassGrind.RouphCount-1;k>=0;k--)
	{
		Dllknife.Knife_SetGraphData((long)m_pHeadList,g_pDoc->m_stcGlassGrind.RouphSize*k+l_iTempSize); 
		CreateList = (PGRAPHLIST)Dllknife.Knife_GetGraphFinishData();
		if(l_iNumOfKnife == g_pDoc->m_stcGlassGrind.RouphCount-k)//如果是最后一刀的话，就要把最后一刀改为原图颜色
			GraphListTurnCreateStc(CreateList,POS,0);
		else
			GraphListTurnCreateStc(CreateList,POS,1);
	}
	//加细磨
	l_iTempSize = g_pDoc->m_stcGlassGrind.PolishCount*g_pDoc->m_stcGlassGrind.PolishSize+g_pDoc->m_stcGlassGrind.KnifeRadius/2;
	for(int j = g_pDoc->m_stcGlassGrind.PerfectCount-1;j>=0;j--)
	{
		Dllknife.Knife_SetGraphData((long)m_pHeadList,g_pDoc->m_stcGlassGrind.PerfectSize*j+l_iTempSize); 
		CreateList = (PGRAPHLIST)Dllknife.Knife_GetGraphFinishData();
		//如果是最后一刀的话，就要把最后一刀改为原图颜色
		if(l_iNumOfKnife == g_pDoc->m_stcGlassGrind.PerfectCount-j+g_pDoc->m_stcGlassGrind.RouphCount)
			GraphListTurnCreateStc(CreateList,POS,0);
		else
			GraphListTurnCreateStc(CreateList,POS,2);
	}
	//加抛光
	l_iTempSize = g_pDoc->m_stcGlassGrind.KnifeRadius/2;
	for(int i = g_pDoc->m_stcGlassGrind.PolishCount-1;i>=0;i--)
	{
		Dllknife.Knife_SetGraphData((long)m_pHeadList,g_pDoc->m_stcGlassGrind.PolishSize*i+l_iTempSize); 
		CreateList = (PGRAPHLIST)Dllknife.Knife_GetGraphFinishData();
		//如果是最后一刀的话，就要把最后一刀改为原图颜色
		if(l_iNumOfKnife == g_pDoc->m_stcGlassGrind.PolishCount-i+g_pDoc->m_stcGlassGrind.RouphCount+g_pDoc->m_stcGlassGrind.PerfectCount)
			GraphListTurnCreateStc(CreateList,POS,0);
		else
			GraphListTurnCreateStc(CreateList,POS,3);
	}
}

/*
 *	向撤消链表里添加
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
 *	把M代码集解析成字符串
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





