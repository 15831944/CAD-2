// OutputGCode.cpp: implementation of the COutputGCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "OutputGCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutputGCode::COutputGCode()
{
	m_dCurX = m_dCurY = 0;	//��ǰ�ĵ�	
	m_strCBM_What = "";	//����M����
	m_iColorLast = -1;	//������ɫ
}

COutputGCode::~COutputGCode()
{

}

/*
 *	д�ļ�-----��
 */
BOOL COutputGCode::OutputGCode(CString filename)
{
// STEP 1 
	// ����һ���ļ�������g��ʽ��Ϣ
	if(!m_cGCode.Open(filename,CFile::modeCreate|CFile::modeWrite))
		return FALSE;
// STEP 2 : ����Ϣд��g�ļ��У����밴����д�ļ�
	if( !WriteHeader() ||!WriteContent() ||!WriteTail() )		// д�ļ�
	{
		return FALSE;
	}
// STEP 3 : �ر��ļ�
	m_cGCode.Close();
	return TRUE;
}

/*
 *	д�ļ�ͷ
 */
BOOL COutputGCode::WriteHeader()
{
	m_cGCode.Write("G90\r\n",sizeof("G90\n"));
	m_bFirstG00 = TRUE;
	CString str;
	//���ļ�ͷ�ļ���M����
	POSITION	position = g_pDoc->m_CCreateBaseList.GetHeadPosition();
	g_pDoc->m_CCreateBaseList.GetNext(position);
	while(position != NULL)
	{
		CCreateBase*	l_pCreateBase = (CCreateBase *)g_pDoc->m_CCreateBaseList.GetNext(position);
		//��������ѡ��ʲô����G����
		if (l_pCreateBase->IsKindOf(RUNTIME_CLASS(CCreateMCode)))
		{
			CCreateMCode *l_pMCode = (CCreateMCode*)l_pCreateBase;
			str = l_pMCode->m_strM_What + "\r\n";
			m_cGCode.Write(str,strlen(str));
		}
		else
			break;
	}
	return TRUE;
}

/*
 *	д�ļ���
 */
BOOL COutputGCode::WriteContent()
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
BOOL COutputGCode::WriteTail()
{
	CString l_str="\r\nG00X0Y0Z0\r\n";
	l_str = m_strCBM_What + l_str+ "M23\r\nM24\r\nM30";//;
	m_cGCode.Write(l_str,strlen(l_str));	
	return TRUE;

}

/*
 *	����ת��G����----��ֱ�ߵ�
 */
BOOL COutputGCode::WriteLine(CCreateLine* pLine)
{
	double x1,y1,x2,y2;	//���յ������
	CString l_content,l_strSBM_What;	//д���ļ��ַ���������
	//��������
	x1 = pLine->m_Begin.x;
	y1 = pLine->m_Begin.y;
	//�Ѷ�ӦM���뼯ת���ַ���M
	l_strSBM_What = AnalysisAryM(pLine->m_strSBM);
	//				�����һ�����һ�㲻���							��ɫ��һ��   ��Ҫ��һ��G00
	if(fabs(m_dCurX-x1)>DISTANCE_ZERO||fabs(m_dCurY-y1)>DISTANCE_ZERO||(m_iColorLast != pLine->m_ColorOfLine))
	{
		if(m_bFirstG00 == TRUE)//����ǵ�һ��������һ�У�\r\n��
			l_content.Format("G00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����
		else
			l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����		
		m_bFirstG00 = FALSE;
		if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//�����ĥ�����Ĺ��վ�Ҫ��G41��G42
			l_content.Format("G%d\r\nG00 X%f Y%f\r\n",pLine->m_iKnifeStyle,x1,y1);//��Ҫ���ٶ�λ�����
		l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";
		m_cGCode.Write(l_content,strlen(l_content));		
	}
	//�յ������
	m_dCurX = x2 = pLine->m_End.x;
	m_dCurY = y2 = pLine->m_End.y;
	l_content.Format("G01 X%f Y%f\r\n",x2,y2);
	m_cGCode.Write(l_content,strlen(l_content));
	//�Ѷ�Ӧ��ɫת���ַ���M����������
	m_strCBM_What = AnalysisAryM(pLine->m_strCBM);	
	//��������ɫ��������
	m_iColorLast = pLine->m_ColorOfLine;
	return TRUE;
}

/*
 *	����ת��G����----��Բ��
 */
BOOL COutputGCode::WriteRound(CCreateRound* pRound)
{
	double x1,y1;	//���յ������
	CString l_content,l_strSBM_What;	//д���ļ��ַ���������
	//��������
	x1 = pRound->m_EndPos.x;
	y1 = pRound->m_EndPos.y;
	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(pRound->m_strSBM);
	
	//				�����һ�����һ�㲻���							��ɫ��һ��   ��Ҫ��һ��G00
	if(fabs(m_dCurX-x1)>DISTANCE_ZERO||fabs(m_dCurY-y1)>DISTANCE_ZERO||(m_iColorLast != pRound->m_ColorOfLine))
	{
		if(m_bFirstG00 == TRUE)//����ǵ�һ��������һ�У�\r\n��
			l_content.Format("G00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����
		else
			l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����		
		m_bFirstG00 = FALSE;
		if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//�����ĥ�����Ĺ��վ�Ҫ��G41��G42
			l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",pRound->m_iKnifeStyle,x1,y1);//��Ҫ���ٶ�λ�����
		l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";
		m_cGCode.Write(l_content,strlen(l_content));
	}
	//�ѵ�ǰ�ĵ㸺��ֵ
	m_dCurX = x1;
	m_dCurY = y1;
	//��Բ
	int a = pRound->m_iDirect;
	if (pRound->m_iDirect == AD_CLOCKWISE)//�����˳ʱ��
		l_content.Format("G02 X%f Y%f I%f J%f\r\n",x1,y1,pRound->m_Center.x-pRound->m_EndPos.x,pRound->m_Center.y-pRound->m_EndPos.y);
	else//������ʱ��
		l_content.Format("G03 X%f Y%f I%f J%f\r\n",x1,y1,pRound->m_Center.x-pRound->m_EndPos.x,pRound->m_Center.y-pRound->m_EndPos.y);
	m_cGCode.Write(l_content,strlen(l_content));
	//�Ѷ�Ӧ��ɫת���ַ���M
	m_strCBM_What = AnalysisAryM(pRound->m_strCBM);	
	return TRUE;
}

/*
 *	����ת��G����----�����߶ε�
 */
BOOL COutputGCode::WritePolyline(CCreatePolyline* Polyline)
{
	//�������Բ��
	Position pos,posNext,l_posCenter;	
	//���յ������,͹��
	double x1,y1,x2,y2,l_dTuDu;	
	CString l_content,l_strSBM_What;	//д���ļ��ַ���������

	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(Polyline->m_strSBM);
	//�㼯�Ķ���
	int l_iSizeOfPoint = Polyline->m_point.GetSize();
	//���ɴ���
	for(int i=0;i<l_iSizeOfPoint-1;i++)
	{
		pos = Polyline->m_point.GetAt(i);
		posNext = Polyline->m_point.GetAt(i+1);
		l_dTuDu = Polyline->m_dTuDu.GetAt(i);
		//�������
		x1 = pos.x;
		y1 = pos.y;
		x2 = posNext.x;
		y2 = posNext.y;
		////����ߵĵ�һ���Ƿ�����һ����ͬ,��ɫ�Ƿ���ͬ 
		if((i==0)&&(fabs(m_dCurX-x1)>DISTANCE_ZERO||fabs(m_dCurY-y1)>DISTANCE_ZERO||m_iColorLast != Polyline->m_ColorOfLine))
		{
			if(m_bFirstG00 == TRUE)//����ǵ�һ��������һ�У�\r\n��
				l_content.Format("G00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����
			else
				l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����		
			m_bFirstG00 = FALSE;
			if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//�����ĥ�����Ĺ��վ�Ҫ��G41��G42
				l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",Polyline->m_iKnifeStyle,x1,y1);//��Ҫ���ٶ�λ�����
			l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";		
			m_cGCode.Write(l_content,strlen(l_content));
		}
		if(l_dTuDu == 0.0)//	ֱ��
			l_content.Format("G01 X%f Y%f\r\n",x2,y2);
		else//Բ��
		{
			g_pDoc->polylineToArc(pos,posNext,l_dTuDu,l_posCenter);	
			if (l_dTuDu<0.0)//�����˳ʱ��
				l_content.Format("G02 X%f Y%f I%f J%f\r\n",x2,y2,l_posCenter.x - pos.x,l_posCenter.y - pos.y);
			else//������ʱ��
				l_content.Format("G03 X%f Y%f I%f J%f\r\n",x2,y2,l_posCenter.x - pos.x,l_posCenter.y - pos.y);			
		}
		//д��G�ļ���
		m_cGCode.Write(l_content,strlen(l_content));
		
		m_dCurX = x2;
		m_dCurY = y2;
	}
	//�Ѷ�Ӧ��ɫת���ַ���M
	m_strCBM_What = AnalysisAryM(Polyline->m_strCBM);	
	//��������ɫ��������
	m_iColorLast = Polyline->m_ColorOfLine;
	return TRUE;
}

/*
 *	����ת��G����----����Բ��
 */
BOOL COutputGCode::WriteEllipse(CCreateEllipse* Ellipse)
{
	Position pos;	//���
	double x1,y1;	//���յ������
	CString l_content,l_strSBM_What;	//д���ļ��ַ���������
	
	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(Ellipse->m_strSBM);
	//���ɴ���
	for(int i=0  ;   i < Ellipse->m_point.GetSize()  ;i++)
	{
		pos = Ellipse->m_point.GetAt(i);
		x1 = pos.x;
		y1 = pos.y;		
		if (i!=0)//��ʱΪ��һ��
		{
			l_content.Format("G01 X%f Y%f\r\n",x1,y1);
			m_cGCode.Write(l_content,strlen(l_content));
		}
		else
		{
			m_dCurX = x1;
			m_dCurY = y1;
			if(m_bFirstG00 == TRUE)//����ǵ�һ��������һ�У�\r\n��
				l_content.Format("G00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����
			else
				l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����		
			m_bFirstG00 = FALSE;
			if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//�����ĥ�����Ĺ��վ�Ҫ��G41��G42
				l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",Ellipse->m_iKnifeStyle,x1,y1);//��һ����ٶ�λ�����
			l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";
			m_cGCode.Write(l_content,strlen(l_content));
		}
	}
	//�Ѷ�Ӧ��ɫת���ַ���M
	m_strCBM_What = AnalysisAryM(Ellipse->m_strCBM);	
	return TRUE;
}

/*
 *	����ת��G����----��Բ����
 */
BOOL COutputGCode::WriteArc(CCreateRoundArc* pArc)
{	
	double x1,y1;	//���յ������
	CString l_content,l_strSBM_What;	//д���ļ��ַ���������
	//��������
	x1 = pArc->m_Begin.x;
	y1 = pArc->m_Begin.y;
	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(pArc->m_strSBM);
	
	//				�����һ�����һ�㲻���							��ɫ��һ��   ��Ҫ��һ��G00
	if(fabs(m_dCurX-x1)>DISTANCE_ZERO||fabs(m_dCurY-y1)>DISTANCE_ZERO||(m_iColorLast != pArc->m_ColorOfLine))
	{
		if(m_bFirstG00 == TRUE)//����ǵ�һ��������һ�У�\r\n��
			l_content.Format("G00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����
		else
			l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����		
		m_bFirstG00 = FALSE;
		if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//�����ĥ�����Ĺ��վ�Ҫ��G41��G42
			l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",pArc->m_iKnifeStyle,x1,y1);//��Ҫ���ٶ�λ�����
		l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";
		m_cGCode.Write(l_content,strlen(l_content));			
	}	
	
	//��Բ��
	m_dCurX = x1 = pArc->m_End.x;
	m_dCurY = y1 = pArc->m_End.y;

	if (pArc->m_iDirect == AD_CLOCKWISE)//�����˳ʱ��
		l_content.Format("G02 X%f Y%f I%f J%f\r\n",x1,y1,   \
		pArc->m_Center.x - pArc->m_Begin.x   ,pArc->m_Center.y - pArc->m_Begin.y);
	else//������ʱ��
		l_content.Format("G03 X%f Y%f I%f J%f\r\n",x1,y1,   \
		pArc->m_Center.x - pArc->m_Begin.x   ,pArc->m_Center.y - pArc->m_Begin.y);
	m_cGCode.Write(l_content,strlen(l_content));
	//�Ѷ�Ӧ��ɫת���ַ���M
	m_strCBM_What = AnalysisAryM(pArc->m_strCBM);	
	//��������ɫ��������
	m_iColorLast = pArc->m_ColorOfLine;
	return TRUE;
}

/*
 *	����ת��G����----�����ε�
 */
BOOL COutputGCode::WriteRectangle(CCreateRectangle* pRectangle)
{
	//�������Բ��
	Position pos,posNext,l_posCenter;	
	//���յ������,͹��
	double x1,y1,x2,y2,l_dTuDu;	
	CString l_content,l_strSBM_What;	//д���ļ��ַ���������
	
	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(pRectangle->m_strSBM);
	//�㼯�Ķ���
	int l_iSizeOfPoint = pRectangle->m_point.GetSize();
	//���ɴ���
	for(int i=0;i<l_iSizeOfPoint-1;i++)
	{
		pos = pRectangle->m_point.GetAt(i);
		posNext = pRectangle->m_point.GetAt(i+1);
		l_dTuDu = pRectangle->m_dTuDu.GetAt(i);
		//�������
		x1 = pos.x;
		y1 = pos.y;
		x2 = posNext.x;
		y2 = posNext.y;
		////����ߵĵ�һ���Ƿ�����һ����ͬ,��ɫ�Ƿ���ͬ 
		if((i==0)&&(fabs(m_dCurX-x1)>DISTANCE_ZERO||fabs(m_dCurY-y1)>DISTANCE_ZERO||m_iColorLast != pRectangle->m_ColorOfLine))
		{
			if(m_bFirstG00 == TRUE)//����ǵ�һ��������һ�У�\r\n��
				l_content.Format("G00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����
			else
				l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����		
			m_bFirstG00 = FALSE;
			if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//�����ĥ�����Ĺ��վ�Ҫ��G41��G42
				l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",pRectangle->m_iKnifeStyle,x1,y1);//��Ҫ���ٶ�λ�����
			l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";		
			m_cGCode.Write(l_content,strlen(l_content));
		}
		if(l_dTuDu == 0.0)//	ֱ��
			l_content.Format("G01 X%f Y%f\r\n",x2,y2);
		else//Բ��
		{
			g_pDoc->polylineToArc(pos,posNext,l_dTuDu,l_posCenter);	
			if (l_dTuDu<0.0)//�����˳ʱ��
				l_content.Format("G02 X%f Y%f I%f J%f\r\n",x2,y2,l_posCenter.x - pos.x,l_posCenter.y - pos.y);
			else//������ʱ��
				l_content.Format("G03 X%f Y%f I%f J%f\r\n",x2,y2,l_posCenter.x - pos.x,l_posCenter.y - pos.y);			
		}
		//д��G�ļ���
		m_cGCode.Write(l_content,strlen(l_content));
		
		m_dCurX = x2;
		m_dCurY = y2;
	}
	//�Ѷ�Ӧ��ɫת���ַ���M
	m_strCBM_What = AnalysisAryM(pRectangle->m_strCBM);	
	//��������ɫ��������
	m_iColorLast = pRectangle->m_ColorOfLine;
	return TRUE;
}

/*
 *	������ɫת���ַ�̧��M���ٵĺ���
 */
CString COutputGCode::CintToCBMstr(int RtnColor)
{
	CString l_strM;		//������ɫת���ַ�
	//�ַ����ⲿ��Ӧ�趨��
	switch(RtnColor)	
	{
	case 1:
		l_strM = g_pDoc->m_stcColorToM.m_CBM1;
		break;
	case 2:
		l_strM = g_pDoc->m_stcColorToM.m_CBM2;
		break;
	case 3:
		l_strM = g_pDoc->m_stcColorToM.m_CBM3;
		break;
	case 4:
		l_strM = g_pDoc->m_stcColorToM.m_CBM4;
		break;
	case 5:
		l_strM = g_pDoc->m_stcColorToM.m_CBM5;
		break;
	default:
		break;
	}
	return l_strM;
}

/*
 *	������ɫת���ַ��µ�M���ٵĺ���
 */
CString COutputGCode::CintToSBMstr(int RtnColor)
{
	CString l_strM;		//������ɫת���ַ�
	//�ַ����ⲿ��Ӧ�趨��
	switch(RtnColor)	
	{
	case 1:
		l_strM = g_pDoc->m_stcColorToM.m_SBM1;
		break;
	case 2:
		l_strM = g_pDoc->m_stcColorToM.m_SBM2;
		break;
	case 3:
		l_strM = g_pDoc->m_stcColorToM.m_SBM3;
		break;
	case 4:
		l_strM = g_pDoc->m_stcColorToM.m_SBM4;
		break;
	case 5:
		l_strM = g_pDoc->m_stcColorToM.m_SBM5;
		break;
	default:
		break;
	}
	return l_strM;
}

/*
 *	����ת��G����----�����
 */
BOOL COutputGCode::WritePoint(CCreatePoint* Point)
{
	double x1,y1;	//���յ������
	CString l_content,l_strSBM_What;	//д���ļ��ַ���������
	//��������
	x1 = Point->m_Begin.x;
	y1 = Point->m_Begin.y;
	//�Ѷ�Ӧ��ɫת���ַ���M
	l_strSBM_What = AnalysisAryM(Point->m_strSBM);	
	//				�����һ�����һ�㲻���							��ɫ��һ��   ��Ҫ��һ��G00
	if(m_bFirstG00 == TRUE)//����ǵ�һ��������һ�У�\r\n��
		l_content.Format("G00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����
	else
		l_content.Format("\r\nG00 X%f Y%f\r\n",x1,y1);//��Ҫ���ٶ�λ�����		
	m_bFirstG00 = FALSE;
	if(g_pDoc->m_stcWorkFor.IsGlassGrind == 1)//�����ĥ�����Ĺ��վ�Ҫ��G41��G42
		l_content.Format("\r\nG%d\r\nG00 X%f Y%f\r\n",Point->m_iKnifeStyle,x1,y1);//��Ҫ���ٶ�λ�����
	l_content = m_strCBM_What + l_content + l_strSBM_What+"\r\n";
	m_cGCode.Write(l_content,strlen(l_content));		
	//�յ������
	m_dCurX = Point->m_Begin.x;
	m_dCurY = Point->m_Begin.x;
	//�Ѷ�Ӧ��ɫת���ַ���M����������
	m_strCBM_What = AnalysisAryM(Point->m_strCBM);	
	//��������ɫ��������
	m_iColorLast = Point->m_ColorOfLine;
	
	return TRUE;
}

/*
 *	��M���뼯�������ַ���
 */
CString COutputGCode::AnalysisAryM(CArray<class CString,class CString &>& aryM)
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





