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
#define PRECISION 8		//ѡȡͼԪ�ľ���

//�����Χ��
typedef struct _box2d BOX2D ;
struct _box2d
{
    double      min[2], max[2] ; // ����������С����ֵ
};

#include "math.h"

//����λ����
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


	//�ƶ��ľ���
	Position Offset(const double & deltax,const double & deltay);
	Position Offset(const Position& off);
	//��ת�㷨	
	Position Rotate(const Position& basePos, const double angle) ;
	//�����㷨
	Position Mirror(const Position& pos1, const Position& pos2) ;
	//�㵽ֱ�ߵľ���
	double PotDisLine(Position pos1,Position pos2);
	//����֮��ľ���
	double Distance(const Position& pos);	
	//����֮��ľ����ƽ��
	double DistanceSqr(const Position& pos);	
	//�㵽ֱ�ߵĴ����-----ǰ�������ǹ�ֱ�ߵģ���һ�����ǵ�ֱ�߾�����Ǹ���
	Position DistancePosFoot(const Position& PosLine1,const Position& PosLine2);
	//�жϵ�������������
	int AnalysisClockDir(Position pos1,Position pos2);



	//���������,�����������������������
	Position  operator+(const Position& position1);
	friend	Position  operator-(const Position& position1,const Position& position2);
	Position operator/(double num);	
	Position operator=(const Position& positionSrc);

	BOOL operator ==(const Position& position1);
	BOOL operator !=(const Position& position1);
};
//������ȫ�ֺ���

//����������X��ĽǶȣ�0~2PI
double GetAngleToXAxis(const Position& pos1,const Position& pos2);

//���������ĽǶ�0~360
double GetAngle(const Position& dStartPoint,const Position& dOtherPoint);

//ȫ�ֺ���������������ļн�0~180
double GetAngleThreePos(Position dStartPoint,Position dMiddlePoint,Position dEndPoint);

//�Ƿ����
BOOL PosEqual(const Position& pos1, const Position& pos2);

//���Ƿ��ڰ�Χ����1111111111
BOOL PosBelongBox(Position begin,Position end,Position pos);
//	���Ƿ��ڰ�Χ����222222222
BOOL IsBelongBox(Position pos,BOX2D pBox);

//��ʾ�ַ�����
void RemindContent(CString Content,int Same = 0);

//	ȫ�ֺ���:����͹�ȷ��ط���
int RtnArcDir(double Tudu);

//	ȫ�ֺ���:��Position�ṹת��POSITION
void PosToPOS(Position& Pos,DOUBLEPOINT POS);

//	ȫ�ֺ���:ֻ����Ч����
double RetainValid(double digits,int valid);

//	ȫ�ֺ���������Բ�ϵ�������Բ
BOOL Pos3OutRound(Position pos1,Position pos2,Position pos3,Position& posCenter,double& Radius);

//������ֱ�ߵĽ���			�������ϵ��ĸ���									���صĽ���
BOOL GetIntersectPoint(Position pos1,Position pos2,Position pos3,Position pos4,Position &pos);

//���Ƿ����߶���
BOOL PosOnLine(Position pos1,Position pos2,Position pos,double dPrecition = DOUBLE_EQUAL);

//���Ƿ���Բ����
BOOL PosOnArc(Position pos,Position posBegin,Position posEnd,Position posCenter,double dRadius,int iDir);

//����Բ�������������Բ
BOOL Pos2LineOutRound(Position posRound1,Position posRound2,Position posLine1,Position posLine2,Position& posCenter1,double& Radius1,Position& posCenter2,double& Radius2);

//��һԪ���η���
BOOL SolveEquation(double a,double b,double c,double& x1,double& x2);

//��ֱ����Բ�Ľ���                   ֱ������                         Բ��Բ��        Բ���İ뾶  
BOOL GetIntersectLineRound(Position& posLine1,Position& posLine2,Position posArcCenter,double dRadius);

//��ֱ����Բ���Ľ���
int GetIntersectLineArc(Position& posLine1,Position& posLine2,Position posBegin,
						 Position posEnd,Position posCenter,double dRadius,int iDir);

//Բ�뻡�Ľ���------��.cpp������
int GetIntersectArcRound(Position posRoundCenter,double dRoundRadius,Position posBegin,Position posEnd,
				 Position posArcCenter,double dArcRadius,int iDir,Position& posInter1,Position& posInter2);

//���뻡�Ľ���------��.cpp������
int GetIntersectTwoArc(Position posBegin1,Position posEnd1,Position posCenter1,double dRadius1,int iDir1
					  ,Position posBegin2,Position posEnd2,Position posCenter2,double dRadius2,int iDir2
					   ,Position& posInter1,Position& posInter2);
//��Բ�Ľ���
int GetIntersectTwoRound(Position posArcCenter1,double dRadius1,Position posArcCenter2,double dRadius2
						 ,Position& posInter1,Position& posInter2);

//����double���Ƿ����
BOOL DoubleEquals(double dDouble1,double dDouble2);

#endif // !defined(AFX_POSITION_H__C51F723F_6C21_4773_B156_1D6D11979226__INCLUDED_)
