// DrawExtendLine.cpp: implementation of the CDrawExtendLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "Draw.h"
#include "math.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawExtendLine::CDrawExtendLine()
{
	m_iNumOfClick = 0;
	m_iExtendWhichTop = 0;
	m_dLongExtend = 0.0;
}

CDrawExtendLine::~CDrawExtendLine()
{

}

/*
 *	��ǰ��������ʱ����1����ǰ����ĩ���ʱ����2��������һ����ʱ����0
 */
int CDrawExtendLine::ClosePos(CCreateBase* pCreateBase,Position pos)
{
	double l_dDisBegin,l_dDisEnd;
	//ֱ��//////////////////////////////////////////
	if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine *pLine = (CCreateLine*)pCreateBase;
		l_dDisBegin = pos.Distance(pLine->m_Begin);
		l_dDisEnd = pos.Distance(pLine->m_End);
		if (l_dDisBegin == l_dDisEnd) 
			return 1;
		if (l_dDisBegin < l_dDisEnd) 
			return 1;
		else 
			return 2;
	}	
	//���߶�//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
	{
		CCreatePolyline *pPolyLine = (CCreatePolyline*)pCreateBase;
		int l_iSize = pPolyLine->m_point.GetSize();
		Position posBegin = pPolyLine->m_point.GetAt(0);
		Position posEnd = pPolyLine->m_point.GetAt(l_iSize-1);
		l_dDisBegin = pos.Distance(posBegin);
		l_dDisEnd = pos.Distance(posEnd);
		if (l_dDisBegin == l_dDisEnd) 
			return 0;
		if (l_dDisBegin < l_dDisEnd) 
			return 1;
		else 
			return 2;
	}	
	//����//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
	}
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{//Բ
		return 0;
	}	
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{//Բ��
		return 0;		
	}
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{//��Բ
		return 0;		
	}
	return 0;
}

int CDrawExtendLine::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{	
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	//�����������˳�����û��˫��ͼ��
	if(nFlags == 1)
	{
		//����ӳ�����Ϊ��Ļ��˳�
		if (m_dLongExtend == 0.0 )
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n���Ȳ�����Ϊ:0");		
			else
				RemindContent("\r\nLength is not zero:");		
			return FALSE;			
		}
		CPoint point;
		g_pView->WorldToScreem(l_cPos,point);
		POSITION	POS = g_pDoc->m_CCreateBaseList.GetHeadPosition();	//���ͷָ�룬������ѭ�����á�
		while(POS != NULL)//����ɨ������
		{
			m_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(POS);
			// ����������ͼԪ��ѡ��
			if( m_pCreateBase->IsPick(point) != -1)
			{ 
				//��ǰ��������ʱ����1����ǰ����ĩ���ʱ����2��������һ����ʱ����0
				m_iExtendWhichTop = ClosePos(m_pCreateBase,l_cPos);
				//ִ�а����ӳ�
				ExtendLine(m_pCreateBase,m_dLongExtend,m_iExtendWhichTop);
				return TRUE;
			}
		}		
	}
	if (nFlags == 2)	//���ǿո��س���Ҫ�������������Ϊ��
	{	
 		m_dLongExtend = atof(cmd);
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n����ӳ���ESC�˳��������ӳ�����Ϊ:");		
		else
			RemindContent("\r\nClick the extension or press ESC to exit. Extended length of input:");		
	}
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");				
	return TRUE;
}

int CDrawExtendLine::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));//�ı������״	
	//����׽
	CDC*	pDC = g_pView->GetDC(); 
	FindPointAll(pDC,pos,pos,NEAR_POINT);
	g_pView->ReleaseDC(pDC); 
	return 0;
}


int CDrawExtendLine::OnRButtonDown(UINT nFlags,const Position &pos)
{
	
	return 0;
}

/*
 *	�����ӳ�
 */
void CDrawExtendLine::ExtendLine(CCreateBase* pCreateBase,double LongExtend,int IsBegin)
{
	//ֱ��//////////////////////////////////////////
	if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		PutLineEx(pCreateBase,LongExtend,IsBegin);	
	//���߶�//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
	{
		CCreatePolyline *Polyline = (CCreatePolyline*)pCreateBase;
		//������ǱպϵĻ�
		if (!PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))
			PutPolylineEx(pCreateBase,LongExtend,IsBegin);
		else
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n�պ϶���߲����ӳ�:");		
			else
				RemindContent("\r\nClosure polyline cannot extend:");											
		}
	}
	
	//����//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n���β����ӳ�:");		
		else
			RemindContent("\r\nRectangle cannot extend:");		
	}
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
	{//Բ
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nԲ�����ӳ�:");		
		else
			RemindContent("\r\nRound cannot extend:");		
	}
	
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
	{//Բ��
//		SuccessExtend();
	}
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
	{//��Բ
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n��Բ�����ӳ�:");		
		else
			RemindContent("\r\nCEllipse cannot extend:");		
	}		
}

//��ֱ���ӳ�
void CDrawExtendLine::PutLineEx(CCreateBase* pCreateBase,double LongExtend,int IsBegin)
{
	CCreateLine *Line = (CCreateLine*)pCreateBase;
	m_posEnd = Line->m_End;
	m_posBegin = Line->m_Begin;
	if (LongExtend < 0&&fabs(LongExtend) > m_posEnd.Distance(m_posBegin))
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n������Χ�����µ����ESC��ȡ��");
		else
			RemindContent("\r\nBeyond the range, click or press the ESC key to cancel��");
		return;		
	}
	//�ӳ��ɹ�
	CCreatePolyline *polyLine = (CCreatePolyline*)pCreateBase;
	AddUndoList(polyLine->m_point,polyLine->m_dTuDu);
	Line = (CCreateLine*)pCreateBase;
	m_posEnd = Line->m_End;
	m_posBegin = Line->m_Begin;
	//�����ӳ��ĸ��˵�
	if (IsBegin == 1)//���
	{
		m_dAgl = GetAngleToXAxis(m_posBegin,m_posEnd);
		Line->m_Begin.x = Line->m_Begin.x - LongExtend*cos(m_dAgl);
		Line->m_Begin.y = Line->m_Begin.y - LongExtend*sin(m_dAgl);
	}
	else if(IsBegin == 2)//ĩ��
	{
		m_dAgl = GetAngleToXAxis(m_posEnd,m_posBegin);
		Line->m_End.x = Line->m_End.x - LongExtend*cos(m_dAgl);
		Line->m_End.y = Line->m_End.y - LongExtend*sin(m_dAgl);
	}
	SuccessExtend();				
}

/*
*	�ӳ��ɹ�
*/
void CDrawExtendLine::SuccessExtend()
{
	//�ӳ��ɹ���Ҫ�ѡ��ӳ����޸����
	m_iNumOfClick = 0;
	g_pView->Invalidate();
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
		RemindContent("\r\n�ӳ��ɹ����ɼ�������ӳ���ESC�˳�:");
	else
		RemindContent("\r\nExtend the success. Can continue to click to extend or press ESC to exit:");		
}

//�Ѷ�����ӳ�
void CDrawExtendLine::PutPolylineEx(CCreateBase* pCreateBase,double LongExtend,int IsBegin)
{
 	CCreatePolyline *Line = (CCreatePolyline*)pCreateBase;	
	Position l_posBegin;
	int l_iWhichLine;
	//�����ӳ��ĸ��˵�
	if (IsBegin == 1)//���
	{
		l_posBegin = Line->m_point.GetAt(0);
		if (LongExtend < 0)//����
		{
			//����ĳ��ȷ������ϵĵ㣬���ؼ�˵������ĳ��Ȳ��ڷ�Χ��
			if (!Line->RtnLongPos(l_posBegin,-LongExtend,m_posBegin,l_iWhichLine)) 
			{
					//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n������Χ�����µ����ESC��ȡ��");
				else
					RemindContent("\r\nBeyond the range, click or press the ESC key to cancel��");
				return;
			}	
			else
			{
				AddUndoList(Line->m_point,Line->m_dTuDu);
				Line->m_point.RemoveAt(0,l_iWhichLine);
				Line->m_point.SetAt(0,m_posBegin);
				//�ӳ��ɹ�
				SuccessExtend();				
			}
		}
		else//����
		{
			AddUndoList(Line->m_point,Line->m_dTuDu);
			m_posEnd = Line->m_point.GetAt(1);
			m_posBegin = Line->m_point.GetAt(0);
			m_dAgl = GetAngleToXAxis(m_posBegin,m_posEnd);
			m_posBegin.x = m_posBegin.x - LongExtend*cos(m_dAgl);
			m_posBegin.y = m_posBegin.y - LongExtend*sin(m_dAgl);
			Line->m_point.SetAt(0,m_posBegin);
			//�ӳ��ɹ�
			SuccessExtend();				
		}
	}
	else if(IsBegin == 2)//ĩ��
	{
		l_posBegin = Line->m_point.GetAt(Line->m_point.GetSize()-1);
		if (LongExtend < 0)//����
		{
			//����ĳ��ȷ������ϵĵ㣬���ؼ�˵������ĳ��Ȳ��ڷ�Χ��
			if (!Line->RtnLongPos(l_posBegin,LongExtend,m_posEnd,l_iWhichLine)) 
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n������Χ�����µ����ESC��ȡ��");
				else
					RemindContent("\r\nBeyond the range, click or press the ESC key to cancel��");
				return;
			}	
			else
			{
				AddUndoList(Line->m_point,Line->m_dTuDu);
				Line->m_point.RemoveAt(l_iWhichLine+1,Line->m_point.GetSize()-2-l_iWhichLine);
				Line->m_point.SetAt(Line->m_point.GetSize()-1,m_posEnd);
				//�ӳ��ɹ�
				SuccessExtend();				
			}
		}
		else//����
		{
			AddUndoList(Line->m_point,Line->m_dTuDu);
			m_posEnd = Line->m_point.GetAt(Line->m_point.GetSize()-1);
			m_posBegin = Line->m_point.GetAt(Line->m_point.GetSize()-2);
			m_dAgl = GetAngleToXAxis(m_posEnd,m_posBegin);
			m_posEnd.x = m_posEnd.x - LongExtend*cos(m_dAgl);
			m_posEnd.y = m_posEnd.y - LongExtend*sin(m_dAgl);
			Line->m_point.SetAt(Line->m_point.GetSize()-1,m_posEnd);
			//�ӳ��ɹ�
			SuccessExtend();				
		}
	}
}

/*
 *	������������Ӳ���
 */
void CDrawExtendLine::AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu)
{
	m_pUndoList = m_UndoRedo.NewUndoList();
	m_pUndoList->OperatieWhat = ID_IO_KNIFE;
	
	m_pCurGraphPos = m_UndoRedo.NewGraphPos();
	m_pCurGraphPos->pCreateBasepos = (LONG)m_pCreateBase;
	//�����ֱ�ߵĻ�
	if(m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		CCreateLine* pLine = (CCreateLine*) m_pCreateBase;
		m_pUndoList->pOperatieParam = m_UndoRedo.NewOperateParam();
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potBase,pLine->m_Begin);
		g_pDoc->PosBeEqualed(m_pUndoList->pOperatieParam->potDes,pLine->m_End);		
	}
	else//�����
	{
		m_pCurGraphPos->aryPoint.SetSize(aryPoint.GetSize());
		m_pCurGraphPos->aryPoint.Copy(aryPoint);
		m_pCurGraphPos->aryTudu.SetSize(aryTudu.GetSize());
		m_pCurGraphPos->aryTudu.Copy(aryTudu);
	}	
	m_pUndoList->pGraphPos = m_pCurGraphPos;
	g_pDoc->m_Undo->AddUndoList(m_pUndoList);
}





