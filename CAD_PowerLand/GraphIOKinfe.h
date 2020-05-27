// GraphIOKinfe.h: interface for the CGraphIOKinfe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHIOKINFE_H__9099639C_76E8_44CD_B23A_62AA08D33223__INCLUDED_)
#define AFX_GRAPHIOKINFE_H__9099639C_76E8_44CD_B23A_62AA08D33223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGraphIOKinfe  //进退刀
{
public:
	CGraphIOKinfe();
	virtual ~CGraphIOKinfe();

	//把图形加进退刀
	void PutGraphIOKife();
private:
	//增加多段线起点和终点,矩形和多段线的用一个进退刀F是矩形，T是多段线
	void ChangePolyBE(CCreateBase *Polyline,POSITION POSCur,BOOL IsPolyline);
	//增加圆起点和终点
	void ChangeRoundBE(CCreateRound *Round,POSITION POSCur);
	//增加椭圆起点和终点
	void ChangeEllipseBE(CCreateEllipse *Ellipse,POSITION POSCur);
	//求进刀圆弧的起点和凸度
	void CalcArcBgPos(Position ptStart,Position ptEnd,Position& posArc,Position& posLine,double& tudu);
	//求退刀圆弧的末点和凸度
	void CalcArcEnPos(Position ptStart,Position ptEnd,Position& posArc,Position& posLine,double& tudu);
	//计算进刀圆弧的中心
	void CalcCenterInput(Position ptStart, Position ptEnd, Position& ptCenter, double radius);
	//计算出刀圆弧的中心
	void CalcCenterOutput(Position ptStart, Position ptEnd, Position& ptCenter, double radius);
	//计算直线中点
	void CalcMidPoint(Position ptStart, Position ptEnd, Position& ptMid);
	//算圆弧起点
	void CalcBeginPoint(Position ptCenter, double radius, double angle, Position& ptBeg);
	//求圆的进出刀圆心
	void CalcArcCenterForRound(int l_iClock,Position Center,Position End,Position& posArc1,Position& posLine1,
		double& tudu1,Position& posArc2,Position& posLine2,double& tudu2);
	//向撤消链表里添加操作
	void AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,LONG CreateBase);


	//撤消类的对象
	CUndoRedo m_UndoRedo;
	//向撤消链表里添加的一个结点	
	PUNDOLIST m_pUndoList;
	//存图形的指针
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
};

#endif // !defined(AFX_GRAPHIOKINFE_H__9099639C_76E8_44CD_B23A_62AA08D33223__INCLUDED_)
