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
	//��ϳ���ߵĽӿ�
	void TurnGraphMill();
	//�Ѳ��պ϶���߱��ϳ����Ҫ��ͼ��
	void TurnOpenPolyline();
	//�ѱպϵĶ��߶�ת�� ��ϳ����Ҫ��ͼ��
	void TurnClosePolyline();
	//��ֱ�߱�ϳ��ͼ��
	void TurnLine();
	//��Բ����ϳ����
	void TurnArc();
	//��Բ��ϳ����
	void TurnRound();
	//����Բ���ϳ����
	void TurnEllipse();
	
	
	//�������ߵĽ���			�������ϵ��ĸ���									���صĽ���
	BOOL GetIntersectPoint(Position pos1,Position pos2,Position pos3,Position pos4,Position &pos);
	//������������Ӳ����պϵ�
	void AddUndoList(long pCreatebase1,long pCreatebase2 = NULL);
	
	CCreateBase* m_pMillGraph;
	
	//������Ķ���
	CUndoRedo m_UndoRedo;
	//������������ӵ�һ�����	
	PUNDOLIST m_pUndoList;
	//��ͼ�ε�ָ��
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
