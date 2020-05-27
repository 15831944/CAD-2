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
#include "OutputGCode.h"//保存g代码的头文件
#include "OutputDXFCode.h"//保存DXF格式的头文件
#include "DlgColorToM.h"//颜色对应着的M代码的定义
#include "DlgMatrix.h"//矩阵对话框
#include "OpenGCode.h"//打开G代码的类的头文件
#include "OpenDXFCode.h"//打开DXF文件的类物头文件
#include "OpenPLTCode.h"//打开PLT文件的类的头文件
#include "DlgParamGlassGrind.h"//打开参数1对话框头文件
#include "DlgParamInoutKnife.h"//打开进退刀对话框头文件
#include "DlgParamClothCut.h"//切布机对话框头文件
#include "TransToGraphList.h"//把结构转成DATASTRUCT结构
#include "KnifeData.h"//刀补动态库
#include "GlassGrind.h"//磨玻璃的库
#include "GraphIOKinfe.h"//进退刀类的头文件
#include "GraphScale.h"//绽放功能 
#include "OverCut.h"//过切	
#include "DlgWorkFor.h"//工艺参数
#include "DlgSendFileName.h"//发送文件对话框的参数
#include "GraphTogether.h"//重新按顺序排列
#include "TurnMill.h"	//把线变成铣刀的类
#include "Undo.h"		//定义撤消结构的头文件
#include "UndoRedo.h"
#include "DlgMillSide.h" //铣边的头文件
#include "ToLessPoint.h"	//简化图形的类，把多点的多段线变成少点的多线段
#include "DlgAddRect.h"	//加边框的类
#include "DlgParallel.h"//平行线的头文件
#include "DlgAddPoint.h"//在边框里面加点的头文件
#include "DeleteMoreLine.h"//删除重刀

//发送提示的窗口
#define SEND_ALL			 0
#define SEND_EDIT			 1
#define SEND_RICHEDIT		 2
//属于哪种图形
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
	// 图元对象链表
	CObList		m_CCreateBaseList,m_CCreateBaselayer1;  
	// 拾取集
	CObArray    m_CollectOfPicked;
	//测量图元
	CCreateBase* m_pMeasureCreateBase;	
	//装所有命令的字符串	
	CString m_strCmdAll;	
	CUndoRedo *m_Undo;


	//模板机的结构数据
	struct stcPARAM_CLOTH_CUT
	{//            过切中的最小角度和长度     缩放的参数，X,Y相对缩放的距离		铣槽宽度
		double m_dOverCutLong,m_dOverCutAngle,m_dScaleX,m_dScaleY             ,m_dChamfer;
		//记录哪些刀的单选框需要过切
		BOOL m_bOverCutKnife1,m_bOverCutKnife2,m_bOverCutKnife3,m_bOverCutKnife4,m_bOverCutKnife5;
		//铣刀和距离		    平行水平垂直距离,指定旋转角度
		double m_dMillSideDis,m_dParallelHori,m_dParallelVert,m_dRotateAngle;
		//加矩形边框参数
		double m_dAddRectTop,m_dAddRectBottom,m_dAddRectLeft,m_dAddRectRight,dAddRectRadius;
		//图形简化的   允许最大半径            精度
		double dLessPointRadius,dSimplify;
		//变虚线参数 空线长度		实线长度
		double	dEditDashEmpty,dEditDashSolid;
		//左刀补还是右刀补
		BOOL m_bMillSideLorR;
		//处加矩形框时是否要倒角，四个角
		BOOL bChamferLT,bChamferLB,bChamferRT,bChamferRB;
		//轮廓图两端的样式
		int iChamferEndStyle;
		//移动到原点的参数
		double dMoveOriginX,dMoveOriginY;
	};
	stcPARAM_CLOTH_CUT m_stcClothCut;

	//g代码中定义M代码的结构数据
	struct stcPARAM_COLOR_FOR_MCODE
	{
		//颜色对话框中EDIT框里的字符
		CString m_CBM1,m_CBM2,m_CBM3,m_CBM4,m_CBM5,m_SBM1,m_SBM2,m_SBM3,m_SBM4,m_SBM5;
		CString m_ColorMean1,m_ColorMean2,m_ColorMean3,m_ColorMean4,m_ColorMean5;	//刀的名字
		COLORREF m_color1,m_color2,m_color3,m_color4,m_color5;//五个线的颜色 
	};
	stcPARAM_COLOR_FOR_MCODE m_stcColorToM;

	//磨玻璃的结构数据
	struct stcPARAM_GLASS_GRIND
	{
		int RouphCount;		//粗磨次数
		double RouphSize;	//粗磨进给量
		int PerfectCount;	//精磨次数
		double PerfectSize;	//精磨进给量
		int PolishCount;	//抛光次数
		double PolishSize;	//抛光进给量
		double KnifeRadius;//刀轮半径
		int DaoBuStyle;//0左刀补1是右刀补
		//图形五种颜色的时针方向
		int iDirClock1,iDirClock2,iDirClock3,iDirClock4,iDirClock5;
	};
	stcPARAM_GLASS_GRIND m_stcGlassGrind;
	
	//加进退刀的结构数据
	struct stcPARAM_IO_KNIFE
	{	
		//进刀圆弧角度，半径，引线长
		double IArcAngle,IArcRadius,IArcRadiusPercent,ILineLong,ILineLongPercent;
		//退刀圆弧角度，半径，引线长                                          //退刀重叠量
		double OArcAngle,OArcRadius,OArcRadiusPercent,OLineLongPercent,OLineLong,OOverLong;	
		BOOL   IsIKnifeVertical;//是否是垂直进刀，0不是，1为是
		BOOL   IsOknifeVertical;//是否是垂直退刀，0不是，1为是
		BOOL   IsCenter;		//是否在线的中心进刀，0为不是，1 为是在中心进刀
	};
	stcPARAM_IO_KNIFE m_stcIOKnife;
	//工艺参数
	struct stcPARAM_WORK_FOR
	{	//										翻转时是否保留原图，发送时是否是默认名，发送G格式，发送DXF格式。									
		BOOL IsGlassCut,IsGlassGrind,IsClothCut,RotateSymmetrySaveOld,bSaveDefault,bSendStyleG,bSendStyleDxf;
		CString strFilePathG,strFilePathDxf;//文件发送路径
		CString FileName;//发送文件名称
		BOOL IsSendFile;//是否发送文件
		BOOL IsShowSize,IsShowRulerLine,IsShowCoordinate;//是否显示机台尺寸
		double MachineX,MachineY;//机台尺寸X，Y
		BOOL IsChinese;
	};
	stcPARAM_WORK_FOR m_stcWorkFor;
	
	void Draw(CDC *pDC);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonDblClk(CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);		
	void RemoveAllCollected();
	//打开颜色定义的M的对话框 
	void OnProperty();	
	//打开颜色1对话框
	void OnOpenDlgParamGlassGrind();	
	//打开矩阵对话框
	void OnOpenDlgMatrix();
	//打开进刀退刀函数
	void OnOpenDlgParamInoutKnife();
	//打开切布机里的过切对话框
	void OnOpenDlgParamOverCut();
	//打开工艺参数对话框 
	void OnOpenDlgWorkFor();
	//文件发送
	void OnFileSend();
	//给磨玻璃加进退刀
	void OnGraphKinfe();
	//缩放功能
	void OnGraphScale();
	//过切功能
	void OverCut();
	//合并功能 
	void OnGraphTogether();
	//磨边
	void OnGraphForGrind();
	//把线变成铣刀的线
	void OnTurnMill();
	//把图形变成铣边的图形
	void OnMillSide();
	//把多点的多段线变成少点的
	void OnToLessPoint();
	//加边框的类
	void OnAddRect();
	//平行线
	void OnParallel();
	//以图形的中心点旋转
	void OnRotateCenter(double angle);
	//以图形的上下，左右，XY,对称
	void OnSymmetry(int Case);
	//删除重刀
	void OnDeleteMoreLine();
	//返回选中的哪条线
	int NumPicked(Position pos,long& createbase);
	//把图形显示到全屏
	void OnFullScreemShow();
	//把图形移动到原点
	void OnMoveOrigin();
	//退出进保存
	BOOL OnCloseSaveFile();
	//把线变成虚线
	void OnSolidToDash();



	// 判断图元对象是否已经在选择集中
	int IsSelected(CCreateBase* pCreateBase);	
	//输入线色号返回颜色。
	COLORREF RtnColorRGB(int num);
	//输入线色号返回M代码。
	BOOL RtnColorMCode(CString& strMsb,CString& strMcb,int num  = -1,COLORREF rgbColor= -1);
	//获得选择集包围盒
	BOOL GetCollectBox(BOX2D *pBox);
	//求凸度;				起点，	末点，				圆心，	和方向，凸度=四分之一圆心角之正切值
	double RtnTudu(Position begin,Position end,Position center,int clock);	
	//计算根据圆弧信息求圆心										凸度
	void polylineToArc(Position& BeginPoint, Position& EndPoint, double u, Position& CenterPoint);
	//让两个值相等
	void PosBeEqualed(CADPOINT &pos1,Position pos2);
	//让两个值相等
	void PosBeEqualed(Position &pos1,CADPOINT pos2);
	//把M字符集中的M字符替换
	BOOL ReplaceMCode(int color,CArray<class CString,class CString &>& aryMSB,CString strMSB,
		CArray<class CString,class CString &>& aryMCB,CString strMCB);
	//输入点返回是否选中图形，	鼠标点击点			返回的图形			图形上的点
	BOOL ClickCreateBase(Position posClick,long& pCreateBase,int& iPick);
	//返回图形类型1直线，2多段线，3矩形，4圆弧，5圆，6椭圆
	int RtnIsKindOf(CCreateBase* pCreateBase);
	//连接两个图形链表PGRAPHPOS
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
	//画测量尺寸
	void DrawMeasure(CDC* pDC);
	//初始化参数
	void InitDate();

	
	//当前文件的路径
	CString m_strPathFile,m_strNameFile;
	//打开文件时的类型 1为g，2为dxf
	int m_iStyleFile;
	//是否是第一次打开
	BOOL m_bFirstOpen;
};




/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAD_POWERLANDDOC_H__90215736_BEAB_4C24_9A56_C2906775573C__INCLUDED_)
