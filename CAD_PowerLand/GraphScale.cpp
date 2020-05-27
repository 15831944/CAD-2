// GraphScale.cpp: implementation of the CGraphScale class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "GraphScale.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphScale::CGraphScale()
{

}

CGraphScale::~CGraphScale()
{

}

short CGraphScale::ZoomInOut(double dValueX, double dValueY) 
{
	if (dValueX == 0.0&&dValueY == 0.0)
	{
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n参数为零时为不缩放");
		else
			RemindContent("\r\nParameter is zero not zoom");
		return FALSE;		
	}
	int l_iSize = g_pDoc->m_CollectOfPicked.GetSize();
	//向撤消链表里添加操作
	CUndoRedo l_UndoRedo;//撤消类的对象 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_SCALE_XY;
	
	pUndoList->pOperatieParam = l_UndoRedo.NewOperateParam();
	pUndoList->pOperatieParam->potBase.x = dValueX;
	pUndoList->pOperatieParam->potBase.y = dValueY;
	
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = headGraphPos;
	BOOL l_bIsScale = FALSE;//标志是否有缩放
	for( int i = 0 ; i < l_iSize ; i++ )
	{
		CCreateBase*	pSelEntity = (CCreateBase*) g_pDoc->m_CollectOfPicked[i];
		if(pSelEntity->GraphZoomXY(dValueX,dValueY))//如果缩放成功
			l_bIsScale = TRUE;
		if (CurGraphPos->pCreateBasepos == NULL)
			CurGraphPos->pCreateBasepos = (long)pSelEntity;
		else
		{
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (long)pSelEntity;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		} 
		
	}
	if (l_bIsScale == FALSE)//没有要绽放的图形
	{
		delete pUndoList->pGraphPos;
		pUndoList->pGraphPos = NULL;
		delete pUndoList->pOperatieParam;
		pUndoList->pOperatieParam = NULL;
		delete pUndoList;
		pUndoList = NULL;
		//中文还是英文
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n没有要缩放的图形");
		else
			RemindContent("\r\nNo graphics to zoom");
		return 0;
	}
	g_pDoc->m_Undo->AddUndoList(pUndoList);
	g_pDoc->RemoveAllCollected();
	g_pView->Invalidate();
	return 0;
}


//得到list的矩形区域
//list
BOX2D CGraphScale::GetListRect(PGRAPHLIST pList)
{
    BOX2D BoxRect;	
	BoxRect.min[0]=0.0;
	BoxRect.min[1]=0.0;
	BoxRect.max[0]=0.0;
	BoxRect.max[1]=0.0;
	for (PGRAPH pGraph=pList->pGraph; pGraph; pGraph = pGraph->Next)
	{		
		if (pList->pCadData == pGraph->pPosBegin)
		{
            BoxRect.min[0]=pGraph->pPosBegin->PointPara.x;
			BoxRect.min[1]=pGraph->pPosBegin->PointPara.y;
            BoxRect.max[0]=pGraph->pPosBegin->PointPara.x;
			BoxRect.max[1]=pGraph->pPosBegin->PointPara.y;
			GetPointBox(pGraph->pPosBegin->PointPara,BoxRect);		
		}
		GetPointBox(pGraph->pPosEnd->PointPara,BoxRect);		
		
		if(pGraph->chLine > 1)//圆弧需要过象限取值
		{
			PARCINFO pArc = pGraph->pArcInfo;
            DOUBLEPOINT point;
			point.z = 0;
			double dRadiu = hypot(pArc->PosIJK.x,pArc->PosIJK.y);
			if(pGraph->chLine == 2)	//如果是顺时针
			{
				if(pArc->dBeginAngle+pArc->dAngleSweep <= 0.0)								//如果经过零度
				{
					point.x = pArc->PosCenter.x + dRadiu;
					point.y = pArc->PosCenter.y;
					GetPointBox(point,BoxRect);	
				}
				if( (pArc->dBeginAngle > 90.0 && pArc->dBeginAngle+pArc->dAngleSweep < 90.0)
					|| pArc->dBeginAngle+pArc->dAngleSweep <= -270.0)	//如果经过90度
				{					
					point.x = pArc->PosCenter.x;
					point.y = pArc->PosCenter.y + dRadiu;
					GetPointBox(point,BoxRect);		
				}
				if( (pArc->dBeginAngle > 180.0 && pArc->dBeginAngle+pArc->dAngleSweep < 180.0)
					|| pArc->dBeginAngle+pArc->dAngleSweep <= -180.0)//如果经过180度
				{
					point.x = pArc->PosCenter.x - dRadiu;
					point.y = pArc->PosCenter.y;
					GetPointBox(point,BoxRect);		
				}
				if( (pArc->dBeginAngle > 270.0 && pArc->dBeginAngle+pArc->dAngleSweep < 270.0)
					|| pArc->dBeginAngle+pArc->dAngleSweep <= -90.0)//如果经过270度
				{
					point.x = pArc->PosCenter.x;
					point.y = pArc->PosCenter.y - dRadiu;
					GetPointBox(point,BoxRect);		
				}
			}
			else				//如果是逆时针
			{
				if(pArc->dBeginAngle+pArc->dAngleSweep >= 360.0)								//如果经过零度
				{
					point.x = pArc->PosCenter.x + dRadiu;
					point.y = pArc->PosCenter.y;
					GetPointBox(point,BoxRect);		
				}
				if( (pArc->dBeginAngle < 90.0 && pArc->dBeginAngle+pArc->dAngleSweep> 90.0)
					|| pArc->dBeginAngle+pArc->dAngleSweep >= 450.0)	//如果经过90度
				{
					point.x = pArc->PosCenter.x;
					point.y = pArc->PosCenter.y + dRadiu;
					GetPointBox(point,BoxRect);		
				}
				if( (pArc->dBeginAngle < 180.0 && int(pArc->dBeginAngle+pArc->dAngleSweep)%360 > 180.0)
					|| pArc->dBeginAngle+pArc->dAngleSweep >= 540.0)//如果经过180度
				{
					point.x = pArc->PosCenter.x - dRadiu;
					point.y = pArc->PosCenter.y;
					GetPointBox(point,BoxRect);		
				}
				if( (pArc->dBeginAngle < 270.0 && int(pArc->dBeginAngle+pArc->dAngleSweep)%360 > 270.0)
					|| pArc->dBeginAngle+pArc->dAngleSweep >= 630.0)//如果经过270度
				{
					point.x = pArc->PosCenter.x;
					point.y = pArc->PosCenter.y - dRadiu;
					GetPointBox(point,BoxRect);		
				}
			}				
		}
	}
	return BoxRect;
}

//更新一个box的区域
//一个新的坐标点
//需要更新的box
void CGraphScale::GetPointBox(DOUBLEPOINT point,BOX2D &BoxRect)
{
	BoxRect.min[0] = min(BoxRect.min[0],point.x);
	BoxRect.min[1] = min(BoxRect.min[1],point.y);
	BoxRect.max[0] = max(BoxRect.max[0],point.x);
	BoxRect.max[1] = max(BoxRect.max[1],point.y);
}