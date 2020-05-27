// OpenGCode.cpp: implementation of the COpenGCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "OpenGCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenGCode::COpenGCode()
	:m_Cur(0,0)
{
	m_iStyle = -1;
	m_dCurTudu = 0.0;
	m_point.RemoveAll();
	m_aryTudu.RemoveAll();
	m_CollectOfG00.RemoveAll();
}

COpenGCode::~COpenGCode()
{
	m_point.RemoveAll();
	m_aryTudu.RemoveAll();
}

/*
 *	��DOC���õĴ��ļ�����G����
 */
BOOL COpenGCode::ReadGCode(CString PathName)
{	
	CString  l_strLineContent;	//ÿ���ı�������
	CFileException ex;	
	BOOL isEof=FALSE;		
	if (!m_cReadGCode.Open(PathName,CFile::modeRead | CFile::shareDenyWrite, &ex)) //���ļ�		
	{	
		//���û�򿪾ͱ���
		TCHAR szError[1024];		
		ex.GetErrorMessage(szError, 1024);		
		AfxMessageBox(szError);		
		return  FALSE;		
	}
	else		
	{	
		//��ȡ�ı�
 		while(!isEof)			
		{	//�ӵ�һ�п�ʼ����
			isEof=!m_cReadGCode.ReadString(l_strLineContent);//��ȡһ���ı���������һ���˾���isEof==1			
			if(isEof)//���û���˾���1���˳�
				break;	
			if(l_strLineContent.Find("G00") != -1 )
			{
				TurnG00(l_strLineContent);
			}
			else if(l_strLineContent.Find("G01") != -1)
			{
				TurnG01(l_strLineContent);
			}
			else if(l_strLineContent.Find("G02") != -1)
			{
				TurnG02(l_strLineContent);
			}
			else if(l_strLineContent.Find("G03") != -1)
			{
				TurnG03(l_strLineContent);
			}
			else if(l_strLineContent.Find("M") != -1)
			{
				MWhat(l_strLineContent);
			}
			else if(l_strLineContent.Find("G41") != -1)
			{
				m_iG41orG42 = 41;
			}
			else if(l_strLineContent.Find("G42") != -1)
			{
				m_iG41orG42 = 42;
			}
		}
		if(m_iStyle == 0&&m_aryOnceM.GetSize()>1)	//����ǵ�Ļ�
		{
			/*������û�е�
			CCreatePoint* pPoint = new CCreatePoint(m_Cur);
			pPoint->m_ColorOfLine = DistriPointMCode();
			pPoint->m_strSBM.Copy(m_aryOnceM);
			pPoint->m_strCBM.Copy(m_aryTwicM);	
			g_pDoc->m_CCreateBaseList.AddTail(pPoint);
			
			m_CollectOfG00.RemoveAll();
			m_point.RemoveAll();
			m_aryTudu.RemoveAll();
			m_aryOnceM.RemoveAll();	
			m_aryTwicM.RemoveAll();	*/	
		}
		//������ͼ�Σ�˵���û�ͼ��
		Draw(RtnColor(),m_Cur);		
		g_pView->Invalidate(TRUE);
	}	

	return TRUE;
}

/*
 *	����G00
 */
void COpenGCode::TurnG00(CString LineG00)
{
	Position posPre = m_Cur;
	int nLength = LineG00.GetLength();
	int nNum = LineG00.FindOneOf("X");
	if (nLength>nNum)
	{
		m_Cur.x = atof(LineG00.Mid(nNum+1));
	}
	nNum = LineG00.FindOneOf("Y");
	if (nLength>nNum)
	{
		m_Cur.y = atof(LineG00.Mid(nNum+1));
	}
	m_iStyle = 0;	//��ǰ����

	if (m_point.GetSize() == 0)//��ĥ���������У���ͷҪ�м���M����Ҫ����ģ�
	{	
		if (m_iStyle == -1)
		{
			for(int i=0;i<m_aryTwicM.GetSize();i++)
			{
				CCreateMCode* pTempMWhat = new CCreateMCode(m_aryTwicM.GetAt(i));
				g_pDoc->m_CCreateBaseList.AddTail(pTempMWhat);
			}
			m_aryTwicM.RemoveAll();
			return;
		}
		else if(m_iStyle == 0&&m_aryOnceM.GetSize()>1)	//����ǵ�Ļ�
		{
/*������û�е�
			CCreatePoint* pPoint = new CCreatePoint(posPre);
			pPoint->m_ColorOfLine = DistriPointMCode();
			pPoint->m_strSBM.Copy(m_aryOnceM);
			pPoint->m_strCBM.Copy(m_aryTwicM);	
			g_pDoc->m_CCreateBaseList.AddTail(pPoint);
			
			m_CollectOfG00.RemoveAll();
			m_point.RemoveAll();
			m_aryTudu.RemoveAll();
			m_aryOnceM.RemoveAll();	
			m_aryTwicM.RemoveAll();	*/
			return;		
		}
	}
	//������ͼ�Σ�˵���û�ͼ��
	Draw(RtnColor(),posPre);
}

/*
 *	����G01
 */
void COpenGCode::TurnG01(CString LineG01)
{
	m_point.Add(m_Cur);
	m_dCurTudu = 0.0;
	m_aryTudu.Add(m_dCurTudu);
	int nLength = LineG01.GetLength();
	int nNum = LineG01.FindOneOf("X");
	if (nLength>nNum)
	{
		m_Cur.x = atof(LineG01.Mid(nNum+1));	
	}
	nNum = LineG01.FindOneOf("Y");
	if (nLength>nNum)
	{
		m_Cur.y = atof(LineG01.Mid(nNum+1));
	}
	m_iStyle = 1;	//��ǰ����
}

/*
 *	����G02
 */
void COpenGCode::TurnG02(CString LineG02)
{
	//Բ����ĩ��	
	Position l_posEnd,l_posCenter,l_posBegin;
	//����X����ʼX      ����X����ʼY 
	double l_dCtrSubBgX,l_dCtrSubBgY,l_dTudu=0.0;
	int nLength = LineG02.GetLength();
	int nNum = LineG02.FindOneOf("X");	//�յ�X
	if (nLength>nNum)
	{
		l_posEnd.x = atof(LineG02.Mid(nNum+1));
	}
	nNum = LineG02.FindOneOf("Y");//�յ�Y
	if (nLength>nNum)
	{
		l_posEnd.y = atof(LineG02.Mid(nNum+1));
	}
	nNum = LineG02.FindOneOf("I");	//����X����ʼX
	if (nLength>nNum)
	{
		l_dCtrSubBgX = atof(LineG02.Mid(nNum+1));
	}
	nNum = LineG02.FindOneOf("J");	//����Y����ʼY
	if (nLength>nNum)
	{
		l_dCtrSubBgY = atof(LineG02.Mid(nNum+1));
	}
	//Բ�������ĵ�
	l_posCenter.x = m_Cur.x+l_dCtrSubBgX;
	l_posCenter.y = m_Cur.y+l_dCtrSubBgY;
	//��͹��
	l_dTudu = g_pDoc->RtnTudu(m_Cur,l_posEnd,l_posCenter,2);
	//�����Բ�Ļ�
	if(fabs(m_Cur.x-l_posEnd.x)<DOUBLE_EQUAL&&fabs(m_Cur.y-l_posEnd.y)<DOUBLE_EQUAL)
	{
		m_point.Add(l_posEnd);
		l_dTudu = 0.0;
		m_aryTudu.Add(l_dTudu);
		if(m_point.GetSize()>=2)
		{
			CCreatePolyline* l_pTempPolyline = new CCreatePolyline(m_point,m_aryTudu);	
			m_CollectOfG00.Add(l_pTempPolyline);//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
		}
		m_point.RemoveAll();	//����ɾ����
		m_aryTudu.RemoveAll();
		//�����Ǽ�һ������ߣ������Բ
		CCreateRound* l_pRound = new CCreateRound(l_posCenter,l_posEnd,AD_CLOCKWISE);	//����
		m_CollectOfG00.Add(l_pRound);//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ	
	}
	else
	{
		m_point.Add(m_Cur);
		m_aryTudu.Add(l_dTudu);
	}
	m_Cur = l_posEnd;
	m_dCurTudu = l_dTudu;
	m_iStyle = 2;	//��ǰ����
}

/*
 *	����G03
 */
void COpenGCode::TurnG03(CString LineG03)
{
	//Բ����ĩ��	
	Position l_posEnd,l_posCenter,l_posBegin;
	//����X����ʼX      ����X����ʼY 
	double l_dCtrSubBgX,l_dCtrSubBgY,l_dTudu=0.0;
	int nLength = LineG03.GetLength();
	int nNum = LineG03.FindOneOf("X");	//�յ�X
	if (nLength>nNum)
	{
		l_posEnd.x = atof(LineG03.Mid(nNum+1));
	}
	nNum = LineG03.FindOneOf("Y");//�յ�Y
	if (nLength>nNum)
	{
		l_posEnd.y = atof(LineG03.Mid(nNum+1));
	}
	nNum = LineG03.FindOneOf("I");	//����X����ʼX
	if (nLength>nNum)
	{
		l_dCtrSubBgX = atof(LineG03.Mid(nNum+1));
	}
	nNum = LineG03.FindOneOf("J");	//����Y����ʼY
	if (nLength>nNum)
	{
		l_dCtrSubBgY = atof(LineG03.Mid(nNum+1));
	}
	//Բ�������ĵ�
	l_posCenter.x = m_Cur.x+l_dCtrSubBgX;
	l_posCenter.y = m_Cur.y+l_dCtrSubBgY;
	//��͹��
	l_dTudu = g_pDoc->RtnTudu(m_Cur,l_posEnd,l_posCenter,3);
	//�����Բ�Ļ�
	if(fabs(m_Cur.x-l_posEnd.x)<DOUBLE_EQUAL&&fabs(m_Cur.y-l_posEnd.y)<DOUBLE_EQUAL)
	{
		m_point.Add(l_posEnd);
		l_dTudu = 0.0;
		m_aryTudu.Add(l_dTudu);
		if(m_point.GetSize()>=2)
		{
			CCreatePolyline* l_pTempPolyline = new CCreatePolyline(m_point,m_aryTudu);	
			m_CollectOfG00.Add(l_pTempPolyline);//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
		}
		m_point.RemoveAll();	//����ɾ����
		m_aryTudu.RemoveAll();
		//�����Ǽ�һ������ߣ������Բ
		CCreateRound* l_pRound = new CCreateRound(l_posCenter,l_posEnd,AD_COUNTERCLOCKWISE);	//����
		m_CollectOfG00.Add(l_pRound);//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ	
	}
	else
	{
		m_point.Add(m_Cur);
		m_aryTudu.Add(l_dTudu);	
	}
	m_dCurTudu = l_dTudu;
	m_Cur = l_posEnd;
	m_iStyle = 3;	//��ǰ����
}

/*
 *	����M����Ȼ�����ͼԪ
 */
void COpenGCode::MWhat(CString LineM)
{
	if (m_iStyle == 0)		//˵����G00�����M����
		m_aryOnceM.Add(LineM);
	else
		m_aryTwicM.Add(LineM);
	return;
}

/*
 *	���ߺ���
 */
void COpenGCode::Draw(COLORREF color,Position posPer)
{	//�������������ͺ���ɫ����
	//�����һ�����
	m_point.Add(posPer);
	double l_dZore = 0.0;
	m_aryTudu.Add(l_dZore);
	if(m_point.GetSize()>1)
	{
		//�����������һ��Բ����Ҫ���Բ���Ķ��� 
		if(m_point.GetSize() == 2&&m_aryTudu.GetAt(0)!=0.0)
		{
			//		Բ�������		����			ĩ��
			Position l_posBeginPos,l_posCenterPos,l_posEndPos;
			//͹��
			double l_dTudu = m_aryTudu.GetAt(0);
			l_posBeginPos = m_point.GetAt(0);
			l_posEndPos = m_point.GetAt(1);
			g_pDoc->polylineToArc(l_posBeginPos,l_posEndPos,l_dTudu,l_posCenterPos);
			//��Բ��
			CCreateRoundArc *l_pTempArc = new CCreateRoundArc(l_posBeginPos,l_posCenterPos,l_posEndPos,RtnArcDir(l_dTudu));
			m_CollectOfG00.Add(l_pTempArc);//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
			m_point.RemoveAll();	//����ɾ����
			m_aryTudu.RemoveAll();
		}
		else//������Ƕ����
		{
			CCreatePolyline* l_pTempPolyline = new CCreatePolyline(m_point,m_aryTudu);	
			m_CollectOfG00.Add(l_pTempPolyline);//��ͼԪ�ӽ����м����������һ��M�������ʱҪ������ɫ
			m_point.RemoveAll();	//����ɾ����
			m_aryTudu.RemoveAll();
		}
	}
	//��һ��M ֮���ͼԪ������ɫ���ӵ�������
	for( int i = 0 ; i < m_CollectOfG00.GetSize() ; i++ )
	{
		CCreateBase*	pSelEntity = (CCreateBase*) m_CollectOfG00[i] ;
		pSelEntity->m_ColorOfLine = color;
		pSelEntity->m_strSBM.Copy(m_aryOnceM);
		pSelEntity->m_strCBM.Copy(m_aryTwicM);
		pSelEntity->m_iKnifeStyle = m_iG41orG42;
		g_pDoc->m_CCreateBaseList.AddTail(pSelEntity);
	}
	m_CollectOfG00.RemoveAll();
	m_point.RemoveAll();
	m_aryTudu.RemoveAll();
	m_aryOnceM.RemoveAll();	
	m_aryTwicM.RemoveAll();
}

/*
 *	//����M�ַ���֮����������ɫ
 */
COLORREF COpenGCode::RtnColor()
{
	COLORREF l_cColorOfLine = g_pDoc->m_stcColorToM.m_color5;;
	//һ��ͼ�����ߵ�M����
	CString l_strOneM,l_strTwoM;
	//����M���뼯�Ĵ�С 
	int l_iSizeOne = m_aryOnceM.GetSize();
	int l_iSizeTwo = m_aryTwicM.GetSize();
	for(int i=0;i<l_iSizeOne;i++)
	{
		l_strOneM = m_aryOnceM.GetAt(i);
		for(int j=0;j<l_iSizeTwo;j++)
		{
			l_strTwoM = m_aryTwicM.GetAt(j);
			if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM1&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM1)
			{
				l_cColorOfLine = g_pDoc->m_stcColorToM.m_color1;	//��һ
				return l_cColorOfLine;
			}
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM2&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM2)
			{
				l_cColorOfLine = g_pDoc->m_stcColorToM.m_color2;	//����ɫ
				return l_cColorOfLine;
			}
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM3&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM3)
			{
				l_cColorOfLine = g_pDoc->m_stcColorToM.m_color3;	//����ɫ
				return l_cColorOfLine;
			}
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM4&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM4)
			{
				l_cColorOfLine = g_pDoc->m_stcColorToM.m_color4;	//����ɫ
				return l_cColorOfLine;
			}
			else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM5&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM5)
			{
				l_cColorOfLine = g_pDoc->m_stcColorToM.m_color5;	//����ɫ
				return l_cColorOfLine;
			}
			else 
				l_cColorOfLine = COLOR_GRAY;	//����ɫ			
		}
	}
	return l_cColorOfLine;
}

/*
*	��M���뼯�������ַ���
*/
CString COpenGCode::AnalysisAryM(CArray<class CString,class CString &>& aryM)
{
	int l_iSizeAryM = aryM.GetSize();
	CString l_strMCode = "";
	for(int i=0;i<l_iSizeAryM;i++)
	{
		if(i != 0)
			l_strMCode = l_strMCode + "\r\n";
		l_strMCode = l_strMCode + aryM.GetAt(i);
	}
	return l_strMCode;
}

/*
 *	�������M����
 */
COLORREF COpenGCode::DistriPointMCode()
{
	COLORREF l_cColorOfLine = g_pDoc->m_stcColorToM.m_color5;;
	//һ��ͼ�����ߵ�M����
	CString l_strOneM,l_strTwoM;
	//����M���뼯�Ĵ�С 
	CArray<class CString,class CString &> l_aryCopyOnceM;//����M����
	l_aryCopyOnceM.Copy(m_aryOnceM);
	m_aryOnceM.RemoveAll();
	int l_iSizeOne = l_aryCopyOnceM.GetSize();
	for(int i=0;i<l_iSizeOne-1;i++)
	{
		l_strOneM = l_aryCopyOnceM.GetAt(i);
		l_strTwoM = l_aryCopyOnceM.GetAt(i+1);
		if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM1&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM1)
		{
			l_cColorOfLine = g_pDoc->m_stcColorToM.m_color1;	//��һ
			m_aryOnceM.Add(l_strOneM);
			m_aryTwicM.Add(l_strTwoM);
			return l_cColorOfLine;
		}
		else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM2&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM2)
		{
			l_cColorOfLine = g_pDoc->m_stcColorToM.m_color2;	//����ɫ
			m_aryOnceM.Add(l_strOneM);
			m_aryTwicM.Add(l_strTwoM);
			return l_cColorOfLine;
		}
		else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM3&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM3)
		{
			l_cColorOfLine = g_pDoc->m_stcColorToM.m_color3;	//����ɫ
			m_aryOnceM.Add(l_strOneM);
			m_aryTwicM.Add(l_strTwoM);
			return l_cColorOfLine;
		}
		else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM4&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM4)
		{
			l_cColorOfLine = g_pDoc->m_stcColorToM.m_color4;	//����ɫ
			m_aryOnceM.Add(l_strOneM);
			m_aryTwicM.Add(l_strTwoM);
			return l_cColorOfLine;
		}
		else if (l_strOneM == g_pDoc->m_stcColorToM.m_SBM5&&l_strTwoM == g_pDoc->m_stcColorToM.m_CBM5)
		{
			l_cColorOfLine = g_pDoc->m_stcColorToM.m_color5;	//����ɫ
			m_aryOnceM.Add(l_strOneM);
			m_aryTwicM.Add(l_strTwoM);
			return l_cColorOfLine;
		}
		else 
			l_cColorOfLine = COLOR_GRAY;	//����ɫ			
	}
	return l_cColorOfLine;	
}


