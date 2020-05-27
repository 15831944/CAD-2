// DrawParallel.cpp: implementation of the CDrawParallel class.
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

CDrawParallel::CDrawParallel()
:m_Begin(0,0),m_End(0,0)
{
	m_lCreateBasePick = NULL;
	m_dLongLine = 0.0;

}

CDrawParallel::~CDrawParallel()
{

}

int CDrawParallel::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{
	Position l_cPos(pos);
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);
	//����ǵ�һ�����룬����û��ѡ��ͼ��
	if (m_lCreateBasePick == NULL &&m_iNumOfClick == 0)
	{
 		if(!g_pDoc->ClickCreateBase(l_cPos,m_lCreateBasePick,m_iPick))
		{
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\nû�е㵽ֱ����:",SEND_RICHEDIT);
			else
				RemindContent("\r\nThere is no point to the line:",SEND_RICHEDIT);					
			return FALSE;
		}
	}		
	if (nFlags == 2&&m_iNumOfClick == 1)	//���ǿո��س���Ҫ�������������Ϊ��
	{
		m_dLongLine= atof(cmd);
		if(m_dLongLine < DOUBLE_EQUAL)
			return InvalidContent();//��������ʾ���������Ч�����
	}
	CDC *pDC = g_pView->GetDC();
	g_refresh = 0;
	//������Ĵ���
	m_iNumOfClick++;
	switch(m_iNumOfClick)
	{
	case 1:
		m_Begin  = l_cPos;
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\n����ƽ����֮��ļ��:");
		else
			RemindContent("\r\nSpacing between input parallel lines:");					
		break;
	case 2:
		//����MOUSEMOVE����󻭵���Щ
		CCreateBase* l_pEraseLine1 = new CCreateLine(m_Begin,m_End);
		l_pEraseLine1->DrawMove(pDC,dmDrag);
		delete l_pEraseLine1;	
		//�������ͼ�ε�����		
		double l_dlong = m_dLongLine;
		GetVecter(m_End);
		//���ߵĽǶ�
		if(nFlags == 2)
			m_dLongLine = l_dlong;
		AddParallel();//��ƽ�к��ͼ��

		g_pDoc->m_CCreateBaseList.AddTail(m_pDragCreatebase);

		m_iNumOfClick = 0;
		m_dLongLine = 0.0;
		m_lCreateBasePick = NULL;
		g_pView->Invalidate();
		//��������
		CUndoRedo l_UndoRedo;//������Ķ��� 
		PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
		pUndoList->OperatieWhat = ID_LINE;
		
		pUndoList->pGraphPos = l_UndoRedo.NewGraphPos();
		pUndoList->pGraphPos->pCreateBasepos = (long)m_pDragCreatebase;
		g_pDoc->m_Undo->AddUndoList(pUndoList);
		
		


		break;		
	}
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");	
	g_pView->ReleaseDC(pDC);
	return 0;
}

int CDrawParallel::OnMouseMove(UINT nFlags,const Position &pos)
{
	SetCursor(AfxGetApp()->LoadCursor(IDC_DRAW));//�ı������״	
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	curPos = pos;
	//����׽
	FindPointAll(pDC,m_Begin,curPos,g_pView->m_iSwitch);	
	if (m_iNumOfClick >= 1)
	{	
		// ���������ڵ�ǰһ��λ��
		prePos = m_End; 
		// �õ��豸����ָ��
		
		// ������ʱ���������һ����Ƥ��
		CCreateBase*	pTempLine1 = new CCreateLine(m_Begin, prePos);
		if(g_refresh != 1)	//����Ϊ�˵�һ��û�в���ʱ������
			// ���Ӵ�û�б�ˢ��ʱ���ػ�ԭ������Ƥ��ʹ�䱻����
		{
			pTempLine1->DrawMove(pDC,dmDrag);
			//�������ͼ�ε�����
			GetVecter(prePos);
			//���ߵĽǶ�
			AddParallel();
			m_pDragCreatebase->DrawMove(pDC,dmDrag);
			delete m_pDragCreatebase;
		}
		delete pTempLine1;
		//�趨��׽���ĵ�
		if((g_pView->m_iSwitch&ORTHOGONAL) == ORTHOGONAL)
			SetLButtonDownPot(curPos,m_iNumOfClick);	
		// ������ʱ���󣬸��ݵ�ǰλ�û���һ����Ƥ��
		CCreateBase*	pTempLine2 = new CCreateLine(m_Begin, curPos);	
		pTempLine2->DrawMove(pDC,dmDrag);
		delete pTempLine2;
		//�������ͼ�ε�����
		GetVecter(curPos);
		//���ߵĽǶ�
		AddParallel();
		m_pDragCreatebase->DrawMove(pDC,dmDrag);
		delete m_pDragCreatebase;		

		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_End = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	return 0;
}



int CDrawParallel::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//������ͼԪ//��Ҫ����ͼԪɾ��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;	//�����ѡ���ͼԪ
	return 0;
}

/*
 *	��ͼ�α�ΪҪϳ��ͼ��
 */
void CDrawParallel::AddParallel()
{

	CCreateBase* pCreateBase = (CCreateBase*)m_lCreateBasePick;
	if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		TurnLine();
	//���߶�//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline *Polyline = (CCreatePolyline*)pCreateBase;
		//������ǱպϵĻ� 			
		if (!PosEqual(Polyline->m_point.GetAt(0), Polyline->m_point.GetAt(Polyline->m_point.GetSize()-1)))
			TurnOpenPolyline();
		else
			TurnClosePolyline();
	}	
	//����//////////////////////////////////////////
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		TurnClosePolyline();
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		TurnRound();
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		TurnArc();
	else if (pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		TurnEllipse();	
}

/*
 *	�Ѳ��պ϶���߱��ϳ����Ҫ��ͼ��
 */
void CDrawParallel::TurnOpenPolyline()
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_lCreateBasePick;
	double l_dLongChamfer = m_dLongLine;
	//����ߵĵ㼯�Ĵ�С
	int l_iSize = Polyline->m_point.GetSize()-1; 
	//�����������������������ĸ����    ����      ������ϵ�������
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin;
	//����ߵ�������������X��ĽǶȺ�͹��
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dAngle90 = MATH_PI/2;	
	//�µĵ㼯��͹��
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	if (!m_bIsLeft)
		l_dAngle90 = -l_dAngle90;
	//�����е㴦��
	for(int i = 0;i<l_iSize;i++)
	{
		l_posPoly1 = Polyline->m_point.GetAt(i);
		l_posPoly2 = Polyline->m_point.GetAt(i+1);
		//����ӵ����һ�㣬�˳�ѭ�������ؼӡ�
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAngle90;
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			if (i ==0)//�������߾�һ��ֱ��
			{
				l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAngle90;
				l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
				l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
				l_posBegin = l_pos1;				
				l_aryPoint1.Add(l_pos1);
				l_aryTudu1.Add(l_dTudu);
			}	
			l_aryPoint1.Add(l_pos2);
			l_aryTudu1.Add(l_dTudu);
			break;
		}
		l_posPoly3 = Polyline->m_point.GetAt(i+2);
		//����������ĸ���
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAngle90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAngle90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
		if (i ==0)//�ӵ�һ��
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
		}
		//�����ƽ�У����ؽ���
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = Polyline->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(Polyline->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(Polyline->m_strSBM);	
}

/*
 *	�ѱպϵĶ��߶�ת�� ��ϳ����Ҫ��ͼ��
 */
void CDrawParallel::TurnClosePolyline()
{
	CCreatePolyline *Polyline = (CCreatePolyline*)m_lCreateBasePick;
	double l_dLongChamfer = m_dLongLine;
	//����ߵĵ㼯�Ĵ�С
	int l_iSize = Polyline->m_point.GetSize()-1,i = 0; 
	//�����������������������ĸ����    ����      ������ϵ�������
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin,l_posEnd;
	//����ߵ�������������X��ĽǶȺ�͹��
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dAgl90 = MATH_PI/2;	
	//��������ֱ���¼�����ֱ��������
	//�µĵ㼯��͹��
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	if (m_bIsLeft) 	//��߼�һȦ
	{
		for(i = 0;i<l_iSize;i++)
		{		
			l_posPoly1 = Polyline->m_point.GetAt(i);
			l_posPoly2 = Polyline->m_point.GetAt(i+1);
			//����ӵ����һ�㣬�˳�ѭ�������������һ��
			if (i+1 == l_iSize) 
			{
				l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
				l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
				l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
				l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
				l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
				//���һ��ֱ����ڶ���ֱ�ߵĽ���,���ƽ��
				if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
				{//����ĩ���غ�
					l_aryPoint1.Add(l_pos);
					l_aryTudu1.Add(l_dTudu);
					l_aryPoint1.SetAt(0,l_pos);
				}
				else
				{
					l_aryPoint1.Add(l_posBegin);
					l_aryTudu1.Add(l_dTudu);
				}
				break;
			}
			l_posPoly3 = Polyline->m_point.GetAt(i+2);
			//����������ĸ���
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
			l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
			l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
			l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
			if (i ==0)//�ӵ�һ��
			{
				l_aryPoint1.Add(l_pos1);
				l_aryTudu1.Add(l_dTudu);
				l_posBegin = l_pos1;
				l_posEnd = l_pos2;
			}
			//�����ƽ�У����ؽ���
			if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
			{
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
			}	
		}
		m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
		m_pDragCreatebase->m_ColorOfLine = Polyline->m_ColorOfLine;
		m_pDragCreatebase->m_strCBM.Copy(Polyline->m_strCBM);	
		m_pDragCreatebase->m_strSBM.Copy(Polyline->m_strSBM);	
		l_aryPoint1.RemoveAll();
		l_aryTudu1.RemoveAll();
	}
	else	//�ұ߼�һȦ
	{
	l_dAgl90 = -l_dAgl90;
	//������һȦ
	for(i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = Polyline->m_point.GetAt(i);
		l_posPoly2 = Polyline->m_point.GetAt(i+1);
		//����ӵ����һ�㣬�˳�ѭ�������������һ��
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//���һ��ֱ����ڶ���ֱ�ߵĽ���,���ƽ��
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//����ĩ���غ�
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
				l_aryPoint1.SetAt(0,l_pos);
			}
			else
			{
				l_aryPoint1.Add(l_posBegin);
				l_aryTudu1.Add(l_dTudu);
			}
			break;
		}
		l_posPoly3 = Polyline->m_point.GetAt(i+2);
		//����������ĸ���
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
		if (i ==0)//�ӵ�һ��
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//�����ƽ�У����ؽ���
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = Polyline->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(Polyline->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(Polyline->m_strSBM);	
	l_aryPoint1.RemoveAll();
	l_aryTudu1.RemoveAll();	
	}
}

/*
 *	��ֱ�߱�ϳ��ͼ��
 */
void CDrawParallel::TurnLine()
{
	CCreateLine *pLine = (CCreateLine*)m_lCreateBasePick;
	double l_dLongChamfer = m_dLongLine;
	//ֱ����������ĸ����                   ���	   ĩ��
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_posPoly1,l_posPoly2;
	//����ߵ�������������X��ĽǶ�
	double l_dAngle1,l_dAngle2,l_dTudu = 0.0,l_dTudu180 = TUDU_180;	
	//�µĵ㼯��͹��
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	l_posPoly1 = pLine->m_Begin;
	l_posPoly2 = pLine->m_End;
	//����������ĸ���
	l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+MATH_PI/2;
	l_dAngle2 = l_dAngle1-MATH_PI;
	l_pos1.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
	l_pos1.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
	l_pos2.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle1),8);
	l_pos2.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle1),8);
	l_pos3.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
	l_pos3.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
	l_pos4.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAngle2),8);
	l_pos4.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAngle2),8);
	if (m_bIsLeft)
	{
		l_aryPoint1.Add(l_pos1);
		l_aryPoint1.Add(l_pos2);
		l_aryTudu1.Add(l_dTudu);
		l_aryTudu1.Add(l_dTudu180);
	}
	else
	{
		l_aryPoint1.Add(l_pos4);
		l_aryPoint1.Add(l_pos3);	
		l_aryTudu1.Add(l_dTudu);
		l_aryTudu1.Add(l_dTudu180);
	}
	m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = pLine->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(pLine->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(pLine->m_strSBM);	
}

/*
 *	��Բ����ϳ����
 */
void CDrawParallel::TurnArc()
{
	CCreateRoundArc *pArc = (CCreateRoundArc*)m_lCreateBasePick;
	//ϳ�����
	double l_dLongChamfer = m_dLongLine,l_dTudu180 = -TUDU_180;
	//ֱ����������ĸ����                   ���	   ĩ��
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_posPoly1,l_posPoly2;
	l_posPoly1 = pArc->m_Begin;
	l_posPoly2 = pArc->m_End;
	//�µĵ㼯��͹��
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	//Բ�����ĽǶ�
	double l_dAglBG = GetAngleToXAxis(pArc->m_Center,pArc->m_Begin);
	//Բ�����ĽǶ�
	double l_dAglEN = GetAngleToXAxis(pArc->m_Center,pArc->m_End);
	l_pos1.x = l_posPoly1.x - l_dLongChamfer*RetainValid(cos(l_dAglBG),8);
	l_pos1.y = l_posPoly1.y - l_dLongChamfer*RetainValid(sin(l_dAglBG),8);
	l_pos2.x = l_posPoly2.x - l_dLongChamfer*RetainValid(cos(l_dAglEN),8);
	l_pos2.y = l_posPoly2.y - l_dLongChamfer*RetainValid(sin(l_dAglEN),8);
	l_pos3.x = l_posPoly2.x + l_dLongChamfer*RetainValid(cos(l_dAglEN),8);
	l_pos3.y = l_posPoly2.y + l_dLongChamfer*RetainValid(sin(l_dAglEN),8);
	l_pos4.x = l_posPoly1.x + l_dLongChamfer*RetainValid(cos(l_dAglBG),8);
	l_pos4.y = l_posPoly1.y + l_dLongChamfer*RetainValid(sin(l_dAglBG),8);
	//͹��
	double l_dTudu = g_pDoc->RtnTudu(l_posPoly1,l_posPoly2,pArc->m_Center,pArc->m_iDirect),l_dzore = 0.0;
	double l_dfuTudu = -l_dTudu;
	if ((!m_bIsLeft&&pArc->m_iDirect == AD_CLOCKWISE)||(m_bIsLeft&&pArc->m_iDirect == AD_COUNTERCLOCKWISE))
	{
		l_aryPoint1.Add(l_pos1);	
		l_aryTudu1.Add(l_dTudu);	
		l_aryPoint1.Add(l_pos2);	
		l_aryTudu1.Add(l_dTudu180);	
	}
	else
	{
		l_aryPoint1.Add(l_pos4);	
		l_aryTudu1.Add(l_dTudu);	
		l_aryPoint1.Add(l_pos3);	
		l_aryTudu1.Add(l_dTudu180);		
	}
	
	m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = pArc->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(pArc->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(pArc->m_strSBM);	
}

/*
 *	��Բ��ϳ����
 */
void CDrawParallel::TurnRound()
{
	CCreateRound *pRound = (CCreateRound*)m_lCreateBasePick;
	//ϳ�����
	double l_dLongChamfer = m_dLongLine;
	//Բ����ĩ��ĽǶ�
	double l_dAgl = GetAngleToXAxis(pRound->m_Center,pRound->m_EndPos);
	//��Բ��ĩ��
	Position posOut(pRound->m_Center.x + (l_dLongChamfer+pRound->m_dRadius)*cos(l_dAgl),
					pRound->m_Center.y + (l_dLongChamfer+pRound->m_dRadius)*sin(l_dAgl));
	//��Բ��ĩ��	
	Position posIn(pRound->m_Center.x + (-l_dLongChamfer+pRound->m_dRadius)*cos(l_dAgl),
		pRound->m_Center.y + (-l_dLongChamfer+pRound->m_dRadius)*sin(l_dAgl));
	//��ϳ�����Բ
	if (m_bIsLeft)
	{
		if (pRound->m_iDirect == AD_CLOCKWISE)
			m_pDragCreatebase = new CCreateRound(pRound->m_Center,posOut,pRound->m_iDirect);			
		else
			m_pDragCreatebase = new CCreateRound(pRound->m_Center,posIn,pRound->m_iDirect);
	}
	else
	{
		if (pRound->m_iDirect == AD_CLOCKWISE)
			m_pDragCreatebase = new CCreateRound(pRound->m_Center,posIn,pRound->m_iDirect);			
		else
			m_pDragCreatebase = new CCreateRound(pRound->m_Center,posOut,pRound->m_iDirect);
		
	}
}

/*
 *	����Բ��ϳ����
 */
void CDrawParallel::TurnEllipse()
{
	CCreateEllipse *pEllipse = (CCreateEllipse*)m_lCreateBasePick;
	//ϳ�����
	double l_dLongChamfer = m_dLongLine;
	//����ߵĵ㼯�Ĵ�С
	int l_iSize = pEllipse->m_point.GetSize()-1,i = 0;; 
	//�����������������������ĸ����    ����      ������ϵ�������
	Position l_pos1,l_pos2,l_pos3,l_pos4,l_pos,l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin,l_posEnd;
	//����ߵ�������������X��ĽǶȺ�͹��
	double l_dAngle1,l_dAngle2,l_dTudu =0.0,l_dAgl90 = MATH_PI/2;	
	//��������ֱ���¼�����ֱ��������
	//�µĵ㼯��͹��
	CArray<Position,Position&> l_aryPoint1;
	CArray<double,double&> l_aryTudu1;
	if (m_bIsLeft) 	//��߼�һȦ
	{
	for(i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = pEllipse->m_point.GetAt(i);
		l_posPoly2 = pEllipse->m_point.GetAt(i+1);
		//����ӵ����һ�㣬�˳�ѭ�������������һ��
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//���һ��ֱ����ڶ���ֱ�ߵĽ���,���ƽ��
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//����ĩ���غ�
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
				l_aryPoint1.SetAt(0,l_pos);
			}
			else
			{
				l_aryPoint1.Add(l_posBegin);
				l_aryTudu1.Add(l_dTudu);
			}
			break;
		}
		l_posPoly3 = pEllipse->m_point.GetAt(i+2);
		//����������ĸ���
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
		if (i ==0)//�ӵ�һ��
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//�����ƽ�У����ؽ���
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	m_pDragCreatebase  = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = pEllipse->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(pEllipse->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(pEllipse->m_strSBM);	
	l_aryPoint1.RemoveAll();
	l_aryTudu1.RemoveAll();
	}
	else	//�ұ߼�һȦ
	{
	l_dAgl90 = -l_dAgl90;
	for(i = 0;i<l_iSize;i++)
	{		
		l_posPoly1 = pEllipse->m_point.GetAt(i);
		l_posPoly2 = pEllipse->m_point.GetAt(i+1);
		//����ӵ����һ�㣬�˳�ѭ�������������һ��
		if (i+1 == l_iSize) 
		{
			l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
			l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
			l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
			l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
			//���һ��ֱ����ڶ���ֱ�ߵĽ���,���ƽ��
			if(GetIntersectPoint(l_posBegin,l_posEnd,l_pos1,l_pos2,l_pos))
			{//����ĩ���غ�
				l_aryPoint1.Add(l_pos);
				l_aryTudu1.Add(l_dTudu);
				l_aryPoint1.SetAt(0,l_pos);
			}
			else
			{
				l_aryPoint1.Add(l_posBegin);
				l_aryTudu1.Add(l_dTudu);
			}
			break;
		}
		l_posPoly3 = pEllipse->m_point.GetAt(i+2);
		//����������ĸ���
		l_dAngle1 = GetAngleToXAxis(l_posPoly1,l_posPoly2)+l_dAgl90;
		l_dAngle2 = GetAngleToXAxis(l_posPoly2,l_posPoly3)+l_dAgl90;
		l_pos1.x = l_posPoly1.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos1.y = l_posPoly1.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos2.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle1);
		l_pos2.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle1);
		l_pos4.x = l_posPoly3.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos4.y = l_posPoly3.y + l_dLongChamfer*sin(l_dAngle2);
		l_pos3.x = l_posPoly2.x + l_dLongChamfer*cos(l_dAngle2);
		l_pos3.y = l_posPoly2.y + l_dLongChamfer*sin(l_dAngle2);
		if (i ==0)//�ӵ�һ��
		{
			l_aryPoint1.Add(l_pos1);
			l_aryTudu1.Add(l_dTudu);
			l_posBegin = l_pos1;
			l_posEnd = l_pos2;
		}
		//�����ƽ�У����ؽ���
		if (GetIntersectPoint(l_pos1,l_pos2,l_pos3,l_pos4,l_pos))
		{
			l_aryPoint1.Add(l_pos);
			l_aryTudu1.Add(l_dTudu);
		}	
	}
	m_pDragCreatebase = new CCreatePolyline(l_aryPoint1,l_aryTudu1);
	m_pDragCreatebase->m_ColorOfLine = pEllipse->m_ColorOfLine;
	m_pDragCreatebase->m_strCBM.Copy(pEllipse->m_strCBM);	
	m_pDragCreatebase->m_strSBM.Copy(pEllipse->m_strSBM);	
	l_aryPoint1.RemoveAll();
	l_aryTudu1.RemoveAll();	
	}
}

/*
 *	ȡ�õ���������㣬�������������ж������ͼ�ε�����
 */
void CDrawParallel::GetVecter(Position posClick)
{
	Position pos1,pos2;
	if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateLine))) 
	{
		CCreateLine* pLine = (CCreateLine*)m_lCreateBasePick;
		pos1 = pLine->m_Begin;
		pos2 = pLine->m_End;
		m_dLongLine = posClick.PotDisLine(pos1,pos2);
		if(posClick.AnalysisClockDir(pos1,pos2)==AD_COUNTERCLOCKWISE)//�������������
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
	}
	else if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
		||((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
	{
		CCreatePolyline* pPolyline = (CCreatePolyline*)m_lCreateBasePick;
		pos1 = pPolyline->m_point.GetAt(m_iPick);
		pos2 = pPolyline->m_point.GetAt(m_iPick+1);
		m_dLongLine = posClick.PotDisLine(pos1,pos2);
		if(posClick.AnalysisClockDir(pos1,pos2)==AD_COUNTERCLOCKWISE)//�������������
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
	}
	else if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateRectangle))) 
	{
		CCreateRectangle* pLine = (CCreateRectangle*)m_lCreateBasePick;
		pos1 = pLine->m_point.GetAt(m_iPick);
		pos2 = pLine->m_point.GetAt(m_iPick+1);
		m_dLongLine = posClick.PotDisLine(pos1,pos2);
		if(posClick.AnalysisClockDir(pos1,pos2)==AD_COUNTERCLOCKWISE)//�������������
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
	}
	else if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateRoundArc))) 
	{
		CCreateRoundArc* pRound = (CCreateRoundArc*)m_lCreateBasePick;
		m_dLongLine = fabs(posClick.Distance(pRound->m_Center) - pRound->m_dRadius);		
		if((posClick.Distance(pRound->m_Center)>=pRound->m_dRadius&&pRound->m_iDirect == AD_CLOCKWISE)||
			(posClick.Distance(pRound->m_Center)<=pRound->m_dRadius&&pRound->m_iDirect == AD_COUNTERCLOCKWISE))
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
	}
	else if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateRound))) 
	{
		CCreateRound* pRound = (CCreateRound*)m_lCreateBasePick;
		m_dLongLine = fabs(posClick.Distance(pRound->m_Center) - pRound->m_dRadius);		
		if((posClick.Distance(pRound->m_Center)>=pRound->m_dRadius&&pRound->m_iDirect == AD_CLOCKWISE)||
			(posClick.Distance(pRound->m_Center)<=pRound->m_dRadius&&pRound->m_iDirect == AD_COUNTERCLOCKWISE))
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
		
	}
	else if (((CCreateBase*)m_lCreateBasePick)->IsKindOf(RUNTIME_CLASS(CCreateEllipse))) 
	{
		CCreateEllipse* pLine = (CCreateEllipse*)m_lCreateBasePick;
		pos1 = pLine->m_point.GetAt(m_iPick);
		pos2 = pLine->m_point.GetAt(m_iPick+1);
		m_dLongLine = posClick.PotDisLine(pos1,pos2);
		if(posClick.AnalysisClockDir(pos1,pos2)==AD_COUNTERCLOCKWISE)//�������������
			m_bIsLeft = TRUE;
		else
			m_bIsLeft = FALSE;
	}
}













