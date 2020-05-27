// CreateRectangle.cpp: implementation of the CCreateRectangle class.
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
IMPLEMENT_SERIAL(CCreateRectangle, CCreateBase, 0)
CCreateRectangle::CCreateRectangle()
{
	
}

//���޸ĵ���Щ����ʹ�ã�Copy()����ʹ�á�
CCreateRectangle::CCreateRectangle(CArray<Position,Position&>&  point,CArray<double,double&>& TuDu)
{
	m_ColorOfLine = g_CurColor;		//����ʱ�Ļ�ɫ���ǵ�ǰ��ѡɫ
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_point.Copy(point);
	m_dTuDu.Copy(TuDu);
	m_iKnifeStyle = 41;//�������� 41�󵶲� 42�ҵ���
	m_iIsBeginGraph = 2;//0����ԭʼͼ�Σ�1��ԭʼͼ�Σ�2�Ǳ���ʱ������
	m_iFlagArcOrLine = -1;
	
}

//����ͼ������ͼԪʱʹ�á�
CCreateRectangle::CCreateRectangle(const Position& begin,const Position& end,BOOL FlagRoundRect,double Radius)
{
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	if (FlagRoundRect == FALSE||Radius < 0.001)//����Բ�Ǿ���
	{
		m_ColorOfLine = g_CurColor;		//����ʱ�Ļ�ɫ���ǵ�ǰ��ѡɫ
		Position pos1,pos2,pos3,pos4;
		pos1 = begin;
		pos3 = end;
		if((begin.x<=end.x&&begin.y>=end.y)||(begin.x > end.x&&begin.y<=end.y))//�����Ͻǵ����½�
		{
			pos2.x = begin.x;
			pos2.y = end.y;
			pos4.x = end.x;
			pos4.y = begin.y;
		}			
		else if((begin.x <= end.x&&begin.y<=end.y)||(begin.x >= end.x&&begin.y>=end.y))//�����½ǵ����Ͻ�
		{
			pos2.x = end.x;
			pos2.y = begin.y;
			pos4.x = begin.x;
			pos4.y = end.y;			
		}
		m_point.Add(pos1);
		m_point.Add(pos2);
		m_point.Add(pos3);
		m_point.Add(pos4);
		m_point.Add(pos1);
		//��͹�ȵ�ֵ����ȥ
		int l_NumOfm_point = m_point.GetSize();
		double aaa = 0.0;
		for(int i = 0;i<l_NumOfm_point;i++)
			m_dTuDu.Add(aaa);
	}
	else//Բ�Ǿ���
	{
		m_ColorOfLine = g_CurColor;		//����ʱ�Ļ�ɫ���ǵ�ǰ��ѡɫ
		Position pos,posb(0,10),posc(10,10),pose(10,0);
		double aaa = g_pDoc->RtnTudu(posb,pose,posc,AD_COUNTERCLOCKWISE);
		double zoer = 0.0;
		if(begin.x<end.x&&begin.y>end.y)//�����Ͻǵ����½�
		{
			pos.x = begin.x;
			pos.y = begin.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x;
			pos.y = end.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x + Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x - Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x;
			pos.y = end.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x;
			pos.y = begin.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x - Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x + Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x;
			pos.y = begin.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
		}
		else if(begin.x < end.x&&begin.y<end.y)//�����½ǵ����Ͻ�
		{
			pos.x = begin.x + Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x - Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x;
			pos.y = begin.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x;
			pos.y = end.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x - Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x + Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x;
			pos.y = end.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x;
			pos.y = begin.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x + Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
		}
		else if(begin.x > end.x&&begin.y<end.y)//�����½ǵ����Ͻ�
		{
			pos.x = begin.x;
			pos.y = begin.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x;
			pos.y = end.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x - Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x + Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x;
			pos.y = end.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x;
			pos.y = begin.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x + Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x - Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x;
			pos.y = begin.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
		}
		else if(begin.x > end.x&&begin.y>end.y)//�����½ǵ����Ͻ�
		{
			pos.x = begin.x - Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x + Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x;
			pos.y = begin.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = end.x;
			pos.y = end.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = end.x + Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x - Radius;
			pos.y = end.y;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x;
			pos.y = end.y + Radius;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
			pos.x = begin.x;
			pos.y = begin.y - Radius;
			m_point.Add(pos);
			m_dTuDu.Add(aaa);
			
			pos.x = begin.x - Radius;
			pos.y = begin.y;
			m_point.Add(pos);
			m_dTuDu.Add(zoer);
		}
	}
	m_iKnifeStyle = 41;//�������� 41�󵶲� 42�ҵ���
	m_iIsBeginGraph = 2;//0����ԭʼͼ�Σ�1��ԭʼͼ�Σ�2�Ǳ���ʱ������	
	m_iFlagArcOrLine = -1;
	
}

CCreateRectangle::~CCreateRectangle()
{
	m_point.RemoveAll();
	m_dTuDu.RemoveAll();
}

/*
void CCreateRectangle::Draw(CDC *pDC)
{
	CPoint  l_ScreemPtO,l_ScreemPtT;
	g_pView->WorldToScreem(m_Begin,l_ScreemPtO);
	g_pView->WorldToScreem(m_End,l_ScreemPtT);
	Graphics myGraphics(pDC->m_hDC);
	Pen myPen(Color(0, 0 ,0),1); 
	
	if (l_ScreemPtT.x > l_ScreemPtO.x  &&  l_ScreemPtT.y < l_ScreemPtO.y)//��һ������
		myGraphics.DrawRectangle(&myPen,l_ScreemPtO.x,l_ScreemPtT.y,
		l_ScreemPtT.x-l_ScreemPtO.x,l_ScreemPtO.y-l_ScreemPtT.y); 

	if (l_ScreemPtT.x > l_ScreemPtO.x  &&  l_ScreemPtT.y > l_ScreemPtO.y)//����������
		myGraphics.DrawRectangle(&myPen,l_ScreemPtO.x,l_ScreemPtO.y,
		l_ScreemPtT.x-l_ScreemPtO.x,l_ScreemPtT.y-l_ScreemPtO.y); 

	if (l_ScreemPtT.x < l_ScreemPtO.x  &&  l_ScreemPtT.y > l_ScreemPtO.y)//����������
		myGraphics.DrawRectangle(&myPen,l_ScreemPtT.x,l_ScreemPtO.y,
		l_ScreemPtO.x-l_ScreemPtT.x,l_ScreemPtT.y-l_ScreemPtO.y); 
	
	if (l_ScreemPtT.x < l_ScreemPtO.x  &&  l_ScreemPtT.y < l_ScreemPtO.y)//���������
		myGraphics.DrawRectangle(&myPen,l_ScreemPtT.x,l_ScreemPtT.y,
		l_ScreemPtO.x-l_ScreemPtT.x,l_ScreemPtO.y-l_ScreemPtT.y); 
	


}*/


void CCreateRectangle::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{	//��ǰ��     ǰһ��
	Position l_posCurPos,l_posNextPos;
	double l_dTuDu;
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
	int l_iSizeOfPos = m_point.GetSize();//����߹��ж��ٸ���
	int l_iStyleFlag = LINE;	//ͼ�����͡�1 Բ����2 ֱ�ߡ�
	for(int i=0;i<l_iSizeOfPos-1;i++)
	{
		//��õ�ǰ�����һ��
		l_posCurPos = m_point.GetAt(i); 
		l_posNextPos = m_point.GetAt(i+1);
		//���͹��
		l_dTuDu = m_dTuDu.GetAt(i);						
		// �õ�ԭ���Ļ�ͼģʽ
		// ������Ļ�������ͼԪ
		DrawLineOrArc(pDC,i,FlagShowArrow,FlagShowNum,l_posCurPos,l_posNextPos,l_dTuDu);
	}
	//////////////////////////////////////////////////////////////////////////	
	// �ָ�ԭ���Ļ��� 
	pDC->SelectObject(pOldPen); 
	// �ָ�ԭ���Ļ�ͼģʽ
	pDC->SetROP2(n); 
	pen.DeleteObject();//�ͷ�pen��Create��;
}

/*
 *	����Բ��
 */
void CCreateRectangle::DrawLineOrArc(CDC *pDC,int i,BOOL FlagShowArrow,BOOL FlagShowNum,Position l_posCurPos,Position l_posNextPos,double TuDu)
{
	// ��Ļ�����
	CPoint pt_begin,pt_End;
	// ����������ת��Ϊ��Ļ����
	g_pView->WorldToScreem(l_posCurPos,pt_begin); 
	g_pView->WorldToScreem(l_posNextPos,pt_End); 
	//��Բ��ֱ��
	if(i==0)
		pDC->MoveTo(pt_begin);
	DrawLineAndArc(pDC,FlagShowArrow,l_posCurPos,l_posNextPos,TuDu);
	if (FlagShowArrow&&i==0)//�Ƿ���ʾ��ʼ��ķ���
	{
		CRect rect(pt_begin.x-5,pt_begin.y-5,pt_begin.x+5,pt_begin.y+5);
 		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
	}
	///////////////////////����������Ŀ��Լ����ڵڼ�λ����˳ʱ��Ȼ����������� ��////////
	if (FlagShowNum&&i==0)//������
	{
		POSITION l_ListPos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
		int l_iNumOfMCode = 0;
		for (int i=0;l_ListPos != NULL;i++)//��������i�ɱ�ʾ������������
		{
			CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(l_ListPos);
			if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
				l_iNumOfMCode++;
			if(this == (CCreateRectangle *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //�����Ϳ���͸����������				
				pDC->TextOut(pt_begin.x+1,pt_begin.y+1,str);//��������� ��
				pDC->SetBkMode(l_iOldMode);				
			}
		}
	}
}

/*
 *	����Բ��
 */
void CCreateRectangle::DrawLineAndArc(CDC *pDC,BOOL FlagShowArrow,Position l_posCurPos,Position l_posNextPos,double TuDu)
{
	//������X��ĽǶ�		Բ���뾶  ��ʼ��    ƫ���
	double angle,    l_fRadius,l_fAngleBG,l_fAngleOffset;
	// ��Ļ�����
	CPoint pt_begin,pt_End,pt_Center;
	// ����������ת��Ϊ��Ļ����
	g_pView->WorldToScreem(l_posCurPos,pt_begin); 
	g_pView->WorldToScreem(l_posNextPos,pt_End); 
	Position l_posCenter(0,0);
	if(TuDu == 0.0)	//ֱ��
	{
		if(m_iFlagArcOrLine == 2)	//�����������ͼ��ʱ��ʾ����
			pDC->MoveTo(pt_begin);
		pDC->LineTo(pt_End); 
		//�Ƿ���ʾ��ͷ
		if (FlagShowArrow)
			angle = GetAngleToXAxis(l_posCurPos,l_posNextPos);
		m_iFlagArcOrLine = 1;
	}
	else if(TuDu < 0.0)//˳��
	{
		//��Բ�ĵ�
		g_pDoc->polylineToArc(l_posCurPos,l_posNextPos,TuDu,l_posCenter);
		// ����������ת��Ϊ��Ļ����
		g_pView->WorldToScreem(l_posCenter,pt_Center);	
		//��뾶����ʼ�Ƕȣ���ֹ�Ƕ�
		l_fRadius = (float)(l_posCenter.Distance(l_posCurPos)/g_pView->ReturnProportion());
		l_fAngleBG = (float)GetAngle(l_posCenter,l_posCurPos);
		l_fAngleOffset = (float)GetAngle(l_posCenter,l_posNextPos);
		//��ƫ��ĽǶ�
		if(l_fAngleOffset<l_fAngleBG)//û��0�Ƚ�ʱ
			l_fAngleOffset = l_fAngleOffset - l_fAngleBG;
		else//����Ƚ�ʱ
			l_fAngleOffset = l_fAngleOffset - 360 -l_fAngleBG;
		//����
		if(m_iFlagArcOrLine == 2)	//�����������ͼ��ʱ��ʾ����
			pDC->MoveTo(pt_begin);
		pDC->AngleArc(pt_Center.x,pt_Center.y,(int)l_fRadius,l_fAngleBG,l_fAngleOffset) ;
		//����ͷ�ĽǶ�		//�Ƿ���ʾ��ͷ
		if (FlagShowArrow)
			angle = atan((l_posCenter.x-l_posNextPos.x) /(l_posNextPos.y -l_posCenter.y ));
		m_iFlagArcOrLine = 2;
	}
	else if(TuDu>0.0)//�满
	{
		//��Բ�ĵ�
		g_pDoc->polylineToArc(l_posCurPos,l_posNextPos,TuDu,l_posCenter);
		// ����������ת��Ϊ��Ļ����
		g_pView->WorldToScreem(l_posCenter,pt_Center);	
		//��뾶����ʼ�Ƕȣ���ֹ�Ƕ�
		l_fRadius = (float)(l_posCenter.Distance(l_posCurPos)/g_pView->ReturnProportion());
		l_fAngleBG = (float)GetAngle(l_posCenter,l_posCurPos);
		l_fAngleOffset = (float)GetAngle(l_posCenter,l_posNextPos);
		//��ƫ��ĽǶ�
		if(l_fAngleOffset>l_fAngleBG)//û��0�Ƚ�ʱ
			l_fAngleOffset = l_fAngleOffset - l_fAngleBG;
		else//����Ƚ�ʱ
			l_fAngleOffset = 360 -l_fAngleBG + l_fAngleOffset;
		//����
		if(m_iFlagArcOrLine == 2)	//�����������ͼ��ʱ��ʾ����
			pDC->MoveTo(pt_begin);
		pDC->AngleArc(pt_Center.x,pt_Center.y,(int)l_fRadius,l_fAngleBG,l_fAngleOffset) ;
		//����ͷ�ĽǶ�		//�Ƿ���ʾ��ͷ
		if (FlagShowArrow)
			angle = atan((l_posCenter.x-l_posNextPos.x) /(l_posNextPos.y -l_posCenter.y ));
		m_iFlagArcOrLine = 2;
	}
	//����ͷ�ĽǶ�		//�Ƿ���ʾ��ͷ
	if (FlagShowArrow)
	{
		if (TuDu>0.0) //�������ʱ��ʱ
			angle = angle+MATH_PI;
		if (l_posCenter.y > l_posNextPos.y&&TuDu!=0.0)	//�����˳ʱ�룬�����������
			angle = angle + MATH_PI;
		DrawArrow(angle,pt_End,pDC);//����ͷ�ĺ��� 
	}
}
/*
 *	ʰȡ����
 */
int CCreateRectangle::IsPick(CPoint point)
{
	Position Pos1,Pos2,pos;// ���������
	double l_dTuDu;//͹��
	g_pView->ScreemToWorld(point,pos);
	//������ű���
	double l_dProportion = g_pView->ReturnProportion();	
	//�㼯�Ĵ�С 
	int l_iSizeOfPos = m_point.GetSize()-1;
	for(int i=0;i<l_iSizeOfPos;i++)
	{
		Pos1=m_point.GetAt(i); 
		Pos2=m_point.GetAt(i+1); 
		//�����������ĩ��ľ����������ʰȡ�����൱��ĩ���Χ������
		if (pos.Distance(Pos1)/l_dProportion<=PRECISION||pos.Distance(Pos2)/l_dProportion<=PRECISION)
			return i;
		l_dTuDu = m_dTuDu.GetAt(i);
		if(l_dTuDu == 0.0)//ֱ��
		{
			if(IsPickLine(Pos1,Pos2,point))//���ѡ�еĻ�
				return i;
		}
		else//Բ��
		{
			if(IsPickArc( Pos1, Pos2, point, l_dTuDu))
				return i;
		}
	}
	return -1;
	
}

/*
 *	ֱ�ߵ�ʰȡ����
 */
BOOL CCreateRectangle::IsPickLine(Position Pos1,Position Pos2,CPoint point)
{
	CRgn	pr;//ֱ����Χ������
	CPoint pt_pos1,pt_pos2;// ��Ļ�����
	// ��������ת��Ϊ��Ļ����
	g_pView->WorldToScreem(Pos1,pt_pos1); 
	g_pView->WorldToScreem(Pos2,pt_pos2); 
	//���߶ε�б��ҲҪ���ȥ����������б�ʵġ�
	double angle = GetAngleToXAxis(Pos1,Pos2);
	double d_Sin = PRECISION * sin(angle);
	double d_Cos = PRECISION * cos(angle);
	
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
		return TRUE;
	}
	pr.DeleteObject();	
	return FALSE;
}

/*
 *	Բ����ʰȡ����
 */
BOOL CCreateRectangle::IsPickArc(Position Pos1,Position Pos2,CPoint point,double TuDu)
{
	Position pos(0,0),l_posCenter;
	g_pDoc->polylineToArc(Pos1,Pos2,TuDu,l_posCenter);
	g_pView->ScreemToWorld(point,pos);
	//����Բ���뾶
	double radius = l_posCenter.Distance(Pos1) ;
	//ʰȡ�㵽Բ�ĵľ���
	double distance =  l_posCenter.Distance(pos); 
	//ת���ı���
	double l_dProportion = g_pView->ReturnProportion();	
	if(!(  (fabs( radius - distance)/l_dProportion)<=PRECISION)  )
		return  FALSE;
	
	//����Բ������ʼ��
	double angle = GetAngle(l_posCenter, pos);
	double angle1 = GetAngle(l_posCenter, Pos1) ;
	//����Բ������ֹ��
	double angle2 = GetAngle(l_posCenter, Pos2) ;
	
	//ʰȡ���Բ������������x��ļн�Ӧ�ô�����
	//��ʼ�Ǻ���ֹ�Ƿ�Χ֮�⣬����ʰȡ�㵽Բ�ĵľ���
	//��Բ���뾶֮��ľ���ֵС�ڸ���ֵʱ������TRUE
	if (TuDu < 0.0)//�����˳ʱ��
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
CCreateBase* CCreateRectangle::Copy()
{
	CCreatePolyline *pCopy = new CCreatePolyline(m_point,m_dTuDu);//���Ƶ�
	pCopy->m_ColorOfLine = m_ColorOfLine;		//������ɫ
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}

//�ƶ�
void CCreateRectangle::Move(const Position& basePos,const Position& desPos)
{
	int l_iSize = m_point.GetSize();//�㼯��С
	Position pt,pt1;
	for (int i=0;i < l_iSize;i++)
	{
		pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Offset(desPos - basePos));	
		pt1 = m_point.ElementAt(i);
	}
}
//��ת
void CCreateRectangle::Rotate(const Position& basePos, const double angle)
{
	for (int i=0;i < m_point.GetSize();i++)
	{
		Position pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Rotate(basePos,angle));	
		Position pt1 = m_point.ElementAt(i);
	}
}
//����
void CCreateRectangle::Mirror(const Position& pos1, const Position& pos2)
{
	for (int i=0;i < m_point.GetSize();i++)
	{
		Position pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Mirror(pos1,pos2));	
		Position pt1 = m_point.ElementAt(i);
	}
}

/*
*	���ԶԻ���
*/
void CCreateRectangle::PropertyView()
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
		if(this == (CCreateRectangle *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
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
void CCreateRectangle::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	Position pos;
	switch(WhichEdit) //�ĸ�EDIT����ˣ��ͱ��ĸ�ֵ
	{
	case 2:
		pos = m_point.GetAt(iNumOfTop-1);
		pos.x = e;
		m_point.SetAt(iNumOfTop-1,pos);
		if(iNumOfTop == 1)
		{
			int l_size = m_point.GetSize();
			pos = m_point.GetAt(l_size-1);
			pos.x = e;
			m_point.SetAt(l_size-1,pos);
		}
		
		break;
	case 3:
		pos = m_point.GetAt(iNumOfTop-1);
		pos.y = e;
		m_point.SetAt(iNumOfTop-1,pos);
		if(iNumOfTop == 1)
		{
			int l_size = m_point.GetSize();
			pos = m_point.GetAt(l_size-1);
			pos.y = e;
			m_point.SetAt(l_size-1,pos);
		}
		
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
BOOL CCreateRectangle::IsSelect(CRgn *selectrect,BOOL bComplex)
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
		if (RectangleOnRect(l_posRgn1,l_posRgn2))
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
BOOL CCreateRectangle::GetBox(BOX2D* pBox,BOOL IsOutside /* = FALSE */)
{
	Position PrePos,CurPos,PrePosMin,PrePosMax,l_posCenter(0,0);//��ǰ��͹�ȥ��
	double l_dTudu = 0.0;//͹��
	int l_iDirect;//Բ������
	BOX2D l_boxArc;
	PrePosMin=m_point.GetAt(0); 
	PrePosMax=m_point.GetAt(0); 
	//�õ���Χ��
	for(int i=0;i<m_point.GetSize()-1;i++)
	{
		PrePos = m_point.GetAt(i);
		CurPos=m_point.GetAt(i+1);
		l_dTudu = m_dTuDu.GetAt(i);
		if(l_dTudu==0.0)//�����ֱ��
		{	//ȡ��������С�ĺ�����
			PrePosMin.x = min( PrePosMin.x, CurPos.x );
			PrePosMin.y = min( PrePosMin.y, CurPos.y );
			PrePosMax.x = max( PrePosMax.x, CurPos.x );
			PrePosMax.y = max( PrePosMax.y, CurPos.y );
		}
		else//Բ��
		{
			g_pDoc->polylineToArc(PrePos,CurPos,l_dTudu,l_posCenter);//��Բ��Բ��
			//�´�һ��Բ�����������еİ�Χ��
			if(l_dTudu<0.0)l_iDirect = AD_CLOCKWISE;
			else l_iDirect = AD_COUNTERCLOCKWISE;
			CCreateRoundArc *pArc = new CCreateRoundArc(PrePos,l_posCenter,CurPos,l_iDirect);
			pArc->GetBox(&l_boxArc);
			delete pArc;
			//ȡ��������С�ĺ�����
			PrePosMin.x = min( PrePosMin.x, l_boxArc.min[0] );
			PrePosMin.y = min( PrePosMin.y, l_boxArc.min[1] );
			PrePosMax.x = max( PrePosMax.x, l_boxArc.max[0] );
			PrePosMax.y = max( PrePosMax.y, l_boxArc.max[1] );
		}
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
BOOL CCreateRectangle::GraphZoomXY(double dValueX, double dValueY)
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
BOOL CCreateRectangle::RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine /* = 0 */)
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
double CCreateRectangle::RtnLongArc(Position posClick,Position posBegin,Position PosEnd,double dTudu)
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
Position CCreateRectangle::RtnLongArcPos(Position posBegin,Position PosEnd,double dTudu,Position posClick,double dLongPos)
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
void CCreateRectangle::ChangeDir()
{
	Position l_ScreemPos;
	double l_dTude;
	CArray<Position,Position&> l_ListPoint; 
	CArray<double,double&> l_ListTuDu;//���͹�ȵ�
	for(int i=m_point.GetSize();  i>1;  i--)
	{
		l_ScreemPos = m_point.GetAt(i-1); 
		l_ListPoint.Add(l_ScreemPos);
		l_dTude = -m_dTuDu.GetAt(i-2); 
		l_ListTuDu.Add(l_dTude);
	}
	l_ScreemPos = m_point.GetAt(0); 
	l_ListPoint.Add(l_ScreemPos);
	l_dTude = 0.0;
	l_ListTuDu.Add(l_dTude);
	m_point.Copy(l_ListPoint);
	m_dTuDu.Copy(l_ListTuDu);	
}


/*
 *	��������Ƿ�������н��㲢�ҽ����Ƿ���ֱ����
 */
BOOL CCreateRectangle::RectangleOnRect(Position pos1,Position pos3)
{
	Position pos2(pos1.x,pos3.y),pos4(pos3.x,pos1.y),l_posInter;
	Position l_posBegin,l_posEnd,l_posCenter;
	Position posTemp1,posTemp2;
	//�㼯�Ĵ�С 
	int l_iSize = m_point.GetSize()-1,l_iDir;
	//͹��
	double l_dTudu,l_dRadius;
	for(int i=0;i<l_iSize;i++)
	{	
		l_dTudu = m_dTuDu.GetAt(i);
		l_posBegin = m_point.GetAt(i);
		l_posEnd = m_point.GetAt(i+1);
		if (l_dTudu == 0.0)//˵����ֱ��
		{		//��������ֱ�ߵĽ���										�ٿ������Ƿ����߶���
			if (GetIntersectPoint(l_posBegin,l_posEnd,pos1,pos2,l_posInter)&&PosOnLine(pos1,pos2,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
				return TRUE;
			else if (GetIntersectPoint(l_posBegin,l_posEnd,pos2,pos3,l_posInter)&&PosOnLine(pos2,pos3,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
				return TRUE;
			else if (GetIntersectPoint(l_posBegin,l_posEnd,pos3,pos4,l_posInter)&&PosOnLine(pos3,pos4,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
				return TRUE;
			else if (GetIntersectPoint(l_posBegin,l_posEnd,pos4,pos1,l_posInter)&&PosOnLine(pos4,pos1,l_posInter)&&PosOnLine(l_posBegin,l_posEnd,l_posInter))
				return TRUE;
		}
		else//˵����Բ��
		{
			g_pDoc->polylineToArc(l_posBegin,l_posEnd,l_dTudu,l_posCenter);
			l_dRadius = l_posCenter.Distance(l_posBegin);
			l_iDir = RtnArcDir(l_dTudu);
			posTemp1 = pos1;
			posTemp2 = pos2;
			//��������ͬ��Բ����������	
			if (GetIntersectLineRound(posTemp1,posTemp2,l_posCenter,l_dRadius))//�Ƿ���Բ��
			{//								�ٿ������Ƿ����߶���
				if((PosOnRectangleArc(posTemp1,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos1,pos2,posTemp1))||
					(PosOnRectangleArc(posTemp2,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos1,pos2,posTemp2)))//�Ƿ��ڻ���
					return TRUE;
			}
			posTemp1 = pos2;
			posTemp2 = pos3;
			if (GetIntersectLineRound(posTemp1,posTemp2,l_posCenter,l_dRadius))    //�Ƿ���Բ��
			{
				if((PosOnRectangleArc(posTemp1,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos3,pos2,posTemp1))||
					(PosOnRectangleArc(posTemp2,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos3,pos2,posTemp2)))//�Ƿ��ڻ���
					return TRUE;
			}
			posTemp1 = pos3;
			posTemp2 = pos4;
			if (GetIntersectLineRound(posTemp1,posTemp2,l_posCenter,l_dRadius))//�Ƿ���Բ��
			{
				if((PosOnRectangleArc(posTemp1,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos3,pos4,posTemp1))||
					(PosOnRectangleArc(posTemp2,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos3,pos4,posTemp2)))//�Ƿ��ڻ���
					return TRUE;
			}
			posTemp1 = pos4;
			posTemp2 = pos1;
			if (GetIntersectLineRound(posTemp1,posTemp2,l_posCenter,l_dRadius))//�Ƿ���Բ��
			{
				if((PosOnRectangleArc(posTemp1,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos1,pos4,posTemp1))||
					(PosOnRectangleArc(posTemp2,l_posBegin,l_posEnd,l_posCenter,l_iDir)&&PosOnLine(pos1,pos4,posTemp2)))//�Ƿ��ڻ���
					return TRUE;
			}
		}
	}
	return FALSE;
}

/*
 *	���Ƿ���Բ����
*/
BOOL CCreateRectangle::PosOnRectangleArc(Position pos,Position posBegin,Position posEnd,Position posCenter,int iDir)
{	
	//����Բ������ʼ��
	double angle = GetAngle(posCenter, pos);
	double angle1 = GetAngle(posCenter, posBegin) ;
	//����Բ������ֹ��
	double angle2 = GetAngle(posCenter, posEnd) ;
	
	if (iDir == AD_CLOCKWISE)//�����˳ʱ��
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
*	����������
*/
void CCreateRectangle::GraphZoomScale(Position posBase,double dScale)
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
PGRAPHPOS CCreateRectangle::SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist)
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
			if(m_aryDash.GetSize() > 0)
			{
				m_aryDash.Add(l_posBegin);
				CCreatePolyline* pLine = new CCreatePolyline(m_aryDash);
				POSITION POS = g_pDoc->m_CCreateBaseList.Find(this);
				g_pDoc->m_CCreateBaseList.InsertBefore(POS,pLine);
				m_aryDash.RemoveAll();							
				PGRAPHPOS GraphPosPara = NULL;
				CUndoRedo l_UndoRedo;
				GraphPosPara =  l_UndoRedo.NewGraphPos();
				GraphPosPara->pCreateBasepos = (long)pLine;
				//���ͼ��
				if (headGraphPos == NULL)
					headGraphPos = GraphPosPara;
				else
					g_pDoc->AddTail(headGraphPos,GraphPosPara);
			}			
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











