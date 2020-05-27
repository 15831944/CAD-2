// DeleteMoreLine.cpp: implementation of the CDeleteMoreLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "DeleteMoreLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDeleteMoreLine::CDeleteMoreLine()
{

}

CDeleteMoreLine::~CDeleteMoreLine()
{

}

/*
 *	������ԭ������ĵ��Ԫ�غ�б��
 */
BOOL CDeleteMoreLine::RtnPosBaseSlope()
{
	POSITION cpyListPos,ListPos = m_cpyBaseList.GetHeadPosition();
	//�����˵���ԭ��ľ���;
	double l_dDisBegin,l_dDisEnd,l_dDisNear = 99999999999;
	//			���
	Position l_posBegin,l_posEnd,l_posOrig(0,0);
	CCreateBase* l_pCreateBase = NULL;
	while (ListPos != NULL)
	{	
		cpyListPos = ListPos;
		m_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;//ֱ��
			l_posBegin = line->m_Begin;
			l_posEnd = line->m_End;
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{
			CCreateRound *Round = (CCreateRound*)l_pCreateBase;//Բ
			l_posBegin = Round->m_EndPos;
			l_posEnd = Round->m_EndPos;
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;//Բ��
			l_posBegin = Arc->m_Begin;
			l_posEnd = Arc->m_End;
		}
		l_dDisBegin = l_posOrig.Distance(l_posBegin);
		l_dDisEnd = l_posOrig.Distance(l_posEnd);
		//ȡ��ԭ�������Ԫ��
		if (l_dDisBegin < l_dDisNear)
		{
			l_dDisNear = l_dDisBegin;
			m_pCreateBase = l_pCreateBase;
		}
		if (l_dDisEnd < l_dDisNear)
		{
			l_dDisNear = l_dDisEnd;
			m_pCreateBase = l_pCreateBase;	
			//�ı��и��
			Changedir(); 
		}	
	}
	//�浽��������
	g_pDoc->m_CCreateBaseList.AddTail(m_pCreateBase);
	//�Ƴ������������
	m_cpyBaseList.RemoveAt(cpyListPos);		
	return TRUE;
}

/*
 *	�ӿں���
 */
void CDeleteMoreLine::DeleteMoreLine()
{
	//����һ������ͼ�ε���������ͼ�ηֳ�ֱ�ߺ�Բ��
	CopyList();
	//��ջ�������
	ClearBaseList();
	POSITION cpyListPos,ListPos = m_cpyBaseList.GetHeadPosition();
//	ListPos = NULL;
	//ȥ�ص�
	while (ListPos)
	{		
		cpyListPos = ListPos;
		m_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
		
		if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			if(CleckLine())
				ListPos = m_cpyBaseList.GetHeadPosition();
		}
		else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{
			if(CleckRound())
				ListPos = m_cpyBaseList.GetHeadPosition();	
		}
		else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
			if(CleckArc())
				ListPos = m_cpyBaseList.GetHeadPosition();
		}
	}
	RtnPosBaseSlope();
	
	
/*
	//�Ƿ����һ������ͼ�ο�ʼ�ı�־
	BOOL l_bIsNext = TRUE;
1
	//������еĵ㼯
	int l_iSizeOfPoint;
	ListPos = m_cpyBaseList.GetHeadPosition();
	POSITION CurListPos;
	cpyListPos = ListPos;
	//ʣ��ͼ�����ѭ��
	while (ListPos != NULL)
	{		
		if(l_bIsNext == TRUE)//����һ��������ͼ�ε���㣬����ĩ�������
		{

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
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
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
				else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline)))
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
	
	
	
	
	*/

	
	g_pDoc->m_CCreateBaseList.AddTail(&m_cpyBaseList);
	g_pView->Invalidate();
}

/*
 *	���ֱ���Ƿ����������غ�
 */
BOOL CDeleteMoreLine::CleckLine()
{
	POSITION ListPos = m_cpyBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase;
	//ʣ��ͼ�����ѭ��
	while (ListPos != NULL)
	{	
		l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
		//�����Լ��Ƚ�
		if (l_pCreateBase != m_pCreateBase)
		{
			if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
			{
				CCreateLine* pLineOri = (CCreateLine*)m_pCreateBase;//ֱ��
				CCreateLine *pLineCur = (CCreateLine*)l_pCreateBase;//ֱ��
				//���ų�ֱ�ߵĵ����ӳ�����
				double angle1 = GetAngleThreePos(pLineOri->m_Begin,pLineCur->m_Begin,pLineCur->m_End);
				double angle2 = GetAngleThreePos(pLineOri->m_Begin,pLineCur->m_End,pLineCur->m_Begin);
				double angle3 = GetAngleThreePos(pLineOri->m_End,pLineCur->m_Begin,pLineCur->m_End);
				double angle4 = GetAngleThreePos(pLineOri->m_End,pLineCur->m_End,pLineCur->m_Begin);
				
				if(angle1 <= MATH_PI/2&&angle2 <= MATH_PI/2&&angle3 <= MATH_PI/2&&angle4 <= MATH_PI/2)
				{				
					//����ֱ�����㵽�ߵľ��뿴�������غ���
					double l_dDis1 = pLineOri->m_Begin.PotDisLine(pLineCur->m_Begin,pLineCur->m_End);
					double l_dDis2 = pLineOri->m_End.PotDisLine(pLineCur->m_Begin,pLineCur->m_End);
					if(l_dDis1<DISTANCE_ZERO&&l_dDis2<DISTANCE_ZERO)
					{
						POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
						m_cpyBaseList.RemoveAt(POS);
						return TRUE;
					}
				}
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
				||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			{
				CCreateLine* pLineOri = (CCreateLine*)m_pCreateBase;//ֱ��
				CCreatePolyline *pPolylineCur = (CCreatePolyline*)l_pCreateBase;//ֱ��
				int l_iSize = pPolylineCur->m_point.GetSize()-1;
				//������ϵ�������
				Position l_posBegin,l_posEnd,l_posMid,l_pos;
				double l_dtudu;
				for(int i = 0;i<l_iSize;i++)
				{
					l_dtudu = pPolylineCur->m_dTuDu.GetAt(i);
					if(l_dtudu == 0.0)//˵����ֱ��
					{
						l_posBegin = pPolylineCur->m_point.GetAt(i);
						l_posEnd = pPolylineCur->m_point.GetAt(i+1);
						l_pos = l_posEnd;
						//��������һ��ֱ����
						while(i<l_iSize-1)
						{
							i++;
							l_posMid = pPolylineCur->m_point.GetAt(i+1);	
							//����һ���Ƿ���ǰ������ӳ�������
							double l_dDis = l_posMid.PotDisLine(l_posBegin,l_pos);
							if (l_dDis>DISTANCE_ZERO)
							{
								i--;
								break;
							}
							else
								l_posEnd = l_posMid;
						}	
						//���ų�ֱ�ߵĵ����ӳ�����
						double angle1 = GetAngleThreePos(pLineOri->m_Begin,l_posBegin,l_posEnd);
						double angle2 = GetAngleThreePos(pLineOri->m_Begin,l_posEnd,l_posBegin);
						double angle3 = GetAngleThreePos(pLineOri->m_End,l_posBegin,l_posEnd);
						double angle4 = GetAngleThreePos(pLineOri->m_End,l_posEnd,l_posBegin);
						if(angle1 <= MATH_PI/2&&angle2 <= MATH_PI/2&&angle3 <= MATH_PI/2&&angle4 <= MATH_PI/2)
						{				
							//����ֱ�����㵽�ߵľ��뿴�������غ���
							double l_dDis1 = pLineOri->m_Begin.PotDisLine(l_posBegin,l_posEnd);
							double l_dDis2 = pLineOri->m_End.PotDisLine(l_posBegin,l_posEnd);
							if(l_dDis1<DISTANCE_ZERO&&l_dDis2<DISTANCE_ZERO)
							{
								POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
								m_cpyBaseList.RemoveAt(POS);
								return TRUE;
							}
						}
					}					
				}
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			{				
				CCreateLine* pLineOri = (CCreateLine*)m_pCreateBase;//ֱ��
				CCreateRectangle *pPolylineCur = (CCreateRectangle*)l_pCreateBase;//ֱ��
				int l_iSize = pPolylineCur->m_point.GetSize()-1;
				//������ϵ�������
				Position l_posBegin,l_posEnd;
				double l_dtudu;
				for(int i = 0;i<l_iSize;i++)
				{
					l_dtudu = pPolylineCur->m_dTuDu.GetAt(i);
					if(l_dtudu == 0.0)//˵����ֱ��
					{
						l_posBegin = pPolylineCur->m_point.GetAt(i);
						l_posEnd = pPolylineCur->m_point.GetAt(i+1);
						//���ų�ֱ�ߵĵ����ӳ�����
						double angle1 = GetAngleThreePos(pLineOri->m_Begin,l_posBegin,l_posEnd);
						double angle2 = GetAngleThreePos(pLineOri->m_Begin,l_posEnd,l_posBegin);
						double angle3 = GetAngleThreePos(pLineOri->m_End,l_posBegin,l_posEnd);
						double angle4 = GetAngleThreePos(pLineOri->m_End,l_posEnd,l_posBegin);
						if(angle1 <= MATH_PI/2&&angle2 <= MATH_PI/2&&angle3 <= MATH_PI/2&&angle4 <= MATH_PI/2)
						{				
							//����ֱ�����㵽�ߵľ��뿴�������غ���
							double l_dDis1 = pLineOri->m_Begin.PotDisLine(l_posBegin,l_posEnd);
							double l_dDis2 = pLineOri->m_End.PotDisLine(l_posBegin,l_posEnd);
							if(l_dDis1<DISTANCE_ZERO&&l_dDis2<DISTANCE_ZERO)
							{
								POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
								m_cpyBaseList.RemoveAt(POS);
								return TRUE;
							}
						}
					}					
				}
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			{
				CCreateLine* pLineOri = (CCreateLine*)m_pCreateBase;//ֱ��
				CCreateEllipse *pPolylineCur = (CCreateEllipse*)l_pCreateBase;//ֱ��
				int l_iSize = pPolylineCur->m_point.GetSize()-1;
				//������ϵ�������
				Position l_posBegin,l_posEnd;
				double l_dtudu;
				for(int i = 0;i<l_iSize;i++)
				{
					l_dtudu = pPolylineCur->m_dTuDu.GetAt(i);
					if(l_dtudu == 0.0)//˵����ֱ��
					{
						l_posBegin = pPolylineCur->m_point.GetAt(i);
						l_posEnd = pPolylineCur->m_point.GetAt(i+1);
						//���ų�ֱ�ߵĵ����ӳ�����
						double angle1 = GetAngleThreePos(pLineOri->m_Begin,l_posBegin,l_posEnd);
						double angle2 = GetAngleThreePos(pLineOri->m_Begin,l_posEnd,l_posBegin);
						double angle3 = GetAngleThreePos(pLineOri->m_End,l_posBegin,l_posEnd);
						double angle4 = GetAngleThreePos(pLineOri->m_End,l_posEnd,l_posBegin);
						if(angle1 <= MATH_PI/2&&angle2 <= MATH_PI/2&&angle3 <= MATH_PI/2&&angle4 <= MATH_PI/2)
						{				
							//����ֱ�����㵽�ߵľ��뿴�������غ���
							double l_dDis1 = pLineOri->m_Begin.PotDisLine(l_posBegin,l_posEnd);
							double l_dDis2 = pLineOri->m_End.PotDisLine(l_posBegin,l_posEnd);
							if(l_dDis1<DISTANCE_ZERO&&l_dDis2<DISTANCE_ZERO)
							{
								POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
								m_cpyBaseList.RemoveAt(POS);
								return TRUE;
							}
						}
					}					
				}
			}
		}
	}
	return FALSE;
}

/*
 *	���Բ�Ƿ����������غ�
 */
BOOL CDeleteMoreLine::CleckRound()
{
	POSITION ListPos = m_cpyBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase;
	//ʣ��ͼ�����ѭ��
	while (ListPos != NULL)
	{	
		l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
		//�����Լ��Ƚ�
		if (l_pCreateBase != m_pCreateBase)
		{
			if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{
				CCreateRound* pLineOri = (CCreateRound*)m_pCreateBase;
				CCreateRound *pLineCur = (CCreateRound*)l_pCreateBase;
				if (pLineOri->m_Center == pLineCur->m_Center&&pLineCur->m_dRadius == pLineOri->m_dRadius)
				{
					POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
					m_cpyBaseList.RemoveAt(POS);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
	
}

/*
 *	���Բ���Ƿ����������غ�
 */
BOOL CDeleteMoreLine::CleckArc()
{
	POSITION ListPos = m_cpyBaseList.GetHeadPosition();
	CCreateBase* l_pCreateBase;
	//ʣ��ͼ�����ѭ��
	while (ListPos != NULL)
	{	
		l_pCreateBase = (CCreateBase *)m_cpyBaseList.GetNext(ListPos);
		//�����Լ��Ƚ�
		if (l_pCreateBase != m_pCreateBase)
		{
			if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
			{
				CCreateRoundArc* pLineOri = (CCreateRoundArc*)m_pCreateBase;
				CCreateRoundArc *pLineCur = (CCreateRoundArc*)l_pCreateBase;
				if (pLineOri->m_Center == pLineCur->m_Center&&fabs(pLineCur->m_dRadius-pLineOri->m_dRadius)<DISTANCE_ZERO)
				{
					BOOL l_bCover = FALSE;//��־�Ƿ��غ�
					//�������˳ʱ��
					if (pLineOri->m_iDirect == AD_CLOCKWISE&&pLineCur->m_iDirect == AD_CLOCKWISE)
					{//����˳ʱ��
						if (pLineOri->m_dAngleStart<=pLineCur->m_dAngleStart&&pLineOri->m_dAngleEnd>=pLineCur->m_dAngleEnd)
							l_bCover = TRUE;							
					}
					else if (pLineOri->m_iDirect == AD_COUNTERCLOCKWISE&&pLineCur->m_iDirect == AD_COUNTERCLOCKWISE)
					{//������ʱ��
						if (pLineOri->m_dAngleStart>=pLineCur->m_dAngleStart&&pLineOri->m_dAngleEnd<=pLineCur->m_dAngleEnd)
							l_bCover = TRUE;							
					}
					else if (pLineOri->m_iDirect == AD_CLOCKWISE&&pLineCur->m_iDirect == AD_COUNTERCLOCKWISE)
					{//ԭͼ��˳ʱ��,�Ա�ͼ��
						if (pLineOri->m_dAngleStart>=pLineCur->m_dAngleStart&&pLineOri->m_dAngleEnd<=pLineCur->m_dAngleEnd)
							l_bCover = TRUE;							
					}
					else if (pLineOri->m_iDirect == AD_COUNTERCLOCKWISE&&pLineCur->m_iDirect == AD_CLOCKWISE)
					{//
						if (pLineOri->m_dAngleStart>=pLineCur->m_dAngleStart&&pLineOri->m_dAngleEnd<=pLineCur->m_dAngleEnd)
							l_bCover = TRUE;							
					}
					if (l_bCover == TRUE)
					{
						POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
						m_cpyBaseList.RemoveAt(POS);
						return TRUE;
					}
				}
			}
			else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
			{
				CCreateRoundArc* pLineOri = (CCreateRoundArc*)m_pCreateBase;
				CCreateRound *pLineCur = (CCreateRound*)l_pCreateBase;
				if (pLineOri->m_Center == pLineCur->m_Center&&pLineCur->m_dRadius == pLineOri->m_dRadius)
				{
					POSITION POS = m_cpyBaseList.Find(m_pCreateBase);
					m_cpyBaseList.RemoveAt(POS);
					return TRUE;
				}
			}
		}
	}
	return FALSE;	
}


/*
 *	�����е�ͼ��������һ�²��ֳ�ֻ��ֱ�ߺ�Բ��
 */
void CDeleteMoreLine::CopyList()
{
//	m_cpyBaseList.AddTail(&(g_pDoc->m_CCreateBaseList));
	POSITION cpyListPos,ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	//������ͼ�ζ����ֱ�ߺ�Բ����
	while (ListPos)
	{		
		cpyListPos = ListPos;
		m_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(ListPos);
		
		if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))||
			m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
			AnlysPolyline();//�ֽ�����,���Ѷ���߷ֳ�ֱ�ߺ�Բ��
		else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
			AnlysRect();//�ֽ����,���Ѿ��ηֳ�ֱ�ߺ�Բ��		
		else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
			AnlysEllipse();//�ֽ���Բ,������Բ�ֳ�ֱ��
		else//����������ľ�ֱ�Ӵ����������
			m_cpyBaseList.AddTail(m_pCreateBase);
	}
}

/*
 *	�ֽ�����
 */
void CDeleteMoreLine::AnlysPolyline()
{
	CCreatePolyline* pPolyline = (CCreatePolyline*)m_pCreateBase;
	//Բ������		�㼯�ĸ���
	int	l_iDir,l_iSize = pPolyline->m_point.GetSize();
	//��㣬�յ㣬Բ��
	Position l_posBegin,l_posEnd,l_posCenter;
	//͹��
	double l_dTudu;
	for(int i = 0;i <= l_iSize-2;i++)
	{
		l_posBegin = pPolyline->m_point.GetAt(i);
		l_posEnd = pPolyline->m_point.GetAt(i+1);
		l_dTudu = pPolyline->m_dTuDu.GetAt(i);
		//˵����ֱ��
		if (l_dTudu == 0.0)
		{	//��ֱ�ߴ��������
			CCreateLine *pLine = new CCreateLine(l_posBegin,l_posEnd);
			pLine->m_ColorOfLine = m_pCreateBase->m_ColorOfLine;
			pLine->m_strCBM.Copy(m_pCreateBase->m_strCBM);	
			pLine->m_strSBM.Copy(m_pCreateBase->m_strSBM);	
			m_cpyBaseList.AddTail(pLine);			
		}
		else//˵����Բ��
		{
			//�����˳ʱ��
			if (l_dTudu<0.0) l_iDir = AD_CLOCKWISE;
			else l_iDir = AD_COUNTERCLOCKWISE;			
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);
			CCreateRoundArc* pArc = new CCreateRoundArc(l_posBegin,l_posEnd,l_posCenter,l_iDir);	
			pArc->m_ColorOfLine = m_pCreateBase->m_ColorOfLine;
			pArc->m_strCBM.Copy(m_pCreateBase->m_strCBM);	
			pArc->m_strSBM.Copy(m_pCreateBase->m_strSBM);	
			m_cpyBaseList.AddTail(pArc);						
		}
	}
}


/*
 *	�ֽ����
 */
void CDeleteMoreLine::AnlysRect()
{
	CCreateRectangle* pPolyline = (CCreateRectangle*)m_pCreateBase;
	//Բ������		�㼯�ĸ���
	int	l_iDir,l_iSize = pPolyline->m_point.GetSize();
	//��㣬�յ㣬Բ��
	Position l_posBegin,l_posEnd,l_posCenter;
	//͹��
	double l_dTudu;
	for(int i = 0;i <= l_iSize-2;i++)
	{
		l_posBegin = pPolyline->m_point.GetAt(i);
		l_posEnd = pPolyline->m_point.GetAt(i+1);
		l_dTudu = pPolyline->m_dTuDu.GetAt(i);
		//˵����ֱ��
		if (l_dTudu == 0.0)
		{	//��ֱ�ߴ��������
			CCreateLine *pLine = new CCreateLine(l_posBegin,l_posEnd);
			pLine->m_ColorOfLine = m_pCreateBase->m_ColorOfLine;
			pLine->m_strCBM.Copy(m_pCreateBase->m_strCBM);	
			pLine->m_strSBM.Copy(m_pCreateBase->m_strSBM);	
			m_cpyBaseList.AddTail(pLine);			
		}
		else//˵����Բ��
		{
			//�����˳ʱ��
			if (l_dTudu<0.0) l_iDir = AD_CLOCKWISE;
			else l_iDir = AD_COUNTERCLOCKWISE;			
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);
			CCreateRoundArc* pArc = new CCreateRoundArc(l_posBegin,l_posEnd,l_posCenter,l_iDir);
			pArc->m_ColorOfLine = m_pCreateBase->m_ColorOfLine;
			pArc->m_strCBM.Copy(m_pCreateBase->m_strCBM);	
			pArc->m_strSBM.Copy(m_pCreateBase->m_strSBM);	
			m_cpyBaseList.AddTail(pArc);						
		}
	}
}


/*
 *	�ֽ���Բ
 */
void CDeleteMoreLine::AnlysEllipse()
{
	CCreateEllipse* pPolyline = (CCreateEllipse*)m_pCreateBase;
	//	�㼯�ĸ���
	int	l_iSize = pPolyline->m_point.GetSize();
	//��㣬�յ�
	Position l_posBegin,l_posEnd;
	for(int i = 0;i <= l_iSize-2;i++)
	{
		l_posBegin = pPolyline->m_point.GetAt(i);
		l_posEnd = pPolyline->m_point.GetAt(i+1);
		//��ֱ�ߴ��������
		CCreateLine *pLine = new CCreateLine(l_posBegin,l_posEnd);
		pLine->m_ColorOfLine = m_pCreateBase->m_ColorOfLine;
		pLine->m_strCBM.Copy(m_pCreateBase->m_strCBM);	
		pLine->m_strSBM.Copy(m_pCreateBase->m_strSBM);	
		m_cpyBaseList.AddTail(pLine);			
	}
}

/*
 *	��ջ�������
 */
void CDeleteMoreLine::ClearBaseList()
{
	//�������ͼԪ	
	g_pDoc->m_CCreateBaseList.RemoveAll();
	//���ѡ��ͼԪ
	g_pDoc->m_CollectOfPicked.RemoveAll();
}


/*
 *	�ı��и��
 */
void CDeleteMoreLine::Changedir() 
{
	// TODO: Add your control notification handler code here
	if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
	{
		//�������յ㻻��λ��
		Position pos = ((CCreateLine*)m_pCreateBase)->m_Begin;
		((CCreateLine*)m_pCreateBase)->m_Begin = ((CCreateLine*)m_pCreateBase)->m_End;
		((CCreateLine*)m_pCreateBase)->m_End = pos;
		//ֱ��
	}
	else if (m_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))//Բ��
	{	//�������յ�����λ�û�һ�£�
		Position pos = ((CCreateRoundArc*)m_pCreateBase)->m_Begin;
		((CCreateRoundArc*)m_pCreateBase)->m_Begin =	\
			((CCreateRoundArc*)m_pCreateBase)->m_End;
		((CCreateRoundArc*)m_pCreateBase)->m_End = pos;
		//�ı�Բ���и��
		if(((CCreateRoundArc*)m_pCreateBase)->m_iDirect == AD_CLOCKWISE)
			((CCreateRoundArc*)m_pCreateBase)->m_iDirect = AD_COUNTERCLOCKWISE;
		else
			((CCreateRoundArc*)m_pCreateBase)->m_iDirect = AD_CLOCKWISE;
		((CCreateRoundArc*)m_pCreateBase)->m_dAngleStart = GetAngle(((CCreateRoundArc*)m_pCreateBase)->m_Center, ((CCreateRoundArc*)m_pCreateBase)->m_Begin);//
		((CCreateRoundArc*)m_pCreateBase)->m_dAngleEnd = GetAngle(((CCreateRoundArc*)m_pCreateBase)->m_Center, ((CCreateRoundArc*)m_pCreateBase)->m_End);
		
	}
}
