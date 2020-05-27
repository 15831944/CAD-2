#if !defined(AFX_DLGMILLSIDE_H__13276FCB_A05E_4694_95FF_C1684F1FB483__INCLUDED_)
#define AFX_DLGMILLSIDE_H__13276FCB_A05E_4694_95FF_C1684F1FB483__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMillSide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMillSide dialog

class CDlgMillSide : public CDialog
{
// Construction
public:
	CDlgMillSide(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMillSide)
	enum { IDD = IDD_DLG_MILL_SIDE };
	double	m_EditMillSideDis;
	//}}AFX_DATA
//	CTurnMill m_cTurnMill;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMillSide)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	
private:
	//变铣刀线的接口
	void TurnGraphMill();
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

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMillSide)
	afx_msg void OnMillsideOk();
	afx_msg void OnMillsideCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMILLSIDE_H__13276FCB_A05E_4694_95FF_C1684F1FB483__INCLUDED_)
