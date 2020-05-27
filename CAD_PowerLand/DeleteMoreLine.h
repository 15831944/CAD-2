// DeleteMoreLine.h: interface for the CDeleteMoreLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELETEMORELINE_H__ED0089F6_6A10_409D_B569_8F1F591CC6A6__INCLUDED_)
#define AFX_DELETEMORELINE_H__ED0089F6_6A10_409D_B569_8F1F591CC6A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDeleteMoreLine  //ɾ���ص�
{
public:
	CDeleteMoreLine();
	virtual ~CDeleteMoreLine();
	
	//�ӿں���
	void DeleteMoreLine();
private:
	
	
	/////////////////////////����////////////////////////////////////////////
	
	//�����е�ͼ��������һ��
	void CopyList();
	//��ջ�������
	void ClearBaseList();
	//���ֱ���Ƿ����������غ�
	BOOL CleckLine();
	//���Բ�Ƿ����������غ�
	BOOL CleckRound();
	//���Բ���Ƿ����������غ�
	BOOL CleckArc();
	//�ֽ�����
	void AnlysPolyline();
	//�ֽ����
	void AnlysRect();
	//�ֽ���Բ
	void AnlysEllipse();
	//�ı��и��
	void Changedir() ;
	//������ԭ������ĵ��Ԫ�غ�б��
	BOOL RtnPosBaseSlope();
	///////////////////����//////////////////////////////////////////////////
	
	//			��������
	CObList m_cpyBaseList;
	//�������ͼԪ
	CCreateBase* m_pCreateBase;
	//��ԭ������ĵ�	,���		�յ�
	Position m_posOriger,m_PosBegin,m_posEnd;
	//ÿ���ߵ�б��
	double l_dSlope;
};

#endif // !defined(AFX_DELETEMORELINE_H__ED0089F6_6A10_409D_B569_8F1F591CC6A6__INCLUDED_)
