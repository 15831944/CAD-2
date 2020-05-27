// CreateBase.cpp: implementation of the CCreateBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "CreateBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCreateBase, CObject, 0)
CCreateBase::CCreateBase()
{
	m_dRemainingDash = 0.0;
}

CCreateBase::~CCreateBase()
{

}

void CCreateBase::DrawRectForPick(CDC* pDC,CPoint Point)
{
	CPoint p1(Point.x+4,Point.y+4);
//	CPoint p2(Point.x-4,Point.y+4);
	CPoint p3(Point.x-4,Point.y-4);
//	CPoint p4(Point.x+4,Point.y-4);
	pDC->Rectangle(p3.x,p3.y,p1.x,p1.y);
	pDC->SelectStockObject(HOLLOW_BRUSH) ;	//���м����
}

// ���ݻ�ͼģʽ���û�ͼ�����������豸������ģʽ����������
void CCreateBase::SetDrawEnvir(CDC*	pDC, int drawMode, CPen* pPen)
{
	int			lineStyle, lineWidth;
	COLORREF	color;
	switch(drawMode) 
	{
	case dmSelect: // ѡ��״̬
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_DOT;
			lineWidth = 1;
			color = m_ColorOfLine;//RGB(0,0,0);
			break;
		}
	case dmPrompt: // ��ʾ״̬
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_DASH;
			lineWidth = 1;
			color = m_ColorOfLine;
			break;
		}
	case dmDrag: // �϶�״̬
		{
			pDC->SetROP2(R2_XORPEN);
			lineStyle = PS_SOLID;
			lineWidth = 1;
			color = RGB(190,190,0);
			break;
		}
	case dmInvalid: // ����״̬
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_SOLID;
			lineWidth = 1;
			color = RGB(255,255,255);; // �ñ���ɫ��
			break;
		}
	case dmLayer: // ͼ��������Ļ�ͼ
		{
			pDC->SetROP2(R2_XORPEN);
			lineStyle = PS_SOLID;//PS_DASHDOT;
			lineWidth = 1;
			color = RGB(55,55,55); // �ñ���ɫ��
			break;
		}
	case dmDragSelect1: // ��ѡ1
		{
			pDC->SetROP2(R2_XORPEN);
			lineStyle = PS_SOLID;
			lineWidth = 1;
			color = RGB(255,55,55);
			break;
		}
	case dmDragSelect2: // ��ѡ2
		{
			pDC->SetROP2(R2_XORPEN);
			lineStyle = PS_DOT;
			lineWidth = 1;
			color = RGB(0,0,0);
			break;
		}
	case dmNormal:   
	default:
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_SOLID;
			lineWidth = 1;
			color = m_ColorOfLine;
			break;
		}		
	}
	pPen->CreatePen(lineStyle,lineWidth,color) ;
}

/*
 *	������ɫ���磬��ɫ1����ɫ2.����
 */
int CCreateBase::RtnColorInt()
{
	//�����ĥ������Ҫ�����һ����G���뻻������̧�µ�
	if(m_iIsBeginGraph == 1&&g_pDoc->m_stcGlassGrind.RouphCount != 0 && g_pDoc->m_stcWorkFor.IsGlassGrind == 1)
	{
		if(g_pDoc->m_stcGlassGrind.PerfectCount == 0 )
			return 1;
		if(g_pDoc->m_stcGlassGrind.PolishCount == 0)
			return 2;
		else 
			return 3;
	}		
	//һ��ͼ�����ߵ�M����
	CString l_strOneM,l_strTwoM;
	//����M���뼯�Ĵ�С 
	int l_iSizeOne = m_strSBM.GetSize();
	int l_iSizeTwo = m_strCBM.GetSize();
	for(int i=0;i<l_iSizeOne;i++)
	{
		l_strOneM = m_strSBM.GetAt(i);
		for(int j=0;j<l_iSizeTwo;j++)
		{
			l_strTwoM = m_strCBM.GetAt(j);
			if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM1&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM1)	
				return 1;
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM2&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM2)
				return 2;
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM3&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM3)
				return 3;
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM4&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM4)
				return 4;
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM5&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM5)
				return 5;
		}
	}
	return 6;
}

/*
 *	���µ������Ƿ����ַ������
 */
BOOL CCreateBase::MstrIsEqualMary(CString strM,CArray<CString,CString&>& aryM)
{
	int l_iSizeofarym = aryM.GetSize();
	CString l_strMCode="";
	//���ַ������������M������
	for(int i=0;i<l_iSizeofarym;i++)
	{
		l_strMCode = aryM.GetAt(i);
		if(l_strMCode == strM)
			return TRUE;
	}
	return FALSE;
}



/*
 *	�����ԶԻ����еĿؼ���ʾ����
 *	IDC_EDIT1 1018
 *	IDC_STATIC1 1026
 */					   //��ʾ����EDIT,�Ƿ���ʾ��Ť
void CCreateBase::ShowControl(int show,BOOL btn)
{
	int i = 0;
	for(i = 0;i<show;i++)//��ʾEdit��STATIC
	{
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(i+1018))->ShowWindow(SW_SHOW);
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(i+1026))->ShowWindow(SW_SHOW);
	}
	for( ;i<8 ;i++)//����Edit��STATIC
	{
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(i+1018))->ShowWindow(SW_HIDE);
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(i+1026))->ShowWindow(SW_HIDE);
	}
	if (btn == TRUE)	//TRUEʱҪ��ʾ��Ť
	{
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(IDC_BTN_FORWARD))->ShowWindow(SW_SHOW);
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(IDC_BTN_BEHIND))->ShowWindow(SW_SHOW);
	}
	else
	{
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(IDC_BTN_FORWARD))->ShowWindow(SW_HIDE);
		(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.GetDlgItem(IDC_BTN_BEHIND))->ShowWindow(SW_HIDE);
	}
}

/*
 *	����ͷ����
 */
void CCreateBase::DrawArrow(double angle,CPoint PosEnd,CDC* pDC)
{
	double d_Sin1 = 10 * sin(angle+MATH_PI/6);
	double d_Cos1 = 10 * cos(angle+MATH_PI/6);	
	double d_Sin2 = 10 * sin(angle-MATH_PI/6);
	double d_Cos2 = 10 * cos(angle-MATH_PI/6);
	CPoint pt[3];
	pt[0] = PosEnd;
	pt[1].x = PosEnd.x - (int)d_Cos1;
	pt[1].y = PosEnd.y + (int)d_Sin1;
	pt[2].x = PosEnd.x - (int)d_Cos2;
	pt[2].y = PosEnd.y + (int)d_Sin2;
	
	pDC->MoveTo(PosEnd);
	pDC->LineTo(pt[1]);
	
	pDC->MoveTo(pt[2]);
	pDC->LineTo(PosEnd);
}

void CCreateBase::DrawMove(CDC *pDC,int DrawMode,BOOL FlagShowArrow /* = FALSE */,BOOL FlagShowNum /* = FALSE */)
{
	int aaa= 0;
}

/*
 *	��ֱ�߻�Բ������
 */
void CCreateBase::DrawArc(CDC* pDC,CPoint potCenter,double dRadius,double dAngleStart,double dAngleOffset)
{
	//�ܳ�
	double l_dPerimeter =dRadius * MATH_PI * 2;
	double l_dAngleOfArc = dAngleOffset/fabs(l_dPerimeter/3*dAngleOffset/360);//Բ����
	CPoint l_potArc;//Բ����ÿ�����
	double l_dAngleStart = dAngleStart + l_dAngleOfArc;
	for(double i = l_dAngleStart;fabs(i-dAngleOffset-dAngleStart)>=fabs(l_dAngleOfArc);i += l_dAngleOfArc)
	{
		l_potArc.x = potCenter.x + dRadius*cos(i/360*MATH_2PI);
		l_potArc.y = potCenter.y - dRadius*sin(i/360*MATH_2PI);
		pDC->LineTo(l_potArc);
	}
	i = dAngleOffset+dAngleStart;
	l_potArc.x = potCenter.x + dRadius*cos(i/360*MATH_2PI);
	l_potArc.y = potCenter.y - dRadius*sin(i/360*MATH_2PI);
	pDC->LineTo(l_potArc);
}

/*
 *	���ԶԻ�����ı༭��ı�ʱ�ĳ���
 */
void CCreateBase::AddUndoList(double dEditNew,double dEditOld,int iWhichEdit,int iNumOfTop/* =0 */)
{
	//������������Ӳ���
	CUndoRedo l_UndoRedo;//������Ķ��� 
	PUNDOLIST pUndoList = l_UndoRedo.NewUndoList();
	pUndoList->OperatieWhat = IDC_BTN_FORWARD;
	
	pUndoList->pOperatieParam = l_UndoRedo.NewOperateParam();
	pUndoList->pOperatieParam->angle = dEditNew;
	pUndoList->pOperatieParam->direction = iWhichEdit;
	pUndoList->pOperatieParam->distance = dEditOld;
	pUndoList->pOperatieParam->pPosition = iNumOfTop;

	pUndoList->pGraphPos  = l_UndoRedo.NewGraphPos();
	pUndoList->pGraphPos->pCreateBasepos = (LONG)this;
	g_pDoc->m_Undo->AddUndoList(pUndoList);
	return;
}

/*
 *	��Բ���������
 */
PGRAPHPOS CCreateBase::TurnArcDash(double dLongSolid,double dLongEmpty,Position posCenter,double dAngleStart,
							  double dAngeleEnd,double dRadius,int iDirect)
{
	Position l_posBegin,l_posEnd;
	double l_dPerimeter = MATH_PI*dRadius*2;//�ܳ�
	//�����ߵ�����
	CUndoRedo l_UndoRedo;
	PGRAPHPOS CurGraphPos,headGraphPos,GraphPos;
	headGraphPos = CurGraphPos = GraphPos =  l_UndoRedo.NewGraphPos();	
	//�ߵĽǶ�
	double l_dAngleSolid = MATH_2PI/(MATH_PI*dRadius*2/dLongSolid);
	double l_dAngleEmpty = MATH_2PI/(MATH_PI*dRadius*2/dLongEmpty);;//�ߵĽǶ�
	POSITION POS = g_pDoc->m_CCreateBaseList.Find(this);
	if(iDirect == AD_CLOCKWISE)//˳ʱ��
	{
		if(dAngeleEnd >= dAngleStart)//���������ʱ
			dAngeleEnd = dAngeleEnd -360;
		for(double l_dDash=dAngleStart/180*MATH_PI;l_dDash>dAngeleEnd/180*MATH_PI;l_dDash=l_dDash-l_dAngleSolid-l_dAngleEmpty)
		{
			l_posBegin.x = posCenter.x + cos(l_dDash)*dRadius;
			l_posBegin.y = posCenter.y + sin(l_dDash)*dRadius;
			if((l_dDash-l_dAngleSolid)< (dAngeleEnd/180*MATH_PI))//�����һ���߶�ʱ
				l_dAngleSolid = l_dDash - (dAngeleEnd/180*MATH_PI);
			l_posEnd.x = posCenter.x + cos(l_dDash-l_dAngleSolid)*dRadius;
			l_posEnd.y = posCenter.y + sin(l_dDash-l_dAngleSolid)*dRadius;
			CCreateRoundArc* pArc= new CCreateRoundArc(l_posBegin,posCenter,l_posEnd,iDirect);
			g_pDoc->m_CCreateBaseList.InsertBefore(POS,pArc);
			//�����������ͼ��
			if(CurGraphPos->pCreateBasepos == NULL)
			{
				CurGraphPos->pCreateBasepos = (long)pArc;
			}
			else
			{
				GraphPos = l_UndoRedo.NewGraphPos();
				GraphPos->pCreateBasepos = (long)pArc;
				CurGraphPos->pNext = GraphPos;
				GraphPos->pPrev = CurGraphPos;
				CurGraphPos = GraphPos;
			}
		}
	}
	else//��ʱ�� 
	{
		if(dAngeleEnd <= dAngleStart)//���������ʱ
			dAngeleEnd = dAngeleEnd +360;
		for(double l_dDash=dAngleStart/180*MATH_PI;l_dDash<dAngeleEnd/180*MATH_PI;l_dDash=l_dDash+l_dAngleSolid+l_dAngleEmpty)
		{
			l_posBegin.x = posCenter.x + cos(l_dDash)*dRadius;
			l_posBegin.y = posCenter.y + sin(l_dDash)*dRadius;
			if((l_dDash+l_dAngleSolid)> (dAngeleEnd/180*MATH_PI))//�����һ���߶�ʱ
				l_dAngleSolid = (dAngeleEnd/180*MATH_PI)-l_dDash;
			l_posEnd.x = posCenter.x + cos(l_dDash+l_dAngleSolid)*dRadius;
			l_posEnd.y = posCenter.y + sin(l_dDash+l_dAngleSolid)*dRadius;
			CCreateRoundArc* pArc= new CCreateRoundArc(l_posBegin,posCenter,l_posEnd,iDirect);
			g_pDoc->m_CCreateBaseList.InsertBefore(POS,pArc);
			//�����������ͼ��
			if(CurGraphPos->pCreateBasepos == NULL)
			{
				CurGraphPos->pCreateBasepos = (long)pArc;
			}
			else
			{
				GraphPos = l_UndoRedo.NewGraphPos();
				GraphPos->pCreateBasepos = (long)pArc;
				CurGraphPos->pNext = GraphPos;
				GraphPos->pPrev = CurGraphPos;
				CurGraphPos = GraphPos;
			}
		}		
	}
	return headGraphPos;	
}


/*
 *	��ֱ�߱������
 */
PGRAPHPOS CCreateBase::TurnLineDash(double dLongSolid,double dLongEmpty,Position posBegin,Position PosEnd)
{
	Position l_posBegin,l_posEnd;
	double l_dLongLine = posBegin.Distance(PosEnd);//�ߵĳ���
	double l_dAngleLine = GetAngleToXAxis(posBegin,PosEnd);//�ߵĽǶ�
	//�����ߵ�����
	CUndoRedo l_UndoRedo;
	PGRAPHPOS CurGraphPos,GraphPos,headGraphPos = NULL;
	headGraphPos = CurGraphPos = GraphPos =  l_UndoRedo.NewGraphPos();
	//��ֱ�߱������
	POSITION POS = g_pDoc->m_CCreateBaseList.Find(this);
	for(double l_dDash=0.0;l_dDash<l_dLongLine;l_dDash=l_dDash+dLongSolid+dLongEmpty)
	{
		if(m_dRemainingDash < 0&&l_dDash==0.0)//˵���հ׵ĳ���Ҫ������߶ε�������//�����
		{
			if(-m_dRemainingDash > l_dLongLine)//������߶γ��Ȼ������հ׳�
			{
				l_dDash = -m_dRemainingDash;
				m_dRemainingDash = 0.0;
				break;
			}
			l_posBegin.x = posBegin.x + cos(l_dAngleLine)*(-m_dRemainingDash);
			l_posBegin.y = posBegin.y + sin(l_dAngleLine)*(-m_dRemainingDash);		
		}
		else
		{
			l_posBegin.x = posBegin.x + cos(l_dAngleLine)*l_dDash;
			l_posBegin.y = posBegin.y + sin(l_dAngleLine)*l_dDash;
		}
		if((l_dDash+dLongSolid - m_dRemainingDash) > l_dLongLine)//���ʣ�µĳ��Ȳ���һ��ʵ�߳�
		{
			m_dRemainingDash = l_dLongLine - l_dDash + m_dRemainingDash;
			m_aryDash.Add(l_posBegin);
		}
		else
		{
			if(l_dDash == 0.0)
				l_dDash -= m_dRemainingDash;
			//��ĩ��
			l_posEnd.x = posBegin.x + cos(l_dAngleLine)*(l_dDash+dLongSolid);
			l_posEnd.y = posBegin.y + sin(l_dAngleLine)*(l_dDash+dLongSolid);
			m_aryDash.Add(l_posBegin);
			m_aryDash.Add(l_posEnd);
			CCreatePolyline* pLine = new CCreatePolyline(m_aryDash);
			g_pDoc->m_CCreateBaseList.InsertBefore(POS,pLine);
			m_aryDash.RemoveAll();							
			//�����������ͼ��
			if(CurGraphPos->pCreateBasepos == NULL)
			{
				CurGraphPos->pCreateBasepos = (long)pLine;
			}
			else
			{
				GraphPos = l_UndoRedo.NewGraphPos();
				GraphPos->pCreateBasepos = (long)pLine;
				CurGraphPos->pNext = GraphPos;
				GraphPos->pPrev = CurGraphPos;
				CurGraphPos = GraphPos;
			}
			m_dRemainingDash = 0.0;			
		}
	}
	if(m_dRemainingDash == 0.0)//˵���߶�ʣ��ĳ����п���û�пհ׳��ȳ�
	{
		m_dRemainingDash = l_dLongLine - l_dDash;
	}
	if(headGraphPos->pCreateBasepos == NULL)//����߶�̫��û������ͼ��
	{
		delete headGraphPos;
		headGraphPos = NULL;
	}
	return headGraphPos;
}









