// OutputGCode.h: interface for the COutputGCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTGCODE_H__12BA7076_6221_4B62_85C3_289D8B04C37D__INCLUDED_)
#define AFX_OUTPUTGCODE_H__12BA7076_6221_4B62_85C3_289D8B04C37D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COutputGCode  
{
public:
	COutputGCode();
	virtual ~COutputGCode();


public:
	// 输出g代码文件
	BOOL	OutputGCode(CString filename);

private:
	//下刀抬刀的M代码
	CString CintToCBMstr(int RtnColor);
	CString CintToSBMstr(int RtnColor);
	// 写文件头
	BOOL	WriteHeader();
	// 写文件尾
	BOOL	WriteTail();
	// 写实体信息，该函数根据类型调用写图元信息的函数
	BOOL	WriteContent();
	// 写图元信息
	BOOL	WriteLine(CCreateLine*	pLine);
	BOOL	WriteRectangle(CCreateRectangle*	pRectangle);
	BOOL	WriteRound(CCreateRound*	pRound);
	BOOL	WriteArc(CCreateRoundArc*	pArc);
	BOOL	WritePolyline(CCreatePolyline* Polyline);
	BOOL	WriteEllipse(CCreateEllipse* Ellipse);
	BOOL	WritePoint(CCreatePoint* Point);
	//把M代码集解析成字符串
	CString AnalysisAryM(CArray<class CString,class CString &>& aryM);
	//是否是第一个GOO
	BOOL m_bFirstG00;
	CFile m_cGCode;
	double m_dCurX;	//当前点X
	double m_dCurY;	//当前点Y
	int m_iColorLast;//最后的颜色
	CString m_strCBM_What;	//最后线的颜色
};

#endif // !defined(AFX_OUTPUTGCODE_H__12BA7076_6221_4B62_85C3_289D8B04C37D__INCLUDED_)
