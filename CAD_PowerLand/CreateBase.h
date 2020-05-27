// CreateBase.h: interface for the CCreateBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEBASE_H__C0489D78_12A7_40EF_AE22_BAC172342421__INCLUDED_)
#define AFX_CREATEBASE_H__C0489D78_12A7_40EF_AE22_BAC172342421__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Position.h"
#include "afxtempl.h"
#include "math.h"
#include "Undo.h"		//定义撤消结构的头文件
#include "spline.h"//多段线


enum EDrawMode // 绘图模式
{
	dmNormal = 0, // 正常状态
	dmSelect = 1, // 选中状态	
	dmPrompt = 2, // 提示状态
	dmDrag   = 3, // 拖动状态
	dmInvalid = 4, // 擦除状态
	dmLayer = 5,
	dmDragSelect1 = 6,
	dmDragSelect2 = 7,
	ARC = 1,
	LINE = 2
};



class CCreateBase : public CObject//这样它的对象也是COblist的对象
{
	DECLARE_SERIAL(CCreateBase)
public:
	CCreateBase();
	virtual ~CCreateBase();
	// 绘制图元对象
//原来用DGI+时，画图的函数	virtual void Draw(CDC *pDC) {};//点击第二点时画的线
 
	virtual void DrawMove(CDC *pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);	//鼠标移动时的橡皮线
	//弹出属性对话框的界面并显示图元属性
	virtual void PropertyView() {};
	//是否被选中
	virtual int IsPick(CPoint point)  {return -1;};
	//图元的移动	
	virtual void Move(const Position& basePos,const Position& desPos) {};
	//对于图形上的一点输入长度返回一个点
	virtual BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine) {return FALSE;};
	// 给定一个基点和一个角度值旋转图元
	virtual void Rotate(const Position& basePos, const double angle) {}	;
	// 给定两点镜像图元，布尔变量bCopy用于确定是否删除原来位置的图元
	virtual void Mirror(const Position& pos1, const Position& pos2){};
	//XY缩放功能
	virtual BOOL GraphZoomXY(double dValueX, double dValueY) {return FALSE;};		
	// 指针拷贝重新生成一个，用于修改的重画的那些函数，MOUSEMOVE函数里。
	virtual	CCreateBase* Copy() { return NULL; } 
	//当在特性对话框弹出改变图元特性时要重画的的函数
	virtual void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0) {};
	// 得到对象的最小包围盒，该包围盒将被用于图元的选取和全屏显示
	virtual BOOL IsSelect(CRgn *selectrect,BOOL bComplex){return FALSE;};
	//得到包围盒						是否加外括
	virtual BOOL GetBox(BOX2D* pBox,BOOL IsOutside = FALSE){return FALSE;};
	//改变切割方向
	virtual void ChangeDir(){};
	//按比例缩放
	virtual void GraphZoomScale(Position posBase,double dScale) {};
	//实线变虚线
	virtual PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist) {return FALSE;};




	//返回线色号如，黑色1，红色2.。。。
	int RtnColorInt();
	//让属性对话框中的控件显示个数
	void ShowControl(int show,BOOL btn);
	//属性对话框里的编辑框改变时的撤消
	void AddUndoList(double dEditNew,double dEditOld,int iWhichEdit,int iNumOfTop=0);
	
	COLORREF m_ColorOfLine;		//每个图元的线色;
	CArray<CString,CString&> m_strSBM;//每个图元对应的M代码
	CArray<CString,CString&> m_strCBM;
	int m_iIsBeginGraph;//标志最开始打开的图形
	int m_iKnifeStyle;//刀补类型 41左刀补 42右刀补
	CArray<Position,Position&> m_aryDash;//变虚线时存每个小线段
	double m_dRemainingDash; //变虚线时的剩余长度
	
protected:
	//用直线画圆弧函数
	void DrawArc(CDC* pDC,CPoint potCenter,double dRadius,double dAngleStart,double dAngleOffset);
	//画箭头的函数
	void DrawArrow(double angle,CPoint PosEnd,CDC* pDC);
	// 根据绘图模式设置绘图环境：设置设备环境的模式并创建画笔
	void SetDrawEnvir(CDC*	pDC, int drawMode, CPen* pPen);
	//选中后的小方块
	void DrawRectForPick(CDC* pDC,CPoint Point);
	//把圆弧变成虚线
	PGRAPHPOS TurnArcDash(double dLongSolid,double dLongEmpty,Position posCenter,double dAngleStart,
		double dAngeleEnd,double dRadius,int iDirect);	
	//把直线变成虚线
	PGRAPHPOS TurnLineDash(double dLongSolid,double dLongEmpty,Position posBegin,Position PosEnd);
	
	



private:
	//看下刀库里是否与字符串相等
	BOOL MstrIsEqualMary(CString strM,CArray<CString,CString&>& aryM);

};

class CCreateLine  :public CCreateBase
{
	DECLARE_SERIAL(CCreateLine)
public:
	Position m_Begin;
	Position m_End;
	
public:
	CCreateLine();
	CCreateLine(const Position& begin,const Position& end);
	virtual ~CCreateLine();
	

	void DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);
	void DrawMove(CDC* pDC,int DrawMode,BOOL Poly);
	int IsPick(CPoint point);
	
	void Move(const Position& basePos,const Position& desPos);
	void Rotate(const Position& basePos, const double angle);
	void Mirror(const Position& pos1, const Position& pos2);
	//改变切割方向
	void ChangeDir();

	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	CCreateBase* Copy();
	
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	void PropertyView();
	//得到包围盒
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	//XY缩放功能
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//按比例缩放
	void GraphZoomScale(Position posBase,double dScale);
	//对于图形上的一点输入长度返回一个点
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//实线变虚线
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	
private:
	//看直线是否与矩形有交点
	BOOL LineOnRect(Position pos1,Position pos3);
	
};


class CCreatePolyline  :public CCreateBase
{
	DECLARE_SERIAL(CCreatePolyline)
public:
	CCreatePolyline();
	//不带圆弧的
	CCreatePolyline(CArray<Position,Position&>&  point);
	//带圆弧的
	CCreatePolyline(CArray<Position,Position&>&  point,CArray<double,double&>& TuDu);
	virtual ~CCreatePolyline();
	void DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);
	//鼠标单击拾取
	int IsPick(CPoint point);
	CCreateBase* Copy();

	void Move(const Position& basePos,const Position& desPos);
	void Rotate(const Position& basePos, const double angle);
	void Mirror(const Position& pos1, const Position& pos2);
	//改变切割方向
	void ChangeDir();
	
	
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	void PropertyView();
	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	//得到包围盒
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	//XY缩放功能
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//对于图形上的一点输入长度返回一个点
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//按比例缩放
	void GraphZoomScale(Position posBase,double dScale);
	//实线变虚线
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	

public:
	CArray<Position,Position&> m_point;//点集
	CArray<double,double&> m_dTuDu;//存放凸度的集合
	
private:
	//画图元，有可能是直线，也有可能是圆弧
	void DrawLineOrArc(CDC *pDC,int i,BOOL FlagShowArrow,BOOL FlagShowNum,Position l_posCurPos,Position l_posNextPos,double TuDu);
	//画直线和圆弧
	void DrawLineAndArc(CDC *pDC,BOOL FlagShowArrow,Position l_posCurPos,Position l_posNextPos,double TuDu);
	//直线的拾取方法
	BOOL IsPickLine(Position Pos1,Position Pos2,CPoint point);
	//圆弧的拾取方法
	BOOL IsPickArc(Position Pos1,Position Pos2,CPoint point,double TuDu);
	//输入起点终点凸度返回弧长
	double RtnLongArc(Position posClick,Position posBegin,Position PosEnd,double dTudu);
	//输入起点，终点，凸度，点击点，弧长，返回弧上点
	Position RtnLongArcPos(Position posBegin,Position PosEnd,double dTudu,Position posClick,double dLongPos);
	//看多段线是否与矩形有交点并且交点是否在直线上
	BOOL PolylineOnRect(Position pos1,Position pos3);
	//点是否在圆弧上
	BOOL PosOnPolylineArc(Position pos,Position posBegin,Position posEnd,Position posCenter,int iDir);
	//返回在屏幕里显示的起始角和转过角度
	BOOL GetAngleStartAndOffset(double& dAngleStart,double& dAngleOffset,CPoint& potBegini,int iDir,Position posCenter,Position posBegin,Position posEnd);
	//计算起始角和转过角度
	BOOL CaleStartAndOffset(double& dAngleStart,double& dAngleOffset,int iDir,double arcStart,double arcEnd);
	//得到圆弧的包围盒
	BOOL GetArcBox(BOX2D* pBox,Position posCenter,Position posBegin,Position posEnd,int iDir);

	//标志上一次是圆弧还是直线1:直线   2：圆弧
	int m_iFlagArcOrLine;	
};


class CCreateRectangle  :public CCreateBase
{
	DECLARE_SERIAL(CCreateRectangle)
public:
	CArray<Position,Position&> m_point;
	CArray<double,double&> m_dTuDu;//存放凸度的
	

public:
	CCreateRectangle();
	//给修改的那些函数使用，Copy()函数使用。
	CCreateRectangle(CArray<Position,Position&>&  point,CArray<double,double&>& TuDu);
	//给画图，创建图元时使用。
	CCreateRectangle(const Position& begin,const Position& end,BOOL FlagRoundRect = FALSE,double Radius = 0);
	virtual ~CCreateRectangle();


	void DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);
	int IsPick(CPoint point);
	CCreateBase* Copy();

	void Move(const Position& basePos,const Position& desPos);
	void Rotate(const Position& basePos, const double angle);
	void Mirror(const Position& pos1, const Position& pos2);
	//改变切割方向
	void ChangeDir();
	
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	
	void PropertyView();
	//得到包围盒
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	//XY缩放功能
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//对于图形上的一点输入长度返回一个点
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//按比例缩放
	void GraphZoomScale(Position posBase,double dScale);
	//实线变虚线
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	

private:
	//画图元，有可能是直线，也有可能是圆弧
	void DrawLineOrArc(CDC *pDC,int i,BOOL FlagShowArrow,BOOL FlagShowNum,Position l_posCurPos,Position l_posNextPos,double TuDu);
	//画直线和圆弧
	void DrawLineAndArc(CDC *pDC,BOOL FlagShowArrow,Position l_posCurPos,Position l_posNextPos,double TuDu);
	//直线的拾取方法
	BOOL IsPickLine(Position Pos1,Position Pos2,CPoint point);
	//圆弧的拾取方法
	BOOL IsPickArc(Position Pos1,Position Pos2,CPoint point,double TuDu);
	//输入起点终点凸度返回弧长
	double RtnLongArc(Position posClick,Position posBegin,Position PosEnd,double dTudu);
	//输入起点，终点，凸度，点击点，弧长，返回弧上点
	Position RtnLongArcPos(Position posBegin,Position PosEnd,double dTudu,Position posClick,double dLongPos);
	//看矩形是否与矩形有交点并且交点是否在直线上
	BOOL RectangleOnRect(Position pos1,Position pos3);
	//点是否在圆弧上
	BOOL PosOnRectangleArc(Position pos,Position posBegin,Position posEnd,Position posCenter,int iDir);
	
	
	
	//标志上一次是圆弧还是直线1:直线   2：圆弧
	int m_iFlagArcOrLine;		
};


class CCreateRoundArc  :public CCreateBase
{
	DECLARE_SERIAL(CCreateRoundArc)
public:
	CCreateRoundArc();
	CCreateRoundArc(const Position Begin,const Position Center,const Position End,int Direct);
	virtual ~CCreateRoundArc();


	void DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);
	int IsPick(CPoint point);
	CCreateBase* Copy();

	void Move(const Position &basePos,const Position &desPos);
	void Rotate(const Position& basePos, const double angle);
	void Mirror(const Position& pos1, const Position& pos2);
	//改变切割方向
	void ChangeDir();
	
	
	void PropertyView();
	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	//得到包围盒
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	//XY缩放功能
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//对于图形上的一点输入长度返回一个点
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//按比例缩放
	void GraphZoomScale(Position posBase,double dScale);
	//实线变虚线
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	
	
public:
	Position m_Begin;
	Position m_Center;
	Position m_End;
	double m_dRadius;
	double m_dAngleStart;
	double m_dAngleEnd;
	int m_iDirect;	//画圆或弧的方向

private:
	//圆弧是否在矩形上
	BOOL ArcOnRect(Position pos1,Position pos3);
	//点是否在圆弧上
	BOOL PosOnArc(Position pos);
	//返回在屏幕里显示的起始角和转过角度
	BOOL GetAngleStartAndOffset(double& dAngleStart,double& dAngleOffset,CPoint& potBegin);
	//计算起始角和转过角度
	BOOL CaleStartAndOffset(double& dAngleStart,double& dAngleOffset);
};


class CCreateRound  :public CCreateBase
{
	DECLARE_SERIAL(CCreateRound)
public:
	CCreateRound();
	CCreateRound(const Position& center,const Position& endpos,int Direct);
	virtual ~CCreateRound();


	void DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);
	int IsPick(CPoint point);
	CCreateBase* Copy();

	void Move(const Position& basePos,const Position& desPos);
	void Rotate(const Position& basePos, const double angle);
	void Mirror(const Position& pos1, const Position& pos2);
	//改变切割方向
	void ChangeDir();
	
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	
	void PropertyView();
	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	//得到包围盒
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	//XY缩放功能
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//对于图形上的一点输入长度返回一个点
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//按比例缩放
	void GraphZoomScale(Position posBase,double dScale);
	//实线变虚线
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	
public:
	Position m_Center;		
	Position m_EndPos;
	double m_dRadius;		
	int m_iDirect;	//画圆或弧的方向

private:
	//圆弧是否在矩形上
	BOOL RoundOnRect(Position pos1,Position pos3);
	
	
};


class CCreateSpline  :public CCreatePolyline
{
	DECLARE_SERIAL(CCreateSpline)
public:
	CCreateSpline();
	virtual ~CCreateSpline();

	
	//构造函数
	CCreateSpline(CArray<Position,Position&>&  point,CArray<Position,Position&>& aryControlPoints,CArray<int,int&>& aryNumControl);
	//画样条曲线
	void DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);
	CCreateBase* Copy();
	
	void Move(const Position& basePos,const Position& desPos);
	void Rotate(const Position& basePos, const double angle);
	void Mirror(const Position& pos1, const Position& pos2);
	//改变切割方向
	void ChangeDir();	
	//改变属性框里的内容时
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	//通过控制点刷新样条曲线
	void RefreshSpline();


	//控制点集
	CArray<Position,Position&> m_aryControlPoints;
	//控制点在曲线点集中的位置
	CArray<int,int&> m_aryNumControl;	
	
private:
	//在屏幕上的起点坐标
	CPoint m_potBegin;
};


class CCreateEllipse  :public CCreatePolyline
{
	DECLARE_SERIAL(CCreateEllipse)
public:
	CCreateEllipse();
	CCreateEllipse(const Position& begin,const Position& second,const Position& end);
	virtual ~CCreateEllipse();


	void DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);
	int IsPick(CPoint point);
	CCreateBase* Copy();

	void Move(const Position& basePos,const Position& desPos);
	void Rotate(const Position& basePos, const double angle);
	void Mirror(const Position& pos1, const Position& pos2);
	//改变切割方向
	void ChangeDir();
	
	void PropertyView();
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	
	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	//得到包围盒
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	//XY缩放功能
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//对于图形上的一点输入长度返回一个点
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//按比例缩放
	void GraphZoomScale(Position posBase,double dScale);
	//实线变虚线
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	
private:
	int GetArcQuadrant(double &fArc);//根据角度求象限	
	//获取椭圆弧上对应角度为fArc的一个点的坐标
	void GetEllipsesPoint(Position &output, double fArc);
	//输入起点终点凸度返回弧长
	double RtnLongArc(Position posClick,Position posBegin,Position PosEnd,double dTudu);
	//输入起点，终点，凸度，点击点，弧长，返回弧上点
	Position RtnLongArcPos(Position posBegin,Position PosEnd,double dTudu,Position posClick,double dLongPos);	
	//看椭圆是否与矩形有交点
	BOOL EllipseOnRect(Position pos1,Position pos3);



public:
	Position m_Begin;
	Position m_Second;
	Position m_End;
	double m_a;	//长轴长
	double m_b;	//短轴长
	Position m_Center;//椭圆的圆心点坐标
	Position m_Focus1,m_Focus2;	//椭圆的焦点坐标
	Position m_PrePos;		//存放前一点
	double m_k;	//长轴的斜率（椭圆旋转角度）
//	CArray<Position,Position&> m_point;	
//	CArray<double,double&> m_dTuDu;//存放凸度的	

private:
	//标志上一次是圆弧还是直线1:直线   2：圆弧
	int m_iFlagArcOrLine;		
	
};


class CCreateMCode  :public CCreateBase
{
	DECLARE_SERIAL(CCreateMCode)
public:
	CCreateMCode();
	CCreateMCode(CString str);
	virtual ~CCreateMCode();
	//得到包围盒
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);

	CString m_strM_What;	//文件头存放的M代码
};


class CCreatePoint  :public CCreateBase
{
	DECLARE_SERIAL(CCreatePoint)		
public:
	CCreatePoint();
	CCreatePoint(Position pot);
	virtual ~CCreatePoint();

	void DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);
	int IsPick(CPoint point);
	
	void Move(const Position& basePos,const Position& desPos);
	void Rotate(const Position& basePos, const double angle);
	void Mirror(const Position& pos1, const Position& pos2);
	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	CCreateBase* Copy();
	
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	void PropertyView();
	//得到包围盒
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	

	Position m_Begin;

};


class CCreateKong : public CCreateBase  
{
	DECLARE_SERIAL(CCreateKong)		
public:
	CCreateKong();
	virtual ~CCreateKong();

};

#endif // !defined(AFX_CREATEBASE_H__C0489D78_12A7_40EF_AE22_BAC172342421__INCLUDED_)




