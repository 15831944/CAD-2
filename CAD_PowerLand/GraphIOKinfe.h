// GraphIOKinfe.h: interface for the CGraphIOKinfe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHIOKINFE_H__9099639C_76E8_44CD_B23A_62AA08D33223__INCLUDED_)
#define AFX_GRAPHIOKINFE_H__9099639C_76E8_44CD_B23A_62AA08D33223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGraphIOKinfe  //���˵�
{
public:
	CGraphIOKinfe();
	virtual ~CGraphIOKinfe();

	//��ͼ�μӽ��˵�
	void PutGraphIOKife();
private:
	//���Ӷ���������յ�,���κͶ���ߵ���һ�����˵�F�Ǿ��Σ�T�Ƕ����
	void ChangePolyBE(CCreateBase *Polyline,POSITION POSCur,BOOL IsPolyline);
	//����Բ�����յ�
	void ChangeRoundBE(CCreateRound *Round,POSITION POSCur);
	//������Բ�����յ�
	void ChangeEllipseBE(CCreateEllipse *Ellipse,POSITION POSCur);
	//�����Բ��������͹��
	void CalcArcBgPos(Position ptStart,Position ptEnd,Position& posArc,Position& posLine,double& tudu);
	//���˵�Բ����ĩ���͹��
	void CalcArcEnPos(Position ptStart,Position ptEnd,Position& posArc,Position& posLine,double& tudu);
	//�������Բ��������
	void CalcCenterInput(Position ptStart, Position ptEnd, Position& ptCenter, double radius);
	//�������Բ��������
	void CalcCenterOutput(Position ptStart, Position ptEnd, Position& ptCenter, double radius);
	//����ֱ���е�
	void CalcMidPoint(Position ptStart, Position ptEnd, Position& ptMid);
	//��Բ�����
	void CalcBeginPoint(Position ptCenter, double radius, double angle, Position& ptBeg);
	//��Բ�Ľ�����Բ��
	void CalcArcCenterForRound(int l_iClock,Position Center,Position End,Position& posArc1,Position& posLine1,
		double& tudu1,Position& posArc2,Position& posLine2,double& tudu2);
	//������������Ӳ���
	void AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,LONG CreateBase);


	//������Ķ���
	CUndoRedo m_UndoRedo;
	//������������ӵ�һ�����	
	PUNDOLIST m_pUndoList;
	//��ͼ�ε�ָ��
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
};

#endif // !defined(AFX_GRAPHIOKINFE_H__9099639C_76E8_44CD_B23A_62AA08D33223__INCLUDED_)
