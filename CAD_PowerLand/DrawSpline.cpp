// DrawSpline.cpp: implementation of the CDrawSpline class.
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

CDrawSpline::CDrawSpline()
{
	m_refresh = 0;
}


CDrawSpline::~CDrawSpline()
{
}

int CDrawSpline::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	m_refresh = 0;
	m_iNumOfClick++;
	if(m_iNumOfClick == 1)
	{
		m_ControlPoints.Add(l_cPos);
		m_NumControl.Add(m_refresh);
	}
	if(m_iNumOfClick == 2)
	{
		if(m_posLast.Distance(l_cPos)<=1)//���������������ܶ̾ͷ���
			return 0;
		CDC* pDC = g_pView->GetDC();
		m_ControlPoints.RemoveAt(m_ControlPoints.GetSize()-1);
		m_ControlPoints.Add(l_cPos);
		if(m_ControlPoints.GetSize()>1)
		{
			//����һ���������߶���
			Spline spline(m_ControlPoints, m_ControlPoints.GetSize());
			//���ݿ��Ƶ�����һ����������
			spline.Generate();
			//�趨���ߵ���				�õ����Ƶ���
			m_CurvePoints.SetSize(spline.GetCurveCount());
			//�õ����ߵ������㼯
			int PointCount = 0;
			spline.GetCurve(m_CurvePoints, PointCount);
			m_NumControl.Add(PointCount);			
			//��ͼ�����������
			CCreateSpline* l_pTempSpline = new CCreateSpline(m_CurvePoints,m_ControlPoints,m_NumControl);
			l_pTempSpline->DrawMove(pDC,dmNormal);	
			delete l_pTempSpline;
			m_iNumOfClick--;
		}
		g_pView->ReleaseDC(pDC);
		g_pView->Invalidate();
	}
	m_posLast = l_cPos;
	return 0;
}

int CDrawSpline::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	Position l_cPos(pos);
	// �õ��豸����ָ��
	CDC*	pDC = g_pView->GetDC(); 
	if (m_refresh == 1000)
		m_refresh = 100;
	m_refresh++;
	if (m_iNumOfClick == 1)
	{
		// ������ʱ���������һ����Ƥ��
		if(m_refresh != 1&&g_refresh!=0)
		{
			// ������ʱ���������һ����Ƥ��
			m_ControlPoints.RemoveAt(m_ControlPoints.GetSize()-1);
			m_ControlPoints.Add(m_prePos);
			//����һ���������߶���
			Spline spline(m_ControlPoints, m_ControlPoints.GetSize());
			//���ݿ��Ƶ�����һ����������
			spline.Generate();
			//�趨���ߵ���				�õ����Ƶ���
			m_CurvePoints.SetSize(spline.GetCurveCount());
			//�õ����ߵ������㼯
			int PointCount = 0;
			spline.GetCurve(m_CurvePoints, PointCount);
			CCreateSpline* l_pTempSpline = new CCreateSpline(m_CurvePoints,m_ControlPoints,m_NumControl);
			l_pTempSpline->DrawMove(pDC,dmDrag);
			delete l_pTempSpline;			
		}
		if(m_refresh != 1)
			m_ControlPoints.RemoveAt(m_ControlPoints.GetSize()-1);		
		m_ControlPoints.Add(l_cPos);
		//����һ���������߶���
		Spline spline(m_ControlPoints, m_ControlPoints.GetSize());
		//���ݿ��Ƶ�����һ����������
		spline.Generate();
		//�趨���ߵ���				�õ����Ƶ���
		m_CurvePoints.SetSize(spline.GetCurveCount());
		//�õ����ߵ������㼯
		int PointCount = 0;
		spline.GetCurve(m_CurvePoints, PointCount);
		CCreateSpline* l_pTempSpline = new CCreateSpline(m_CurvePoints,m_ControlPoints,m_NumControl);
		l_pTempSpline->DrawMove(pDC,dmDrag);
		delete l_pTempSpline;
		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_prePos = l_cPos; 
		g_refresh = 2;
	}
	//����׽
	FindPointAll(pDC,m_prePos,pos,g_pView->m_iSwitch);
	// �ͷ��豸����ָ��
	g_pView->ReleaseDC(pDC); 
	return 0;
}



int CDrawSpline::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//���ֻ���һ����ͷ���
	if(m_ControlPoints.GetSize() <= 2)
		return 0;
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	if(l_cPos != m_posLast)//����������������һ��ʱ����겻���ƶ���Ҳ����û��������Ƶ㼯��ӵ㣩���Ͳ���ɾ�����һ����
		m_ControlPoints.RemoveAt(m_ControlPoints.GetSize()-1);				
	//����һ���������߶���
	Spline spline(m_ControlPoints, m_ControlPoints.GetSize());
	//���ݿ��Ƶ�����һ����������
	spline.Generate();
	//�趨���ߵ���				�õ����Ƶ���
	m_CurvePoints.SetSize(spline.GetCurveCount());
	//�õ����ߵ������㼯
	int PointCount = 0;
	spline.GetCurve(m_CurvePoints, PointCount);
	//��ͼ�����������
	CCreateSpline* l_pTempSpline = new CCreateSpline(m_CurvePoints,m_ControlPoints,m_NumControl);
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempSpline);
	//�Ѳ���������ӵ�����������
	AddUndoList((LONG)l_pTempSpline);				
	g_pView->Invalidate();
	//������ͼԪ//��Ҫ����ͼԪɾ��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	return 0;
}

int CDrawSpline::IsInsideControlPoint(Position pos)
{
	int count = m_ControlPoints.GetSize();	
	CPoint point;
	g_pView->WorldToScreem(pos,point);	
	CPoint l_potClick;
	Position l_posCurve;
	if(count>0)	
	{
		for(int i=0; i<count; i++)
		{
			l_posCurve = m_ControlPoints.GetAt(i);
			g_pView->WorldToScreem(l_posCurve,l_potClick);
			if(Distance(l_potClick, point)<4)
				return i;
		}
	}
	
	return -1;
	
}

double CDrawSpline::Distance(CPoint p1, CPoint p2)
{
	int dx = abs(p1.x - p2.x);
	int dy = abs(p1.y - p2.y);
	
	return sqrt(dx*dx + dy*dy);
	
}

/*
 *	�����������ӵ���������
 */
void CDrawSpline::AddUndoList(LONG pCreateBase)
{
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = ID_LINE;
	
	pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = pCreateBase;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
}