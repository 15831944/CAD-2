// CAD_PowerLandView.h : interface of the CCAD_PowerLandView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_POWERLANDVIEW_H__4E4A45CA_F823_4439_A367_D51355ABF4F0__INCLUDED_)
#define AFX_CAD_POWERLANDVIEW_H__4E4A45CA_F823_4439_A367_D51355ABF4F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Position.h"
#include "DrawBase.h"
#include "Modify.h"
#include "DlgCommandView.h"


#define MEIYOU			 0
#define COLOR			 1
#define DRAW		     2
#define MODIFY			 3
#define PROPERTY	     4
#define COMMAND			 5
#define DRAW_SELECT      6
#define TEMPLATE		 7
#define VIEW			 8
#define QWERT

#include <afxtempl.h>
class CCAD_PowerLandView : public CView
{
public:
	CCAD_PowerLandDoc* GetDocument();
	
	// Operations
	//世界坐标转屏幕坐标
	void WorldToScreem(const Position& pos,CPoint& ScreemPt);		
	//屏幕坐标转世界坐标
	void ScreemToWorld(const CPoint& ScreemPt,Position& pos);
	//获得命令然后发送执行（如在点完直线时，要输入起点终点啥的）
	void GetAndSendCommand();		
	//返回缩放比例系数
	double ReturnProportion();
	//清除框选留下的方框
	void ClearSelectLine();	
	//删除图元函数，上面的点击“删除”按钮要实现的功能
	void DeleteList();

	//用到鼠标的功能对角
	CDrawBase* m_pDrawBase;
	//一些按钮的开关
	struct stcSwitchBottom
	{	//是否显示刀和 轨迹和刀顺和最近点和端点        和正交模式
		BOOL bShowArrow,bShowNum,bPointNear,bPointEnd,bOrthogonal;
	};
	stcSwitchBottom m_stcSwitch;

	//最后的功能ID号
	int m_iLastFuntion;
	//开关的位
	int m_iSwitch;
	//哪个按扭被按下
	int m_iFlagWhichBtnDown;
	//框选用的两个点
	Position m_PosBegin,m_PosLast;
	//标尺的X和Y
	double m_dRulerLineX,m_dRulerLineY;
	


protected: // create from serialization only
	CCAD_PowerLandView();
	DECLARE_DYNCREATE(CCAD_PowerLandView)

	double m_dOriginX,m_dOriginY;		//原点位置。
	double m_dProportion;				//鼠标滚轮转动时，放大或缩小的比例系数。
// Attributes

public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCAD_PowerLandView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCAD_PowerLandView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


	
	
// Generated message map functions
protected:
	//{{AFX_MSG(CCAD_PowerLandView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags,CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnButtonDraw(int BtnID);
	afx_msg void OnButtonFix(int BtnID);
	afx_msg void OnUpdateButtonFix(CCmdUI* pCmdUI);
	afx_msg void OnButtonParam(int BtnID);
	afx_msg void OnButtonShowBar(int BtnID);
	afx_msg void OnUpdateButtonShowBar(CCmdUI* pCmdUI);
	afx_msg void OnButtonSwitch(int BtnID);
	afx_msg void OnUpdateButtonSwitch(CCmdUI* pCmdUI);
	afx_msg void OnButtonRotateSymmetry(int BtnID);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
private:
	void DrawCoord(CDC* pDC);	//画坐标XY坐标轴
	BOOL RemindNoSelect();//提示没有单击选中图形
	void AngleTurnHoriMode(double& dAngleHori);//角度变成按线水平应该的角度
	void AngleTurnVertMode(double& dAngleHori);//角度变成按线垂直应该的角度
	
	//模拟轨迹的线程
	static DWORD __stdcall ThreadFunOne(LPVOID lpParam);
	HANDLE	m_ThreadOne;
	BOOL m_bBreakThreadOne;
	//鼠标中键按下时用的
	Position m_cOrigin;
	int m_bFlagMBtnMove;
	//右键弹出菜单时点击的图形对应的角度
	double m_dAngleRightBtn;
};

#ifndef _DEBUG  // debug version in CAD_PowerLandView.cpp
inline CCAD_PowerLandDoc* CCAD_PowerLandView::GetDocument()
   { return (CCAD_PowerLandDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAD_POWERLANDVIEW_H__4E4A45CA_F823_4439_A367_D51355ABF4F0__INCLUDED_)
