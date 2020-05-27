// DrawChamfer.cpp: implementation of the CDrawChamfer class.
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

CDrawChamfer::CDrawChamfer()
{
	m_iNumOfClick = 0;
	//���ǰ뾶
	m_dRadius = 0;
	m_bConvex = TRUE;//Ĭ����͹Բ
}

CDrawChamfer::~CDrawChamfer()
{

}

int CDrawChamfer::OnLButtonDown(UINT nFlags,const Position &pos,CString cmd)
{	
	Position l_cPos(pos);
	m_inFlags = nFlags;
	//�趨��׽���ĵ�
	SetLButtonDownPot(l_cPos,m_iNumOfClick);	
	//����İ뾶
	if (nFlags == 2)
	{
		if (cmd == "o"||cmd == "O")//͹��
		{
			m_bConvex = TRUE;
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n��������ߣ�����н�:");	
			else
				RemindContent("\r\nClick on the two lines, chamfer its Angle��");	
			return 0;
		}
		else if (cmd == "i"||cmd == "I")//����
		{
			m_bConvex = FALSE;
			//���Ļ���Ӣ��
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n��������ߣ�����н�:");	
			else
				RemindContent("\r\nClick on the two lines, chamfer its Angle��");	
			return 0;
		}
		else
		{
			m_dRadius = atof(cmd);
			if(m_dRadius == 0.0)
				return InvalidContent();
		}
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	}
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
	if(l_bPicked == TRUE||m_iNumOfClick == 2)
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
		}
		else if (m_iNumOfClick == 2)//������ĵڶ���
		{
			m_createbaseSecond = l_pCreateBase;
			//��������������ͼ��
			if (m_createbaseFirst != m_createbaseSecond)
			{
				//���ѡ��
				g_pDoc->RemoveAllCollected();
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n�㵽��������ͼ�Σ���Ч������ѡ���һ���ߣ�");			
				else
					RemindContent("\r\nPoint to the two graphics, invalid! To choose the first line��");	
				m_iNumOfClick = 0;	
				return FALSE;
			}
			else
			{
				//���Ļ���Ӣ��
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n���뵹�ǰ뾶:");
				else
					RemindContent("\r\nEnter the chamfering radius:");
			}
			m_iPick2 = l_iPick;//�����ͼ���ϵ��ĸ��߶�
		}
		else if(m_iNumOfClick ==3)
		{
			m_posClickThree = l_cPos;
			if(ChamferPolyline())
			{
				//�Ѹ���Ĵ����ǵ�ͼ����ӵ���������
				CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(m_AryPoint,m_aryTudu);
				l_pNewPolyline1->m_ColorOfLine = m_createbaseFirst->m_ColorOfLine;//����ɫҲ�����ȥ
				l_pNewPolyline1->m_strCBM.Copy(m_createbaseFirst->m_strCBM);	
				l_pNewPolyline1->m_strSBM.Copy(m_createbaseFirst->m_strSBM);	
				
				POSITION posOld = g_pDoc->m_CCreateBaseList.Find(m_createbaseFirst);
				POSITION posNew = g_pDoc->m_CCreateBaseList.InsertBefore(posOld,l_pNewPolyline1);	
				g_pDoc->RemoveAllCollected();
				g_pDoc->m_CCreateBaseList.RemoveAt(posOld) ;
				//����������Ӳ���
				CUndoRedo l_UndoRedo;//������Ķ��� 
				PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
				pUndoList->OperatieWhat = ID_CHAMFER;
				PGRAPHPOS headGraphPos;
				headGraphPos = l_UndoRedo.NewGraphPos();
				headGraphPos->pCreateBasepos = (LONG)m_createbaseFirst;
				headGraphPos->pListPos = posNew;
				pUndoList->pGraphPos = headGraphPos;
				g_pDoc->m_Undo->AddUndoList(pUndoList);
				m_createbaseFirst =NULL;
				if (g_pDoc->m_stcWorkFor.IsChinese)
					RemindContent("\r\n���ǳɹ���������������ߵ��ǻ�����ѡ�񵹽ǰ�͹�ǣ�");			
				else
					RemindContent("\r\nChamfering success. Continue to chamfer or press the ESC key to exit");			
				m_iNumOfClick = 0;	
			}			
		}
	}
	else
	{
		//���Ļ���Ӣ��
		if (g_pDoc->m_stcWorkFor.IsChinese)
			RemindContent("\r\nû��ѡ�У�����ѡ��",2);
		else
			RemindContent("\r\nNot selected, choose again��");	
		return FALSE;
	}
	g_pView->Invalidate();
	g_pView->ReleaseDC(pDC);
	//�ѽ�����������������
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItem(IDC_EDIT_COMMAND)->SetFocus();
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_COMMAND,"");
	return TRUE;
}

int CDrawChamfer::OnMouseMove(UINT nFlags,const Position &pos)
{
	if (g_refresh == 1000)
		g_refresh = 100;
	g_refresh++;
	CDC*	pDC = g_pView->GetDC(); 
	Position	prePos, curPos;
	curPos = pos;
	//����׽
	FindPointAll(pDC,m_middle,curPos,g_pView->m_iSwitch);	
	if (m_iNumOfClick == 2)
	{	

		// ���������ڵ�ǰһ��λ��
		prePos = m_End; 
		// ������ʱ���������һ����Ƥ��
		m_posClickThree = prePos;
		if(ChamferPolyline())
		{
			CCreatePolyline* l_pNewPolyline1 = new CCreatePolyline(m_AryPoint,m_aryTudu);
		
			if(g_refresh != 1)	//����Ϊ�˵�һ��û�в���ʱ������
				// ���Ӵ�û�б�ˢ��ʱ���ػ�ԭ������Ƥ��ʹ�䱻����
				l_pNewPolyline1->DrawMove(pDC,dmDrag);
			delete l_pNewPolyline1;
		}
		// ������ʱ���󣬸��ݵ�ǰλ�û���һ����Ƥ��
		m_posClickThree = curPos;
		if(ChamferPolyline())
		{
			CCreatePolyline* l_pNewPolyline2 = new CCreatePolyline(m_AryPoint,m_aryTudu);
			l_pNewPolyline2->DrawMove(pDC,dmDrag);
			delete l_pNewPolyline2;
		}
		// �ͷ��豸����ָ��		
		// ����ǰλ������Ϊֱ���յ㣬�Ա���һ������ƶ�ʱ��
		m_End = curPos; 
	}
	g_pView->ReleaseDC(pDC); 
	
	
	SetCursor(AfxGetApp()->LoadCursor(IDC_BREAK_TWO));//�ı������״		
	return 0;
}

int CDrawChamfer::OnRButtonDown(UINT nFlags,const Position &pos)
{
	//������ͼԪ//��Ҫ����ͼԪɾ��
	delete g_pView->m_pDrawBase;
	g_pView->m_pDrawBase = NULL;
	return 0;
}
/*
 *	�����߶�
 */
BOOL CDrawChamfer::ChamferPolyline()
{
	CCreatePolyline* pRect = (CCreatePolyline*)m_createbaseFirst;
	//����һ���µĵ㼯�Ѿ��α�ɶ����
	CArray<Position,Position&> l_point1,l_point2;
	CArray<double,double&> l_tudu1,l_tudu2;
	//������ϵ�������							���       �յ�		Բ��
	Position l_posPoly1,l_posPoly2,l_posPoly3,l_posBegin,l_posEnd,l_posCenter;
	//  ��͹��
	double  l_dTudu = 0.0;
	//������˳ʱ�뻹����ʱ��
	int l_iClock,l_iSize = pRect->m_point.GetSize();
	//����ȵ��һ���ٵ�ڶ���
	if (m_iPick1+1 == m_iPick2)
	{
		//�Ȱ�ǰ������ӽ���
		for(int i=0;i<m_iPick2;i++)
		{
			l_point1.Add(pRect->m_point.GetAt(i));
			l_dTudu = pRect->m_dTuDu.GetAt(i);
			l_tudu1.Add(l_dTudu);
		}
		l_posPoly1 = pRect->m_point.GetAt(i-1);
		l_posPoly2 = pRect->m_point.GetAt(i);
		l_posPoly3 = pRect->m_point.GetAt(i+1);
		//�жϵ�����������������
		double l_dRightOrLeft = (l_posPoly2.x - l_posPoly1.x) * (l_posPoly3.y - l_posPoly1.y) - 
					(l_posPoly3.x - l_posPoly1.x) * (l_posPoly2.y - l_posPoly1.y);
		if(l_dRightOrLeft > 0)//�������������
		{
			l_iClock = AD_COUNTERCLOCKWISE;
		}
		else if(l_dRightOrLeft == 0)//����������
		{
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n����������ƽ��",2);
			else
				RemindContent("\r\nError! Two lines parallel��");			
			
			m_iNumOfClick = 0;
			return FALSE;
		}	
		else//�����������ұ�
		{
			l_iClock = AD_CLOCKWISE;			
		}
		//������㣬�յ㣬Բ
		GetArcPoint(l_posPoly1,l_posPoly2,l_posPoly3,l_iClock,l_posBegin,l_posEnd,l_posCenter);
		//����͹��
		l_dTudu = g_pDoc->RtnTudu(l_posBegin,l_posEnd,l_posCenter,l_iClock);
		if(m_bConvex == FALSE)//����ǰ��ǵĻ�
			l_dTudu = -l_dTudu;
		l_point1.Add(l_posBegin);
		l_tudu1.Add(l_dTudu);
		l_dTudu = 0.0;
		l_point1.Add(l_posEnd);
		l_tudu1.Add(l_dTudu);
		
		for(i=m_iPick2+1;i<l_iSize;i++)
		{
			l_point1.Add(pRect->m_point.GetAt(i));
			l_dTudu = pRect->m_dTuDu.GetAt(i);
			l_tudu1.Add(l_dTudu);
		}
	}
	//�ȵں�����ߣ��ٵ�һ����
	else if (m_iPick2+1 == m_iPick1)
	{
		double l_dTudu = 0.0;
		for(int i=0;i<m_iPick1;i++)
		{
			l_point1.Add(pRect->m_point.GetAt(i));
			l_dTudu = pRect->m_dTuDu.GetAt(i);
			l_tudu1.Add(l_dTudu);
		}
		l_posPoly1 = pRect->m_point.GetAt(i-1);
		l_posPoly2 = pRect->m_point.GetAt(i);
		l_posPoly3 = pRect->m_point.GetAt(i+1);
		//�жϵ�����������������
		double l_dRightOrLeft = (l_posPoly2.x - l_posPoly1.x) * (l_posPoly3.y - l_posPoly1.y) - 
			(l_posPoly3.x - l_posPoly1.x) * (l_posPoly2.y - l_posPoly1.y);
		if(l_dRightOrLeft > 0)//�������������
		{
			l_iClock = AD_COUNTERCLOCKWISE;
		}
		else if(l_dRightOrLeft == 0)//����������
		{
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n����������ƽ��",2);
			else
				RemindContent("\r\nError! Two lines parallel��");			
			m_iNumOfClick = 0;
			return FALSE;
		}	
		else//�����������ұ�
		{
			l_iClock = AD_CLOCKWISE;			
		}
		//������㣬�յ㣬Բ
		GetArcPoint(l_posPoly1,l_posPoly2,l_posPoly3,l_iClock,l_posBegin,l_posEnd,l_posCenter);
		//����͹��
		l_dTudu = g_pDoc->RtnTudu(l_posBegin,l_posEnd,l_posCenter,l_iClock);
		if(m_bConvex == FALSE)//����ǰ��ǵĻ�
			l_dTudu = -l_dTudu;
		l_point1.Add(l_posBegin);
		l_tudu1.Add(l_dTudu);
		l_dTudu = 0.0;
		l_point1.Add(l_posEnd);
		l_tudu1.Add(l_dTudu);
		
		for(i=m_iPick1+1;i<l_iSize;i++)
		{
			l_point1.Add(pRect->m_point.GetAt(i));
			l_dTudu = pRect->m_dTuDu.GetAt(i);
			l_tudu1.Add(l_dTudu);
		}

	}
	else if ((m_iPick2 == l_iSize-2&&m_iPick1 == 0)||(m_iPick1 == l_iSize-2&&m_iPick2 == 0))//���������β��ӵ���
	{
		if(pRect->m_point.GetAt(0) != pRect->m_point.GetAt(l_iSize-1))
		{
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n���������߲����",2);
			else
				RemindContent("\r\nError! Polyline no close��",2);			
			m_iNumOfClick = 0;
			return FALSE;
			
		}
		l_posPoly1 = pRect->m_point.GetAt(l_iSize-2);
		l_posPoly2 = pRect->m_point.GetAt(0);
		l_posPoly3 = pRect->m_point.GetAt(1);
		//�жϵ�����������������
		double l_dRightOrLeft = (l_posPoly2.x - l_posPoly1.x) * (l_posPoly3.y - l_posPoly1.y) - 
			(l_posPoly3.x - l_posPoly1.x) * (l_posPoly2.y - l_posPoly1.y);
		if(l_dRightOrLeft > 0)//�������������
		{
			l_iClock = AD_COUNTERCLOCKWISE;
		}
		else if(l_dRightOrLeft == 0)//����������
		{
			if (g_pDoc->m_stcWorkFor.IsChinese)
				RemindContent("\r\n����������ƽ��",2);
			else
				RemindContent("\r\nError! Two lines parallel��",2);			
			m_iNumOfClick = 0;
			return FALSE;
		}	
		else//�����������ұ�
		{
			l_iClock = AD_CLOCKWISE;			
		}
		//������㣬�յ㣬Բ
		GetArcPoint(l_posPoly1,l_posPoly2,l_posPoly3,l_iClock,l_posBegin,l_posEnd,l_posCenter);
		//����͹��
		l_dTudu = g_pDoc->RtnTudu(l_posBegin,l_posEnd,l_posCenter,l_iClock);
		if(m_bConvex == FALSE)//����ǰ��ǵĻ�
			l_dTudu = -l_dTudu;
		l_point1.Add(l_posBegin);
		l_tudu1.Add(l_dTudu);
		l_dTudu =0.0;
		l_point1.Add(l_posEnd);
		l_tudu1.Add(l_dTudu);	
		for(int i=1;i<l_iSize-1;i++)
		{
			l_point1.Add(pRect->m_point.GetAt(i));
			l_dTudu = pRect->m_dTuDu.GetAt(i);
			l_tudu1.Add(l_dTudu);
		}
		l_dTudu =0.0;
		l_point1.Add(l_posBegin);
		l_tudu1.Add(l_dTudu);
	}
	else//����Ĳ����������ڵ���
	{
		m_iNumOfClick = 0;		
		if (g_pDoc->m_stcWorkFor.IsChinese)
		{	
			RemindContent("\r\n��Ч��û�е㵽��������������",2);
			RemindContent("\r\n���µ����һ����",1);
		}
		else
		{
			RemindContent("\r\nInvalid, there is no point to two consecutive line",2);
			RemindContent("\r\nClick on the first line again",1);
		}
		//���ѡ��
		g_pDoc->RemoveAllCollected();
		return FALSE;
	}
	m_aryTudu.Copy(l_tudu1);
	m_AryPoint.Copy(l_point1);
	return TRUE;
}

/*
 *	�õ�Բ�ģ�����յ�
 */
void CDrawChamfer::GetArcPoint(Position pos1,Position pos2,Position pos3,int clock,Position& begin,Position& end,Position& center)
{
	//��Բ��:Բ�������ߵĽ���
	double l_dAngle1,l_dAngle2;
	//��Բ�������ߵ��ĸ���
	Position l_pos1,l_pos2,l_pos3,l_pos4;
	//����ֱ����X��ƽ��
	int l_iNumParallel = 0;
	if (clock == AD_CLOCKWISE)//�����˳ʱ���Բ
	{
		l_dAngle1 = GetAngleToXAxis(pos1,pos2)-MATH_PI/2;
		l_dAngle2 = GetAngleToXAxis(pos2,pos3)-MATH_PI/2;		
	}
	else//��������ʱ���Բ
	{
		l_dAngle1 = GetAngleToXAxis(pos1,pos2)+MATH_PI/2;
		l_dAngle2 = GetAngleToXAxis(pos2,pos3)+MATH_PI/2;
	}
	if(m_inFlags == 2)//����İ뾶 
	{
		//������Բ�ĵ��ߵ��ĸ����������
		l_pos1.x = pos1.x + m_dRadius*cos(l_dAngle1);
		l_pos1.y = pos1.y + m_dRadius*sin(l_dAngle1);
		l_pos2.x = pos2.x + m_dRadius*cos(l_dAngle1);
		l_pos2.y = pos2.y + m_dRadius*sin(l_dAngle1);
		l_pos3.x = pos2.x + m_dRadius*cos(l_dAngle2);
		l_pos3.y = pos2.y + m_dRadius*sin(l_dAngle2);
		l_pos4.x = pos3.x + m_dRadius*cos(l_dAngle2);
		l_pos4.y = pos3.y + m_dRadius*sin(l_dAngle2);		
	}
	else//������뾶
	{
		double l_dLongPosThree = pos2.Distance(m_posClickThree);//����������ľ���
		double l_dAnglePos1 = GetAngleToXAxis(pos2,pos1);		
		double l_dAnglePos2 = GetAngleToXAxis(pos2,pos3);
		l_pos1.x = pos2.x + l_dLongPosThree*cos(l_dAnglePos1);
		l_pos1.y = pos2.y + l_dLongPosThree*sin(l_dAnglePos1);
		l_pos2.x = l_pos1.x + l_dLongPosThree*cos(l_dAngle1);
		l_pos2.y = l_pos1.y + l_dLongPosThree*sin(l_dAngle1);
		l_pos3.x = pos2.x + l_dLongPosThree*cos(l_dAnglePos2);
		l_pos3.y = pos2.y + l_dLongPosThree*sin(l_dAnglePos2);
		l_pos4.x = l_pos3.x + l_dLongPosThree*cos(l_dAngle2);
		l_pos4.y = l_pos3.y + l_dLongPosThree*sin(l_dAngle2);		
	}
	//�����ߵ�б�ʺͽؾ�
	double k1,k2,b1,b2;
	if (fabs(l_pos1.x - l_pos2.x ) < DISTANCE_ZERO)//�����һ������Y��ƽ��
	{	
		l_iNumParallel = 1;
		center.x = l_pos1.x;
	}
	else
	{
		//��б��//��ؾ�
		k1 = (l_pos1.y - l_pos2.y)/(l_pos1.x - l_pos2.x);
		b1 = l_pos1.y - k1*l_pos1.x;
	}
	if (fabs(l_pos3.x - l_pos4.x ) < DISTANCE_ZERO)//����ڶ�������Y��ƽ��
	{	
		l_iNumParallel = 2;
		center.x = l_pos3.x;
	}
	else
	{
		k2 = (l_pos3.y - l_pos4.y)/(l_pos3.x - l_pos4.x);
		b2 = l_pos3.y - k2*l_pos3.x;
	}
	
	//��Բ�ĵ㡢���û������Y��ƽ��
	if (l_iNumParallel == 0)
		center.x = (b2-b1)/(k1-k2);		
	//�����һ������X��ƽ��
	if (l_iNumParallel == 1)
		center.y = k2*center.x+b2;
	else
		center.y = k1*center.x+b1;
	//�������յ�
	begin = center.DistancePosFoot(pos1,pos2);
	end = center.DistancePosFoot(pos2,pos3);

	
}