// Position.cpp: implementation of the Position class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cad_powerland.h"
#include "Position.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


Position::Position()
{
	x = y = 0;
}

Position::Position(double inX,double inY)
{
	x = inX;
	y = inY;
}

Position::~Position()
{

}

double Position::Distance(const Position& pos)
{
	double l_dDistance = sqrt( (x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y));
	return l_dDistance;
}

/*
 *	两点之间的距离的平方
 */
double Position::DistanceSqr(const Position& pos)
{
	double l_dSqr = (x-pos.x)*(x-pos.x) + (y-pos.y)*(y-pos.y);
	return l_dSqr;
}

Position Position::operator=(const Position& positionSrc)
{
	x=positionSrc.x; y=positionSrc.y;
	return *this;
}

Position Position::operator+(const Position& position1)
{
	Position p(position1.x+x, position1.y+y);
	return p;
}

BOOL Position::operator==(const Position& position1)
{
	if(fabs(position1.x - x) < DISTANCE_ZERO&&fabs(position1.y - y) < DISTANCE_ZERO)
		return TRUE;
	return FALSE;
}

BOOL Position::operator!=(const Position& position1)
{
	if(fabs(position1.x - x) > DOUBLE_EQUAL||fabs(position1.y - y) > DOUBLE_EQUAL)
		return TRUE;
	return FALSE;
}

Position  operator-(const Position& position1,const Position& position2)
{
	Position p(position1.x-position2.x, position1.y-position2.y);
	return p;
}

Position Position::operator /(double num)
{
	Position p(x/num, y/num);
	return p;
}


// 全局函数：计算两点posSrc1 和 posSrc2连线与x轴的夹角同，计算两点角度0-2PI这间
double GetAngleToXAxis(const Position& posSrc1,const Position& posSrc2)
{
	Position pos1(posSrc1),pos2(posSrc2);	//给定两点
	double len = pos1.Distance(pos2);		//两点这间距离
	if(len < DISTANCE_ZERO)		//超出计算精度，就返回
		return	0 ;
	Position dp = pos2 - pos1;	//向量点
	double cosv = dp.x/len ;	//邻边比斜边
	double sinv = dp.y/len ;	//对边比斜边
	if(sinv >= 0)//一二象限0~180
		return acos(cosv) ;//反余弦
	else if(sinv < 0)//三四象限180~360
		return MATH_2PI-acos(cosv) ;//
	return 0 ;
}


// 全局函数:点到直线的垂足点-----前两个点是过直线的，后一个点是到直线距离的那个点
Position Position::DistancePosFoot(const Position& PosLine1,const Position& PosLine2)
{
	//直线方程一般式Ax+By+c=0的系数
	double A,B,C;
	//根据两点求直线方程和系数，
	//斜率k=y1-y2/x1-x2,y=kx+b,再把一点代入能得到直线方程，化简后再推出一般式
	//则一般式中的A = y2 -y1; B = x1 - x2; C = x2*y1 - x1*y2;
	A = PosLine2.y - PosLine1.y;
	B = PosLine1.x - PosLine2.x;
	C = PosLine2.x*PosLine1.y - PosLine1.x*PosLine2.y;
	//垂足：设垂足坐标(x0,y0),求解两个方程：Ax + By + C = 0;(y - y0) / (x - x0) = B / A(斜率相反);
	//解得，x=(B*B*x0-A*B*y0-A*C)/(A*A + B*B);     y=(-A*B*x0+A*A*y0-B*C)/(A*A+B*B);
	Position PosFoot;
	PosFoot.x = (B*B*x-A*B*y-A*C)/(A*A + B*B);
	PosFoot.y = (-A*B*x+A*A*y-B*C)/(A*A+B*B);
	return PosFoot;
}

// 全局函数：计算两点角度0-360这间
double GetAngle(const Position& dStartPoint,const Position& dOtherPoint)
{
	double   angle;
	double   dx = dOtherPoint.x - dStartPoint.x;
	double   dy = dOtherPoint.y - dStartPoint.y;
	
	if (dx == 0)
	{
		if (dy > 0)			       angle = 90;
		else		    	       angle = 270;
	}
	else if (dy == 0)
	{
		if (dx > 0)			       angle = 0;
		else     			       angle = 180;
	}
	else
	{
		if (dy == 0 && dx == 0)	   angle = 0;
		else   		        	   angle = (180/MATH_PI) * (atan2(dy, dx));
		
		if (angle < 0)			   angle += 360;      
		else if (angle > 360)      angle -= 360;
	}
	
	return angle;
}

/*
 *	全局函数：返回三个点的夹角0~180
 *  根据余弦定理： 
 *  cosA = (c^2 + b^2 - a^2) / (2·b·c) 
 */
double GetAngleThreePos(Position dStartPoint,Position dMiddlePoint,Position dEndPoint)
{
	double a = dStartPoint.Distance(dEndPoint);
	double b = dMiddlePoint.Distance(dEndPoint);
	double c = dMiddlePoint.Distance(dStartPoint);
	if(b==0||c==0)//说明在一个点上
		return 0;
	double cos = (b*b + c*c - a*a )/ (2*b*c);
	if (fabs(1.0+cos)<DOUBLE_EQUAL)
		return MATH_PI;
	if(fabs(1.0-cos)<DOUBLE_EQUAL)
		return 0;
	double angle = acos(cos);
	return angle;	
}

/*
 *	 全局函数：判断两点是否相等
 */
BOOL PosEqual(const Position& pos1, const Position& pos2)
{
	if(DISTANCE_ZERO>fabs(pos1.x-pos2.x)&&DISTANCE_ZERO>fabs(pos1.y-pos2.y))
		return TRUE;
	return FALSE;
}

//全局函数：点是否在包围盒内
BOOL PosBelongBox(Position begin,Position end,Position pos)
{
	BOX2D pBox;		//包围盒结构
	double curRadius = 2.5 * g_pView->ReturnProportion();
	//得到包围盒的两个点，世界坐标（左下，右上）//2.5是把正好的包围盒放大，以防垂直的线
	pBox.min[0] = min( begin.x, end.x )-curRadius;
	pBox.min[1] = min( begin.y, end.y )-curRadius;
	pBox.max[0] = max( begin.x, end.x )+curRadius;
	pBox.max[1] = max( begin.y, end.y )+curRadius;
	if(pos.x>pBox.min[0]&&pos.x<pBox.max[0]&&pos.y>pBox.min[1]&&pos.y<pBox.max[1])
		return TRUE;
	return FALSE;
}


/*
 *	点是否在包围盒里
 */
BOOL IsBelongBox(Position pos,BOX2D pBox)
{
	if (pos.x < pBox.max[0]&&pos.x > pBox.min[0]&&pos.y < pBox.max[1]&&pos.y > pBox.min[1])
		return TRUE;
	return FALSE;
}

/*
 *	 全局函数：向提示框里加内容
 */
void RemindContent(CString Content,int Same)
{	
	CString str;
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItemText(IDC_EDIT_COMMAND,str);
	if (Same == SEND_ALL)//两个都发
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_REMIND,Content);
		Content+=str;//加命令框里的字符
		g_pDoc->m_strCmdAll+=Content;
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_RICHEDIT4,g_pDoc->m_strCmdAll);
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.m_RichEdit_Cmd.SendMessage(WM_VSCROLL,SB_BOTTOM ,0);
	}
	if(Same == SEND_EDIT)//只向EDIT框里发
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_REMIND,Content);
	if (Same == SEND_RICHEDIT)//只向RICHEDIT框里发
	{
		Content+=str;//加命令框里的字符
		g_pDoc->m_strCmdAll+=Content;
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_RICHEDIT4,g_pDoc->m_strCmdAll);
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.m_RichEdit_Cmd.SendMessage(WM_VSCROLL,SB_BOTTOM ,0);
	}
}

/*
 *	全局函数:输入凸度返回方向
 */
int RtnArcDir(double Tudu)
{
	int Dir;
	if(Tudu<0)
		Dir = AD_CLOCKWISE;
	else
		Dir = AD_COUNTERCLOCKWISE;
	return Dir;
}

//	全局函数:只留有效数字
double RetainValid(double digits,int valid)
{
	int num1 = (int)(digits*pow(10,valid));
	double num2 = num1/pow(10,valid);
	return num2;
}

//	全局函数:把Position结构转成POSITION
void PosToPOS(Position& Pos,DOUBLEPOINT POS)
{
	Pos.x = POS.x;
	Pos.y = POS.y;
}
//位置类对象的平移操作
Position Position::Offset(const double & deltax,const double & deltay)
{
	Position	pnt(x + deltax, y + deltay);
	return pnt;	
}
//位置类对象的平移操作
Position Position::Offset(const Position& off)
{
	Position	pnt(x + off.x, y + off.y);
	return pnt;
}

//位置类对象的旋转操作					
Position Position::Rotate(const Position& basePos, const double angle)
{
	Position p;
	double cosv = cos(angle) ; // cos value
	double sinv = sin(angle) ;
	double xc = x ;
	double yc = y ;
	//向量旋转后加上起点
	//向量旋转公式:（xcosa-ysina，xsina+ycosa）
	//点X为：x=(xc-basePos.x)* cosv - (yc-basePos.y)*sinv +basePos.x分解后如下：
	p.x = xc * cosv - yc * sinv + (1.- cosv) * basePos.x + basePos.y * sinv ;
	//点Y为：y=(xc-basePos.x)* sinv + (yc-basePos.y)*cosv +basePos.y分解后如下：
	p.y = sinv * xc + cosv * yc + (1.- cosv) * basePos.y - sinv * basePos.x ;
	return p;
}

//位置类对象的镜向操作
Position Position::Mirror(const Position& pos1, const Position& pos2)
{
	Position	p1(pos1), p2(pos2);
	double	length = p1.Distance(p2);
	if(length <= DISTANCE_ZERO)
		return *this;
	
	double angle = GetAngleToXAxis(pos1, pos2);
	double cos_v = cos(angle); // cos value
	double sin_v = sin(angle);
	
	double t1 = 2. * cos_v * sin_v;
	double t2 = cos_v * cos_v - sin_v * sin_v;
	//已知点和对称点的中点为(x+desX)/2,(y+desY)/2即在镜像线上也在过已知点的垂线上
	//列二次方程，求出如下两点！
	double desX = x*t2 + y*t1 + pos1.x*(-t2) - pos1.y*t1 + pos1.x;
	double dexY = x*t1 + y*(-t2) + pos1.y*t2 - pos1.x*t1 + pos1.y;
	
	Position des_pos(desX, dexY);
	return des_pos;
}

/*
 *	点到直线的距离
 */
double Position::PotDisLine(Position pos1,Position pos2)
{
	//直线方程一般式Ax+By+c=0的系数
	double A,B,C;
	//根据两点求直线方程和系数，
	//斜率k=y1-y2/x1-x2,y=kx+b,再把一点代入能得到直线方程，化简后再推出一般式
	//则一般式中的A = y2 -y1; B = x1 - x2; C = x2*y1 - x1*y2;
	A = pos2.y - pos1.y;
	B = pos1.x - pos2.x;
	C = pos2.x*pos1.y - pos1.x*pos2.y;
	//求点到线的距离（AXo＋BYo＋C）的绝对值除以根号下（A的平方加上B的平方)
	double dis = fabs(A*x+B*y+C)/sqrt((A*A)+(B*B));
	return dis;
	
}


/*
 *	全局函数：根据圆上的三点求圆
 *	就是解三元二次方程组！
 */
BOOL Pos3OutRound(Position pos1,Position pos2,Position pos3,Position& posCenter,double& Radius)
{
	double a=2*(pos2.x-pos1.x);
	double b=2*(pos2.y-pos1.y);
	double c=pos2.x*pos2.x+pos2.y*pos2.y-pos1.x*pos1.x-pos1.y*pos1.y;
	double d=2*(pos3.x-pos2.x);
	double e=2*(pos3.y-pos2.y);
	double f=pos3.x*pos3.x+pos3.y*pos3.y-pos2.x*pos2.x-pos2.y*pos2.y;
	if(fabs(b*d-e*a) < DOUBLE_EQUALEST)
		return FALSE;
	posCenter.x=(b*f-e*c)/(b*d-e*a);
	posCenter.y=(d*c-a*f)/(b*d-e*a);
	Radius=sqrt((posCenter.x-pos1.x)*(posCenter.x-pos1.x)+(posCenter.y-pos1.y)*(posCenter.y-pos1.y));
	return TRUE;
}

/*

*********判断点在向量的左右

 *平面上的三点P1(x1,y1),P2(x2,y2),P3(x3,y3)的面积量：
 * S(P1,P2,P3)=|y1 y2 y3|= (x1-x3)*(y2-y3)-(y1-y3)*(x2-x3) 
 *当P1P2P3逆时针时S为正的，当P1P2P3顺时针时S为负的。 
 *令矢量的起点为A，终点为B，判断的点为C， 
 *如果S（A，B，C）为正数，则C在矢量AB的左侧； 
 *如果S（A，B，C）为负数，则C在矢量AB的右侧； 
 *如果S（A，B，C）为0，则C在直线AB上。	
 */
int Position::AnalysisClockDir(Position pos1,Position pos2)
{
	double size = (pos1.x - x)*(pos2.y - y) - (pos1.y - y)*(pos2.x - x);
	if (size > 0.0)
		return AD_COUNTERCLOCKWISE;
	else if (size<0.0)
		return AD_CLOCKWISE;
	return 0;
}

/*
 *	求两条线的交点
 */
BOOL GetIntersectPoint(Position pos1,Position pos2,Position pos3,Position pos4,Position &pos)
{
	//两条线的斜率和截距
	double k1,k2,b1,b2;
	double xVal1 = (pos1.x - pos2.x);
	double xVal2 = (pos3.x - pos4.x);
	//如果有一个线与X轴没有交点
	if (xVal1 == 0&&xVal2 != 0)
	{
		//求斜率
		k2 = (pos3.y - pos4.y)/xVal2;
		//求截距
		b2 = pos3.y - k2*pos3.x;
		//求交点
		pos.x = pos1.x;
		pos.y = k2*pos1.x+b2;
		return TRUE;
		
	}
	//如果有一个线与X轴没有交点
	else if (xVal2 == 0&&xVal1 != 0)
	{
		//求斜率
		k1 = (pos1.y - pos2.y)/xVal1;
		//求截距
		b1 = pos1.y - k1*pos1.x;
		//求交点
		pos.x = pos3.x;
		pos.y = k1*pos3.x+b1;
		return TRUE;
	}
	else if (xVal2 == 0&&xVal1 == 0)
		return FALSE;
	//求斜率
	k1 = (pos1.y - pos2.y)/xVal1;
	k2 = (pos3.y - pos4.y)/xVal2;
	//如果斜率一样没有交点
	if (fabs(k1-k2)<DISTANCE_ZERO)
		return FALSE;
	//求截距
	b1 = pos1.y - k1*pos1.x;
	b2 = pos3.y - k2*pos3.x;
	//求交点
	pos.x = (b2-b1)/(k1-k2);
	pos.y = k1*pos.x+b1;
	return TRUE;
}

/*
 *	根据圆上两点和切线求圆，切割线定理
 */
BOOL Pos2LineOutRound(Position posRound1,Position posRound2,Position posLine1,Position posLine2,Position& posCenter1,double& Radius1,Position& posCenter2,double& Radius2)
{
	//切点
	Position posLine;
	//求两条线的交点
	if(GetIntersectPoint(posRound1,posRound2,posLine1,posLine2,posCenter1))
	{
		double l_dDis1 = posCenter1.Distance(posRound1);
		double l_dDis2 = posCenter1.Distance(posRound2);	
		//如果交点不在弦的线段上
		if(fabs(l_dDis1+l_dDis2-posRound1.Distance(posRound2)) > DOUBLE_EQUAL)
		{
			double k = (posLine2.y - posLine1.y)/(posLine2.x - posLine1.x);
			double l = l_dDis1*l_dDis2;
			double x1,x2,y1,y2;
			if((posLine2.x - posLine1.x)<DOUBLE_EQUAL)
			{
				x1 = x2 = posCenter1.x ;
				y1 = posCenter1.y - sqrt(l);
				y2 = posCenter1.y + sqrt(l);
				Position posss1(x1,y1),posss2(x2,y2);
				Pos3OutRound(posRound1,posss1,posRound2,posCenter1,Radius1);
				Pos3OutRound(posRound1,posss2,posRound2,posCenter2,Radius2);
				return TRUE;
			}
			double b = posLine2.y - k*posLine2.x;
			double Eqa = 1+k*k;
			double Eqb = -2*posCenter1.x -2*(posCenter1.y-b)*k;
			double Eqc = posCenter1.x*posCenter1.x+(posCenter1.y-b)*(posCenter1.y-b)-l;
			SolveEquation(Eqa,Eqb,Eqc,x1,x2);
			y1 = k*x1+b;
			y2 = k*x2+b;
			Position pos1(x1,y1),pos2(x2,y2);
			Pos3OutRound(posRound1,pos1,posRound2,posCenter1,Radius1);
			Pos3OutRound(posRound1,pos2,posRound2,posCenter2,Radius2);
		}
		else
			return FALSE;
	}
				
	return TRUE;
}

/*
 *	解一元二次方程
 */
BOOL SolveEquation(double a,double b,double c,double& x1,double& x2)
{
	double d=b*b-4*a*c;
	if(d>0)
	{
		x1=(-b+sqrt(d))/(2*a);
		x2=(-b-sqrt(d))/(2*a);
		return TRUE;
	}
	else if(d==0)
	{
		x1=x2=(-b)/(2*a);
		return TRUE;
	}
	else
	{
		x1=(-b)/(2*a);
		x2=sqrt(-d)/(2*a);
	}
	return FALSE;			
}

/*
 *	点是否在线段上
 */
BOOL PosOnLine(Position pos1,Position pos2,Position pos,double dPrecition)
{
	double l_dDis1 = pos.Distance(pos1);
	double l_dDis2 = pos.Distance(pos2);
	double l_dDis3 = pos2.Distance(pos1);
	if (fabs(l_dDis3-l_dDis1-l_dDis2) <= dPrecition)
		return TRUE;
	return FALSE;
}

/*
 *	求直线与圆的交点
 */
BOOL GetIntersectLineRound(Position& posLine1,Position& posLine2,Position posArcCenter,double dRadius)
{
	double l_dValXSub = posLine1.x- posLine2.x;
	double i = posArcCenter.x,j = posArcCenter.y;
	//两个交点的值
	double x1,x2,y1,y2;
	//一元二次方程的a,b,c
	double AA,BB,CC;
	//求直线与圆的两个交点
	if (fabs(l_dValXSub) <= DOUBLE_EQUAL)//说明没有斜率，与Y轴平行
	{
		AA = 1;
		BB = -2*j;
		CC = j*j -dRadius*dRadius + (posLine1.x-i)*(posLine1.x-i);
		if(!SolveEquation(AA,BB,CC,y1,y2))
			return FALSE;
		x1 = x2 = posLine1.x;	
	}
	else//正常
	{
		double k = (posLine1.y - posLine2.y)/l_dValXSub;
		double b = posLine1.y - k*posLine1.x;
		AA = 1+k*k;
		BB = 2*k*(b-j)-2*i;
		CC = i*i+(b-j)*(b-j)-dRadius*dRadius;
		//如果与圆没有交点
		if(!SolveEquation(AA,BB,CC,x1,x2))
			return FALSE;
		y1 = k*x1 +b;
		y2 = k*x2 +b;
	}
	posLine1.x = x1;
	posLine1.y = y1;
	posLine2.x = x2;
	posLine2.y = y2;
	if(posLine1 == posLine2)
		return 1;
	else
		return 2;
	return 2;
}

/*
 *点是否在圆弧上
 */
BOOL PosOnArc(Position pos,Position posBegin,Position posEnd,Position posCenter,double dRadius,int iDir)
{
	//计算圆弧的起始角
	double angle = GetAngle(posCenter, pos);
	double angle1 = GetAngle(posCenter, posBegin) ;
	//计算圆弧的终止角
	double angle2 = GetAngle(posCenter, posEnd) ;
	//看点到圆心的距离是否等于半径，若不等于说明不在圆弧上
	if(fabs(pos.Distance(posCenter) - dRadius) > 0.0001)
		return FALSE;

	if (iDir == AD_CLOCKWISE)//如果是顺时针
	{
		if (angle1<angle2)//过零度线时，就是X轴正方向
		{
			if (angle<=angle1||angle>=angle2)
				return TRUE;
			else
				return FALSE;
		}
		if (angle<angle1&&angle>angle2)
			return TRUE;
		return FALSE;
	}
	else//逆时针
	{
		if (angle1>angle2)//过零度线时，就是X轴正方向
		{
			if (angle<=angle2||angle>=angle1)
				return TRUE;
			else
				return FALSE;
		}
		if (angle<angle2&&angle>angle1)
			return TRUE;
		return FALSE;		
	}
	return FALSE;
}

//求直线与圆弧的交点
int GetIntersectLineArc(Position& posLine1,Position& posLine2,Position posBegin,
						 Position posEnd,Position posCenter,double dRadius,int iDir)
{
	int l_iNumInter = 0;
	if(GetIntersectLineRound(posLine1,posLine2,posCenter,dRadius))//与圆的交点
	{
		if(PosOnArc(posLine1,posBegin,posEnd,posCenter,dRadius,iDir))//点是否在弧上
			l_iNumInter++;
		else
			posLine1 = posLine2;
		if(PosOnArc(posLine2,posBegin,posEnd,posCenter,dRadius,iDir))//点是否在弧上
			l_iNumInter++;
		else
			posLine2 = posLine1;
		
	}
	return l_iNumInter;
}

/*
 *	圆与圆的交点
  (下面分析中x^y表示x的y次方)
  大家还记得圆的参数方程吧, 圆心 (x0, y0), 半径为 r 的圆的参数方程是:
  x = r * cosθ + x0
  y = r * sinθ + y0
  假设现在两圆参数x1, y1, r1, x2, y2, r2(这些分别表示, 咳, 有谁看不出来它们分别表示什么吗?), 设交点为 (x, y), 代入其中一个圆中的参数方程有
  x = r1 * cosθ + x1 且 y = r1 * sinθ + y1
  代入另一圆的标准方程, 得到
  (r1 * cosθ + x1 - x2)^2 + (r1 * sinθ + y1 - y2)^2 = r2^2
  是的, 看起来有关于正余弦二次项, 不过不要惊慌, 展开合并同类项之后, 正好这两项会合并成常数:
  左边 = (r1 * cosθ)^2 + (r1 * sinθ)^2 + 2 * r1 * (x1 - x2) * cosθ + 2 * r1 * (y1 - y2) * sinθ
  = r2^2 - (x1 - x2)^2 - (y1 - y2)^2 = 右边
  这样就好办了, 把 r1^2 转移到等式右边, 令:
  a = 2 * r1 * (x1 - x2)
  b = 2 * r1 * (y1 - y2)
  c = r2^2 - r1^2 - (x1 - x2)^2 - (y1 - y2)^2
  那么方程便成为:
  a * cosθ + b * sinθ = c
  用 (1 - (cosθ)^2)^(1 / 2) 表示sinθ, 令：
  p = a^2 + b^2
  q = -2 * a * c
  r = c^2 - b^2
  便化为一个一元二次方程, 解得:
  cosθ = (±(q^2 - 4 * p * r)^(1 / 2) - q) / (2 * p)
 */
int GetIntersectTwoRound(Position posArcCenter1,double dRadius1,Position posArcCenter2,double dRadius2
						 ,Position& posInter1,Position& posInter2)
{
    double d, a, b, c, p, q, r;  
    double cos_value[2], sin_value[2];  
	//如果两圆相等
    if (posArcCenter1 == posArcCenter2&& fabs(dRadius1 -dRadius2)<DOUBLE_EQUAL)
        return -1; 
	
    d = posArcCenter1.Distance(posArcCenter2);  
    if (d > dRadius1 +dRadius2|| d < fabs(dRadius1 - dRadius2)) //通过圆心距离看有是否有交点,没有交点
	{  
        return 0;  
    }  
	
    a = 2.0 * dRadius1 * (posArcCenter1.x - posArcCenter2.x);  
    b = 2.0 * dRadius1 * (posArcCenter1.y - posArcCenter2.y);  
    c = dRadius2*dRadius2 - dRadius1*dRadius1 - posArcCenter1.DistanceSqr(posArcCenter2);  
    p = a * a + b * b;  
    q = -2.0 * a * c;  
    if (fabs(d-dRadius1-dRadius2) <DOUBLE_EQUAL || fabs(d-fabs(dRadius1-dRadius2))<DOUBLE_EQUAL) 
	{  //只有一个交点
        cos_value[0] = -q / p / 2.0;  
        sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);  
		
        posInter1.x = dRadius1 * cos_value[0] + posArcCenter1.x;  
        posInter1.y = dRadius1 * sin_value[0] + posArcCenter1.y;  
		
        if (fabs(posInter1.DistanceSqr(posArcCenter2)-dRadius2*dRadius2)>DOUBLE_EQUAL)
		{  
            posInter1.y = posArcCenter1.y - dRadius1 * sin_value[0];  
        }  
        return 1;  
    }  
	
    r = c * c - b * b;  
    cos_value[0] = (sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;  
    cos_value[1] = (-sqrt(q * q - 4.0 * p * r) - q) / p / 2.0;  
    sin_value[0] = sqrt(1 - cos_value[0] * cos_value[0]);  
    sin_value[1] = sqrt(1 - cos_value[1] * cos_value[1]);  
	
    posInter1.x = dRadius1 * cos_value[0] + posArcCenter1.x;  
    posInter2.x = dRadius1 * cos_value[1] + posArcCenter1.x;  
    posInter1.y = dRadius1 * sin_value[0] + posArcCenter1.y;  
    posInter2.y = dRadius1 * sin_value[1] + posArcCenter1.y;  
	
    if (!DoubleEquals(posInter1.DistanceSqr(posArcCenter2),dRadius2*dRadius2))
        posInter1.y = posArcCenter1.y - dRadius1 * sin_value[0];   
    if (!DoubleEquals(posInter2.DistanceSqr(posArcCenter2),dRadius2*dRadius2)) 
        posInter2.y = posArcCenter1.y - dRadius1 * sin_value[1];   
    if (posInter1 == posInter2)
	{  
        if (posInter1.y > 0) 
            posInter2.y = -posInter2.y;  
        else
            posInter1.y = -posInter1.y;
    }  
    return 2;  //有两个交点	
}

//两个double型是否相等
BOOL DoubleEquals(double dDouble1,double dDouble2)
{
	//return fabs(dDouble1-dDouble2) <DOUBLE_EQUAL;
	if(fabs(dDouble1-dDouble2) <DOUBLE_EQUAL)
		return TRUE;
	return FALSE;
}

//圆与弧的交点
int GetIntersectArcRound(Position posRoundCenter,double dRoundRadius,Position posBegin,Position posEnd,
			 Position posArcCenter,double dArcRadius,int iDir,Position& posInter1,Position& posInter2)
{
	//交点个数
	int l_iNumInter = GetIntersectTwoRound(posRoundCenter,dRoundRadius,posArcCenter,dArcRadius,posInter1,posInter2);
	if(l_iNumInter == -1)	//如果两圆相等		
		return -1;
	if(l_iNumInter == 0)//没有交点
		return 0;
	else if(l_iNumInter == 1)//只有一个交点
	{
		if(PosOnArc(posInter1,posBegin,posEnd,posArcCenter,dArcRadius,iDir))
			return 1;
		else
			return 0;
	}
	else if(l_iNumInter == 2)//有两个交点
	{
		int l_iNumInterArc = 0;
		if(PosOnArc(posInter1,posBegin,posEnd,posArcCenter,dArcRadius,iDir))
			l_iNumInterArc++;
		if(PosOnArc(posInter2,posBegin,posEnd,posArcCenter,dArcRadius,iDir))
		{
			if(l_iNumInterArc == 0)
				posInter1 = posInter2;
			l_iNumInterArc++;
		}
		return l_iNumInterArc;
	}
	return -1;
}

//弧与弧的交点
int GetIntersectTwoArc(Position posBegin1,Position posEnd1,Position posCenter1,double dRadius1,int iDir1
			,Position posBegin2,Position posEnd2,Position posCenter2,double dRadius2,int iDir2
			,Position& posInter1,Position& posInter2)
{
	//交点个数
	int l_iNumInter = GetIntersectTwoRound(posCenter1,dRadius1,posCenter2,dRadius2,posInter1,posInter2);
	if(l_iNumInter == -1)	//如果两圆相等		
		return -1;
	if(l_iNumInter == 0)//没有交点
		return 0;
	else if(l_iNumInter == 1)//只有一个交点
	{
		if(PosOnArc(posInter1,posBegin1,posEnd1,posCenter1,dRadius1,iDir1)
			&&PosOnArc(posInter1,posBegin2,posEnd2,posCenter2,dRadius2,iDir2))
			return 1;
		else
			return 0;
	}
	else if(l_iNumInter == 2)//有两个交点
	{
		int l_iNumInterArc = 0;
		if(PosOnArc(posInter1,posBegin1,posEnd1,posCenter1,dRadius1,iDir1)
			&&PosOnArc(posInter1,posBegin2,posEnd2,posCenter2,dRadius2,iDir2))
			l_iNumInterArc++;
		if(PosOnArc(posInter2,posBegin1,posEnd1,posCenter1,dRadius1,iDir1)
			&&PosOnArc(posInter2,posBegin2,posEnd2,posCenter2,dRadius2,iDir2))
		{
			if(l_iNumInterArc == 0)
				posInter1 = posInter2;
			l_iNumInterArc++;			
		}
		return l_iNumInterArc;
	}
	return -1;
}




///////////////////////////////////////////////////