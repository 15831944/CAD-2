// OutputDXFCode.h: interface for the COutputDXFCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTDXFCODE_H__8B6DDDAB_FB1E_47F7_B3E0_BE2DE7EC1F88__INCLUDED_)
#define AFX_OUTPUTDXFCODE_H__8B6DDDAB_FB1E_47F7_B3E0_BE2DE7EC1F88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////////
/*						����DXF��ͼԪ��
0	//ͼԪ���Ϳ鿪ʼ 
LINE	
5	//������ɲ�Ҫ��
140
8	//ͼ��
0
10		//10,20,30����Ҫ���XYZ����
158.3427961847824
20
79.877121304845559
30
0.0
11		//11��21��31���������XYZ����
212.7003931202612
21
117.0009327169245
31
0.0
*/
//////////////////////////////////////////////////////////////////////////

class COutputDXFCode  
{
public://���к���

	COutputDXFCode();
	virtual ~COutputDXFCode();
	
	// ���dxf�����ļ�
	BOOL	OutputDXFCode(CString filename);
		

public://���б���

	CFile m_cDXFCode;
	double m_dCurX;	//��ǰ��X
	double m_dCurY;	//��ǰ��Y

private:
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
	//////////////////////////////////////////////////////////////////////////
	

};

#endif // !defined(AFX_OUTPUTDXFCODE_H__8B6DDDAB_FB1E_47F7_B3E0_BE2DE7EC1F88__INCLUDED_)
