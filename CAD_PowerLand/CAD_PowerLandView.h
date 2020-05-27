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
	//��������ת��Ļ����
	void WorldToScreem(const Position& pos,CPoint& ScreemPt);		
	//��Ļ����ת��������
	void ScreemToWorld(const CPoint& ScreemPt,Position& pos);
	//�������Ȼ����ִ�У����ڵ���ֱ��ʱ��Ҫ��������յ�ɶ�ģ�
	void GetAndSendCommand();		
	//�������ű���ϵ��
	double ReturnProportion();
	//�����ѡ���µķ���
	void ClearSelectLine();	
	//ɾ��ͼԪ����������ĵ����ɾ������ťҪʵ�ֵĹ���
	void DeleteList();

	//�õ����Ĺ��ܶԽ�
	CDrawBase* m_pDrawBase;
	//һЩ��ť�Ŀ���
	struct stcSwitchBottom
	{	//�Ƿ���ʾ���� �켣�͵�˳�������Ͷ˵�        ������ģʽ
		BOOL bShowArrow,bShowNum,bPointNear,bPointEnd,bOrthogonal;
	};
	stcSwitchBottom m_stcSwitch;

	//���Ĺ���ID��
	int m_iLastFuntion;
	//���ص�λ
	int m_iSwitch;
	//�ĸ���Ť������
	int m_iFlagWhichBtnDown;
	//��ѡ�õ�������
	Position m_PosBegin,m_PosLast;
	//��ߵ�X��Y
	double m_dRulerLineX,m_dRulerLineY;
	


protected: // create from serialization only
	CCAD_PowerLandView();
	DECLARE_DYNCREATE(CCAD_PowerLandView)

	double m_dOriginX,m_dOriginY;		//ԭ��λ�á�
	double m_dProportion;				//������ת��ʱ���Ŵ����С�ı���ϵ����
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
	void DrawCoord(CDC* pDC);	//������XY������
	BOOL RemindNoSelect();//��ʾû�е���ѡ��ͼ��
	void AngleTurnHoriMode(double& dAngleHori);//�Ƕȱ�ɰ���ˮƽӦ�õĽǶ�
	void AngleTurnVertMode(double& dAngleHori);//�Ƕȱ�ɰ��ߴ�ֱӦ�õĽǶ�
	
	//ģ��켣���߳�
	static DWORD __stdcall ThreadFunOne(LPVOID lpParam);
	HANDLE	m_ThreadOne;
	BOOL m_bBreakThreadOne;
	//����м�����ʱ�õ�
	Position m_cOrigin;
	int m_bFlagMBtnMove;
	//�Ҽ������˵�ʱ�����ͼ�ζ�Ӧ�ĽǶ�
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
