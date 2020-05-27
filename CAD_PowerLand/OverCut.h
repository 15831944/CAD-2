// OverCut.h: interface for the COverCut class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OVERCUT_H__8ECCD33A_20C9_426D_A5E2_057F05375A99__INCLUDED_)
#define AFX_OVERCUT_H__8ECCD33A_20C9_426D_A5E2_057F05375A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COverCut  //���е���
{
public:
	COverCut();
	virtual ~COverCut();
	//�ǲ���ֱ�ߣ�Ȼ��Ź���
	void NeedOverCut();
private:
	void ExecuteOverCut(CCreatePolyline* l_pIsPolyline,POSITION l_POSOverCut);//ִ�й��к���
	//���ܣ���ֱ�߼��ӳ���
	//������CMyLine lnIn:ԭֱ��
	//	int nType: 0 ��ʾ�����ӳ� 1��ʾ�����ӳ�
	//	double distance: �ӳ�����
	//	CMyLine& lnOut:�ӳ���ֱ��
	void ProcLine(Position ptStart1,Position ptEnd1, int nType, double distance,
						  Position& ptStart2,Position& ptEnd2);
	//�Ѷ���߸��ݹ��еĵ�ֽ�Ϊ���
	void PutPloyBreak(CCreatePolyline* l_pIsPolyline,CByteArray* l_carryNumPoint,POSITION l_POSOverCut);

	
	//������Ķ���
	CUndoRedo m_UndoRedo;
	//������������ӵ�һ�����	
	PUNDOLIST m_pUndoList;
	//��ͼ�ε�ָ��
	PGRAPHPOS m_pCurGraphPos,m_pheadGraphPos,m_pGraphPos;


};

#endif // !defined(AFX_OVERCUT_H__8ECCD33A_20C9_426D_A5E2_057F05375A99__INCLUDED_)
