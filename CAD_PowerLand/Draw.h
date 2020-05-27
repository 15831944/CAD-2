// Draw.h: interface for the CDrawLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW_H__EFDD11CF_FCEC_4D5A_9044_F86D3DE72299__INCLUDED_)
#define AFX_DRAW_H__EFDD11CF_FCEC_4D5A_9044_F86D3DE72299__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DrawBase.h"//这是以下向个图元类的基类的头文件
#include "CreateBase.h"
#include "UndoRedo.h"
#include "spline.h"//多段线


class CDrawLine  :public CDrawBase
{
public:
	CDrawLine();
	virtual ~CDrawLine();
	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	//把这个操作添加到撤消链表
	void AddUndoList(LONG pCreateBase);

	Position m_Begin;
	Position m_End;
	Position m_middle;
};

/*
 *	画多线段与单线段的区别是：
	单线段存了多个指针进入总链表
	多线段只存了 一个
	然后OnLButtonDown和OnMouseMove里跟单线段一样，
	多线段是在OnRButtonDown里动态创建还有画的。
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
	//把这个操作添加到撤消链表
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

	//把这个操作添加到撤消链表
	void AddUndoList(LONG pCreateBase);

	Position m_Begin;
	Position m_End;
	Position m_BeginDown;
	Position m_BeginRight;
	//是否是圆角矩形	是否是输入长宽的矩形
	BOOL m_bFlagRoundRect,m_bSizeRect;
	double m_dRadius;
	//矩形的长			宽
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
	//把这个操作添加到撤消链表
	void AddUndoList(LONG pCreateBase);

	Position m_Begin;
	Position m_End;
	Position m_Center;
	int m_iDirect;	//画圆或弧的方向

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
	//把这个操作添加到撤消链表
	void AddUndoList(LONG pCreateBase);

	Position m_Center;
	Position m_EndPos;
	int m_iDirect;	//画圆或弧的方向
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
	//把这个操作添加到撤消链表
	void AddUndoList(LONG pCreateBase);
	
	
	//控制点集
	CArray<Position,Position&> m_ControlPoints;
	CArray<Position,Position&> m_CurvePoints;
	//控制点在曲线点集中的位置
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

	//把这个操作添加到撤消链表
	void AddUndoList(LONG pCreateBase);
	
	Position m_Begin;
	Position m_End;
	Position m_Second;


};


class CDrawSelect  :public CDrawBase//框选的类
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



class CDrawBreakTwo  :public CDrawBase//一个图形打断成两个图形的类
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
	
	//点击右键的点
	Position m_posRight;
	//被选中的图形
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
	void SuccessBreak();//打断成功
	//把直线通过点击分成三个点														标志是否是闭合图形
	BOOL PutLineThree(Position begin,Position end,Position &breakpos,Position clilkpos,BOOL FlagClose);
	//根据断点把号把点集分成两个
	BOOL PutArrayTwo(int num,CArray<Position,Position&> &l_point,CArray<double,double&> &tudu,
		CArray<Position,Position&> &l_point1,CArray<Position,Position&> &l_point2,CArray<double,double&> &tudu1,
		CArray<double,double&> &tudu2,Position breakpos);
	BOOL IsClickArc(Position begin,Position end,Position clickpos,Position &breakpos,double tudu);
	//返回哪个元素
	BOOL RtnPickLine( CPoint point,int NumClick);
	//向撤消链表里添加操作
	void AddUndoList(LONG CreateBase1,LONG CreateBase2,LONG CreateBase3,POSITION pos);
//	BOOL TurnPoly(CArray<Position,Position&> &point,CArray<Position,Position&> &l_point1 );
	//擦除最后留下的线，并且标注上第一点
	void InvalLastLine();
};




class CDrawArrange  :public CDrawBase//重新排序的类
{
public:
	CDrawArrange();
	virtual ~CDrawArrange();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	POSITION m_PosOfArrange;

	//把这个操作添加到撤消链表
	void AddUndoList(POSITION pos,LONG pCreateBase);

};


class CDrawSetBeginPos  :public CDrawBase//选择图形起点的类
{
public:
	CDrawSetBeginPos();
	virtual ~CDrawSetBeginPos();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	void SuccessBreak();//打断成功
	//闭合的多段线选择起点
	BOOL PolylineChoiceBegin(Position ClickPos,int iNumPick);
	//矩形选择起点
	BOOL RectChoiceBegin(Position ClickPos,int iNumPick);
	//圆选择起点
	BOOL RoundChoiceBegin(Position ClickPos);
	//椭圆选择起点
	BOOL EllipseChoiceBegin(Position ClickPos,int iNumPick);
	//向撤消链表里添加操作
	void AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);
	
	//撤消类的对象
	CUndoRedo m_UndoRedo;
	//向撤消链表里添加的一个结点	
	PUNDOLIST m_pUndoList;
	//存图形的指针
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
	//被选中的图形
	CCreateBase *m_pCreateBase;	
};


class CDrawExtendLine   :public CDrawBase//延长线的类
{
public:
	CDrawExtendLine();
	virtual ~CDrawExtendLine();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

	//把直线延长
	void PutLineEx(CCreateBase* pCreateBase,double LongExtend,int IsBegin);
	//把多段线延长
	void PutPolylineEx(CCreateBase* pCreateBase,double LongExtend,int IsBegin);
	
private:
	//选择的是哪端点延长,1起点，2末点，3两头都延长，0没有延长;
	int m_iExtendWhichTop;
	//临时用
	Position m_posBegin,m_posEnd;
	//延长长度
	double m_dLongExtend,m_dAgl;
	//撤消类的对象
	CUndoRedo m_UndoRedo;
	//向撤消链表里添加的一个结点	
	PUNDOLIST m_pUndoList;
	//存图形的指针
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
	//被选中的图形
	CCreateBase *m_pCreateBase;	
	
	
	//对线延长
	void ExtendLine(CCreateBase* pCreateBase,double LongExtend,int IsBegin);
	//打断成功
	void SuccessExtend();
	//当前点与起点近时返回1，当前点与末点近时返回2，与两边一样近时返回0
	int ClosePos(CCreateBase* pCreateBase,Position pos);
	//向撤消链表里添加操作
	void AddUndoList(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);

};


class CDrawChamfer  :public CDrawBase//倒角
{
public:
	CDrawChamfer();
	virtual ~CDrawChamfer();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);



private:
	CCreateBase* m_createbaseFirst,*m_createbaseSecond;//两次选中的图形
	//选中单个图形的第几条线。。。
	int m_iPick1,m_iPick2;
	//倒角的半径
	double m_dRadius;
	//是不是倒凸角
	BOOL m_bConvex;
	//是不是输入的半径
	UINT m_inFlags;
	//鼠标点击的第三点
	Position m_posClickThree;
	CArray<Position,Position&> m_AryPoint;
	CArray<double,double&> m_aryTudu;
	
	//得到圆心，起点终点	两个向量上的点									弧的方向	起点，			终点，			圆心
	void GetArcPoint(Position pos1,Position pos2,Position pos3,int clock,Position& begin,Position& end,Position& center);
	//铣多线段
	BOOL ChamferPolyline();
	//向链表里添加一个操作结点
	
};


class CDrawMeasure :public CDrawBase  //测量
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


class CDrawPoint  :public CDrawBase//画点
{
public:
	CDrawPoint();
	virtual ~CDrawPoint();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:
	//把这个操作添加到撤消链表
	void AddUndoList(LONG pCreateBase);

	
	//点击右键的点
	Position m_posRight;
	//被选中的图形
	CCreateBase *m_pCreateBase;
	

};


class CDrawAddPoint : public CDrawBase //增加点 
{
public:
	CDrawAddPoint();
	virtual ~CDrawAddPoint();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
private:
	//把这个操作添加到撤消链表
//	void AddUndoList(LONG pCreateBase);
	
	Position m_Begin;
};


class CDrawMoveView  : public CDrawBase //移动图形
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


class CDrawZoomScale  : public CDrawBase //图形按比例缩放
{
public:
	CDrawZoomScale();
	virtual ~CDrawZoomScale();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
	
private:
	//基点
	Position m_posBase;
	//是否输入基点
	BOOL m_bBase;

	int OnZoomScale(double dScale);
};


class CDrawVerticalLine  :public CDrawBase//作垂直线
{
public:
	CDrawVerticalLine();
	virtual ~CDrawVerticalLine();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
	
private:
	//把这个操作添加到撤消链表
	void AddUndoList(LONG pCreateBase);
	//求点击到的线的垂线的角度
	BOOL GetAngleVert(Position pos);
	//向角度集合里加角度
	void AddAngleToAry(double dAngle);
	//当点到多段线上或矩形或椭圆上时垂直的角度
							//点击的哪条线，点击的点，	多段线的和数据
	void GetPolylineAngleVert(int iPick,Position posClick,CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);


	Position m_Begin;
	Position m_End;
	double m_dLongLine;//线长
	CArray<double,double&>	m_aryAngleVertline;  //选中的线的角度集合
	
};


class CDrawParallel : public CDrawBase  //平等线
{
public:
	CDrawParallel();
	virtual ~CDrawParallel();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);

private:

	//变铣刀线的接口
	void AddParallel();
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
	//取得点击的两个点，根据这两个点判断鼠标在图形的左右
	void GetVecter(Position posClick);

	Position m_Begin,m_End;
	double m_dLongLine;//线长
	BOOL m_bIsLeft;// 是不是左边平行
	long m_lCreateBasePick;//选中的图形的指针
	//点中多段线的哪条线
	int m_iPick;
	//鼠标移动时图形指针
	CCreateBase* m_pDragCreatebase;
	
};


class CDrawTrim  : public CDrawBase  //修剪
{
public:
	CDrawTrim();
	virtual ~CDrawTrim();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);
private:
	//向撤消链表里添加操作
	void AddUndoList();
	CCreateBase* m_createbaseFirst,*m_createbaseSecond;//两次选中的图形
	//选中单个图形的第几条线。。。
	int m_iPick1,m_iPick2;
	//第一次点击的点和第二次点击的点
	Position m_posFirst,m_posSecond;
	//撤消类的对象
	CUndoRedo m_UndoRedo;
	//向撤消链表里添加的一个结点	
	PUNDOLIST m_pUndoList;
	//存图形的指针
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;

	
	//把图形修剪
	void DoTrimGraph();
	//修剪直线
	void TrimLine();
	//修剪圆弧
	void TrimAarc();
	//修剪圆
	void TrimRound();	
	//修剪多段线
	void TrimPolyline(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);

	//返回直线与多段线的交点修剪直线    哪段线					返回的交点			
	BOOL GetInterPosLineAndPolyline(int& iNumPoint,Position& posInterClose,Position posBegin,Position posEnd,
		CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);
	
	//返回圆弧与多段线的交点修剪直线    哪段线					返回的交点			
	BOOL GetInterPosArcAndPolyline(int& iNumPoint,Position& posInterClose,Position posBegin,
		Position posEnd,Position posCenter,double dRadius,int iDir,
		CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);
	
	//返回圆与多段线的交点修剪直线    哪段线					返回的交点			
	BOOL GetInterPosRoundAndPolyline(int& iNumPoint,Position& posInterClose,Position posCenter,
		double dRadius,CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu);
	
	
	//返回多段线与多段线的交点修剪多段线   哪段线					返回的交点			
	BOOL GetInterPosPolylineAndPolyline(int& iNumPoint,Position& posInterClose,CArray<Position,Position&> &aryPoint1,
		CArray<double,double&> &aryTudu1,CArray<Position,Position&> &aryPoint2,CArray<double,double&> &aryTudu2);
	
	//生成新的多段线
	BOOL GeneratePolyline(CArray<Position,Position&> &aryPoint,CArray<double,double&> &aryTudu,
		Position posInter,int l_iNumPoint);
	//		       交点	      交点在多段线的哪段线上

	//生成新圆弧
	BOOL GenerateArc(LONG pArc,Position posInter);
	
	//通过圆生成新圆弧
	BOOL GenerateRound(LONG pRound,Position posInter);
	
	//提示没有交点
	void RemindNoInter();
};


class CDrawMovePoint  : public CDrawBase  //移动点
{
public:
	CDrawMovePoint();
	virtual ~CDrawMovePoint();

	int OnLButtonDown(UINT nFlags,const Position &pos,CString cmd);
	int OnMouseMove(UINT nFlags,const Position &pos);
	int OnRButtonDown(UINT nFlags,const Position &pos);	
private:
	//向撤消链表里添加操作
	void AddUndoList(CCreateBase* pCreateBase);
	//查找端点
	BOOL FindPointEnd(Position pos);
	//复制一个鼠标移动时要画的链表
	void CopyList();
	//释放m_aryDrawMouseMove
	void ClearList();

	struct stcMovePoint
	{	
		//被点中的图形
		CCreateBase* pCreateBase;
		//哪种图形
		int iStyleGraph;
		//是否是直线的起点
		BOOL bIsBeginPos;
		//多段线上的哪个点
		int iNumPolyMovePoint;
	};
	//执行点移动
	void DoMovePoint(CArray<stcMovePoint,stcMovePoint&>& aryMovePoint,Position pos,BOOL bAddUndo = TRUE);
	
	//选定的点
	Position m_posFirst,m_posPre;
	//要移动图形的链表	
	CArray<stcMovePoint,stcMovePoint&> m_aryMovePoint;
	//鼠标移动时要画的复制的链表	
	CArray<stcMovePoint,stcMovePoint&> m_aryDrawMouseMove;
	//撤消类的对象
	CUndoRedo m_UndoRedo;
	//向撤消链表里添加的一个结点	
	PUNDOLIST m_pUndoList;
	//存图形的指针
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;
	
};

#endif // !defined(AFX_DRAW_H__EFDD11CF_FCEC_4D5A_9044_F86D3DE72299__INCLUDED_)
