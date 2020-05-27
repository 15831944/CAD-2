// ToLessPoint.h: interface for the CToLessPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOLESSPOINT_H__17A20833_E51C_40C5_807F_B277781222ED__INCLUDED_)
#define AFX_TOLESSPOINT_H__17A20833_E51C_40C5_807F_B277781222ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CToLessPoint  
{
public:
	CToLessPoint();
	virtual ~CToLessPoint();
	
	void ToLessPoint();

private:

	//ȥ����һ��ֱ���ϵĶ���ĵ�
	void DelEqualSlope();
	//��С��ֱ����ϳ�Բ��
	void LineToArc();
	//�ѵ㼯����
	void UpdataPolyline();
	//�鿴Բ����ԭͼ�ľ���
	BOOL ClickPrecision(int& i,int& j);
	//�ı�Բ�����ٴβ鿴����
	BOOL TurnArcClickPrecs(int& i,int& j,int iMaxPos,int iMinPos,BOOL bLineFarest);
	//��������ӵ�
	BOOL AddPoint(int& i,int& j);
	//����Բ�������Բ��һ�㵽Բ�ϵľ�����Բ
	BOOL Pos2OnRdPosInRdOutRd(Position posRound1,Position posRound2,Position posInRound,double dDisToRound,Position& posCenter1,double& Radius1,Position& posCenter2,double& Radius2);
	//������ĵ��Ƿ���Բ����
	BOOL PosOnRound(int i,int j,Position posCenter,double dRadius);
	//������������Ӳ���
	void AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,LONG CreateBase);
	
	CCreatePolyline* m_pPolyline;
	CArray<Position,Position&> m_aryPoint;//�㼯
	CArray<double,double&> m_aryTuDu;//���͹�ȵ�
	//ǰһ��Բ������㣬�յ㣬��Բ��һ�㣬Բ��
	Position m_posBegin,m_posEnd,m_posMid,m_posCenter,m_posPasCT,m_posPasEN,m_posPasMD;
	//ÿ�λ��İ뾶	������  ��С����
	double m_dRadius,m_dMaxDis,m_dMinDis,m_dPrecision,m_dMaxRadius;

	//������Ķ���
	CUndoRedo m_UndoRedo;
	//������������ӵ�һ�����	
	PUNDOLIST m_pUndoList;
	//��ͼ�ε�ָ��
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
	
};

#endif // !defined(AFX_TOLESSPOINT_H__17A20833_E51C_40C5_807F_B277781222ED__INCLUDED_)
