// CreateLine.cpp: implementation of the CCreateLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "CreateBase.h"
#include "CAD_PowerLand.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCreateLine, CCreateBase, 0)
CCreateLine::CCreateLine()
{

}

CCreateLine::CCreateLine(const Position& begin,const Position& end)
{
	m_ColorOfLine = g_CurColor;		//����ʱ�Ļ�ɫ���ǵ�ǰ��ѡɫ
	m_Begin = begin;
	m_End = end;
	m_iKnifeStyle = 41;//�������� 41�󵶲� 42�ҵ���
	m_iIsBeginGraph = 2;//0����ԭʼͼ�Σ�1��ԭʼͼ�Σ�2�Ǳ���ʱ������
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
}

CCreateLine::~CCreateLine()
{

}

/*//������DGI+���ĺ���
void CCreateLine::Draw(CDC *pDC)
{	//����������ת����Ļ����
	CPoint  l_ScreemPtO,l_ScreemPtT;
	g_pView->WorldToScreem(m_Begin,l_ScreemPtO);
	g_pView->WorldToScreem(m_End,l_ScreemPtT);	
	//����GDI+�Ķ���
	Graphics myGraphics(pDC->m_hDC);
	myGraphics.TranslateTransform(m_dMoveX,m_dMoveY);
	Pen myPen(Color(0, 0 ,0),1); 
	myGraphics.DrawLine(&myPen,l_ScreemPtO.x,l_ScreemPtO.y,l_ScreemPtT.x,l_ScreemPtT.y); 
	myGraphics.TranslateTransform(0,0);
	GraphicsPath aaa;
	aaa.AddEllipse(100,100,200,300);
}*/


void CCreateLine::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{
	CPoint pt_begin, pt_end; // ��Ļ����������յ�
	// ����������ת��Ϊ��Ļ����
	g_pView->WorldToScreem(m_Begin,pt_begin); 
	g_pView->WorldToScreem(m_End,pt_end) ;
	
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
	// ������Ļ�������ͼԪ

	pDC->MoveTo(pt_begin); 
	pDC->LineTo(pt_end);
	if (FlagShowArrow)	//������ͼ�ͷ
	{
		//����ʼ��ķ���
		CRect rect(pt_begin.x-5,pt_begin.y-5,pt_begin.x+5,pt_begin.y+5);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
		
		double angle = GetAngleToXAxis(m_Begin,m_End);
		DrawArrow(angle,pt_end,pDC);//����ͷ�ĺ��� 
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
			if(this == (CCreateLine *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //�����Ϳ���͸����������				
				pDC->TextOut(pt_begin.x,pt_begin.y,str);//��������� ��
				pDC->SetBkMode(l_iOldMode);
				break;
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

int CCreateLine::IsPick(CPoint point)
{
	CPoint pt_begin, pt_end; // ��Ļ����������յ�
	Position pos;
	g_pView->ScreemToWorld(point,pos);	
	//������ű���
	double l_dProportion = g_pView->ReturnProportion();	
	//�����������ĩ��ľ����������ʰȡ�����൱��ĩ���Χ������
	if (pos.Distance(m_Begin)/l_dProportion<=PRECISION||pos.Distance(m_End)/l_dProportion<=PRECISION)
		return TRUE;
	// ����������ת��Ϊ��Ļ����
	g_pView->WorldToScreem(m_Begin,pt_begin); 
	g_pView->WorldToScreem(m_End,pt_end) ;
	//���߶ε�б��ҲҪ���ȥ����������б�ʵġ�
	double angle = GetAngleToXAxis(m_Begin,m_End);
	double d_Sin = PRECISION * sin(angle);
	double d_Cos = PRECISION * cos(angle);

	CRgn	pr;
	POINT	pt[4];//����������ĸ�����ɣ�
	pt[0].x = pt_begin.x - (int)d_Sin;
	pt[0].y = pt_begin.y - (int)d_Cos;//��ʼ���ϵ�һ��

	pt[1].x = pt_begin.x + (int)d_Sin;
	pt[1].y = pt_begin.y + (int)d_Cos;//��ʼ���µ�һ��
	
	pt[2].x = pt_end.x + (int)d_Sin;
	pt[2].y = pt_end.y + (int)d_Cos;//�յ��µ�һ��
	
	pt[3].x = pt_end.x - (int)d_Sin;
	pt[3].y = pt_end.y - (int)d_Cos;//�յ��ϵ�һ��
	
	pr.CreatePolygonRgn(pt,4,ALTERNATE);
	if(pr.PtInRegion(point)) 
	{
		pr.DeleteObject();
		return TRUE;
	}
	else

	{
		pr.DeleteObject();
		return -1;
	}
}

//�ƶ�
void CCreateLine::Move(const Position& basePos,const Position& desPos)
{
	m_Begin = m_Begin.Offset(desPos - basePos);
	m_End = m_End.Offset(desPos - basePos);
}
//��ת
void CCreateLine::Rotate(const Position& basePos, const double angle)
{
	m_Begin = m_Begin.Rotate(basePos, angle) ;
	m_End =m_End.Rotate(basePos, angle) ;
}
//����
void CCreateLine::Mirror(const Position& pos1, const Position& pos2)
{
	m_Begin = m_Begin.Mirror(pos1, pos2) ;
	m_End =m_End.Mirror(pos1, pos2) ;
}

/*
 *	���ԶԻ���
 */
void CCreateLine::PropertyView()
{
	//��ʾ���ԶԻ���
	(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 

	//�����ص�STATIC��Edit��ʾ����
	ShowControl(4,FALSE);
	//�趨Ҫ��ʾ����ɫ
	
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(RtnColorInt()-1);
	//����STATICҪ��ʾ�������յ㡱
	//����EditҪ��ʾ�����յ��λ��
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"���X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"���Y");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Start X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Start Y");
	}
	CString str;
	str.Format("%.4f",m_Begin.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
	str.Format("%.4f",m_Begin.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
	if (g_pDoc->m_stcWorkFor.IsChinese)
	{
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"�յ�X");
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"�յ�Y");
	}
	else
	{
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"End X");
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC4,"End Y");
	}
	str.Format("%.4f",m_End.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	str.Format("%.4f",m_End.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT4,str);
	////////////////////////����������Ŀ��Լ����ڵڼ�λ����˳ʱ��///////////////////////////////////
	POSITION pos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	for (int i=0;pos != NULL;i++)//��������i�ɱ�ʾ������������
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(pos);
		//�����M����
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(this == (CCreateLine *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
		{
			str.Format("%d",i);
			((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,str);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	
}

CCreateBase* CCreateLine::Copy()
{
	CCreateLine *pCopy = new CCreateLine(m_Begin,m_End);	//���Ƶ�
	pCopy->m_ColorOfLine = m_ColorOfLine;		//������ɫ
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}


/*
*	�������ԶԻ��򵯳��ı�ͼԪ����ʱҪ�ػ��ĵĺ���
*/
void CCreateLine::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	switch(WhichEdit) //�ĸ�EDIT����ˣ��ͱ��ĸ�ֵ
	{
	case 1:
		m_Begin.x = e;
		break;
	case 2:
		m_Begin.y = e;
		break;
	case 3:
		m_End.x = e;
		break;
	case 4:
		m_End.y = e;
		break;
	default:
		break;
	}
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);	
}

void CCreateLine::DrawMove(CDC* pDC,int DrawMode,BOOL Poly)
{
	CPoint pt_begin, pt_end; // ��Ļ����������յ�
	// ����������ת��Ϊ��Ļ����
	g_pView->WorldToScreem(m_Begin,pt_begin); 
	g_pView->WorldToScreem(m_End,pt_end) ;
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
	// ������Ļ�������ͼԪ
	pDC->MoveTo(pt_begin); 
	pDC->LineTo(pt_end);
	// �ָ�ԭ���Ļ��� 
	pDC->SelectObject(pOldPen); 
	// �ָ�ԭ���Ļ�ͼģʽ
	pDC->SetROP2(n);
	pen.DeleteObject();//�ͷ�pen��Create��;
}

/*
 *	��ѡʱ�Ƿ�ѡ��
 */
BOOL CCreateLine::IsSelect(CRgn *selectrect,BOOL bComplex)
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
		if (LineOnRect(l_posRgn1,l_posRgn2))
			return TRUE;
	}
	BOX2D pBox;		//��Χ�нṹ
	CPoint pos1,pos2;	//��Χ�е�������Ļ���㣨���ϣ����£�
	//�õ���Χ�е������㣬�������꣨���£����ϣ�	//�Ѱ�Χ������
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
BOOL CCreateLine::GetBox(BOX2D* pBox,BOOL IsOutside /* = FALSE */)
{
	double curRadius = g_pView->ReturnProportion()*PRECISION;
	//���û������
	if (IsOutside == FALSE)
		curRadius = 0.0;
	pBox->min[0] = min( m_Begin.x, m_End.x ) - curRadius;
	pBox->min[1] = min( m_Begin.y, m_End.y ) - curRadius;
	pBox->max[0] = max( m_Begin.x, m_End.x ) + curRadius;
	pBox->max[1] = max( m_Begin.y, m_End.y ) + curRadius;
	return TRUE;
}

//XY���Ź���
BOOL CCreateLine::GraphZoomXY(double dValueX, double dValueY)
{
	double dXRadio = 1;
	double dYRadio = 1;
	BOX2D pBox;		//��Χ�нṹ
	//�õ���Χ�е������㣬�������꣨���£����ϣ�	//�Ѱ�Χ������
	GetBox(&pBox);
	if (fabs(dValueX)>DISTANCE_ZERO)
	{
		dXRadio = pBox.max[0]-pBox.min[0];
	}
	if (fabs(dValueY)>DISTANCE_ZERO)
	{
		dYRadio = pBox.max[1]-pBox.min[1];
	}
	double d1=0;//��ǰ������ڶ�С��ľ����������ı�ֵ
	double d2=0;//���ӵľ���
	d1 = (m_Begin.x - pBox.min[0])/dXRadio;
	if (d1>1)//�����ֵΪ��ʱ
		d1 = 1;
	d2 = d1 * dValueX;
	m_Begin.x += d2;

	d1 = (m_Begin.y - pBox.min[1])/dYRadio;
	if (d1>1)
		d1 = 1;
	d2 = d1 * dValueY;
	m_Begin.y += d2;

	d1 = (m_End.x - pBox.min[0])/dXRadio;
	if (d1>1)
		d1 = 1;
	d2 = d1 * dValueX;
	m_End.x += d2;
	
	d1 = (m_End.y - pBox.min[1])/dYRadio;
	if (d1>1)
		d1 = 1;
	d2 = d1 * dValueY;
	m_End.y += d2;
	return TRUE;
}

/*
 *	����ͼ���ϵ�һ�����볤�ȷ���һ����
 */
BOOL CCreateLine::RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine /* = 0 */)
{
	//�������ĳ��ȴ���ֱ�ߵĳ��ȾͲ�����
	double l_dAngle;
	if (dLongPos > 0 && dLongPos < ClickPos.Distance(m_End))
	{
		l_dAngle = GetAngleToXAxis(m_Begin,m_End);
		posTarget.x = ClickPos.x + dLongPos*cos(l_dAngle);
		posTarget.y = ClickPos.y + dLongPos*sin(l_dAngle);
		return TRUE;
	}
	else if (dLongPos < 0 &&  fabs(dLongPos) < ClickPos.Distance(m_Begin))
	{
		dLongPos = fabs(dLongPos);
		l_dAngle = GetAngleToXAxis(m_Begin,m_End);
		posTarget.x = ClickPos.x - dLongPos*cos(l_dAngle);
		posTarget.y = ClickPos.y - dLongPos*sin(l_dAngle);
		return TRUE;
	}
	return FALSE;
}

/*
 *	�ı��и��
 */
void CCreateLine::ChangeDir()
{
	//�������յ㻻��λ��
	Position pos = m_Begin;
	m_Begin = m_End;
	m_End = pos;	
}

/*
 *	��ֱ���Ƿ�������н��㲢�ҽ����Ƿ���ֱ����
 */
BOOL CCreateLine::LineOnRect(Position pos1,Position pos3)
{
	Position pos2(pos1.x,pos3.y),pos4(pos3.x,pos1.y),l_posInter;
	if (GetIntersectPoint(m_Begin,m_End,pos1,pos2,l_posInter)&&PosOnLine(pos1,pos2,l_posInter)&&PosOnLine(m_Begin,m_End,l_posInter))
		return TRUE;
	else if (GetIntersectPoint(m_Begin,m_End,pos2,pos3,l_posInter)&&PosOnLine(pos2,pos3,l_posInter)&&PosOnLine(m_Begin,m_End,l_posInter))
		return TRUE;
	else if (GetIntersectPoint(m_Begin,m_End,pos3,pos4,l_posInter)&&PosOnLine(pos3,pos4,l_posInter)&&PosOnLine(m_Begin,m_End,l_posInter))
		return TRUE;
	else if (GetIntersectPoint(m_Begin,m_End,pos4,pos1,l_posInter)&&PosOnLine(pos4,pos1,l_posInter)&&PosOnLine(m_Begin,m_End,l_posInter))
		return TRUE;
	return FALSE;
}

/*
 *	����������
 */
void CCreateLine::GraphZoomScale(Position posBase,double dScale)
{
	//��������ĩ��ĽǶ�
	double l_dAngleBG = GetAngleToXAxis(posBase,m_Begin);
	double l_dAngleEN = GetAngleToXAxis(posBase,m_End);
	//���㵽��ĩ��ĳ���
	double l_dLongBG = posBase.Distance(m_Begin);
	m_Begin.x = posBase.x + cos(l_dAngleBG)*l_dLongBG*dScale; 
	m_Begin.y = posBase.y + sin(l_dAngleBG)*l_dLongBG*dScale; 
	//���㵽��ĩ��ĳ���
	double l_dLongEN = posBase.Distance(m_End);
	m_End.x = posBase.x + cos(l_dAngleEN)*l_dLongEN*dScale; 
	m_End.y = posBase.y + sin(l_dAngleEN)*l_dLongEN*dScale; 
}

/*
 *	ʵ�߱�����
 */
PGRAPHPOS CCreateLine::SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist)
{
	double l_dRemainingEmty =0.0;
	PGRAPHPOS headGraphPos =  TurnLineDash(dLongSolid,dLongEmpty,m_Begin,m_End);
	return headGraphPos;
}








