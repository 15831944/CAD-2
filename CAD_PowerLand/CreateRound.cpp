// CreateRound.cpp: implementation of the CCreateRound class.
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
IMPLEMENT_SERIAL(CCreateRound, CCreateBase, 0)
CCreateRound::CCreateRound()
{

}

CCreateRound::CCreateRound(const Position& center,const Position& endpos,int Direct)
{
	m_iDirect = Direct;
	m_ColorOfLine = g_CurColor;		//����ʱ�Ļ�ɫ���ǵ�ǰ��ѡɫ
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_Center = center;
	m_EndPos = endpos;
	m_dRadius = m_Center.Distance(m_EndPos);
	m_iKnifeStyle = 41;//�������� 41�󵶲� 42�ҵ���
	m_iIsBeginGraph = 2;//0����ԭʼͼ�Σ�1��ԭʼͼ�Σ�2�Ǳ���ʱ������
	
}

CCreateRound::~CCreateRound()
{

}

/*
void CCreateRound::Draw(CDC *pDC)
{
	double l_dRadius = m_Center.Distance(m_EndPos);
	// �õ�Բ����Ӿ��ο�
	Position ltpos(m_Center.x - l_dRadius, m_Center.y + l_dRadius ) ;
	Position rbpos(m_Center.x + l_dRadius, m_Center.y - l_dRadius ) ;
	// ��Ļ�������Ӿ��ο�������ǵ��Բ�ĵ�
	CPoint slt, srb, scenp;
	// ����������ת��Ϊ��Ļ����
	g_pView->WorldToScreem(ltpos,slt) ;
	g_pView->WorldToScreem(rbpos,srb) ;
	pDC->SelectStockObject(NULL_BRUSH) ;
	Graphics myGraphics(pDC->m_hDC);
	Pen myPen(Color(0, 0 ,0),1); 
	myGraphics.DrawEllipse(&myPen,slt.x, slt.y, srb.x-slt.x, srb.y-slt.y);
}*/


void CCreateRound::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{
	Position ltpos(m_Center.x - m_dRadius, m_Center.y + m_dRadius ) ;
	Position rbpos(m_Center.x + m_dRadius, m_Center.y - m_dRadius ) ;
	// ��Ļ�������Ӿ��ο�������ǵ��Բ�ĵ�
	CPoint slt, srb,pt_begin;//������꣨��Ļ����ϵ��
	// ����������ת��Ϊ��Ļ����
	g_pView->WorldToScreem(m_EndPos,pt_begin) ;
	g_pView->WorldToScreem(ltpos,slt) ;
	g_pView->WorldToScreem(rbpos,srb) ;
	
	int		n = GetROP2(pDC->GetSafeHdc()); 

	CPen	pen; 
	if (DrawMode == dmNormal)		
		pen.CreatePen(PS_SOLID,1,m_ColorOfLine);//�����������ͼģʽ����ɫ��Ҫȡ������ѡɫ
	else
		SetDrawEnvir(pDC, DrawMode, &pen);	//����SetDrawEnvir�������û�ͼ����
	CPen* pOldPen = pDC->SelectObject(&pen); // �õ�ԭ���Ļ���
	// ������Ļ�������ͼԪ	
	pDC->SelectStockObject(NULL_BRUSH) ;	//��Բ�м�͸��
	pDC->Ellipse(slt.x, slt.y, srb.x, srb.y) ;
	if (FlagShowArrow)	//������ͼ�ͷ
	{
		//����ʼ��ķ���
		CRect rect(pt_begin.x-5,pt_begin.y-5,pt_begin.x+5,pt_begin.y+5);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect); 
		double angle = atan((m_Center.x-m_EndPos.x) /(m_EndPos.y -m_Center.y ));
		if (m_iDirect == AD_COUNTERCLOCKWISE) //�������ʱ��ʱ
			angle = angle+MATH_PI;
		if (m_Center.y > m_EndPos.y)	//�����˳ʱ�룬�����������
			angle = angle + MATH_PI;
		if (m_iDirect == AD_CLOCKWISE)
			DrawArrow(angle,pt_begin,pDC);//����ͷ�ĺ��� 
		else
			DrawArrow(angle,pt_begin,pDC);//����ͷ�ĺ���
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
			if(this == (CCreateRound *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //�����Ϳ���͸����������				
				pDC->TextOut(pt_begin.x+1,pt_begin.y+1,str);//��������� ��
				pDC->SetBkMode(l_iOldMode);				
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////	
	// �ָ�ԭ���Ļ��� 
	pDC->SelectObject(pOldPen); 
	// �ָ�ԭ���Ļ�ͼģʽ
	pDC->SetROP2(n); 	
	pen.DeleteObject();//�ͷ�pen��Create��;
}

int CCreateRound::IsPick(CPoint point)
{
	double  d_lRadius = m_Center.Distance(m_EndPos);//Բ�İ뾶
	Position pos(0,0);
	g_pView->ScreemToWorld(point,pos);
	double d_lDistance = m_Center.Distance(pos);
	double l_dProportion = g_pView->ReturnProportion();
	if ((fabs(d_lRadius-d_lDistance)/l_dProportion)<=PRECISION)
		return TRUE;
	return -1;
}

CCreateBase* CCreateRound::Copy()
{
	CCreateRound *pCopy = new CCreateRound(m_Center,m_EndPos,m_iDirect);//���Ƶ�
	pCopy->m_ColorOfLine = m_ColorOfLine;		//������ɫ
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}

//�ƶ�
void CCreateRound::Move(const Position& basePos,const Position& desPos)
{
	Position pos(basePos.x,basePos.y);
	m_Center = m_Center.Offset(desPos - pos);
	m_EndPos = m_EndPos.Offset(desPos - pos);
}
//��ת
void CCreateRound::Rotate(const Position& basePos, const double angle)
{
	m_Center = m_Center.Rotate(basePos,angle);
	m_EndPos = m_EndPos.Rotate(basePos,angle);
}
//����
void CCreateRound::Mirror(const Position& pos1, const Position& pos2)
{
	m_Center = m_Center.Mirror(pos1,pos2);
	m_EndPos = m_EndPos.Mirror(pos1,pos2);
}

/*
*	���ԶԻ���
*/
void CCreateRound::PropertyView()
{
	//��ʾ���ԶԻ���
	(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 
	
	//�����ص�STATIC��Edit��ʾ����
	ShowControl(3,FALSE);
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
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Center X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Center Y");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"Radius");
	}
	
	str.Format("%.4f",m_Center.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
	str.Format("%.4f",m_Center.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
	str.Format("%.4f",m_dRadius);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	////////////////////////����������Ŀ��Լ����ڵڼ�λ����˳ʱ��///////////////////////////////////
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	for (int i=0;l_ListPos != NULL;i++)//��������i�ɱ�ʾ������������
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//�����M����
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(this == (CCreateRound *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
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
void CCreateRound::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
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
		m_EndPos.x = m_Center.x - m_dRadius;
		m_EndPos.y = m_Center.y;
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
BOOL CCreateRound::IsSelect(CRgn *selectrect,BOOL bComplex)
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
		if (RoundOnRect(l_posRgn1,l_posRgn2))
			return TRUE;
	}
	BOX2D pBox;		//��Χ�нṹ
	CPoint pos1,pos2;	//��Χ�е�������Ļ���㣨���ϣ����£�
	//�õ���Χ�е������㣬�������꣨���£����ϣ�
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
	BOOL l_bIsSelect= RgnLine.EqualRgn(&RgnRect);
	return l_bIsSelect;
}

/*
 *	�õ���Χ��
 */
BOOL CCreateRound::GetBox(BOX2D* pBox,BOOL IsOutside /* = FALSE */)
{
	double curRadius =  g_pView->ReturnProportion();
	//���û������
	if (IsOutside == FALSE)
		curRadius = 0.0;
	pBox->min[0] = m_Center.x-m_dRadius - curRadius;
	pBox->min[1] = m_Center.y-m_dRadius - curRadius;
	pBox->max[0] = m_Center.x+m_dRadius + curRadius;
	pBox->max[1] = m_Center.y+m_dRadius + curRadius;
	return TRUE;
}

//XY���Ź���
BOOL CCreateRound::GraphZoomXY(double dValueX, double dValueY)
{
	return FALSE;
	
}

/*
 *	����ͼ���ϵ�һ�����볤�ȷ���һ����
 */
BOOL CCreateRound::RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine /* = 0 */)
{
	//Բ�ĵ�������ĽǶ�
	double l_dAngle = GetAngleToXAxis(m_Center,ClickPos);
	double l_dLongArc;
	//Բ�����ܳ�
	double l_dPerimeter = 2*MATH_PI*m_dRadius;
	//�����������ܳ��˳�
	if (l_dPerimeter < dLongPos)
		return FALSE;
	else
	{//������ĳ��ȶ�Ӧ�ĽǶ�
		l_dLongArc = ((dLongPos/l_dPerimeter)*MATH_2PI);
	}
	//�������ĳ��ȴ���Բ���ĳ��ȾͲ�����
	if (dLongPos > 0)//�ж���ĩ��ľ���
	{
		if (m_iDirect == AD_CLOCKWISE)//˳ʱ�� 
		{//ĩ��͵����������ߵ�����
			posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle-l_dLongArc);
			posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle-l_dLongArc);	
			return TRUE;
		}
		else//��ʱ��
		{
			posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle+l_dLongArc);
			posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle+l_dLongArc);				
			return TRUE;
		}
	}
	else//�ж������ľ���
	{
		dLongPos = fabs(dLongPos);			
		if (m_iDirect == AD_CLOCKWISE)//˳ʱ�� 
		{//ĩ��͵����������ߵ�����
			posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle+l_dLongArc);
			posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle+l_dLongArc);				
			return TRUE;
		}
		else//��ʱ��
		{
			posTarget.x = m_Center.x + m_dRadius*cos(l_dAngle-l_dLongArc);
			posTarget.y = m_Center.y + m_dRadius*sin(l_dAngle-l_dLongArc);				
			return TRUE;
		}
	}
	return FALSE;
}


/*
 *	�ı��и��
 */
void CCreateRound::ChangeDir()
{
	//�ı�Բ���и��
	if(m_iDirect == AD_CLOCKWISE)
		m_iDirect = AD_COUNTERCLOCKWISE;
	else
		m_iDirect = AD_CLOCKWISE;
	
}

/*
 *	Բ���Ƿ��ھ�����
 */
BOOL CCreateRound::RoundOnRect(Position pos1,Position pos3)
{
	Position posTemp1 = pos1,posTemp2,pos2(pos1.x,pos3.y),pos4(pos3.x,pos1.y),l_posInter;
	posTemp2 = pos2;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//�Ƿ���Բ��
	{
		if(PosOnLine(pos1,pos2,posTemp1)||PosOnLine(pos1,pos2,posTemp2))//�Ƿ��ڻ���
			return TRUE;
	}
	posTemp1 = pos2;
	posTemp2 = pos3;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))    //�Ƿ���Բ��
	{
		if(PosOnLine(pos3,pos2,posTemp1)||PosOnLine(pos3,pos2,posTemp2))//�Ƿ��ڻ���
			return TRUE;
	}
	posTemp1 = pos3;
	posTemp2 = pos4;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//�Ƿ���Բ��
	{
		if(PosOnLine(pos3,pos4,posTemp1)||PosOnLine(pos3,pos4,posTemp2))//�Ƿ��ڻ���
			return TRUE;
	}
	posTemp1 = pos4;
	posTemp2 = pos1;
	if (GetIntersectLineRound(posTemp1,posTemp2,m_Center,m_dRadius))//�Ƿ���Բ��
	{
		if(PosOnLine(pos1,pos4,posTemp1)||PosOnLine(pos1,pos4,posTemp2))//�Ƿ��ڻ���
			return TRUE;
	}
	return FALSE;
}

/*
*	����������
*/
void CCreateRound::GraphZoomScale(Position posBase,double dScale)
{
	//��������ĩ��ĽǶ�
	double l_dAngleCT = GetAngleToXAxis(posBase,m_Center);
	double l_dAngleEN = GetAngleToXAxis(posBase,m_EndPos);
	//���㵽��ĩ��ĳ���
	double l_dLongCT = posBase.Distance(m_Center);
	m_Center.x = posBase.x + cos(l_dAngleCT)*l_dLongCT*dScale; 
	m_Center.y = posBase.y + sin(l_dAngleCT)*l_dLongCT*dScale; 
	//���㵽��ĩ��ĳ���
	double l_dLongEN = posBase.Distance(m_EndPos);
	m_EndPos.x = posBase.x + cos(l_dAngleEN)*l_dLongEN*dScale; 
	m_EndPos.y = posBase.y + sin(l_dAngleEN)*l_dLongEN*dScale; 
	//�뾶�仯
	m_dRadius *= dScale;
}

/*
 *	ʵ�߱�����
 */
PGRAPHPOS CCreateRound::SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist)
{
	double l_dAngle = GetAngleToXAxis(m_Center,m_EndPos);
	PGRAPHPOS headGraphPos =  TurnArcDash(dLongSolid,dLongEmpty,m_Center,l_dAngle,l_dAngle,m_dRadius,m_iDirect);	
	return headGraphPos;
}





