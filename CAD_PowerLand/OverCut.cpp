// OverCut.cpp: implementation of the COverCut class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "OverCut.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COverCut::COverCut()
{

}

COverCut::~COverCut()
{

}

/*
 *	���Ƿ���ֱ�ߣ�Ȼ�����
 */
void COverCut::NeedOverCut()
{
	g_pDoc->RemoveAllCollected();
	POSITION pos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	POSITION l_POSOverCut;
	while (pos != NULL)
	{	//������ͼԪ���Ҷ����
		l_POSOverCut = pos;
		CCreateBase* l_pIsPolyline = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(pos);
		if(l_pIsPolyline->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
		{	//�ҵ���ִ�й��к���
			CCreatePolyline *l_pPolyline = (CCreatePolyline*)l_pIsPolyline;
			//ֻ����ѡ�ĵ����ͽ����и�
			if(
	(g_pDoc->m_stcClothCut.m_bOverCutKnife1==1 && l_pIsPolyline->m_ColorOfLine==g_pDoc->m_stcColorToM.m_color1)||
	(g_pDoc->m_stcClothCut.m_bOverCutKnife2==1 && l_pIsPolyline->m_ColorOfLine==g_pDoc->m_stcColorToM.m_color2)||
	(g_pDoc->m_stcClothCut.m_bOverCutKnife3==1 && l_pIsPolyline->m_ColorOfLine==g_pDoc->m_stcColorToM.m_color3)||
	(g_pDoc->m_stcClothCut.m_bOverCutKnife4==1 && l_pIsPolyline->m_ColorOfLine==g_pDoc->m_stcColorToM.m_color4)||
	(g_pDoc->m_stcClothCut.m_bOverCutKnife5==1 && l_pIsPolyline->m_ColorOfLine==g_pDoc->m_stcColorToM.m_color5)
				)
			{
				//��ӳ�������
				m_pUndoList = m_UndoRedo.NewUndoList();
				m_pUndoList->OperatieWhat = ID_OVER_CUT;	
				m_pheadGraphPos = m_pCurGraphPos = m_UndoRedo.NewGraphPos();
				m_pUndoList->pGraphPos = m_pheadGraphPos;
				m_pUndoList->pOperatieParam = m_UndoRedo.NewOperateParam();
				//ִ�й���
				ExecuteOverCut(l_pPolyline,l_POSOverCut);	
			}
		}
	}
	g_pView->Invalidate();
}

/*
 *	ִ�й��к���
 */
void COverCut::ExecuteOverCut(CCreatePolyline* l_pIsPolyline,POSITION l_POSOverCut)
{
	if(l_pIsPolyline->m_point.GetSize() < 3)
	{	//���û�й��е�ͼ�Σ��ѿ�ʼ������ڴ��ͷŵ�
		if (m_pheadGraphPos->pCreateBasepos == NULL)
		{
			delete m_pUndoList->pGraphPos;
			m_pUndoList->pGraphPos = NULL;
			delete m_pUndoList->pOperatieParam;
			m_pUndoList->pOperatieParam = NULL;
			delete m_pUndoList;
			m_pUndoList = NULL;
		}
		return;
	}
	CByteArray l_carryNumPoint;
	//һ���Ƕ�������ֱ������ϵ�������
	Position l_posBegin,l_posEnd,l_posMiddle;
	//͹��								��������X��ĽǶ�
	double l_dTude1=0.0,l_dTude2=0.0,l_dAngle1,l_dAngle2;
	//�㼯�Ĵ�С
	int l_iSize = l_pIsPolyline->m_point.GetSize();
	//������ǲ��ǹ��н�
	l_posBegin = l_pIsPolyline->m_point.GetAt(l_iSize-1);
	l_posMiddle = l_pIsPolyline->m_point.GetAt(0);
	//������������˵����ֱ��
	l_dTude1 = l_pIsPolyline->m_dTuDu.GetAt(l_iSize-2);
	l_dTude2 = l_pIsPolyline->m_dTuDu.GetAt(0);
	//����Ǳպϵ�ͼ������ֱ��	
	if(PosEqual(l_posBegin,l_posMiddle)&&l_dTude1 == 0.0&&l_dTude2 == 0.0)
	{
		l_posBegin = l_pIsPolyline->m_point.GetAt(l_iSize-2);
		l_posEnd = l_pIsPolyline->m_point.GetAt(1);
		//����������X��ĽǶ�
		l_dAngle1 = GetAngle(l_posMiddle,l_posBegin);
		l_dAngle2 = GetAngle(l_posMiddle,l_posEnd);
		l_dAngle1 = fabs(l_dAngle1-l_dAngle2);//�����ߵļн�
		if(l_dAngle1 > 180) l_dAngle1 = 360 - l_dAngle1;
		//���㼯��ÿ����ֱ��֮��Ƕ��Ƿ���������
		if(l_dAngle1<g_pDoc->m_stcClothCut.m_dOverCutAngle)
			l_carryNumPoint.Add(TRUE);
		else
			l_carryNumPoint.Add(FALSE);	
	}
	else
		l_carryNumPoint.Add(FALSE);//��ס�����Ҫ���е�
	//���㼯��ÿ����ֱ��֮��Ƕ��Ƿ���������
	for(int i=0;i<l_iSize-2;i++)
	{
		//��ȡһ���Ƕ��е������������͹��
		l_posBegin = l_pIsPolyline->m_point.GetAt(i);
		l_posMiddle = l_pIsPolyline->m_point.GetAt(i+1);
		l_posEnd = l_pIsPolyline->m_point.GetAt(i+2);
		l_dTude1 = l_pIsPolyline->m_dTuDu.GetAt(i);
		l_dTude2 = l_pIsPolyline->m_dTuDu.GetAt(i+1);
		//������֮����������Ƿ���ֱ��
		if(l_dTude1==0.0&&l_dTude2==0.0)
		{
			//����������X��ĽǶ�
			l_dAngle1 = GetAngle(l_posBegin,l_posMiddle);
			l_dAngle2 = GetAngle(l_posEnd,l_posMiddle);
			l_dAngle1 = fabs(l_dAngle1-l_dAngle2);//�����ߵļн�
			if(l_dAngle1 > 180) l_dAngle1 = 360 - l_dAngle1;
			//���㼯��ÿ����ֱ��֮��Ƕ��Ƿ���������
			if(l_dAngle1<g_pDoc->m_stcClothCut.m_dOverCutAngle)
				l_carryNumPoint.Add(TRUE);//��ס�����Ҫ���е�
			else
				l_carryNumPoint.Add(FALSE);//�����ù���
		}
		else
			l_carryNumPoint.Add(FALSE);//�����ù���
	}
	//�Ѷ���߸��ݹ��еĵ�ֽ�Ϊ���
	PutPloyBreak(l_pIsPolyline,&l_carryNumPoint,l_POSOverCut);
}

/*
 *	�Ѷ���߸��ݹ��еĵ�ֽ�Ϊ���
 */
void COverCut::PutPloyBreak(CCreatePolyline* l_pIsPolyline,CByteArray* l_carryNumPoint,POSITION l_POSOverCut)
{
	CArray<Position,Position&> l_carrayPoint;//�㼯
	CArray<double,double&> l_carrayTuDu;//���͹�ȵ�
	//ֱ���ϵ�������
	Position l_posBegin,l_posEnd,l_posOutBegin,l_posOutEnd,l_posNext;
	//͹��					
	double l_dTude=0.0;
	//�㼯�Ĵ�С									���ǲ��ǹ��е�
	int l_iSize = l_carryNumPoint->GetSize(),l_iBool;
	for(int i=0;i<l_iSize;i++)
	{
		l_iBool = l_carryNumPoint->GetAt(i);
		//����������͹��
		l_posBegin = l_pIsPolyline->m_point.GetAt(i);
		l_posEnd = l_pIsPolyline->m_point.GetAt(i+1);
		l_dTude = l_pIsPolyline->m_dTuDu.GetAt(i);
		if(l_iBool == TRUE)//��������Ҫ����
		{	
			ProcLine(l_posNext,l_posBegin,0,g_pDoc->m_stcClothCut.m_dOverCutLong,l_posOutBegin,l_posOutEnd);//��ĩ��
			l_carrayPoint.Add(l_posOutEnd);
			l_carrayTuDu.Add(l_dTude);	
			//�������
			if(i!=0)
			{
				CCreatePolyline* pTemp = new CCreatePolyline(l_carrayPoint,l_carrayTuDu);
				pTemp->m_ColorOfLine = l_pIsPolyline->m_ColorOfLine;
				pTemp->m_strCBM.Copy(l_pIsPolyline->m_strCBM);	
				pTemp->m_strSBM.Copy(l_pIsPolyline->m_strSBM);	
				g_pDoc->m_CCreateBaseList.InsertBefore(l_POSOverCut,pTemp);
				//�������������Ԫ��
				if (m_pheadGraphPos->pCreateBasepos == NULL)	
				{
					m_pCurGraphPos->pCreateBasepos = (long)pTemp;
					POSITION pPOSITION  = g_pDoc->m_CCreateBaseList.Find(pTemp);
					m_pUndoList->pOperatieParam->pPosition = (LONG)pPOSITION;
					
				}
				else
				{
					m_pGraphPos = m_UndoRedo.NewGraphPos();						
					m_pGraphPos->pCreateBasepos = (long)pTemp;
					m_pCurGraphPos->pNext = m_pGraphPos;
					m_pGraphPos->pPrev = m_pCurGraphPos;
					m_pCurGraphPos = m_pGraphPos;
				}
			}
			l_carrayPoint.RemoveAll();
			l_carrayTuDu.RemoveAll();
			//��ĩ��
			ProcLine(l_posBegin,l_posEnd,1,g_pDoc->m_stcClothCut.m_dOverCutLong,l_posOutBegin,l_posOutEnd);//�����
			l_carrayPoint.Add(l_posOutBegin);
			l_carrayTuDu.Add(l_dTude);
			l_posNext = l_posBegin;
		}
		else //����Ҫ
		{
			l_posNext = l_posBegin;			
			//�ӵ�
			l_carrayPoint.Add(l_posBegin);
			l_carrayTuDu.Add(l_dTude);
		}
	}

	//��ĩ��ӽ�ȥ	
	l_posBegin = l_pIsPolyline->m_point.GetAt(l_iSize-1);
	l_posEnd = l_pIsPolyline->m_point.GetAt(l_iSize);
	l_dTude = l_pIsPolyline->m_dTuDu.GetAt(i);
	l_carrayTuDu.Add(l_dTude);	
	if(l_carryNumPoint->GetAt(0) == TRUE)//��β��ӵĲ���Ҫ���е�
	{
		ProcLine(l_posBegin,l_posEnd,0,g_pDoc->m_stcClothCut.m_dOverCutLong,l_posOutBegin,l_posOutEnd);//��ĩ��
		l_carrayPoint.Add(l_posOutEnd);
	}
	else
		l_carrayPoint.Add(l_posEnd);


	//�������������Ԫ��,����������߲��ù���
	if (m_pheadGraphPos->pCreateBasepos != NULL)
	{	//���ͼ������
		CCreatePolyline* pTemp = new CCreatePolyline(l_carrayPoint,l_carrayTuDu);
		pTemp->m_ColorOfLine = l_pIsPolyline->m_ColorOfLine;
		pTemp->m_strCBM.Copy(l_pIsPolyline->m_strCBM);	
		pTemp->m_strSBM.Copy(l_pIsPolyline->m_strSBM);	
		g_pDoc->m_CCreateBaseList.InsertBefore(l_POSOverCut,pTemp);
		//����������Ӹ�ͼ��
		m_pGraphPos = m_UndoRedo.NewGraphPos();						
		m_pGraphPos->pCreateBasepos = (long)pTemp;
		m_pCurGraphPos->pNext = m_pGraphPos;
		m_pGraphPos->pPrev = m_pCurGraphPos;
		m_pCurGraphPos = m_pGraphPos;
		m_pheadGraphPos->pCreateBaseposSpare1 = (LONG)l_pIsPolyline;
	}
	//���û�й��е�ͼ�Σ��ѿ�ʼ������ڴ��ͷŵ�
	if (m_pheadGraphPos->pCreateBasepos == NULL)
	{
		delete m_pUndoList->pGraphPos;
		m_pUndoList->pGraphPos = NULL;
		delete m_pUndoList->pOperatieParam;
		m_pUndoList->pOperatieParam = NULL;
		delete m_pUndoList;
		m_pUndoList = NULL;
	}
	else//����оͰ����������ӵ��ﳷ������
	{
		g_pDoc->m_Undo->AddUndoList(m_pUndoList);
		g_pDoc->m_CCreateBaseList.RemoveAt(l_POSOverCut);
	}
}

//���ܣ���ֱ�߼��ӳ���
//������CMyLine lnIn:ԭֱ��
//	int nType: 0 ��ʾ�����ӳ� 1��ʾ�����ӳ�
//	double distance: �ӳ�����
//	CMyLine& lnOut:�ӳ���ֱ��
void COverCut::ProcLine(Position ptStart1,Position ptEnd1, int nType, double distance,
						  Position& ptStart2,Position& ptEnd2)
{ 
	if ( fabs(distance-0.0)<DOUBLE_EQUAL )	//�ӳ�����Ϊ0
	{
		ptStart2.x = ptStart1.x;
		ptStart2.y = ptStart1.y;
		ptEnd2.x = ptEnd1.x;
		ptEnd2.y = ptEnd1.y;
		return;
	}

	//����б��
	if ( fabs(ptEnd1.x-ptStart1.x)<DOUBLE_EQUAL )		//б�ʲ����ڵ����
	{
		if (0 == nType)			//�����ӳ�
		{
			ptStart2.x = ptStart1.x;
			ptStart2.y = ptStart1.y;
			ptEnd2.x = ptEnd1.x;

			if (ptEnd1.y > ptStart1.y)
				ptEnd2.y = ptEnd1.y + distance;
			else
				ptEnd2.y = ptEnd1.y - distance;
		}
		else					//�����ӳ�
		{
			ptStart2.x = ptStart1.x;
			ptEnd2.x = ptEnd1.x;
			ptEnd2.y = ptEnd1.y;
			
			if (ptEnd1.y > ptStart1.y)
				ptStart2.y = ptStart1.y - distance;
			else
				ptStart2.y = ptStart1.y + distance;
		}
	}
	else														//б�ʴ��ڵ����
	{
		//б�ʴ��ڣ���б��Ϊ
		double K = (ptEnd1.y-ptStart1.y) / (ptEnd1.x-ptStart1.x);
		
		if ( fabs(K-0.0)<DOUBLE_EQUAL )		//б��Ϊ0
		{
			if (0 == nType)			//�����ӳ�
			{
				ptStart2.x = ptStart1.x;
				ptStart2.y = ptStart1.y;
				ptEnd2.y = ptEnd1.y;
				if (ptEnd1.x > ptStart1.x)
					ptEnd2.x = ptEnd1.x + distance;
				else
					ptEnd2.x = ptEnd1.x - distance;
			}
			else					//�����ӳ�
			{
				ptEnd2.x = ptEnd1.x;
				ptEnd2.y = ptEnd1.y;
				ptStart2.y = ptStart1.y;
				if (ptEnd1.x > ptStart1.x)
					ptStart2.x = ptStart1.x - distance;
				else
					ptStart2.x = ptStart1.x + distance;
				
			}
		}
		else if (K > 0)					//б��>0
		{
			double angle = atan(K);		//��X�ļн�
			if (0 == nType)
			{
				ptStart2.x = ptStart1.x;
				ptStart2.y = ptStart1.y;

				if (ptEnd1.x > ptStart1.x)
				{
					ptEnd2.x = ptEnd1.x + cos(angle) * distance;
					ptEnd2.y = ptEnd1.y + sin(angle) * distance;
				}
				else
				{
					ptEnd2.x = ptEnd1.x - cos(angle) * distance;
					ptEnd2.y = ptEnd1.y - sin(angle) * distance;
				}
			}
			else
			{
				ptEnd2.x = ptEnd1.x;
				ptEnd2.y = ptEnd1.y;

				if (ptEnd1.x > ptStart1.x)
				{
					ptStart2.x = ptStart1.x - cos(angle) * distance;
					ptStart2.y = ptStart1.y - sin(angle) * distance;
				}
				else
				{
					ptStart2.x = ptStart1.x + cos(angle) * distance;
					ptStart2.y = ptStart1.y + sin(angle) * distance;
				}
			}
		}
		else							//б��С��0
		{
			double angle = atan(fabs(K));	//��X��н�
			if (0 == nType)
			{
				ptStart2.x = ptStart1.x;
				ptStart2.y = ptStart1.y;

				if (ptEnd1.x > ptStart1.x)
				{
					ptEnd2.x = ptEnd1.x + cos(angle) * distance;
					ptEnd2.y = ptEnd1.y - sin(angle) * distance;
				}
				else
				{
					ptEnd2.x = ptEnd1.x - cos(angle) * distance;
					ptEnd2.y = ptEnd1.y + sin(angle) * distance;
				}
			}
			else
			{
				ptEnd2.x = ptEnd1.x;
				ptEnd2.y = ptEnd1.y;

				if (ptEnd1.x > ptStart1.x)
				{
					ptStart2.x = ptStart1.x - cos(angle) * distance;
					ptStart2.y = ptStart1.y + sin(angle) * distance;
				}
				else
				{
					ptStart2.x = ptStart1.x + cos(angle) * distance;
					ptStart2.y = ptStart1.y - sin(angle) * distance;
				}
			}
		}
		
	}
}