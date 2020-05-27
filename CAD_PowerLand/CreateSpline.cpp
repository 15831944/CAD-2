// CreateSpline.cpp: implementation of the CCreateSpline class.
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
IMPLEMENT_SERIAL(CCreateSpline, CCreateBase, 0)
CCreateSpline::CCreateSpline()
{

}

CCreateSpline::~CCreateSpline()
{

}

CCreateSpline::CCreateSpline(CArray<Position,Position&>&  point,
							 CArray<Position,Position&>& aryControlPoints,CArray<int,int&>& aryNumControl)
{
	m_ColorOfLine = g_CurColor;		//����ʱ�Ļ�ɫ���ǵ�ǰ��ѡɫ
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_point.Copy(point);
	int l_NumOfm_point = m_point.GetSize();
	double aaa = 0.0;
	for(int i = 0;i<l_NumOfm_point;i++)
		m_dTuDu.Add(aaa);
	m_aryControlPoints.Copy(aryControlPoints);
	m_aryNumControl.Copy(aryNumControl);
	m_iKnifeStyle = 41;//�������� 41�󵶲� 42�ҵ���
	m_iIsBeginGraph = 2;//0����ԭʼͼ�Σ�1��ԭʼͼ�Σ�2�Ǳ���ʱ������
}

/*
 *	����������
 */
void CCreateSpline::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow,BOOL FlagShowNum)
{
	int		n = GetROP2(pDC->GetSafeHdc()); 	
	CPen	pen; 
	if (DrawMode == dmNormal)		
		pen.CreatePen(PS_SOLID,1,m_ColorOfLine);//�����������ͼģʽ����ɫ��Ҫȡ������ѡɫ
	else
		SetDrawEnvir(pDC, DrawMode, &pen);	//����SetDrawEnvir�������û�ͼ����
	CPen* pOldPen = pDC->SelectObject(&pen); // �õ�ԭ���Ļ���	

	CArray<POINT,POINT&> l_ArtScreemPos;//��Ļ�ĵ㼯
	Position l_posWord;
	CPoint l_potScreem;
	int l_iSizeOfPos = m_point.GetSize();//����߹��ж��ٸ���
	for(int i=0;i<l_iSizeOfPos;i++)
	{
		l_posWord = m_point.GetAt(i);
		g_pView->WorldToScreem(l_posWord,l_potScreem);
		l_ArtScreemPos.Add(l_potScreem);
		if(i == 0)
			m_potBegin = l_potScreem;
	}
	pDC->Polyline(l_ArtScreemPos.GetData(), l_ArtScreemPos.GetSize());	
	//������ͼ�ͷ
	if (FlagShowArrow)	
	{
		//����ʼ��ķ���
		CRect rect(m_potBegin.x-5,m_potBegin.y-5,m_potBegin.x+5,m_potBegin.y+5);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(rect);
		
		int l_iSize = m_aryNumControl.GetSize();
		int l_iControlOnCurve = 0;//���Ƶ������ߵ㼯�е�λ��
		Position l_posBegin,l_posEnd;
		for(int i=1;i<l_iSize;i++)
		{
			l_iControlOnCurve = m_aryNumControl.GetAt(i);
			l_posBegin = m_point.GetAt(l_iControlOnCurve-2);
			l_posEnd = m_point.GetAt(l_iControlOnCurve-1);
			g_pView->WorldToScreem(l_posEnd,l_potScreem);		
			double angle = GetAngleToXAxis(l_posBegin,l_posEnd);
			DrawArrow(angle,l_potScreem,pDC);//����ͷ�ĺ��� 
		}
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
			if(this == (CCreateSpline *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				int l_iOldMode = pDC->SetBkMode(TRANSPARENT);  //�����Ϳ���͸����������				
				pDC->TextOut(m_potBegin.x,m_potBegin.y,str);//��������� ��
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

CCreateBase* CCreateSpline::Copy()
{
	CCreateSpline *pCopy = new CCreateSpline(m_point,m_aryControlPoints,m_aryNumControl);//���Ƶ�
	pCopy->m_ColorOfLine = m_ColorOfLine;		//������ɫ
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}

//�ƶ�
void CCreateSpline::Move(const Position& basePos,const Position& desPos)
{
	int l_iSize = m_point.GetSize();//�㼯��С
	Position pt;
	for (int i=0;i < l_iSize;i++)
	{
		pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Offset(desPos - basePos));	
	}	
	l_iSize = m_aryControlPoints.GetSize();//�㼯��С
	for (int j=0;j < l_iSize;j++)
	{
		pt = m_aryControlPoints.ElementAt(j);
		m_aryControlPoints.SetAt(j,pt.Offset(desPos - basePos));	
	}	
	
}

//��ת
void CCreateSpline::Rotate(const Position& basePos, const double angle)
{
	int l_iSize = m_point.GetSize();//���ض���ߵĵ�ĸ���
	Position pt;
	//��ÿ������ת
	for (int i=0;i < l_iSize;i++)
	{
		pt = m_point.ElementAt(i);//�ӵ㼯����ȡÿһ����
		m_point.SetAt(i,pt.Rotate(basePos,angle));//��ת���ٷŻص㼯��	
	}	
	l_iSize = m_aryControlPoints.GetSize();//���ض���ߵĵ�ĸ���
	//��ÿ������ת
	for (int j=0;j < l_iSize;j++)
	{
		pt = m_aryControlPoints.ElementAt(j);//�ӵ㼯����ȡÿһ����
		m_aryControlPoints.SetAt(j,pt.Rotate(basePos,angle));//��ת���ٷŻص㼯��	
	}	
	
}

//����
void CCreateSpline::Mirror(const Position& pos1, const Position& pos2)
{
	int l_iSize = m_point.GetSize();
	Position pt;
	double l_dTudu;
	for (int i=0;i < l_iSize;i++)
	{
		pt = m_point.ElementAt(i);
		m_point.SetAt(i,pt.Mirror(pos1,pos2));	
		l_dTudu = -(m_dTuDu.ElementAt(i));
		m_dTuDu.SetAt(i,l_dTudu);	
	}
	l_iSize = m_aryControlPoints.GetSize();
	for (int j=0;j < l_iSize;j++)
	{
		pt = m_aryControlPoints.ElementAt(j);
		m_aryControlPoints.SetAt(j,pt.Mirror(pos1,pos2));	
	}
}

/*
 *	�ı��и��
 */
void CCreateSpline::ChangeDir()
{
	Position l_ScreemPos;
	int l_iNumControl;
	CArray<Position,Position&> l_ListPoint; 
	CArray<Position,Position&> l_ListControlPoint; 
	CArray<int,int&> l_ListNumControl;//���͹�ȵ�
	int i=0;
	for(i=m_point.GetSize();  i>0;  i--)
	{
		l_ScreemPos = m_point.GetAt(i-1); 
		l_ListPoint.Add(l_ScreemPos);
	}
	m_point.Copy(l_ListPoint);

	for(i=m_aryControlPoints.GetSize();  i>0;  i--)
	{
		l_ScreemPos = m_aryControlPoints.GetAt(i-1); 
		l_ListControlPoint.Add(l_ScreemPos);
	}
	m_aryControlPoints.Copy(l_ListControlPoint);

	int l_iSize = m_aryNumControl.GetAt(m_aryNumControl.GetSize()-1);
	for(i=m_aryNumControl.GetSize();  i>0;  i--)
	{
		if(i==m_aryNumControl.GetSize()||i==1)
			l_iNumControl = l_iSize - m_aryNumControl.GetAt(i-1); 
		else
			l_iNumControl = l_iSize - m_aryNumControl.GetAt(i-1)+1; 		
		l_ListNumControl.Add(l_iNumControl);
	}
	m_aryNumControl.Copy(l_ListNumControl);
	
}

/*
*	�������ԶԻ��򵯳��ı�ͼԪ����ʱҪ�ػ��ĵĺ���
*/
void CCreateSpline::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	Position pos;
	switch(WhichEdit) //�ĸ�EDIT����ˣ��ͱ��ĸ�ֵ
	{
			case 2:
				pos = m_aryControlPoints.GetAt(iNumOfTop-1);
				pos.x = e;
				m_aryControlPoints.SetAt(iNumOfTop-1,pos);
				break;
			case 3:
				pos = m_aryControlPoints.GetAt(iNumOfTop-1);
				pos.y = e;
				m_aryControlPoints.SetAt(iNumOfTop-1,pos);
				break;
			default:
				break;		
	}
	CArray<Position,Position&> l_aryEveryPoint;
	int l_iSize = m_aryControlPoints.GetSize(),i=0,j=0;
	Position l_posControl;
	m_aryNumControl.RemoveAll();
	m_aryNumControl.Add(i);			
	//
	for(j=2;j<=l_iSize;j++)
	{
		l_aryEveryPoint.RemoveAll();
		for(i=0;i<j;i++)
		{
			l_posControl = m_aryControlPoints.GetAt(i);
			l_aryEveryPoint.Add(l_posControl);
		}
		//����һ���������߶���
		Spline spline(l_aryEveryPoint, l_aryEveryPoint.GetSize());
		//���ݿ��Ƶ�����һ����������
		spline.Generate();
		//�趨���ߵ���				�õ����Ƶ���
		m_point.SetSize(spline.GetCurveCount());
		//�õ����ߵ������㼯
		int PointCount = 0;
		spline.GetCurve(m_point, PointCount);
		m_aryNumControl.Add(PointCount);			
		//��ͼ�����������	
	}

	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);	
}

/*
 *	ͨ�����Ƶ�ˢ����������
 */
void CCreateSpline::RefreshSpline()
{
	CArray<Position,Position&> l_aryEveryPoint;
	int l_iSize = m_aryControlPoints.GetSize(),i=0,j=0;
	Position l_posControl;
	m_aryNumControl.RemoveAll();
	m_aryNumControl.Add(i);			
	//
	for(j=2;j<=l_iSize;j++)
	{
		l_aryEveryPoint.RemoveAll();
		for(i=0;i<j;i++)
		{
			l_posControl = m_aryControlPoints.GetAt(i);
			l_aryEveryPoint.Add(l_posControl);
		}
		//����һ���������߶���
		Spline spline(l_aryEveryPoint, l_aryEveryPoint.GetSize());
		//���ݿ��Ƶ�����һ����������
		spline.Generate();
		//�趨���ߵ���				�õ����Ƶ���
		m_point.SetSize(spline.GetCurveCount());
		//�õ����ߵ������㼯
		int PointCount = 0;
		spline.GetCurve(m_point, PointCount);
		m_aryNumControl.Add(PointCount);			
		//��ͼ�����������	
	}
	int l_NumOfm_point = m_point.GetSize();
	double aaa = 0.0;
	for(int k = 0;k<l_NumOfm_point;k++)
		m_dTuDu.Add(aaa);
	
}








