// OutputGCode.h: interface for the COutputGCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTGCODE_H__12BA7076_6221_4B62_85C3_289D8B04C37D__INCLUDED_)
#define AFX_OUTPUTGCODE_H__12BA7076_6221_4B62_85C3_289D8B04C37D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputGCode  
{
public:
	COutputGCode();
	virtual ~COutputGCode();


public:
	// ���g�����ļ�
	BOOL	OutputGCode(CString filename);

private:
	//�µ�̧����M����
	CString CintToCBMstr(int RtnColor);
	CString CintToSBMstr(int RtnColor);
	// д�ļ�ͷ
	BOOL	WriteHeader();
	// д�ļ�β
	BOOL	WriteTail();
	// дʵ����Ϣ���ú����������͵���дͼԪ��Ϣ�ĺ���
	BOOL	WriteContent();
	// дͼԪ��Ϣ
	BOOL	WriteLine(CCreateLine*	pLine);
	BOOL	WriteRectangle(CCreateRectangle*	pRectangle);
	BOOL	WriteRound(CCreateRound*	pRound);
	BOOL	WriteArc(CCreateRoundArc*	pArc);
	BOOL	WritePolyline(CCreatePolyline* Polyline);
	BOOL	WriteEllipse(CCreateEllipse* Ellipse);
	BOOL	WritePoint(CCreatePoint* Point);
	//��M���뼯�������ַ���
	CString AnalysisAryM(CArray<class CString,class CString &>& aryM);
	//�Ƿ��ǵ�һ��GOO
	BOOL m_bFirstG00;
	CFile m_cGCode;
	double m_dCurX;	//��ǰ��X
	double m_dCurY;	//��ǰ��Y
	int m_iColorLast;//������ɫ
	CString m_strCBM_What;	//����ߵ���ɫ
};

#endif // !defined(AFX_OUTPUTGCODE_H__12BA7076_6221_4B62_85C3_289D8B04C37D__INCLUDED_)
