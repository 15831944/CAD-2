// DrawBase.h: interface for the CDrawBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWBASE_H__1719DA0B_4A01_4AE5_9689_5756BC40833E__INCLUDED_)
#define AFX_DRAWBASE_H__1719DA0B_4A01_4AE5_9689_5756BC40833E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Position.h"

#define END_POINT			1
#define NEAR_POINT			2
#define FOOT_POINT			4
#define ORTHOGONAL			8
#define MID_POINT			16

class CDrawBase  
{
public:
	CDrawBase();
	virtual ~CDrawBase();
	//                     ����nFlagsΪ1�������Ϊ2���̵��
	virtual int OnLButtonDown(UINT nFlags, const Position& pos,CString cmd) = 0;   //���麯�� 
	virtual int OnMouseMove(UINT nFlags, const Position& pos) = 0;	
	virtual int OnRButtonDown(UINT nFlags, const Position& pos)  = 0;	
	virtual int OnLButtonUp(UINT nFlags, const Position& pos) {return 0;};	
	
protected:
	
	//����׽
	BOOL FindPointAll(CDC* pDC,Position posBase,Position posMove,int iCatch = 3);
	//�趨�����µĵ�
	void SetLButtonDownPot(Position& pos,int NumOfClick);
	//������Ч������
	int InvalidContent();
	//ִ�гɹ����ƺ���
	void SuccessWord(CString str);


	//���������
	int m_iNumOfClick;	
	Position m_Begin;
	Position m_middle;
	Position m_End;
	//����еĵ�
	Position m_posWillEnd,m_posPassEnd,m_posWillNear,m_posPassNear,m_posWillFoot,m_posPassFoot,m_posWillMid,m_posPassMid,m_posOrthogonal;
	//�Ƿ�㵽�˵���
	BOOL m_bIsClickEnd,m_bIsClickNear,m_bIsClickMid,m_bIsClickFoot,m_bisClickOrthogonal;

private:
	//���Ҷ˵�
	BOOL FindPointEnd(Position pos);
	//���������
	BOOL FindPointNear(Position pos);
	//���Ҵ����
	BOOL FindPointFoot(Position posBase,Position posMove);
	//�����е�
	BOOL FindPointMid(Position pos);
	//����������
	Position RtnOrthogonalPos(Position posBase,Position posMove);
	


	//�������
	void DrawNearPoint(CDC* pDC,CPoint pot);
	//�������
	void DrawFootPoint(CDC* pDC,CPoint pot);
	//���е�
	void DrawMidPoint(CDC* pDC,CPoint pot);

};




#endif // !defined(AFX_DRAWBASE_H__1719DA0B_4A01_4AE5_9689_5756BC40833E__INCLUDED_)
