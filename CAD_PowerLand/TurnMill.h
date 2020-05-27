// TurnMill.h: interface for the CTurnMill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TURNMILL_H__7E5C3CB6_357D_4E92_B072_65BDF0136384__INCLUDED_)
#define AFX_TURNMILL_H__7E5C3CB6_357D_4E92_B072_65BDF0136384__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define TUDU_180 tan(-MATH_PI / 4.0);


class CTurnMill  //��ϴ�����࣬��ͼ������һȦ��
{
public:
	CTurnMill();
	virtual ~CTurnMill();
	
	//��ϳ���ߵĽӿ�
	void TurnGraphMill();

private:
	//�Ѳ��պ϶���߱��ϳ����Ҫ��ͼ��
	void TurnOpenPolyline();
	//�ѱպϵĶ��߶�ת�� ��ϳ����Ҫ��ͼ��
	void TurnClosePolyline();
	//��ֱ�߱�ϳ��ͼ��
	void TurnLine();
	//��Բ����ϳ����
	void TurnArc();
	//��Բ��ϳ����
	void TurnRound();
	//����Բ���ϳ����
	void TurnEllipse();
	
	
	//�������ߵĽ���			�������ϵ��ĸ���									���صĽ���
	BOOL GetIntersectPoint(Position pos1,Position pos2,Position pos3,Position pos4,Position &pos);
	//������������Ӳ����պϵ�
	void AddUndoList(long pCreatebase1,long pCreatebase2 = NULL);

	CCreateBase* m_pMillGraph;

	//������Ķ���
	CUndoRedo m_UndoRedo;
	//������������ӵ�һ�����	
	PUNDOLIST m_pUndoList;
	//��ͼ�ε�ָ��
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
};

#endif // !defined(AFX_TURNMILL_H__7E5C3CB6_357D_4E92_B072_65BDF0136384__INCLUDED_)
