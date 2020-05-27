// GraphTogether.cpp: implementation of the CGraphTogether class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "GraphTogether.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphTogether::CGraphTogether()
{

}

CGraphTogether::~CGraphTogether()
{
	m_cpyBaseList.RemoveAll();
}

/*
 *	ʹ��λͼ�κϲ�
 */
void CGraphTogether::PutGraphTogether()
{
	//����һ������ͼ�ε�����
	CopyList();
	//��ջ�������
	ClearBaseList();
	//�Ƿ����һ������ͼ�ο�ʼ�ı�־
	BOOL l_bIsNext = TRUE;
	//ÿ��ͼ�ε���ĩ��
	Position l_PosBegin,l_PosEnd;
	//������еĵ㼯
	int l_iSizeOfPoint;
	//�����Ǵ�������//
	POSITION ListPos = m_cpyBaseList.GetHeadPosition();
	POSITION CurListPos,cpyListPos = ListPos;
	//ʣ��ͼ�����ѭ��
	while (ListPos != NULL)
	{		
		if(l_bIsNext == TRUE)//����һ��������ͼ�ε���㣬����ĩ�������
		{
			cpyListPos = ListPos;
			CCreateBase* l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
			if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				CCreateLine *line = (CCreateLine*)l_pCreateBase;//ֱ��
				m_PosBegin = line->m_Begin;
				m_PosEnd = line->m_End;
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{
				CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;//���߶�
				l_iSizeOfPoint = Polyline->m_point.GetSize()-1;
				m_PosBegin = Polyline->m_point.GetAt(0);
				m_PosEnd = Polyline->m_point.GetAt(l_iSizeOfPoint);
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{
				CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;//����
				l_iSizeOfPoint = Rectangle->m_point.GetSize()-1;
				m_PosBegin = Rectangle->m_point.GetAt(0);
				m_PosEnd = Rectangle->m_point.GetAt(l_iSizeOfPoint);
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{
				CCreateRound *Round = (CCreateRound*)l_pCreateBase;//Բ
				m_PosBegin = Round->m_EndPos;
				m_PosEnd = Round->m_EndPos;
			}
			
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
			{
				CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;//Բ��
				m_PosBegin = Arc->m_Begin;
				m_PosEnd = Arc->m_End;
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{
				CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;//��Բ
				m_PosBegin = Ellipse->m_Begin;
				m_PosEnd = Ellipse->m_End;
			}
			//�����浽��������
			g_pDoc->m_CCreateBaseList.AddTail(l_pCreateBase);
			CurListPos = g_pDoc->m_CCreateBaseList.GetTailPosition();
			//�Ƴ������������
			m_cpyBaseList.RemoveAt(cpyListPos);		
			ListPos = m_cpyBaseList.GetHeadPosition();
		}
		//���Ƿ��и�ĩ����ͬ����
		while (ListPos != NULL)
		{
			cpyListPos = ListPos;
			CCreateBase* l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
			
			if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				CCreateLine *line = (CCreateLine*)l_pCreateBase;//ֱ��
				l_PosBegin = line->m_Begin;
				l_PosEnd = line->m_End;
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{
				CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;//���߶�
				l_iSizeOfPoint = Polyline->m_point.GetSize()-1;
				l_PosBegin = Polyline->m_point.GetAt(0);
				l_PosEnd = Polyline->m_point.GetAt(l_iSizeOfPoint);
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{
				CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;//����
				l_iSizeOfPoint = Rectangle->m_point.GetSize()-1;
				l_PosBegin = Rectangle->m_point.GetAt(0);
				l_PosEnd = Rectangle->m_point.GetAt(l_iSizeOfPoint);
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{
				CCreateRound *Round = (CCreateRound*)l_pCreateBase;//Բ
				l_PosBegin = Round->m_EndPos;
				l_PosEnd = Round->m_EndPos;
			}
			
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
			{
				CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;//Բ��
				l_PosBegin = Arc->m_Begin;
				l_PosEnd = Arc->m_End;
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{
				CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;//��Բ
				l_PosBegin = Ellipse->m_Begin;
				l_PosEnd = Ellipse->m_End;
			}
			//�ж��Ƿ���ͬһ����
			if(PosEqual(m_PosEnd,l_PosBegin))//���ĩ����������
			{
				m_PosEnd = l_PosEnd;
				g_pDoc->m_CCreateBaseList.AddTail(l_pCreateBase);
				l_bIsNext = FALSE;
				break;
			}
			else if(PosEqual(m_PosEnd,l_PosEnd))//���ĩ����ĩ�����
			{
				m_ChangeDir = l_pCreateBase;
				Changedir();
				m_PosEnd = l_PosBegin;
				g_pDoc->m_CCreateBaseList.AddTail(l_pCreateBase);
				l_bIsNext = FALSE;
				break;
			}
			l_bIsNext = TRUE;			
		}
		//���һĩ��û����ͬ�ģ����Ƿ��и������ͬ����
		if(l_bIsNext == TRUE)
		{			
			ListPos = m_cpyBaseList.GetHeadPosition();	
			while (ListPos != NULL)
			{
				cpyListPos = ListPos;
				CCreateBase* l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
				
				if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
				{
					CCreateLine *line = (CCreateLine*)l_pCreateBase;//ֱ��
					l_PosBegin = line->m_Begin;
					l_PosEnd = line->m_End;
				}
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
					||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
				{
					CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;//���߶�
					l_iSizeOfPoint = Polyline->m_point.GetSize()-1;
					l_PosBegin = Polyline->m_point.GetAt(0);
					l_PosEnd = Polyline->m_point.GetAt(l_iSizeOfPoint);;
				}
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
				{
					CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;//����
					l_iSizeOfPoint = Rectangle->m_point.GetSize()-1;
					l_PosBegin = Rectangle->m_point.GetAt(0);
					l_PosEnd = Rectangle->m_point.GetAt(l_iSizeOfPoint);
				}
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
				{
					CCreateRound *Round = (CCreateRound*)l_pCreateBase;//Բ
					l_PosBegin = Round->m_EndPos;
					l_PosEnd = Round->m_EndPos;
				}
				
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
				{
					CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;//Բ��
					l_PosBegin = Arc->m_Begin;
					l_PosEnd = Arc->m_End;
				}
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
				{
					CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;//��Բ
					l_PosBegin = Ellipse->m_Begin;
					l_PosEnd = Ellipse->m_End;
				}
				//�ж��Ƿ���ͬһ����
				if(PosEqual(m_PosBegin,l_PosBegin))//��������������
				{
					m_ChangeDir = l_pCreateBase;
					Changedir();
					m_PosBegin = l_PosEnd;
					CurListPos = g_pDoc->m_CCreateBaseList.InsertBefore(CurListPos,l_pCreateBase);
					l_bIsNext = FALSE;
					break;
				}
				else if(PosEqual(m_PosBegin,l_PosEnd))//��������ĩ�����
				{
					m_PosBegin = l_PosBegin;
					CurListPos = g_pDoc->m_CCreateBaseList.InsertBefore(CurListPos,l_pCreateBase);
					l_bIsNext = FALSE;
					break;
				}
				l_bIsNext = TRUE;			
			}
		}
		if (l_bIsNext == FALSE)
			m_cpyBaseList.RemoveAt(cpyListPos);
		ListPos = m_cpyBaseList.GetHeadPosition();			
	}			
}

/*
 *	�����е�ͼ��������һ��
 */
void CGraphTogether::CopyList()
{
	//������������Ӳ���
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_TOGETHER;

	PGRAPHPOS CurGraphPos,GraphPos;
	CurGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos = CurGraphPos;
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	while(position != NULL)
	{
		CCreateBase* l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
		m_cpyBaseList.AddTail(l_pCreateBase);	
		//�������������
		if (CurGraphPos->pCreateBasepos == NULL)
			CurGraphPos->pCreateBasepos = (LONG)l_pCreateBase;
		else
		{
			GraphPos = l_UndoRedo.NewGraphPos();						
			GraphPos->pCreateBasepos = (LONG)l_pCreateBase;
			CurGraphPos->pNext = GraphPos;
			GraphPos->pPrev = CurGraphPos;
			CurGraphPos = GraphPos;
		} 
	}
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}

/*
 *	��ջ�������
 */
void CGraphTogether::ClearBaseList()
{
	//�������ͼԪ	
	g_pDoc->m_CCreateBaseList.RemoveAll();
	//���ѡ��ͼԪ
	g_pDoc->m_CollectOfPicked.RemoveAll();
}

/*
 *	�ı��и��
 */
void CGraphTogether::Changedir() 
{
	// TODO: Add your control notification handler code here
	if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		//�������յ㻻��λ��
		Position pos = ((CCreateLine*)m_ChangeDir)->m_Begin;
		((CCreateLine*)m_ChangeDir)->m_Begin = ((CCreateLine*)m_ChangeDir)->m_End;
		((CCreateLine*)m_ChangeDir)->m_End = pos;
		//ֱ��
	}
	else if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateSpline)))//���߶�
	{	
		Position l_ScreemPos;
		double l_dTude;
		CArray<Position,Position&> l_ListPoint; 
		CArray<double,double&> l_ListTuDu;//���͹�ȵ�
		for(int i=((CCreatePolyline*)m_ChangeDir)->m_point.GetSize();  i>1;  i--)
		{
			l_ScreemPos = ((CCreatePolyline*)m_ChangeDir)->m_point.GetAt(i-1); 
			l_ListPoint.Add(l_ScreemPos);
			l_dTude = -(((CCreatePolyline*)m_ChangeDir)->m_dTuDu.GetAt(i-2)); 
			l_ListTuDu.Add(l_dTude);
		}
		l_ScreemPos = ((CCreatePolyline*)m_ChangeDir)->m_point.GetAt(0); 
		l_ListPoint.Add(l_ScreemPos);
		l_dTude = 0.0;
		l_ListTuDu.Add(l_dTude);
		((CCreatePolyline*)m_ChangeDir)->m_point.Copy(l_ListPoint);
		((CCreatePolyline*)m_ChangeDir)->m_dTuDu.Copy(l_ListTuDu);
	}
	else if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))//����
	{
		Position l_ScreemPos;
		double l_dTude;
		CArray<Position,Position&> l_ListPoint; 
		CArray<double,double&> l_ListTuDu;//���͹�ȵ�
		for(int i=((CCreateRectangle*)m_ChangeDir)->m_point.GetSize();  i>1;  i--)
		{
			l_ScreemPos = ((CCreateRectangle*)m_ChangeDir)->m_point.GetAt(i-1); 
			l_ListPoint.Add(l_ScreemPos);
			l_dTude = -(((CCreateRectangle*)m_ChangeDir)->m_dTuDu.GetAt(i-2)); 
			l_ListTuDu.Add(l_dTude);
		}
		l_ScreemPos = ((CCreateRectangle*)m_ChangeDir)->m_point.GetAt(0); 
		l_ListPoint.Add(l_ScreemPos);
		l_dTude = 0.0;
		l_ListTuDu.Add(l_dTude);
		((CCreateRectangle*)m_ChangeDir)->m_point.Copy(l_ListPoint);
		((CCreateRectangle*)m_ChangeDir)->m_dTuDu.Copy(l_ListTuDu);	}
	else if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateRound)))//Բ
	{	//�ı�Բ���и��
		if(((CCreateRound*)m_ChangeDir)->m_iDirect == AD_CLOCKWISE)
			((CCreateRound*)m_ChangeDir)->m_iDirect = AD_COUNTERCLOCKWISE;
		else
			((CCreateRound*)m_ChangeDir)->m_iDirect = AD_CLOCKWISE;
	}
	
	else if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))//Բ��
	{	//�������յ�����λ�û�һ�£�
		Position pos = ((CCreateRoundArc*)m_ChangeDir)->m_Begin;
		((CCreateRoundArc*)m_ChangeDir)->m_Begin =	\
			((CCreateRoundArc*)m_ChangeDir)->m_End;
		((CCreateRoundArc*)m_ChangeDir)->m_End = pos;
		//�ı�Բ���и��
		if(((CCreateRoundArc*)m_ChangeDir)->m_iDirect == AD_CLOCKWISE)
			((CCreateRoundArc*)m_ChangeDir)->m_iDirect = AD_COUNTERCLOCKWISE;
		else
			((CCreateRoundArc*)m_ChangeDir)->m_iDirect = AD_CLOCKWISE;
		((CCreateRoundArc*)m_ChangeDir)->m_dAngleStart = GetAngle(((CCreateRoundArc*)m_ChangeDir)->m_Center, ((CCreateRoundArc*)m_ChangeDir)->m_Begin);//
		((CCreateRoundArc*)m_ChangeDir)->m_dAngleEnd = GetAngle(((CCreateRoundArc*)m_ChangeDir)->m_Center, ((CCreateRoundArc*)m_ChangeDir)->m_End);
		
	}
	else if (m_ChangeDir->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))//��Բ
	{
		Position l_ScreemPos;
		CArray<Position,Position&> l_ListPoint; 
		for(int i=((CCreateEllipse*)m_ChangeDir)->m_point.GetSize();  i>0;  i--)
		{
			l_ScreemPos = ((CCreateEllipse*)m_ChangeDir)->m_point.GetAt(i-1); 
			l_ListPoint.Add(l_ScreemPos);			
		}
		((CCreateEllipse*)m_ChangeDir)->m_point.Copy(l_ListPoint);
	}
}
