// OpenGCode.h: interface for the COpenGCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGCODE_H__DACAF320_703B_4CE2_B75E_5886F3F3B298__INCLUDED_)
#define AFX_OPENGCODE_H__DACAF320_703B_4CE2_B75E_5886F3F3B298__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COpenGCode  
{
public:
	BOOL ReadGCode(CString PathName);	//��G����
	COpenGCode();
	virtual ~COpenGCode();
public:
	CStdioFile m_cReadGCode;	//�ļ��Ķ�����������ReadString��������һ��һ�ж��ļ�

private:
	//��M���뼯�������ַ���
	CString AnalysisAryM(CArray<class CString,class CString &>& aryM);
	void TurnG00(CString LineG00);	//����G00
	void TurnG01(CString LineG01);	//����G01
	void TurnG02(CString LineG02);	//����G02
	void TurnG03(CString LineG03);	//����G02
	void MWhat(CString lineM);//����M����
	void Draw(COLORREF color,Position posPer);	//���ߺ��� 
	COLORREF RtnColor();//����M�ַ���֮����������ɫ
	COLORREF DistriPointMCode();//�������M����		



private:
	Position m_Cur;	//��ǰ������
	//����G00�����M�����õ� 
	CString m_strOnceM;	
	//�ߵ�������ʲô ,����G41����G42
	int m_iStyle,     m_iG41orG42;	
	double m_dCurTudu;
	CObArray    m_CollectOfG00;	// ������G00֮����ͼԪ
	CArray<double,double&> m_aryTudu;//͹��m_aryTudu;
	CArray<Position,Position&> m_point;		//����߶εĵ�
	CArray<class CString,class CString &> m_aryOnceM;//g00�����M����
	CArray<class CString,class CString &> m_aryTwicM;//g00�����M����
	
};

#endif // !defined(AFX_OPENGCODE_H__DACAF320_703B_4CE2_B75E_5886F3F3B298__INCLUDED_)
