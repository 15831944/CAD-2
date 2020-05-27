// TransToGraphList.h: interface for the TransToGraphList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSTOGRAPHLIST_H__7D1F946D_6D6B_4B6C_BEA9_C657ACED3C7F__INCLUDED_)
#define AFX_TRANSTOGRAPHLIST_H__7D1F946D_6D6B_4B6C_BEA9_C657ACED3C7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TransToGraphList  //把DATASTRUCT结构转成CAD格式
{
public:
	TransToGraphList();
	virtual ~TransToGraphList();

	//磨玻璃输出的函数
	PGRAPHLIST OutGraphList(PUNDOLIST list);
	//磨边输出的函数
	void OutMillSide();


	//------------------------------------------
private:

//////////////////////////////////////////////////////////////////////////
	//清空PGRAPHLIST
	void ClearGraphList();
	//获取MCode的值
	int MCodeValue(CString str);
	//下刀抬刀的M代码
	CString CintToCBMstr(int RtnColor);
	CString CintToSBMstr(int RtnColor);	
	// 写文件尾
	BOOL	TransTail();	
	// 写图元信息
	BOOL	TransLine(CCreateLine*	pLine);
	BOOL	TransRectangle(CCreateRectangle*	pRectangle);
	BOOL	TransRound(CCreateRound*	pRound);
	BOOL	TransArc(CCreateRoundArc*	pArc);
	BOOL	TransPolyline(CCreatePolyline* Polyline);
	BOOL	TransEllipse(CCreateEllipse* Ellipse);
	
	void AddAction(); 
	void AddGraphList(); 
	void AddArcInfo();
	void AddCADData();
	void AddGraph();
	BOOL AddG01(DOUBLEPOINT ptEnd);			//添加G00
	BOOL AddG0X(CCreateRoundArc *pArc);		//添加G02/03
	BOOL AddG0X(CCreateRound *pRound, DOUBLEPOINT ptBegAndEnd);		//添加G02/03
	BOOL AddG00(DOUBLEPOINT ptEnd, CString l_strSBM_What);			//添加G00
	double PointToPointAngle(DOUBLEPOINT ptBeg, DOUBLEPOINT ptEnd);	//计算圆弧中起点/中点/终点角度
	double CalculateTraverseAngle(char chDirection, double dBeginAngle, double dEndAngle);	//计算扫过角度
	//打印函数
	void WriteGraphList(PGRAPHLIST pList);
	//把M代码集解析成字符串
	CString AnalysisAryM(CArray<class CString,class CString &>& aryM);
	
	PGRAPHLIST		m_pHeadList;
	PGRAPHLIST		m_pCurrList;
	PGRAPH			m_pCurrGraph;
	PCADDATA		m_pCurrData;
	DOUBLEPOINT		m_ptSet;
	
	CFile m_cGCode;
	double m_dCurX;	//当前点X
	double m_dCurY;	//当前点Y
	int m_iColorLast;//最后的颜色
	CString m_strCBM_What;	//最后线的颜色
	BOOL m_bFlagAddList;//是否向结构里加图形
/////////////////////////////////////////////////////////////////////////

	//转换数据
	void Transfer();
	//转换成铣刀后的图形
	void TransMillside();

	//加刀补类型
	PGRAPHLIST AddKnifeStyle(long lData);
	//加刀补类型
	PGRAPHLIST AddKnifeStyleForMillSide(long lData);
	

	//加刀补尺寸
	void AddKnifeLong(POSITION POS);
	//加刀补尺寸给铣边图形
	void AddKnifeLongForMillSide(POSITION POS);

	//把DATASTRUCT的结构转成CreateBase结构			    			    哪种磨法  
	BOOL GraphListTurnCreateStc(PGRAPHLIST pHeadList,POSITION POS,int StyleKnife);
	//GRAPHLIST转换Createbase			    			          
	void ListToCreateForMillSide(PGRAPHLIST pHeadList,POSITION POS);

	//返回粗，精，抛光的颜色
	COLORREF RtnColorOfKnife(int StyleKnife,int l_iMcode);//0为原图色，1为粗磨，2为精磨，3为抛光

	//向撤消链表里添加
	void AddUndoList(LONG CreateBase,POSITION pos);
	//删除图元函数，上面的点击“删除”按钮要实现的功能
	void DeleteOriginal(PUNDOLIST pUndoList);


	CObArray    m_CollectOfG00;	// 在两个G00之间存的图元
	//撤消类的对象
	CUndoRedo m_UndoRedo;
	//向撤消链表里添加的一个结点	
	PUNDOLIST m_pUndoList;
	//存图形的指针
	PGRAPHPOS m_pCurGraphPos,m_pGraphPos;

};

#endif // !defined(AFX_TRANSTOGRAPHLIST_H__7D1F946D_6D6B_4B6C_BEA9_C657ACED3C7F__INCLUDED_)
