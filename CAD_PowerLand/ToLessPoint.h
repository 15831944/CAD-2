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

	//去掉在一条直线上的多余的点
	void DelEqualSlope();
	//把小碎直线拟合成圆弧
	void LineToArc();
	//把点集更新
	void UpdataPolyline();
	//查看圆弧与原图的精度
	BOOL ClickPrecision(int& i,int& j);
	//改变圆弧后再次查看精度
	BOOL TurnArcClickPrecs(int& i,int& j,int iMaxPos,int iMinPos,BOOL bLineFarest);
	//向多段线里加点
	BOOL AddPoint(int& i,int& j);
	//根据圆上两点和圆内一点到圆上的距离求圆
	BOOL Pos2OnRdPosInRdOutRd(Position posRound1,Position posRound2,Position posInRound,double dDisToRound,Position& posCenter1,double& Radius1,Position& posCenter2,double& Radius2);
	//查给定的点是否在圆弧上
	BOOL PosOnRound(int i,int j,Position posCenter,double dRadius);
	//向撤消链表里添加操作
	void AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,LONG CreateBase);
	
	CCreatePolyline* m_pPolyline;
	CArray<Position,Position&> m_aryPoint;//点集
	CArray<double,double&> m_aryTuDu;//存放凸度的
	//前一段圆弧的起点，终点，过圆弧一点，圆心
	Position m_posBegin,m_posEnd,m_posMid,m_posCenter,m_posPasCT,m_posPasEN,m_posPasMD;
	//每段弧的半径	最大距离  最小距离
	double m_dRadius,m_dMaxDis,m_dMinDis,m_dPrecision,m_dMaxRadius;

	//撤消类的对象
	CUndoRedo m_UndoRedo;
	//向撤消链表里添加的一个结点	
	PUNDOLIST m_pUndoList;
	//存图形的指针
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
	
};

#endif // !defined(AFX_TOLESSPOINT_H__17A20833_E51C_40C5_807F_B277781222ED__INCLUDED_)
