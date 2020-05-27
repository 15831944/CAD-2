// TurnMill.h: interface for the CTurnMill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TURNMILL_H__7E5C3CB6_357D_4E92_B072_65BDF0136384__INCLUDED_)
#define AFX_TURNMILL_H__7E5C3CB6_357D_4E92_B072_65BDF0136384__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define TUDU_180 tan(-MATH_PI / 4.0);


class CTurnMill  //变洗刀的类，让图形外括一圈。
{
public:
	CTurnMill();
	virtual ~CTurnMill();
	
	//变铣刀线的接口
	void TurnGraphMill();

private:
	//把不闭合多段线变成铣刀需要的图形
	void TurnOpenPolyline();
	//把闭合的多线段转化 成铣刀需要的图形
	void TurnClosePolyline();
	//把直线变铣刀图形
	void TurnLine();
	//把圆弧变铣刀线
	void TurnArc();
	//把圆变铣刀线
	void TurnRound();
	//把椭圆变成铣刀线
	void TurnEllipse();
	
	
	//求两条线的交点			两条线上的四个点									返回的交点
	BOOL GetIntersectPoint(Position pos1,Position pos2,Position pos3,Position pos4,Position &pos);
	//向撤消链表里添加操作闭合的
	void AddUndoList(long pCreatebase1,long pCreatebase2 = NULL);

	CCreateBase* m_pMillGraph;

	//撤消类的对象
	CUndoRedo m_UndoRedo;
	//向撤消链表里添加的一个结点	
	PUNDOLIST m_pUndoList;
	//存图形的指针
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
};

#endif // !defined(AFX_TURNMILL_H__7E5C3CB6_357D_4E92_B072_65BDF0136384__INCLUDED_)
