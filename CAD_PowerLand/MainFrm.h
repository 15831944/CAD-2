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

#include "HarddogInfo.h"//���ܹ�ͷ�ļ�
#include "galil\\Dmcwin.h"//DMCͷ�ļ�

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
	CDlgPropertyDraw  m_DlgPropertyBar;		//���������Ҫ��CDialogBar����
	CToolBar    m_wndDrawToolBar;//��ͼ��
	CToolBar    m_wndExcuteBar;//�����и��������
	CToolBar    m_wndModifyToolBar;//�޸�ͼ��
	CToolBar    m_wndTemplateToolBar;//ģ���������
	CToolBar    m_wndParamTollbar;//����������
	CToolBar    m_wndGrindGlassToolBar;//ĥ����������
	CToolBar    m_wndViewBar;//��ͼ���ڹ�����
	CPropertyBar m_wndPropBar;		//ͼ��ɫ������

	int m_iNumOfTop;		//������
//	COLORREF m_cColor1,m_cColor2,m_cColor3,m_cColor4,m_cColor5;//�ߵ���ɫ
	Position m_PerXPoint;		//�����ʱ�������ǰһ��λ��
	CPoint m_potMouseCurPos;
	//�Ƿ�򿪿�����
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
	CStatusBar  m_wndStatusBar;		//״̬��
	CToolBar    m_wndToolBar;		//������


// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillfocusEdit1();		//�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
	afx_msg void OnKillfocusEdit2();		//�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
	afx_msg void OnKillfocusEdit3();		//�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
	afx_msg void OnKillfocusEdit4();		//�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
	afx_msg void OnKillfocusEdit5();		//�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
	afx_msg void OnKillfocusEdit6();		//�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
	afx_msg void OnKillfocusEdit7();		//�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����;
	afx_msg void OnKillfocusEdit8();		//�����ԶԻ����Edit�ؼ�ʧȥ����ʱ����Ӧ����
	afx_msg void OnSetfocusEdit1();			//�����ԶԻ����Edit�ؼ���ý���ʱ����Ӧ����
	afx_msg void OnSetfocusEdit2();			//�����ԶԻ����Edit�ؼ���ý���ʱ����Ӧ����
	afx_msg void OnSetfocusEdit3();			//�����ԶԻ����Edit�ؼ���ý���ʱ����Ӧ����
	afx_msg void OnSetfocusEdit4();			//�����ԶԻ����Edit�ؼ���ý���ʱ����Ӧ����
	afx_msg void OnSetfocusEdit5();			//�����ԶԻ����Edit�ؼ���ý���ʱ����Ӧ����
	afx_msg void OnSetfocusEdit6();			//�����ԶԻ����Edit�ؼ���ý���ʱ����Ӧ����
	afx_msg void OnSetfocusEdit7();			//�����ԶԻ����Edit�ؼ���ý���ʱ����Ӧ����
	afx_msg void OnSetfocusEdit8();			//�����ԶԻ����Edit�ؼ���ý���ʱ����Ӧ����
	afx_msg void OnChangeEdit1();			//�����ԶԻ����Edit�ؼ����ݸı�ʱ����Ӧ����	
	afx_msg void OnBtnForward();
	afx_msg void OnBtnBehind();
	afx_msg void OnBtnInsertBefore();	//��ǰ�����
	afx_msg void OnBtnInsertAfter();
	afx_msg void OnBtnCutdir();	//�ı��и��
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
//�Լ�����ĺ���
protected:

private:

	//���ԶԻ�����ı༭���������
	struct stcPROPERTY_EDIT
	{
		double dEdit1,dEdit2,dEdit3,dEdit4,dEdit5,dEdit6,dEdit7,dEdit8; 
	};
	stcPROPERTY_EDIT m_stcProperEdit;


	void DrawPointX(CDC* pDC,Position pos);	//�ڵ��ϻ���X;
	//����������
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
