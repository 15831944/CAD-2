// Draw.h: interface for the CDrawLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW_H__EFDD11CF_FCEC_4D5A_9044_F86D3DE72299__INCLUDED_)
#define AFX_DRAW_H__EFDD11CF_FCEC_4D5A_9044_F86D3DE72299__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DrawBase.h"//�����������ͼԪ��Ļ����ͷ�ļ�
#include "CreateBase.h"
#include "UndoRedo.h"
#include "spline.h"//�����


class CDrawLine  :public CDrawBase
{
public:
	CDrawLine();
	virtual ~CDrawLine();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	//�����������ӵ���������
	void AddUndoList(LONG pCreateBase);

	Position m_Begin;
	Position m_End;
	Position m_middle;
};

/*
 *	�����߶��뵥�߶ε������ǣ�
	���߶δ��˶��ָ�����������
	���߶�ֻ���� һ��
	Ȼ��OnLButtonDown��OnMouseMove������߶�һ����
	���߶�����OnRButtonDown�ﶯ̬�������л��ġ�
 */
class CDrawPolyline  :public CDrawBase
{
public:
	CDrawPolyline();
	virtual ~CDrawPolyline();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
	CArray<Position,Position&> m_point;

private:
	//�����������ӵ���������
	void AddUndoList(LONG pCreateBase);

	Position m_Begin;
	Position m_End;
	Position m_middle;
	CCreatePolyline* m_PolylineLast;
	
};


class CDrawRectangle  :public CDrawBase
{
public:
	CDrawRectangle();
	virtual ~CDrawRectangle();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:

	//�����������ӵ���������
	void AddUndoList(LONG pCreateBase);

	Position m_Begin;
	Position m_End;
	Position m_BeginDown;
	Position m_BeginRight;
	//�Ƿ���Բ�Ǿ���	�Ƿ������볤��ľ���
	BOOL m_bFlagRoundRect,m_bSizeRect;
	double m_dRadius;
	//���εĳ�			��
	double m_dRectLong,m_dRectWide;
};


class CDrawRoundArc  :public CDrawBase
{
public:
	CDrawRoundArc();
	virtual ~CDrawRoundArc();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	//�����������ӵ���������
	void AddUndoList(LONG pCreateBase);

	Position m_Begin;
	Position m_End;
	Position m_Center;
	int m_iDirect;	//��Բ�򻡵ķ���

};


class CDrawRound  :public CDrawBase
{
public:
	CDrawRound();
	virtual ~CDrawRound();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	//�����������ӵ���������
	void AddUndoList(LONG pCreateBase);

	Position m_Center;
	Position m_EndPos;
	int m_iDirect;	//��Բ�򻡵ķ���
};


class CDrawSpline  :public CDrawBase
{
public:
	CDrawSpline();
	virtual ~CDrawSpline();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);


private:	
	double Distance(CPoint p1, CPoint p2);
	int IsInsideControlPoint(Position point);
	//�����������ӵ���������
	void AddUndoList(LONG pCreateBase);
	
	
	//���Ƶ㼯
	CArray<Position,Position&> m_ControlPoints;
	CArray<Position,Position&> m_CurvePoints;
	//���Ƶ������ߵ㼯�е�λ��
	CArray<int,int&> m_NumControl;	
	int m_MoveIndex,m_refresh;
	Position m_prePos,m_posLast;
	
};


class CDrawEllipse  :public CDrawBase
{
public:
	CDrawEllipse();
	virtual ~CDrawEllipse();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
private:

	//�����������ӵ���������
	void AddUndoList(LONG pCreateBase);
	
	Position m_Begin;
	Position m_End;
	Position m_Second;


};


class CDrawSelect  :public CDrawBase//��ѡ����
{
public:
	CDrawSelect();
	virtual ~CDrawSelect();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

	Position m_Begin;
	Position m_End;	
	int m_draw1,m_draw2;
	
};



class CDrawBreakTwo  :public CDrawBase//һ��ͼ�δ�ϳ�����ͼ�ε���
{
public:
	CDrawBreakTwo();
	virtual ~CDrawBreakTwo();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	Position m_Begin;
	Position m_End;	
	
	//����Ҽ��ĵ�
	Position m_posRight;
	//��ѡ�е�ͼ��
	CCreateBase *m_pCreateBase,*m_pCreateBase1,*m_pCreateBase2;
	

	BOOL BreakLine(Position l_cPos);
	BOOL BreakPolyline(Position l_cPos,BOOL FlagMove = FALSE);
	BOOL BreakPolyline(Position l_cPos1,Position l_cPos2,BOOL FlagMove = FALSE);
	BOOL BreakRect(Position l_cPos1,Position l_cPos2,BOOL FlagMove = FALSE);
	BOOL BreakRound(Position l_cPos1,Position l_cPos2);
	BOOL BreakArc(Position l_cPos);
	BOOL BreakEllipse(Position l_cPos1,Position l_cPos2,BOOL FlagMove = FALSE);
	BOOL BreakClose(CArray<Position,Position&> &l_point,CArray<double,double&> &tudu,CArray<Position,Position&> &l_point1,
		CArray<Position,Position&> &l_point2,CArray<double,double&> &tudu1,CArray<double,double&> &tudu2,
		Position l_cPos1,Position l_cPos2 );
	void SuccessBreak();//��ϳɹ�
	//��ֱ��ͨ������ֳ�������														��־�Ƿ��Ǳպ�ͼ��
	BOOL PutLineThree(Position begin,Position end,Position &breakpos,Position clilkpos,BOOL FlagClose);
	//���ݶϵ�ѺŰѵ㼯�ֳ�����
	BOOL PutArrayTwo(int num,CArray<Position,Position&> &l_point,CArray<double,double&> &tudu,
		CArray<Position,Position&> &l_point1,CArray<Position,Position&> &l_point2,CArray<double,double&> &tudu1,
		CArray<double,double&> &tudu2,Position breakpos);
	BOOL IsClickArc(Position begin,Position end,Position clickpos,Position &breakpos,double tudu);
	//�����ĸ�Ԫ��
	BOOL RtnPickLine( CPoint point,int NumClick);
	//������������Ӳ���
	void AddUndoList(LONG CreateBase1,LONG CreateBase2,LONG CreateBase3,POSITION pos);
//	BOOL TurnPoly(CArray<Position,Position&> &point,CArray<Position,Position&> &l_point1 );
	//����������µ��ߣ����ұ�ע�ϵ�һ��
	void InvalLastLine();
};




class CDrawArrange  :public CDrawBase//�����������
{
public:
	CDrawArrange();
	virtual ~CDrawArrange();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	POSITION m_PosOfArrange;

	//�����������ӵ���������
	void AddUndoList(POSITION pos,LONG pCreateBase);

};


class CDrawSetBeginPos  :public CDrawBase//ѡ��ͼ��������
{
public:
	CDrawSetBeginPos();
	virtual ~CDrawSetBeginPos();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	void SuccessBreak();//��ϳɹ�
	//�պϵĶ����ѡ�����
	BOOL PolylineChoiceBegin(Position ClickPos,int iNumPick);
	//����ѡ�����
	BOOL RectChoiceBegin(Position ClickPos,int iNumPick);
	//Բѡ�����
	BOOL RoundChoiceBegin(Position ClickPos);
	//��Բѡ�����
	BOOL EllipseChoiceBegin(Position ClickPos,int iNumPick);
	//������������Ӳ���
	void AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);
	
	//������Ķ���
	CUndoRedo m_UndoRedo;
	//������������ӵ�һ�����	
	PUNDOLIST m_pUndoList;
	//��ͼ�ε�ָ��
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
	//��ѡ�е�ͼ��
	CCreateBase *m_pCreateBase;	
};


class CDrawExtendLine   :public CDrawBase//�ӳ��ߵ���
{
public:
	CDrawExtendLine();
	virtual ~CDrawExtendLine();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

	//��ֱ���ӳ�
	void PutLineEx(CCreateBase* pCreateBase,double LongExtend,int IsBegin);
	//�Ѷ�����ӳ�
	void PutPolylineEx(CCreateBase* pCreateBase,double LongExtend,int IsBegin);
	
private:
	//ѡ������Ķ˵��ӳ�,1��㣬2ĩ�㣬3��ͷ���ӳ���0û���ӳ�;
	int m_iExtendWhichTop;
	//��ʱ��
	Position m_posBegin,m_posEnd;
	//�ӳ�����
	double m_dLongExtend,m_dAgl;
	//������Ķ���
	CUndoRedo m_UndoRedo;
	//������������ӵ�һ�����	
	PUNDOLIST m_pUndoList;
	//��ͼ�ε�ָ��
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
	//��ѡ�е�ͼ��
	CCreateBase *m_pCreateBase;	
	
	
	//�����ӳ�
	void ExtendLine(CCreateBase* pCreateBase,double LongExtend,int IsBegin);
	//��ϳɹ�
	void SuccessExtend();
	//��ǰ��������ʱ����1����ǰ����ĩ���ʱ����2��������һ����ʱ����0
	int ClosePos(CCreateBase* pCreateBase,Position pos);
	//������������Ӳ���
	void AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);

};


class CDrawChamfer  :public CDrawBase//����
{
public:
	CDrawChamfer();
	virtual ~CDrawChamfer();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);



private:
	CCreateBase* m_createbaseFirst,*m_createbaseSecond;//����ѡ�е�ͼ��
	//ѡ�е���ͼ�εĵڼ����ߡ�����
	int m_iPick1,m_iPick2;
	//���ǵİ뾶
	double m_dRadius;
	//�ǲ��ǵ�͹��
	BOOL m_bConvex;
	//�ǲ�������İ뾶
	UINT m_inFlags;
	//������ĵ�����
	Position m_posClickThree;
	CArray<Position,Position&> m_AryPoint;
	CArray<double,double&> m_aryTudu;
	
	//�õ�Բ�ģ�����յ�	���������ϵĵ�									���ķ���	��㣬			�յ㣬			Բ��
	void GetArcPoint(Position pos1,Position pos2,Position pos3,int clock,Position& begin,Position& end,Position& center);
	//ϳ���߶�
	BOOL ChamferPolyline();
	//�����������һ���������
	
};


class CDrawMeasure :public CDrawBase  //����
{
public:
	CDrawMeasure();
	virtual ~CDrawMeasure();
	
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);	


private:

	Position m_Begin;
	Position m_middle;
	Position m_End;
};


class CDrawPoint  :public CDrawBase//����
{
public:
	CDrawPoint();
	virtual ~CDrawPoint();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	//�����������ӵ���������
	void AddUndoList(LONG pCreateBase);

	
	//����Ҽ��ĵ�
	Position m_posRight;
	//��ѡ�е�ͼ��
	CCreateBase *m_pCreateBase;
	

};


class CDrawAddPoint : public CDrawBase //���ӵ� 
{
public:
	CDrawAddPoint();
	virtual ~CDrawAddPoint();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
private:
	//�����������ӵ���������
//	void AddUndoList(LONG pCreateBase);
	
	Position m_Begin;
};


class CDrawMoveView  : public CDrawBase //�ƶ�ͼ��
{
public:
	CDrawMoveView();
	virtual ~CDrawMoveView();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
	int OnLButtonUp(UINT nFlags,const Position &pos);

private:
	Position m_posBegin;
	
};


class CDrawZoomScale  : public CDrawBase //ͼ�ΰ���������
{
public:
	CDrawZoomScale();
	virtual ~CDrawZoomScale();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
	
private:
	//����
	Position m_posBase;
	//�Ƿ��������
	BOOL m_bBase;

	int OnZoomScale(double dScale);
};


class CDrawVerticalLine  :public CDrawBase//����ֱ��
{
public:
	CDrawVerticalLine();
	virtual ~CDrawVerticalLine();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
	
private:
	//�����������ӵ���������
	void AddUndoList(LONG pCreateBase);
	//���������ߵĴ��ߵĽǶ�
	BOOL GetAngleVert(Position pos);
	//��Ƕȼ�����ӽǶ�
	void AddAngleToAry(double dAngle);
	//���㵽������ϻ���λ���Բ��ʱ��ֱ�ĽǶ�
							//����������ߣ�����ĵ㣬	����ߵĺ�����
	void GetPolylineAngleVert(int iPick,Position posClick,CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);


	Position m_Begin;
	Position m_End;
	double m_dLongLine;//�߳�
	CArray<double,double&>	m_aryAngleVertline;  //ѡ�е��ߵĽǶȼ���
	
};


class CDrawParallel : public CDrawBase  //ƽ����
{
public:
	CDrawParallel();
	virtual ~CDrawParallel();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:

	//��ϳ���ߵĽӿ�
	void AddParallel();
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
	//ȡ�õ���������㣬�������������ж������ͼ�ε�����
	void GetVecter(Position posClick);

	Position m_Begin,m_End;
	double m_dLongLine;//�߳�
	BOOL m_bIsLeft;// �ǲ������ƽ��
	long m_lCreateBasePick;//ѡ�е�ͼ�ε�ָ��
	//���ж���ߵ�������
	int m_iPick;
	//����ƶ�ʱͼ��ָ��
	CCreateBase* m_pDragCreatebase;
	
};


class CDrawTrim  : public CDrawBase  //�޼�
{
public:
	CDrawTrim();
	virtual ~CDrawTrim();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
private:
	//������������Ӳ���
	void AddUndoList();
	CCreateBase* m_createbaseFirst,*m_createbaseSecond;//����ѡ�е�ͼ��
	//ѡ�е���ͼ�εĵڼ����ߡ�����
	int m_iPick1,m_iPick2;
	//��һ�ε���ĵ�͵ڶ��ε���ĵ�
	Position m_posFirst,m_posSecond;
	//������Ķ���
	CUndoRedo m_UndoRedo;
	//������������ӵ�һ�����	
	PUNDOLIST m_pUndoList;
	//��ͼ�ε�ָ��
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;

	
	//��ͼ���޼�
	void DoTrimGraph();
	//�޼�ֱ��
	void TrimLine();
	//�޼�Բ��
	void TrimAarc();
	//�޼�Բ
	void TrimRound();	
	//�޼������
	void TrimPolyline(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);

	//����ֱ�������ߵĽ����޼�ֱ��    �Ķ���					���صĽ���			
	BOOL GetInterPosLineAndPolyline(int& iNumPoint,Position& posInterClose,Position posBegin,Position posEnd,
		CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);
	
	//����Բ�������ߵĽ����޼�ֱ��    �Ķ���					���صĽ���			
	BOOL GetInterPosArcAndPolyline(int& iNumPoint,Position& posInterClose,Position posBegin,
		Position posEnd,Position posCenter,double dRadius,int iDir,
		CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);
	
	//����Բ�����ߵĽ����޼�ֱ��    �Ķ���					���صĽ���			
	BOOL GetInterPosRoundAndPolyline(int& iNumPoint,Position& posInterClose,Position posCenter,
		double dRadius,CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);
	
	
	//���ض���������ߵĽ����޼������   �Ķ���					���صĽ���			
	BOOL GetInterPosPolylineAndPolyline(int& iNumPoint,Position& posInterClose,CArray<Position,Position&> &aryPoint1,
		CArray<double,double&> &aryTudu1,CArray<Position,Position&> &aryPoint2,CArray<double,double&> &aryTudu2);
	
	//�����µĶ����
	BOOL GeneratePolyline(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,
		Position posInter,int l_iNumPoint);
	//		       ����	      �����ڶ���ߵ��Ķ�����

	//������Բ��
	BOOL GenerateArc(LONG pArc,Position posInter);
	
	//ͨ��Բ������Բ��
	BOOL GenerateRound(LONG pRound,Position posInter);
	
	//��ʾû�н���
	void RemindNoInter();
};


class CDrawMovePoint  : public CDrawBase  //�ƶ���
{
public:
	CDrawMovePoint();
	virtual ~CDrawMovePoint();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);	
private:
	//������������Ӳ���
	void AddUndoList(CCreateBase* pCreateBase);
	//���Ҷ˵�
	BOOL FindPointEnd(Position pos);
	//����һ������ƶ�ʱҪ��������
	void CopyList();
	//�ͷ�m_aryDrawMouseMove
	void ClearList();

	struct stcMovePoint
	{	
		//�����е�ͼ��
		CCreateBase* pCreateBase;
		//����ͼ��
		int iStyleGraph;
		//�Ƿ���ֱ�ߵ����
		BOOL bIsBeginPos;
		//������ϵ��ĸ���
		int iNumPolyMovePoint;
	};
	//ִ�е��ƶ�
	void DoMovePoint(CArray<stcMovePoint,stcMovePoint&>& aryMovePoint,Position pos,BOOL bAddUndo = TRUE);
	
	//ѡ���ĵ�
	Position m_posFirst,m_posPre;
	//Ҫ�ƶ�ͼ�ε�����	
	CArray<stcMovePoint,stcMovePoint&> m_aryMovePoint;
	//����ƶ�ʱҪ���ĸ��Ƶ�����	
	CArray<stcMovePoint,stcMovePoint&> m_aryDrawMouseMove;
	//������Ķ���
	CUndoRedo m_UndoRedo;
	//������������ӵ�һ�����	
	PUNDOLIST m_pUndoList;
	//��ͼ�ε�ָ��
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
	
};

#endif // !defined(AFX_DRAW_H__EFDD11CF_FCEC_4D5A_9044_F86D3DE72299__INCLUDED_)
