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
 *	����֮��ľ����ƽ��
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


// ȫ�ֺ�������������posSrc1 �� posSrc2������x��ļн�ͬ����������Ƕ�0-2PI���
double GetAngleToXAxis(const Position& posSrc1,const Position& posSrc2)
{
	Position pos1(posSrc1),pos2(posSrc2);	//��������
	double len = pos1.Distance(pos2);		//����������
	if(len < DISTANCE_ZERO)		//�������㾫�ȣ��ͷ���
		return	0 ;
	Position dp = pos2 - pos1;	//������
	double cosv = dp.x/len ;	//�ڱ߱�б��
	double sinv = dp.y/len ;	//�Ա߱�б��
	if(sinv >= 0)//һ������0~180
		return acos(cosv) ;//������
	else if(sinv < 0)//��������180~360
		return MATH_2PI-acos(cosv) ;//
	return 0 ;
}


// ȫ�ֺ���:�㵽ֱ�ߵĴ����-----ǰ�������ǹ�ֱ�ߵģ���һ�����ǵ�ֱ�߾�����Ǹ���
Position Position::DistancePosFoot(const Position& PosLine1,const Position& PosLine2)
{
	//ֱ�߷���һ��ʽAx+By+c=0��ϵ��
	double A,B,C;
	//����������ֱ�߷��̺�ϵ����
	//б��k=y1-y2/x1-x2,y=kx+b,�ٰ�һ������ܵõ�ֱ�߷��̣���������Ƴ�һ��ʽ
	//��һ��ʽ�е�A = y2 -y1; B = x1 - x2; C = x2*y1 - x1*y2;
	A = PosLine2.y - PosLine1.y;
	B = PosLine1.x - PosLine2.x;
	C = PosLine2.x*PosLine1.y - PosLine1.x*PosLine2.y;
	//���㣺�费������(x0,y0),����������̣�Ax + By + C = 0;(y - y0) / (x - x0) = B / A(б���෴);
	//��ã�x=(B*B*x0-A*B*y0-A*C)/(A*A + B*B);     y=(-A*B*x0+A*A*y0-B*C)/(A*A+B*B);
	Position PosFoot;
	PosFoot.x = (B*B*x-A*B*y-A*C)/(A*A + B*B);
	PosFoot.y = (-A*B*x+A*A*y-B*C)/(A*A+B*B);
	return PosFoot;
}

// ȫ�ֺ�������������Ƕ�0-360���
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
 *	ȫ�ֺ���������������ļн�0~180
 *  �������Ҷ��� 
 *  cosA = (c^2 + b^2 - a^2) / (2��b��c) 
 */
double GetAngleThreePos(Position dStartPoint,Position dMiddlePoint,Position dEndPoint)
{
	double a = dStartPoint.Distance(dEndPoint);
	double b = dMiddlePoint.Distance(dEndPoint);
	double c = dMiddlePoint.Distance(dStartPoint);
	if(b==0||c==0)//˵����һ������
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
 *	 ȫ�ֺ������ж������Ƿ����
 */
BOOL PosEqual(const Position& pos1, const Position& pos2)
{
	if(DISTANCE_ZERO>fabs(pos1.x-pos2.x)&&DISTANCE_ZERO>fabs(pos1.y-pos2.y))
		return TRUE;
	return FALSE;
}

//ȫ�ֺ��������Ƿ��ڰ�Χ����
BOOL PosBelongBox(Position begin,Position end,Position pos)
{
	BOX2D pBox;		//��Χ�нṹ
	double curRadius = 2.5 * g_pView->ReturnProportion();
	//�õ���Χ�е������㣬�������꣨���£����ϣ�//2.5�ǰ����õİ�Χ�зŴ��Է���ֱ����
	pBox.min[0] = min( begin.x, end.x )-curRadius;
	pBox.min[1] = min( begin.y, end.y )-curRadius;
	pBox.max[0] = max( begin.x, end.x )+curRadius;
	pBox.max[1] = max( begin.y, end.y )+curRadius;
	if(pos.x>pBox.min[0]&&pos.x<pBox.max[0]&&pos.y>pBox.min[1]&&pos.y<pBox.max[1])
		return TRUE;
	return FALSE;
}


/*
 *	���Ƿ��ڰ�Χ����
 */
BOOL IsBelongBox(Position pos,BOX2D pBox)
{
	if (pos.x < pBox.max[0]&&pos.x > pBox.min[0]&&pos.y < pBox.max[1]&&pos.y > pBox.min[1])
		return TRUE;
	return FALSE;
}

/*
 *	 ȫ�ֺ���������ʾ���������
 */
void RemindContent(CString Content,int Same)
{	
	CString str;
	((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.GetDlgItemText(IDC_EDIT_COMMAND,str);
	if (Same == SEND_ALL)//��������
	{
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_REMIND,Content);
		Content+=str;//�����������ַ�
		g_pDoc->m_strCmdAll+=Content;
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_RICHEDIT4,g_pDoc->m_strCmdAll);
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.m_RichEdit_Cmd.SendMessage(WM_VSCROLL,SB_BOTTOM ,0);
	}
	if(Same == SEND_EDIT)//ֻ��EDIT���﷢
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_EDIT_REMIND,Content);
	if (Same == SEND_RICHEDIT)//ֻ��RICHEDIT���﷢
	{
		Content+=str;//�����������ַ�
		g_pDoc->m_strCmdAll+=Content;
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.SetDlgItemText(IDC_RICHEDIT4,g_pDoc->m_strCmdAll);
		((CMainFrame*)AfxGetMainWnd())->m_DlgCommandBar.m_RichEdit_Cmd.SendMessage(WM_VSCROLL,SB_BOTTOM ,0);
	}
}

/*
 *	ȫ�ֺ���:����͹�ȷ��ط���
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

//	ȫ�ֺ���:ֻ����Ч����
double RetainValid(double digits,int valid)
{
	int num1 = (int)(digits*pow(10,valid));
	double num2 = num1/pow(10,valid);
	return num2;
}

//	ȫ�ֺ���:��Position�ṹת��POSITION
void PosToPOS(Position& Pos,DOUBLEPOINT POS)
{
	Pos.x = POS.x;
	Pos.y = POS.y;
}
//λ��������ƽ�Ʋ���
Position Position::Offset(const double & deltax,const double & deltay)
{
	Position	pnt(x + deltax, y + deltay);
	return pnt;	
}
//λ��������ƽ�Ʋ���
Position Position::Offset(const Position& off)
{
	Position	pnt(x + off.x, y + off.y);
	return pnt;
}

//λ����������ת����					
Position Position::Rotate(const Position& basePos, const double angle)
{
	Position p;
	double cosv = cos(angle) ; // cos value
	double sinv = sin(angle) ;
	double xc = x ;
	double yc = y ;
	//������ת��������
	//������ת��ʽ:��xcosa-ysina��xsina+ycosa��
	//��XΪ��x=(xc-basePos.x)* cosv - (yc-basePos.y)*sinv +basePos.x�ֽ�����£�
	p.x = xc * cosv - yc * sinv + (1.- cosv) * basePos.x + basePos.y * sinv ;
	//��YΪ��y=(xc-basePos.x)* sinv + (yc-basePos.y)*cosv +basePos.y�ֽ�����£�
	p.y = sinv * xc + cosv * yc + (1.- cosv) * basePos.y - sinv * basePos.x ;
	return p;
}

//λ�������ľ������
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
	//��֪��ͶԳƵ���е�Ϊ(x+desX)/2,(y+desY)/2���ھ�������Ҳ�ڹ���֪��Ĵ�����
	//�ж��η��̣�����������㣡
	double desX = x*t2 + y*t1 + pos1.x*(-t2) - pos1.y*t1 + pos1.x;
	double dexY = x*t1 + y*(-t2) + pos1.y*t2 - pos1.x*t1 + pos1.y;
	
	Position des_pos(desX, dexY);
	return des_pos;
}

/*
 *	�㵽ֱ�ߵľ���
 */
double Position::PotDisLine(Position pos1,Position pos2)
{
	//ֱ�߷���һ��ʽAx+By+c=0��ϵ��
	double A,B,C;
	//����������ֱ�߷��̺�ϵ����
	//б��k=y1-y2/x1-x2,y=kx+b,�ٰ�һ������ܵõ�ֱ�߷��̣���������Ƴ�һ��ʽ
	//��һ��ʽ�е�A = y2 -y1; B = x1 - x2; C = x2*y1 - x1*y2;
	A = pos2.y - pos1.y;
	B = pos1.x - pos2.x;
	C = pos2.x*pos1.y - pos1.x*pos2.y;
	//��㵽�ߵľ��루AXo��BYo��C���ľ���ֵ���Ը����£�A��ƽ������B��ƽ��)
	double dis = fabs(A*x+B*y+C)/sqrt((A*A)+(B*B));
	return dis;
	
}


/*
 *	ȫ�ֺ���������Բ�ϵ�������Բ
 *	���ǽ���Ԫ���η����飡
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

*********�жϵ�������������

 *ƽ���ϵ�����P1(x1,y1),P2(x2,y2),P3(x3,y3)���������
 * S(P1,P2,P3)=|y1 y2 y3|= (x1-x3)*(y2-y3)-(y1-y3)*(x2-x3) 
 *��P1P2P3��ʱ��ʱSΪ���ģ���P1P2P3˳ʱ��ʱSΪ���ġ� 
 *��ʸ�������ΪA���յ�ΪB���жϵĵ�ΪC�� 
 *���S��A��B��C��Ϊ��������C��ʸ��AB����ࣻ 
 *���S��A��B��C��Ϊ��������C��ʸ��AB���Ҳࣻ 
 *���S��A��B��C��Ϊ0����C��ֱ��AB�ϡ�	
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
 *	�������ߵĽ���
 */
BOOL GetIntersectPoint(Position pos1,Position pos2,Position pos3,Position pos4,Position &pos)
{
	//�����ߵ�б�ʺͽؾ�
	double k1,k2,b1,b2;
	double xVal1 = (pos1.x - pos2.x);
	double xVal2 = (pos3.x - pos4.x);
	//�����һ������X��û�н���
	if (xVal1 == 0&&xVal2 != 0)
	{
		//��б��
		k2 = (pos3.y - pos4.y)/xVal2;
		//��ؾ�
		b2 = pos3.y - k2*pos3.x;
		//�󽻵�
		pos.x = pos1.x;
		pos.y = k2*pos1.x+b2;
		return TRUE;
		
	}
	//�����һ������X��û�н���
	else if (xVal2 == 0&&xVal1 != 0)
	{
		//��б��
		k1 = (pos1.y - pos2.y)/xVal1;
		//��ؾ�
		b1 = pos1.y - k1*pos1.x;
		//�󽻵�
		pos.x = pos3.x;
		pos.y = k1*pos3.x+b1;
		return TRUE;
	}
	else if (xVal2 == 0&&xVal1 == 0)
		return FALSE;
	//��б��
	k1 = (pos1.y - pos2.y)/xVal1;
	k2 = (pos3.y - pos4.y)/xVal2;
	//���б��һ��û�н���
	if (fabs(k1-k2)<DISTANCE_ZERO)
		return FALSE;
	//��ؾ�
	b1 = pos1.y - k1*pos1.x;
	b2 = pos3.y - k2*pos3.x;
	//�󽻵�
	pos.x = (b2-b1)/(k1-k2);
	pos.y = k1*pos.x+b1;
	return TRUE;
}

/*
 *	����Բ�������������Բ���и��߶���
 */
BOOL Pos2LineOutRound(Position posRound1,Position posRound2,Position posLine1,Position posLine2,Position& posCenter1,double& Radius1,Position& posCenter2,double& Radius2)
{
	//�е�
	Position posLine;
	//�������ߵĽ���
	if(GetIntersectPoint(posRound1,posRound2,posLine1,posLine2,posCenter1))
	{
		double l_dDis1 = posCenter1.Distance(posRound1);
		double l_dDis2 = posCenter1.Distance(posRound2);	
		//������㲻���ҵ��߶���
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
 *	��һԪ���η���
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
 *	���Ƿ����߶���
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
 *	��ֱ����Բ�Ľ���
 */
BOOL GetIntersectLineRound(Position& posLine1,Position& posLine2,Position posArcCenter,double dRadius)
{
	double l_dValXSub = posLine1.x- posLine2.x;
	double i = posArcCenter.x,j = posArcCenter.y;
	//���������ֵ
	double x1,x2,y1,y2;
	//һԪ���η��̵�a,b,c
	double AA,BB,CC;
	//��ֱ����Բ����������
	if (fabs(l_dValXSub) <= DOUBLE_EQUAL)//˵��û��б�ʣ���Y��ƽ��
	{
		AA = 1;
		BB = -2*j;
		CC = j*j -dRadius*dRadius + (posLine1.x-i)*(posLine1.x-i);
		if(!SolveEquation(AA,BB,CC,y1,y2))
			return FALSE;
		x1 = x2 = posLine1.x;	
	}
	else//����
	{
		double k = (posLine1.y - posLine2.y)/l_dValXSub;
		double b = posLine1.y - k*posLine1.x;
		AA = 1+k*k;
		BB = 2*k*(b-j)-2*i;
		CC = i*i+(b-j)*(b-j)-dRadius*dRadius;
		//�����Բû�н���
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
 *���Ƿ���Բ����
 */
BOOL PosOnArc(Position pos,Position posBegin,Position posEnd,Position posCenter,double dRadius,int iDir)
{
	//����Բ������ʼ��
	double angle = GetAngle(posCenter, pos);
	double angle1 = GetAngle(posCenter, posBegin) ;
	//����Բ������ֹ��
	double angle2 = GetAngle(posCenter, posEnd) ;
	//���㵽Բ�ĵľ����Ƿ���ڰ뾶����������˵������Բ����
	if(fabs(pos.Distance(posCenter) - dRadius) > 0.0001)
		return FALSE;

	if (iDir == AD_CLOCKWISE)//�����˳ʱ��
	{
		if (angle1<angle2)//�������ʱ������X��������
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
	else//��ʱ��
	{
		if (angle1>angle2)//�������ʱ������X��������
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

//��ֱ����Բ���Ľ���
int GetIntersectLineArc(Position& posLine1,Position& posLine2,Position posBegin,
						 Position posEnd,Position posCenter,double dRadius,int iDir)
{
	int l_iNumInter = 0;
	if(GetIntersectLineRound(posLine1,posLine2,posCenter,dRadius))//��Բ�Ľ���
	{
		if(PosOnArc(posLine1,posBegin,posEnd,posCenter,dRadius,iDir))//���Ƿ��ڻ���
			l_iNumInter++;
		else
			posLine1 = posLine2;
		if(PosOnArc(posLine2,posBegin,posEnd,posCenter,dRadius,iDir))//���Ƿ��ڻ���
			l_iNumInter++;
		else
			posLine2 = posLine1;
		
	}
	return l_iNumInter;
}

/*
 *	Բ��Բ�Ľ���
  (���������x^y��ʾx��y�η�)
  ��һ��ǵ�Բ�Ĳ������̰�, Բ�� (x0, y0), �뾶Ϊ r ��Բ�Ĳ���������:
  x = r * cos�� + x0
  y = r * sin�� + y0
  ����������Բ����x1, y1, r1, x2, y2, r2(��Щ�ֱ��ʾ, ��, ��˭�����������Ƿֱ��ʾʲô��?), �轻��Ϊ (x, y), ��������һ��Բ�еĲ���������
  x = r1 * cos�� + x1 �� y = r1 * sin�� + y1
  ������һԲ�ı�׼����, �õ�
  (r1 * cos�� + x1 - x2)^2 + (r1 * sin�� + y1 - y2)^2 = r2^2
  �ǵ�, �������й��������Ҷ�����, ������Ҫ����, չ���ϲ�ͬ����֮��, �����������ϲ��ɳ���:
  ��� = (r1 * cos��)^2 + (r1 * sin��)^2 + 2 * r1 * (x1 - x2) * cos�� + 2 * r1 * (y1 - y2) * sin��
  = r2^2 - (x1 - x2)^2 - (y1 - y2)^2 = �ұ�
  �����ͺð���, �� r1^2 ת�Ƶ���ʽ�ұ�, ��:
  a = 2 * r1 * (x1 - x2)
  b = 2 * r1 * (y1 - y2)
  c = r2^2 - r1^2 - (x1 - x2)^2 - (y1 - y2)^2
  ��ô���̱��Ϊ:
  a * cos�� + b * sin�� = c
  �� (1 - (cos��)^2)^(1 / 2) ��ʾsin��, �
  p = a^2 + b^2
  q = -2 * a * c
  r = c^2 - b^2
  �㻯Ϊһ��һԪ���η���, ���:
  cos�� = (��(q^2 - 4 * p * r)^(1 / 2) - q) / (2 * p)
 */
int GetIntersectTwoRound(Position posArcCenter1,double dRadius1,Position posArcCenter2,double dRadius2
						 ,Position& posInter1,Position& posInter2)
{
    double d, a, b, c, p, q, r;  
    double cos_value[2], sin_value[2];  
	//�����Բ���
    if (posArcCenter1 == posArcCenter2&& fabs(dRadius1 -dRadius2)<DOUBLE_EQUAL)
        return -1; 
	
    d = posArcCenter1.Distance(posArcCenter2);  
    if (d > dRadius1 +dRadius2|| d < fabs(dRadius1 - dRadius2)) //ͨ��Բ�ľ��뿴���Ƿ��н���,û�н���
	{  
        return 0;  
    }  
	
    a = 2.0 * dRadius1 * (posArcCenter1.x - posArcCenter2.x);  
    b = 2.0 * dRadius1 * (posArcCenter1.y - posArcCenter2.y);  
    c = dRadius2*dRadius2 - dRadius1*dRadius1 - posArcCenter1.DistanceSqr(posArcCenter2);  
    p = a * a + b * b;  
    q = -2.0 * a * c;  
    if (fabs(d-dRadius1-dRadius2) <DOUBLE_EQUAL || fabs(d-fabs(dRadius1-dRadius2))<DOUBLE_EQUAL) 
	{  //ֻ��һ������
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
    return 2;  //����������	
}

//����double���Ƿ����
BOOL DoubleEquals(double dDouble1,double dDouble2)
{
	//return fabs(dDouble1-dDouble2) <DOUBLE_EQUAL;
	if(fabs(dDouble1-dDouble2) <DOUBLE_EQUAL)
		return TRUE;
	return FALSE;
}

//Բ�뻡�Ľ���
int GetIntersectArcRound(Position posRoundCenter,double dRoundRadius,Position posBegin,Position posEnd,
			 Position posArcCenter,double dArcRadius,int iDir,Position& posInter1,Position& posInter2)
{
	//�������
	int l_iNumInter = GetIntersectTwoRound(posRoundCenter,dRoundRadius,posArcCenter,dArcRadius,posInter1,posInter2);
	if(l_iNumInter == -1)	//�����Բ���		
		return -1;
	if(l_iNumInter == 0)//û�н���
		return 0;
	else if(l_iNumInter == 1)//ֻ��һ������
	{
		if(PosOnArc(posInter1,posBegin,posEnd,posArcCenter,dArcRadius,iDir))
			return 1;
		else
			return 0;
	}
	else if(l_iNumInter == 2)//����������
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

//���뻡�Ľ���
int GetIntersectTwoArc(Position posBegin1,Position posEnd1,Position posCenter1,double dRadius1,int iDir1
			,Position posBegin2,Position posEnd2,Position posCenter2,double dRadius2,int iDir2
			,Position& posInter1,Position& posInter2)
{
	//�������
	int l_iNumInter = GetIntersectTwoRound(posCenter1,dRadius1,posCenter2,dRadius2,posInter1,posInter2);
	if(l_iNumInter == -1)	//�����Բ���		
		return -1;
	if(l_iNumInter == 0)//û�н���
		return 0;
	else if(l_iNumInter == 1)//ֻ��һ������
	{
		if(PosOnArc(posInter1,posBegin1,posEnd1,posCenter1,dRadius1,iDir1)
			&&PosOnArc(posInter1,posBegin2,posEnd2,posCenter2,dRadius2,iDir2))
			return 1;
		else
			return 0;
	}
	else if(l_iNumInter == 2)//����������
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