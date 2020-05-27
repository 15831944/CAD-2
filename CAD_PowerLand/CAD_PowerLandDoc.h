// CAD_PowerLandDoc.h : interface of the CCAD_PowerLandDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAD_POWERLANDDOC_H__90215736_BEAB_4C24_9A56_C2906775573C__INCLUDED_)
#define AFX_CAD_POWERLANDDOC_H__90215736_BEAB_4C24_9A56_C2906775573C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Position.h"
#include "CreateBase.h"
#include "OutputGCode.h"//����g�����ͷ�ļ�
#include "OutputDXFCode.h"//����DXF��ʽ��ͷ�ļ�
#include "DlgColorToM.h"//��ɫ��Ӧ�ŵ�M����Ķ���
#include "DlgMatrix.h"//����Ի���
#include "OpenGCode.h"//��G��������ͷ�ļ�
#include "OpenDXFCode.h"//��DXF�ļ�������ͷ�ļ�
#include "OpenPLTCode.h"//��PLT�ļ������ͷ�ļ�
#include "DlgParamGlassGrind.h"//�򿪲���1�Ի���ͷ�ļ�
#include "DlgParamInoutKnife.h"//�򿪽��˵��Ի���ͷ�ļ�
#include "DlgParamClothCut.h"//�в����Ի���ͷ�ļ�
#include "TransToGraphList.h"//�ѽṹת��DATASTRUCT�ṹ
#include "KnifeData.h"//������̬��
#include "GlassGrind.h"//ĥ�����Ŀ�
#include "GraphIOKinfe.h"//���˵����ͷ�ļ�
#include "GraphScale.h"//���Ź��� 
#include "OverCut.h"//����	
#include "DlgWorkFor.h"//���ղ���
#include "DlgSendFileName.h"//�����ļ��Ի���Ĳ���
#include "GraphTogether.h"//���°�˳������
#include "TurnMill.h"	//���߱��ϳ������
#include "Undo.h"		//���峷���ṹ��ͷ�ļ�
#include "UndoRedo.h"
#include "DlgMillSide.h" //ϳ�ߵ�ͷ�ļ�
#include "ToLessPoint.h"	//��ͼ�ε��࣬�Ѷ��Ķ���߱���ٵ�Ķ��߶�
#include "DlgAddRect.h"	//�ӱ߿����
#include "DlgParallel.h"//ƽ���ߵ�ͷ�ļ�
#include "DlgAddPoint.h"//�ڱ߿�����ӵ��ͷ�ļ�
#include "DeleteMoreLine.h"//ɾ���ص�

//������ʾ�Ĵ���
#define SEND_ALL			 0
#define SEND_EDIT			 1
#define SEND_RICHEDIT		 2
//��������ͼ��
#define MY_LINE				 0
#define MY_POLYLINE			 1
#define MY_RECTANGLE		 2
#define MY_ARC				 3
#define MY_ROUND			 4
#define MY_ELLIPSE			 5
#define MY_SPLINE			 6
#define MY_POINT			 7




class CCAD_PowerLandDoc : public CDocument
{
protected: // create from serialization only
	CCAD_PowerLandDoc();
	DECLARE_DYNCREATE(CCAD_PowerLandDoc)

// Attributes
public:
	// ͼԪ��������
	CObList		m_CCreateBaseList,m_CCreateBaselayer1;  
	// ʰȡ��
	CObArray    m_CollectOfPicked;
	//����ͼԪ
	CCreateBase* m_pMeasureCreateBase;	
	//װ����������ַ���	
	CString m_strCmdAll;	
	CUndoRedo *m_Undo;


	//ģ����Ľṹ����
	struct stcPARAM_CLOTH_CUT
	{//            �����е���С�ǶȺͳ���     ���ŵĲ�����X,Y������ŵľ���		ϳ�ۿ��
		double m_dOverCutLong,m_dOverCutAngle,m_dScaleX,m_dScaleY             ,m_dChamfer;
		//��¼��Щ���ĵ�ѡ����Ҫ����
		BOOL m_bOverCutKnife1,m_bOverCutKnife2,m_bOverCutKnife3,m_bOverCutKnife4,m_bOverCutKnife5;
		//ϳ���;���		    ƽ��ˮƽ��ֱ����,ָ����ת�Ƕ�
		double m_dMillSideDis,m_dParallelHori,m_dParallelVert,m_dRotateAngle;
		//�Ӿ��α߿����
		double m_dAddRectTop,m_dAddRectBottom,m_dAddRectLeft,m_dAddRectRight,dAddRectRadius;
		//ͼ�μ򻯵�   �������뾶            ����
		double dLessPointRadius,dSimplify;
		//�����߲��� ���߳���		ʵ�߳���
		double	dEditDashEmpty,dEditDashSolid;
		//�󵶲������ҵ���
		BOOL m_bMillSideLorR;
		//���Ӿ��ο�ʱ�Ƿ�Ҫ���ǣ��ĸ���
		BOOL bChamferLT,bChamferLB,bChamferRT,bChamferRB;
		//����ͼ���˵���ʽ
		int iChamferEndStyle;
		//�ƶ���ԭ��Ĳ���
		double dMoveOriginX,dMoveOriginY;
	};
	stcPARAM_CLOTH_CUT m_stcClothCut;

	//g�����ж���M����Ľṹ����
	struct stcPARAM_COLOR_FOR_MCODE
	{
		//��ɫ�Ի�����EDIT������ַ�
		CString m_CBM1,m_CBM2,m_CBM3,m_CBM4,m_CBM5,m_SBM1,m_SBM2,m_SBM3,m_SBM4,m_SBM5;
		CString m_ColorMean1,m_ColorMean2,m_ColorMean3,m_ColorMean4,m_ColorMean5;	//��������
		COLORREF m_color1,m_color2,m_color3,m_color4,m_color5;//����ߵ���ɫ 
	};
	stcPARAM_COLOR_FOR_MCODE m_stcColorToM;

	//ĥ�����Ľṹ����
	struct stcPARAM_GLASS_GRIND
	{
		int RouphCount;		//��ĥ����
		double RouphSize;	//��ĥ������
		int PerfectCount;	//��ĥ����
		double PerfectSize;	//��ĥ������
		int PolishCount;	//�׹����
		double PolishSize;	//�׹������
		double KnifeRadius;//���ְ뾶
		int DaoBuStyle;//0�󵶲�1���ҵ���
		//ͼ��������ɫ��ʱ�뷽��
		int iDirClock1,iDirClock2,iDirClock3,iDirClock4,iDirClock5;
	};
	stcPARAM_GLASS_GRIND m_stcGlassGrind;
	
	//�ӽ��˵��Ľṹ����
	struct stcPARAM_IO_KNIFE
	{	
		//����Բ���Ƕȣ��뾶�����߳�
		double IArcAngle,IArcRadius,IArcRadiusPercent,ILineLong,ILineLongPercent;
		//�˵�Բ���Ƕȣ��뾶�����߳�                                          //�˵��ص���
		double OArcAngle,OArcRadius,OArcRadiusPercent,OLineLongPercent,OLineLong,OOverLong;	
		BOOL   IsIKnifeVertical;//�Ƿ��Ǵ�ֱ������0���ǣ�1Ϊ��
		BOOL   IsOknifeVertical;//�Ƿ��Ǵ�ֱ�˵���0���ǣ�1Ϊ��
		BOOL   IsCenter;		//�Ƿ����ߵ����Ľ�����0Ϊ���ǣ�1 Ϊ�������Ľ���
	};
	stcPARAM_IO_KNIFE m_stcIOKnife;
	//���ղ���
	struct stcPARAM_WORK_FOR
	{	//										��תʱ�Ƿ���ԭͼ������ʱ�Ƿ���Ĭ����������G��ʽ������DXF��ʽ��									
		BOOL IsGlassCut,IsGlassGrind,IsClothCut,RotateSymmetrySaveOld,bSaveDefault,bSendStyleG,bSendStyleDxf;
		CString strFilePathG,strFilePathDxf;//�ļ�����·��
		CString FileName;//�����ļ�����
		BOOL IsSendFile;//�Ƿ����ļ�
		BOOL IsShowSize,IsShowRulerLine,IsShowCoordinate;//�Ƿ���ʾ��̨�ߴ�
		double MachineX,MachineY;//��̨�ߴ�X��Y
		BOOL IsChinese;
	};
	stcPARAM_WORK_FOR m_stcWorkFor;
	
	void Draw(CDC *pDC);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonDblClk(CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);		
	void RemoveAllCollected();
	//����ɫ�����M�ĶԻ��� 
	void OnProperty();	
	//����ɫ1�Ի���
	void OnOpenDlgParamGlassGrind();	
	//�򿪾���Ի���
	void OnOpenDlgMatrix();
	//�򿪽����˵�����
	void OnOpenDlgParamInoutKnife();
	//���в�����Ĺ��жԻ���
	void OnOpenDlgParamOverCut();
	//�򿪹��ղ����Ի��� 
	void OnOpenDlgWorkFor();
	//�ļ�����
	void OnFileSend();
	//��ĥ�����ӽ��˵�
	void OnGraphKinfe();
	//���Ź���
	void OnGraphScale();
	//���й���
	void OverCut();
	//�ϲ����� 
	void OnGraphTogether();
	//ĥ��
	void OnGraphForGrind();
	//���߱��ϳ������
	void OnTurnMill();
	//��ͼ�α��ϳ�ߵ�ͼ��
	void OnMillSide();
	//�Ѷ��Ķ���߱���ٵ��
	void OnToLessPoint();
	//�ӱ߿����
	void OnAddRect();
	//ƽ����
	void OnParallel();
	//��ͼ�ε����ĵ���ת
	void OnRotateCenter(double angle);
	//��ͼ�ε����£����ң�XY,�Գ�
	void OnSymmetry(int Case);
	//ɾ���ص�
	void OnDeleteMoreLine();
	//����ѡ�е�������
	int NumPicked(Position pos,long& createbase);
	//��ͼ����ʾ��ȫ��
	void OnFullScreemShow();
	//��ͼ���ƶ���ԭ��
	void OnMoveOrigin();
	//�˳�������
	BOOL OnCloseSaveFile();
	//���߱������
	void OnSolidToDash();



	// �ж�ͼԪ�����Ƿ��Ѿ���ѡ����
	int IsSelected(CCreateBase* pCreateBase);	
	//������ɫ�ŷ�����ɫ��
	COLORREF RtnColorRGB(int num);
	//������ɫ�ŷ���M���롣
	BOOL RtnColorMCode(CString& strMsb,CString& strMcb,int num  = -1,COLORREF rgbColor= -1);
	//���ѡ�񼯰�Χ��
	BOOL GetCollectBox(BOX2D *pBox);
	//��͹��;				��㣬	ĩ�㣬				Բ�ģ�	�ͷ���͹��=�ķ�֮һԲ�Ľ�֮����ֵ
	double RtnTudu(Position begin,Position end,Position center,int clock);	
	//�������Բ����Ϣ��Բ��										͹��
	void polylineToArc(Position& BeginPoint, Position& EndPoint, double u, Position& CenterPoint);
	//������ֵ���
	void PosBeEqualed(CADPOINT &pos1,Position pos2);
	//������ֵ���
	void PosBeEqualed(Position &pos1,CADPOINT pos2);
	//��M�ַ����е�M�ַ��滻
	BOOL ReplaceMCode(int color,CArray<class CString,class CString &>& aryMSB,CString strMSB,
		CArray<class CString,class CString &>& aryMCB,CString strMCB);
	//����㷵���Ƿ�ѡ��ͼ�Σ�	�������			���ص�ͼ��			ͼ���ϵĵ�
	BOOL ClickCreateBase(Position posClick,long& pCreateBase,int& iPick);
	//����ͼ������1ֱ�ߣ�2����ߣ�3���Σ�4Բ����5Բ��6��Բ
	int RtnIsKindOf(CCreateBase* pCreateBase);
	//��������ͼ������PGRAPHPOS
	void AddTail(PGRAPHPOS headGraphPos,PGRAPHPOS tailGraphPos);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCAD_PowerLandDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCAD_PowerLandDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//{{AFX_MSG(CCAD_PowerLandDoc)
	afx_msg void OnFileOpen();
	afx_msg BOOL OnFileSave();
	afx_msg BOOL OnFileSaveAs();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


private:
	//�������ߴ�
	void DrawMeasure(CDC* pDC);
	//��ʼ������
	void InitDate();

	
	//��ǰ�ļ���·��
	CString m_strPathFile,m_strNameFile;
	//���ļ�ʱ������ 1Ϊg��2Ϊdxf
	int m_iStyleFile;
	//�Ƿ��ǵ�һ�δ�
	BOOL m_bFirstOpen;
};




/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAD_POWERLANDDOC_H__90215736_BEAB_4C24_9A56_C2906775573C__INCLUDED_)
