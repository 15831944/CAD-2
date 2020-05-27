// CreateEllipse.cpp: implementation of the CCreateEllipse class.
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
	IMPLEMENT_SERIAL(CCreateEllipse, CCreateBase, 0)
CCreateEllipse::CCreateEllipse()
{
		m_iFlagArcOrLine = -1;
		
}

CCreateEllipse::CCreateEllipse(const Position& begin,const Position& second,const Position& end)
{
	m_iFlagArcOrLine = -1;
	m_ColorOfLine = g_CurColor;		//����ʱ�Ļ�ɫ���ǵ�ǰ��ѡɫ
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_Begin = begin;
	m_End = end;
	m_Second = second;

	m_Center.x = (m_Begin.x+m_Second.x)/2;
	m_Center.y = (m_Begin.y+m_Second.y)/2;	//��Բ�����ĵ�����
	m_a = m_Begin.Distance(m_Center);	//���᳤
	m_b = m_End.Distance(m_Center);		//���᳤
	double c = sqrt(fabs((m_a*m_a)-(m_b*m_b)));		//���㳤��
	if (m_a>=m_b)
	{
		m_Focus1.x = c;
		m_Focus1.y = 0;
		m_Focus2.x = -c;
		m_Focus2.y = 0;
	}
	else
	{
		m_Focus1.x = 0;
		m_Focus1.y = c;
		m_Focus2.x = 0;
		m_Focus2.y = -c;
	}
	m_k = GetAngleToXAxis(m_Begin,m_Second);	//�����б�ʣ���Բ��ת�Ƕȣ�
	//��һ������
	m_Focus1 = m_Focus1.Offset(m_Center);
	m_Focus1 = m_Focus1.Rotate(m_Center,m_k);
	//�ڶ�������
	m_Focus2 = m_Focus2.Offset(m_Center);
	m_Focus2 = m_Focus2.Rotate(m_Center,m_k);
	//������ԲԲ���ϵĵ�
/*	Position pos(0,0);
	double angle200 = MATH_PI/100;
	for(double i=0;i<MATH_2PI;i=i+angle200)
	{
		GetEllipsesPoint(pos,i);	//������ԲԲ���ϵĵ㣨���ĵ���Բ���ϣ�
		pos = pos.Offset(m_Center);
		pos = pos.Rotate(m_Center,m_k);	
		m_point.Add(pos);
	}*/
/*
	Position pos(0,0);
	double angle3_6 = MATH_2PI/100;
	double l_dAngleBig = angle3_6*m_a/(m_a+m_b);//���Ƕ�
	double l_dAnglelit = angle3_6*m_b/(m_a+m_b);//��С�Ƕ�
	double l_dAngleAdd = (l_dAngleBig - l_dAnglelit)/49;
	double l_dAngleEll = 0.0;
	double l_dAngleAll = 0.0;
	for(int i=1;i<201;i++)
	{
		GetEllipsesPoint(pos,l_dAngleAll);	//������ԲԲ���ϵĵ㣨���ĵ���Բ���ϣ�
		pos = pos.Offset(m_Center);
		pos = pos.Rotate(m_Center,m_k);	
		m_point.Add(pos);	

		if(i<=50)
			l_dAngleEll = l_dAnglelit + (l_dAngleAdd*(i-1));
		else if (i>50&&i<=100)
			l_dAngleEll = l_dAngleBig - (l_dAngleAdd*(i-51));
		else if(i>100&&i<=150)
			l_dAngleEll = l_dAnglelit + (l_dAngleAdd*(i-101));
		else
			l_dAngleEll = l_dAngleBig - (l_dAngleAdd*(i-151));
		
		l_dAngleAll = l_dAngleAll+l_dAngleEll;

	}
*/
	Position pos(0,0);
	double angle3_6 = MATH_2PI/100;
	double l_dAngleBig = angle3_6*m_a*m_a*m_a*m_a/(m_a*m_a*m_a*m_a+m_b*m_b*m_b*m_b);//���Ƕ�
	double l_dAnglelit = angle3_6*m_b*m_b*m_b*m_b/(m_a*m_a*m_a*m_a+m_b*m_b*m_b*m_b);//��С�Ƕ�
	double l_dAngleAdd = (l_dAngleBig - l_dAnglelit)/49;
	double l_dAngleEll = 0.0;
	double l_dAngleAll = 0.0;
	for(int i=1;i<202;i++)
	{
		GetEllipsesPoint(pos,l_dAngleAll);	//������ԲԲ���ϵĵ㣨���ĵ���Բ���ϣ�
		pos = pos.Offset(m_Center);
		pos = pos.Rotate(m_Center,m_k);	
		m_point.Add(pos);	
		
		if(i<=50)
			l_dAngleEll = l_dAnglelit + (l_dAngleAdd*(i-1));
		else if (i>50&&i<=100)
			l_dAngleEll = l_dAngleBig - (l_dAngleAdd*(i-51));
		else if(i>100&&i<=150)
			l_dAngleEll = l_dAnglelit + (l_dAngleAdd*(i-101));
		else if(i>150)
			l_dAngleEll = l_dAngleBig - (l_dAngleAdd*(i-151));
		
		l_dAngleAll = l_dAngleAll+l_dAngleEll;
		
	}
	//��͹�ȵ�ֵ����ȥ
	int l_NumOfm_point = m_point.GetSize();
	double aaa = 0.0;
	for(int k = 0;k<l_NumOfm_point;k++)
		m_dTuDu.Add(aaa);
	m_iKnifeStyle = 41;//�������� 41�󵶲� 42�ҵ���
	m_iIsBeginGraph = 2;//0����ԭʼͼ�Σ�1��ԭʼͼ�Σ�2�Ǳ���ʱ������
}

CCreateEllipse::~CCreateEllipse()
{
	m_point.RemoveAll();
}


void CCreateEllipse::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{	
	CPoint pt_begin;// ��Ļ�����
	Position pt;
	double angle;//������X��ĽǶ�
	
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
	
	for(int i=0;i<m_point.GetSize();i++)
	{
		pt=m_point.GetAt(i); 
		// ����������ת��Ϊ��Ļ����
		g_pView->WorldToScreem(pt,pt_begin); 
		// �õ�ԭ���Ļ�ͼģʽ
		// ������Ļ�������ͼԪ
		if (i!=0)
		{
			pDC->LineTo(pt_begin); 
			if (FlagShowArrow&&i==200)//�Ƿ���ʾ��ͷ
			{
				angle = GetAngleToXAxis(m_PrePos,pt);
				DrawArrow(angle,pt_begin,pDC);//����ͷ�ĺ��� 
			}
		}
		else
		{
			if (FlagShowArrow)//�Ƿ���ʾ����
			{
				CRect rect(pt_begin.x-5,pt_begin.y-5,pt_begin.x+5,pt_begin.y+5);
				pDC->SelectStockObject(NULL_BRUSH);
				pDC->Rectangle(rect);
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
					if(this == (CCreateEllipse *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
					{
						CString str;
						str.Format("%d",i-l_iNumOfMCode);
						int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //�����Ϳ���͸����������				
						pDC->TextOut(pt_begin.x,pt_begin.y,str);//��������� ��
						pDC->SetBkMode(l_iOldMode);
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			pDC->MoveTo(pt_begin);
		}
		m_PrePos = pt;
	}
	// �ָ�ԭ���Ļ��� 
	pDC->SelectObject(pOldPen); 
	// �ָ�ԭ���Ļ�ͼģʽ
	pDC->SetROP2(n);
	pen.DeleteObject();//�ͷ�pen��Create��;
}

int CCreateEllipse::IsPick(CPoint point)
{
	Position Pos1,Pos2;// ���������
	CPoint pt_pos1,pt_pos2;// ��Ļ�����
	for(int i=0;i<m_point.GetSize()-1;i++)
	{
		Pos1=m_point.GetAt(i); 
		Pos2=m_point.GetAt(i+1); 
		// ��������ת��Ϊ��Ļ����
		g_pView->WorldToScreem(Pos1,pt_pos1); 
		g_pView->WorldToScreem(Pos2,pt_pos2); 
		//���߶ε�б��ҲҪ���ȥ����������б�ʵġ�
		double angle = GetAngleToXAxis(Pos1,Pos2);
		double d_Sin = PRECISION * sin(angle);
		double d_Cos = PRECISION * cos(angle);
		
		CRgn	pr;
		POINT	pt[4];//����������ĸ�����ɣ�
		pt[0].x = pt_pos1.x - (int)d_Sin;
		pt[0].y = pt_pos1.y - (int)d_Cos;//��ʼ���ϵ�һ��
		
		pt[1].x = pt_pos1.x + (int)d_Sin;
		pt[1].y = pt_pos1.y + (int)d_Cos;//��ʼ���µ�һ��
		
		pt[2].x = pt_pos2.x + (int)d_Sin;
		pt[2].y = pt_pos2.y + (int)d_Cos;//�յ��µ�һ��
		
		pt[3].x = pt_pos2.x - (int)d_Sin;
		pt[3].y = pt_pos2.y - (int)d_Cos;//�յ��ϵ�һ��
		
		pr.CreatePolygonRgn(pt,4,ALTERNATE);
		if(pr.PtInRegion(point)) 
		{
			pr.DeleteObject();
			return i;
			break;
		}
		pr.DeleteObject();
	}
	return -1;
/*		����ķ���
	Position pos;
	g_pView->ScreemToWorld(point,pos);
	double l_dLong = m_Focus1.Distance(pos)+m_Focus2.Distance(pos) ;//�㵽������ľ����;
	double l_dIsPick;	//ʰȡ�ľ���
	if(m_a>m_b)
		l_dIsPick = fabs(l_dLong - 2*m_a)/g_pView->ReturnProportion();	
	else
		l_dIsPick = fabs(l_dLong - 2*m_b)/g_pView->ReturnProportion();
	if (l_dIsPick<PRECISION)
		return TRUE;
	return FALSE;*/

}

CCreateBase* CCreateEllipse::Copy()
{
	CCreateEllipse *pCopy = new CCreateEllipse(m_Begin,m_Second,m_End);//���Ƶ�
	pCopy->m_ColorOfLine = m_ColorOfLine;		//������ɫ
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	pCopy->m_a = m_a;
	pCopy->m_b = m_b;
	pCopy->m_Center = m_Center;
	pCopy->m_k = m_k;
	pCopy->m_Focus1 = m_Focus1;
	pCopy->m_Focus2 = m_Focus2;
	return pCopy;
}

//�ƶ�
void CCreateEllipse::Move(const Position& basePos,const Position& desPos)
{
	for (int i=0;i < m_point.GetSize();i++)
	{
		Position pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Offset(desPos - basePos));	
		Position pt1 = m_point.ElementAt(i);
	}
	m_Begin = m_Begin.Offset(desPos - basePos);
	m_End = m_End.Offset(desPos - basePos);
	m_Second = m_Second.Offset(desPos - basePos);
	m_Center = m_Center.Offset(desPos - basePos);
	m_Focus1 = m_Focus1.Offset(desPos - basePos);
	m_Focus2 = m_Focus2.Offset(desPos - basePos);
}
//��ת
void CCreateEllipse::Rotate(const Position& basePos, const double angle)
{
	for (int i=0;i < m_point.GetSize();i++)
	{
		Position pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Rotate(basePos,angle));	
		Position pt1 = m_point.ElementAt(i);
	}
	m_Begin = m_Begin.Rotate(basePos, angle) ;
	m_End =m_End.Rotate(basePos, angle) ;
	m_Second =m_Second.Rotate(basePos, angle) ;
	m_Center = m_Center.Rotate(basePos,angle);
	m_Focus1 =m_Focus1.Rotate(basePos, angle) ;
	m_Focus2 =m_Focus2.Rotate(basePos, angle) ;
}
//����
void CCreateEllipse::Mirror(const Position& pos1, const Position& pos2)
{
	for (int i=0;i < m_point.GetSize();i++)
	{
		Position pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Mirror(pos1,pos2));	
		Position pt1 = m_point.ElementAt(i);
	}
	m_Begin = m_Begin.Mirror(pos1, pos2) ;
	m_End =m_End.Mirror(pos1, pos2) ;
	m_Second =m_Second.Mirror(pos1, pos2) ;
	m_Center = m_Center.Mirror(pos1,pos2);
	m_Focus1 =m_Focus1.Mirror(pos1, pos2) ;
	m_Focus2 = m_Focus2.Mirror(pos1,pos2);
}

//���ݽǶ�������
int CCreateEllipse::GetArcQuadrant(double &fArc)
{
	UINT nIndex;
	if(fArc<0)
		fArc += MATH_2PI;
	if(fArc>MATH_2PI)
		fArc -= MATH_2PI;
	if(fArc<=(MATH_PI/2))			nIndex=0;
	else if(fArc<=(MATH_PI))			nIndex=1;
	else if(fArc<=(MATH_PI*1.5f))	nIndex=2;
	else							nIndex=3;
	return nIndex;
}

//��ȡ��Բ���϶�Ӧ�Ƕ�ΪfArc��һ���������
void CCreateEllipse::GetEllipsesPoint(Position &output, double fArc)
{
	double fTan, fFx, fFy;
	UINT nQuadrant;
	
	if(fArc<0)
		fArc += MATH_2PI;
	if(fArc>MATH_2PI)
		fArc -= MATH_2PI;
	
   	nQuadrant = GetArcQuadrant(fArc);
	switch(nQuadrant)
	{
	case 0:
		fArc = fArc;
		fFx = 1;	fFy = 1;
		break;
	case 1:
		fArc = MATH_PI-fArc;
		fFx = -1;	fFy = 1;
		break;
	case 2:
		fArc = fArc-MATH_PI;
		fFx = -1;	fFy = -1;
		break;
	case 3:
		fArc = MATH_2PI-fArc;
		fFx = 1;	fFy = -1;
		break;
	default:
		break;
	}
	
	fTan = tan(fArc);
	
	output.x = (m_a*m_b)/(sqrt((m_b*m_b)+(m_a*m_a)*(fTan*fTan)));
	output.y = output.x*fTan;
	output.x *= fFx;
	output.y *= fFy;
}


/*
*	���ԶԻ���
*/
void CCreateEllipse::PropertyView()
{
	//��ʾ���ԶԻ���
	(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 
	
	//�����ص�STATIC��Edit��ʾ����
	ShowControl(3,TRUE);
	
	//�趨Ҫ��ʾ����ɫ	
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(RtnColorInt()-1);
	
	//����STATICҪ��ʾ�������յ㡱
	//����EditҪ��ʾ�����յ��λ��
	//���Ļ���Ӣ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"����");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"����X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"����Y");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"Vertex");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Vertex X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC3,"Vertex Y");
	}
	CString str;
	Position pos = m_point.GetAt(0);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,"1");
	str.Format("%.4f",pos.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
	str.Format("%.4f",pos.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT3,str);
	//�ö�������1��ʼ
	((CMainFrame*)AfxGetMainWnd())->m_iNumOfTop = 1;
	////////////////////////����������Ŀ��Լ����ڵڼ�λ����˳ʱ��///////////////////////////////////
	POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	for (int i=0;l_ListPos != NULL;i++)//��������i�ɱ�ʾ������������
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
		//�����M����
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(this == (CCreateEllipse *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
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
void CCreateEllipse::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	Position pos;
	switch(WhichEdit) //�ĸ�EDIT����ˣ��ͱ��ĸ�ֵ
	{
	case 2:
		pos = m_point.GetAt(iNumOfTop-1);
		pos.x = e;
		m_point.SetAt(iNumOfTop-1,pos);
		break;
	case 3:
		pos = m_point.GetAt(iNumOfTop-1);
		pos.y = e;
		m_point.SetAt(iNumOfTop-1,pos);
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
BOOL CCreateEllipse::IsSelect(CRgn *selectrect,BOOL bComplex)
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
		if (EllipseOnRect(l_posRgn1,l_posRgn2))
			return TRUE;
	}
	BOX2D pBox;		//��Χ�нṹ
	CPoint pos1,pos2;	//��Χ�е�������Ļ���㣨���ϣ����£�
	//�����ǵõ���Χ�е������㣬�������꣨���£����ϣ�
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
BOOL CCreateEllipse::GetBox(BOX2D *pBox,BOOL IsOutside /* = FALSE */)
{
	Position CurPos,PrePosMin,PrePosMax;//��ǰ��͹�ȥ��
	PrePosMin=m_point.GetAt(0); 
	PrePosMax=m_point.GetAt(0); 
	for(int i=0;i<m_point.GetSize()-1;i++)
	{
		CurPos=m_point.GetAt(i+1);
		//ȡ���е�����С�ĺ�����
		PrePosMin.x = min( PrePosMin.x, CurPos.x );
		PrePosMin.y = min( PrePosMin.y, CurPos.y );
		PrePosMax.x = max( PrePosMax.x, CurPos.x );
		PrePosMax.y = max( PrePosMax.y, CurPos.y );
	}
	//�Ѱ�Χ������
	double curRadius = g_pView->ReturnProportion()*PRECISION;
	//���û������
	if (IsOutside == FALSE)
	curRadius = 0.0;
	pBox->min[0] = PrePosMin.x - curRadius;
	pBox->min[1] = PrePosMin.y - curRadius;
	pBox->max[0] = PrePosMax.x + curRadius;
	pBox->max[1] = PrePosMax.y + curRadius;
	return TRUE;
}

//XY���Ź���
BOOL CCreateEllipse::GraphZoomXY(double dValueX, double dValueY)
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
	Position pos1(0,0);
	int l_iSizeOfPoint = m_point.GetSize();
	for(int i=0;i<l_iSizeOfPoint;i++)
	{
		pos1 = m_point.GetAt(i);
		d1 = (pos1.x - pBox.min[0])/dXRadio;
		if (d1>1)//�����ֵΪ��ʱ
			d1 = 1;
		d2 = d1 * dValueX;
		pos1.x += d2;
		
		d1 = (pos1.y - pBox.min[1])/dYRadio;
		if (d1>1)
			d1 = 1;
		d2 = d1 * dValueY;
		pos1.y += d2;
		
		m_point.SetAt(i, pos1);
	}	
	return TRUE;
}


/*
 *	����ͼ���ϵ�һ�����볤�ȷ���һ����
 */
BOOL CCreateEllipse::RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine /* = 0 */)
{
	//���ص�����������
	CPoint pot;
	g_pView->WorldToScreem(ClickPos,pot);
	int l_iNumClick = this->IsPick(pot);
	//���			�յ�			Բ��
	Position l_posBegin,l_posEnd,l_posCenter;
	//͹��
	double l_dTudu,l_dAngle,l_dFuLongPos = -dLongPos;
	int l_iSize = m_point.GetSize();
	for(int i = l_iNumClick;i<l_iSize-1&&i>=0;)
	{
		l_dTudu = m_dTuDu.GetAt(i);
		l_posBegin = m_point.GetAt(i);		
		l_posEnd = m_point.GetAt(i+1);
		//ֱ��
		if (l_dTudu == 0.0)
		{
			if (dLongPos>0)//����Ĵ���0˵���Ǿ�ĩ��ľ���
			{
				if(ClickPos.Distance(l_posEnd)<dLongPos)//�������ıȱ���ֱ�߳���Ҫ��ȥ������
				{
					dLongPos = dLongPos - ClickPos.Distance(l_posEnd);
					ClickPos = l_posEnd;
					i++;
				}
				else//�������
				{
					//ֱ��������յ�ĽǶ� 
					l_dAngle = GetAngleToXAxis(l_posBegin,l_posEnd);
					posTarget.x = ClickPos.x + dLongPos*cos(l_dAngle);
					posTarget.y = ClickPos.y + dLongPos*sin(l_dAngle);
					return TRUE;
				}
			}
			else//�����С��0˵���Ǿ����ľ���    (dLongPos<0)
			{
					
				if(ClickPos.Distance(l_posBegin)<l_dFuLongPos)//�������ıȱ���ֱ�߳���Ҫ��ȥ������
				{
					l_dFuLongPos = l_dFuLongPos - ClickPos.Distance(l_posBegin);
					ClickPos = l_posBegin;
					i--;
				}
				else//�������
				{
					//ֱ��������յ�ĽǶ� 
					l_dAngle = GetAngleToXAxis(l_posBegin,l_posEnd);
					posTarget.x = ClickPos.x - l_dFuLongPos*cos(l_dAngle);
					posTarget.y = ClickPos.y - l_dFuLongPos*sin(l_dAngle);	
					return TRUE;
				}
			}
		}
		else//Բ�� 
		{
			double l_dLongArc;//����
			if (dLongPos>0)//����Ĵ���0˵���Ǿ�ĩ��ľ���
			{
				l_dLongArc = RtnLongArc(ClickPos,l_posBegin,l_posEnd,l_dTudu);//����
				if(l_dLongArc < dLongPos)//�������ıȱ��λ��߳���Ҫ��ȥ������
				{
					dLongPos = dLongPos - l_dLongArc;
					ClickPos = l_posEnd;
					i++;
				}
				else//�������
				{
					posTarget = RtnLongArcPos(l_posBegin,l_posEnd,l_dTudu,ClickPos,dLongPos);
					return TRUE;
				}
			}
			else//�����С��0˵���Ǿ����ľ���    (dLongPos<0)
			{
				l_dLongArc = RtnLongArc(ClickPos,l_posEnd,l_posBegin,-l_dTudu);//����				
				if(l_dLongArc < l_dFuLongPos)//�������ıȱ���ֱ�߳���Ҫ��ȥ������
				{
					l_dFuLongPos = l_dFuLongPos - l_dLongArc;
					ClickPos = l_posBegin;
					i--;
				}
				else//�������
				{
					posTarget = RtnLongArcPos(l_posEnd,l_posBegin,-l_dTudu,ClickPos,l_dFuLongPos);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

/*
 *	��������յ�͹�ȷ��ػ���
 */
double CCreateEllipse::RtnLongArc(Position posClick,Position posBegin,Position PosEnd,double dTudu)
{
	//��Բ��
	Position l_posCenter;
	g_pDoc->polylineToArc(posBegin,PosEnd,dTudu,l_posCenter);
	//��뾶
	double l_dRadius = posBegin.Distance(l_posCenter);
	//���ܳ�
	double l_dPerimeter = 2*MATH_PI*l_dRadius;
	//��н�
	double l_dAngleInclude,l_dLongArc,l_dAngleStart,l_dAngleEnd;
	//��ʼ�ǣ���ֹ��
	l_dAngleStart = GetAngleToXAxis(l_posCenter,posBegin);
	l_dAngleEnd = GetAngleToXAxis(l_posCenter,PosEnd);
	l_dLongArc = GetAngleToXAxis(l_posCenter,posClick);
	if (dTudu<0.0)//˳ʱ��
	{
		if (l_dLongArc<l_dAngleEnd)//�������ʱ������X��������
			l_dAngleInclude = l_dLongArc +MATH_2PI - l_dAngleEnd;		
		else
			l_dAngleInclude = l_dLongArc - l_dAngleEnd;
	}
	else//��ʱ��
	{
		if (l_dLongArc>l_dAngleEnd)//�������ʱ������X��������
			l_dAngleInclude = l_dAngleEnd +MATH_2PI - l_dLongArc;		
		else
			l_dAngleInclude = l_dAngleEnd - l_dLongArc;		
	}	
	return (l_dAngleInclude/MATH_2PI)*l_dPerimeter;
}

/*
 *	������㣬�յ㣬͹�ȣ�����㣬���������ػ��ϵ�
 *	�������ʱҪע�⣬�Ƕ���Բ��������յ�ͷ���
 */
Position CCreateEllipse::RtnLongArcPos(Position posBegin,Position PosEnd,double dTudu,Position posClick,double dLongPos)
{
	//��Բ��
	Position l_posCenter,posTarget(0,0);
	g_pDoc->polylineToArc(posBegin,PosEnd,dTudu,l_posCenter);
	//�뾶
	double l_dRadius = l_posCenter.Distance(posBegin);
	//Բ�ĵ�������ĽǶ�
	double l_dAngle = GetAngleToXAxis(l_posCenter,posClick);
	double l_dLongArc,l_dAngle2,l_dAngleStart,l_dAngleEnd;
	//��ʼ�ǣ���ֹ��
	l_dAngleStart = GetAngleToXAxis(l_posCenter,posBegin);
	l_dAngleEnd = GetAngleToXAxis(l_posCenter,PosEnd);
	//Բ�����ܳ�
	double l_dPerimeter = 2*MATH_PI*l_dRadius;
	//�������ĳ��ȴ���Բ���ĳ��ȾͲ�����
	if (dLongPos > 0)//�ж���ĩ��ľ���
	{
		if (dTudu < 0.0)//˳ʱ�� 
		{	//ĩ��͵����������ߵ�����
			if (l_dAngleStart<l_dAngleEnd&&l_dAngle<l_dAngleEnd)//ĩ�������߲��ҵ��û�������
			{
				l_dLongArc = ((l_dAngle+MATH_2PI-l_dAngleEnd)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle+MATH_2PI-((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
			else//ĩ���������������һ��
			{
				l_dLongArc = ((l_dAngle-l_dAngleEnd)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle-((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
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
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
			else//ĩ���������������һ��
			{
				l_dLongArc = ((l_dAngleEnd-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = ((dLongPos/l_dPerimeter)*MATH_2PI)+l_dAngle;//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
		}
	}
	else if (dLongPos < 0 )
	{
		dLongPos = fabs(dLongPos);			
		if (dTudu < 0.0)//˳ʱ�� 
		{	//���͵����������ߵ�����
			if (l_dAngleStart<l_dAngleEnd&&l_dAngle>l_dAngleStart)//��������߲��ҵ��û�������
			{
				l_dLongArc = ((l_dAngleStart+MATH_2PI-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle-MATH_2PI+((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
			else//����������������һ��
			{
				l_dLongArc = ((l_dAngleStart-l_dAngle)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle+((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
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
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
			else//����������������һ��
			{
				l_dLongArc = ((l_dAngle-l_dAngleStart)/MATH_2PI)*l_dPerimeter;
				if (l_dLongArc > dLongPos)//�������ĳ������㻡��
				{
					l_dAngle2 = l_dAngle-((dLongPos/l_dPerimeter)*MATH_2PI);//���ȶԾ͵ĽǶ�
					//�󻡳���Ӧ�нǶ�
					posTarget.x = l_posCenter.x + l_dRadius*cos(l_dAngle2);
					posTarget.y = l_posCenter.y + l_dRadius*sin(l_dAngle2);	
					return posTarget;
				}
				else
					return posTarget;	
			}
		}
	}
	return posTarget;
}

/*
 *	�ı��и��
 */
void CCreateEllipse::ChangeDir()
{
	Position l_ScreemPos;
	CArray<Position,Position&> l_ListPoint; 
	for(int i=m_point.GetSize();  i>0;  i--)
	{
		l_ScreemPos = m_point.GetAt(i-1); 
		l_ListPoint.Add(l_ScreemPos);			
	}
	m_point.Copy(l_ListPoint);	
}

/*
 *	����Բ�Ƿ�������н��㲢�ҽ����Ƿ���ֱ����
 */
BOOL CCreateEllipse::EllipseOnRect(Position pos1,Position pos3)
{
	Position pos2(pos1.x,pos3.y),pos4(pos3.x,pos1.y),l_posInter;
	Position l_posBegin,l_posEnd;
	int l_iEllipseOfSize = m_point.GetSize();
	for(int i=0;i<l_iEllipseOfSize-1;i++)
	{	
		l_posBegin = m_point.GetAt(i);
		l_posEnd = m_point.GetAt(i+1);
		if (GetIntersectPoint(l_posBegin,l_posEnd,pos1,pos2,l_posInter)&&PosOnLine(pos1,pos2,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
			return TRUE;
		else if (GetIntersectPoint(l_posBegin,l_posEnd,pos2,pos3,l_posInter)&&PosOnLine(pos2,pos3,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
			return TRUE;
		else if (GetIntersectPoint(l_posBegin,l_posEnd,pos3,pos4,l_posInter)&&PosOnLine(pos3,pos4,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
			return TRUE;
		else if (GetIntersectPoint(l_posBegin,l_posEnd,pos4,pos1,l_posInter)&&PosOnLine(pos4,pos1,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
			return TRUE;
	}
	return FALSE;
}

/*
*	����������
*/
void CCreateEllipse::GraphZoomScale(Position posBase,double dScale)
{
	//�㼯�Ĵ�С 
	int l_iSize = m_point.GetSize();
	//ÿ����
	Position posCur;
	//��������,�Ƕ�,	 ����
	double l_dAnglePos,l_dLongPos;
	for(int i=0;i<l_iSize;i++)
	{
		posCur = m_point.GetAt(i);
		l_dAnglePos = GetAngleToXAxis(posBase,posCur);
		l_dLongPos = posBase.Distance(posCur);
		//�γɵ��µ�
		posCur.x = posBase.x + cos(l_dAnglePos)*l_dLongPos*dScale; 
		posCur.y = posBase.y + sin(l_dAnglePos)*l_dLongPos*dScale; 
		m_point.SetAt(i,posCur);		
	}
}

/*
*	ʵ�߱�����
*/
PGRAPHPOS CCreateEllipse::SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist)
{
	int l_iSize = m_point.GetSize()-1;
	double l_dTudu = 0.0,l_dRadius,l_dAngleStart,l_dAngleEnd,l_dRemainingEmty =0.0;
	Position l_posBegin,l_posEnd,l_posCenter;//����յ�
	PGRAPHPOS headGraphPos = NULL;//�����߶εļ�
	for(int i=0;i<l_iSize;i++)
	{
		l_dTudu = m_dTuDu.GetAt(i);
		l_posBegin = m_point.GetAt(i);
		l_posEnd = m_point.GetAt(i+1);
		PGRAPHPOS GraphPos;//���߶εļ�
		if(l_dTudu == 0.0)//ֱ��
		{
			GraphPos = TurnLineDash(dLongSolid,dLongEmpty,l_posBegin,l_posEnd);			
		}
		else//Բ��
		{
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);
			l_dAngleStart = GetAngleToXAxis(l_posCenter,l_posBegin)/MATH_PI*180;
			l_dAngleEnd = GetAngleToXAxis(l_posCenter,l_posEnd)/MATH_PI*180;
			l_dRadius = l_posCenter.Distance(l_posBegin);
			GraphPos = TurnArcDash(dLongSolid,dLongEmpty,l_posCenter,l_dAngleStart,l_dAngleEnd,l_dRadius,RtnArcDir(l_dTudu));			
		}
		//���ͼ��
		if(GraphPos != NULL)//���û��ͼ�����ɾͲ���
		{
			if (headGraphPos == NULL)
				headGraphPos = GraphPos;
			else
				g_pDoc->AddTail(headGraphPos,GraphPos);
		}
	}
	return headGraphPos;
}
