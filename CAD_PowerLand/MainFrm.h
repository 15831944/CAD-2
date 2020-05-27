// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__C34601BC_AB26_4C07_B88C_A4D40091F8DB__INCLUDED_)
#define AFX_MAINFRM_H__C34601BC_AB26_4C07_B88C_A4D40091F8DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "PropertyBar.h"
#include "dlgpropertydraw.h"
#include "DlgCommandView.h"
#include "Position.h"

#include "HarddogInfo.h"//加密狗头文件
#include "galil\\Dmcwin.h"//DMC头文件

class CMainFrame : public CFrameWnd
{
	
	
protected: // create from serialization only

	CMainFrame();
	
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	void DockControlBarLeftOf(CControlBar* Bar,CControlBar* LeftOf);
	
	CHarddogInfo m_Harddog;	

// Operations
public:
	CDlgCommandView   m_DlgCommandBar;
	CDlgPropertyDraw  m_DlgPropertyBar;		//这个建立不要是CDialogBar的类
	CToolBar    m_wndDrawToolBar;//画图栏
	CToolBar    m_wndExcuteBar;//启动切割机工具栏
	CToolBar    m_wndModifyToolBar;//修改图栏
	CToolBar    m_wndTemplateToolBar;//模板机工具栏
	CToolBar    m_wndParamTollbar;//参数工具栏
	CToolBar    m_wndGrindGlassToolBar;//磨玻璃工具栏
	CToolBar    m_wndViewBar;//视图窗口工具栏
	CPropertyBar m_wndPropBar;		//图颜色工具栏

	int m_iNumOfTop;		//顶点数
//	COLORREF m_cColor1,m_cColor2,m_cColor3,m_cColor4,m_cColor5;//线的颜色
	Position m_PerXPoint;		//多段线时标明点的前一个位置
	CPoint m_potMouseCurPos;
	//是否打开控制器
	BOOL m_bSaveFile;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;		//状态栏
	CToolBar    m_wndToolBar;		//工具栏


// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillfocusEdit1();		//当属性对话框的Edit控件失去焦点时的响应函数;
	afx_msg void OnKillfocusEdit2();		//当属性对话框的Edit控件失去焦点时的响应函数;
	afx_msg void OnKillfocusEdit3();		//当属性对话框的Edit控件失去焦点时的响应函数;
	afx_msg void OnKillfocusEdit4();		//当属性对话框的Edit控件失去焦点时的响应函数;
	afx_msg void OnKillfocusEdit5();		//当属性对话框的Edit控件失去焦点时的响应函数;
	afx_msg void OnKillfocusEdit6();		//当属性对话框的Edit控件失去焦点时的响应函数;
	afx_msg void OnKillfocusEdit7();		//当属性对话框的Edit控件失去焦点时的响应函数;
	afx_msg void OnKillfocusEdit8();		//当属性对话框的Edit控件失去焦点时的响应函数
	afx_msg void OnSetfocusEdit1();			//当属性对话框的Edit控件获得焦点时的响应函数
	afx_msg void OnSetfocusEdit2();			//当属性对话框的Edit控件获得焦点时的响应函数
	afx_msg void OnSetfocusEdit3();			//当属性对话框的Edit控件获得焦点时的响应函数
	afx_msg void OnSetfocusEdit4();			//当属性对话框的Edit控件获得焦点时的响应函数
	afx_msg void OnSetfocusEdit5();			//当属性对话框的Edit控件获得焦点时的响应函数
	afx_msg void OnSetfocusEdit6();			//当属性对话框的Edit控件获得焦点时的响应函数
	afx_msg void OnSetfocusEdit7();			//当属性对话框的Edit控件获得焦点时的响应函数
	afx_msg void OnSetfocusEdit8();			//当属性对话框的Edit控件获得焦点时的响应函数
	afx_msg void OnChangeEdit1();			//当属性对话框的Edit控件内容改变时的响应函数	
	afx_msg void OnBtnForward();
	afx_msg void OnBtnBehind();
	afx_msg void OnBtnInsertBefore();	//向前后插入
	afx_msg void OnBtnInsertAfter();
	afx_msg void OnBtnCutdir();	//改变切割方向
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
//自己定义的函数
protected:

private:

	//属性对话框里的编辑框里的数字
	struct stcPROPERTY_EDIT
	{
		double dEdit1,dEdit2,dEdit3,dEdit4,dEdit5,dEdit6,dEdit7,dEdit8; 
	};
	stcPROPERTY_EDIT m_stcProperEdit;


	void DrawPointX(CDC* pDC,Position pos);	//在点上画个X;
	//创建工具栏
	BOOL CreateDrawToolBar();	
	BOOL CreateModifyToolBar();
	BOOL CreatePropertyBar();
	BOOL CreateParamToolBar();
	BOOL CreateTemplateToolBar();
	BOOL CreateGrindGlassToolBar();
	BOOL CreateViewToolBar();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__C34601BC_AB26_4C07_B88C_A4D40091F8DB__INCLUDED_)
