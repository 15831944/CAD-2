// CreateRoundArc.cpp: implementation of the CCreateRoundArc class.
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
IMPLEMENT_SERIAL(CCreateRoundArc, CCreateBase, 0)
CCreateRoundArc::CCreateRoundArc()
{

}

CCreateRoundArc::CCreateRoundArc(const Position Begin,const Position Center,const Position End,int Direct)
{
	m_ColorOfLine = g_CurColor;		//����ʱ�Ļ�ɫ���ǵ�ǰ��ѡɫ
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_Begin = Begin;	//���
	m_Center = Center;	//�е�
	m_iDirect = Direct;	//����
	m_dRadius = m_Center.Distance(m_Begin) ;//�뾶
	
	double  angle2 = GetAngleToXAxis(m_Center, End);//������ĩ��ĽǶ�
	
	m_End.x = m_dRadius * cos(angle2) + m_Center.x ;
	m_End.y = m_dRadius * sin(angle2) + m_Center.y ;
	m_dAngleEnd = GetAngle(m_Center, m_End);
	m_dAngleStart = GetAngle(m_Center, m_Begin);
	m_iKnifeStyle = 41;//�������� 41�󵶲� 42�ҵ���
	m_iIsBeginGraph = 2;//0����ԭʼͼ�Σ�1��ԭʼͼ�Σ�2�Ǳ���ʱ������
	
}

CCreateRoundArc::~CCreateRoundArc()
{

}

/*
void CCreateRoundArc::Draw(CDC *pDC)
{
	double l_dRadius = m_Center.Distance(m_End);
	// �õ�Բ������Ӿ��ο�
	Position	offset(-l_dRadius,l_dRadius);
	Position	ltpos = m_Center + offset;
	Position	rbpos = m_Center - offset;
	// ��Ļ�����Բ�ĵ㡢��ʼ�����ֹ��
	CPoint sltp, srbp, ssp, sep, center ;
	// ����������ת��Ϊ��Ļ����
	g_pView->WorldToScreem(m_End, sep ) ;
//	g_pView->WorldToScreem(m_Begin, ssp) ;
	g_pView->WorldToScreem(ltpos,sltp) ;
	g_pView->WorldToScreem(rbpos,srbp) ;

	g_pView->WorldToScreem(m_Center,center ) ;


	Graphics myGraphics(pDC->m_hDC);
	Pen myPen(Color(0, 0 ,0),1); 

	double  angle1 = GetAngleToXAxis(m_Center, m_Begin);
	double  angle2 = GetAngleToXAxis(m_Center, m_End);
	double l_StartAngle = 360-angle1/PI*180;
	double l_SweepAngle1 = 360-angle2/PI*180;
	double l_SweepAngle;
	if (l_StartAngle<l_SweepAngle1)
		l_SweepAngle = l_SweepAngle1 - l_StartAngle;
	if (l_StartAngle>l_SweepAngle1)
		l_SweepAngle = 360 - l_StartAngle + l_SweepAngle1;
		
	myGraphics.DrawArc(&myPen,sltp.x,sltp.y,srbp.x-sltp.x,srbp.y-sltp.y ,l_StartAngle,l_SweepAngle);	
}*/


void CCreateRoundArc::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{		
	// ��Ļ�����Բ�ĵ㡢��ʼ�����ֹ��
	CPoint l_ptCenter, l_ptBegin, l_ptEnd ,l_ptDrawBegin;
	// ����������ת��Ϊ��Ļ����
	g_pView->WorldToScreem(m_End, l_ptEnd ) ;
	g_pView->WorldToScreem(m_Begin, l_ptBegin) ;
	g_pView->WorldToScreem(m_Center,l_ptCenter) ;
	//		 ƫ���
	double l_dAngleOffset,l_dAngleStart = m_dAngleStart;
	l_ptDrawBegin = l_ptBegin;
	if(!GetAngleStartAndOffset(l_dAngleStart,l_dAngleOffset, l_ptDrawBegin))
		return;
	// �õ�ԭ���Ļ�ͼģʽ	
	int		n = GetROP2(pDC->GetSafeHdc());
	// �������ʵ�ԭ��
	// ����������Ļ�ͼģʽ�£�ʹ�ó�Ա������������
	// �����������ģʽ,ʹ��ȫ�ֺ���"SetDrawEnvir"��������
	CPen	pen; 
	if (DrawMode == dmNormal)		
		pen.CreatePen(PS_SOLID,1,m_ColorOfLine);//�����������ͼģʽ����ɫ��Ҫȡ������ѡɫ
	else
		SetDrawEnvir(pDC, DrawMode, &pen);	//����SetDrawEnvir�������û�ͼ����
	CPen* pOldPen = pDC->SelectObject(&pen); // �õ�ԭ���Ļ���
	//��Բ��
	pDC->MoveTo(l_ptDrawBegin);
	DrawArc(pDC,l_ptCenter,m_dRadius/g_pView->m_dProportion,l_dAngleStart,l_dAngleOffset);
	//������ͼ�ͷ
	if (FlagShowArrow)	
	{
		//����ʼ��ķ���
		CRect rect(l_ptBegin.x-5,l_ptBegin.y-5,l_ptBegin.x+5,l_ptBegin.y+5);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
		double angle = atan((m_Center.x-m_End.x) /(m_End.y -m_Center.y ));
		if (m_iDirect == AD_COUNTERCLOCKWISE) //�������ʱ��ʱ
			angle = angle+MATH_PI;
		if (m_Center.y > m_End.y)	//�����˳ʱ�룬�����������
			angle = angle + MATH_PI;
		DrawArrow(angle,l_ptEnd,pDC);//����ͷ�ĺ��� 
	}
	///////////////////////����������Ŀ��Լ����ڵڼ�λ����˳ʱ��Ȼ����������� ��////////
	if (FlagShowNum)
	{
		POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
		int l_iNumOfMCode = 0;
		for (int i=0;l_ListPos != NULL;i++)//��������i�ɱ�ʾ������������
		{
			CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
			if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))//��ǰ���ж��ٸ�M����
				l_iNumOfMCode++;
			if(this == (CCreateRoundArc *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //�����Ϳ���͸����������				
				pDC->TextOut(l_ptBegin.x,l_ptBegin.y,str);//��������� ��
				pDC->SetBkMode(l_iOldMode);				
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////	
	// �ָ�ԭ���Ļ��� 
	pDC->SelectObject(pOldPen) ;
	// �ָ�ԭ���Ļ�ͼģʽ
	pDC->SetROP2(n);
	pen.DeleteObject();
}

int CCreateRoundArc::IsPick(CPoint point)
{
	Position pos(0,0);
	//������ű���
	double l_dProportion = g_pView->ReturnProportion();	
	g_pView->ScreemToWorld(point,pos);
	//�����������ĩ��ľ����������ʰȡ�����൱��ĩ���Χ������
	if (pos.Distance(m_Begin)/l_dProportion<=PRECISION||pos.Distance(m_End)/l_dProportion<=PRECISION)
		return TRUE;
	//����Բ���뾶
	double radius = m_Center.Distance(m_Begin) ;
	//����ʰȡ���Բ������������x��ļн�
	double distance =  m_Center.Distance(pos); 
	//ʰȡ�㵽Բ�ĵľ���
	if(!(  (fabs( radius - distance)/l_dProportion)<=PRECISION)  )
		return  -1;

	//����Բ������ʼ��
	double angle = GetAngle(m_Center, pos);
	double angle1 = GetAngle(m_Center, m_Begin) ;
	//����Բ������ֹ��
	double angle2 = GetAngle(m_Center, m_End) ;

	//ʰȡ���Բ������������x��ļн�Ӧ�ô�����
	//��ʼ�Ǻ���ֹ�Ƿ�Χ֮�⣬����ʰȡ�㵽Բ�ĵľ���
	//��Բ���뾶֮��ľ���ֵС�ڸ���ֵʱ������TRUE
	if (m_iDirect == AD_CLOCKWISE)//�����˳ʱ��
	{
		if (angle1<angle2)//�������ʱ������X��������
		{
			if (angle<angle1||angle>angle2)
				return TRUE;
			else
				return -1;
		}
		if (angle<angle1&&angle>angle2)
			return TRUE;
		return -1;
	}
	else//��ʱ��
	{
		if (angle1>angle2)//�������ʱ������X��������
		{
			if (angle<angle2||angle>angle1)
				return TRUE;
			else
				return -1;
		}
		if (angle<angle2&&angle>angle1)
			return TRUE;
		return -1;		
	}
}

CCreateBase* CCreateRoundArc::Copy()
{
	
	CCreateRoundArc *pCopy = new CCreateRoundArc(m_Begin,m_Center,m_End,m_iDirect);//���Ƶ�
	pCopy->m_ColorOfLine = m_ColorOfLine;		//������ɫ
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}


//�ƶ�
void CCreateRoundArc::Move(const Position& basePos,const Position& desPos)
{
	m_Begin = m_Begin.Offset(desPos - basePos);
	m_End = m_End.Offset(desPos - basePos);
	m_Center = m_Center.Offset(desPos - basePos);
	
}
//��ת
void CCreateRoundArc::Rotate(const Position& basePos, const double angle)
{
	m_Begin = m_Begin.Rotate(basePos, angle) ;
	m_End =m_End.Rotate(basePos, angle) ;
	m_Center =m_Center.Rotate(basePos, angle) ;
	m_dAngleStart = GetAngle(m_Center, m_Begin);
	m_dAngleEnd = GetAngle(m_Center, m_End);
}
//����
void CCreateRoundArc::Mirror(const Position& pos1, const Position& pos2)
{
	m_Begin = m_Begin.Mirror(pos1, pos2) ;
	m_End =m_End.Mirror(pos1, pos2) ;
	m_Center =m_Center.Mirror(pos1, pos2) ;
	m_dAngleStart = GetAngle(m_Center, m_Begin);
	m_dAngleEnd = GetAngle(m_Center, m_End);
	if(m_iDirect == AD_CLOCKWISE)
		m_iDirect = AD_COUNTERCLOCKWISE;
	else
		m_iDirect = AD_CLOCKWISE;
}

/*
*	���ԶԻ���
*/
void CCreateRoundArc::PropertyView()
{
	//��ʾ���ԶԻ���
	(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 
	
	//�����ص�STATIC��Edit��ʾ����
	ShowControl(5,FALSE);
	//�趨Ҫ��ʾ����ɫ
	
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(RtnColorInt()-1);
	//����STATICҪ��ʾ�������յ㡱
	//����EditҪ��ʾ�����յ��λ��
	CString str;
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Բ��X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Բ��Y");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"�뾶");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"��ʼ�Ƕ�");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC5,"�˵�Ƕ�");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Center X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Center Y");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"Radius");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"Angle start");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC5,"Angle End");
	}
	
	str.Format("%.4f",m_Center.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
	str.Format("%.4f",m_Center.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
	str.Format("%.4f",m_dRadius);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	str.Format("%.4f",m_dAngleStart);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT4,str);
	str.Format("%.4f",m_dAngleEnd);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT5,str);
	////////////////////////����������Ŀ��Լ����ڵڼ�λ����˳ʱ��///////////////////////////////////
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	for (int i=0;l_ListPos != NULL;i++)//��������i�ɱ�ʾ������������
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//�����M����
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(this == (CCreateRoundArc *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
		{
			str.Format("%d",i);
			((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,str);
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

/*
*	�������ԶԻ��򵯳��ı�ͼԪ����ʱҪ�ػ��ĵĺ���
*/
void CCreateRoundArc::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	Position l_posDes;
	switch(WhichEdit) //�ĸ�EDIT����ˣ��ͱ��ĸ�ֵ
	{
	case 1:
		l_posDes.x = e;
		l_posDes.y = m_Center.y;
		Move(m_Center,l_posDes);
		break;
	case 2:
		l_posDes.x = m_Center.x;
		l_posDes.y = e;
		Move(m_Center,l_posDes);
		break;
	case 3:
		m_dRadius = e;
		m_Begin.x = m_dRadius*cos(m_dAngleStart/360*MATH_2PI)+m_Center.x;
		m_Begin.y = m_dRadius*sin(m_dAngleStart/360*MATH_2PI)+m_Center.y;
		m_End.x = m_dRadius*cos(m_dAngleEnd/360*MATH_2PI)+m_Center.x;
		m_End.y = m_dRadius*sin(m_dAngleEnd/360*MATH_2PI)+m_Center.y;
		break;
	case 4:
		m_dAngleStart = e;
		m_Begin.x = m_dRadius*cos(m_dAngleStart/360*MATH_2PI)+m_Center.x;
		m_Begin.y = m_dRadius*sin(m_dAngleStart/360*MATH_2PI)+m_Center.y;
		break;
	case 5:
		m_dAngleEnd = e;
		m_End.x = m_dRadius*cos(m_dAngleEnd/360*MATH_2PI)+m_Center.x;
		m_End.y = m_dRadius*sin(m_dAngleEnd/360*MATH_2PI)+m_Center.y;
		break;
	default:
		break;
	}
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);	
}

/*
 *	��ѡʱ�Ƿ�ѡ��
 */
BOOL CCreateRoundArc::IsSelect(CRgn *selectrect,BOOL bComplex)
{
	//�����ѡʱ����Ǵ�������ѡ�Ļ�
	if (bComplex == TRUE)
	{
		//�ѿ�ѡ�����þ��α�ʾ
		CRect l_rectRgn;
		selectrect->GetRgnBox(l_rectRgn);
		//�ѿ�ѡ����Ļ����ת����������
		CPoint l_potRgn1(l_rectRgn.left,l_rectRgn.top),l_potRgn2(l_rectRgn.right,l_rectRgn.bottom);
		Position l_posRgn1,l_posRgn2;
		g_pView->ScreemToWorld(l_potRgn1,l_posRgn1);
		g_pView->ScreemToWorld(l_potRgn2,l_posRgn2);
		//��ֱ���Ƿ�������ཻ
		if (ArcOnRect(l_posRgn1,l_posRgn2))
			return TRUE;
	}
	
	BOX2D pBox;		//��Χ�нṹ
	CPoint pos1,pos2;	//��Χ�е�������Ļ���㣨���ϣ����£�
	GetBox(&pBox,TRUE);
	Position l_cPosLT(pBox.min[0],pBox.max[1]),l_cPosRB(pBox.max[0],pBox.min[1]);	
	//ת����Ļ����
	g_pView->WorldToScreem(l_cPosLT,pos1);
	g_pView->WorldToScreem(l_cPosRB,pos2);
	//ֱ�ߵİ�Χ�еķ�������
	CRgn RgnLine;
	RgnLine.CreateRectRgn(pos1.x,pos1.y,pos2.x,pos2.y);
	//�������ֵ�����
	CRgn RgnRect;
	RgnRect.CreateRectRgn(0,0,50,50);
	//ȡ��������Ľ���
	RgnRect.CombineRgn(selectrect,&RgnLine,RGN_AND);
	//�����������Ƿ����
	BOOL IsSelec= RgnLine.EqualRgn(&RgnRect);
	return IsSelec;
}

/*
 *	�õ���Χ��
 */
BOOL CCreateRoundArc::GetBox(BOX2D* pBox,BOOL IsOutside /* = FALSE */)
{
	double MinX, MinY, MaxX, MaxY ,tempX, tempY;
	int pb = 0 ;
	
	double radius = m_Center.Distance(m_Begin) ;
	double StartRa = GetAngleToXAxis(m_Center, m_Begin) ;
	double EndRa = GetAngleToXAxis(m_Center, m_End) ;
	MinX = min( m_Begin.x, m_End.x );
	MinY = min( m_Begin.y, m_End.y );
	MaxX = max( m_Begin.x, m_End.x );
	MaxY = max( m_Begin.y, m_End.y );
	
	if (m_iDirect == AD_COUNTERCLOCKWISE)	//�������ʱ��
	{
		if(StartRa < EndRa) pb = 1 ;
		else
		{
			MinX = min(MinX, m_Center.x+radius) ;
			MinY = min(MinY, m_Center.y) ;
			MaxX = max(MaxX, m_Center.x+radius) ;
			MaxY = max(MaxY, m_Center.y) ;
			tempX = MaxX ;
			tempY = MaxY ;
			pb = 2 ;
		}
		for(int i=0 ;i<4 ;i++)
		{
			if(pb==1 && StartRa<MATH_PI*i/2. && EndRa>MATH_PI*i/2.||
				pb==2 &&!(StartRa>MATH_PI*i/2. && EndRa<MATH_PI*i/2.))			
			{
				if(i==1)
				{
					tempX = m_Center.x  ; tempY = m_Center.y+radius ;
				}
				if(i==2)
				{
					tempX = m_Center.x - radius ; tempY = m_Center.y ;
				}
				if(i==3)
				{
					tempX = m_Center.x ; tempY = m_Center.y - radius ;
				}
				MinX = min(MinX, tempX) ;
				MinY = min(MinY, tempY) ;
				MaxX = max(MaxX, tempX) ;
				MaxY = max(MaxY, tempY) ;
			}
		}
	}
	else//˳ʱ��
	{
		if(StartRa > EndRa) pb = 1 ;
		else
		{
			MinX = min(MinX, m_Center.x+radius) ;
			MinY = min(MinY, m_Center.y) ;
			MaxX = max(MaxX, m_Center.x+radius) ;
			MaxY = max(MaxY, m_Center.y) ;
			tempX = MaxX ;
			tempY = MaxY ;
			pb = 2 ;
		}
		for(int i=0 ;i<4 ;i++)
		{
			if(pb==1 && StartRa>MATH_PI*i/2. && EndRa<MATH_PI*i/2.||
				pb==2 &&!(StartRa<MATH_PI*i/2. && EndRa>MATH_PI*i/2.))			
			{
				if(i==1)
				{
					tempX = m_Center.x  ; tempY = m_Center.y+radius ;
				}
				if(i==2)
				{
					tempX = m_Center.x - radius ; tempY = m_Center.y ;
				}
				if(i==3)
				{
					tempX = m_Center.x ; tempY = m_Center.y - radius ;
				}
				MinX = min(MinX, tempX) ;
				MinY = min(MinY, tempY) ;
				MaxX = max(MaxX, tempX) ;
				MaxY = max(MaxY, tempY) ;
			}
		}
	}
	//	//�õ���Χ�е������㣬�������꣨���£����ϣ�
	//�Ѱ�Χ������
	double curRadius = g_pView->ReturnProportion()*PRECISION;
	//���û������
	if (IsOutside == FALSE)
		curRadius = 0.0;
	pBox->min[0] = MinX - curRadius;
	pBox->min[1] = MinY - curRadius;
	pBox->max[0] = MaxX + curRadius;
	pBox->max[1] = MaxY + curRadius;
	return TRUE;
}

//XY���Ź���
BOOL CCreateRoundArc::GraphZoomXY(double dValueX, double dValueY)
{
	return FALSE;
	
}

/*
 *	����ͼ���ϵ�һ�����볤�ȷ���һ����
 */
BOOL CCreateRoundArc::RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine /* = 0 */)
{
	//Բ�ĵ�������ĽǶ�
	double l_dAngle = GetAngleToXAxis(m_Center,ClickPos);
	double l_dLongArc,l_dAngle2,l_dAngleStart = m_dAngleStart/180*MATH_PI,l_dAngleEnd = m_dAngleEnd/180*MATH_PI;
	//Բ�����ܳ�
	double l_dPerimeter = 2*MATH_PI*m_dRadius;
	//�������ĳ��ȴ���Բ���ĳ��ȾͲ�����
	if (dLongPos > 0)//�ж���ĩ��ľ���
	{
		if (m_iDirect == AD_CLOCKWISE)//˳ʱ�� 
		{	//ĩ��͵����������ߵ�����
			if (l_dAngleStart<l_dAngleEnd&&l_dAngle<l_dAngleEnd)//ĩ�������߲��ҵ��û�������
			{
				l_dLongArc = ((l_dAngle+MATH_2PI-l_dAngleEnd)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle+MATH_2PI-((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
			else//ĩ���������������һ��
			{
				l_dLongArc = ((l_dAngle-l_dAngleEnd)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle-((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
		}
		else//��ʱ��
		{	//ĩ��͵����������ߵ�����
			if (l_dAngleStart>l_dAngleEnd&&l_dAngle>l_dAngleEnd)//ĩ�������߲��ҵ��û�������
			{
				l_dLongArc = ((l_dAngleEnd+MATH_2PI-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle-MATH_2PI+((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
			else//ĩ���������������һ��
			{
				l_dLongArc = ((l_dAngleEnd-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = ((dLongPos/l_dPerimeter)*MATH_2PI)+l_dAngle;//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
		}
	}
	else if (dLongPos < 0 )
	{
		dLongPos = fabs(dLongPos);			
		if (m_iDirect == AD_CLOCKWISE)//˳ʱ�� 
		{	//���͵����������ߵ�����
			if (l_dAngleStart<l_dAngleEnd&&l_dAngle>l_dAngleStart)//��������߲��ҵ��û�������
			{
				l_dLongArc = ((l_dAngleStart+MATH_2PI-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle-MATH_2PI+((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
			else//����������������һ��
			{
				l_dLongArc = ((l_dAngleStart-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle+((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
		}
		else//��ʱ��
		{	//���͵����������ߵ�����
			if (l_dAngleStart>l_dAngleEnd&&l_dAngle<l_dAngleStart)//��������߲��ҵ��û�������
			{
				l_dLongArc = ((l_dAngle+MATH_2PI-l_dAngleStart)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle+MATH_2PI-((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
			else//����������������һ��
			{
				l_dLongArc = ((l_dAngle-l_dAngleStart)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle-((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle2);
					posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle2);	
					return TRUE;
				}
				else
					return FALSE;	
			}
		}
	}
	return FALSE;
}


/*
 *	�ı��и��
 */
void CCreateRoundArc::ChangeDir()
{
	//�������յ�����λ�û�һ�£�
	Position pos = m_Begin;
	m_Begin = m_End;
	m_End = pos;
	//�ı�Բ���и��
	if(m_iDirect == AD_CLOCKWISE)
		m_iDirect = AD_COUNTERCLOCKWISE;
	else
		m_iDirect = AD_CLOCKWISE;
	m_dAngleStart = GetAngle(m_Center, m_Begin);
	m_dAngleEnd = GetAngle(m_Center, m_End);
	
}

/*
 *	Բ���Ƿ��ھ�����
 */
BOOL CCreateRoundArc::ArcOnRect(Position pos1,Position pos3)
{
	Position posTemp1,posTemp2,pos2(pos1.x,pos3.y),pos4(pos3.x,pos1.y),l_posInter;
	//�������������Ƿ���Բ����
	posTemp1 = pos1;
	posTemp2 = pos2;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//�Ƿ���Բ��
	{
		if((PosOnArc(posTemp1)&&PosOnLine(pos1,pos2,posTemp1))||(PosOnArc(posTemp2)&&PosOnLine(pos1,pos2,posTemp2)))//�Ƿ��ڻ���
			return TRUE;
	}
	posTemp1 = pos2;
	posTemp2 = pos3;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))    //�Ƿ���Բ��
	{
		if((PosOnArc(posTemp1)&&PosOnLine(pos2,pos3,posTemp1))||(PosOnArc(posTemp2)&&PosOnLine(pos2,pos3,posTemp2)))//�Ƿ��ڻ���
			return TRUE;
	}
	posTemp1 = pos3;
	posTemp2 = pos4;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//�Ƿ���Բ��
	{
		if((PosOnArc(posTemp1)&&PosOnLine(pos3,pos4,posTemp1))||(PosOnArc(posTemp2)&&PosOnLine(pos3,pos4,posTemp2)))//�Ƿ��ڻ���
			return TRUE;
	}
	posTemp1 = pos4;
	posTemp2 = pos1;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//�Ƿ���Բ��
	{
		if((PosOnArc(posTemp1)&&PosOnLine(pos1,pos4,posTemp1))||(PosOnArc(posTemp2)&&PosOnLine(pos1,pos4,posTemp2)))//�Ƿ��ڻ���
			return TRUE;
	}
	return FALSE;
}

/*
 *	���Ƿ���Բ����
 */
BOOL CCreateRoundArc::PosOnArc(Position pos)
{	
	//����Բ������ʼ��
	double angle = GetAngle(m_Center, pos);
	double angle1 = GetAngle(m_Center, m_Begin) ;
	//����Բ������ֹ��
	double angle2 = GetAngle(m_Center, m_End) ;
	
	if (m_iDirect == AD_CLOCKWISE)//�����˳ʱ��
	{
		if (angle1<angle2)//�������ʱ������X��������
		{
			if (angle<angle1||angle>angle2)
				return TRUE;
			else
				return FALSE;
		}
		if (angle<angle1&&angle>angle2)
			return TRUE;
		return FALSE;
	}
	else//��ʱ��
	{
		if (angle1>angle2)//�������ʱ������X��������
		{
			if (angle<angle2||angle>angle1)
				return TRUE;
			else
				return FALSE;
		}
		if (angle<angle2&&angle>angle1)
			return TRUE;
		return FALSE;		
	}
	return FALSE;
}

/*
 *	��������Ļ����ʾ����ʼ�Ǻ�ת���Ƕ�
 */
BOOL CCreateRoundArc::GetAngleStartAndOffset(double& dAngleStart,double& dAngleOffset,CPoint& potBegin)
{
//�ж�ͼ���Ƿ���ȫ���ڿͻ���
	//����Ļ�����þ��α�ʾ
	CRect l_rectRgn;
	g_pView->GetClientRect(l_rectRgn);
	CRgn selectrect;
	CPoint l_potRgn1(0,0),l_potRgn2(l_rectRgn.right,l_rectRgn.bottom);
	selectrect.CreateRectRgn(l_potRgn1.x,l_potRgn1.y,l_potRgn2.x,l_potRgn2.y);
	//�ѿͻ�������Ļ����ת����������
	Position l_posRgn1,l_posRgn3;
	g_pView->ScreemToWorld(l_potRgn1,l_posRgn1);
	g_pView->ScreemToWorld(l_potRgn2,l_posRgn3);
	//��Χ�нṹ
	BOX2D pBox;		
	CPoint pos1,pos2;	//��Χ�е�������Ļ���㣨���ϣ����£�
	GetBox(&pBox);
	Position l_cPosLT(pBox.min[0],pBox.max[1]),l_cPosRB(pBox.max[0],pBox.min[1]);	
	//תͼ����Ļ����
	g_pView->WorldToScreem(l_cPosLT,pos1);
	g_pView->WorldToScreem(l_cPosRB,pos2);
	//ֱ�ߵİ�Χ�еķ�������
	CRgn RgnLine;
	RgnLine.CreateRectRgn(pos1.x,pos1.y,pos2.x,pos2.y);
	//�������ֵ�����
	CRgn RgnRect;
	RgnRect.CreateRectRgn(0,0,50,50);
	//ȡ��������Ľ���
	RgnRect.CombineRgn(&selectrect,&RgnLine,RGN_AND);
	//�����������Ƿ����
	if(RgnLine.EqualRgn(&RgnRect))
		return CaleStartAndOffset(dAngleStart,dAngleOffset);
//�����ڿͻ��������	
	//��߿�Ľ���
	CArray<Position,Position&> l_aryArcPoint;//�㼯	
	Position posTemp1,posTemp2,l_posRgn2(l_posRgn1.x,l_posRgn3.y),l_posRgn4(l_posRgn3.x,l_posRgn1.y),l_posInter;
	//�������������Ƿ���Բ����
	posTemp1 = l_posRgn1;
	posTemp2 = l_posRgn2;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//�Ƿ���Բ��
	{
		//��������������
		if (posTemp1 == posTemp2)
			l_aryArcPoint.Add(posTemp1);
		else
		{
			if(PosOnArc(posTemp1)&&PosOnLine(l_posRgn1,l_posRgn2,posTemp1))
				l_aryArcPoint.Add(posTemp1);
			if (PosOnArc(posTemp2)&&PosOnLine(l_posRgn1,l_posRgn2,posTemp2))
				l_aryArcPoint.Add(posTemp2);
		}
	}
	posTemp1 = l_posRgn2;
	posTemp2 = l_posRgn3;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))    //�Ƿ���Բ��
	{
		//��������������
		if (posTemp1 == posTemp2)
			l_aryArcPoint.Add(posTemp1);
		else
		{
			if(PosOnArc(posTemp1)&&PosOnLine(l_posRgn2,l_posRgn3,posTemp1))
				l_aryArcPoint.Add(posTemp1);
			if (PosOnArc(posTemp2)&&PosOnLine(l_posRgn2,l_posRgn3,posTemp2))
				l_aryArcPoint.Add(posTemp2);
		}
	}
	posTemp1 = l_posRgn3;
	posTemp2 = l_posRgn4;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//�Ƿ���Բ��
	{
		//��������������
		if (posTemp1 == posTemp2)
			l_aryArcPoint.Add(posTemp1);
		else
		{
			if(PosOnArc(posTemp1)&&PosOnLine(l_posRgn3,l_posRgn4,posTemp1))
				l_aryArcPoint.Add(posTemp1);
			if (PosOnArc(posTemp2)&&PosOnLine(l_posRgn3,l_posRgn4,posTemp2))
				l_aryArcPoint.Add(posTemp2);
		}
	}
	posTemp1 = l_posRgn4;
	posTemp2 = l_posRgn1;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//�Ƿ���Բ��
	{
		//��������������
		if (posTemp1 == posTemp2)
			l_aryArcPoint.Add(posTemp1);
		else
		{
			if(PosOnArc(posTemp1)&&PosOnLine(l_posRgn1,l_posRgn4,posTemp1))
				l_aryArcPoint.Add(posTemp1);
			if (PosOnArc(posTemp2)&&PosOnLine(l_posRgn1,l_posRgn4,posTemp2))
				l_aryArcPoint.Add(posTemp2);
		}
	}
	//�������
	int l_iNumArcPoint = l_aryArcPoint.GetSize();
	//�н�
	double l_dAngle = 0.0;
	if (l_iNumArcPoint >2)
		return CaleStartAndOffset(dAngleStart,dAngleOffset);
	else if(l_iNumArcPoint == 1)//Բ����ͻ���ֻ��һ������
	{
		Position l_posInterArc = l_aryArcPoint.GetAt(0);
		if(PosBelongBox(l_posRgn1,l_posRgn3,m_Begin))//����ڿͻ���
		{
			l_dAngle = GetAngle(m_Center,l_posInterArc);
			if(m_iDirect == AD_CLOCKWISE)		//˳ʱ��
			{
				if(l_dAngle<m_dAngleStart)//û��0�Ƚ�ʱ
					dAngleOffset = l_dAngle - m_dAngleStart;
				else//����Ƚ�ʱ
					dAngleOffset = l_dAngle - 360 -m_dAngleStart;
			}
			else//��ʱ��
			{
				if(l_dAngle>m_dAngleStart)//û��0�Ƚ�ʱ
					dAngleOffset = l_dAngle - m_dAngleStart;
				else//����Ƚ�ʱ
					dAngleOffset = 360 -m_dAngleStart + l_dAngle;
			}
			dAngleStart = m_dAngleStart;
			return TRUE;
		}
		if(PosBelongBox(l_posRgn1,l_posRgn3,m_End))//�յ��ڿͻ���
		{
			l_dAngle = GetAngle(m_Center,l_posInterArc);
			g_pView->WorldToScreem(l_posInterArc,potBegin); 
			dAngleStart = l_dAngle;
			if(m_iDirect == AD_CLOCKWISE)		//˳ʱ��
			{
				if(m_dAngleEnd<l_dAngle)//û��0�Ƚ�ʱ
					dAngleOffset = m_dAngleEnd - l_dAngle;
				else//����Ƚ�ʱ
					dAngleOffset = m_dAngleEnd - 360 -l_dAngle;
			}
			else//��ʱ��
			{
				if(m_dAngleEnd>l_dAngle)//û��0�Ƚ�ʱ
					dAngleOffset = m_dAngleEnd - l_dAngle;
				else//����Ƚ�ʱ
					dAngleOffset = 360 -l_dAngle + m_dAngleEnd;
			}
			return TRUE;
		}	
	}
	else if(l_iNumArcPoint == 2)
	{	//�յ������ڿͻ���
		if(PosBelongBox(l_posRgn1,l_posRgn3,m_End)&&PosBelongBox(l_posRgn1,l_posRgn3,m_Begin))
			return CaleStartAndOffset(dAngleStart,dAngleOffset);
		else
		{
			Position l_posInterArc1 = l_aryArcPoint.GetAt(0);			
			Position l_posInterArc2 = l_aryArcPoint.GetAt(1);
			//����������Բ�ĽǶȣ�����ʼ���ƫ�ý�
			double angle1,angle2,l_dAngleOffset1,l_dAngleOffset2;
			angle1 = GetAngle(m_Center,l_posInterArc1);
			angle2 = GetAngle(m_Center,l_posInterArc2);
			if(m_iDirect == AD_CLOCKWISE)		//˳ʱ��
			{
				if(angle1<m_dAngleStart)//û��0�Ƚ�ʱ
					l_dAngleOffset1 = angle1 - m_dAngleStart;
				else//����Ƚ�ʱ
					l_dAngleOffset1 = angle1 - 360 -m_dAngleStart;
				if(angle2<m_dAngleStart)//û��0�Ƚ�ʱ
					l_dAngleOffset2 = angle2 - m_dAngleStart;
				else//����Ƚ�ʱ
					l_dAngleOffset2 = angle2 - 360 -m_dAngleStart;
				if(l_dAngleOffset1 < l_dAngleOffset2)
				{
					dAngleStart = angle2;
					g_pView->WorldToScreem(l_posInterArc2,potBegin); 
					dAngleOffset = l_dAngleOffset1 - l_dAngleOffset2;
				}				
				else
				{
					dAngleStart = angle1;
					g_pView->WorldToScreem(l_posInterArc1,potBegin); 
					dAngleOffset = l_dAngleOffset2 - l_dAngleOffset1;					
				}
				
			}
			else//��ʱ��
			{
				if(angle1>m_dAngleStart)//û��0�Ƚ�ʱ
					l_dAngleOffset1 = angle1 - m_dAngleStart;
				else//����Ƚ�ʱ
					l_dAngleOffset1 = 360 -m_dAngleStart + angle1;
				if(angle2>m_dAngleStart)//û��0�Ƚ�ʱ
					l_dAngleOffset2 = angle2 - m_dAngleStart;
				else//����Ƚ�ʱ
					l_dAngleOffset2 = 360 -m_dAngleStart + angle2;
				if(l_dAngleOffset1 < l_dAngleOffset2)
				{
					dAngleStart = angle1;
					g_pView->WorldToScreem(l_posInterArc1,potBegin); 
					dAngleOffset = l_dAngleOffset2 - l_dAngleOffset1;
				}				
				else
				{
					dAngleStart = angle2;
					g_pView->WorldToScreem(l_posInterArc2,potBegin); 
					dAngleOffset = l_dAngleOffset1 - l_dAngleOffset2;					
				}
			}
			return TRUE;
		}
	}
	return FALSE;	
}

/*
 *	������ʼ�Ǻ�ת���Ƕ�
 */
BOOL CCreateRoundArc::CaleStartAndOffset(double& dAngleStart,double& dAngleOffset)
{
	if(m_iDirect == AD_CLOCKWISE)		//˳ʱ��
	{
	 if(m_dAngleEnd<m_dAngleStart)//û��0�Ƚ�ʱ
		 dAngleOffset = m_dAngleEnd - m_dAngleStart;
	 else//����Ƚ�ʱ
		 dAngleOffset = m_dAngleEnd - 360 -m_dAngleStart;
	}
	else//��ʱ��
	{
	 if(m_dAngleEnd>m_dAngleStart)//û��0�Ƚ�ʱ
		 dAngleOffset = m_dAngleEnd - m_dAngleStart;
	 else//����Ƚ�ʱ
		 dAngleOffset = 360 -m_dAngleStart + m_dAngleEnd;
	}
	dAngleStart = m_dAngleStart;
	return TRUE;
}

/*
*	����������
*/
void CCreateRoundArc::GraphZoomScale(Position posBase,double dScale)
{
	//��������ĩ��ĽǶ�
	double l_dAngleCT = GetAngleToXAxis(posBase,m_Center);
	double l_dAngleBG = GetAngleToXAxis(posBase,m_Begin);
	double l_dAngleEN = GetAngleToXAxis(posBase,m_End);
	//���㵽��ĩ��ĳ���
	double l_dLongCT = posBase.Distance(m_Center);
	m_Center.x = posBase.x + cos(l_dAngleCT)*l_dLongCT*dScale; 
	m_Center.y = posBase.y + sin(l_dAngleCT)*l_dLongCT*dScale; 
	//���㵽��ĩ��ĳ���
	double l_dLongEN = posBase.Distance(m_End);
	m_End.x = posBase.x + cos(l_dAngleEN)*l_dLongEN*dScale; 
	m_End.y = posBase.y + sin(l_dAngleEN)*l_dLongEN*dScale; 
	//���㵽��ĩ��ĳ���
	double l_dLongBG = posBase.Distance(m_Begin);
	m_Begin.x = posBase.x + cos(l_dAngleBG)*l_dLongBG*dScale; 
	m_Begin.y = posBase.y + sin(l_dAngleBG)*l_dLongBG*dScale; 
	//�뾶�仯
	m_dRadius *= dScale;
}

/*
 *	ʵ�߱�����
 */
PGRAPHPOS CCreateRoundArc::SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist)
{
	PGRAPHPOS headGraphPos =  TurnArcDash(dLongSolid,dLongEmpty,m_Center,m_dAngleStart,m_dAngleEnd,m_dRadius,m_iDirect);
	return headGraphPos;
}











