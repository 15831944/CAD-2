// DrawTrim.cpp: implementation of the CDrawTrim class.
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

CDrawTrim::CDrawTrim()
{

}

CDrawTrim::~CDrawTrim()
{

}


int CDrawTrim::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	CDC* pDC = g_pView->GetDC();
	CPoint l_pot;
	g_pView->WorldToScreem(l_cPos,l_pot);
	//ѡ��ͼ�ε��ĸ���
	int l_iPick=0;
	//�Ƿ�ѡ�еı�־
	BOOL	l_bPicked = FALSE;
	CCreateBase*	l_pCreateBase = NULL;
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//���ͷָ�룬������ѭ�����á�
	while(position != NULL)//����ɨ������
	{
		l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
		// ����������ͼԪ��ѡ��
		l_iPick = l_pCreateBase->IsPick(l_pot);
		if( l_iPick != -1 )
		{ 
			l_bPicked = TRUE;
			break;// ��ĳһ��ͼԪ��ʰȡ���󣬼��˳�ѭ��
		}
	}	
	//���ѡ��
	if(l_bPicked == TRUE)
	{
		l_pCreateBase->DrawMove(pDC,dmSelect);
		m_iNumOfClick++;
		if (m_iNumOfClick == 1)//������ĵ�һ��
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n��ѡ�е�һ���ߣ���ѡ�ڶ���");	
			else
				RemindContent("\r\nSelect the first line, and then choose the second��");	
			
			m_createbaseFirst = l_pCreateBase;
			g_pDoc->m_CollectOfPicked.Add(m_createbaseFirst);  // ��ͼԪ����ѡ����
			m_iPick1 = l_iPick;//�����ͼ���ϵ��ĸ��߶�
			m_posFirst = l_cPos;
		}
		if (m_iNumOfClick == 2)//������ĵڶ���
		{
			m_createbaseSecond = l_pCreateBase;
			//��������������ͼ��
			if (m_createbaseFirst == m_createbaseSecond)
			{
				//���ѡ��
				g_pDoc->RemoveAllCollected();
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n�㵽����һ��ͼ�Σ���Ч������ѡ���һ���ߣ�");			
				else
					RemindContent("\r\nPoint to the one graphics, invalid! To choose the first line��");	
				m_iNumOfClick = 1;	
				return FALSE;
			}
			m_posSecond =  l_cPos;
			m_iPick2 = l_iPick;//�����ͼ���ϵ��ĸ��߶�
			//�����޼�
			DoTrimGraph();
			//���ѡ��
			g_pDoc->RemoveAllCollected();
			m_iNumOfClick = 0;			
		}
	}
	else
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû��ѡ�У�����ѡ��",SEND_RICHEDIT);
		else
			RemindContent("\r\nNot selected, choose again��",SEND_RICHEDIT);	
		return FALSE;
	}
	g_pView->Invalidate();
	g_pView->ReleaseDC(pDC);
	return 0;	
}

int CDrawTrim::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	curPos = pos;
	//����׽
	FindPointAll(pDC,m_posFirst,curPos,g_pView->m_iSwitch);	
	if (m_iNumOfClick == 1)
	{	
		// ���������ڵ�ǰһ��λ��
		prePos = m_posSecond; 
		// �õ��豸����ָ��
		
		// ������ʱ���������һ����Ƥ��
		CCreateBase*	pTempLine1 = new CCreateLine(m_posFirst, prePos);
		if(g_refresh != 1)	//����Ϊ�˵�һ��û�в���ʱ������
			// ���Ӵ�û�б�ˢ��ʱ���ػ�ԭ������Ƥ��ʹ�䱻����
			pTempLine1->DrawMove(pDC,dmDrag);
		delete pTempLine1;
		//�趨��׽���ĵ�
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);	
		// ������ʱ���󣬸��ݵ�ǰλ�û���һ����Ƥ��
		CCreateBase*	pTempLine2 = new CCreateLine(m_posFirst, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		// �ͷ��豸����ָ��		
		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_posSecond = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	return 0;
}



int CDrawTrim::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//������ͼԪ//��Ҫ����ͼԪɾ��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;	//�����ѡ���ͼԪ
	return 0;
}

/*
 *	�����޼�
 */
void CDrawTrim::DoTrimGraph()
{
	if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{	
		AddUndoList();
		TrimLine();
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
	{
		AddUndoList();
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_createbaseFirst;	
		TrimPolyline(pPolyline->m_point,pPolyline->m_dTuDu);
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		AddUndoList();
		CCreateRectangle* pPolyline = (CCreateRectangle*)m_createbaseFirst;	
		TrimPolyline(pPolyline->m_point,pPolyline->m_dTuDu);
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{
		AddUndoList();
		TrimAarc();
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{
		AddUndoList();
		TrimRound();
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		AddUndoList();
		CCreateEllipse* pPolyline = (CCreateEllipse*)m_createbaseFirst;	
		TrimPolyline(pPolyline->m_point,pPolyline->m_dTuDu);
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
	}
	
}

/*
 *	�޼�ֱ��
 */
void CDrawTrim::TrimLine()
{
	CCreateLine* pLine = (CCreateLine*)m_createbaseFirst;
	int l_iNoUse = 0;
	//����
	Position posInter;
	if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateLine)))//ֱ��
	{
		CCreateLine* pLine2 = (CCreateLine*)m_createbaseSecond;
		//�󽻵�
		GetIntersectPoint(pLine->m_Begin,pLine->m_End,pLine2->m_Begin,pLine2->m_End,posInter);
		//������㲻���и����ϣ������в���
		if(!PosOnLine(pLine2->m_Begin,pLine2->m_End,posInter))
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû���ཻ��",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		//���Ƿ���ֱ����
		if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
			pLine->m_End = posInter;
		else
			pLine->m_Begin = posInter;
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateSpline)))//�����
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_createbaseSecond;
		//����������н���
		if(GetInterPosLineAndPolyline(l_iNoUse,posInter,pLine->m_Begin,pLine->m_End,pPolyline->m_point,pPolyline->m_dTuDu))
		{
			//���Ƿ���ֱ����
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
		}	
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*)m_createbaseSecond;
		//����������н���
		if(GetInterPosLineAndPolyline(l_iNoUse,posInter,pLine->m_Begin,pLine->m_End,pPolyline->m_point,pPolyline->m_dTuDu))
		{
			//���Ƿ���ֱ����
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
		}	
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{
		CCreateRoundArc* pArc = (CCreateRoundArc*)m_createbaseSecond;
		//��Բ���Ľ������
		int l_iNumOfInter =0;
		//����Բ����������
		Position l_posLine1 = pLine->m_Begin,l_posLine2 = pLine->m_End;
		if(GetIntersectLineRound(l_posLine1,l_posLine2,pArc->m_Center,pArc->m_dRadius))//�����Բ�н���
		{
			//��������ڻ���
			if(PosOnArc(l_posLine1,pArc->m_Begin,pArc->m_End,pArc->m_Center,pArc->m_dRadius,pArc->m_iDirect))
			{
				l_iNumOfInter++;
				posInter = l_posLine1;
			}
			if(PosOnArc(l_posLine2,pArc->m_Begin,pArc->m_End,pArc->m_Center,pArc->m_dRadius,pArc->m_iDirect))
			{
				l_iNumOfInter++;
				posInter = l_posLine2;
			}
		}
		//������㲻���и����ϣ������в���
		if(l_iNumOfInter == 0)
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû���ཻ��",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		else if(l_iNumOfInter == 1)//��һ������
		{
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
		}
		else if(l_iNumOfInter == 2)//�ж�������
		{
			//��Ӧ��ȡ�ĸ�����
			if(l_posLine1.Distance(m_posSecond) >= l_posLine2.Distance(m_posSecond))
				posInter = l_posLine2;
			else
				posInter = l_posLine1;
			//�޼�
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
			
		}
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{
		CCreateRound* pRound = (CCreateRound*)m_createbaseSecond;
		//��Բ���Ľ������
		int l_iNumOfInter =0;
		//����Բ����������
		Position l_posLine1 = pLine->m_Begin,l_posLine2 = pLine->m_End;
		if(GetIntersectLineRound(l_posLine1,l_posLine2,pRound->m_Center,pRound->m_dRadius))//�����Բ�н���
		{
			//��Ӧ��ȡ�ĸ�����
			if(l_posLine1.Distance(m_posSecond) >= l_posLine2.Distance(m_posSecond))
				posInter = l_posLine2;
			else
				posInter = l_posLine1;
			//�޼�
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
			
		}
		else//������㲻���и����ϣ������в���
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû���ཻ��",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*)m_createbaseSecond;
		//����������н���
		if(GetInterPosLineAndPolyline(l_iNoUse,posInter,pLine->m_Begin,pLine->m_End,pPolyline->m_point,pPolyline->m_dTuDu))
		{
			//���Ƿ���ֱ����
			if(PosOnLine(pLine->m_Begin,posInter,m_posFirst))
				pLine->m_End = posInter;
			else
				pLine->m_Begin = posInter;
		}	
	}
}

/*
 *	�޼�Բ��
 */
void CDrawTrim::TrimAarc()
{	
	CCreateRoundArc* pArc = (CCreateRoundArc*)m_createbaseFirst;	
	//���� 
	Position posInter1,posInter2;
	//�����ڶ���ߵ��Ķ�����,�������
	int l_iNumOfInter = 0;
	if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*)m_createbaseSecond;
		//����Բ����������
		posInter1 = pLine->m_Begin;posInter2 = pLine->m_End;
		//�����Բ�н���
		l_iNumOfInter = GetIntersectLineRound(posInter1,posInter2,pArc->m_Center,pArc->m_dRadius);		
		if(l_iNumOfInter == 0)
		{
			RemindNoInter();//��ʾû�н���		
			return;			
		}
		//ѡ����Ľ���//�ж������� //�����㶼�����ϣ����ҵ�һ��ľ�����ڵڶ���ʱѡ��ڶ���
		else if(l_iNumOfInter == 2)
		{
			int l_iLastInter = 0;
			if(PosOnLine(pLine->m_Begin,pLine->m_End,posInter1))
				l_iLastInter++;
			if(PosOnLine(pLine->m_Begin,pLine->m_End,posInter2))
				l_iLastInter++;
			if(l_iLastInter == 0)	
			{
				RemindNoInter();//��ʾû�н���		
				return;							
			}
			if(l_iLastInter == 1&&PosOnLine(pLine->m_Begin,pLine->m_End,posInter2))
				posInter1 = posInter2;	
			if(l_iLastInter == 2&&m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
				posInter1 = posInter2;	
		}
		GenerateArc((LONG)pArc,posInter1);
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_createbaseSecond;
		//����������н���
		if(GetInterPosArcAndPolyline(l_iNumOfInter,posInter1,pArc->m_Begin,pArc->m_End,pArc->m_Center
								,pArc->m_dRadius,pArc->m_iDirect,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateArc((LONG)pArc,posInter1);
		else
			RemindNoInter();//��ʾû�н���		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*)m_createbaseSecond;
		//����������н���
		if(GetInterPosArcAndPolyline(l_iNumOfInter,posInter1,pArc->m_Begin,pArc->m_End,pArc->m_Center
			,pArc->m_dRadius,pArc->m_iDirect,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateArc((LONG)pArc,posInter1);
		else
			RemindNoInter();//��ʾû�н���		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{
		CCreateRoundArc* pArcSecond = (CCreateRoundArc*)m_createbaseSecond;
		//�����Բ�н���
		l_iNumOfInter = GetIntersectArcRound(pArc->m_Center,pArc->m_dRadius,pArcSecond->m_Center,
			pArcSecond->m_End,pArcSecond->m_Center,pArcSecond->m_dRadius,pArcSecond->m_iDirect,posInter1,posInter2);		
		if(l_iNumOfInter == 0||l_iNumOfInter == -1)
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû���ཻ��",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		//ѡ����Ľ���//�ж������� //�����㶼�����ϣ����ҵ�һ��ľ�����ڵڶ���ʱѡ��ڶ���
		else if(m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
			posInter1 = posInter2;
		GenerateArc((LONG)pArc,posInter1);		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{
		CCreateRound* pRound = (CCreateRound*)m_createbaseSecond;
		//�����Բ�н���
		l_iNumOfInter = GetIntersectTwoRound(pRound->m_Center,pRound->m_dRadius,pArc->m_Center,
			pArc->m_dRadius,posInter1,posInter2);		
		if(l_iNumOfInter == 0||l_iNumOfInter == -1)
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû���ཻ��",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		//ѡ����Ľ���//�ж������� //�����㶼�����ϣ����ҵ�һ��ľ�����ڵڶ���ʱѡ��ڶ���
		else if(l_iNumOfInter == 2&&m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
			posInter1 = posInter2;
		GenerateArc((LONG)pArc,posInter1);
		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*)m_createbaseSecond;
		//����������н���
		if(GetInterPosArcAndPolyline(l_iNumOfInter,posInter1,pArc->m_Begin,pArc->m_End,pArc->m_Center
			,pArc->m_dRadius,pArc->m_iDirect,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateArc((LONG)pArc,posInter1);
		else
			RemindNoInter();//��ʾû�н���		
	}	
}

/*
 *	�޼�Բ
 */

void CDrawTrim::TrimRound()
{
	CCreateRound* pRound = (CCreateRound*)m_createbaseFirst;	
	//���� 
	Position posInter1,posInter2;
	//�����ڶ���ߵ��Ķ�����,�������
	int l_iNumOfInter = 0;
	if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*)m_createbaseSecond;
		//����Բ����������
		posInter1 = pLine->m_Begin;posInter2 = pLine->m_End;
		//�����Բ�н���
		l_iNumOfInter = GetIntersectLineRound(posInter1,posInter2,pRound->m_Center,pRound->m_dRadius);		
		if(l_iNumOfInter == 0)
		{
			RemindNoInter();//��ʾû�н���		
			return;			
		}
		//ѡ����Ľ���//�ж������� //�����㶼�����ϣ����ҵ�һ��ľ�����ڵڶ���ʱѡ��ڶ���
		else if(l_iNumOfInter == 2)
		{
			int l_iLastInter = 0;
			if(PosOnLine(pLine->m_Begin,pLine->m_End,posInter1))
				l_iLastInter++;
			if(PosOnLine(pLine->m_Begin,pLine->m_End,posInter2))
				l_iLastInter++;
			if(l_iLastInter == 0)	
			{
				RemindNoInter();//��ʾû�н���		
				return;							
			}
			if(l_iLastInter == 1&&PosOnLine(pLine->m_Begin,pLine->m_End,posInter2))
				posInter1 = posInter2;	
			if(l_iLastInter == 2&&m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
				posInter1 = posInter2;	
		}
		GenerateRound((LONG)m_createbaseFirst,posInter1);
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_createbaseSecond;
		//����������н���
		if(GetInterPosRoundAndPolyline(l_iNumOfInter,posInter1,pRound->m_Center,pRound->m_dRadius,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateRound((LONG)m_createbaseFirst,posInter1);
		else
			RemindNoInter();//��ʾû�н���		
		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*)m_createbaseSecond;
		//����������н���
		if(GetInterPosRoundAndPolyline(l_iNumOfInter,posInter1,pRound->m_Center,pRound->m_dRadius,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateRound((LONG)m_createbaseFirst,posInter1);
		else
			RemindNoInter();//��ʾû�н���		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{
		CCreateRoundArc* pArc = (CCreateRoundArc*)m_createbaseSecond;
		//�����Բ�н���
		l_iNumOfInter = GetIntersectArcRound(pRound->m_Center,pRound->m_dRadius,pArc->m_Center,
			pArc->m_End,pArc->m_Center,pArc->m_dRadius,pArc->m_iDirect,posInter1,posInter2);		
		if(l_iNumOfInter == 0||l_iNumOfInter == -1)
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû���ཻ��",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		//ѡ����Ľ���//�ж������� //�����㶼�����ϣ����ҵ�һ��ľ�����ڵڶ���ʱѡ��ڶ���
		else if(l_iNumOfInter == 2&&m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
			posInter1 = posInter2;
		GenerateRound((LONG)m_createbaseFirst,posInter1);		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{
		CCreateRound* pRoundSecond = (CCreateRound*)m_createbaseSecond;	
		//�����Բ�н���
		l_iNumOfInter = GetIntersectTwoRound(pRound->m_Center,pRound->m_dRadius,pRoundSecond->m_Center,
			pRoundSecond->m_dRadius,posInter1,posInter2);		
		if(l_iNumOfInter == 0||l_iNumOfInter == -1)
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû���ཻ��",SEND_RICHEDIT);
			else
				RemindContent("\r\nNo intersection point",SEND_RICHEDIT);	
			return;			
		}
		//ѡ����Ľ���//�ж������� //�����㶼�����ϣ����ҵ�һ��ľ�����ڵڶ���ʱѡ��ڶ���
		else if(l_iNumOfInter == 2&&m_posSecond.Distance(posInter1) >= m_posSecond.Distance(posInter2))
			posInter1 = posInter2;
		GenerateRound((LONG)m_createbaseFirst,posInter1);		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*)m_createbaseSecond;
		//����������н���
		if(GetInterPosRoundAndPolyline(l_iNumOfInter,posInter1,pRound->m_Center,pRound->m_dRadius,pPolyline->m_point,pPolyline->m_dTuDu))
			GenerateRound((LONG)m_createbaseFirst,posInter1);
		else
			RemindNoInter();//��ʾû�н���		
	}	
}

/*
 *	�޼������
 */
void CDrawTrim::TrimPolyline(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	//���� 
	Position posInter;
	//�����ڶ���ߵ��Ķ�����
	int l_iNumPoint = 0;
	if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*)m_createbaseSecond;
		//����������н���
		if(GetInterPosLineAndPolyline(l_iNumPoint,posInter,pLine->m_Begin,pLine->m_End,aryPoint,aryTudu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//�������ɶ����
		else
			RemindNoInter();//��ʾû�н���		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_createbaseSecond;
		//����������н���
		if(GetInterPosPolylineAndPolyline(l_iNumPoint,posInter,aryPoint,aryTudu,pPolyline->m_point,pPolyline->m_dTuDu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//�������ɶ����
		else
			RemindNoInter();//��ʾû�н���		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*)m_createbaseSecond;
		//����������н���
		if(GetInterPosPolylineAndPolyline(l_iNumPoint,posInter,aryPoint,aryTudu,pPolyline->m_point,pPolyline->m_dTuDu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//�������ɶ����
		else
			RemindNoInter();//��ʾû�н���		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{
		CCreateRoundArc* pArc = (CCreateRoundArc*)m_createbaseSecond;
		//����������н���
		if(GetInterPosArcAndPolyline(l_iNumPoint,posInter,pArc->m_Begin,pArc->m_End,pArc->m_Center,
			pArc->m_dRadius,pArc->m_iDirect,aryPoint,aryTudu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//�������ɶ����
		else
			RemindNoInter();//��ʾû�н���		
		
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{
		CCreateRound* pRound = (CCreateRound*)m_createbaseSecond;
		//����������н���
		if(GetInterPosRoundAndPolyline(l_iNumPoint,posInter,pRound->m_Center,pRound->m_dRadius,aryPoint,aryTudu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//�������ɶ����
		else
			RemindNoInter();//��ʾû�н���			
	}
	else if(m_createbaseSecond->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*)m_createbaseSecond;
		//����������н���
		if(GetInterPosPolylineAndPolyline(l_iNumPoint,posInter,aryPoint,aryTudu,pPolyline->m_point,pPolyline->m_dTuDu))
			GeneratePolyline(aryPoint,aryTudu,posInter,l_iNumPoint);//�������ɶ����
		else
			RemindNoInter();//��ʾû�н���		
	}
}

/*
 *	����Բ�������ߵĽ����޼�ֱ��
 */
BOOL CDrawTrim::GetInterPosArcAndPolyline(int& iNumPoint,Position& posInterClose,Position posBegin,
										  Position posEnd,Position posCenter,double dRadius,int iDir,
								CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	Position l_posPolyline1,l_posPolyline2,l_posInter1,l_posInter2,l_posCenter;//������ϵ�������
	//	����߽ڵ��͹��	����ĵڶ����뽻�����̾���		�ڶ����뽻��ľ���
	double l_dTudu,l_dMinDisInter = 999999999999,				l_dDis;
	//�㼯�Ĵ�С 
	int l_iSize = aryPoint.GetSize()-1,l_iNumInterPos;
	//�ȵ���Ƿ���Բ��
	BOOL l_bFirstArc = FALSE;
	if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		l_bFirstArc = TRUE;
	for(int i=0;i<l_iSize;i++)
	{
		l_posPolyline1 = aryPoint.GetAt(i);
		l_posPolyline2 = aryPoint.GetAt(i+1);
		l_dTudu = aryTudu.GetAt(i);
		//�����ζ������ֱ��
		if(l_dTudu == 0.0)
		{
			//�������󽻵�			
			if(l_bFirstArc==TRUE)//����ȵ����Բ��
			{
				//�õ�Բ��ֱ�ߵĽ���
				l_posInter1 = l_posPolyline1;l_posInter2 = l_posPolyline2;
				l_iNumInterPos = GetIntersectLineRound(l_posInter1,l_posInter2,posCenter,dRadius);
				if(l_iNumInterPos == 1)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter&&PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1))
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
				else if(l_iNumInterPos == 2)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter&&PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1))
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}			
					l_dDis = l_posInter2.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter&&PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter2))
					{
						posInterClose = l_posInter2;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
			}
			else//����ȵ���Ƕ����
			{
				l_posInter1 = l_posPolyline1;l_posInter2 = l_posPolyline2;
				l_iNumInterPos = GetIntersectLineArc(l_posInter1,l_posInter2,posBegin,posEnd,posCenter,dRadius,iDir);
				if(l_iNumInterPos == 1)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
						&&PosOnArc(l_posInter1,posBegin,posEnd,posCenter,dRadius,iDir))
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
				else if(l_iNumInterPos == 2)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
						&&PosOnArc(l_posInter1,posBegin,posEnd,posCenter,dRadius,iDir))
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}			
					l_dDis = l_posInter2.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter2)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
						&&PosOnArc(l_posInter2,posBegin,posEnd,posCenter,dRadius,iDir))
					{
						posInterClose = l_posInter2;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}				
			}
		}
		else//�����ζ������Բ��		
		{
			g_pDoc->polylineToArc(l_posPolyline1,l_posPolyline2,l_dTudu,l_posCenter);
			double l_dRadius = l_posCenter.Distance(l_posPolyline1);
			int l_iDir = RtnArcDir(l_dTudu);
			//�������󽻵�			
			if(l_bFirstArc==TRUE)//����ȵ����Բ��
			{
				//�õ�Բ��ֱ�ߵĽ���
				l_iNumInterPos = GetIntersectArcRound(posCenter,dRadius,l_posPolyline1,l_posPolyline2,l_posCenter,
					l_dRadius,l_iDir,l_posInter1,l_posInter2);
				if(l_iNumInterPos == 1)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter)
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
				else if(l_iNumInterPos == 2)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter)
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}			
					l_dDis = l_posInter2.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter)
					{
						posInterClose = l_posInter2;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
			}
			else//����ȵ���Ƕ����
			{
				//�뻡�н������			
				l_iNumInterPos = GetIntersectTwoArc(l_posPolyline1,l_posPolyline2,l_posCenter,l_dRadius,l_iDir,
					posBegin,posEnd,posCenter,dRadius,iDir,l_posInter1,l_posInter2);
				if((i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))//���������ĩ��Ҫ����Բ�Ľ���
					//�õ�Բ�뻡�Ľ���
					l_iNumInterPos = GetIntersectArcRound(l_posCenter,l_dRadius,posBegin,posEnd,
												posCenter,dRadius,iDir,l_posInter1,l_posInter2);				
				if(l_iNumInterPos == 1)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter)
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}
				else if(l_iNumInterPos == 2)
				{
					l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
						&&PosOnArc(l_posInter1,posBegin,posEnd,posCenter,dRadius,iDir))
					{
						posInterClose = l_posInter1;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}			
					l_dDis = l_posInter2.Distance(m_posSecond);//����������ľ���
					if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter2)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
						&&PosOnArc(l_posInter2,posBegin,posEnd,posCenter,dRadius,iDir))
					{
						posInterClose = l_posInter2;
						l_dMinDisInter = l_dDis;
						iNumPoint = i;
					}								
				}				
			}			
		}
	}	
	if(l_dMinDisInter < 999999999990)//˵���н���
		return TRUE;
	return FALSE;
}

/*
 *	����Բ�����ߵĽ����޼�ֱ��
 */
 BOOL CDrawTrim::GetInterPosRoundAndPolyline(int& iNumPoint,Position& posInterClose,Position posCenter,
	 double dRadius,CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	Position l_posPolyline1,l_posPolyline2,l_posInter1,l_posInter2,l_posCenter;//������ϵ�������
	//	����߽ڵ��͹��	����ĵڶ����뽻�����̾���		�ڶ����뽻��ľ���
	double l_dTudu,l_dMinDisInter = 999999999999,				l_dDis;
	//�㼯�Ĵ�С 
	int l_iSize = aryPoint.GetSize()-1,l_iNumInterPos;
	//�ȵ���Ƿ���Բ��
	BOOL l_bFirstArc = FALSE;
	if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		l_bFirstArc = TRUE;
	for(int i=0;i<l_iSize;i++)
	{
		l_posPolyline1 = aryPoint.GetAt(i);
		l_posPolyline2 = aryPoint.GetAt(i+1);
		l_dTudu = aryTudu.GetAt(i);
		//�����ζ������ֱ��
		if(l_dTudu == 0.0)
		{
			//�������󽻵�			
			if(l_bFirstArc==TRUE)//����ȵ����Բ��
			//�õ�Բ��ֱ�ߵĽ���
			l_posInter1 = l_posPolyline1;l_posInter2 = l_posPolyline2;
			l_iNumInterPos = GetIntersectLineRound(l_posInter1,l_posInter2,posCenter,dRadius);
			if(l_iNumInterPos == 1)
			{
				l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
				//��������������� ���� ���������ϡ����ȵ���Ƕ���߲�������ĩ��ʱ���Բ���������
				if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1)||
					(l_bFirstArc==FALSE&&((i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1)))))
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}								
			}
			else if(l_iNumInterPos == 2)
			{
				l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
				if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter1)||
					(l_bFirstArc==FALSE&&((i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1)))))
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}			
				l_dDis = l_posInter2.Distance(m_posSecond);//����������ľ���
				if(l_dDis <= l_dMinDisInter&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter2)||
					(l_bFirstArc==FALSE&&((i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1)))))
				{
					posInterClose = l_posInter2;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}								
			}
		}
		else//�����ζ������Բ��		
		{
			g_pDoc->polylineToArc(l_posPolyline1,l_posPolyline2,l_dTudu,l_posCenter);
			double l_dRadius = l_posCenter.Distance(l_posPolyline1);
			int l_iDir = RtnArcDir(l_dTudu);
			//�õ�Բ�뻡�Ľ���
			l_iNumInterPos = GetIntersectArcRound(posCenter,dRadius,l_posPolyline1,l_posPolyline2,l_posCenter,
				l_dRadius,l_iDir,l_posInter1,l_posInter2);
			//����ȵ���Ƕ���߲�������ĩ��	
			if(l_bFirstArc==FALSE&&((i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1)))
				l_iNumInterPos = GetIntersectTwoRound(posCenter,dRadius,l_posCenter,l_dRadius,l_posInter1,l_posInter2);
			if(l_iNumInterPos == 1)
			{
				l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}								
			}
			else if(l_iNumInterPos == 2)
			{
				l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}			
				l_dDis = l_posInter2.Distance(m_posSecond);//����������ľ���
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter2;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}								
			}
		}
	}	
	if(l_dMinDisInter < 999999999990)//˵���н���
		return TRUE;
	return FALSE;
}

/*
 *	����ֱ�������ߵĽ����޼�ֱ��
 */
BOOL CDrawTrim::GetInterPosLineAndPolyline(int& iNumPoint,Position& posInterClose,Position posBegin,Position posEnd,
		CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	Position l_posPolyline1,l_posPolyline2,l_posInter;//������ϵ�������
	double l_dTudu,l_dMinDisInter = 999999999999;
	//�㼯�Ĵ�С 
	int l_iSize = aryPoint.GetSize()-1;
	//�ȵ���Ƿ���ֱ��
	BOOL l_bFirstLine = FALSE;
	if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		l_bFirstLine = TRUE;
	for(int i=0;i<l_iSize;i++)
	{
		l_posPolyline1 = aryPoint.GetAt(i);
		l_posPolyline2 = aryPoint.GetAt(i+1);
		l_dTudu = aryTudu.GetAt(i);
		//�����ֱ��
		if(l_dTudu == 0.0)
		{
			//�󽻵�
			if(GetIntersectPoint(posBegin,posEnd,l_posPolyline1,l_posPolyline2,l_posInter))
			{
				double l_dDis = l_posInter.Distance(m_posSecond);//����������ľ���
				//����������и����ϲ��ұ��ϸ���������,�ȵ������ֱ��
				if(l_bFirstLine==TRUE&&PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter)&&l_dDis < l_dMinDisInter)
				{
					posInterClose = l_posInter;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}	//���ȵ�Ķ����					���ȵ�Ķ���ߵĵ�һ�������һ�����Բ����ڶ������
				else if(l_bFirstLine==FALSE&&(PosOnLine(l_posPolyline1,l_posPolyline2,l_posInter)||(i==0&&m_iPick1==0)||(i==l_iSize-1&&m_iPick1==l_iSize-1))
					&&PosOnLine(posBegin,posEnd,l_posInter)&&l_dDis < l_dMinDisInter)
				{
					posInterClose = l_posInter;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;					
				}
			}
		}
		else
		{
			Position l_posInter1 = posBegin,l_posInter2 = posEnd,l_posCenter;
			g_pDoc->polylineToArc(l_posPolyline1,l_posPolyline2,l_dTudu,l_posCenter);
			//�뻡�н������
			int l_iNumInterPos = GetIntersectLineArc(l_posInter1,l_posInter2,l_posPolyline1,
				l_posPolyline2,l_posCenter,l_posCenter.Distance(l_posPolyline1),RtnArcDir(l_dTudu));
			if((i==0||i==l_iSize-1)&& l_bFirstLine==FALSE)//����ǵ�һ���߻����һ����Ҫ�ӳ����͵ÿ���Բ
			{
				l_posInter1 = posBegin;l_posInter2 = posEnd;
				l_iNumInterPos = GetIntersectLineRound(l_posInter1,l_posInter2,l_posCenter,l_posCenter.Distance(l_posPolyline1));
			}
			if(l_iNumInterPos == 1)//�����һ������
			{
				double l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}			
			}				
			if(l_iNumInterPos == 2)//�������������
			{
				double l_dDis = l_posInter1.Distance(m_posSecond);//����������ľ���
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter1;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}			
				l_dDis = l_posInter2.Distance(m_posSecond);//����������ľ���
				if(l_dDis <= l_dMinDisInter)
				{
					posInterClose = l_posInter2;
					l_dMinDisInter = l_dDis;
					iNumPoint = i;
				}			
			}
		}
	}	
	if(l_dMinDisInter < 999999999990)//˵���н���
		return TRUE;
	return FALSE;
}

/*
 *	���ض���������ߵĽ����޼������
 */
BOOL CDrawTrim::GetInterPosPolylineAndPolyline(int& iNumPoint,Position& posInterClose,CArray<Position,Position&> &aryPoint1, CArray<double,double&> &aryTudu1,CArray<Position,Position&> &aryPoint2,CArray<double,double&> &aryTudu2)
{
	Position l_posPolyline11,l_posPolyline12,l_posPolyline21,l_posPolyline22,l_posCenter,l_posInter;//������ϵ�������
	double l_dTudu1,l_dTudu2,l_dRadius1,l_dRadius2,l_dMinDisInter = 999999999999;
	//�㼯�Ĵ�С 
	int l_iSize1 = aryPoint1.GetSize()-1;
	int l_iSize2 = aryPoint2.GetSize()-1;
	//����������ľ���
	double l_dDis;
	for(int i=0;i<l_iSize1;i++)//��һ�ε������
	{
		l_posPolyline11 = aryPoint1.GetAt(i);
		l_posPolyline12 = aryPoint1.GetAt(i+1);
		l_dTudu1 = aryTudu1.GetAt(i);
		//��һ�ε�����������ֱ��
		if(l_dTudu1 == 0.0)
		{
			for(int j=0;j<l_iSize2;j++)//�ڶ��ε������
			{
				l_posPolyline21 = aryPoint2.GetAt(j);
				l_posPolyline22 = aryPoint2.GetAt(j+1);
				//�󽻵�
				l_dTudu2 = aryTudu2.GetAt(j);
				if(l_dTudu2 == 0.0)//ֱ����ֱ�߽���
				{
					if(GetIntersectPoint(l_posPolyline21,l_posPolyline22,l_posPolyline11,l_posPolyline12,l_posInter))
					{
						l_dDis = l_posInter.Distance(m_posSecond);//����������ľ���
						//����������и����ϲ��ұ��ϸ���������					ͷ��βʱ�п������ӳ�����û��Ҫ��������
						if((PosOnLine(l_posPolyline11,l_posPolyline12,l_posInter)||i==0||i==l_iSize1-1)&&
							PosOnLine(l_posPolyline21,l_posPolyline22,l_posInter)&&l_dDis < l_dMinDisInter)
						{
							posInterClose = l_posInter;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}			
					}
				}
				else//ֱ����Բ������
				{
					//��Բ������������
					Position posArcInter1 = l_posPolyline11,posArcInter2 = l_posPolyline12;
					g_pDoc->polylineToArc(l_posPolyline21,l_posPolyline22,l_dTudu2,l_posCenter);//��Բ��
					l_dRadius1 = l_posCenter.Distance(l_posPolyline21);
					//�������
					int l_iNumInterPos = GetIntersectLineArc(posArcInter1,posArcInter2,l_posPolyline21,
						l_posPolyline22,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu2));
					if(l_iNumInterPos == 1)//���ֻ��һ������
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//����������ľ���
						//����������и����ϲ��ұ��ϸ���������					ͷ��βʱ�п������ӳ�����û��Ҫ��������
						if((PosOnLine(l_posPolyline11,l_posPolyline12,posArcInter1)||i==0||i==l_iSize1-1)&&
							PosOnArc(posArcInter1,l_posPolyline21,l_posPolyline22,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu2))
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
					}
					else if(l_iNumInterPos == 2)//����ж�������
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//����������ľ���
						//����������и����ϲ��ұ��ϸ���������					ͷ��βʱ�п������ӳ�����û��Ҫ��������
						if((PosOnLine(l_posPolyline11,l_posPolyline12,posArcInter1)||i==0||i==l_iSize1-1)&&
							PosOnArc(posArcInter1,l_posPolyline21,l_posPolyline22,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu2))
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
						l_dDis = posArcInter2.Distance(m_posSecond);//����������ľ���
						//����������и����ϲ��ұ��ϸ���������					ͷ��βʱ�п������ӳ�����û��Ҫ��������
						if((PosOnLine(l_posPolyline11,l_posPolyline12,posArcInter2)||i==0||i==l_iSize1-1)&&
							PosOnArc(posArcInter2,l_posPolyline21,l_posPolyline22,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu2))
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter2;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}											
					}
				}
			}			
		}
		else//��һ�ε�����������Բ��		
		{
			for(int j=0;j<l_iSize2;j++)//�ڶ��ε������
			{
				l_posPolyline21 = aryPoint2.GetAt(j);
				l_posPolyline22 = aryPoint2.GetAt(j+1);
				//�󽻵�
				l_dTudu2 = aryTudu2.GetAt(j);
				if(l_dTudu2 == 0.0)//Բ����ֱ�߽���
				{
					//��Բ������������
					Position posArcInter1 = l_posPolyline21,posArcInter2 = l_posPolyline22;
					g_pDoc->polylineToArc(l_posPolyline11,l_posPolyline12,l_dTudu1,l_posCenter);//��Բ��
					l_dRadius1 = l_posCenter.Distance(l_posPolyline11);
					//�������
					int l_iNumInterPos = GetIntersectLineArc(posArcInter1,posArcInter2,l_posPolyline11,
						l_posPolyline12,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu1));
					if(i==0||i==l_iSize1-1 )//����ǵ�һ���߻����һ����Ҫ�ӳ����͵ÿ���Բ
					{
						posArcInter1 = l_posPolyline21;posArcInter2 = l_posPolyline22;
						l_iNumInterPos = GetIntersectLineRound(posArcInter1,posArcInter2,l_posCenter,l_dRadius1);
					}
					if(l_iNumInterPos == 1)//���ֻ��һ������
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//����������ľ���
						//����������и����ϲ��ұ��ϸ���������					ͷ��βʱ�п������ӳ�����û��Ҫ��������
						if(PosOnLine(l_posPolyline21,l_posPolyline22,posArcInter1)&&
							(PosOnArc(posArcInter1,l_posPolyline11,l_posPolyline12,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
					}
					else if(l_iNumInterPos == 2)//����ж�������
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//����������ľ���
						//����������и����ϲ��ұ��ϸ���������					ͷ��βʱ�п������ӳ�����û��Ҫ��������
						if(PosOnLine(l_posPolyline21,l_posPolyline22,posArcInter1)&&
							(PosOnArc(posArcInter1,l_posPolyline11,l_posPolyline12,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
						l_dDis = posArcInter2.Distance(m_posSecond);//����������ľ���
						//����������и����ϲ��ұ��ϸ���������					ͷ��βʱ�п������ӳ�����û��Ҫ��������
						if(PosOnLine(l_posPolyline21,l_posPolyline22,posArcInter2)&&
							(PosOnArc(posArcInter2,l_posPolyline11,l_posPolyline12,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter2;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}											
					}
				}
				else//Բ����Բ���ཻ
				{
					//Բ����Բ������������
					Position posArcInter1,posArcInter2,l_posCenter1,l_posCenter2;
					g_pDoc->polylineToArc(l_posPolyline11,l_posPolyline12,l_dTudu1,l_posCenter1);//��Բ��
					l_dRadius1 = l_posCenter1.Distance(l_posPolyline11);
					g_pDoc->polylineToArc(l_posPolyline21,l_posPolyline22,l_dTudu2,l_posCenter2);//��Բ��
					l_dRadius2 = l_posCenter2.Distance(l_posPolyline21);
					
					//�������
					int l_iNumInterPos = GetIntersectTwoArc(l_posPolyline11,l_posPolyline12,l_posCenter1,
						l_dRadius1,RtnArcDir(l_dTudu1),l_posPolyline21,l_posPolyline22,l_posCenter2,
						l_dRadius2,RtnArcDir(l_dTudu2),posArcInter1,posArcInter2);
					if(i==0||i==l_iSize1-1 )//����ǵ�һ���߻����һ����Ҫ�ӳ����͵ÿ���Բ
					{
						l_iNumInterPos = GetIntersectArcRound(l_posCenter1,l_dRadius1,l_posPolyline21,
							l_posPolyline22,l_posCenter2,l_dRadius2,RtnArcDir(l_dTudu2),posArcInter1,posArcInter2);
					}
					if(l_iNumInterPos == 1)//���ֻ��һ������
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//����������ľ���
						//����������и����ϲ��ұ��ϸ���������					ͷ��βʱ�п������ӳ�����û��Ҫ��������
						if(PosOnArc(posArcInter1,l_posPolyline21,l_posPolyline22,l_posCenter2,l_dRadius2,RtnArcDir(l_dTudu2))&&
							(PosOnArc(posArcInter1,l_posPolyline11,l_posPolyline12,l_posCenter1,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
					}
					else if(l_iNumInterPos == 2)//����ж�������
					{
						l_dDis = posArcInter1.Distance(m_posSecond);//����������ľ���
						//����������и����ϲ��ұ��ϸ���������					ͷ��βʱ�п������ӳ�����û��Ҫ��������
						if(PosOnArc(posArcInter1,l_posPolyline21,l_posPolyline22,l_posCenter2,l_dRadius2,RtnArcDir(l_dTudu2))&&
							(PosOnArc(posArcInter1,l_posPolyline11,l_posPolyline12,l_posCenter1,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter1;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}									
						l_dDis = posArcInter2.Distance(m_posSecond);//����������ľ���
						//����������и����ϲ��ұ��ϸ���������					ͷ��βʱ�п������ӳ�����û��Ҫ��������
						if(PosOnArc(posArcInter2,l_posPolyline21,l_posPolyline22,l_posCenter2,l_dRadius2,RtnArcDir(l_dTudu2))&&
							(PosOnArc(posArcInter2,l_posPolyline11,l_posPolyline12,l_posCenter,l_dRadius1,RtnArcDir(l_dTudu1))||i==0||i==l_iSize1-1)
							&&l_dDis < l_dMinDisInter)
						{
							posInterClose = posArcInter2;
							l_dMinDisInter = l_dDis;
							iNumPoint = i;
						}											
					}			
				}
			}								
		}
	}	
	if(l_dMinDisInter < 999999999990)//˵���н���
		return TRUE;
	return FALSE;
}

/*
 *	������������Ӳ���
 */
void CDrawTrim::AddUndoList()
{
	m_pUndoList = m_UndoRedo.NewUndoList();
	m_pUndoList->OperatieWhat = ID_IO_KNIFE;
	
	m_pCurGraphPos = m_UndoRedo.NewGraphPos();
	m_pCurGraphPos->pCreateBasepos = (LONG)m_createbaseFirst;
	//�����ֱ�ߵĻ�
	if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*) m_createbaseFirst;
		m_pUndoList->pOperatieParam = m_UndoRedo.NewOperateParam();
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potBase,pLine->m_Begin);
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potDes,pLine->m_End);		
	}
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))//�����
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*) m_createbaseFirst;
		m_pCurGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		m_pCurGraphPos->aryPoint.Copy(pPolyline->m_point);
		m_pCurGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		m_pCurGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))//����
	{
		CCreateRectangle* pPolyline = (CCreateRectangle*) m_createbaseFirst;
		m_pCurGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		m_pCurGraphPos->aryPoint.Copy(pPolyline->m_point);
		m_pCurGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		m_pCurGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))//��Բ
	{
		CCreateEllipse* pPolyline = (CCreateEllipse*) m_createbaseFirst;
		m_pCurGraphPos->aryPoint.SetSize(pPolyline->m_point.GetSize());
		m_pCurGraphPos->aryPoint.Copy(pPolyline->m_point);
		m_pCurGraphPos->aryTudu.SetSize(pPolyline->m_dTuDu.GetSize());
		m_pCurGraphPos->aryTudu.Copy(pPolyline->m_dTuDu);
	}	
	else if(m_createbaseFirst->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))//Բ��
	{
		CCreateRoundArc* pArc = (CCreateRoundArc*) m_createbaseFirst;
		m_pUndoList->pOperatieParam = m_UndoRedo.NewOperateParam();
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potBase,pArc->m_Begin);
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potDes,pArc->m_End);	
		m_pUndoList->pOperatieParam->angle = pArc->m_dAngleStart;
		m_pUndoList->pOperatieParam->distance = pArc->m_dAngleEnd;
	}
	m_pUndoList->pGraphPos = m_pCurGraphPos;
	g_pDoc->m_Undo->AddUndoList(m_pUndoList);
}

/*
 *	ͨ��Բ������Բ��
 */
BOOL CDrawTrim::GenerateRound(LONG pRound,Position posInter)
{
	CCreateRound* pRoundTurn = (CCreateRound*)pRound;
	CCreateRoundArc* pNewArc = NULL;
	//�����Ͷϵ�֮��
	if(PosOnArc(m_posFirst,pRoundTurn->m_EndPos,posInter,pRoundTurn->m_Center,pRoundTurn->m_dRadius,pRoundTurn->m_iDirect))
		pNewArc = new CCreateRoundArc(pRoundTurn->m_EndPos,pRoundTurn->m_Center,posInter,pRoundTurn->m_iDirect);
	else
		pNewArc = new CCreateRoundArc(posInter,pRoundTurn->m_Center,pRoundTurn->m_EndPos,pRoundTurn->m_iDirect);
	POSITION POS = g_pDoc->m_CCreateBaseList.Find(m_createbaseFirst);
	g_pDoc->m_CCreateBaseList.InsertBefore(POS,pNewArc);
	m_pCurGraphPos->pCreateBaseposSpare1 = (LONG)pNewArc;	
	g_pDoc->m_CCreateBaseList.RemoveAt(POS);		
	
	return TRUE;
}

/*
 *	������Բ��
 */
BOOL CDrawTrim::GenerateArc(LONG pArc,Position posInter)
{
	CCreateRoundArc* pArcTurn = (CCreateRoundArc*)pArc;
	//�����Բ����
	if(PosOnArc(posInter,pArcTurn->m_Begin,pArcTurn->m_End,pArcTurn->m_Center,pArcTurn->m_dRadius,pArcTurn->m_iDirect))
	{
		if(PosOnArc(m_posFirst,pArcTurn->m_Begin,posInter,pArcTurn->m_Center,pArcTurn->m_dRadius,pArcTurn->m_iDirect))
			pArcTurn->m_End = posInter;
		else
			pArcTurn->m_Begin = posInter;		
	}	
	else//����Բ����
	{
		if(m_posFirst.Distance(pArcTurn->m_Begin) >= m_posFirst.Distance(pArcTurn->m_End))
			pArcTurn->m_End = posInter;
		else
			pArcTurn->m_Begin = posInter;
	}
	pArcTurn->m_dAngleEnd = GetAngle(pArcTurn->m_Center,pArcTurn->m_End);
	pArcTurn->m_dAngleStart = GetAngle(pArcTurn->m_Center,pArcTurn->m_Begin);
	return TRUE;
}

/*
 *	�����µĶ����
 */
BOOL CDrawTrim::GeneratePolyline(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,
								 Position posInter,int l_iNumPoint)
{
	//���� 
	Position l_posBegin,l_posEnd,l_posCenter;
	double l_dTudu = 0.0;
	l_posBegin = aryPoint.GetAt(l_iNumPoint);
	l_posEnd = aryPoint.GetAt(l_iNumPoint + 1);
	l_dTudu = aryTudu.GetAt(l_iNumPoint);
	//������Ƿ��ڿ�ʼ���뽻��֮��
	BOOL l_bClickOnBegin = FALSE;
	double l_dNewTudu = 0.0;
	//��������������һ������ʱ�����ݽ������ߵ��ı���ɾ��������
	if(l_dTudu == 0.0)
		l_bClickOnBegin = PosOnLine(l_posBegin,posInter,m_posFirst);
	else
	{
		g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);//��Բ��
		//���µ�Բ����͹��
		l_dNewTudu = g_pDoc->RtnTudu(l_posBegin,posInter,l_posCenter,RtnArcDir(l_dTudu));
		l_bClickOnBegin = PosOnArc(m_posFirst,l_posBegin,posInter,l_posCenter,l_posCenter.Distance(l_posBegin),RtnArcDir(l_dTudu));
		if(l_iNumPoint == 0&&!PosOnArc(posInter,l_posBegin,l_posEnd,l_posCenter,l_posCenter.Distance(l_posBegin),RtnArcDir(l_dTudu)))
		{
			l_dNewTudu = g_pDoc->RtnTudu(posInter,l_posEnd,l_posCenter,RtnArcDir(l_dTudu));					
			l_bClickOnBegin = FALSE;
		}
	}
	//�������ɶ����
	if(l_iNumPoint>m_iPick1||(l_iNumPoint==m_iPick1&&l_bClickOnBegin))
	{
		((CCreatePolyline*)m_createbaseFirst)->m_point.RemoveAt(l_iNumPoint+1,aryPoint.GetSize()-l_iNumPoint-1);
		((CCreatePolyline*)m_createbaseFirst)->m_dTuDu.RemoveAt(l_iNumPoint+1,aryTudu.GetSize()-l_iNumPoint-1);
		((CCreatePolyline*)m_createbaseFirst)->m_point.Add(posInter);
		((CCreatePolyline*)m_createbaseFirst)->m_dTuDu.SetAt(aryTudu.GetSize()-1,l_dNewTudu);
	}
	else
	{
		((CCreatePolyline*)m_createbaseFirst)->m_point.RemoveAt(0,l_iNumPoint+1);
		((CCreatePolyline*)m_createbaseFirst)->m_dTuDu.RemoveAt(0,l_iNumPoint+1);
		((CCreatePolyline*)m_createbaseFirst)->m_point.InsertAt(0,posInter);
		if(l_dNewTudu!=0.0)
			l_dNewTudu = g_pDoc->RtnTudu(posInter,l_posEnd,l_posCenter,RtnArcDir(l_dTudu));
		((CCreatePolyline*)m_createbaseFirst)->m_dTuDu.InsertAt(0,l_dNewTudu);
	}				
	return TRUE;
}

/*
 *	��ʾû�н���
 */
void CDrawTrim::RemindNoInter()
{
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\nû�н���:");
	else
		RemindContent("\r\nNo intersection:");					
	
}



