// Position.h: interface for the Position class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POSITION_H__C51F723F_6C21_4773_B156_1D6D11979226__INCLUDED_)
#define AFX_POSITION_H__C51F723F_6C21_4773_B156_1D6D11979226__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MATH_PI			acos(-1)
#define MATH_2PI	(MATH_PI*2)
#define DISTANCE_ZERO 1.e-5 
#define DOUBLE_EQUAL 1.e-10
#define DOUBLE_EQUALEST 1.e-20
#define PRECISION 8		//选取图元的精度

//定义包围盒
typedef struct _box2d BOX2D ;
struct _box2d
{
    double      min[2], max[2] ; // 定义最大和最小坐标值
};

#include "math.h"

//定义位置类
class Position  
{
public:
	double x;
	double y;

public:


public:
	Position();
	Position(double inX,double inY);
	virtual ~Position();


	//移动的距离
	Position Offset(const double & deltax,const double & deltay);
	Position Offset(const Position& off);
	//旋转算法	
	Position Rotate(const Position& basePos, const double angle) ;
	//镜像算法
	Position Mirror(const Position& pos1, const Position& pos2) ;
	//点到直线的距离
	double PotDisLine(Position pos1,Position pos2);
	//两点之间的距离
	double Distance(const Position& pos);	
	//两点之间的距离的平方
	double DistanceSqr(const Position& pos);	
	//点到直线的垂足点-----前两个点是过直线的，后一个点是到直线距离的那个点
	Position DistancePosFoot(const Position& PosLine1,const Position& PosLine2);
	//判断点在向量的左右
	int AnalysisClockDir(Position pos1,Position pos2);



	//重载运算符,以下是两种重载运算符方法
	Position  operator+(const Position& position1);
	friend	Position  operator-(const Position& position1,const Position& position2);
	Position operator/(double num);	
	Position operator=(const Position& positionSrc);

	BOOL operator ==(const Position& position1);
	BOOL operator !=(const Position& position1);
};
//以下是全局函数

//反回两点与X轴的角度，0~2PI
double GetAngleToXAxis(const Position& pos1,const Position& pos2);

//返回向量的角度0~360
double GetAngle(const Position& dStartPoint,const Position& dOtherPoint);

//全局函数：返回三个点的夹角0~180
double GetAngleThreePos(Position dStartPoint,Position dMiddlePoint,Position dEndPoint);

//是否相等
BOOL PosEqual(const Position& pos1, const Position& pos2);

//点是否在包围盒内1111111111
BOOL PosBelongBox(Position begin,Position end,Position pos);
//	点是否在包围盒里222222222
BOOL IsBelongBox(Position pos,BOX2D pBox);

//提示字符内容
void RemindContent(CString Content,int Same = 0);

//	全局函数:输入凸度返回方向
int RtnArcDir(double Tudu);

//	全局函数:把Position结构转成POSITION
void PosToPOS(Position& Pos,DOUBLEPOINT POS);

//	全局函数:只留有效数字
double RetainValid(double digits,int valid);

//	全局函数：根据圆上的三点求圆
BOOL Pos3OutRound(Position pos1,Position pos2,Position pos3,Position& posCenter,double& Radius);

//求两条直线的交点			两条线上的四个点									返回的交点
BOOL GetIntersectPoint(Position pos1,Position pos2,Position pos3,Position pos4,Position &pos);

//点是否在线段上
BOOL PosOnLine(Position pos1,Position pos2,Position pos,double dPrecition = DOUBLE_EQUAL);

//点是否在圆弧上
BOOL PosOnArc(Position pos,Position posBegin,Position posEnd,Position posCenter,double dRadius,int iDir);

//根据圆上两点和切线求圆
BOOL Pos2LineOutRound(Position posRound1,Position posRound2,Position posLine1,Position posLine2,Position& posCenter1,double& Radius1,Position& posCenter2,double& Radius2);

//解一元二次方程
BOOL SolveEquation(double a,double b,double c,double& x1,double& x2);

//求直线与圆的交点                   直线两点                         圆弧圆心        圆弧的半径  
BOOL GetIntersectLineRound(Position& posLine1,Position& posLine2,Position posArcCenter,double dRadius);

//求直线与圆弧的交点
int GetIntersectLineArc(Position& posLine1,Position& posLine2,Position posBegin,
						 Position posEnd,Position posCenter,double dRadius,int iDir);

//圆与弧的交点------在.cpp里声明
int GetIntersectArcRound(Position posRoundCenter,double dRoundRadius,Position posBegin,Position posEnd,
				 Position posArcCenter,double dArcRadius,int iDir,Position& posInter1,Position& posInter2);

//弧与弧的交点------在.cpp里声明
int GetIntersectTwoArc(Position posBegin1,Position posEnd1,Position posCenter1,double dRadius1,int iDir1
					  ,Position posBegin2,Position posEnd2,Position posCenter2,double dRadius2,int iDir2
					   ,Position& posInter1,Position& posInter2);
//两圆的交点
int GetIntersectTwoRound(Position posArcCenter1,double dRadius1,Position posArcCenter2,double dRadius2
						 ,Position& posInter1,Position& posInter2);

//两个double型是否相等
BOOL DoubleEquals(double dDouble1,double dDouble2);

#endif // !defined(AFX_POSITION_H__C51F723F_6C21_4773_B156_1D6D11979226__INCLUDED_)
