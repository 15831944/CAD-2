// CreatePoint.cpp: implementation of the CCreatePoint class.
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
IMPLEMENT_SERIAL(CCreatePoint, CCreateBase, 0)
CCreatePoint::CCreatePoint()
{

}

CCreatePoint::CCreatePoint(Position pot)
{
	m_Begin = pot;
	m_ColorOfLine = g_CurColor;		//����ʱ�Ļ�ɫ���ǵ�ǰ��ѡɫ
	m_strCBM.Add(g_strCBM);	
	m_strSBM.Add(g_strSBM);
	m_iKnifeStyle = 41;//�������� 41�󵶲� 42�ҵ���
	m_iIsBeginGraph = 2;//0����ԭʼͼ�Σ�1��ԭʼͼ�Σ�2�Ǳ���ʱ������
}

CCreatePoint::~CCreatePoint()
{

}

/*
 *	����
 */
void CCreatePoint::DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow /* = FALSE */,BOOL FlagShowNum /* = FALSE */)
{
	CPoint pt_begin; // ��Ļ����
	// ����������ת��Ϊ��Ļ����
	g_pView->WorldToScreem(m_Begin,pt_begin); 

//////////////////////////////////////////////////////////////////////////
	// �õ�ԭ���Ļ�ͼģʽ
	int		n = GetROP2(pDC->GetSafeHdc()); 
	// �������ʵ�ԭ��
	// ����������Ļ�ͼģʽ�£�ʹ�ó�Ա������������
	// �����������ģʽ,ʹ��ȫ�ֺ���"SetDrawEnvir"��������
	CPen	pen; 
	int			lineStyle, lineWidth;
	COLORREF	color;
	switch(DrawMode) 
	{
	case dmSelect: // ѡ��״̬
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_SOLID;
			lineWidth = 1;
			color = m_ColorOfLine;//RGB(0,0,0);
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
	case dmPrompt: // ��ʾ״̬
		{
			pDC->SetROP2(R2_COPYPEN);
			lineStyle = PS_DOT;
			lineWidth = 1;
			color = m_ColorOfLine;
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
	pen.CreatePen(lineStyle,lineWidth,color) ;

	CPen* pOldPen = pDC->SelectObject(&pen); // �õ�ԭ���Ļ���
//////////////////////////////////////////////////////////////////////////
	if (DrawMode == dmNormal)
	{
		pDC->SetPixel(pt_begin,m_ColorOfLine);	// ������Ļ�������ͼԪ
	}
	else	
	{		
		CRect rect(pt_begin.x-5,pt_begin.y-5,pt_begin.x+5,pt_begin.y+5);		
		pDC->SelectStockObject(GRAY_BRUSH);	
		pDC->Rectangle(rect);
		FlagShowArrow = TRUE;
	}
	if (FlagShowArrow)	//������ͼ�ͷ
	{
		//����ʼ��ķ���
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
			if(this == (CCreatePoint *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
			{
				CString str;
				str.Format("%d",i-l_iNumOfMCode);
				//pDC->SetBkMode(TRANSPARENT);  //�����Ϳ���͸����������
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

/*
 *	�����Ƿ�ѡ��
 */
int CCreatePoint::IsPick(CPoint point)
{
	Position l_posPick;
	g_pView->ScreemToWorld(point,l_posPick);
	if(l_posPick.Distance(m_Begin)/g_pView->ReturnProportion()<=PRECISION)
		return TRUE;
	return -1;
}


CCreateBase* CCreatePoint::Copy()
{
	CCreatePoint *pCopy = new CCreatePoint(m_Begin);	//���Ƶ�
	pCopy->m_ColorOfLine = m_ColorOfLine;		//������ɫ
	pCopy->m_strCBM.Copy(m_strCBM);	
	pCopy->m_strSBM.Copy(m_strSBM);	
	return pCopy;
}


void CCreatePoint::Move(const Position& basePos,const Position& desPos)
{
	m_Begin = m_Begin.Offset(desPos - basePos);
}

void CCreatePoint::Mirror(const Position& pos1, const Position& pos2)
{
	m_Begin = m_Begin.Mirror(pos1, pos2) ;
}

void CCreatePoint::Rotate(const Position& basePos, const double angle)
{
	m_Begin = m_Begin.Rotate(basePos, angle) ;
}





BOOL CCreatePoint::IsSelect(CRgn *selectrect,BOOL bComplex)
{
	CPoint pos1;	
	//ת����Ļ����
	g_pView->WorldToScreem(m_Begin,pos1);
	//�����Ƿ���������
	BOOL l_bIsSelect= selectrect->PtInRegion(pos1);
	return l_bIsSelect;
}

void CCreatePoint::ChangeProperty(double e,int WhichEdit,int iNumOfTop/* =0 */)
{
	switch(WhichEdit) //�ĸ�EDIT����ˣ��ͱ��ĸ�ֵ
	{
	case 1:
		m_Begin.x = e;
		break;
	case 2:
		m_Begin.y = e;
		break;
	default:
		break;
	}
	CRect rect;
	g_pView->GetClientRect(rect);
	g_pView->InvalidateRect(rect);	
}

void CCreatePoint::PropertyView()
{
	//��ʾ���ԶԻ���
	(((CMainFrame*)AfxGetMainWnd()))->ShowControlBar(&(((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar), TRUE, TRUE); 
	
	//�����ص�STATIC��Edit��ʾ����
	ShowControl(2,FALSE);
	//�趨Ҫ��ʾ����ɫ
	
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.m_ComboColor.SetCurSel(RtnColorInt()-1);
	//����STATICҪ��ʾ�������յ㡱
	//����EditҪ��ʾ�����յ��λ��
	if (g_pDoc->m_stcWorkFor.IsChinese)
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"��X����");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"��Y����");
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC1,"X");
		((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_STATIC2,"Y");
	}
	CString str;
	str.Format("%.4f",m_Begin.x);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT1,str);
	str.Format("%.4f",m_Begin.y);
	((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT2,str);
	////////////////////////����������Ŀ��Լ����ڵڼ�λ����˳ʱ��///////////////////////////////////
	POSITION pos = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	for (int i=0;pos != NULL;i++)//��������i�ɱ�ʾ������������
	{
		CCreateBase* l_pDelList = (CCreateBase*)g_pDoc->m_CCreateBaseList.GetNext(pos);
		//�����M����
		if (l_pDelList->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
			i--;
		if(this == (CCreatePoint *)l_pDelList)//������������Ԫ��һ������Ҫ����λ��
		{
			str.Format("%d",i);
			((CMainFrame*)AfxGetMainWnd())->m_DlgPropertyBar.SetDlgItemText(IDC_EDIT_SEQUENCE,str);
		}
	}
}

//�õ���Χ��
BOOL CCreatePoint::GetBox(BOX2D *pBox,BOOL IsOutside /* = FALSE */)
{
	double curRadius = g_pView->ReturnProportion();
	//���û������
	if (IsOutside == FALSE)
		curRadius = 0.0;
	pBox->min[0] = m_Begin.x - curRadius;
	pBox->min[1] = m_Begin.y - curRadius;
	pBox->max[0] = m_Begin.x + curRadius;
	pBox->max[1] = m_Begin.y + curRadius;
	return TRUE;
	return FALSE;
}











