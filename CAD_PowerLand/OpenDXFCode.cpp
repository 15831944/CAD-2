// OpenDXFCode.cpp: implementation of the COpenDXFCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "OpenDXFCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COpenDXFCode::COpenDXFCode()
{

}

COpenDXFCode::~COpenDXFCode()
{
	m_point.RemoveAll();
}

BOOL COpenDXFCode::ReadDXFCode(CString PathOfFile)
{
	CString  l_strLineContent;	//ÿ���ı�������
	CFileException ex;	
	BOOL isEof=FALSE;
	BOOL aaa = FALSE;//ֱ��������ENTITIES��ʱ�Ž���ͼ��
	if (!m_cReadDXFCode.Open(PathOfFile,CFile::modeRead | CFile::shareDenyWrite, &ex)) //���ļ�		
	{	
		//���û�򿪾ͱ���
		TCHAR szError[1024];		
		ex.GetErrorMessage(szError, 1024);		
		AfxMessageBox(szError);		
		return  FALSE;		
	}
	else		
	{	//��ȡ�ı�
		for(int i = 1;;i++)
		{	
			//�ӵ�һ�п�ʼ����
			isEof=!m_cReadDXFCode.ReadString(l_strLineContent);//��ȡһ���ı���������һ���˾���isEof==1			
			if(isEof)//���û���˾���1���˳�
				break;	
			if(l_strLineContent == "ENTITIES")
				aaa = TRUE;
			if(l_strLineContent == "LINE"&& aaa == TRUE )
			{
				TurnLine();			//��DXF���ֱ����Ϣת���ɱ������ֱ����Ϣ
			}
			else if(l_strLineContent == "ARC"&& aaa == TRUE)
			{
				TurnArc();		//��DXF�ļ��з���ARC��ת��Բ��
			}
			else if(l_strLineContent == "CIRCLE"&& aaa == TRUE)
			{
				TurnCircle();	//��DXF�ļ��з���CIRCLE��ת��Բ
			}
			else if(l_strLineContent == "POLYLINE"&& aaa == TRUE)	
			{
				TurnPolyline();		//��DXF�ļ��з���POLYLINE��ת�ɶ��߶�
			}
			else if(l_strLineContent == "POINT"&& aaa == TRUE)	
			{
				TurnPoint();		//��DXF�ļ��з���POLYLINE��ת�ɶ��߶�
			}
			else if(l_strLineContent == "EOF"&& aaa == TRUE)
			{
				break;
			}
		}
		g_pView->Invalidate();
	}		
	return TRUE;
}

/*
 *��DXF���ֱ����Ϣת���ɱ������ֱ����Ϣ	
 */
void COpenDXFCode::TurnLine()
{
	CString l_strLineContent;
	Position l_cBeginPos,l_cEndPos;
	COLORREF l_cColor =g_pDoc->m_stcColorToM.m_color5;	//��ɫ

	while(l_strLineContent != "  0")//����߽���
	{
		m_cReadDXFCode.ReadString(l_strLineContent);
		if(l_strLineContent == " 62")	//�����62˵��������ɫ�ģ�62��������ɫ�ţ��̶���
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cColor = g_pDoc->RtnColorRGB(atoi(l_strLineContent));	//�ǵڼ�����ɫ	
		}
		else if(l_strLineContent == " 10")	//�ߵ��� 10�������棬��������ʼ��X������
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cBeginPos.x = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 20")	//�ߵ��� 20�������棬��������ʼ��Y������
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cBeginPos.y = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 11")	//�ߵ��� 10�������棬��������ʼ��X������
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cEndPos.x = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 21")	//�ߵ��� 20�������棬��������ʼ��Y������
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cEndPos.y = atof(l_strLineContent);	
		}
	}
	//���浽������
	CCreateLine* l_pTempLine = new CCreateLine(l_cBeginPos,l_cEndPos);	
	l_pTempLine->m_ColorOfLine = l_cColor;		//�ѻ�õ�����ɫ�ӵ�ͼԪ��Ȼ������ӽ�����
	CString l_strSBM,l_strCBM;//����M����
	g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_cColor);//������ɫ����M����
	l_pTempLine->m_strSBM.RemoveAll();
	l_pTempLine->m_strCBM.RemoveAll();	
	l_pTempLine->m_strSBM.Add(l_strSBM);
	l_pTempLine->m_strCBM.Add(l_strCBM);
				
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempLine);	
}

//��DXF�ļ��з���ARC��ת��Բ��
void COpenDXFCode::TurnArc()	
{
	CString l_strLineContent;
	Position l_cBeginPos,l_cEndPos,l_cCenter;//���գ����ĵ�
	double l_cRadius,l_cAngleBegin,l_cAngleEnd;	//�뾶�����Ƕȣ�ĩ��Ƕ�
	COLORREF l_cColor =g_pDoc->m_stcColorToM.m_color5;	//��ɫ

	while(l_strLineContent != "  0")//����߽���
	{
		m_cReadDXFCode.ReadString(l_strLineContent);
		if(l_strLineContent == " 62")	//�����62˵��������ɫ�ģ�62��������ɫ�ţ��̶���
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cColor = g_pDoc->RtnColorRGB(atoi(l_strLineContent));	//�ǵڼ�����ɫ	
		}
		else if(l_strLineContent == " 10")	//�ߵ��� 10�������棬������Բ�ĵ�X������
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cCenter.x = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 20")	//�ߵ��� 20�������棬������Բ�ĵ�Y������
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cCenter.y = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 40")	//�ߵ���40�������棬�����ǰ뾶
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cRadius = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 50")	//�ߵ��� 50�������棬���������Ƕ�
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cAngleBegin = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 51")	//�ߵ��� 51�������棬�������յ�Ƕ�
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cAngleEnd = atof(l_strLineContent);	
		}
	}
	//����㣬�յ㡣
	l_cBeginPos.x = l_cCenter.x + l_cRadius*cos(MATH_PI*l_cAngleBegin/180);
	l_cBeginPos.y = l_cCenter.y + l_cRadius*sin(MATH_PI*l_cAngleBegin/180);

	l_cEndPos.x = l_cCenter.x + l_cRadius*cos(MATH_PI*l_cAngleEnd/180);
	l_cEndPos.y = l_cCenter.y + l_cRadius*sin(MATH_PI*l_cAngleEnd/180);

	//���浽������
	CCreateRoundArc* l_pTempArc = new CCreateRoundArc(l_cBeginPos,l_cCenter,l_cEndPos,AD_COUNTERCLOCKWISE);
	CString l_strSBM,l_strCBM;//����M����
	g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_cColor);//������ɫ����M����
	l_pTempArc->m_strSBM.RemoveAll();
	l_pTempArc->m_strCBM.RemoveAll();	
	l_pTempArc->m_strSBM.Add(l_strSBM);
	l_pTempArc->m_strCBM.Add(l_strCBM);
	l_pTempArc->m_ColorOfLine = l_cColor;		//�ѻ�õ�����ɫ�ӵ�ͼԪ��Ȼ������ӽ�����
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempArc);		
}

//��DXF�ļ��з���CIRCLE��ת��Բ
void COpenDXFCode::TurnCircle()
{
	CString l_strLineContent;
	Position l_cCenter,l_cEndPos;//���գ����ĵ�
	double l_cRadius;	//�뾶
	COLORREF l_cColor =g_pDoc->m_stcColorToM.m_color5;	//��ɫ

	while(l_strLineContent != "  0")//����߽���
	{
		m_cReadDXFCode.ReadString(l_strLineContent);
		if(l_strLineContent == " 62")	//�����62˵��������ɫ�ģ�62��������ɫ�ţ��̶���
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cColor = g_pDoc->RtnColorRGB(atoi(l_strLineContent));	//�ǵڼ�����ɫ	
		}
		else if(l_strLineContent == " 10")	//�ߵ��� 10�������棬��������ʼ��X������
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cCenter.x = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 20")	//�ߵ��� 20�������棬��������ʼ��Y������
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cCenter.y = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 40")	//�ߵ���40�������棬�����ǰ뾶
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cRadius = atof(l_strLineContent);	
		}
	}
	//���յ㡣
	l_cEndPos.x = l_cCenter.x + l_cRadius;
	l_cEndPos.y = l_cCenter.y;	
	//���浽������
	CCreateRound* l_pTempRound = new CCreateRound(l_cCenter,l_cEndPos,AD_COUNTERCLOCKWISE);
	CString l_strSBM,l_strCBM;//����M����
	g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_cColor);//������ɫ����M����
	l_pTempRound->m_strSBM.RemoveAll();
	l_pTempRound->m_strCBM.RemoveAll();	
	l_pTempRound->m_strSBM.Add(l_strSBM);
	l_pTempRound->m_strCBM.Add(l_strCBM);
	l_pTempRound->m_ColorOfLine = l_cColor;		//�ѻ�õ�����ɫ�ӵ�ͼԪ��Ȼ������ӽ�����
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempRound);			
}

//��DXF�ļ��з���POLYLINE��ת�ɶ��߶�
void COpenDXFCode::TurnPolyline()
{
	CString l_strLineContent;
	//   �����еĵ�		���ĵ�		��ʼ��  	������
	Position l_cPos,l_PosCenter,l_PosBegin,l_PosEnd;
	COLORREF l_cColor = g_pDoc->m_stcColorToM.m_color5;	//��ɫ
		//�Ƿ���Բ��	Բ���ķ���         
	int l_iNumOfPoint = 1,l_iClockWise = 0;
	double l_dArcParm;	//͹��
	//    �ǲ��ǻ�   �Ƿ��Ǳպϵ�
	BOOL l_bIsArc,l_bIsRect=FALSE;
	//ÿ������͹��
	CArray<double,double&> TuDu;//͹��
	//�ڼ�Ķ��߶�
	CArray<Position,Position&> l_point;
	
	while(l_strLineContent != "  0")//���ǲ��Ǳպϵ�
	{
		m_cReadDXFCode.ReadString(l_strLineContent);
		if (l_strLineContent == " 70")
		{			
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_bIsRect = atoi(l_strLineContent);	//Ϊ1�Ǳպϵ�				
		}
	}
	//3,�������������ɨ��//////////////////////////////////////////////
	while (l_strLineContent != "SEQEND")//ɨ�����PolyLineֱ��������SEQEND������
	{	
		m_cReadDXFCode.ReadString(l_strLineContent);
		//2,/////////////////////���������VERTEX��˵����һ������///////////////////////
		if (l_strLineContent == "VERTEX") //������VERTEX��˵����һ������
		{	
			l_bIsArc = FALSE;	//��ʼ��û�л��ı�־�����������42˵���л��ı�־��
			//1,//////////////////��������ɨ��///////////////////////////////
			while(l_strLineContent != "  0")//����߽���
			{
				m_cReadDXFCode.ReadString(l_strLineContent);
				if(l_strLineContent == " 62")	//�����62˵��������ɫ�ģ�62��������ɫ�ţ��̶���
				{
					m_cReadDXFCode.ReadString(l_strLineContent);
					l_cColor = g_pDoc->RtnColorRGB(atoi(l_strLineContent));	//�ǵڼ�����ɫ	
				}
				else if(l_strLineContent == " 10")	//�ߵ��� 10�������棬��������ʼ��X������
				{
					m_cReadDXFCode.ReadString(l_strLineContent);
					l_cPos.x = atof(l_strLineContent);	
				}
				else if(l_strLineContent == " 20")	//�ߵ��� 20�������棬��������ʼ��Y������
				{
					m_cReadDXFCode.ReadString(l_strLineContent);
					l_cPos.y = atof(l_strLineContent);	
				}	

				else if(l_strLineContent == " 42")	//�ߵ��� 42�������棬��������Բ��
				{	
					l_bIsArc = TRUE;	//��־�ǻ�
					m_cReadDXFCode.ReadString(l_strLineContent);
					l_dArcParm = atof(l_strLineContent);//͹��
				}
				l_iNumOfPoint++;
			}	
			//1./////////////////////��������ɨ��///////////////////////////////////
			m_point.Add(l_cPos);	//ɨ����ĵ�Ҫ�ŵ��㼯��
			if (l_bIsArc == FALSE)	//�������Բ��
			{
				double l_dZore = 0;
				TuDu.Add(l_dZore);
			}
			else	//������Բ��
				TuDu.Add(l_dArcParm);
		//2.////////////////���������VERTEX��˵����һ������///////////////////////////////
		}
	}
	//3.�������������ɨ��/////////////////////////////////////////////////////////////
	if(l_bIsRect == TRUE)	//ɨ���������Ǳպϵ�ͼ��,���ٰѵ�һ�����
	{
		double l_dZore1 = 0.0;
		m_point.Add(m_point.GetAt(0));	//�Ͱѵ�һ���ټӽ�ȥ
		TuDu.Add(l_dZore1);//͹��Ϊ0
	}
	//4�������Ѿ�ɨ�����ˣ�����Ҫ�鿴�ж��ٸ��㣬���������ǻ����������¾�������ͼ��	
	CCreatePolyline* l_pTempPolyline = new CCreatePolyline(m_point,TuDu);
	CString l_strSBM,l_strCBM;//����M����
	g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_cColor);//������ɫ����M����
	l_pTempPolyline->m_strSBM.RemoveAll();
	l_pTempPolyline->m_strCBM.RemoveAll();	
	l_pTempPolyline->m_strSBM.Add(l_strSBM);
	l_pTempPolyline->m_strCBM.Add(l_strCBM);
	l_pTempPolyline->m_ColorOfLine = l_cColor;		//�ѻ�õ�����ɫ�ӵ�ͼԪ��Ȼ������ӽ�����
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempPolyline);	//�ӵ�������
	//�����������
	m_point.RemoveAll();
	TuDu.RemoveAll();
}
/*
 *	��DXF�ļ��з���POINT��ת�ɶ��߶�
 */
void COpenDXFCode::TurnPoint()
{
	CString l_strLineContent;
	Position l_cBeginPos;
	COLORREF l_cColor =g_pDoc->m_stcColorToM.m_color5;	//��ɫ
	
	while(l_strLineContent != "  0")//����߽���
	{
		m_cReadDXFCode.ReadString(l_strLineContent);
		if(l_strLineContent == " 62")	//�����62˵��������ɫ�ģ�62��������ɫ�ţ��̶���
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cColor = g_pDoc->RtnColorRGB(atoi(l_strLineContent));	//�ǵڼ�����ɫ	
		}
		else if(l_strLineContent == " 10")	//�ߵ��� 10�������棬��������ʼ��X������
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cBeginPos.x = atof(l_strLineContent);	
		}
		else if(l_strLineContent == " 20")	//�ߵ��� 20�������棬��������ʼ��Y������
		{
			m_cReadDXFCode.ReadString(l_strLineContent);
			l_cBeginPos.y = atof(l_strLineContent);	
		}
	}
	//���浽������
	CCreatePoint* l_pTempLine = new CCreatePoint(l_cBeginPos);	
	CString l_strSBM,l_strCBM;//����M����
	g_pDoc->RtnColorMCode(l_strSBM,l_strCBM,-1,l_cColor);//������ɫ����M����
	l_pTempLine->m_strSBM.RemoveAll();
	l_pTempLine->m_strCBM.RemoveAll();	
	l_pTempLine->m_strSBM.Add(l_strSBM);
	l_pTempLine->m_strCBM.Add(l_strCBM);
	l_pTempLine->m_ColorOfLine = l_cColor;		//�ѻ�õ�����ɫ�ӵ�ͼԪ��Ȼ������ӽ�����
	g_pDoc->m_CCreateBaseList.AddTail(l_pTempLine);	
}
