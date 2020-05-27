// DrawBreakTwo.cpp: implementation of the CDrawBreakTwo class.
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

CDrawBreakTwo::CDrawBreakTwo()
: m_Begin(0,0), m_End(0,0)
{
	m_pCreateBase = NULL;
	m_pCreateBase1 = NULL;
	m_pCreateBase2 = NULL;
	
}

CDrawBreakTwo::~CDrawBreakTwo()
{

}

/*
 *	����������µ��ߣ����ұ�ע�ϵ�һ��
 */
void CDrawBreakTwo::InvalLastLine()
{
	CDC *pDC = g_pView->GetDC();	
	CPoint pt;
	g_pView->WorldToScreem(m_Begin,pt);
	pDC->MoveTo(pt.x-5,pt.y-5);
	pDC->LineTo(pt.x+5,pt.y+5);
	pDC->MoveTo(pt.x+5,pt.y-5);
	pDC->LineTo(pt.x-5,pt.y+5);
//	pDC->TextOut(pt.x,pt.y,"��һ��");
	
/*
	pDC->MoveTo(pt.x-5,pt.y-5);
	pDC->LineTo(pt.x-5,pt.y+1);
	pDC->LineTo(pt.x-1,pt.y+1);
	pDC->LineTo(pt.x-1,pt.y+5);
	pDC->LineTo(pt.x+5,pt.y+5);
	pDC->LineTo(pt.x+5,pt.y-1);
	pDC->LineTo(pt.x+1,pt.y-1);
	pDC->LineTo(pt.x+1,pt.y-5);
	pDC->LineTo(pt.x-5,pt.y-5);
*/
	g_pView->ReleaseDC(pDC);
	
}

int CDrawBreakTwo::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	int l_iInvali;
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	//����ĳ���
	double l_dInputLong=0.0;
	CPoint l_pot;
	g_pView->WorldToScreem(l_cPos,l_pot);
	//����ڶ���ʱ
	if (m_iNumOfClick == 1)
	{
		//���������ĳ��ȵĻ�
		if(nFlags == 2)//��������
		{
			if (m_pCreateBase2 == NULL)//����������ʱû���Ҽ����
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nû���Ҽ�����ͼ�Σ����µ����ESC��ȡ��");
				else
					RemindContent("\r\nNo right point graphic, click or press the ESC key to cancel��");	
				return FALSE;				
			}
			l_dInputLong = atof(cmd);
			//����ĳ��ȷ������ϵĵ㣬���ؼ�˵������ĳ��Ȳ��ڷ�Χ��
			if (!m_pCreateBase->RtnLongPos(m_posRight,l_dInputLong,l_cPos,l_iInvali)) 
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n������Χ�����µ����ESC��ȡ��");
				else
					RemindContent("\r\nBeyond the range, click or press the ESC key to cancel��");	
				return FALSE;
			}
			else
			{
				m_pCreateBase = m_pCreateBase1;
				m_iNumOfClick++;	
			}
		}
		else
		{
			if (RtnPickLine(l_pot,2)&&m_pCreateBase2 == m_pCreateBase1)
			{
				m_pCreateBase = m_pCreateBase2;
				m_iNumOfClick++;
			}	
			else
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nû�е���ͼ�Σ����µ����ESC��ȡ��");				
				else
					RemindContent("\r\nThere is no point graphics, click or press the ESC key to cancel��");	
				return FALSE;
			}
		}
	}
	// �������ͼԪ
	if(m_iNumOfClick == 0)	
	{	
		//���������ĳ��ȵĻ�
		if(nFlags == 2)//��������
		{
			if (m_pCreateBase == NULL)//����������ʱû���Ҽ����
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nû���Ҽ�����ͼ�Σ����µ����ESC��ȡ��");
				else
					RemindContent("\r\nNo right point graphic, click or press the ESC key to cancel��");	
				return FALSE;				
			}
			l_dInputLong = atof(cmd);
			//����ĳ��ȷ������ϵĵ㣬���ؼ�˵������ĳ��Ȳ��ڷ�Χ��
			if (!m_pCreateBase->RtnLongPos(m_posRight,l_dInputLong,l_cPos,l_iInvali)) 
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n������Χ�����µ����ESC��ȡ��");
				else
					RemindContent("\r\nBeyond the range, click or press the ESC key to cancel��");	
				return FALSE;
			}
			else
			{
				m_pCreateBase = m_pCreateBase1;
				m_iNumOfClick++;	
			}
		}
		else//�������
		{
			if ( RtnPickLine(l_pot,1))
			{
				m_pCreateBase = m_pCreateBase1;
				m_iNumOfClick++;
			}
			else
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nû�е���ͼ�Σ����µ����ESC��ȡ��");				
				else
					RemindContent("\r\nThere is no point graphics, click or press the ESC key to cancel��");	
				return FALSE;
			}
		}
	}

	switch(m_iNumOfClick)//���������
	{
	case 1:
		m_Begin = l_cPos;
		if (m_pCreateBase != NULL)//���û��˫��ѡ�е�ͼ�Σ��Ͳ�
		{
			//ֱ��//////////////////////////////////////////
			if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				if(BreakLine(l_cPos))//��ֱ�ߴ��������ֱ��
					SuccessBreak();
				else
					m_iNumOfClick = 0;
			}

			//���߶�//////////////////////////////////////////
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{
				CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
				//������ǱպϵĻ�
				if (!PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))
				{
					if(BreakPolyline(l_cPos))//��ֱ�ߴ��������ֱ��
						SuccessBreak();
					else
						m_iNumOfClick = 0;
				}
				else
				{
					InvalLastLine();
					//���Ļ���Ӣ��
					if (g_pDoc->m_stcWorkFor.IsChinese)
						RemindContent("\r\nѡ�е�һ�㣬��ѡ�ڶ���");
					else
						RemindContent("\r\nThe first point selected, then choose the second point��");	
				}
			}
			
			//����//////////////////////////////////////////
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{
				InvalLastLine();
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nѡ�е�һ�㣬��ѡ�ڶ���");
				else
					RemindContent("\r\nThe first point selected, then choose the second point��");	
			}
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{//Բ
				InvalLastLine();
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nѡ�е�һ�㣬��ѡ�ڶ���");
				else
					RemindContent("\r\nThe first point selected, then choose the second point��");	
			}
			
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
			{//Բ��
				if(BreakArc(m_Begin))
					SuccessBreak();
				else
					m_iNumOfClick = 0;				
			}
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{//��Բ
				InvalLastLine();
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\nѡ�е�һ�㣬��ѡ�ڶ���");
				else
					RemindContent("\r\nThe first point selected, then choose the second point��");	
			}
		}
		break;
	case 2:			
		//����������µ�
		m_End = l_cPos;
		//��������������̫������
		if (m_Begin.Distance(m_End)<2)
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n������һ���غ�!�����µ����");
			else
				RemindContent("\r\nClick with the first point! Please click again��");	
			m_iNumOfClick = 1;
			return 0;
		}
		if (m_pCreateBase != NULL)
		{
			if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{//���߶�
				if(BreakPolyline(m_Begin,m_End))
				{
					SuccessBreak();
					break;
				}		
			}
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{//����
				
				if(BreakRect(m_Begin,m_End))
				{
					SuccessBreak();
					break;
				}
			}
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{//Բ
				if(BreakRound(m_Begin,m_End))
				{
					SuccessBreak();
					break;
				}
			}
			else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{//��Բ
				if(BreakEllipse(m_Begin,m_End))
				{
					SuccessBreak();
					break;
				}
			}
		}
		break;		
	}
	return 0;
}

int CDrawBreakTwo::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC *pDC = g_pView->GetDC();
	Position	prePos(m_End), curPos(pos);
	//����׽
	FindPointAll(pDC,m_posRight,pos,(g_pView->m_iSwitch&NEAR_POINT)|(g_pView->m_iSwitch&END_POINT)|(g_pView->m_iSwitch&MID_POINT));	
	g_pView->ReleaseDC(pDC);
	return 0;	
}


int CDrawBreakTwo::OnRButtonDown(UINT nFlags,const Position &pos)
{
	g_refresh = 0;	
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);

	//�Ƿ�ѡ��ͼ��
	BOOL l_bIsPick = FALSE;
	//��Ļ���ĵ�
	CPoint point;
	g_pView->WorldToScreem(pos,point);
	POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//���ͷָ�룬������ѭ�����á�
	while(POS != NULL)//����ɨ������
	{
		m_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);
		// ����������ͼԪ��ѡ��
		if( m_pCreateBase->IsPick(point) != -1)
		{
			l_bIsPick = TRUE;
			break;
		}
	}
	//���û��ѡ���˳�
	if (l_bIsPick == FALSE)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n�Ҽ�û�е���ͼ��");						
		else
			RemindContent("\r\nRight without some graphics��");	
		return FALSE;
	}

	//����ڶ���ʱ
	if (m_iNumOfClick == 1)
	{
		m_pCreateBase2 = m_pCreateBase;
		if(m_pCreateBase2 == m_pCreateBase1)
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n�������˵�ĳ��Ȼ�ESC��ȡ��");						
			else
				RemindContent("\r\nEnter the length of the distance from this point or press the ESC key to cancel��");	
		}
		else
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû�е���ͼ�λ���еĲ���һ��ͼ��!�������Ҽ������");		
			else
				RemindContent("\r\nThere is no point graphics or not a graphic in your site! Can right click again��");	
		}
	}
	// �������ͼԪ
	if(m_iNumOfClick == 0)	
	{
		m_pCreateBase1 = m_pCreateBase;
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n�������˵�ĳ��Ȼ�ESC��ȡ��");						
		else
			RemindContent("\r\nEnter the length of the distance from this point or press the ESC key to cancel��");	
	}
	m_posRight = l_cPos;
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	
	return 0;
}

/*
 *	 ��ֱ�ߴ��������
 */
BOOL CDrawBreakTwo::BreakLine(Position l_cPos)
{
	CCreateLine *line = (CCreateLine*)m_pCreateBase;
	//�������´�����ֱ�ߵ��ӳ�����
	if(!PosBelongBox(line->m_Begin,line->m_End,l_cPos))
		return FALSE;
	//����ֱ���ϴ���ĵ�
	Position l_cBreakPos = l_cPos.DistancePosFoot(line->m_Begin,line->m_End);
	//����㵽�ߵľ���̫Զ������Ϊ�㵽��
	if(l_cBreakPos.Distance(l_cPos)/g_pView->ReturnProportion()>=PRECISION)
		return FALSE;
	//���������뿪ʼ�������Ϊ��һ����,����
	if(l_cBreakPos.Distance(line->m_Begin)/g_pView->ReturnProportion()<=PRECISION)
	{		
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n�㵽�����Ч!�����µ����",2);		
		else
			RemindContent("\r\nPoint to the starting point is invalid! Please click again��");	
		return FALSE;
	}
	//��������������������Ϊ��һ����
	else if(l_cBreakPos.Distance(line->m_End)/g_pView->ReturnProportion()<=PRECISION)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n�㵽ĩ����Ч!�����µ����",2);						
		else
			RemindContent("\r\nAt the end of point to point is invalid! Please click again��");	
		return FALSE;
	}

	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//���������µ�ֱ��
	CCreateLine* l_pNewLine1 = new CCreateLine(line->m_Begin,l_cBreakPos);
	l_pNewLine1->m_ColorOfLine = line->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewLine1->m_strCBM.Copy(line->m_strCBM);	
	l_pNewLine1->m_strSBM.Copy(line->m_strSBM);	
	CCreateLine* l_pNewLine2 = new CCreateLine(l_cBreakPos,line->m_End);
	l_pNewLine2->m_ColorOfLine = line->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewLine2->m_strCBM.Copy(line->m_strCBM);	
	l_pNewLine2->m_strSBM.Copy(line->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewLine1);//Ȼ����������һ��
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewLine2);//Ȼ����������һ��
	//�Ѹý���Ƴ�
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewLine2);
	AddUndoList((LONG)l_pNewLine1,(LONG)l_pNewLine2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();

	return TRUE;
}

/*
 *	��һ�����ϲ��պ϶��߶�
 */
BOOL CDrawBreakTwo::BreakPolyline(Position l_cPos,BOOL FlagMove/*FlagMove == Falsh*/)
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
	//�������ĵ�ĸ���
	int l_iCountOfPoint = Polyline->m_point.GetSize();
	
	Position l_cPolyBgPos,l_cPolyEnPos;
	l_cPolyBgPos = Polyline->m_point.GetAt(0) ;
	l_cPolyEnPos = Polyline->m_point.GetAt( l_iCountOfPoint-1 );
	//������ڲ��պϵĶ���ߵ����ˣ�
	if(l_cPos==l_cPolyBgPos||l_cPos==l_cPolyEnPos)
	{		
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n�㵽����ĩ����Ч!�����µ����",2);						
		else
			RemindContent("\r\nPoints to the beginning or the end of the invalid! Please click again��");			
		return FALSE;		
	}	

	//�������´�����ֱ�ߵ��ӳ�����
	Position l_cBegin,l_cEnd,l_cBreakPos;
	BOOL l_bFlagBelongBox = FALSE;
	double l_dTudu = 0.0;
	//���������µĵ㼯���Ѷ���߷ֿ�
	CArray<Position,Position&> l_point1,l_point2,l_point;
	CArray<double,double&> l_tudu1,l_tudu2,l_tudu;
	l_tudu.Copy(Polyline->m_dTuDu);
	l_point.Copy(Polyline->m_point);
	for(int j=0;j<l_iCountOfPoint-1;j++)
	{
		l_cBegin = Polyline->m_point.GetAt(j);
		l_cEnd = Polyline->m_point.GetAt(j+1);
		l_dTudu = Polyline->m_dTuDu.GetAt(j);
		//�����ֱ�߲����ڰ�Χ����
		if(l_dTudu == 0.0&&PosBelongBox(l_cBegin,l_cEnd,l_cPos))//�����ֱ��
		{
			if(PutLineThree(l_cBegin,l_cEnd,l_cBreakPos,l_cPos,FALSE))//����㵽ֱ�ߵľ�����Է�����������
			{	//���������㼯������͹�ȼ�
				PutArrayTwo(j,l_point,l_tudu,l_point1,l_point2,l_tudu1,l_tudu2,l_cBreakPos);
				l_bFlagBelongBox = TRUE;
				break;
			}
		}
		else if(l_dTudu != 0.0&&IsClickArc(l_cBegin,l_cEnd,l_cPos,l_cBreakPos,l_dTudu))//�Ƿ���Բ��������ضϵ�
		{
			PutArrayTwo(j,l_point,l_tudu,l_point1,l_point2,l_tudu1,l_tudu2,l_cBreakPos);
			l_bFlagBelongBox = TRUE;
			break;				
		}
	}
	//û�е��У���㵽ͷ��β���ˣ����������ƶ��У��Ͳ����½����ˣ������ǰѵ����
	if(l_bFlagBelongBox == FALSE||FlagMove==TRUE)
		return FALSE;

	//��������������Ԫ��		
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//���������µ�ֱ��
	CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(l_point1,l_tudu1);
	l_pNewPolyline1->m_ColorOfLine = Polyline->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewPolyline1->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline1->m_strSBM.Copy(Polyline->m_strSBM);	
	CCreatePolyline* l_pNewPolyline2 = new CCreatePolyline(l_point2,l_tudu2);		
	l_pNewPolyline2->m_ColorOfLine = Polyline->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewPolyline2->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline2->m_strSBM.Copy(Polyline->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline1);//Ȼ����������һ��
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline2);//Ȼ����������һ��
	//ɾ��˫��ѡ�еģ���Ϊ���Ѿ������������			//�Ѹý���Ƴ���
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//������������Ӳ�������
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewPolyline2);
	AddUndoList((LONG)l_pNewPolyline1,(LONG)l_pNewPolyline2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();
	
	return TRUE;
}

/*
 *	//��ֱ��ͨ������ֳ�������
 */	
BOOL CDrawBreakTwo::PutLineThree(Position begin,Position end,Position &breakpos,Position clickpos,BOOL FlagClose)
{
	
	//����������յ��غϷ���
	Position l_cPolyBgPos,l_cPolyEnPos;
	if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
		l_cPolyBgPos = Polyline->m_point.GetAt(0) ;
		l_cPolyEnPos = Polyline->m_point.GetAt( Polyline->m_point.GetSize()-1 );
	}
	else if(m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		CCreateRectangle *Polyline = (CCreateRectangle*)m_pCreateBase;
		l_cPolyBgPos = Polyline->m_point.GetAt(0) ;
		l_cPolyEnPos = Polyline->m_point.GetAt( Polyline->m_point.GetSize()-1 );
	}
	else if(m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{
		CCreateEllipse *Polyline = (CCreateEllipse*)m_pCreateBase;
		l_cPolyBgPos = Polyline->m_point.GetAt(0) ;
		l_cPolyEnPos = Polyline->m_point.GetAt( Polyline->m_point.GetSize()-1 );
	}

	//������ڲ��պϵĶ���ߵ����ˣ�
	if((clickpos==l_cPolyBgPos||clickpos==l_cPolyEnPos)&&FlagClose == FALSE)
	{		
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n�㵽����ĩ����Ч!�����µ����",2);						
		else
			RemindContent("\r\nPoints to the beginning or the end of the invalid! Please click again��");			
		return FALSE;		
	}
	//����ֱ���ϴ���ĵ�
	breakpos = clickpos.DistancePosFoot(begin,end);
	//����㵽�ߵľ���̫Զ������Ϊ�㵽������
	if(PosOnLine(begin,end,breakpos))
		return TRUE;
	return FALSE;
}

//���ݶϵ�Űѵ㼯�ֳ�����
BOOL CDrawBreakTwo::PutArrayTwo(int num,CArray<Position,Position&> &l_point,CArray<double,double&> &tudu,
								CArray<Position,Position&> &l_point1,CArray<Position,Position&> &l_point2,
								CArray<double,double&> &tudu1,CArray<double,double&> &tudu2,Position breakpos)
{
	Position l_posBegin(0,0),l_posEnd(0,0);
	double l_dTuDu = 0.0;
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
	//��õ㼯�Ĵ�С
	int l_iSizeOfPoint = l_point.GetSize();
	//�Ѷϵ�ǰ�����㼯һ
	for(int i = 0;i<=num;i++)
	{
		if(i == num)//�����һ��ʱ͹�Ȳ�һ��
		{
			l_posBegin = l_point.GetAt(i);
			l_posEnd = l_point.GetAt(i+1);
			l_point1.Add(l_posBegin);
			l_dTuDu = tudu.GetAt(i);
			if(l_dTuDu == 0.0)	//��ֱ�ߵķֵ㼯��
				tudu1.Add(l_dTuDu);
			else//��Բ���ķֵ㼯��
			{
				Position l_posCenter;
				g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTuDu,l_posCenter);
				double l_dTuDu1 =  g_pDoc->RtnTudu(l_posBegin,breakpos,l_posCenter,RtnArcDir(l_dTuDu));
				tudu1.Add(l_dTuDu1);
			}
			break;
		}
		l_posBegin = l_point.GetAt(i);
		l_point1.Add(l_posBegin);
		l_dTuDu = tudu.GetAt(i);
		tudu1.Add(l_dTuDu);
	}
	//�ϵ����ߵĵ�
	l_posBegin = l_point.GetAt(num);
	l_posEnd = l_point.GetAt(num+1);
	//�����һ����ϵ���ȣ������ӵ���һ���㼯��
	if(PosEqual(l_posBegin,breakpos))
	{
		l_point2.Add(l_posBegin);
		l_dTuDu = tudu.GetAt(num);
		tudu2.Add(l_dTuDu);
	}
	//�����һ����ϵ���ȣ��Ѻ�һ��ӵ�ǰһ�㼯��
	else if(PosEqual(l_posEnd,breakpos))
	{
		l_point1.Add(l_posEnd);
		l_dTuDu = tudu.GetAt(num+1);
		tudu1.Add(l_dTuDu);
	}
	else//����Ѷϵ�ӵ�����
	{
		l_posBegin = l_point.GetAt(num);
		l_posEnd = l_point.GetAt(num+1);
		l_dTuDu = tudu.GetAt(num);
		//�Ѷϵ����
		l_point1.Add(breakpos);				
		l_point2.Add(breakpos);	
		//��͹�ȼ���
		if(l_dTuDu == 0.0)	//��ֱ�ߵķֵ㼯��
			tudu2.Add(l_dTuDu);
		else//��Բ���ķֵ㼯��
		{
			Position l_posCenter;
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTuDu,l_posCenter);
			double l_dTuDu2 =  g_pDoc->RtnTudu(breakpos,l_posEnd,l_posCenter,RtnArcDir(l_dTuDu));
			tudu2.Add(l_dTuDu2);
		}
		l_dTuDu = 0.0;
		tudu1.Add(l_dTuDu);
	}
	//�Ѷϵ�������㼯��
	for(int j = num+1;j<l_iSizeOfPoint;j++)
	{
		l_posBegin = l_point.GetAt(j);
		l_point2.Add(l_posBegin);
		l_dTuDu = tudu.GetAt(j);
		tudu2.Add(l_dTuDu);
	}
	return TRUE;
}

/*
 *	��϶��߶����Բ��
 */
BOOL CDrawBreakTwo::IsClickArc(Position begin,Position end,Position clickpos,Position &breakpos,double tudu)
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
	//�����ĵ�
	Position l_posCenter;
	g_pDoc->polylineToArc(begin,end,tudu,l_posCenter);
	//����һ����Բ��
	CCreateRoundArc *pTempArc = new CCreateRoundArc(begin,l_posCenter,end,RtnArcDir(tudu));
	
	CPoint pt;
	g_pView->WorldToScreem(clickpos,pt);
	if(pTempArc->IsPick(pt) == -1)
	{
		delete pTempArc;
		return FALSE;
	}
	//��������Բ�ĵĽǶ�
	double l_dRadius = pTempArc->m_dRadius;
	double Angle = GetAngleToXAxis(l_posCenter,clickpos);
	//����Բ���ϵĶϵ�
	breakpos.x = l_dRadius*cos(Angle)+pTempArc->m_Center.x;
	breakpos.y = l_dRadius*sin(Angle)+pTempArc->m_Center.y;
/*
	//���������뿪ʼ�������Ϊ��һ����
	if(breakpos.Distance(begin)/g_pView->ReturnProportion()<=PRECISION)
		m_posWillClick = breakpos = begin;
	//��������������������Ϊ��һ����
	else if(breakpos.Distance(end)/g_pView->ReturnProportion()<=PRECISION)
		m_posWillClick = breakpos = end;
*/
	delete pTempArc;
	return TRUE;
}
/*
 *	���������ϱպ϶��߶�
 */
BOOL CDrawBreakTwo::BreakPolyline(Position l_cPos1,Position l_cPos2,BOOL FlagMove /* = FALSE */)
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_pCreateBase;
	CArray<Position,Position&> l_point;
	CArray<double,double&> tudu;
	l_point.Copy(Polyline->m_point);
	tudu.Copy(Polyline->m_dTuDu);
	//���������µĵ㼯���Ѷ���߷ֿ�
	CArray<Position,Position&> l_point1,l_point2;
	CArray<double,double&> l_tudu1,l_tudu2;
	//�ѵ�������������ֳ������㼯
	if(!BreakClose(l_point,tudu,l_point1,l_point2,l_tudu1,l_tudu2,l_cPos1,l_cPos2 ))
		return FALSE;
	//���������ƶ��У��Ͳ����½����ˣ������ǰѵ����
	if (FlagMove == TRUE)
		return FALSE;
	//��������������Ԫ��
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//���������µ�ֱ��
	CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(l_point1,l_tudu1);
	l_pNewPolyline1->m_ColorOfLine = Polyline->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewPolyline1->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline1->m_strSBM.Copy(Polyline->m_strSBM);	
	CCreatePolyline* l_pNewPolyline2 = new CCreatePolyline(l_point2,l_tudu2);		
	l_pNewPolyline2->m_ColorOfLine = Polyline->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewPolyline2->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline2->m_strSBM.Copy(Polyline->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline1);//Ȼ����������һ��
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline2);//Ȼ����������һ��
	//ɾ��˫��ѡ�еģ���Ϊ���Ѿ������������//�Ѹý���Ƴ���			
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//������������Ӳ�������
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewPolyline2);
	AddUndoList((LONG)l_pNewPolyline1,(LONG)l_pNewPolyline2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();	
	return TRUE;
}

/*
 *	�����������Ͼ���
 */
BOOL CDrawBreakTwo::BreakRect(Position l_cPos1,Position l_cPos2,BOOL FlagMove /* = FALSE */)
{
	CCreateRectangle *Polyline = (CCreateRectangle*)m_pCreateBase;

	CArray<Position,Position&> l_point;
	CArray<double,double&> tudu;
	l_point.Copy(Polyline->m_point);
	tudu.Copy(Polyline->m_dTuDu);
	//���������µĵ㼯���Ѷ���߷ֿ�
	CArray<Position,Position&> l_point1,l_point2;
	CArray<double,double&> l_tudu1,l_tudu2;
	//�ѵ�������������ֳ������㼯
	if(!BreakClose(l_point,tudu,l_point1,l_point2,l_tudu1,l_tudu2,l_cPos1,l_cPos2 ))
		return FALSE;
	//���������ƶ��У��Ͳ����½����ˣ������ǰѵ����
	if (FlagMove == TRUE)
		return FALSE;
	//��������������Ԫ��
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//���������µ�ֱ��
	CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(l_point1,l_tudu1);
	l_pNewPolyline1->m_ColorOfLine = Polyline->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewPolyline1->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline1->m_strSBM.Copy(Polyline->m_strSBM);	
	CCreatePolyline* l_pNewPolyline2 = new CCreatePolyline(l_point2,l_tudu2);		
	l_pNewPolyline2->m_ColorOfLine = Polyline->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewPolyline2->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline2->m_strSBM.Copy(Polyline->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline1);//Ȼ����������һ��
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline2);//Ȼ����������һ��
	//ɾ��˫��ѡ�еģ���Ϊ���Ѿ������������//�Ѹý���Ƴ���			
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//������������Ӳ�������
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewPolyline2);
	AddUndoList((LONG)l_pNewPolyline1,(LONG)l_pNewPolyline2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();		
	return TRUE;
}

/*
 *	������������Բ
 */
BOOL CDrawBreakTwo::BreakRound(Position l_cPos1,Position l_cPos2)
{
	CCreateRound *Round = (CCreateRound*)m_pCreateBase;
	//�������㿴�Ƿ�㵽��Բ��
	CPoint pt1,pt2;
	g_pView->WorldToScreem(l_cPos1,pt1);
	g_pView->WorldToScreem(l_cPos2,pt2);
	if(Round->IsPick(pt1)==-1&&Round->IsPick(pt2)==-1)
		return FALSE;
	//��Բ�ϵ������и���Բ�ĵ�
	Position PosInRound1,PosInRound2,PosCenter;
	PosCenter = Round->m_Center;
	//��������Բ�ĵĽǶ�
	double l_dRadius = Round->m_dRadius;
	double Angle1 = GetAngleToXAxis(PosCenter,l_cPos1);
	double Angle2 = GetAngleToXAxis(PosCenter,l_cPos2);
	PosInRound1.x = l_dRadius*cos(Angle1)+Round->m_Center.x;
	PosInRound1.y = l_dRadius*sin(Angle1)+Round->m_Center.y;
	PosInRound2.x = l_dRadius*cos(Angle2)+Round->m_Center.x;
	PosInRound2.y = l_dRadius*sin(Angle2)+Round->m_Center.y;
	//Բ�ķ���
	int l_iDirect = Round->m_iDirect;
	
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//���������µ�ֱ��
	CCreateRoundArc* l_pNewArc1 = new CCreateRoundArc(PosInRound1,PosCenter,PosInRound2,l_iDirect);
	l_pNewArc1->m_ColorOfLine = Round->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewArc1->m_strCBM.Copy(Round->m_strCBM);	
	l_pNewArc1->m_strSBM.Copy(Round->m_strSBM);	
	CCreateRoundArc* l_pNewArc2 = new CCreateRoundArc(PosInRound2,PosCenter,PosInRound1,l_iDirect);	
	l_pNewArc2->m_ColorOfLine = Round->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewArc2->m_strCBM.Copy(Round->m_strCBM);	
	l_pNewArc2->m_strSBM.Copy(Round->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewArc1);//Ȼ����������һ��
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewArc2);//Ȼ����������һ��
	//ɾ��˫��ѡ�еģ���Ϊ���Ѿ������������
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//������������Ӳ�������
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewArc2);
	AddUndoList((LONG)l_pNewArc1,(LONG)l_pNewArc2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();
	return TRUE;
}

/*
 *	������������Բ��
 */
BOOL CDrawBreakTwo::BreakArc(Position l_cPos)
{
	CCreateRoundArc *RoundArc = (CCreateRoundArc*)m_pCreateBase;
	//���¿����Ƿ�㵽��Բ��
	CPoint pt;
	g_pView->WorldToScreem(l_cPos,pt);
	if(-1 == RoundArc->IsPick(pt))
		return FALSE;
	if (l_cPos.Distance(RoundArc->m_Begin)/g_pView->ReturnProportion()<=PRECISION||l_cPos.Distance(RoundArc->m_End)/g_pView->ReturnProportion()<=PRECISION)
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n�㵽����ĩ����Ч!�����µ����",2);						
		else
			RemindContent("\r\nPoints to the beginning or the end of the invalid! Please click again��");			
		return FALSE;
	}
	//��Բ�ϵ������и���Բ�ĵ�
	Position PosInRound,PosCenter;
	PosCenter = RoundArc->m_Center;
	//��������Բ�ĵĽǶ�
	double l_dRadius = RoundArc->m_dRadius;
	double Angle = GetAngleToXAxis(PosCenter,l_cPos);
	PosInRound.x = l_dRadius*cos(Angle)+RoundArc->m_Center.x;
	PosInRound.y = l_dRadius*sin(Angle)+RoundArc->m_Center.y;
	//Բ�ķ���
	int l_iDirect = RoundArc->m_iDirect;
	
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//���������µ�ֱ��
	CCreateRoundArc* l_pNewArc1 = new CCreateRoundArc(RoundArc->m_Begin,PosCenter,PosInRound,l_iDirect);
	l_pNewArc1->m_ColorOfLine = RoundArc->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewArc1->m_strCBM.Copy(RoundArc->m_strCBM);	
	l_pNewArc1->m_strSBM.Copy(RoundArc->m_strSBM);	
	CCreateRoundArc* l_pNewArc2 = new CCreateRoundArc(PosInRound,PosCenter,RoundArc->m_End,l_iDirect);
	l_pNewArc2->m_ColorOfLine = RoundArc->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewArc2->m_strCBM.Copy(RoundArc->m_strCBM);	
	l_pNewArc2->m_strSBM.Copy(RoundArc->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewArc1);//Ȼ����������һ��
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewArc2);//Ȼ����������һ��
	//ɾ��˫��ѡ�еģ���Ϊ���Ѿ������������//�Ѹý���Ƴ���			
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//������������Ӳ�������
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewArc2);
	AddUndoList((LONG)l_pNewArc1,(LONG)l_pNewArc2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();
	return TRUE;
}


/*
 *	��������������Բ
 */
BOOL CDrawBreakTwo::BreakEllipse(Position l_cPos1,Position l_cPos2,BOOL FlagMove /* = FALSE */)
{
	CCreateEllipse *Polyline = (CCreateEllipse*)m_pCreateBase;

	CArray<Position,Position&> l_point;
	CArray<double,double&> tudu;
	l_point.Copy(Polyline->m_point);
	tudu.Copy(Polyline->m_dTuDu);
	//���������µĵ㼯���Ѷ���߷ֿ�
	CArray<Position,Position&> l_point1,l_point2;
	CArray<double,double&> l_tudu1,l_tudu2;
	//�ѵ�������������ֳ������㼯
	if(!BreakClose(l_point,tudu,l_point1,l_point2,l_tudu1,l_tudu2,l_cPos1,l_cPos2 ))
		return FALSE;
	//���������ƶ��У��Ͳ����½����ˣ������ǰѵ����
	if (FlagMove == TRUE)
		return FALSE;
	//��������������Ԫ��
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.Find(m_pCreateBase);
	//���������µ�ֱ��
	CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(l_point1,l_tudu1);
	l_pNewPolyline1->m_ColorOfLine = Polyline->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewPolyline1->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline1->m_strSBM.Copy(Polyline->m_strSBM);	
	CCreatePolyline* l_pNewPolyline2 = new CCreatePolyline(l_point2,l_tudu2);		
	l_pNewPolyline2->m_ColorOfLine = Polyline->m_ColorOfLine;//����ɫҲ�����ȥ
	l_pNewPolyline2->m_strCBM.Copy(Polyline->m_strCBM);	
	l_pNewPolyline2->m_strSBM.Copy(Polyline->m_strSBM);	
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline1);//Ȼ����������һ��
	g_pDoc->m_CCreateBaseList.InsertBefore(l_ListPos,l_pNewPolyline2);//Ȼ����������һ��
	//ɾ��˫��ѡ�еģ���Ϊ���Ѿ������������//�Ѹý���Ƴ���			
	g_pDoc->m_CCreateBaseList.RemoveAt(l_ListPos);
	//������������Ӳ�������
	l_ListPos = g_pDoc->m_CCreateBaseList.Find(l_pNewPolyline2);
	AddUndoList((LONG)l_pNewPolyline1,(LONG)l_pNewPolyline2,(LONG)m_pCreateBase,l_ListPos);
	m_pCreateBase = NULL;
	g_pView->Invalidate();	
	return TRUE;
}

/*
 *	��һ���պϵĵ㼯���պϵĶ��߶Σ�����������ֳ������㼯
 *  �Ȱѱպϵĵ㼯�õ�һ����ϵ㻻���Զϵ�Ϊ���ıպ�ͼ�Σ�
 *	Ȼ���ٴ�ϳ�����
 */
BOOL CDrawBreakTwo::BreakClose(CArray<Position,Position&> &l_point,CArray<double,double&> &tudu,
				CArray<Position,Position&> &l_point1, CArray<Position,Position&> &l_point2,
				CArray<double,double&> &tudu1,CArray<double,double&> &tudu2,Position l_cPos1,Position l_cPos2 )
 {
	//ÿ��ֱ�ߵ������˵�              �������ϵ�         ����
	Position  l_cBegin,l_cEnd,l_cBreakPos1,l_cBreakPos2,l_posCenter,p1,p2;
	double l_dTudu=0.0;
	//������ֱ�㵽�����еĵڼ����߶�//���ϵ��Ƿ������ �ĵ��غ� 
	int  l_iNumPos1=-1,l_iNumPos2=-1;
	//�������ĵ�ĸ���
	int l_iCountOfPoint = l_point.GetSize();
	//�ҳ��ڶ�����ϵ�������
	for(int j=0;j<l_iCountOfPoint-1;j++)
	{
		p1 = l_cBegin = l_point.GetAt(j);
		p2 = l_cEnd = l_point.GetAt(j+1);
		l_dTudu = tudu.GetAt(j);
		//��һ���Ƿ���ÿ��ֱ�ߵİ�Χ����,�㵽ֱ�߾����Ƿ����ֱ�ߵģ��������ضϵ�
		if(l_dTudu == 0.0&&PosBelongBox(l_cBegin,l_cEnd,l_cPos1)&&\
			PutLineThree(l_cBegin,l_cEnd,l_cBreakPos1,l_cPos1,TRUE))
		{
			l_iNumPos1=j;	
			break;
		}
		//Բ���ģ��ɹ��ܷ��������ϵĶϵ�
		else if(l_dTudu != 0.0&&IsClickArc(l_cBegin,l_cEnd,l_cPos1,l_cBreakPos1,l_dTudu))
		{
			l_iNumPos1=j;
			break;
		}
	}
	//û��ʰȡ������
	if (l_iNumPos1<0)
		return FALSE;
//////////////�Ѷ������㻻���ϵ�1////////////////////////////////
	l_dTudu = tudu.GetAt(l_iNumPos1);
	//�����ϵ���һ������ʱ��ֻ�е㵽������ʱ��
	if(PosEqual(l_cBegin,l_cBreakPos1))
	{
		l_point1.Copy(l_point);
		tudu1.Copy(tudu);
/*
		for(int a = l_iNumPos1;;)
		{
			l_cBegin = l_point.GetAt(a);
			l_dTudu = tudu.GetAt(a);
			l_point1.Add(l_cBegin);
			tudu1.Add(l_dTudu);
			a++;
			if(a == l_iNumPos1)
			{
				l_cBegin = l_point.GetAt(a);
				l_dTudu = 0.0;
				l_point1.Add(l_cBegin);
				tudu1.Add(l_dTudu);
				break;
			}
			if (a == l_iCountOfPoint-1)a=0;
		}
*/
	}
	//�����һ����ϵ���ȣ��Ѻ�һ��ӵ�ǰһ�㼯��
	else if(PosEqual(l_cEnd,l_cBreakPos1))
	{
		for(int a = l_iNumPos1+1;;)
		{
			l_cBegin = l_point.GetAt(a);
			l_dTudu = tudu.GetAt(a);
			l_point1.Add(l_cBegin);
			tudu1.Add(l_dTudu);
			a++;
			if(a == l_iNumPos1+1)
			{
				l_cBegin = l_point.GetAt(a);
				l_dTudu = 0.0;
				l_point1.Add(l_cBegin);
				tudu1.Add(l_dTudu);
				break;
			}
			if (a == l_iCountOfPoint-1)a=0;
		}
	}
	else//����Ѷϵ�ӵ�����
	{
		double l_dTudu1=0.0;
		l_cBegin = l_point.GetAt(l_iNumPos1);
		l_cEnd = l_point.GetAt(l_iNumPos1+1);
		l_dTudu = tudu.GetAt(l_iNumPos1);
		//�Ѷϵ����
		l_point1.Add(l_cBreakPos1);				
		//��͹�ȼ���
		if(l_dTudu == 0.0)	//��ֱ�ߵķֵ㼯��
		{
			tudu1.Add(l_dTudu);
		}
		else//��Բ���ķֵ㼯��
		{
			g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_posCenter);
			l_dTudu1 =  g_pDoc->RtnTudu(l_cBreakPos1,l_cEnd,l_posCenter,RtnArcDir(l_dTudu));
			tudu1.Add(l_dTudu1);
		}
		//����������////
		for(int a = l_iNumPos1+1;;a++)
		{
			if (a == l_iCountOfPoint-1)a=0;
			//
			if(a == l_iNumPos1)
			{
				l_cBegin = l_point.GetAt(l_iNumPos1);
				l_cEnd = l_point.GetAt(l_iNumPos1+1);
				l_dTudu = tudu.GetAt(l_iNumPos1);
				l_point1.Add(l_cBegin);
				if(l_dTudu == 0.0)	//��ֱ�ߵķֵ㼯��
					tudu1.Add(l_dTudu);
				else//��Բ���ķֵ㼯��
				{
					g_pDoc->polylineToArc(l_cBegin,l_cEnd,l_dTudu,l_posCenter);
					l_dTudu1 =  g_pDoc->RtnTudu(l_cBegin,l_cBreakPos1,l_posCenter,RtnArcDir(l_dTudu));
					tudu1.Add(l_dTudu1);
				}
				l_dTudu = 0.0;
				l_point1.Add(l_cBreakPos1);
				tudu1.Add(l_dTudu);
				break;
			}	
			l_cBegin = l_point.GetAt(a);
			l_dTudu = tudu.GetAt(a);
			l_point1.Add(l_cBegin);
			tudu1.Add(l_dTudu);			

		}
		//������/////
	}
	///////////////�ֳ������㼯/////////////////////////
	l_point.RemoveAll();
	l_point.Copy(l_point1);
	l_point1.RemoveAll();
	CArray<double,double&> l_tudu;
	l_tudu.Copy(tudu1);
	tudu1.RemoveAll();
	int l_iCountOfPoint1 = l_point.GetSize();
	for(int b=0;b<l_iCountOfPoint1-1;b++)
	{
		l_cBegin = l_point.GetAt(b);
		l_cEnd = l_point.GetAt(b+1);
		l_dTudu = l_tudu.GetAt(b);
		//�����ֱ�߲����ڰ�Χ����
		if(l_dTudu == 0.0&&PosBelongBox(l_cBegin,l_cEnd,l_cPos2))//�����ֱ��
		{
			if(PutLineThree(l_cBegin,l_cEnd,l_cBreakPos2,l_cPos2,FALSE))//����㵽ֱ�ߵľ�����Է�����������
			{	//���������㼯������͹�ȼ�
				PutArrayTwo(b,l_point,l_tudu,l_point1,l_point2,tudu1,tudu2,l_cBreakPos2);
				return TRUE;
			}
		}
		else if(l_dTudu != 0.0&&IsClickArc(l_cBegin,l_cEnd,l_cPos2,l_cBreakPos2,l_dTudu))//�Ƿ���Բ��������ضϵ�
		{
			PutArrayTwo(b,l_point,l_tudu,l_point1,l_point2,tudu1,tudu2,l_cBreakPos2);
			return TRUE;				
		}
	}
	return FALSE;
 }

 /*
  *	��ϳɹ�
  */
void CDrawBreakTwo::SuccessBreak()
{
	//��ϳɹ���Ҫ�ѡ���ϡ��޸����
	m_iNumOfClick = 0;
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n��ϳɹ�:");					
	else
		RemindContent("\r\nInterrupt the success��");			
	
	
}

/*
 *	������������Ӳ���
 */
void CDrawBreakTwo::AddUndoList(LONG CreateBase1,LONG CreateBase2,LONG CreateBase3,POSITION pos)
{
	//������������Ӳ���
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_BREAK_TWO;

	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBaseposSpare1 = CreateBase1;
	pUndoList->pGraphPos->pCreateBaseposSpare2 = CreateBase2;
	pUndoList->pGraphPos->pCreateBasepos = CreateBase3;
	pUndoList->pGraphPos->pListPos = pos;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}

/*
 *	
 */
BOOL CDrawBreakTwo::RtnPickLine(CPoint point,int NumClick)
{
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//���ͷָ�룬������ѭ�����á�
	if (NumClick == 1)//�����һ��ʱ
	{
		while(position != NULL)//����ɨ������
		{
			m_pCreateBase1 = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
			// ����������ͼԪ��ѡ��
			if( m_pCreateBase1->IsPick(point) != -1)
				return TRUE;
			else
				m_pCreateBase1 = NULL;
		}		
	}
	else//����ڶ���ʱ
	{
		while(position != NULL)//����ɨ������
		{
			m_pCreateBase2 = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
			// ����������ͼԪ��ѡ��
			if( m_pCreateBase2->IsPick(point) != -1)
				return TRUE;
			else
				m_pCreateBase2 = NULL;
		}	
	}

	return FALSE;
}
