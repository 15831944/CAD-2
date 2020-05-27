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
#include "Undo.h"		//���峷���ṹ��ͷ�ļ�
#include "spline.h"//�����


enum EDrawMode // ��ͼģʽ
{
	dmNormal = 0, // ����״̬
	dmSelect = 1, // ѡ��״̬	
	dmPrompt = 2, // ��ʾ״̬
	dmDrag   = 3, // �϶�״̬
	dmInvalid = 4, // ����״̬
	dmLayer = 5,
	dmDragSelect1 = 6,
	dmDragSelect2 = 7,
	ARC = 1,
	LINE = 2
};



class CCreateBase : public CObject//�������Ķ���Ҳ��COblist�Ķ���
{
	DECLARE_SERIAL(CCreateBase)
public:
	CCreateBase();
	virtual ~CCreateBase();
	// ����ͼԪ����
//ԭ����DGI+ʱ����ͼ�ĺ���	virtual void Draw(CDC *pDC) {};//����ڶ���ʱ������
 
	virtual void DrawMove(CDC *pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);	//����ƶ�ʱ����Ƥ��
	//�������ԶԻ���Ľ��沢��ʾͼԪ����
	virtual void PropertyView() {};
	//�Ƿ�ѡ��
	virtual int IsPick(CPoint point)  {return -1;};
	//ͼԪ���ƶ�	
	virtual void Move(const Position& basePos,const Position& desPos) {};
	//����ͼ���ϵ�һ�����볤�ȷ���һ����
	virtual BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine) {return FALSE;};
	// ����һ�������һ���Ƕ�ֵ��תͼԪ
	virtual void Rotate(const Position& basePos, const double angle) {}	;
	// �������㾵��ͼԪ����������bCopy����ȷ���Ƿ�ɾ��ԭ��λ�õ�ͼԪ
	virtual void Mirror(const Position& pos1, const Position& pos2){};
	//XY���Ź���
	virtual BOOL GraphZoomXY(double dValueX, double dValueY) {return FALSE;};		
	// ָ�뿽����������һ���������޸ĵ��ػ�����Щ������MOUSEMOVE�����
	virtual	CCreateBase* Copy() { return NULL; } 
	//�������ԶԻ��򵯳��ı�ͼԪ����ʱҪ�ػ��ĵĺ���
	virtual void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0) {};
	// �õ��������С��Χ�У��ð�Χ�н�������ͼԪ��ѡȡ��ȫ����ʾ
	virtual BOOL IsSelect(CRgn *selectrect,BOOL bComplex){return FALSE;};
	//�õ���Χ��						�Ƿ������
	virtual BOOL GetBox(BOX2D* pBox,BOOL IsOutside = FALSE){return FALSE;};
	//�ı��и��
	virtual void ChangeDir(){};
	//����������
	virtual void GraphZoomScale(Position posBase,double dScale) {};
	//ʵ�߱�����
	virtual PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist) {return FALSE;};




	//������ɫ���磬��ɫ1����ɫ2.������
	int RtnColorInt();
	//�����ԶԻ����еĿؼ���ʾ����
	void ShowControl(int show,BOOL btn);
	//���ԶԻ�����ı༭��ı�ʱ�ĳ���
	void AddUndoList(double dEditNew,double dEditOld,int iWhichEdit,int iNumOfTop=0);
	
	COLORREF m_ColorOfLine;		//ÿ��ͼԪ����ɫ;
	CArray<CString,CString&> m_strSBM;//ÿ��ͼԪ��Ӧ��M����
	CArray<CString,CString&> m_strCBM;
	int m_iIsBeginGraph;//��־�ʼ�򿪵�ͼ��
	int m_iKnifeStyle;//�������� 41�󵶲� 42�ҵ���
	CArray<Position,Position&> m_aryDash;//������ʱ��ÿ��С�߶�
	double m_dRemainingDash; //������ʱ��ʣ�೤��
	
protected:
	//��ֱ�߻�Բ������
	void DrawArc(CDC* pDC,CPoint potCenter,double dRadius,double dAngleStart,double dAngleOffset);
	//����ͷ�ĺ���
	void DrawArrow(double angle,CPoint PosEnd,CDC* pDC);
	// ���ݻ�ͼģʽ���û�ͼ�����������豸������ģʽ����������
	void SetDrawEnvir(CDC*	pDC, int drawMode, CPen* pPen);
	//ѡ�к��С����
	void DrawRectForPick(CDC* pDC,CPoint Point);
	//��Բ���������
	PGRAPHPOS TurnArcDash(double dLongSolid,double dLongEmpty,Position posCenter,double dAngleStart,
		double dAngeleEnd,double dRadius,int iDirect);	
	//��ֱ�߱������
	PGRAPHPOS TurnLineDash(double dLongSolid,double dLongEmpty,Position posBegin,Position PosEnd);
	
	



private:
	//���µ������Ƿ����ַ������
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
	//�ı��и��
	void ChangeDir();

	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	CCreateBase* Copy();
	
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	void PropertyView();
	//�õ���Χ��
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	//XY���Ź���
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//����������
	void GraphZoomScale(Position posBase,double dScale);
	//����ͼ���ϵ�һ�����볤�ȷ���һ����
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//ʵ�߱�����
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	
private:
	//��ֱ���Ƿ�������н���
	BOOL LineOnRect(Position pos1,Position pos3);
	
};


class CCreatePolyline  :public CCreateBase
{
	DECLARE_SERIAL(CCreatePolyline)
public:
	CCreatePolyline();
	//����Բ����
	CCreatePolyline(CArray<Position,Position&>&  point);
	//��Բ����
	CCreatePolyline(CArray<Position,Position&>&  point,CArray<double,double&>& TuDu);
	virtual ~CCreatePolyline();
	void DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);
	//��굥��ʰȡ
	int IsPick(CPoint point);
	CCreateBase* Copy();

	void Move(const Position& basePos,const Position& desPos);
	void Rotate(const Position& basePos, const double angle);
	void Mirror(const Position& pos1, const Position& pos2);
	//�ı��и��
	void ChangeDir();
	
	
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	void PropertyView();
	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	//�õ���Χ��
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	//XY���Ź���
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//����ͼ���ϵ�һ�����볤�ȷ���һ����
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//����������
	void GraphZoomScale(Position posBase,double dScale);
	//ʵ�߱�����
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	

public:
	CArray<Position,Position&> m_point;//�㼯
	CArray<double,double&> m_dTuDu;//���͹�ȵļ���
	
private:
	//��ͼԪ���п�����ֱ�ߣ�Ҳ�п�����Բ��
	void DrawLineOrArc(CDC *pDC,int i,BOOL FlagShowArrow,BOOL FlagShowNum,Position l_posCurPos,Position l_posNextPos,double TuDu);
	//��ֱ�ߺ�Բ��
	void DrawLineAndArc(CDC *pDC,BOOL FlagShowArrow,Position l_posCurPos,Position l_posNextPos,double TuDu);
	//ֱ�ߵ�ʰȡ����
	BOOL IsPickLine(Position Pos1,Position Pos2,CPoint point);
	//Բ����ʰȡ����
	BOOL IsPickArc(Position Pos1,Position Pos2,CPoint point,double TuDu);
	//��������յ�͹�ȷ��ػ���
	double RtnLongArc(Position posClick,Position posBegin,Position PosEnd,double dTudu);
	//������㣬�յ㣬͹�ȣ�����㣬���������ػ��ϵ�
	Position RtnLongArcPos(Position posBegin,Position PosEnd,double dTudu,Position posClick,double dLongPos);
	//��������Ƿ�������н��㲢�ҽ����Ƿ���ֱ����
	BOOL PolylineOnRect(Position pos1,Position pos3);
	//���Ƿ���Բ����
	BOOL PosOnPolylineArc(Position pos,Position posBegin,Position posEnd,Position posCenter,int iDir);
	//��������Ļ����ʾ����ʼ�Ǻ�ת���Ƕ�
	BOOL GetAngleStartAndOffset(double& dAngleStart,double& dAngleOffset,CPoint& potBegini,int iDir,Position posCenter,Position posBegin,Position posEnd);
	//������ʼ�Ǻ�ת���Ƕ�
	BOOL CaleStartAndOffset(double& dAngleStart,double& dAngleOffset,int iDir,double arcStart,double arcEnd);
	//�õ�Բ���İ�Χ��
	BOOL GetArcBox(BOX2D* pBox,Position posCenter,Position posBegin,Position posEnd,int iDir);

	//��־��һ����Բ������ֱ��1:ֱ��   2��Բ��
	int m_iFlagArcOrLine;	
};


class CCreateRectangle  :public CCreateBase
{
	DECLARE_SERIAL(CCreateRectangle)
public:
	CArray<Position,Position&> m_point;
	CArray<double,double&> m_dTuDu;//���͹�ȵ�
	

public:
	CCreateRectangle();
	//���޸ĵ���Щ����ʹ�ã�Copy()����ʹ�á�
	CCreateRectangle(CArray<Position,Position&>&  point,CArray<double,double&>& TuDu);
	//����ͼ������ͼԪʱʹ�á�
	CCreateRectangle(const Position& begin,const Position& end,BOOL FlagRoundRect = FALSE,double Radius = 0);
	virtual ~CCreateRectangle();


	void DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);
	int IsPick(CPoint point);
	CCreateBase* Copy();

	void Move(const Position& basePos,const Position& desPos);
	void Rotate(const Position& basePos, const double angle);
	void Mirror(const Position& pos1, const Position& pos2);
	//�ı��и��
	void ChangeDir();
	
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	
	void PropertyView();
	//�õ���Χ��
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	//XY���Ź���
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//����ͼ���ϵ�һ�����볤�ȷ���һ����
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//����������
	void GraphZoomScale(Position posBase,double dScale);
	//ʵ�߱�����
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	

private:
	//��ͼԪ���п�����ֱ�ߣ�Ҳ�п�����Բ��
	void DrawLineOrArc(CDC *pDC,int i,BOOL FlagShowArrow,BOOL FlagShowNum,Position l_posCurPos,Position l_posNextPos,double TuDu);
	//��ֱ�ߺ�Բ��
	void DrawLineAndArc(CDC *pDC,BOOL FlagShowArrow,Position l_posCurPos,Position l_posNextPos,double TuDu);
	//ֱ�ߵ�ʰȡ����
	BOOL IsPickLine(Position Pos1,Position Pos2,CPoint point);
	//Բ����ʰȡ����
	BOOL IsPickArc(Position Pos1,Position Pos2,CPoint point,double TuDu);
	//��������յ�͹�ȷ��ػ���
	double RtnLongArc(Position posClick,Position posBegin,Position PosEnd,double dTudu);
	//������㣬�յ㣬͹�ȣ�����㣬���������ػ��ϵ�
	Position RtnLongArcPos(Position posBegin,Position PosEnd,double dTudu,Position posClick,double dLongPos);
	//�������Ƿ�������н��㲢�ҽ����Ƿ���ֱ����
	BOOL RectangleOnRect(Position pos1,Position pos3);
	//���Ƿ���Բ����
	BOOL PosOnRectangleArc(Position pos,Position posBegin,Position posEnd,Position posCenter,int iDir);
	
	
	
	//��־��һ����Բ������ֱ��1:ֱ��   2��Բ��
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
	//�ı��и��
	void ChangeDir();
	
	
	void PropertyView();
	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	//�õ���Χ��
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	//XY���Ź���
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//����ͼ���ϵ�һ�����볤�ȷ���һ����
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//����������
	void GraphZoomScale(Position posBase,double dScale);
	//ʵ�߱�����
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	
	
public:
	Position m_Begin;
	Position m_Center;
	Position m_End;
	double m_dRadius;
	double m_dAngleStart;
	double m_dAngleEnd;
	int m_iDirect;	//��Բ�򻡵ķ���

private:
	//Բ���Ƿ��ھ�����
	BOOL ArcOnRect(Position pos1,Position pos3);
	//���Ƿ���Բ����
	BOOL PosOnArc(Position pos);
	//��������Ļ����ʾ����ʼ�Ǻ�ת���Ƕ�
	BOOL GetAngleStartAndOffset(double& dAngleStart,double& dAngleOffset,CPoint& potBegin);
	//������ʼ�Ǻ�ת���Ƕ�
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
	//�ı��и��
	void ChangeDir();
	
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	
	void PropertyView();
	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	//�õ���Χ��
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	//XY���Ź���
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//����ͼ���ϵ�һ�����볤�ȷ���һ����
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//����������
	void GraphZoomScale(Position posBase,double dScale);
	//ʵ�߱�����
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	
public:
	Position m_Center;		
	Position m_EndPos;
	double m_dRadius;		
	int m_iDirect;	//��Բ�򻡵ķ���

private:
	//Բ���Ƿ��ھ�����
	BOOL RoundOnRect(Position pos1,Position pos3);
	
	
};


class CCreateSpline  :public CCreatePolyline
{
	DECLARE_SERIAL(CCreateSpline)
public:
	CCreateSpline();
	virtual ~CCreateSpline();

	
	//���캯��
	CCreateSpline(CArray<Position,Position&>&  point,CArray<Position,Position&>& aryControlPoints,CArray<int,int&>& aryNumControl);
	//����������
	void DrawMove(CDC* pDC,int DrawMode,BOOL FlagShowArrow = FALSE,BOOL FlagShowNum = FALSE);
	CCreateBase* Copy();
	
	void Move(const Position& basePos,const Position& desPos);
	void Rotate(const Position& basePos, const double angle);
	void Mirror(const Position& pos1, const Position& pos2);
	//�ı��и��
	void ChangeDir();	
	//�ı����Կ��������ʱ
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	//ͨ�����Ƶ�ˢ����������
	void RefreshSpline();


	//���Ƶ㼯
	CArray<Position,Position&> m_aryControlPoints;
	//���Ƶ������ߵ㼯�е�λ��
	CArray<int,int&> m_aryNumControl;	
	
private:
	//����Ļ�ϵ��������
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
	//�ı��и��
	void ChangeDir();
	
	void PropertyView();
	void ChangeProperty(double e,int WhichEdit,int iNumOfTop=0);
	
	BOOL IsSelect(CRgn *selectrect,BOOL bComplex);
	//�õ���Χ��
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);
	//XY���Ź���
	BOOL GraphZoomXY(double dValueX, double dValueY);
	//����ͼ���ϵ�һ�����볤�ȷ���һ����
	BOOL RtnLongPos(Position ClickPos,double dLongPos,Position& posTarget,int& iWhichLine);
	//����������
	void GraphZoomScale(Position posBase,double dScale);
	//ʵ�߱�����
	PGRAPHPOS SolidTurnDash(double dLongSolid,double dLongEmpty,PUNDOLIST pUndolist);
	
private:
	int GetArcQuadrant(double &fArc);//���ݽǶ�������	
	//��ȡ��Բ���϶�Ӧ�Ƕ�ΪfArc��һ���������
	void GetEllipsesPoint(Position &output, double fArc);
	//��������յ�͹�ȷ��ػ���
	double RtnLongArc(Position posClick,Position posBegin,Position PosEnd,double dTudu);
	//������㣬�յ㣬͹�ȣ�����㣬���������ػ��ϵ�
	Position RtnLongArcPos(Position posBegin,Position PosEnd,double dTudu,Position posClick,double dLongPos);	
	//����Բ�Ƿ�������н���
	BOOL EllipseOnRect(Position pos1,Position pos3);



public:
	Position m_Begin;
	Position m_Second;
	Position m_End;
	double m_a;	//���᳤
	double m_b;	//���᳤
	Position m_Center;//��Բ��Բ�ĵ�����
	Position m_Focus1,m_Focus2;	//��Բ�Ľ�������
	Position m_PrePos;		//���ǰһ��
	double m_k;	//�����б�ʣ���Բ��ת�Ƕȣ�
//	CArray<Position,Position&> m_point;	
//	CArray<double,double&> m_dTuDu;//���͹�ȵ�	

private:
	//��־��һ����Բ������ֱ��1:ֱ��   2��Բ��
	int m_iFlagArcOrLine;		
	
};


class CCreateMCode  :public CCreateBase
{
	DECLARE_SERIAL(CCreateMCode)
public:
	CCreateMCode();
	CCreateMCode(CString str);
	virtual ~CCreateMCode();
	//�õ���Χ��
	BOOL GetBox(BOX2D *pBox,BOOL IsOutside = FALSE);

	CString m_strM_What;	//�ļ�ͷ��ŵ�M����
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
	//�õ���Χ��
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




