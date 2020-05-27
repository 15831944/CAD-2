// OutputDXFCode.cpp: implementation of the COutputDXFCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "OutputDXFCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutputDXFCode::COutputDXFCode()
{

}

COutputDXFCode::~COutputDXFCode()
{

}

BOOL COutputDXFCode::OutputDXFCode(CString filename)
{

	// STEP 1 
	// ����һ���ļ�������g��ʽ��Ϣ
	if(!m_cDXFCode.Open(filename,CFile::modeCreate|CFile::modeWrite))
		return FALSE;
	// STEP 2 : ����Ϣд��g�ļ��У����밴����д�ļ�
	if( !WriteHeader() ||!WriteContent() ||!WriteTail() )		// д�ļ�
	{
		return FALSE;
	}
	// STEP 3 : �ر��ļ�
	m_cDXFCode.Close();
	return TRUE;
}

/*
 *	д�ļ�ͷ
 */
BOOL COutputDXFCode::WriteHeader()
{
	CString l_strHeader = "  0\nSECTION\n  2\nHEADER\n  9\n$ACADVER\n  1\nAC1009\n  9\n";
	m_cDXFCode.Write(l_strHeader,strlen(l_strHeader));
	l_strHeader = "$ANGBASE\n 50\n0.0\n  9\n$ANGDIR\n 70\n     0\n  9\n$LUNITS\n 70\n     0\n  0\n";
	m_cDXFCode.Write(l_strHeader,strlen(l_strHeader));
	l_strHeader = "ENDSEC\n  0\nSECTION\n  2\nENTITIES\n";
	m_cDXFCode.Write(l_strHeader,strlen(l_strHeader));
	return TRUE;
}

/*
 *	д�ļ���
 */
BOOL COutputDXFCode::WriteContent()
{
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	while(position != NULL)
	{
		CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
		//��������ѡ��ʲô����G����
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateLine)))
		{
			CCreateLine *line = (CCreateLine*)l_pCreateBase;
			WriteLine(line);	//ֱ��
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePolyline))
			||l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateSpline)))
		{
			CCreatePolyline *Polyline = (CCreatePolyline*)l_pCreateBase;
			WritePolyline(Polyline);	//���߶�
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRectangle)))
		{
			CCreateRectangle *Rectangle = (CCreateRectangle*)l_pCreateBase;
			WriteRectangle(Rectangle);//����
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRound)))
		{
			CCreateRound *Round = (CCreateRound*)l_pCreateBase;
			int A = Round->m_iDirect;
			WriteRound(Round);//Բ
		}

		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateRoundArc)))
		{
			CCreateRoundArc *Arc = (CCreateRoundArc*)l_pCreateBase;
			WriteArc(Arc);	//Բ��
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateEllipse)))
		{
			CCreateEllipse *Ellipse = (CCreateEllipse*)l_pCreateBase;
			WriteEllipse(Ellipse);//��Բ
		}
		else if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreatePoint)))
		{
			CCreatePoint *Point = (CCreatePoint*)l_pCreateBase;
			WritePoint(Point);//��
		}
	}
	return TRUE;
}

/*
 *	д�ļ�β
 */
BOOL COutputDXFCode::WriteTail()
{
	CString l_strHeader = "  0\nENDSEC\n  0\nEOF";
	m_cDXFCode.Write(l_strHeader,strlen(l_strHeader));
	return TRUE;
}

/*
 *	����ת��G����----��ֱ�ߵ�
 */
BOOL COutputDXFCode::WriteLine(CCreateLine* pLine)
{
	double x1,y1,x2,y2;	//���յ������
	CString l_content;	//д���ļ��ַ���������
	//��������
	x1 = pLine->m_Begin.x;
	y1 = pLine->m_Begin.y;
	//�յ������
	x2 = pLine->m_End.x;
	y2 = pLine->m_End.y;
	//�Ѷ�Ӧ��ɫ�������ʽ���ַ�����溬�⿴������ͷ�ļ����ע��
	//				ͼԪ����	ͼ��	  ��ɫ		���X	���Y								
	l_content.Format("  0\nLINE\n  8\n0\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n\
 11\n%.16f\n 21\n%.16f\n 31\n0.0\n",pLine->RtnColorInt()-1,x1,y1,x2,y2);
	//д���ļ���      �յ�X		�յ�Y					
	m_cDXFCode.Write(l_content,strlen(l_content));		
	return TRUE;
}

/*
 *	����ת��G����----��Բ��
 */
BOOL COutputDXFCode::WriteRound(CCreateRound* pRound)
{
	double x1,y1,radius;	//���յ������
	CString l_content;	//д���ļ��ַ���������
	//��������Ͱ뾶
	x1 = pRound->m_Center.x;
	y1 = pRound->m_Center.y;
	radius = pRound->m_dRadius;
	//�Ѷ�Ӧ��ɫ�������ʽ���ַ�����溬�⿴������ͷ�ļ����ע��
	//			    ͼԪ����        ͼ��	  ��ɫ		Բ��X	Բ��Y                �뾶
	l_content.Format("  0\nCIRCLE\n  8\n0\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n 40\n%.16f\n"\
					,pRound->RtnColorInt()-1,x1,y1,radius);//��Ҫ���ٶ�λ�����
	m_cDXFCode.Write(l_content,strlen(l_content));
	return TRUE;
}

/*
 *	����ת��G����----�����߶ε�
 */
BOOL COutputDXFCode::WritePolyline(CCreatePolyline* Polyline)
{
	int l_iPointSize = Polyline->m_point.GetSize();
	if(l_iPointSize<2)	//�ڶ����������ǻ�ʱ�����һ�㲻Ҫ��
		return FALSE;
	CString l_content;	//д���ļ��ַ���������
	Position pos;	//������е�ÿ����
	//      ���յ������
	double l_dPosx,l_dPosy,l_dTuDu;	
	//����ͼԪ����ɫ
	int l_iColor = Polyline->RtnColorInt()-1;
	//����߿�Ŀ�ͷ	ͼԪ����		ͼ��					��ɫ
	l_content.Format("  0\nPOLYLINE\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 66\n     1\n\
 10\n0.0\n 20\n0.0\n 30\n0.0\n",l_iColor);
	//д���ļ���
	m_cDXFCode.Write(l_content,strlen(l_content));
	//�ѵ�ȡ����Ȼ��д��DXF�ļ���
	for(int i=0  ;   i < l_iPointSize  ;i++)
	{
		pos = Polyline->m_point.GetAt(i);
		l_dTuDu = Polyline->m_dTuDu.GetAt(i);
		l_dPosx = pos.x;
		l_dPosy = pos.y;	
		if (l_dTuDu == 0.0)
		//����߿���м�����	ͼԪ����  ͼ��						��ɫ		 ��X		��Y
		l_content.Format("  0\nVERTEX\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n",
			l_iColor,l_dPosx,l_dPosy);//��һ����ٶ�λ�����
		else
		//����߿���м�����	ͼԪ����  ͼ��						��ɫ		 ��X		��Y
		l_content.Format("  0\nVERTEX\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n 42\n%.16f\n",
			l_iColor,l_dPosx,l_dPosy,l_dTuDu);//��һ����ٶ�λ�����
		//д���ļ�
		m_cDXFCode.Write(l_content,strlen(l_content));
	}
	//����߿�Ľ�������
	l_content.Format("  0\nSEQEND\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n",l_iColor);
	m_cDXFCode.Write(l_content,strlen(l_content));		
	return TRUE;
}

/*
 *	����ת��G����----����Բ��
 */
BOOL COutputDXFCode::WriteEllipse(CCreateEllipse* Ellipse)
{
	CString l_content;	//д���ļ��ַ���������
	Position pos;	//������е�ÿ����
	double l_dPosx,l_dPosy;	//���յ������
	//����ͼԪ����ɫ
	int l_iColor = Ellipse->RtnColorInt()-1;
	//����߿�Ŀ�ͷ	ͼԪ����		ͼ��					��ɫ
	l_content.Format("  0\nPOLYLINE\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 66\n     1\n\
 10\n0.0\n 20\n0.0\n 30\n0.0\n",l_iColor);
	//д���ļ���
	m_cDXFCode.Write(l_content,strlen(l_content));
	//�ѵ�ȡ����Ȼ��д��DXF�ļ���
	for(int i=0  ;   i < Ellipse->m_point.GetSize()  ;i++)
	{
		pos = Ellipse->m_point.GetAt(i);
		l_dPosx = pos.x;
		l_dPosy = pos.y;		
		//����߿���м�����	ͼԪ����  ͼ��						��ɫ     ��X		��Y
		l_content.Format("  0\nVERTEX\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n",
			l_iColor,l_dPosx,l_dPosy);//��һ����ٶ�λ�����
		//д���ļ�
		m_cDXFCode.Write(l_content,strlen(l_content));
	}
	//����߿�Ľ�������
	l_content.Format("  0\nSEQEND\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n",l_iColor);
	m_cDXFCode.Write(l_content,strlen(l_content));	
	
	return TRUE;
}

/*
 *	����ת��G����----��Բ����
 */
BOOL COutputDXFCode::WriteArc(CCreateRoundArc* pArc)
{
	
//	"0\nARC\n  8\n0\n  10\n%f\n  20\n%f\n  30\n0.0\n  40\n%f  50\n%f\n  51\n%f\n"
	//	    ���X			���Y		�뾶		��ʼ�Ƕ�   ��ֹ�Ƕ�
	double l_dCenterPosX,l_dCenterPosY,l_dRadius,l_dAngleBG,  l_dAngleEN;	
	//Բ�ĵ�����
	l_dCenterPosX = pArc->m_Center.x;
	l_dCenterPosY = pArc->m_Center.y;
	//�뾶
	l_dRadius = pArc->m_Begin.Distance(pArc->m_Center);
	//��ʼ�ǶȺ���ֹ�Ƕ�
	l_dAngleBG = GetAngle(pArc->m_Center,pArc->m_Begin);
	l_dAngleEN = GetAngle(pArc->m_Center,pArc->m_End);
	//д���ļ��ַ���������
	CString l_strCcontent;	
	//�ж�һ������ת
	if (pArc->m_iDirect == AD_COUNTERCLOCKWISE)//
		//			          ͼԪ����	 ͼ��	 ��ɫ      Բ��X	 Բ��Y							   �뾶	   ��ʼ�Ƕ�   ��ֹ�Ƕ�
		l_strCcontent.Format("  0\nARC\n  8\n0\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n 40\n%.16f\n 50\n%.16f\n 51\n%.16f\n", \
			pArc->RtnColorInt()-1,l_dCenterPosX,l_dCenterPosY,l_dRadius,l_dAngleBG,l_dAngleEN);
	else
		//			          ͼԪ����	 ͼ��	 ��ɫ					 Բ��X	 Բ��Y		           �뾶	   ��ʼ�Ƕ�   ��ֹ�Ƕ�
		l_strCcontent.Format("  0\nARC\n  8\n0\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n 40\n%.16f\n 50\n%.16f\n 51\n%.16f\n", \
			pArc->RtnColorInt()-1,l_dCenterPosX,l_dCenterPosY,l_dRadius,l_dAngleEN,l_dAngleBG);
	//д���ļ�
	m_cDXFCode.Write(l_strCcontent,strlen(l_strCcontent));
	return TRUE;
}

/*
 *	����ת��G����----�����ε�
 */
BOOL COutputDXFCode::WriteRectangle(CCreateRectangle* pRectangle)
{
	int l_iPointSize = pRectangle->m_point.GetSize();
	if(l_iPointSize<2)	//�ڶ����������ǻ�ʱ�����һ�㲻Ҫ��
		return FALSE;
	CString l_content;	//д���ļ��ַ���������
	Position pos;	//������е�ÿ����
	//      ���յ������
	double l_dPosx,l_dPosy,l_dTuDu;	
	//����ͼԪ����ɫ
	int l_iColor = pRectangle->RtnColorInt()-1;
	//����߿�Ŀ�ͷ	ͼԪ����		ͼ��					��ɫ
	l_content.Format("  0\nPOLYLINE\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 66\n     1\n\
		10\n0.0\n 20\n0.0\n 30\n0.0\n",l_iColor);
	//д���ļ���
	m_cDXFCode.Write(l_content,strlen(l_content));
	//�ѵ�ȡ����Ȼ��д��DXF�ļ���
	for(int i=0  ;   i < l_iPointSize  ;i++)
	{
		pos = pRectangle->m_point.GetAt(i);
		l_dTuDu = pRectangle->m_dTuDu.GetAt(i);
		l_dPosx = pos.x;
		l_dPosy = pos.y;	
		if (l_dTuDu == 0.0)
			//����߿���м�����	ͼԪ����  ͼ��						��ɫ		 ��X		��Y
			l_content.Format("  0\nVERTEX\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n",
			l_iColor,l_dPosx,l_dPosy);//��һ����ٶ�λ�����
		else
			//����߿���м�����	ͼԪ����  ͼ��						��ɫ		 ��X		��Y
			l_content.Format("  0\nVERTEX\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n 42\n%.16f\n",
			l_iColor,l_dPosx,l_dPosy,l_dTuDu);//��һ����ٶ�λ�����
		//д���ļ�
		m_cDXFCode.Write(l_content,strlen(l_content));
	}
	//����߿�Ľ�������
	l_content.Format("  0\nSEQEND\n  8\n0\n  6\nCONTINUOUS\n 62\n     %d\n",l_iColor);
	m_cDXFCode.Write(l_content,strlen(l_content));			
	return TRUE;
}

/*
 *	����ת��G����----�����
 */
BOOL COutputDXFCode::WritePoint(CCreatePoint* Point)
{
	double x1,y1;	//���յ������
	CString l_content;	//д���ļ��ַ���������
	//�������
	x1 = Point->m_Begin.x;
	y1 = Point->m_Begin.y;

	//�Ѷ�Ӧ��ɫ�������ʽ���ַ�����溬�⿴������ͷ�ļ����ע��
	//				ͼԪ����	ͼ��	  ��ɫ		���X	���Y								
	l_content.Format("  0\nPOINT\n  8\n0\n 62\n     %d\n 10\n%.16f\n 20\n%.16f\n 30\n0.0\n",Point->RtnColorInt()-1,x1,y1);
	//д���ļ���      �յ�X		�յ�Y					
	m_cDXFCode.Write(l_content,strlen(l_content));	
	return TRUE;
}