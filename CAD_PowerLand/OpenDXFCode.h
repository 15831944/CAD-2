// OpenDXFCode.h: interface for the COpenDXFCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENDXFCODE_H__61D97F31_FB82_4990_988F_9EF783039AE1__INCLUDED_)
#define AFX_OPENDXFCODE_H__61D97F31_FB82_4990_988F_9EF783039AE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COpenDXFCode  
{
public:
	BOOL ReadDXFCode(CString PathOfFile);
	COpenDXFCode();
	virtual ~COpenDXFCode();

	CStdioFile m_cReadDXFCode;	//�ļ��Ķ�����������ReadString��������һ��һ�ж��ļ�
	CArray<Position,Position&> m_point;		//����߶εĵ�
private:
	void TurnLine();	//��DXF�ļ��з���LINE��ת��ֱ��
	void TurnArc();		//��DXF�ļ��з���ARC��ת��Բ��
	void TurnCircle();	//��DXF�ļ��з���CIRCLE��ת��Բ
	void TurnPolyline();//��DXF�ļ��з���POLYLINE��ת�ɶ��߶�
	void TurnPoint();	//��DXF�ļ��з���POINT��ת�ɶ��߶�
};

#endif // !defined(AFX_OPENDXFCODE_H__61D97F31_FB82_4990_988F_9EF783039AE1__INCLUDED_)
